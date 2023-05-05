#include "ff_delta_weight_signif.h"

#include "interaction.h"
#include "phase.h"

/// This class is a small experiment using a different form of clamping.
/// Yes, this is expected to explode in variance.

namespace pbrt {

Spectrum FF_Trans_Weighted_Signif_Sampler::Sample_ManualSigs(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
    MediumInteraction* mi, Float tMin, Float tMax, Float sig_t, Float sig_s,
    bool& success) const {
    sampler.BeforeFreeFlightSample();
    Float invMaxDensity = 0.33;

    // TODO: this method not currently in use

    // Run delta-tracking iterations to sample a medium interaction
    Float t = tMin;
    while (true) {
        t -= std::log(1 - sampler.Get1D()) * invMaxDensity / sig_t;
        if (t >= tMax) break;
        Float dense = density->D(ray(t))[0];

        if (dense * invMaxDensity > sampler.Get1D()) {
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

Spectrum FF_Trans_Weighted_Signif_Sampler::Sample_MutableSigs(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
    MediumInteraction* mi, Float tMin, Float tMax, Float sig_t, Float sig_s,
    bool& success) {
    sampler.BeforeFreeFlightSample();

    std::vector<MacroGridQuery> intervals =
        majorant->MacroVoxelTraversal(ray(tMin), ray(tMax), tMin, tMax);

    Float weight = 1.0;

    for (int i = 0; i < intervals.size(); ++i) {
        Float maxD = intervals[i].maj;

        if (maxD < 1e-4) continue;  // ignore zero density regions

        Float invMaxDensity = 1.0 / maxD;

        // Run delta-tracking iterations to sample a medium interaction
        Float t = intervals[i].t_start;

        while (true) {
            t -= std::log(1 - sampler.Get1D()) * invMaxDensity / sig_t;
            if (t >= intervals[i].t_end) break;

            Float dense = density->D(ray(t))[0];

            Float diff = maxD - dense;

            // special handling of the case where we are clamping
            // and interact with negative null density. We then
            // assume that diff = 0.0, which means a real collision
            // always happens.
            // if (!Globals::NO_CLAMP && diff < -maxD) {
            //     majorant->UpdateMajorant(ray(t), dense);

            //     PhaseFunction* phase = ARENA_ALLOC(arena,
            //     HenyeyGreenstein)(g); *mi = MediumInteraction(rWorld(t),
            //     -rWorld.d, rWorld.time,
            //                             mediumRef, phase, ray, t, tMin);
            //     success = true;
            //     return weight * sig_s / sig_t;
            // }

            // if (diff < -maxD) diff = -maxD;
            // dense = 2.0 * maxD;

            if (dense > maxD) {
                majorant->UpdateMajorant(ray(t), dense);
            }

            Float null_weight = diff * invMaxDensity;
            Float prob_null = std::abs(diff) / (std::abs(diff) + dense);

            Float prob_real = dense / (std::abs(diff) + dense);
            Float real_weight = dense * invMaxDensity;

            Float scat_coeff = sig_s / sig_t;

            // if (dense > maxD) {
            //     std::cout << "NON-BOUNDING DELTA WEIGHTED" << std::endl;
            // }

            // Float real_tot = (dense + std::abs(diff));
            // Float inv = 1.0 / real_tot;

            if (prob_real > sampler.Get1D()) {
                // Populate _mi_ with medium interaction information and
                PhaseFunction* phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
                *mi = MediumInteraction(rWorld(t), -rWorld.d, rWorld.time,
                                        mediumRef, phase, ray, t, tMin);
                success = true;
                weight *= real_weight / prob_real * scat_coeff;
                return weight;
            }

            weight *= null_weight / prob_null;
        }
    }

    return Spectrum(weight);
}

Spectrum FF_Trans_Weighted_Signif_Sampler::Sample_Cull(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
    MediumInteraction* mi, Float tMin, Float tMax, Float sig_t, Float sig_s,
    bool& success) const {
    sampler.BeforeFreeFlightSample();

    // TODO: this method is not currently in use

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
