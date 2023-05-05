#include "exp_sqr.h"
#include "sampler.h"

namespace pbrt
{

Spectrum ExponentialSquaredTransmittance::surfaceSurface(Spectrum tau) const
{
    return Exp(-tau * tau);
}

Spectrum ExponentialSquaredTransmittance::surfaceMedium(Spectrum tau) const
{
    return 2.0 * tau * Exp(-tau * tau);
}

Spectrum ExponentialSquaredTransmittance::mediumSurface(Spectrum tau) const
{
    return surfaceMedium(tau);
}

Spectrum ExponentialSquaredTransmittance::mediumMedium(Spectrum tau) const
{
    return (4.0 * tau * tau - 2.0) * Exp(-tau * tau);
}

Float ExponentialSquaredTransmittance::sigmaBar() const
{
    return 1.0f;
}

Float ExponentialSquaredTransmittance::sampleSurface(Sampler &sampler) const
{
    return std::sqrt(-std::log(1.0f - sampler.Get1D()));
}

Float ExponentialSquaredTransmittance::sampleMedium(Sampler &sampler) const
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
