#include "ff_analytic.h"
#include "phase.h"
#include "interaction.h"

namespace pbrt
{

BlanchetCorrelatedResults FF_Analytic::Sample_Blanchet
(
    const Ray& ray,
    const Ray& rWorld,
    Sampler& sampler,
    MemoryArena& arena,
    MediumInteraction* mi,
    Float tMin,
    Float tMax,
    bool& success,
    int blanchet_n) const
{
    std::cout << "whoop" << std::endl;
    return BlanchetCorrelatedResults(Sample_Cull_Function(ray,
                                rWorld,
                                sampler,
                                arena,
                                mi,
                                tMin,
                                tMax,
                                success,
                                1.0));

    // Point3f a = ray(tMin);
    // Point3f b = ray(tMax);

    // std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    // MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    // if (path.tMin >= path.tMax)
    // {
    //     return BlanchetCorrelatedResults(Spectrum(1.0));
    // }

    // // bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
    // // bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;

    // Float od_sample = trans_func->sample(sampler, true);

    // Float od = 0.0;
    // Float t = path.tMin;
    // for (int i = 0; i < path.non_empty.size(); ++i)
    // {
    //     Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
    //     Float past_od = od;
    //     od += path.non_empty[i].maj * step * sigma_t;

    //     if (od > od_sample)
    //     {
    //         Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

    //         PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
    //         *mi = MediumInteraction(rWorld(path.unmap(t)),
    //                                 -rWorld.d,
    //                                 rWorld.time,
    //                                 mediumRef,
    //                                 phase,
    //                                 ray,
    //                                 path.unmap(t),
    //                                 tMin);

    //         success = true;
    //         return BlanchetCorrelatedResults(sigma_s / (sigma_t));
    //     }
    //     t += step;
    // }

    // return BlanchetCorrelatedResults(1.0);
}

Spectrum FF_Analytic::Sample_Cull_Function(const Ray& ray,
                                                       const Ray& rWorld,
                                                       Sampler& sampler,
                                                       MemoryArena& arena,
                                                       MediumInteraction* mi,
                                                       Float tMin,
                                                       Float tMax,
                                                       bool& success,
                                                       Float scale) const
{
    sampler.BeforeFreeFlightSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    // if (path.tMin >= path.tMax)
    // {
    //     return Spectrum(1.0);
    // }

    std::cout << "analytic: this is broken" << std::endl;

    // 1 - rand() for numberical stability
    Float od_sample = -std::log(1.0 - sampler.Get1D());

    // not using path
    Float od = 0.0;
    Float t = tMin;
    for (int i = 0; i < queries.size(); ++i)
    {
        Float step = (queries[i].t_end - queries[i].t_start) * (tMax - tMin);
        Float past_od = od;
        od += queries[i].maj * step * sigma_t;

        if (od > od_sample)
        {
            Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

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
            return Spectrum(sigma_s / (sigma_t));
        }
        t += step;
    }

    return Spectrum(1.0);
}

Spectrum FF_Analytic::Sample_ManualSigs
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
    bool& success) const
{
    // std::cout << "fuck" << std::endl;
    // throw new std::exception();
    return Sample_Cull_Function(ray,
                                rWorld,
                                sampler,
                                arena,
                                mi,
                                tMin,
                                tMax,
                                success,
                                1.0);

    sampler.BeforeFreeFlightSample();

    // std::cout << "Pre_tMin: " << tMin << std::endl;
    // std::cout << "Pre_tMax: " << tMax << std::endl;
    //
    // if (!cull_unnecessary_space(ray, tMin, tMax))
    // {
    //     success = false;
    //     return Spectrum(1.f);
    // }
    //
    // std::cout << "Post_tMin: " << tMin << std::endl;
    // std::cout << "Post_tMax: " << tMax << std::endl;
    // std::cout << std::endl;

    // std::cout << "bab" << std::endl;
    //
    // std::vector<MacroGridQuery> macros = majorant->MacroVoxelTraversal(ray(tMin), ray(tMax));
    //
    // std::cout << "boop" << std::endl;
    //
    // std::cout << macros.size() << std::endl;

    // TODO: replace 1.0 with the majorant
    Float invMaxDensity = 1.0 / sigma_t;
    Float base_step = invMaxDensity * base_step_size;
    Float t = tMin;

    // 1 - rand() for numberical stability
    Float od_sample = -std::log(1.0 - sampler.Get1D());
    Float od = 0.0;
    // Float samp = sampler.Get1D();

    // std::cout << "tMin: " << tMin << std::endl;
    // std::cout << "tMax: " << tMax << std::endl;

    while (t < tMax)
    {
        // std::cout << "pergatory is hell: " << t << std::endl;
        Float step = std::min(base_step, tMax - t);
        // Float loc = t + samp * step;
        Float loc = t + sampler.Get1D() * step;

        // NOT THIS DUMBASS

        Spectrum t_dense = density->D(ray(loc)) * sigma_t;
        // Spectrum t_dense = majorant->Maj(ray(loc)) * sigma_t;

        // if (t_dense[0] > majorant->Maj(ray(loc)))
        // {
        //     throw new std::exception();
        // }

        // std::cout << "post D" << std::endl;

        Float dense = ThreeWiseMax(t_dense[0], t_dense[1], t_dense[2]);
        // dense = std::max((0.5 * base_step_size) / (tMax - tMin), dense);
        // dense = std::max(0.01, dense);

        Float past_od = od;
        od += dense * step;

        if (od >= od_sample)
        {
            Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

            Spectrum scat_dense = density->D(ray(actual_loc));
            // Spectrum scat_dense = majorant->Maj(ray(actual_loc));
            Spectrum sigma_st = sigma_s * scat_dense;
            // Spectrum sigma_st = sigma_s * t_dense;
            Spectrum sigma_tt = Spectrum(dense);

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

            Float biased_transmittance = 1.0;
            // Float biased_transmittance = exp(-od_sample);
            // Float biased_transmittance = exp(-(past_od + (loc-t) * dense));

            return sigma_st / (sigma_tt * biased_transmittance);
        }

        t += base_step;
    }

    success = false;

    // Float biased_transmittance = exp(-od);
    Float biased_transmittance = 1.0;

    return Spectrum(1.0 / biased_transmittance);
}

Spectrum FF_Analytic::Sample
(
    const Ray& ray,
    const Ray& rWorld,
    Sampler& sampler,
    MemoryArena& arena,
    MediumInteraction* mi,
    Float tMin,
    Float tMax,
    bool& success,
    uint32_t flags,
    TransportMode mode
) const
{
    sampler.BeforeFreeFlightSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax)
    {
        return (Spectrum(1.0));
    }

    bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
    // bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;

    Float od_sample = trans_func->sample(sampler, startOnSurface);

    // std::cout << "hey_listen" << std::endl;

    Float od = 0.0;
    Float t = path.tMin;
    for (int i = 0; i < path.non_empty.size(); ++i)
    {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step * sigma_t;
        if (od > od_sample)
        {
            Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

            // TODO: current Cloud renders are bugged, switch to "t"
            //       instead for those comparisons
            PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(path.unmap(actual_loc)),
                                    -rWorld.d,
                                    rWorld.time,
                                    mediumRef,
                                    phase,
                                    ray,
                                    path.unmap(actual_loc),
                                    tMin);

            success = true;
            // std::cout << "whiip" << std::endl;
            #if FULL_TRANS_COMPARE
                return sigma_s / (sigma_t) * Spectrum(1.0) / trans_func->eval(od_sample, startOnSurface, false, mode == TransportMode::Radiance);
            #endif
            return Spectrum(sigma_s / (sigma_t));
        }
        t += step;
    }

    #if FULL_TRANS_COMPARE
        return Spectrum(1.0) / trans_func->eval(od, startOnSurface, true, mode == TransportMode::Radiance);
    #endif

    return Spectrum(1.0);
}

Spectrum FF_Analytic::Sample_Beta(const Ray& ray,
                                         const Ray& rWorld,
                                         Sampler& sampler,
                                         MemoryArena& arena,
                                         MediumInteraction* mi,
                                         Float tMin,
                                         Float tMax,
                                         Float beta, Float c,
                                         bool& success,
                                         uint32_t flags,
                                         TransportMode mode) const
{
    sampler.BeforeFreeFlightSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax)
    {
        return (Spectrum(1.0));
    }

    bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
    bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;

    Float od_sample = trans_func->sample(sampler, beta, c, startOnSurface);

    Float od = 0.0;
    Float t = path.tMin;
    for (int i = 0; i < path.non_empty.size(); ++i)
    {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step;
        if (od > od_sample)
        {
            Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

            PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(path.unmap(t)),
                                    -rWorld.d,
                                    rWorld.time,
                                    mediumRef,
                                    phase,
                                    ray,
                                    path.unmap(t),
                                    tMin);

            success = true;
            // std::cout << "whiip" << std::endl;
            return Spectrum(sigma_s / (sigma_t));
        }
        t += step;
    }

    return Spectrum(1.0);

    // sampler.BeforeFreeFlightSample();
    //
    // Point3f a = ray(tMin);
    // Point3f b = ray(tMax);
    //
    // // if (beta >= 1.0 || c >= 1.0)
    // // {
    // //     std::cout << "ACK taylor" << std::endl;
    // // }
    //
    // std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);
    //
    // MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);
    //
    // // TODO: replace this with the majorant
    // Float invMaxDensity = 1.0;
    // Float base_step = invMaxDensity * base_step_size / sigma_t;
    // Float t = path.tMin;
    //
    // if (path.tMin >= path.tMax)
    // {
    //     return Spectrum(1.0);
    // }
    //
    // base_step = 0.01;
    //
    // bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
    // bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
    //
    // // if (!startOnSurface) std::cout << "fuck blanchet" << std::endl;
    //
    // Float od_sample = trans_func->sample(sampler, beta, c, startOnSurface);
    // // std::cout << "od_sample: " << od_sample << std::endl;
    // Float od = 0.0;
    //
    // // std::cout << "startOD: " << od << std::endl;
    // // std::cout << "base_step: " << base_step << std::endl;
    //
    // Float samp = sampler.Get1D();
    //
    // while (t < path.tMax)
    // {
    //     Float step = std::min(base_step, path.tMax - t);
    //     Float loc = t + samp * step;
    //
    //     Spectrum dden = density->D(ray(path.unmap(loc)));
    //
    //     Float dense = ThreeWiseMax(dden[0], dden[1], dden[2]);
    //
    //     Float past_od = od;
    //     od += dense * step * sigma_t;
    //
    //     if (od > od_sample)
    //     {
    //         // std::cout << "found it: " << loc << std::endl;
    //         Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);
    //
    //         Spectrum scat_dense = density->D(ray(path.unmap(actual_loc)));
    //
    //         Spectrum sigma_st = sigma_s * scat_dense;
    //         Spectrum sigma_tt = Spectrum(dense) * sigma_t;
    //
    //         // Populate _mi_ with medium interaction information and return
    //         PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
    //         *mi = MediumInteraction(rWorld(path.unmap(actual_loc)),
    //                                 -rWorld.d,
    //                                 rWorld.time,
    //                                 mediumRef,
    //                                 phase,
    //                                 ray,
    //                                 path.unmap(actual_loc),
    //                                 tMin);
    //
    //         Spectrum biased_transmittance = trans_func->eval(od_sample,
    //                                                          beta, c,
    //                                                          startOnSurface,
    //                                                          endOnSurface,
    //                                                          true);
    //
    //         if (biased_transmittance[0] == 0.0)
    //         {
    //             std::cout << "WAHT FUF" << std::endl;
    //         }
    //         success = true;
    //         // return sigma_s / sigma_t;
    //         return sigma_st / (sigma_tt * biased_transmittance);
    //     }
    //
    //     t += base_step;
    // }
    //
    // success = false;
    // Spectrum biased_transmittance = trans_func->eval(od,
    //                                                  beta, c,
    //                                                  startOnSurface,
    //                                                  endOnSurface,
    //                                                  true);
    //
    // if (biased_transmittance[0] == 0.0)
    // {
    //  std::cout << "WAHT FUF 2" << std::endl;
    // }
    //
    // return Spectrum(1.f) / biased_transmittance;
}

// BlanchetCorrelatedResults FF_Unbiased_Raymarching::Sample_Blanchet
// (
//     const Ray& ray,
//     const Ray& rWorld,
//     Sampler& sampler,
//     MemoryArena& arena,
//     MediumInteraction* mi,
//     Float tMin,
//     Float tMax,
//     bool& success,
//     int blanchet_n,
//     uint32_t flags,
//     TransportMode mode
// ) const
// {
//     sampler.BeforeFreeFlightSample();
//
//     Point3f a = ray(tMin);
//     Point3f b = ray(tMax);
//
//     std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);
//
//     MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);
//
//     Float invMaxDensity = 1.0 / sigma_t;
//     Float base_step = invMaxDensity * base_step_size;
//     Float t = path.tMin;
//
//     bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     // bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//
//     Float trans_sample = sampler.Get1D(); //trans_func->sample(sampler, startOnSurface);
//     Float od = 0.0;
//     Float samp = sampler.Get1D();
//
//     Spectrum trans = Spectrum(1.0);
//
//     while (t < path.tMax)
//     {
//         Float step = std::min(base_step, path.tMax - t);
//         Float loc = t + samp * step;
//
//         Spectrum t_dense = density->D(ray(path.unmap(loc))) * sigma_t;
//
//         Float dense = ThreeWiseMax(t_dense[0], t_dense[1], t_dense[2]);
//         dense = std::max(0.02, dense);
//
//         Float past_od = od;
//         od += dense * step;
//
//         Spectrum trans = trans_func->eval(od,
//                                           startOnSurface,
//                                           false,
//                                           true);
//
//         if (trans[0] < trans_sample)
//         {
//             Spectrum scat_dense = density->D(ray(path.unmap(loc)));
//
//             Spectrum sigma_st = sigma_s * scat_dense;
//             Spectrum sigma_tt = Spectrum(dense);
//
//             // Populate _mi_ with medium interaction information and return
//             PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
//             *mi = MediumInteraction(rWorld(path.unmap(loc)),
//                                     -rWorld.d,
//                                     rWorld.time,
//                                     mediumRef,
//                                     phase,
//                                     ray,
//                                     path.unmap(loc),
//                                     tMin);
//
//             // if (endOnSurface) throw new std::exception();
//
//             // Spectrum biased_transmittance = trans_func->eval(od_sample,
//             //                                                  startOnSurface,
//             //                                                  endOnSurface,
//             //                                                  true);
//
//             Spectrum biased_transmittance = trans_func->eval(od_sample,
//                                                              startOnSurface,
//                                                              false,
//                                                              true);
//
//             // Spectrum biased_transmittance = Spectrum(trans_sample);
//
//             success = true;
//             return BlanchetCorrelatedResults(sigma_st / (sigma_tt * biased_transmittance));
//         }
//
//         t += base_step;
//     }
//
//     // if (!endOnSurface) throw new std::exception();
//
//     success = false;
//     // Spectrum biased_transmittance = trans_func->eval(od,
//     //                                                  startOnSurface,
//     //                                                  endOnSurface,
//     //                                                  true);
//
//     Spectrum biased_transmittance = trans_func->eval(od,
//                                                      startOnSurface,
//                                                      true,
//                                                      true);
//
//     // Spectrum biased_transmittance = trans;
//
//     return BlanchetCorrelatedResults(Spectrum(1.f) / biased_transmittance);
// }

BlanchetCorrelatedResults FF_Analytic::Sample_Blanchet
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
) const
{
    sampler.BeforeFreeFlightSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax)
    {
        return BlanchetCorrelatedResults(Spectrum(1.0));
    }

    bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
    // bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;

    Float od_sample = trans_func->sample(sampler, startOnSurface);

    Float od = 0.0;
    Float t = path.tMin;
    for (int i = 0; i < path.non_empty.size(); ++i)
    {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step * sigma_t;

        if (od > od_sample)
        {
            Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

            PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(path.unmap(actual_loc)),
                                    -rWorld.d,
                                    rWorld.time,
                                    mediumRef,
                                    phase,
                                    ray,
                                    path.unmap(actual_loc),
                                    tMin);

            success = true;
            #if FULL_TRANS_COMPARE
                return BlanchetCorrelatedResults(sigma_s / sigma_t) * BlanchetCorrelatedResults(Spectrum(1.0) / trans_func->eval(od_sample, startOnSurface, false, mode == TransportMode::Radiance));
            #endif
            // std::cout << "hello" << std::endl;
            return BlanchetCorrelatedResults(sigma_s / (sigma_t));
        }
        t += step;
    }

    #if FULL_TRANS_COMPARE
        return BlanchetCorrelatedResults(1.0) / BlanchetCorrelatedResults(trans_func->eval(od, startOnSurface, true, mode == TransportMode::Radiance));
    #endif

    return BlanchetCorrelatedResults(1.0);

    // Point3f a = ray(tMin);
    // Point3f b = ray(tMax);
    //
    // std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);
    //
    // MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);
    //
    // Float invMaxDensity = 1.0 / sigma_t;
    // Float base_step = invMaxDensity * base_step_size;
    // Float t = path.tMin;
    //
    // base_step = 0.1;
    //
    // bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
    // // bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
    //
    // Float od_sample = trans_func->sample(sampler, startOnSurface);
    // Float od = 0.0;
    // Float samp = sampler.Get1D();
    //
    // // std::cout << "START" << std::endl;
    //
    // while (t < path.tMax)
    // {
    //     Float step = std::min(base_step, path.tMax - t);
    //     Float loc = t + samp * step;
    //
    //     Spectrum t_dense = density->D(ray(path.unmap(loc))) * sigma_t;
    //
    //     Float dense = ThreeWiseMax(t_dense[0], t_dense[1], t_dense[2]);
    //     dense = std::max(0.02, dense);
    //
    //     Float past_od = od;
    //     od += dense * step;
    //
    //     if (od > od_sample)
    //     {
    //         Spectrum scat_dense = density->D(ray(path.unmap(loc)));
    //
    //         Spectrum sigma_st = sigma_s * scat_dense;
    //         Spectrum sigma_tt = Spectrum(dense);
    //
    //         // Populate _mi_ with medium interaction information and return
    //         PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
    //         *mi = MediumInteraction(rWorld(path.unmap(loc)),
    //                                 -rWorld.d,
    //                                 rWorld.time,
    //                                 mediumRef,
    //                                 phase,
    //                                 ray,
    //                                 path.unmap(loc),
    //                                 tMin);
    //
    //         // if (endOnSurface) throw new std::exception();
    //
    //         // Spectrum biased_transmittance = trans_func->eval(od_sample,
    //         //                                                  startOnSurface,
    //         //                                                  endOnSurface,
    //         //                                                  true);
    //
    //
    //         // std::cout << "fuf" << std::endl;
    //         Spectrum biased_pdf = trans_func->eval(od_sample,
    //                                                startOnSurface,
    //                                                false,
    //                                                true);
    //         // if (!startOnSurface)
    //         // {
    //         //     std::cout << "od_sample: " << od_sample << std::endl;
    //         //     std::cout << biased_pdf << std::endl;
    //         // }
    //
    //         // biased_pdf = Spectrum(1.0);
    //
    //         // std::cout << "biased_pdf: " << biased_pdf << std::endl;
    //
    //         // if (biased_pdf[0] > 1.0)
    //         // {
    //         //     std::cout << "trans: " << biased_pdf[0] << std::endl;
    //         //     std::cout << "od_sample: " << od_sample << std::endl;
    //         // }
    //
    //         success = true;
    //         return BlanchetCorrelatedResults(sigma_st / (sigma_tt * biased_pdf));
    //     }
    //
    //     t += base_step;
    // }
    //
    // // if (!endOnSurface) throw new std::exception();
    //
    // success = false;
    //
    // Spectrum surface_prob = trans_func->eval(od,
    //                                          startOnSurface,
    //                                          true,
    //                                          true);
    //
    // // surface_prob = Spectrum(1.0);
    //
    // return BlanchetCorrelatedResults(Spectrum(1.f) / surface_prob);
}

BlanchetCorrelatedResults FF_Analytic::Sample_Blanchet_Beta
(
    const Ray& ray,
    const Ray& rWorld,
    Sampler& sampler,
    Float beta, Float c,
    MemoryArena& arena,
    MediumInteraction* mi,
    Float tMin,
    Float tMax,
    bool& success,
    int blanchet_n,
    uint32_t flags,
    TransportMode mode
) const
{
    sampler.BeforeFreeFlightSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax)
    {
        return BlanchetCorrelatedResults(Spectrum(1.0));
    }

    bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
    bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;

    Float od_sample = trans_func->sample(sampler, beta, c, startOnSurface);

    Float od = 0.0;
    Float t = path.tMin;
    for (int i = 0; i < path.non_empty.size(); ++i)
    {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step * sigma_t;
        if (od > od_sample)
        {
            Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

            PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(path.unmap(t)),
                                    -rWorld.d,
                                    rWorld.time,
                                    mediumRef,
                                    phase,
                                    ray,
                                    path.unmap(t),
                                    tMin);

            success = true;
            return BlanchetCorrelatedResults(sigma_s / (sigma_t));
        }
        t += step;
    }

    return BlanchetCorrelatedResults(1.0);
}

BlanchetCorrelatedFdDebiasing FF_Analytic::Sample_Blanchet_FdBeta
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
) const
{
    sampler.BeforeFreeFlightSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    // BlanchetCorrelatedFdDebiasing ret =
    //     BlanchetCorrelatedFdDebiasing(Sample_Blanchet(ray, rWorld, sampler, arena, mi, tMin, tMax, success, blanchet_n, flags, mode).bias);
    //
    // return ret;

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    bool adjoint = mode == TransportMode::Radiance;

    if (path.tMin >= path.tMax)
    {
        return BlanchetCorrelatedFdDebiasing(Spectrum(1.0));
    }

    bool startOnSurface = getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
    bool endOnSurface = getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;

    // std::cout << "c: " << davis_c << std::endl;

    Float od_sample = trans_func->sample(sampler, davis_beta, davis_c, startOnSurface);
    // Float od_sample = trans_func->sample(sampler, startOnSurface);

    Float h1 = base_h * pow(2.0, -Float(davis_n));
    Float h2 = base_h * pow(2.0, -Float(davis_n+1));

    Float od = 0.0;
    Float t = path.tMin;
    for (int i = 0; i < path.non_empty.size(); ++i)
    {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step * sigma_t;

        if (od > od_sample)
        {
            Float actual_loc = t + step * (od_sample - past_od) / (od - past_od);

            PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(path.unmap(t)),
                                    -rWorld.d,
                                    rWorld.time,
                                    mediumRef,
                                    phase,
                                    ray,
                                    path.unmap(t),
                                    tMin);

            success = true;

            BlanchetCorrelatedFdDebiasing tr = BlanchetCorrelatedFdDebiasing(sigma_s / sigma_t);

            // od = od_sample;
            //
            // Spectrum val = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, false, adjoint);
            // //
            // tr *= val;
            return tr;

            // // null
            // tr.bias[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.even[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.odd[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.all[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
            //
            // // h
            // tr.bias[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.even[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.odd[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.all[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
            //
            // tr.bias[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.even[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.odd[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.all[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
            //
            // // h1
            // tr.bias[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.even[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.odd[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.all[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
            //
            // tr.bias[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.even[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.odd[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.all[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
            //
            // // h2
            // tr.bias[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.even[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.odd[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.all[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
            //
            // tr.bias[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.even[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.odd[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.all[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);

            // null
            // tr.bias[0] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.even[0] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.odd[0] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
            // tr.all[0] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
            //
            // // h
            // tr.bias[1] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + base_h, startOnSurface, endOnSurface, adjoint);
            // tr.even[1] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + base_h, startOnSurface, endOnSurface, adjoint);
            // tr.odd[1] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + base_h, startOnSurface, endOnSurface, adjoint);
            // tr.all[1] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + base_h, startOnSurface, endOnSurface, adjoint);
            //
            // tr.bias[2] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - base_h, startOnSurface, endOnSurface, adjoint);
            // tr.even[2] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - base_h, startOnSurface, endOnSurface, adjoint);
            // tr.odd[2] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - base_h, startOnSurface, endOnSurface, adjoint);
            // tr.all[2] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - base_h, startOnSurface, endOnSurface, adjoint);
            //
            // // h1
            // tr.bias[3] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + h1, startOnSurface, endOnSurface, adjoint);
            // tr.even[3] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + h1, startOnSurface, endOnSurface, adjoint);
            // tr.odd[3] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + h1, startOnSurface, endOnSurface, adjoint);
            // tr.all[3] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + h1, startOnSurface, endOnSurface, adjoint);
            //
            // tr.bias[4] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - h1, startOnSurface, endOnSurface, adjoint);
            // tr.even[4] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - h1, startOnSurface, endOnSurface, adjoint);
            // tr.odd[4] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - h1, startOnSurface, endOnSurface, adjoint);
            // tr.all[4] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - h1, startOnSurface, endOnSurface, adjoint);
            //
            // // h2
            // tr.bias[5] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + h2, startOnSurface, endOnSurface, adjoint);
            // tr.even[5] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + h2, startOnSurface, endOnSurface, adjoint);
            // tr.odd[5] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + h2, startOnSurface, endOnSurface, adjoint);
            // tr.all[5] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c + h2, startOnSurface, endOnSurface, adjoint);
            //
            // tr.bias[6] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - h2, startOnSurface, endOnSurface, adjoint);
            // tr.even[6] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - h2, startOnSurface, endOnSurface, adjoint);
            // tr.odd[6] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - h2, startOnSurface, endOnSurface, adjoint);
            // tr.all[6] = Spectrum(1.f) / trans_func->eval(od_sample, davis_beta, davis_c - h2, startOnSurface, endOnSurface, adjoint);

            // tr.bias[0] = sigma_s / sigma_t;
            // tr.bias[1] = sigma_s / sigma_t + base_h;
            // tr.bias[2] = sigma_s / sigma_t - base_h;
            // tr.bias[3] = sigma_s / sigma_t + h1;
            // tr.bias[4] = sigma_s / sigma_t - h1;
            // tr.bias[5] = sigma_s / sigma_t + h2;
            // tr.bias[6] = sigma_s / sigma_t - h2;

            // return tr;

            return tr;
        }
        t += step;
    }

    // if (od == 0.0) std::cout << "THERE I T IS" << std::endl;

    Spectrum val = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, true, adjoint);

    // BlanchetCorrelatedFdDebiasing tr = BlanchetCorrelatedFdDebiasing(val);
    BlanchetCorrelatedFdDebiasing tr = BlanchetCorrelatedFdDebiasing(Spectrum(1.f));
    return val;
    // // null
    // tr.bias[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.even[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.all[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // // h
    // tr.bias[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.even[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.all[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.even[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.all[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // // h1
    // tr.bias[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.even[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.all[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.even[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.all[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // // h2
    // tr.bias[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.even[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.all[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.even[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.all[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);

    // null
    // tr.bias[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.even[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.all[0] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // // h
    // tr.bias[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + base_h, startOnSurface, endOnSurface, adjoint);
    // tr.even[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + base_h, startOnSurface, endOnSurface, adjoint);
    // tr.odd[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + base_h, startOnSurface, endOnSurface, adjoint);
    // tr.all[1] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + base_h, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - base_h, startOnSurface, endOnSurface, adjoint);
    // tr.even[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - base_h, startOnSurface, endOnSurface, adjoint);
    // tr.odd[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - base_h, startOnSurface, endOnSurface, adjoint);
    // tr.all[2] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - base_h, startOnSurface, endOnSurface, adjoint);
    //
    // // h1
    // tr.bias[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + h1, startOnSurface, endOnSurface, adjoint);
    // tr.even[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + h1, startOnSurface, endOnSurface, adjoint);
    // tr.odd[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + h1, startOnSurface, endOnSurface, adjoint);
    // tr.all[3] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + h1, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - h1, startOnSurface, endOnSurface, adjoint);
    // tr.even[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - h1, startOnSurface, endOnSurface, adjoint);
    // tr.odd[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - h1, startOnSurface, endOnSurface, adjoint);
    // tr.all[4] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - h1, startOnSurface, endOnSurface, adjoint);
    //
    // // h2
    // tr.bias[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + h2, startOnSurface, endOnSurface, adjoint);
    // tr.even[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + h2, startOnSurface, endOnSurface, adjoint);
    // tr.odd[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + h2, startOnSurface, endOnSurface, adjoint);
    // tr.all[5] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c + h2, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - h2, startOnSurface, endOnSurface, adjoint);
    // tr.even[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - h2, startOnSurface, endOnSurface, adjoint);
    // tr.odd[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - h2, startOnSurface, endOnSurface, adjoint);
    // tr.all[6] = Spectrum(1.f) / trans_func->eval(od, davis_beta, davis_c - h2, startOnSurface, endOnSurface, adjoint);

    return tr;
}

}
