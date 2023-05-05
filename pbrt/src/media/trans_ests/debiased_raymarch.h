#pragma once

#include "t_est.h"

namespace pbrt {

// this implementation is a single instance implementation and not
// an entire rendering algorithm.
class DebiasedRaymarch : public T_Estimator {
  public:
    DebiasedRaymarch() : maj(-1.0) {}
    DebiasedRaymarch(Float maj) : maj(maj) {}

    virtual Spectrum Tr_MutableSig(const Ray& ray, Sampler& sampler, Float tMin,
                                   Float tMax, Float& maxD, Float sig_t) const;

    virtual Spectrum Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                  Float tMax, Float maxD, Float sig_t) const;

    // this is now deprecated
    virtual bool is_null_track() const { return false; }

    Float maj;
};

}  // namespace pbrt
