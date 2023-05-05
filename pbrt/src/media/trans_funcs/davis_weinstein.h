#pragma once

#include "trans_func.h"

namespace pbrt {

class DavisWeinsteinTransmittance : public Transmittance_Func
{
public:
    DavisWeinsteinTransmittance(Float h, Float c);

    virtual Spectrum surfaceSurface(Spectrum tau) const override final;
    virtual Spectrum surfaceMedium(Spectrum tau) const override final;
    virtual Spectrum mediumSurface(Spectrum tau) const override final;
    virtual Spectrum mediumMedium(Spectrum tau) const override final;

    virtual Spectrum surfaceSurface(Spectrum tau, Float bbeta, Float cc) const override final;
    virtual Spectrum surfaceMedium(Spectrum tau, Float bbeta, Float cc) const override final;
    virtual Spectrum mediumSurface(Spectrum tau, Float bbeta, Float cc) const override final;
    virtual Spectrum mediumMedium(Spectrum tau, Float bbeta, Float cc) const override final;

    virtual Float sigmaBar() const override final;

    virtual Float sampleSurface(Sampler &sampler) const override final;
    virtual Float sampleMedium(Sampler &sampler) const override final;

    virtual Float sampleSurface(Sampler &sampler, Float bbeta, Float cc) const override final;
    virtual Float sampleMedium(Sampler &sampler, Float bbeta, Float cc) const override final;

    Float computeAlpha(Float tau) const;
    Float computeAlpha(Float tau, Float bbeta, Float cc) const;

    virtual bool isDavis() const override { return true; }
    virtual Float davisC() const override { return c; }
    virtual Float davisBeta() const override { return beta; }

    Float beta;
    Float c;
};

}
