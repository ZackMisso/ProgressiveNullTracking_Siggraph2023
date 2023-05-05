#include "track_length.h"
#include "spectrum.h"
#include "sampler.h"

namespace pbrt
{

// TODO: incorporate nTrSteps
Spectrum TrackLength::Tr_ManualSig(const Ray& ray,
                                   Sampler& sampler,
                                   Float tMin,
                                   Float tMax,
                                   Float maxD,
                                   Float sig_t) const
{
    sampler.BeforeTransmittanceSample();

    Float t = tMin;

    maxD = 5.0;
    Float invMaxD = 1.0 / maxD;

    Spectrum ret = Spectrum(1.0);

    while (true)
    {
        t -= std::log(1 - sampler.Get1D()) * invMaxD / sig_t;
        if (t >= tMax) break;

        Spectrum dense = density->D(ray(t));

        Float rrOne = sampler.Get1D();

        // if (rrOne <= dense[0] * invMaxD) ret[0] = 0.0;
        // if (rrOne <= dense[1] * invMaxD) ret[1] = 0.0;
        // if (rrOne <= dense[2] * invMaxD) ret[2] = 0.0;

        if (rrOne <= dense[0] * invMaxD)
        {
            // I am going to assume no spectral density for now
            return Spectrum(0.0);
        }
    }

    return ret;
}

}
