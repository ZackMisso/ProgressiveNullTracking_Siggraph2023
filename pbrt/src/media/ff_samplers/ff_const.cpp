#include "ff_const.h"
#include "phase.h"
#include "interaction.h"

namespace pbrt
{

// this method assumes that the density is constant everywhere
Spectrum FF_ConstTracking::Sample_ManualSigs
(
    const Ray& ray,
    const Ray& rWorld,
    Sampler& sampler,
    MemoryArena& arena,
    MediumInteraction* mi,
    Float tMin,
    Float tMax,
    Float sig_t,
    Float sig_s,
    bool& success
) const
{
    sampler.BeforeFreeFlightSample();

    Float t = tMin;

    Spectrum dens = density->D(ray((tMax+tMin) / 2.0));

    Float dense = ThreeWiseMax(dens[0], dens[1], dens[2]) * sig_t;

    t -= std::log(1.0 - sampler.Get1D()) / dense;
    // std::cout << "boy" << std::endl;

    if (t < tMax)
    {
        PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
        *mi = MediumInteraction(rWorld(t),
                                -rWorld.d,
                                rWorld.time,
                                mediumRef,
                                phase,
                                ray,
                                t,
                                tMin);
        success = true;
        return sig_s / sig_t;
    }

    return Spectrum(1.f);
}

}
