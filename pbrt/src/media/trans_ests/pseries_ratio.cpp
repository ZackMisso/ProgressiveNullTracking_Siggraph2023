#include "pseries_ratio.h"
#include "spectrum.h"
#include "sampler.h"

namespace pbrt
{

// TODO: DO THIS PER CHANNEL
Spectrum Pseries_Ratio::Tr_ManualSig(const Ray& ray,
                                     Sampler& sampler,
                                     Float tMin,
                                     Float tMax,
                                     Float maxD,
                                     Float sig_t) const
{
    sampler.BeforeTransmittanceSample();

    Float tau = maxD * (tMax - tMin) * sig_t;
    Float sample = sampler.Get1D();
    Float val = exp(-tau);
    Float cdf = val;
    Float prob = val;
    Spectrum Tr = Spectrum(1.0);
    Float i = 1.0;

    while(cdf < sample)
    {
        Float x = sampler.Get1D() * (tMax - tMin) + tMin;
        Spectrum dense = density->D(ray(x));
        Tr[0] *= 1.0 - dense[0] / maxD;
        Tr[1] *= 1.0 - dense[1] / maxD;
        Tr[2] *= 1.0 - dense[2] / maxD;
        prob *= tau / i;

        cdf += prob;
        i += 1.0;
    }

    return Tr;
}

}
