#include "davis_pink_tay.h"
#include "spectrum.h"
#include "sampler.h"

namespace pbrt
{

Float Davis_Pink_Taylor::compute_alpha(Float tau, Float c) const
{
    return 1.0 / (c * c);
}

Float Davis_Pink_Taylor::bk_expected_order(Float c, Float p_z) const
{
    Float K = floor(c);
    Float t = 1.0;
    Float sum = 1.0;

    for (int k = 1; k <= K; ++k)
    {
        t = t * c / Float(k);
        sum += t;
    }

    Float E_n = K + (exp(c) - sum) / t;

    return (1.0 - p_z) * E_n;
}

int Davis_Pink_Taylor::ncmf(Float maj) const
{
    return ceil(pow((0.015+maj)*(0.65+maj)*(60.3+maj), 1.0 / 3.0));
}

// this determintes the tuple size to make this estimator have the same cost as
// the pseries-cmf estimator
int Davis_Pink_Taylor::determine_tuple_size(Float maj, Float p_z, Float c) const
{
    Float cmf_n = ncmf(maj);
    Float bk_n = bk_expected_order(c, p_z);

    return fmax(1, floor(cmf_n / (bk_n + 1.0) + 0.5));
}

void Davis_Pink_Taylor::aggressive_russian_roulette
(
    Sampler& sampler,
    Float p_z,
    std::vector<Float>& weights,
    Float davis_c,
    int& k
) const
{
    Float w0 = 1.0;
    // initial probability for continuing
    Float P = 1.0 - p_z;
    Float u = sampler.Get1D();

    weights.push_back(w0);

    if (P <= u)
    {
        k = 0;
        return;
    }

    k = 2;
    int K = 2;
    Float c = 2;

    for (int i = 0; i < K; ++i)
    {
        weights.push_back(1.0 / P);
    }

    Float i = K + 1.0;
    Float fact = 1.0;
    while (true)
    {
        Float c_i = fmin((1.0 + davis_c * davis_c * (i-1.0)) / i, 1.0);
        P *= c_i;

        if (P <= u)
        {
            k = int(i - 1.0);
            return;
        }
        i += 1.0;

        weights.push_back(1.0 / P);
    }
}

void Davis_Pink_Taylor::pivot_array
(
    const std::vector<Float>& samples,
    std::vector<Float>& pivoted,
    int j
) const
{
    pivoted.clear();

    Float x_j = samples[j];

    for (int i = 0; i < samples.size(); ++i)
    {
        if (i != j)
        {
            pivoted.push_back(x_j - samples[i]);
        }
    }
}

void Davis_Pink_Taylor::elementry_means
(
    const std::vector<Float>& samples,
    std::vector<Float>& means
) const
{
    means.push_back(1.0);
    for (int i = 0; i < samples.size(); ++i) means.push_back(0.0);

    for (int i = 0; i < samples.size(); ++i)
    {
        for (int k = i; k > 0; k--)
        {
            means[k] = means[k] + Float(k) / Float(samples.size()) *
                       (means[k-1] * samples[i] - means[k]);
        }
    }
}

Spectrum Davis_Pink_Taylor::Tr_ManualSig
(
    const Ray& ray,
    Sampler& sampler,
    Float tMin,
    Float tMax,
    Float maxD,
    Float sig_t
) const
{
    std::cout << "This is not supported - pink taylor" << std::endl;
    return Spectrum(1.f);
}

Spectrum Davis_Pink_Taylor::Tr
(
    const Ray& ray,
    Sampler& sampler,
    Float tMin,
    Float tMax,
    bool startOnSurface,
    bool endOnSurface,
    bool adjoint,
    Float maxD
) const
{
    sampler.BeforeTransmittanceSample();

    if (!trans_func->isDavis())
    {
        std::cout << "pink noise davis only supports davis" << std::endl;
    }

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMax == path.tMin) // there is no density so no reason to evaluate this
    {
        return Spectrum(1.f);
    }

    int deriv;
    if (startOnSurface && endOnSurface) deriv = 0;
    else if (!startOnSurface && !endOnSurface) deriv = 2;
    else deriv = 1;

    Float p_z = 0.9;
    Float c = 2.0;
    Float l = path.tMax - path.tMin;

    Float davis_c = trans_func->davisC();

    int m = determine_tuple_size(heckinfeckin * sigma_t * std::max(l, 1.0), p_z, c);

    int k = 0; // this is N
    std::vector<Float> rr_weights = std::vector<Float>();

    aggressive_russian_roulette(sampler, p_z, rr_weights, davis_c, k);

    std::vector<Float> X_i = std::vector<Float>();

    for (int i = 0; i <= k; ++i)
    {
        Float u_i = sampler.Get1D();
        Float x_i = 0.0;

        for (int j = 0; j < m; ++j)
        {
            Float loc = path.tMin + l / Float(m) * (u_i + j);
            x_i += density->D(ray(path.unmap(loc)))[0] * sigma_t;
        }

        X_i.push_back(x_i * l / Float(m));
    }

    Spectrum T = Spectrum(0.0);

    for (int i = 0; i <= k; ++i)
    {
        std::vector<Float> means = std::vector<Float>();
        std::vector<Float> pivot = std::vector<Float>();

        pivot_array(X_i, pivot, i);
        elementry_means(pivot, means);

        Float X_j = X_i[i];
        Float sum = 0.0;
        Float k_fact = 1.0;
        Float c_fact = 1.0;

        for (int j = 0; j <= k; ++j)
        {
            if (j != 0)
            {
                k_fact /= Float(j);
            }
            if (j + deriv != 0)
            {
                c_fact *= (1.0 + Float(j - 1 + deriv) * davis_c * davis_c);
            }

            sum += means[j] * k_fact * rr_weights[j] * c_fact *
                   pow(1.0 + X_j * davis_c * davis_c, -1.0 / (davis_c * davis_c) - Float(j+deriv));
        }

        T += Spectrum(1.0 / Float(k + 1) * sum);
    }

    return T;
}

}
