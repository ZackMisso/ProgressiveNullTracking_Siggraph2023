#include "ff_raymarch.h"
#include "phase.h"
#include "interaction.h"

namespace pbrt
{

Spectrum FF_Raymarching::Sample_ManualSigs(const Ray& ray,
                                           const Ray& rWorld,
                                           Sampler& sampler,
                                           MemoryArena& arena,
                                           MediumInteraction* mi,
                                           Float tMin,
                                           Float tMax,
                                           Float sig_t,
                                           Float sig_s,
                                           bool& success) const
{
    sampler.BeforeFreeFlightSample();
    // TODO: replace this with the majorant
    Float invMaxDensity = 1.0;
    Float base_step = invMaxDensity * base_step_size / sig_t;
    Float t = tMin;
    // 1 - rand() for numberical stability
    Float od_sample = -std::log(1.0 - sampler.Get1D());

    Float od = 0.0;

    while (t < tMax)
    {
        Float step = std::min(base_step, tMax - t);
        Float loc = t + sampler.Get1D() * step;

        Float dense = ThreeWiseMax(density->D(ray(loc))[0],
                                   density->D(ray(loc))[1],
                                   density->D(ray(loc))[2]);

        Float past_od = od;
        od += dense * step;

        if (od > od_sample)
        {
            Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

            // Populate _mi_ with medium interaction information and return
            PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(actual_loc),
                                    -rWorld.d,
                                    rWorld.time,
                                    mediumRef,
                                    phase,
                                    ray,
                                    actual_loc,
                                    tMin);

            success = true;
            // std::cout << "post" << std::endl;
            return sig_s / sig_t;
        }

        t += base_step;
    }


    // std::cout << "post" << std::endl;

    return Spectrum(1.f);

    /*// Run delta-tracking iterations to sample a medium interaction
    Float t = tMin;
    while (true) {
        t -= std::log(1 - sampler.Get1D()) * invMaxDensity / sig_t;
        if (t >= tMax) break;

        Float dense = ThreeWiseMax(density->D(ray(t))[0],
                                   density->D(ray(t))[1],
                                   density->D(ray(t))[2]);

        if (dense * invMaxDensity > sampler.Get1D()) {
            // Populate _mi_ with medium interaction information and return
            PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(t),
                                    -rWorld.d,
                                    rWorld.time,
                                    mediumRef,
                                    phase,
                                    ray,
                                    t,
                                    tMin);
            return sig_s / sig_t;
        }
    }

    return Spectrum(1.f);*/
}

}
