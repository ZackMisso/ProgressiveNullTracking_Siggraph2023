#include "pseries_cdf.h"

#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

// Spectrum Pseries_CDF::Tr_ManualSig(const Ray& ray, Sampler& sampler, Float
// tMin,
//                                    Float tMax, Float maxD, Float sig_t) const
//                                    {
//     sampler.BeforeTransmittanceSample();

//     Float runningCDF = 0.0;
//     Float bMa = tMax - tMin;
//     Float i = 1.0;
//     Float pdf = sig_t * bMa;
//     Spectrum W = (1.0);
//     Float tau = pdf * maxD;
//     Spectrum Tr = Spectrum(0.0);

//     Float exponent = exp(-tau);
//     double goal = 0.99;
//     // double goal = 0.75;
//     Float lastPDF = exponent;

//     Float rr;
//     do {
//         rr = sampler.Get1D();
//     } while (rr == 0.0);

//     while (runningCDF < goal) {
//         Float tmpT = sampler.Get1D() * bMa + tMin;
//         runningCDF += lastPDF;
//         Float invI = 1.0 / i;
//         Spectrum dense = density->D(ray(tmpT));
//         Spectrum wi = invI * pdf * (Spectrum(maxD) - dense);
//         lastPDF *= tau * invI;
//         Tr += W;
//         W *= wi;
//         i += 1.0;
//     }

//     while (true) {
//         double accept = tau / i;

//         Tr += W;

//         if (accept <= rr) break;

//         rr /= accept;

//         runningCDF += lastPDF;  // why is this here

//         Float tmpT = tMin + sampler.Get1D() * bMa;
//         Float invI = 1.0 / i;
//         Spectrum dense = density->D(ray(tmpT));
//         Spectrum wi = invI * (Spectrum(maxD) - dense) * pdf;

//         lastPDF *= tau * invI;  // why is this here
//         W *= (wi / accept);
//         i += 1.0;
//     }

//     return (Tr * exponent);
// }

Spectrum Pseries_CDF::Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                   Float tMax, Float maxD, Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (Spectrum(1.0));
    }

    // if (maj > 0.0) maxD = maj;

    Float runningCDF = 0.0;
    Float bMa = path.tMax - path.tMin;
    Float i = 1.0;
    Float pdf = sig_t * bMa;
    Spectrum W = (1.0);
    Float tau = pdf * maxD;
    Spectrum Tr = Spectrum(0.0);

    Float exponent = exp(-tau);
    double goal = 0.99;
    // double goal = 0.75;
    Float lastPDF = exponent;

    Float rr;
    do {
        rr = sampler.Get1D();
    } while (rr == 0.0);

    while (runningCDF < goal) {
        Float tmpT = sampler.Get1D() * bMa + path.tMin;
        runningCDF += lastPDF;
        Float invI = 1.0 / i;
        Spectrum dense = density->D(ray(path.unmap(tmpT)));
        Spectrum wi = invI * pdf * (Spectrum(maxD) - dense);
        lastPDF *= tau * invI;
        Tr += W;
        W *= wi;
        i += 1.0;
    }

    while (true) {
        double accept = tau / i;

        Tr += W;

        if (accept <= rr) break;

        rr /= accept;

        runningCDF += lastPDF;  // why is this here

        Float tmpT = path.tMin + sampler.Get1D() * bMa;
        Float invI = 1.0 / i;
        Spectrum dense = density->D(ray(path.unmap(tmpT)));
        Spectrum wi = invI * (Spectrum(maxD) - dense) * pdf;

        lastPDF *= tau * invI;  // why is this here
        W *= (wi / accept);
        i += 1.0;
    }

    return (Tr * exponent);
}

Spectrum Pseries_CDF::Tr_MutableSig(const Ray& ray, Sampler& sampler,
                                    Float tMin, Float tMax, Float& maxD,
                                    Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    // Point3f a = ray(tMin);
    // Point3f b = ray(tMax);

    if (maxD <= 0.0001) {
        return (Spectrum(1.0));
    }

    // if (maj > 0.0) maxD = maj;

    Float runningCDF = 0.0;
    Float bMa = tMax - tMin;
    Float i = 1.0;
    Float pdf = sig_t * bMa;
    Spectrum W = (1.0);
    Float tau = pdf * maxD;
    Spectrum Tr = Spectrum(0.0);

    Float exponent = exp(-tau);
    double goal = 0.99;
    // double goal = 0.75;
    Float lastPDF = exponent;

    Float rr;
    do {
        rr = sampler.Get1D();
    } while (rr == 0.0);

    while (runningCDF < goal) {
        Float tmpT = sampler.Get1D() * bMa + tMin;
        runningCDF += lastPDF;
        Float invI = 1.0 / i;
        Spectrum dense = density->D(ray(tmpT));
        Spectrum wi = invI * pdf * (Spectrum(maxD) - dense);
        lastPDF *= tau * invI;
        Tr += W;
        W *= wi;
        i += 1.0;
    }

    while (true) {
        double accept = tau / i;

        Tr += W;

        if (accept <= rr) break;

        rr /= accept;

        runningCDF += lastPDF;  // why is this here

        Float tmpT = tMin + sampler.Get1D() * bMa;
        Float invI = 1.0 / i;
        Spectrum dense = density->D(ray(tmpT));
        Spectrum wi = invI * (Spectrum(maxD) - dense) * pdf;

        lastPDF *= tau * invI;  // why is this here
        W *= (wi / accept);
        i += 1.0;
    }

    return (Tr * exponent);
}

}  // namespace pbrt
