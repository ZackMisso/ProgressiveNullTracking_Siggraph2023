#pragma once

#include "t_est.h"

namespace pbrt {

class Pseries_CDF : public T_Estimator {
  public:
    virtual Spectrum Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                  Float tMax, Float maxD, Float sig_t) const;

    virtual Spectrum Tr_MutableSig(const Ray& ray, Sampler& sampler, Float tMin,
                                   Float tMax, Float& maxD, Float sig_t) const;
};

}  // namespace pbrt
