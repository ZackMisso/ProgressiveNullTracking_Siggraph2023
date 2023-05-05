#include "davis_weinstein.h"

#include "sampler.h"

namespace pbrt {

DavisWeinsteinTransmittance::DavisWeinsteinTransmittance(Float h, Float c)
    : c(c)
{
    beta = 2.0f * Clamp(h, 0.0f, 1.0f) - 1.0f;
    std::cout << "BETA: " << beta << std::endl;

    beta = h;
}

Float DavisWeinsteinTransmittance::computeAlpha(Float tau) const
{
    return std::pow(tau, 1.f - beta) / (std::pow(c, 1.f + beta));
}

Spectrum DavisWeinsteinTransmittance::surfaceSurface(Spectrum tau) const
{
    Float t = tau[0];
    if (t < 1e-4f) return Spectrum(1.0f);

    Float alpha = computeAlpha(t);
    Float Tr = std::pow(1.0f + t / alpha, -alpha);

    return Spectrum(std::isnan(Tr) ? 0 : Tr);
}

Spectrum DavisWeinsteinTransmittance::surfaceMedium(Spectrum tau) const
{
    Float t = tau[0];
    if (t < 1e-4f) return Spectrum(1.0f);

    Float alpha = computeAlpha(t);
    Float base = 1.0f + t / alpha;

    Float trSurface = std::pow(base, -alpha);

    Float Tr = trSurface*(beta / base - (beta - 1.0f) * alpha / t * std::log(base));

    return Spectrum(std::isnan(Tr) ? 0 : Tr);
}

Spectrum DavisWeinsteinTransmittance::mediumSurface(Spectrum tau) const
{
    return surfaceMedium(tau);
}

Spectrum DavisWeinsteinTransmittance::mediumMedium(Spectrum tau) const
{
    Float t = tau[0];
    if (t < 1e-4f) return Spectrum(1.0f);

    Float alpha = computeAlpha(t);
    Float base = 1.0f + t / alpha;
    Float logBase = std::log(base);
    Float trSurface = std::pow(base, -alpha);

    Float term1 = beta * (-1.0f + beta * (1.0f + t) + (-1.0f + 2.0f * beta) * t / alpha) / (t * base * base);
    Float term2 = ((-1.0f + beta) * beta * alpha / (t * t) * (2.0f * t + base) * logBase) / base;
    Float term3 = (beta - 1.0f)*alpha/t*logBase;

    Float Tr = trSurface * (term1 - term2 + term3 * term3);

    return Spectrum(std::isnan(Tr) ? 0 : Tr);
}

Float DavisWeinsteinTransmittance::computeAlpha(Float tau, Float bbeta, Float cc) const
{
    return std::pow(tau, 1.f - bbeta) / (std::pow(cc, 1.f + bbeta));
}

Spectrum DavisWeinsteinTransmittance::surfaceSurface(Spectrum tau, Float bbeta, Float cc) const
{
    Float t = tau[0];
    if (t < 1e-4f) return Spectrum(1.0f);

    Float alpha = computeAlpha(t, bbeta, cc);
    Float Tr = std::pow(1.0f + t / alpha, -alpha);

    return Spectrum(std::isnan(Tr) ? 0 : Tr);
}

Spectrum DavisWeinsteinTransmittance::surfaceMedium(Spectrum tau, Float bbeta, Float cc) const
{
    Float t = tau[0];
    if (t < 1e-4f) return Spectrum(1.0f);

    Float alpha = computeAlpha(t, bbeta, cc);
    Float base = 1.0f + t / alpha;

    Float trSurface = std::pow(base, -alpha);

    Float Tr = trSurface*(bbeta / base - (bbeta - 1.0f) * alpha / t * std::log(base));

    return Spectrum(std::isnan(Tr) ? 0 : Tr);
}

Spectrum DavisWeinsteinTransmittance::mediumSurface(Spectrum tau, Float bbeta, Float cc) const
{
    return surfaceMedium(tau, bbeta, cc);
}

Spectrum DavisWeinsteinTransmittance::mediumMedium(Spectrum tau, Float bbeta, Float cc) const
{
    Float t = tau[0];
    if (t < 1e-4f) return Spectrum(1.0f);

    Float alpha = computeAlpha(t, bbeta, cc);
    Float base = 1.0f + t / alpha;
    Float logBase = std::log(base);
    Float trSurface = std::pow(base, -alpha);

    Float term1 = bbeta * (-1.0f + bbeta * (1.0f + t) + (-1.0f + 2.0f * bbeta) * t / alpha) / (t * base * base);
    Float term2 = ((-1.0f + bbeta) * bbeta * alpha / (t * t) * (2.0f * t + base) * logBase) / base;
    Float term3 = (bbeta - 1.0f)*alpha/t*logBase;

    Float Tr = trSurface * (term1 - term2 + term3 * term3);

    return Spectrum(std::isnan(Tr) ? 0 : Tr);
}

Float DavisWeinsteinTransmittance::sigmaBar() const
{
    return 1.0f;
}

Float DavisWeinsteinTransmittance::sampleSurface(Sampler &sampler) const
{
    Float xi = sampler.Get1D();
    auto cdf = [this](Float tau) { return 1.0f - surfaceSurface(tau)[0]; };
    Float step = 1e6;
    Float result = step*2;

    while (step > 1e-6)
    {
        if (cdf(result) > xi) result -= step;
        else result += step;
        step /= 2;
    }

    return result;
}

Float DavisWeinsteinTransmittance::sampleMedium(Sampler &sampler) const
{
    Float xi = sampler.Get1D();
    auto cdf = [this](Float tau) { return 1.0f - mediumSurface(tau)[0]; };
    Float step = 1e6;
    Float result = step * 2;

    while (step > 1e-6)
    {
        if (cdf(result) > xi) result -= step;
        else result += step;
        step /= 2;
    }

    return result;
}

Float DavisWeinsteinTransmittance::sampleSurface(Sampler &sampler, Float bbeta, Float cc) const
{
    Float xi = sampler.Get1D();
    auto cdf = [this](Float tau, Float bbeta, Float cc) { return 1.0f - surfaceSurface(tau, bbeta, cc)[0]; };
    Float step = 1e6;
    Float result = step*2;

    while (step > 1e-6)
    {
        if (cdf(result, bbeta, cc) > xi) result -= step;
        else result += step;
        step /= 2;
        if (result < 0.0) result = 0.0;
    }

    // if (result < 0.0) std::cout << "WHAT THE FUCK" << std::endl;
    // std::cout << "result: " << result << std::endl;

    return result;
}

Float DavisWeinsteinTransmittance::sampleMedium(Sampler &sampler, Float bbeta, Float cc) const
{
    Float xi = sampler.Get1D();
    auto cdf = [this](Float tau, Float bbeta, Float cc) { return 1.0 - mediumSurface(tau, bbeta, cc)[0]; };
    Float step = 1e6;
    Float result = step * 2;

    while (step > 1e-6)
    {
        if (cdf(result, bbeta, cc) > xi) result -= step;
        else result += step;
        step /= 2;
        // if (result < 0.0) result = 0.0;
    }

    if (result < 0.0) std::cout << "WHAT THE FUCK" << std::endl;

    return result;
}

}
