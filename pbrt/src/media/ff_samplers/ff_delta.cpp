#include "ff_delta.h"

#include "interaction.h"
#include "phase.h"

namespace pbrt {

Spectrum FF_Trans_Sampler::Sample_ManualSigs(const Ray& ray, const Ray& rWorld,
                                             Sampler& sampler,
                                             MemoryArena& arena,
                                             MediumInteraction* mi, Float tMin,
                                             Float tMax, Float sig_t,
                                             Float sig_s, bool& success) const {
    // return Sample_Cull(ray,
    //                    rWorld,
    //                    sampler,
    //                    arena,
    //                    mi,
    //                    tMin,
    //                    tMax,
    //                    success);

    // std::cout << "WHOOOO" << std::endl;

    sampler.BeforeFreeFlightSample();
    Float invMaxDensity = 0.33;
    // std::cout << "WHAT" << std::endl;
    // std::cout << "maj: " << maj << std::endl;
    // std::cout << "in here" << std::endl;

    // std::cout << "sig_T: " << sig_t << std::endl;
    // std::cout << "sig_S: " << sig_s << std::endl;

    // Run delta-tracking iterations to sample a medium interaction
    Float t = tMin;
    while (true) {
        // // THIS SHOULD BE DELETED AFTER TESTING
        // Float test_t = 1.0;

        t -= std::log(1 - sampler.Get1D()) * invMaxDensity / sig_t;
        // t -= std::log(1 - sampler.Get1D()) * invMaxDensity / test_t;
        if (t >= tMax) break;

        // Float dense = ThreeWiseMax(density->D(ray(t))[0],
        //                            density->D(ray(t))[1],
        //                            density->D(ray(t))[2]);

        Float dense = density->D(ray(t))[0];

        // if (dense > 1)
        //     std::cout << dense << std::endl;
        // if (dense * invMaxDensity > 1.0)
        // {
        //     std::cout << "FUCK" << std::endl;
        //     std::cout << "DENSE: " << dense << std::endl;
        // }

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

Spectrum FF_Trans_Sampler::Sample_MutableSigs(const Ray& ray, const Ray& rWorld,
                                              Sampler& sampler,
                                              MemoryArena& arena,
                                              MediumInteraction* mi, Float tMin,
                                              Float tMax, Float sig_t,
                                              Float sig_s, bool& success) {
    sampler.BeforeFreeFlightSample();

    // NEW VERSION:
    std::vector<MacroGridQuery> intervals =
        majorant->MacroVoxelTraversal(ray(tMin), ray(tMax), tMin, tMax);

    // std::cout << "Num intervals: " << intervals.size() << std::endl;
    // std::cout << "Maj: " << intervals[0].maj << std::endl;
    // std::cout << std::endl;

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

            if (dense > maxD) {
                // std::cout << "NON-BOUNDING DELTA" << std::endl;
                // std::cout << "maxD: " << maxD << std::endl;
                // std::cout << "dense: " << dense << std::endl;
                // std::cout << "dense2: " << density->D_Debug(ray(t))[0]
                //           << std::endl;
                // std::cout << std::endl;
#if NO_CLAMP
                // std::cout << "IN HERE" << std::endl;
                PhaseFunction* phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
                *mi = MediumInteraction(rWorld(t), -rWorld.d, rWorld.time,
                                        mediumRef, phase, ray, t, tMin);
                success = true;
                return sig_s / sig_t;
#else
                // maxD = dense;
                Float tmp = dense;
                dense = maxD;
                maxD = tmp;
                majorant->UpdateMajorant(ray(t), maxD);
                // return Spectrum(0.0);
#endif
            }

            if (dense * invMaxDensity > sampler.Get1D()) {
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

Spectrum FF_Trans_Sampler::Sample_Cull(const Ray& ray, const Ray& rWorld,
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
