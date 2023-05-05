#include "davis.h"
#include "sampler.h"

namespace pbrt {

DavisTransmittance::DavisTransmittance(Float alpha) : alpha(alpha) { }

Spectrum DavisTransmittance::surfaceSurface(Spectrum tau) const
{
    return Pow(tau / alpha + 1.0f, -alpha);
}

Spectrum DavisTransmittance::surfaceMedium(Spectrum tau) const
{
    return Pow(tau / alpha + 1.0f, -(alpha + 1.0f));
}

Spectrum DavisTransmittance::mediumSurface(Spectrum tau) const
{
    return surfaceMedium(tau);
}

Spectrum DavisTransmittance::mediumMedium(Spectrum tau) const
{
    return (1.0f + 1.0f / alpha) * Pow(tau / alpha + 1.0f, -( alpha + 2.0f));
}

// what is sigma_bar again?
Float DavisTransmittance::sigmaBar() const
{
    return 1.0f;
}

// this samples a transmittance value, but what if we want to sample an optical depth?
Float DavisTransmittance::sampleSurface(Sampler &sampler) const
{
    return alpha * (std::pow(1.0f - sampler.Get1D(), -1.0f / alpha) - 1.0f);
}

// this samples a transmittance value, but what if we want to sample an optical depth?
Float DavisTransmittance::sampleMedium(Sampler &sampler) const
{
    return alpha * (std::pow(1.0f - sampler.Get1D(), -1.0f / (1.0f + alpha)) - 1.0f);
}

}
