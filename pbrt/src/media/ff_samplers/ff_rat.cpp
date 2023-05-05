#include "ff_rat.h"

#include "interaction.h"
#include "phase.h"

namespace pbrt {

Spectrum FF_Rat_Sampler::Sample_ManualSigs(const Ray& ray, const Ray& rWorld,
                                           Sampler& sampler, MemoryArena& arena,
                                           MediumInteraction* mi, Float tMin,
                                           Float tMax, Float sig_t, Float sig_s,
                                           bool& success) const {
    sampler.BeforeFreeFlightSample();
    Float invMaxDensity = 0.33;

    // Run delta-tracking iterations to sample a medium interaction
    Float t = tMin;
    Float tr_samp = sampler.Get1D();
    Float tr = 1.0;
    while (true) {
        t -= std::log(1 - sampler.Get1D()) * invMaxDensity / sig_t;
        if (t >= tMax) break;

        Float dense = density->D(ray(t))[0];
        tr *= (1.0 - dense * invMaxDensity);

        if (tr_samp > tr) {
            // Populate _mi_ with medium interaction information and return
            PhaseFunction* phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(t), -rWorld.d, rWorld.time,
                                    mediumRef, phase, ray, t, tMin);
            success = true;
            return sig_s / sig_t;
        }
    }

    return Spectrum(1.f);
}

Spectrum FF_Rat_Sampler::Sample_MutableSigs(const Ray& ray, const Ray& rWorld,
                                            Sampler& sampler,
                                            MemoryArena& arena,
                                            MediumInteraction* mi, Float tMin,
                                            Float tMax, Float sig_t,
                                            Float sig_s, bool& success) {
    sampler.BeforeFreeFlightSample();

    std::vector<MacroGridQuery> intervals =
        majorant->MacroVoxelTraversal(ray(tMin), ray(tMax), tMin, tMax);

    Float tr_samp = sampler.Get1D();
    Float tr = 1.0;

    for (int i = 0; i < intervals.size(); ++i) {
        Float maxD = intervals[i].maj;
        Float invMaxDensity = 1.0 / maxD;

        std::cout << "maj: " << maxD << std::endl;

        // Run delta-tracking iterations to sample a medium interaction
        Float t = intervals[i].t_start;

        while (true) {
            t -= std::log(1 - sampler.Get1D()) * invMaxDensity / sig_t;
            if (t >= intervals[i].t_end) break;

            Float dense = density->D(ray(t))[0];

            if (dense > maxD) {
                maxD = dense;
                majorant->UpdateMajorant(ray(t), maxD);
                return Spectrum(0.0);
            }

            tr *= (1.0 - dense * invMaxDensity);

            if (tr_samp > tr) {
                // Populate _mi_ with medium interaction information and return
                PhaseFunction* phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
                *mi = MediumInteraction(rWorld(t), -rWorld.d, rWorld.time,
                                        mediumRef, phase, ray, t, tMin);
                success = true;
                return sig_s / sig_t;
            }
        }
    }

    return Spectrum(1.f);
}

Spectrum FF_Rat_Sampler::Sample_Cull(const Ray& ray, const Ray& rWorld,
                                     Sampler& sampler, MemoryArena& arena,
                                     MediumInteraction* mi, Float tMin,
                                     Float tMax, Float sig_t, Float sig_s,
                                     bool& success) const {
    sampler.BeforeFreeFlightSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    Float invMaxDensity = 1.0 / maj;

    std::cout << "maj: " << maj << std::endl;

    // Run delta-tracking iterations to sample a medium interaction
    Float t = path.tMin;
    while (true) {
        t -= std::log(1 - sampler.Get1D()) * invMaxDensity / sig_t;
        if (t >= path.tMax) break;

        Float dense = ThreeWiseMax(density->D(ray(path.unmap(t)))[0],
                                   density->D(ray(path.unmap(t)))[1],
                                   density->D(ray(path.unmap(t)))[2]) *
                      sig_t;

        // if (dense * invMaxDensity > 1.0) std::cout << "fuck: " << dense *
        // invMaxDensity << std::endl;

        if (dense * invMaxDensity > sampler.Get1D()) {
            // std::cout << "scat: " << std::endl;
            // Populate _mi_ with medium interaction information and return
            PhaseFunction* phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi =
                MediumInteraction(rWorld(path.unmap(t)), -rWorld.d, rWorld.time,
                                  mediumRef, phase, ray, path.unmap(t), tMin);
            success = true;
            // std::cout << sig_s / sig_t << std::endl;
            return sig_s / sig_t;
        }
    }

    return Spectrum(1.f);
}

}  // namespace pbrt
