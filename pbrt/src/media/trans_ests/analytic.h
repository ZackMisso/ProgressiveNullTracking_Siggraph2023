#pragma once

#include "t_est.h"

namespace pbrt {

class AnalyticTrans : public T_Estimator {
  public:
    AnalyticTrans(Float step = 0.02) : T_Estimator(), base_step_size(step) {}

    virtual Spectrum Tr_Beta(const Ray& ray, Sampler& sampler, Float tMin,
                             Float tMax, Float beta, Float c,
                             bool startOnSurface, bool endOnSurface,
                             bool adjoint, Float maxD) const;

    virtual Spectrum Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                  Float tMax, Float maxD, Float sig_t) const;

    virtual Spectrum Tr_MutableSig(const Ray& ray, Sampler& sampler, Float tMin,
                                   Float tMax, Float& maxD, Float sig_t) const;

    virtual Spectrum Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax, bool startOnSurface, bool endOnSurface,
                        bool adjoint, Float maxD) const;

    virtual BlanchetCorrelatedResults Tr_Blanchet(
        const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
        bool startOnSurface, bool endOnSurface, bool adjoint, int blanchet_n,
        Float maxD) const;

    virtual BlanchetCorrelatedResults Tr_Blanchet_Beta(
        const Ray& ray, Sampler& sampler, Float tMin, Float tMax, Float beta,
        Float c, bool startOnSurface, bool endOnSurface, bool adjoint,
        int blanchet_n, Float maxD) const;

    virtual BlanchetCorrelatedFdDebiasing Tr_Blanchet_FdBeta(
        const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
        bool startOnSurface, bool endOnSurface, bool adjoint, int blanchet_n,
        int davis_n, Float base_h, Float davis_beta, Float davis_c,
        Float maxD) const;

    virtual BlanchetCorrelatedResults Tr_Blanchet(const Ray& ray,
                                                  Sampler& sampler, Float tMin,
                                                  Float tMax, Float maxD,
                                                  int blanchet_n) const;

    BlanchetCorrelatedResults Tr_Blanchet_Macro(const Ray& ray,
                                                Sampler& sampler, Float tMin,
                                                Float tMax, Float maxD,
                                                int blanchet_n) const;

    virtual bool supportsNonExponential() const { return true; }

    Float base_step_size;
};

}  // namespace pbrt
