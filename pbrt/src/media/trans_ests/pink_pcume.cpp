#include "pink_pcume.h"

#include <random>

#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

Spectrum Pink_Pseries_Cumulative::Tr(const Ray& ray, Sampler& sampler,
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

    maxD = 1.0;  // this assumes clamped media
    Spectrum scaled_maj = 1.0;

    Spectrum Tr = Spectrum(0.0);
    Spectrum W = Spectrum(1.0);
    Float i = 1.0;

    Float bMa = path.tMax - path.tMin;

    // sigma is being multiplied in here for performance improvements
    // this assumes clamped media
    Float pdf = sigma_t * bMa;

    Float inv_factor = 1.0 / (1.0 + pdf * davis_c * davis_c);
    Float prod_weight = 1.0;
    Float prod_i = 1.0;

    Float rr;
    do {
        rr = sampler.Get1D();
    } while (rr == 0.0);

    // handling of the type of bounce
    if (startOnSurface && !endOnSurface)
    {
        prod_i = 2.0;
        // prod_weight = (1.0 + (prod_i - 1.0) * davis_c * davis_c) * inv_factor;
        // W *= prod_weight;
    }
    if (!startOnSurface && endOnSurface)
    {
        prod_i = 2.0;
        // prod_weight = (1.0 + (prod_i - 1.0) * davis_c * davis_c) * inv_factor;
        // W *= prod_weight;
    }
    if (!startOnSurface && !endOnSurface)
    {
        prod_i = 3.0;
        // prod_weight = (1.0 + (prod_i - 1.0) * davis_c * davis_c) * inv_factor;
        // W *= prod_weight;
    }

    Spectrum trans = std::pow(1.0 + pdf * davis_c * davis_c, -1.0 / (davis_c * davis_c) - (prod_i-1.0));

    while (true) {
        Tr += W;

        Float tmpT = sampler.Get1D() * bMa + path.tMin;
        Spectrum dense = density->D(ray(path.unmap(tmpT)));
        prod_weight = (1.0 + (prod_i - 1.0) * davis_c * davis_c) * inv_factor;

        Spectrum wi = (1.0 / i) * (scaled_maj - dense) * pdf * prod_weight;
        W *= wi;

        Float accept = (W).MaxAbsComponentValue();

        if (accept >= 1.0) {
            i += 1.0;
            prod_i += 1.0;
        } else {
            if (accept <= rr) break;

            rr /= accept;

            i += 1.0;
            prod_i += 1.0;
            W /= accept;
        }
    }

    return (Tr * trans);
}

Spectrum Pink_Pseries_Cumulative::Tr_ManualSig(const Ray& ray, Sampler& sampler,
                                               Float tMin, Float tMax,
                                               Float maxD, Float sig_t) const {
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
