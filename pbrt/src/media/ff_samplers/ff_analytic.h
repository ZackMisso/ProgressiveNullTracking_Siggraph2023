#pragma once

#include "ff_sampler.h"

namespace pbrt
{

class FF_Analytic : public FF_Sampler
{
public:
    FF_Analytic(Float step = 0.001) : FF_Sampler(), base_step_size(step) { }

    virtual Spectrum Sample_ManualSigs (const Ray& ray,
                                        const Ray& rWorld,
                                        Sampler& sampler,
                                        MemoryArena& arena,
                                        MediumInteraction* mi,
                                        Float tMin,
                                        Float tMax,
                                        Float sig_t,
                                        Float sig_s,
                                        bool& success) const;

    virtual BlanchetCorrelatedResults Sample_Blanchet(const Ray& ray,
                                                      const Ray& rWorld,
                                                      Sampler& sampler,
                                                      MemoryArena& arena,
                                                      MediumInteraction* mi,
                                                      Float tMin,
                                                      Float tMax,
                                                      bool& success,
                                                      int blanchet_n) const;

    virtual Spectrum Sample_Cull_Function (const Ray& ray,
                                           const Ray& rWorld,
                                           Sampler& sampler,
                                           MemoryArena& arena,
                                           MediumInteraction* mi,
                                           Float tMin,
                                           Float tMax,
                                           bool& success,
                                           Float scale = 1.0) const;

    virtual Spectrum Sample (const Ray& ray,
                             const Ray& rWorld,
                             Sampler& sampler,
                             MemoryArena& arena,
                             MediumInteraction* mi,
                             Float tMin,
                             Float tMax,
                             bool& success,
                             uint32_t flags,
                             TransportMode mode) const;

    virtual Spectrum Sample_Beta (const Ray& ray,
                          const Ray& rWorld,
                          Sampler& sampler,
                          MemoryArena& arena,
                          MediumInteraction* mi,
                          Float tMin,
                          Float tMax,
                          Float beta, Float c,
                          bool& success,
                          uint32_t flags,
                          TransportMode mode) const;

     virtual BlanchetCorrelatedResults Sample_Blanchet
     (
         const Ray& ray,
         const Ray& rWorld,
         Sampler& sampler,
         MemoryArena& arena,
         MediumInteraction* mi,
         Float tMin,
         Float tMax,
         bool& success,
         int blanchet_n,
         uint32_t flags,
         TransportMode mode
     ) const;

     virtual BlanchetCorrelatedResults Sample_Blanchet_Beta
     (
         const Ray& ray,
         const Ray& rWorld,
         Sampler& sampler,
         Float beta,
         Float c,
         MemoryArena& arena,
         MediumInteraction* mi,
         Float tMin,
         Float tMax,
         bool& success,
         int blanchet_n,
         uint32_t flags,
         TransportMode mode
     ) const;

     virtual BlanchetCorrelatedFdDebiasing Sample_Blanchet_FdBeta
     (
         const Ray& ray,
         const Ray& rWorld,
         Sampler& sampler,
         MemoryArena& arena,
         MediumInteraction* mi,
         Float tMin,
         Float tMax,
         bool& success,
         int blanchet_n,
         int davis_n,
         Float base_h,
         Float davis_beta,
         Float davis_c,
         uint32_t flags,
         TransportMode mode
     ) const;

    virtual bool RequiresTr() const { return false; }
    virtual bool RequiresTrOnScatterOnly() const { return FULL_TRANS_COMPARE; }
    virtual bool RequiresLightFirst() const { return false; }

    virtual bool supportsNonExponential() const { return true; }

    Float base_step_size;
};

}
