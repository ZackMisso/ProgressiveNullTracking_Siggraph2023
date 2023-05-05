#pragma once

#include "trans_func.h"

namespace pbrt
{

class DavisTransmittance : public Transmittance_Func
{
    // why does this not have beta?
public:
    DavisTransmittance(Float alpha);

    virtual Spectrum surfaceSurface(Spectrum tau) const override final;
    virtual Spectrum surfaceMedium(Spectrum tau) const override final;
    virtual Spectrum mediumSurface(Spectrum tau) const override final;
    virtual Spectrum mediumMedium(Spectrum tau) const override final;

    virtual Float sigmaBar() const override final;

    virtual Float sampleSurface(Sampler &sampler) const override final;
    virtual Float sampleMedium(Sampler &sampler) const override final;

    Float alpha;
};

}
