#include "pink_taylor_ray.h"

#include "sampler.h"
#include "spectrum.h"
#include <random>

namespace pbrt {

int PinkTaylorRay::determine_tuple_size(Float maj_thik, Float p_z, Float c) const {
    Float cmf_n = ncmf(maj_thik);
    Float bk_n = expected_order(c, p_z);

    return fmax(1, floor(cmf_n / (bk_n + 1.0) + 0.5));
}

int PinkTaylorRay::ncmf(Float maj_thik) const {
    return ceil(pow((0.015 + maj_thik) * (0.65 + maj_thik) * (60.3 + maj_thik),
                    1.0 / 3.0));
}

void PinkTaylorRay::aggressive_russian_roulette(Sampler &sampler, Float p_z,
                                            std::vector<Float> &weights,
                                            int &k) const {
    Float w0 = 1.0;
    // initial probability for continuing
    Float P = 1.0 - p_z;
    Float u = 1.0 - sampler.Get1D();

    weights.push_back(w0);

    if (P <= u) {
        k = 0;
        return;
    }

    k = 2;
    int K = 2;
    Float c = 2;

    for (int i = 0; i < K; ++i) {
        weights.push_back(1.0 / P);
    }

    Float i = K + 1.0;
    while (true) {
        Float c_i = fmin(c / i, 1.0);
        P *= c_i;

        if (P <= u) {
            k = int(i - 1.0);
            return;
        }
        i += 1.0;

        weights.push_back(1.0 / P);
    }
}

void PinkTaylorRay::aggressive_russian_roulette_pink(Sampler &sampler, Float p_z, Float prod_i,
                                            std::vector<Float> &weights,
                                            int &k) const {
    Float w0 = 1.0;
    // initial probability for continuing
    Float P = 1.0 - p_z;
    Float u = 1.0 - sampler.Get1D();
    Float C = trans_func->davisC();

    weights.push_back(w0);

    if (P <= u) {
        k = 0;
        return;
    }

    k = 2;
    int K = 2;// + int(deriv);
    Float c = 1;

    for (int i = 0; i < K; ++i) {
        weights.push_back(1.0 / P);
    }

    Float i = K + 1.0;
    while (true) {
        Float c_i = fmin(c * (1.0 + C * C * ((i) - 1.0)) / (i), 1.0);
        P *= c_i;

        if (P <= u) {
            k = int(i - 1.0);
            return;
        }
        i += 1.0;

        weights.push_back(1.0 / P);
    }
}

void PinkTaylorRay::pivot_array(const std::vector<Spectrum> &samples,
                            std::vector<Spectrum> &pivoted, Float maj,
                            int j) const {
    pivoted.clear();

    Spectrum x_j = samples[j];

    for (int i = 0; i < samples.size(); ++i) {
        if (i != j) {
            pivoted.push_back(x_j - samples[i]);
        }
    }
}

void PinkTaylorRay::elementry_means(const std::vector<Spectrum> &samples,
                                std::vector<Spectrum> &means) const {
    means.push_back(1.0);
    for (int i = 0; i < samples.size(); ++i) means.push_back(0.0);

    for (int i = 0; i < samples.size(); ++i) {
        for (int k = i; k > 0; k--) {
            means[k] = means[k] + Float(k) / Float(samples.size()) *
                                      (means[k - 1] * samples[i] - means[k]);
        }
    }
}

Float PinkTaylorRay::expected_order(Float c, Float p_z) const {
    Float K = floor(c);
    Float t = 1.0;
    Float sum = 1.0;

    for (int k = 1; k <= K; ++k) {
        t = t * c / Float(k);
        sum += t;
    }

    Float E_n = K + (exp(c) - sum) / t;

    return (1.0 - p_z) * E_n;
}

Spectrum PinkTaylorRay::Tr_ManualSig(const Ray &ray, Sampler &sampler, Float tMin,
                                 Float tMax, Float maxD, Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (Spectrum(1.0));
    }

    if (maj > 0.0) maxD = maj;

    Float invMaxD = 1.0 / maxD;

    Float p_z = 0.9;
    Float c = 2.0;
    Float l = path.tMax - path.tMin;

    int m = determine_tuple_size(maj * l, p_z, c);

    int k = 0;  // this is N
    std::vector<Float> rr_weights = std::vector<Float>();

    aggressive_russian_roulette(sampler, p_z, rr_weights, k);

    std::vector<Spectrum> X_i = std::vector<Spectrum>();

    for (int i = 0; i <= k; ++i) {
        Float u_i = sampler.Get1D();

        Spectrum x_i = 0.0;

        for (int j = 0; j < m; ++j) {
            Float loc = l / Float(m) * (u_i + j);
            x_i += density->D(ray(path.unmap(loc))) * sig_t;
        }

        X_i.push_back(-x_i * l / Float(m));
    }

    Spectrum trans = 0.0;

    for (int i = 0; i <= k; ++i) {
        std::vector<Spectrum> means = std::vector<Spectrum>();
        std::vector<Spectrum> pivot = std::vector<Spectrum>();

        pivot_array(X_i, pivot, maj, i);
        elementry_means(pivot, means);

        Spectrum X_j = X_i[i];
        Spectrum sum = 0.0;
        Float k_fact = 1.0;

        for (int j = 0; j <= k; ++j) {
            if (j != 0) k_fact /= Float(j);
            sum += means[j] * k_fact * rr_weights[j];
        }

        trans += 1.0 / Float(k + 1) * Exp(X_j) * sum;
    }

    return trans;
}

Spectrum PinkTaylorRay::pink(Spectrum tau, Float prod_i, Float C) const
{
    Spectrum tmp = tau;
    tmp[0] = std::pow(1.0 + tau[0] * C * C, -1.0 / (C * C) - prod_i);
    tmp[1] = std::pow(1.0 + tau[1] * C * C, -1.0 / (C * C) - prod_i);
    tmp[2] = std::pow(1.0 + tau[2] * C * C, -1.0 / (C * C) - prod_i);
    return tmp;
}

Spectrum PinkTaylorRay::Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax, bool startOnSurface, bool endOnSurface,
                        bool adjoint, Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (Spectrum(1.0));
    }

    Float C = trans_func->davisC();
    // std::default_random_engine generator;
    // generator.seed(
    //     uint32_t(sampler.Get1D() * 0xFFFFFFFFFFFF));  // todo make nicer
    // std::gamma_distribution<Float> distribution(1.0 / (davis_c * davis_c),
    //                                             davis_c * davis_c);

    // Float mult = distribution(generator);

    if (maj > 0.0) maxD = maj;

    // TODO: compute prod_i
    Float prod_i = 0.0;
    int deriv = 0;

    // handling of the type of bounce
    if (startOnSurface && !endOnSurface)
    {
        prod_i = 1.0;
        deriv = 1;
        // prod_weight = (1.0 + (prod_i - 1.0) * davis_c * davis_c) * inv_factor;
        // W *= prod_weight;
    }
    if (!startOnSurface && endOnSurface)
    {
        prod_i = 1.0;
        deriv = 1;
        // prod_weight = (1.0 + (prod_i - 1.0) * davis_c * davis_c) * inv_factor;
        // W *= prod_weight;
    }
    if (!startOnSurface && !endOnSurface)
    {
        prod_i = 2.0;
        deriv = 2;
        // prod_weight = (1.0 + (prod_i - 1.0) * davis_c * davis_c) * inv_factor;
        // W *= prod_weight;
    }
    // prod_i = 0.0;
    // deriv = 0;

    Float p_z = 0.9;
    Float c = 2.0;
    Float l = path.tMax - path.tMin;

    int m = 4 * std::max(l, 1.0);

    int k = 0;  // this is N
    std::vector<Float> rr_weights = std::vector<Float>();

    aggressive_russian_roulette_pink(sampler, p_z, prod_i, rr_weights, k);

    std::vector<Spectrum> X_i = std::vector<Spectrum>();

    for (int i = 0; i <= k; ++i) {
        Float u_i = sampler.Get1D();

        Spectrum x_i = 0.0;

        for (int j = 0; j < m; ++j) {
            Float loc = l / Float(m) * (u_i + j);
            x_i += density->D(ray(path.unmap(loc))) * sigma_t;
        }

        X_i.push_back(x_i * l / Float(m));
    }

    Spectrum trans = 0.0;
    // std::cout << "FUCK" << std::endl;

    for (int i = 0; i <= k; ++i) {
        std::vector<Spectrum> means = std::vector<Spectrum>();
        std::vector<Spectrum> pivot = std::vector<Spectrum>();

        pivot_array(X_i, pivot, maj, i);
        elementry_means(pivot, means);

        Spectrum X_j = X_i[i];
        Spectrum sum = 0.0;
        Float k_fact = 1.0;
        Spectrum c_fact = 1.0;

        for (int j = 0; j <= k; ++j) {
            if (j != 0) k_fact /= Float(j);
            if ((j+deriv) != 0)
            {
                c_fact *= (1.0 + (Float(j) + prod_i - 1.0) * C * C);
                // c_fact = c_fact / (X_j * C * C + 1.0);
            }
            sum += means[j] * k_fact * c_fact * rr_weights[j] * pink(X_j, prod_i + Float(j), C);
        }

        trans += 1.0 / Float(k + 1) * sum;
    }

    return trans;
}

}  // namespace pbrt
