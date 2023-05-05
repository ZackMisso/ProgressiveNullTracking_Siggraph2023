#pragma once

#include "trans_func.h"

namespace pbrt {

class ExponentialTransmittance : public Transmittance_Func
{
public:
    ExponentialTransmittance() = default;

    virtual Spectrum surfaceSurface(Spectrum tau) const override final;
    virtual Spectrum surfaceMedium(Spectrum tau) const override final;
    virtual Spectrum mediumSurface(Spectrum tau) const override final;
    virtual Spectrum mediumMedium(Spectrum tau) const override final;

    virtual Float sigmaBar() const override final;

    virtual Float sampleSurface(Sampler &sampler) const override final;
    virtual Float sampleMedium(Sampler &sampler) const override final;
};

}
