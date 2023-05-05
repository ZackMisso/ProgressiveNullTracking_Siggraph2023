#include "track_length_adap.h"
#include "spectrum.h"
#include "sampler.h"

namespace pbrt
{

// TODO: incorporate nTrSteps
Spectrum TrackLengthAdaptive::Tr_ManualSig(const Ray& ray,
                                   Sampler& sampler,
                                   Float tMin,
                                   Float tMax,
                                   Float maxD,
                                   Float sig_t) const
{
    sampler.BeforeTransmittanceSample();

    // maxD = 5.0;
    Float base_maj = maxD;
    Float maj_to_use = base_maj;
    Float invMaj = 1.0 / maj_to_use;

    Spectrum ret = Spectrum(1.0);
    // Float weight = 1.0;
    Float t = tMin;
    Float last_t = t;

    while (true)
    {
        last_t = t;
        
        t -= std::log(1 - sampler.Get1D()) * invMaj / sig_t;
        if (t >= tMax) break;

        Spectrum dense = density->D(ray(t));
        Float rrOne = sampler.Get1D();

        Float chk = (base_maj - dense[0]) * invMaj;
        chk *= exp((maj_to_use - base_maj) * (t - last_t));
        Float accept = std::min(chk, 1.0);

        if (rrOne > accept)
        {
            ret = Spectrum(0.0);
            return ret;
        }

        ret *= chk / accept;

        maj_to_use = std::max(base_maj - dense[0], base_maj * 0.1);
        invMaj = 1.0 / maj_to_use;
    }

    return ret * exp((maj_to_use - base_maj) * (tMax - last_t));
}

}
