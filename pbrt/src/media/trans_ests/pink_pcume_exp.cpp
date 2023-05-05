#include "pink_pcume_exp.h"

#include <random>

#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

Spectrum Pink_Pseries_Cumulative_Exp::Tr(const Ray& ray, Sampler& sampler,
                                         Float tMin, Float tMax,
                                         bool startOnSurface, bool endOnSurface,
                                         bool adjoint, Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a,
    b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (Spectrum(1.0));
    }

    Float davis_c = trans_func->davisC();
    std::default_random_engine generator;
    generator.seed(
        uint32_t(sampler.Get1D() * 0xFFFFFFFFFFFF));  // todo make nicer
    std::gamma_distribution<Float> distribution(1.0 / (davis_c * davis_c),
                                                davis_c * davis_c);

    Float mult = distribution(generator);

    maxD = 1.0;  // this assumes clamped media
    Spectrum scaled_maj = 1.0 * mult;

    Spectrum Tr = Spectrum(0.0);
    Spectrum W = Spectrum(1.0);
    Float i = 1.0;

    Float bMa = path.tMax - path.tMin;

    // sigma is being multiplied in here for performance improvements
    Float pdf = sigma_t * bMa;

    Float exponent = exp(-mult * pdf);

    Float rr;
    do {
        rr = sampler.Get1D();
    } while (rr == 0.0);

    while (true) {
        Float tmpT = sampler.Get1D() * bMa + path.tMin;
        Spectrum dense = density->D(ray(path.unmap(tmpT))) * mult;

        Spectrum wi = (1.0 / i) * (scaled_maj - dense) * pdf;
        Tr += W;

        W *= wi;

        Float accept = (W).MaxAbsComponentValue();

        if (accept >= 1.0) {
            i += 1.0;
        } else {
            if (accept <= rr) break;

            rr /= accept;

            i += 1.0;
            W /= accept;
        }
    }

    // handling the derivatives
    if (startOnSurface && !endOnSurface) return Tr * mult * exponent;
    if (!startOnSurface && endOnSurface) return Tr * mult * exponent;
    if (!startOnSurface && !endOnSurface) return Tr * mult * mult * exponent;

    return (Tr * exponent);
}

Spectrum Pink_Pseries_Cumulative_Exp::Tr_ManualSig(const Ray& ray,
                                                   Sampler& sampler, Float tMin,
                                                   Float tMax, Float maxD,
                                                   Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    Spectrum Tr = Spectrum(0.0);
    Spectrum W = Spectrum(1.0);
    Float i = 1.0;

    Float bMa = tMax - tMin;

    // sigma is being multiplied in here for performance improvements
    Float pdf = sig_t * bMa;

    Float exponent = exp(-maxD * pdf);

    Float rr;
    do {
        rr = sampler.Get1D();
    } while (rr == 0.0);

    while (true) {
        Float tmpT = sampler.Get1D() * bMa + tMin;
        Spectrum dense = density->D(ray(tmpT));

        Spectrum wi = (1.0 / i) * (Spectrum(maxD) - dense) * pdf;
        Tr += W;

        W *= wi;

        Float accept = (W).MaxAbsComponentValue();

        if (accept >= 1.0) {
            i += 1.0;
        } else {
            if (accept <= rr) break;

            rr /= accept;

            i += 1.0;
            W /= accept;
        }
    }

    return (Tr * exponent);
}

}  // namespace pbrt
