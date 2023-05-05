#include "exp_cub.h"
#include "sampler.h"

namespace pbrt
{

Spectrum ExponentialCubedTransmittance::surfaceSurface(Spectrum tau) const
{
    return Exp(-tau * tau * tau);
}

Spectrum ExponentialCubedTransmittance::surfaceMedium(Spectrum tau) const
{
    return 3.0 * tau * tau * Exp(-tau * tau * tau);
}

Spectrum ExponentialCubedTransmittance::mediumSurface(Spectrum tau) const
{
    return surfaceMedium(tau);
}

Spectrum ExponentialCubedTransmittance::mediumMedium(Spectrum tau) const
{
    return (9.0 * tau * tau * tau * tau - 6.0 * tau) * Exp(-tau * tau * tau);
}

Float ExponentialCubedTransmittance::sigmaBar() const
{
    return 1.0f;
}

Float ExponentialCubedTransmittance::sampleSurface(Sampler &sampler) const
{
    return std::pow(-std::log(1.0f - sampler.Get1D()), 1.0 / 3.0);
}

Float ExponentialCubedTransmittance::sampleMedium(Sampler &sampler) const
{
    // this can not be evaluated analytically without evaluating the product
    // logorithm for (-tau^2), so we resort to numerical estimation.

    Float xi = sampler.Get1D();
    auto cdf = [this](Float tau) { return 1.0 - mediumSurface(tau)[0]; };
    Float step = 1e6;
    Float result = step * 2.0;

    while (step > 1e-6)
    {
        if (cdf(result) > xi) result -= step;
        else result += step;
        step /= 2.0;
    }

    return result;
}

}
