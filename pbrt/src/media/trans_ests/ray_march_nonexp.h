#pragma once

#include "t_est.h"

namespace pbrt
{

class RayMarchNonExpTrans : public T_Estimator
{
public:
    RayMarchNonExpTrans(Float step = 0.002) : T_Estimator(), base_step_size(step) { }

    virtual Spectrum Tr_ManualSig(const Ray& ray,
                                  Sampler& sampler,
                                  Float tMin,
                                  Float tMax,
                                  Float maxD,
                                  Float sig_t) const;

    virtual Spectrum Tr(const Ray& ray,
                        Sampler& sampler,
                        Float tMin,
                        Float tMax,
                        bool startOnSurface,
                        bool endOnSurface,
                        bool adjoint,
                        Float maxD) const;

    virtual bool supportsNonExponential() const { return true; }

    Float base_step_size;
};

}
