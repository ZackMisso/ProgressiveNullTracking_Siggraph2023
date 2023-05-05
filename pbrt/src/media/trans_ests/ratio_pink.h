#pragma once

#include "t_est.h"
#include <random>

namespace pbrt
{

class RatioPinkTracking : public T_Estimator
{
public:
    RatioPinkTracking() : maj(-1.0) { }
    RatioPinkTracking(Float maj) : maj(maj) { } // TODO: make not hardcoded later

    virtual Spectrum Tr_ManualSig(const Ray& ray,
                                  Sampler& sampler,
                                  Float tMin,
                                  Float tMax,
                                  Float maxD,
                                  Float sig_t) const;

    virtual Spectrum Tr
    (
        const Ray& ray,
        Sampler& sampler,
        Float tMin,
        Float tMax,
        bool startOnSurface,
        bool endOnSurface,
        bool adjoint,
        Float maxD
    ) const;

    virtual bool supportsNonExponential() const { return true; }

    Float maj;
    // std::default_random_engine generator;
};

}
