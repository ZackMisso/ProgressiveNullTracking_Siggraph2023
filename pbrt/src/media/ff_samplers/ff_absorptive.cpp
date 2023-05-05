#include "ff_absorptive.h"

#include "interaction.h"
#include "phase.h"

namespace pbrt {

Spectrum FF_Absorptive_Sampler::Sample_ManualSigs(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
    MediumInteraction* mi, Float tMin, Float tMax, Float sig_t, Float sig_s,
    bool& success) const {
    sampler.BeforeFreeFlightSample();
    success = false;
    return Spectrum(1.f);
}

Spectrum FF_Absorptive_Sampler::Sample(const Ray& ray, const Ray& rWorld,
                                       Sampler& sampler, MemoryArena& arena,
                                       MediumInteraction* mi, Float tMin,
                                       Float tMax, bool& success,
                                       uint32_t flags,
                                       TransportMode mode) const {
    sampler.BeforeFreeFlightSample();
    success = false;
    return Spectrum(1.f);
}

BlanchetCorrelatedResults FF_Absorptive_Sampler::Sample_Blanchet(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
    MediumInteraction* mi, Float tMin, Float tMax, bool& success,
    int blanchet_n, uint32_t flags, TransportMode mode) const {
    sampler.BeforeFreeFlightSample();
    success = false;
    return Spectrum(1.f);
}

BlanchetCorrelatedResults FF_Absorptive_Sampler::Sample_Blanchet_Beta(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, Float beta, Float c,
    MemoryArena& arena, MediumInteraction* mi, Float tMin, Float tMax,
    bool& success, int blanchet_n, uint32_t flags, TransportMode mode) const {
    sampler.BeforeFreeFlightSample();
    success = false;
    return Spectrum(1.f);
}

}  // namespace pbrt
