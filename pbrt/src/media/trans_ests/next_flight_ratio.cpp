#include "next_flight_ratio.h"
#include "spectrum.h"
#include "sampler.h"

namespace pbrt
{

Spectrum NextFlightRatioTracking::Tr_ManualSig(const Ray& ray,
                                               Sampler& sampler,
                                               Float tMin,
                                               Float tMax,
                                               Float maxD,
                                               Float sig_t) const
{
    sampler.BeforeTransmittanceSample();

    Float t = tMin;

    Float bMa = tMax - tMin;

    Float invMaxD = 1.0 / maxD;

    Spectrum Tr = Spectrum(exp(-maxD * bMa * sig_t));

    Spectrum runningProd = Spectrum(1.0);

    while (true) {
        t -= std::log(1 - sampler.Get1D()) * invMaxD / sig_t;

        if (t >= tMax) return Tr;

        Spectrum dense = density->D(ray(t));

        runningProd *= (Spectrum(maxD) - dense) * invMaxD;

        Tr += runningProd * exp(-maxD * sig_t * (tMax - t));
    }

    return Spectrum(0.0);
}

}
