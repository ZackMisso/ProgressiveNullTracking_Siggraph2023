#include "lin.h"
#include "sampler.h"

namespace pbrt
{

LinearTransmittance::LinearTransmittance(Float maxT_t) : maxT(1.0)
{
    // maxT = 3.0;
}

Spectrum LinearTransmittance::surfaceSurface(Spectrum tau) const
{
    return Spectrum(1.0f) - Min(tau / maxT, 1.0f);
}

Spectrum LinearTransmittance::surfaceMedium(Spectrum tau) const
{
    return Step(tau, maxT) * (1.0f / maxT);
}

Spectrum LinearTransmittance::mediumSurface(Spectrum tau) const
{
    return Step(tau, maxT);
}

Spectrum LinearTransmittance::mediumMedium(Spectrum tau) const
{
    return Boxcar(tau, maxT - 1e-3f, maxT + 1e-3f);
}

Float LinearTransmittance::sigmaBar() const
{
    return 1.0f / maxT;
}

Float LinearTransmittance::sampleSurface(Sampler &sampler) const
{
    return maxT * (1.0f - 1e-5f) * sampler.Get1D();
}

Float LinearTransmittance::sampleMedium(Sampler &/*sampler*/) const
{
    return maxT;
}

}
