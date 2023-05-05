#pragma once

// this file contains the definitions of all the different
// transmittance functions

#include "../density/densityfunc.h"
#include "../majorant/majorant.h"
#include "../minorant.h"
#include "../trans_funcs/trans_func.h"
#include "../trans_samplers/trans_sampler.h"
#include "blanchet_spectrum.h"

namespace pbrt {

class T_Estimator {
  public:
    T_Estimator()
        : trans_func(nullptr),
          minorant(nullptr),
          majorant(nullptr),
          density(nullptr),
          sigma_t(1.0),
          sigma_t_spec(Spectrum(1.0)) {}

#if RENDER_MUTABLE
    virtual Spectrum Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax);
#else
    virtual Spectrum Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax) const;
#endif

    virtual Spectrum Tr_MutableSig(const Ray& ray, Sampler& sampler, Float tMin,
                                   Float tMax, Float sig_t);

    virtual Spectrum Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                  Float tMax, Float sig_t) const;

    virtual Spectrum Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax, bool startOnSurface, bool endOnSurface,
                        bool adjoint) const;

    virtual BlanchetCorrelatedResults Tr_Blanchet(
        const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
        bool startOnSurface, bool endOnSurface, bool adjoint,
        int blanchet_n) const;

    virtual BlanchetCorrelatedResults Tr_Blanchet(const Ray& ray,
                                                  Sampler& sampler, Float tMin,
                                                  Float tMax,
                                                  int blanchet_n) const;

    virtual Spectrum Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax, Float maxD) const {
        return Tr_ManualSig(ray, sampler, tMin, tMax, maxD, sigma_t);
    }

    virtual Spectrum Tr_MutableSig(const Ray& ray, Sampler& sampler, Float tMin,
                                   Float tMax, Float& maxD, Float sig_t) const {
        return Tr_ManualSig(ray, sampler, tMin, tMax, maxD, sig_t);
    }

    virtual Spectrum Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                  Float tMax, Float maxD,
                                  Float sig_t) const = 0;

    virtual Spectrum Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax, bool startOnSurface, bool endOnSurface,
                        bool adjoint, Float maxD) const {
        return Tr(ray, sampler, tMin, tMax, maxD);
    }

    virtual BlanchetCorrelatedResults Tr_Blanchet(
        const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
        bool startOnSurface, bool endOnSurface, bool adjoint, int blanchet_n,
        Float maxD) const {
        return Tr(ray, sampler, tMin, tMax, maxD);
    }

    virtual BlanchetCorrelatedResults Tr_Blanchet(const Ray& ray,
                                                  Sampler& sampler, Float tMin,
                                                  Float tMax, Float maxD,
                                                  int blanchet_n) const {
        return Tr(ray, sampler, tMin, tMax, maxD);
    }

    virtual bool supportsNonExponential() const { return false; }

    virtual bool is_null_track() const { return true; }

    virtual void preProcess();

    MinorantFunction* minorant;
    MajorantFunction* majorant;
    DensityFunction* density;
    Transmittance_Func* trans_func;
    Float sigma_t;
    Spectrum sigma_t_spec;
};

}  // namespace pbrt
