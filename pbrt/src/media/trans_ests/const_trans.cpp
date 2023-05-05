#include "const_trans.h"
#include "spectrum.h"
#include "sampler.h"

namespace pbrt
{

Spectrum ConstTracking::Tr_ManualSig(const Ray& ray,
                                     Sampler& sampler,
                                     Float tMin,
                                     Float tMax,
                                     Float maxD,
                                     Float sig_t) const
{
    sampler.BeforeTransmittanceSample();

    Spectrum dens = density->D(ray((tMax + tMin) / 2.0));

    return Spectrum(exp(-dens[0] * (tMax - tMin) * sig_t));
}

}
