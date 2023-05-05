#include "ff_delta_adap.h"

#include "interaction.h"
#include "phase.h"

namespace pbrt {

Spectrum FF_Trans_Adaptive_Sampler::Sample_ManualSigs(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
    MediumInteraction* mi, Float tMin, Float tMax, Float sig_t, Float sig_s,
    bool& success) const {
    sampler.BeforeFreeFlightSample();
    // Float invMaxDensity = 0.33;

    Float base_maj = 3.0;
    Float maj_to_use = base_maj;
    Float invMaj = 1.0 / maj_to_use;

    Spectrum ret = Spectrum(1.0);

    // Run delta-tracking iterations to sample a medium interaction
    Float t = tMin;
    Float wi = 1.0;

    while (true) {
        Float last_t = t;

        t -= std::log(1 - sampler.Get1D()) * invMaj / sig_t;
        if (t >= tMax) {
            t = tMax;
            Float mult_fact = exp((maj_to_use - base_maj) * (t - last_t));
            // if (mult_fact > 1e10 || isnan(mult_fact))
            //     std::cout << "Big bOI: " << mult_fact << std::endl;
            ret *= exp((maj_to_use - base_maj) * (t - last_t));
            break;
        }

        Float dense = density->D(ray(t))[0];
        Float chk = (base_maj - dense) / base_maj;
        // if (chk < 0.0) std::cout << "FACK" << std::endl;
        wi = (base_maj - dense) * invMaj *
             exp((maj_to_use - base_maj) * (t - last_t));

        Float accept = std::min(chk, 1.0);
        wi = wi / accept;
        maj_to_use = std::max(base_maj - dense, base_maj * 0.25);
        invMaj = 1.0 / maj_to_use;
        ret *= wi;

        if (accept < sampler.Get1D()) {
            // Populate _mi_ with medium interaction information and return
            PhaseFunction* phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
            *mi = MediumInteraction(rWorld(t), -rWorld.d, rWorld.time,
                                    mediumRef, phase, ray, t, tMin);
            success = true;
            // std::cout << "ret: " << ret << std::endl;
            return sig_s / sig_t * ret;
        }
    }

    return ret;
}

Spectrum FF_Trans_Adaptive_Sampler::Sample_MutableSigs(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
    MediumInteraction* mi, Float tMin, Float tMax, Float sig_t, Float sig_s,
    bool& success) {
    sampler.BeforeFreeFlightSample();
    // Float invMaxDensity = 0.33;

    // Float base_maj = 3.0;
    // Float maj_to_use = base_maj;
    // Float invMaj = 1.0 / maj_to_use;

    Spectrum ret = Spectrum(1.0);

    // Run delta-tracking iterations to sample a medium interaction
    Float t = tMin;
    Float wi = 1.0;

    std::vector<MacroGridQuery> intervals =
        majorant->MacroVoxelTraversal(ray(tMin), ray(tMax), tMin, tMax);

    // std::vector<Float> accepts = std::vector<Float>();
    // std::vector<Float> wis = std::vector<Float>();
    // std::vector<Float> rets = std::vector<Float>();
    // std::vector<Float> rands = std::vector<Float>();

    for (int i = 0; i < intervals.size(); ++i) {
        Float base_maj = intervals[i].maj;
        Float maj_to_use = base_maj;
        Float invMaj = 1.0 / maj_to_use;

        // Run delta-tracking iterations to sample a medium interaction
        Float t = intervals[i].t_start;
        while (true) {
            Float last_t = t;

            t -= std::log(1 - sampler.Get1D()) * invMaj / sig_t;
            if (t >= intervals[i].t_end) {
                t = intervals[i].t_end;
                // Float mult_fact = exp((maj_to_use - base_maj) * (t -
                // last_t)); if (std::abs(mult_fact) > 1e10 || isnan(mult_fact))
                //     std::cout << "Big bOI: " << mult_fact << std::endl;
                ret *= exp((maj_to_use - base_maj) * (t - last_t));
                break;
            }

            Float dense = density->D(ray(t))[0];

            if (dense > base_maj) {
                base_maj = dense;
                majorant->UpdateMajorant(ray(t), base_maj);
                return Spectrum(0.0);
            }

            Float chk = (base_maj - dense) / base_maj;
            // if (chk < 0.0) std::cout << "NON-BOUNDING" << std::endl;
            wi = (base_maj - dense) * invMaj *
                 exp((maj_to_use - base_maj) * (t - last_t));

            // wis.push_back(wi);

            Float accept = std::min(chk, 1.0);
            // accepts.push_back(accept);
            // wi = wi / accept;
            maj_to_use = std::max(base_maj - dense, base_maj * 0.25);
            invMaj = 1.0 / maj_to_use;
            ret *= wi;
            // rets.push_back(ret[0]);
            Float rand = sampler.Get1D();
            // rands.push_back(rand);

            if (accept < rand) {
                // Populate _mi_ with medium interaction information and return
                PhaseFunction* phase = ARENA_ALLOC(arena, HenyeyGreenstein)(g);
                *mi = MediumInteraction(rWorld(t), -rWorld.d, rWorld.time,
                                        mediumRef, phase, ray, t, tMin);
                success = true;
                // if (isnan(ret[0])) {
                //     std::cout << "ret: " << ret << std::endl;
                //     std::cout << "rets" << std::endl;
                //     for (int i = 0; i < rets.size(); ++i) {
                //         std::cout << rets[i] << std::endl;
                //     }
                //     std::cout << "rands" << std::endl;
                //     for (int i = 0; i < rands.size(); ++i) {
                //         std::cout << rands[i] << std::endl;
                //     }
                //     std::cout << "accepts" << std::endl;
                //     for (int i = 0; i < accepts.size(); ++i) {
                //         std::cout << accepts[i] << std::endl;
                //     }
                //     std::cout << "wis" << std::endl;
                //     for (int i = 0; i < wis.size(); ++i) {
                //         std::cout << wis[i] << std::endl;
                //     }
                //     std::cout << "end ret: " << ret << std::endl;
                // }

                return sig_s / sig_t * ret;
            }

            ret /= accept;
        }

        // if (ret[0] > 10.0 || isnan(ret[0]))
        //     std::cout << "ret_final: " << ret << std::endl;

        return ret;
    }
}

Spectrum FF_Trans_Adaptive_Sampler::Sample_Cull(
    const Ray& ray, const Ray& rWorld, Sampler& sampler, MemoryArena& arena,
    MediumInteraction* mi, Float tMin, Float tMax, Float sig_t, Float sig_s,
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
