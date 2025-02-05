#pragma once

#include "ff_sampler.h"

namespace pbrt
{

class FF_Raymarching : public FF_Sampler
{
public:
    FF_Raymarching(Float step = 0.02) : FF_Sampler(), base_step_size(step) { }

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

    virtual bool RequiresTr() const { return false; }
    virtual bool RequiresTrOnScatterOnly() const { return false; }
    virtual bool RequiresLightFirst() const { return false; }

    Float base_step_size;
};

}
