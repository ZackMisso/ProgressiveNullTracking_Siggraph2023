#pragma once

#include "t_est.h"

namespace pbrt {

class Pink_Pseries_Cumulative_Exp : public T_Estimator {
  public:
    virtual Spectrum Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                  Float tMax, Float maxD, Float sig_t) const;

    virtual Spectrum Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax, bool startOnSurface, bool endOnSurface,
                        bool adjoint, Float maxD) const;

    virtual bool supportsNonExponential() const { return true; }

    Float maj;
};

}  // namespace pbrt
