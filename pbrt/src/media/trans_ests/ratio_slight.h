#pragma once

#include "t_est.h"

namespace pbrt {

class RatioTracking_Slight : public T_Estimator {
  public:
    RatioTracking_Slight() : maj(-1.0) {}
    RatioTracking_Slight(Float maj) : maj(maj) {}

    virtual Spectrum Tr_MutableSig(const Ray& ray, Sampler& sampler, Float tMin,
                                   Float tMax, Float& maxD, Float sig_t) const;

    virtual Spectrum Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                  Float tMax, Float maxD, Float sig_t) const;

    virtual BlanchetCorrelatedResults Tr_Blanchet(const Ray& ray,
                                                  Sampler& sampler, Float tMin,
                                                  Float tMax, Float maxD,
                                                  int blanchet_n) const;

    Spectrum Tr_Cull(const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
                     Float maxD, Float sig_t) const;

    Float maj;
};

}  // namespace pbrt
