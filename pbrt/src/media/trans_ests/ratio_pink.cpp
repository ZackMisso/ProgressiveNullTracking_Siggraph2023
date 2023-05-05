// std::cout << "evaluating beni for c: " << c << std::endl;
// std::default_random_engine generator;
// generator.seed(0x31241);
// std::gamma_distribution<double> distribution(1.0/(c*c), (c*c));
//
// double actual = pow(1.0 + c*c*ext*b, -1.0 / (c*c));
//
// // actual = exp(-b*ext);
//
// double beni_trans = 0.0;
// for (int i = 0; i < num; ++i)
// {
//     double mult = distribution(generator);
//     // mult=1.0;
//     double trans = run_ratio_lin(rng, mult * maj, mult * ext, a, b);
//     double trans_new = beni_trans + 1.0 / double(i+1) * (trans - beni_trans);
//     beni_trans = trans_new;
// }

#include "ratio_pink.h"
#include "spectrum.h"
#include "sampler.h"

namespace pbrt
{

Spectrum RatioPinkTracking::Tr_ManualSig
(
    const Ray& ray,
    Sampler& sampler,
    Float tMin,
    Float tMax,
    Float maxD,
    Float sig_t
) const
{
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax)
    {
        return (Spectrum(1.0));
    }

    Float davis_c = trans_func->davisC();
    // std::cout << "davis_c: " << davis_c << std::endl;
    std::default_random_engine generator;
    // std::cout << uint32_t(sampler.Get1D() * 0xFFFFFFFFFFFF) << std::endl;
    generator.seed(uint32_t(sampler.Get1D() * 0xFFFFFFFFFFFF)); // todo make nicer
    std::gamma_distribution<Float> distribution(1.0 / (davis_c*davis_c), davis_c*davis_c);

    // std::cout << "maxD: " << maxD << std::endl;

    Float mult = distribution(generator);

    // std::cout << "mult: " << mult << std::endl;

    Float scaled_maj = maj * mult * sig_t;

    if (scaled_maj > 0.0) maxD = scaled_maj;

    Float invMaxD = 1.0 / (maxD);

    // Perform ratio tracking to estimate the transmittance value
    Spectrum Tr = Spectrum(1.0);
    Float t = path.tMin;

    while (true)
    {
        t -= std::log(1 - sampler.Get1D()) * invMaxD;
        if (t >= path.tMax) break;

        Spectrum dens = density->D(ray(path.unmap(t))) * mult * sig_t;

        // std::cout << "dense: " << dens << std::endl;

        Tr *= (Spectrum(scaled_maj) - dens) * invMaxD;
    }

    // std::cout << "TR: " << Tr << std::endl;

    // if (!startOnSurface || !endOnSurface) std::cout << "DFADA" << std::endl;

    // if (startOnSurface && !endOnSurface) return Tr * mult;
    // if (!startOnSurface && endOnSurface) return Tr * mult;
    // if (!startOnSurface && !endOnSurface) return Tr * mult * mult;

    return Tr;
}

Spectrum RatioPinkTracking::Tr
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

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax)
    {
        return (Spectrum(1.0));
    }

    Float davis_c = trans_func->davisC();

    std::default_random_engine generator;

    generator.seed(uint32_t(sampler.Get1D() * 0xFFFFFFFFFFFF)); // todo make nicer
    std::gamma_distribution<Float> distribution(1.0 / (davis_c*davis_c), davis_c*davis_c);

    Float mult = distribution(generator);

    Float scaled_maj = maj * mult * sigma_t;

    if (scaled_maj > 0.0) maxD = scaled_maj;

    Float invMaxD = 1.0 / (maxD);
    // int nums = 0;

    // Perform ratio tracking to estimate the transmittance value
    Spectrum Tr = Spectrum(1.0);
    Float t = path.tMin;

    while (true)
    {
        t -= std::log(1 - sampler.Get1D()) * invMaxD;
        if (t >= path.tMax) break;
        // nums++;

        Spectrum dens = density->D(ray(path.unmap(t))) * mult * sigma_t;

        Tr *= (Spectrum(scaled_maj) - dens) * invMaxD;
    }

    // return Spectrum(nums);

    // handling the derivatives
    if (startOnSurface && !endOnSurface) return Tr * mult;
    if (!startOnSurface && endOnSurface) return Tr * mult;
    if (!startOnSurface && !endOnSurface) return Tr * mult * mult;

    return Tr;

    // sampler.BeforeTransmittanceSample();

    // Point3f a = ray(tMin);
    // Point3f b = ray(tMax);

    // std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    // MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    // if (path.tMin >= path.tMax)
    // {
    //     return (Spectrum(1.0));
    // }

    // Float davis_c = trans_func->davisC();

    // std::default_random_engine generator;

    // generator.seed(uint32_t(sampler.Get1D() * 0xFFFFFFFFFFFF)); // todo make nicer
    // std::gamma_distribution<Float> distribution(1.0 / (davis_c*davis_c), davis_c*davis_c);

    // Float mult = distribution(generator);

    // Float scaled_maj = maj * mult * sigma_t;

    // if (scaled_maj > 0.0) maxD = scaled_maj;

    // Float invMaxD = 1.0 / (maxD);

    // // Perform ratio tracking to estimate the transmittance value
    // Spectrum Tr = Spectrum(1.0);
    // Float t = tMin;

    // while (true)
    // {
    //     t -= std::log(1 - sampler.Get1D()) * invMaxD;
    //     if (t >= tMax) break;

    //     Spectrum dens = density->D(ray(t)) * mult * sigma_t;

    //     Tr *= (Spectrum(scaled_maj) - dens) * invMaxD;
    // }

    // // handling the derivatives
    // if (startOnSurface && !endOnSurface) return Tr * mult;
    // if (!startOnSurface && endOnSurface) return Tr * mult;
    // if (!startOnSurface && !endOnSurface) return Tr * mult * mult;

    // return Tr;
}

}
