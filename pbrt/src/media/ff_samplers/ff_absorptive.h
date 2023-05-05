#pragma once

#include "ff_sampler.h"

namespace pbrt {

class FF_Absorptive_Sampler : public FF_Sampler {
  public:
    virtual Spectrum Sample_ManualSigs(const Ray& ray, const Ray& rWorld,
                                       Sampler& sampler, MemoryArena& arena,
                                       MediumInteraction* mi, Float tMin,
                                       Float tMax, Float sig_t, Float sig_s,
                                       bool& success) const;

    virtual Spectrum Sample(const Ray& ray, const Ray& rWorld, Sampler& sampler,
                            MemoryArena& arena, MediumInteraction* mi,
                            Float tMin, Float tMax, bool& success,
                            uint32_t flags, TransportMode mode) const;

    virtual BlanchetCorrelatedResults Sample_Blanchet(
        const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
        MediumInteraction* mi, Float tMin, Float tMax, bool& success,
        int blanchet_n, uint32_t flags, TransportMode mode) const;

    virtual BlanchetCorrelatedResults Sample_Blanchet_Beta(
        const Ray& ray, const Ray& rWorld, Sampler& sampler, Float beta,
        Float c, MemoryArena& arena, MediumInteraction* mi, Float tMin,
        Float tMax, bool& success, int blanchet_n, uint32_t flags,
        TransportMode mode) const;

    virtual bool RequiresTr() const { return true; }
    virtual bool RequiresLightFirst() const { return false; }
    virtual bool RequiresTrOnScatterOnly() const { return false; }

    virtual bool supportsNonExponential() const { return true; }
};

}  // namespace pbrt
