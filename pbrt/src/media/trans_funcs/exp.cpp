#include "exp.h"

#include "sampler.h"

namespace pbrt
{

Spectrum ExponentialTransmittance::surfaceSurface(Spectrum tau) const
{
    return Exp(-tau);
}

Spectrum ExponentialTransmittance::surfaceMedium(Spectrum tau) const
{
    return Exp(-tau);
}

Spectrum ExponentialTransmittance::mediumSurface(Spectrum tau) const
{
    return Exp(-tau);
}

Spectrum ExponentialTransmittance::mediumMedium(Spectrum tau) const
{
    return Exp(-tau);
}

Float ExponentialTransmittance::sigmaBar() const
{
    return 1.0f;
}

Float ExponentialTransmittance::sampleSurface(Sampler &sampler) const
{
    return -std::log(1.0f - sampler.Get1D());
}

Float ExponentialTransmittance::sampleMedium(Sampler &sampler) const
{
    return -std::log(1.0f - sampler.Get1D());
}

}
