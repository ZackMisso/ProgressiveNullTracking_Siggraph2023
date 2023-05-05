#pragma once

#include "ff_sampler.h"

namespace pbrt {

class FF_Trans_Weighted_Sampler : public FF_Sampler {
  public:
    FF_Trans_Weighted_Sampler() : maj(1.f) {}
    FF_Trans_Weighted_Sampler(Float maj) : maj(maj) {}

    virtual Spectrum Sample_ManualSigs(const Ray& ray, const Ray& rWorld,
                                       Sampler& sampler, MemoryArena& arena,
                                       MediumInteraction* mi, Float tMin,
                                       Float tMax, Float sig_t, Float sig_s,
                                       bool& success) const;

    virtual Spectrum Sample_MutableSigs(const Ray& ray, const Ray& rWorld,
                                        Sampler& sampler, MemoryArena& arena,
                                        MediumInteraction* mi, Float tMin,
                                        Float tMax, Float sig_t, Float sig_s,
                                        bool& success);

    Spectrum Sample_Cull(const Ray& ray, const Ray& rWorld, Sampler& sampler,
                         MemoryArena& arena, MediumInteraction* mi, Float tMin,
                         Float tMax, Float sig_t, Float sig_s,
                         bool& success) const;

    virtual bool RequiresTr() const { return false; }
    virtual bool RequiresLightFirst() const { return false; }
    virtual bool RequiresTrOnScatterOnly() const { return false; }

    Float maj;
};

}  // namespace pbrt
