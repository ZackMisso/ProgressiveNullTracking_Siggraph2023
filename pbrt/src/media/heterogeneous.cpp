#include "heterogeneous.h"

#include "interaction.h"
#include "samplers/random.h"

namespace pbrt {

HeterogeneousMedium::HeterogeneousMedium(
    const Spectrum& sigma_a, const Spectrum& sigma_s, Float g,
    T_Estimator* t_est, DensityFunction* density, MinorantFunction* minorant,
    MajorantFunction* majorant, FF_Sampler* ff, const Transform& WorldToMedium,
    const Bounds3f b)
    : sigma_a(sigma_a),
      sigma_s(sigma_s),
      sigma_t(sigma_s + sigma_a),
      g(g),
      t_est(t_est),
      density(density),
      minorant(minorant),
      majorant(majorant),
      ff_sampler(ff),
      WorldToMedium(Inverse(WorldToMedium)),
      mediumBounds(b) {
    // initialize atomics
    Medium::nTrCalls = 0;
    Medium::nDensityCalls = 0;

    density->preProcess(WorldToMedium, mediumBounds);
    t_est->minorant = minorant;
    t_est->majorant = majorant;
    t_est->density = density;
    t_est->sigma_t = std::max(sigma_t[0], std::max(sigma_t[1], sigma_t[2]));
    // t_est->sigma_t = sigma_t[0];
    t_est->sigma_t_spec = sigma_t;
    t_est->preProcess();
    ff_sampler->mediumRef = this;
    ff_sampler->majorant = majorant;
    ff_sampler->density = density;
    ff_sampler->sigma_s = sigma_s[0];
    ff_sampler->sigma_t = sigma_t[0];
    ff_sampler->g = g;
    nTrCalls = 0;
    nDensityCalls = 0;
    t_est->preProcess();
}

HeterogeneousMedium::~HeterogeneousMedium() {
    delete density;
    delete t_est;
    delete majorant;
    delete minorant;
    delete ff_sampler;
}

void HeterogeneousMedium::setMajScale(Float scale) {
    majorant->setScale(scale);
}

#if RENDER_MUTABLE
Spectrum HeterogeneousMedium::Tr(const Ray& rWorld, Sampler& sampler) {
    // std::cout << "tr" << std::endl;
    ProfilePhase _(Prof::MediumTr);
    ++nTrCalls;

    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // check bounds
    if (outsideBounds) return Spectrum(1.f);

    // calculate transmittance using the specified estimator
    Spectrum tr = t_est->Tr(ray, sampler, tMin, tMax);

    Medium::nDensityCalls += density->getDensityCalls();
    density->clearDensityCalls();

    return tr;
}

Spectrum HeterogeneousMedium::Majorant(const Ray& rWorld) {
    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // check bounds
    if (outsideBounds) return Spectrum(0.f);

    Float maj = 0.0;

    std::vector<MacroGridQuery> queries =
        majorant->MacroVoxelTraversal(ray(tMin), ray(tMax), tMin, tMax);

    for (int i = 0; i < queries.size(); ++i) {
        // maj_max
        // if (queries[i].maj > maj) maj = queries[i].maj;
        // maj_avg
        maj += queries[i].maj * (queries[i].t_end - queries[i].t_start);
    }

    return Spectrum(maj /= (tMax - tMin));
}

BlanchetCorrelatedResults HeterogeneousMedium::Tr_Blanchet(const Ray& rWorld,
                                                           Sampler& sampler,
                                                           int blanchet_n) {
    ProfilePhase _(Prof::MediumTr);
    ++nTrCalls;

    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // check bounds
    if (outsideBounds) {
        return BlanchetCorrelatedResults(1.f);
    }

    // calculate transmittance using the specified estimator
    BlanchetCorrelatedResults tr =
        t_est->Tr_Blanchet(ray, sampler, tMin, tMax, blanchet_n);

    Medium::nDensityCalls += density->getDensityCalls();
    density->clearDensityCalls();

    return tr;
}

BlanchetCorrelatedResults HeterogeneousMedium::LocalTr_Blanchet(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax, int blanchet_n) {
    ++Medium::nTrCalls;

    BlanchetCorrelatedResults tr =
        t_est->Tr_Blanchet(ray, sampler, tMin, tMax, blanchet_n);

    Medium::nDensityCalls += density->getDensityCalls();
    density->clearDensityCalls();

    return tr;
}

Spectrum HeterogeneousMedium::LocalTr(const Ray& ray, Sampler& sampler,
                                      Float tMin, Float tMax) {
    ++Medium::nTrCalls;

    Spectrum tr = t_est->Tr(ray, sampler, tMin, tMax);

    Medium::nDensityCalls += density->getDensityCalls();
    density->clearDensityCalls();

    return tr;
}

Spectrum HeterogeneousMedium::Sample(const Ray& rWorld, Sampler& ff_samp,
                                     Sampler& trans_samp, MemoryArena& arena,
                                     MediumInteraction* mi) {
    ProfilePhase _(Prof::MediumSample);

    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // std::cout << "BOOM" << std::endl;

    // check bounds
    if (outsideBounds) return Spectrum(1.f);
    // return Spectrum(0.0); // debug

    // std::cout << "BOOM" << std::endl;

    bool scats = false;

    // return the sampled contribution and medium interaction
    Spectrum ret =
        ff_sampler->Sample(ray, rWorld, ff_samp, arena, mi, tMin, tMax, scats);

    // multiply by transmittance if ff sampling method requires it
    if (ff_sampler->RequiresTr()) {
        // std::cout << "IN HERE" << std::endl;
        ret *= LocalTr(ray, trans_samp, tMin, tMax);
    }
    // std::cout << "ret " << ret[0] << std::endl;

    if (ff_sampler->RequiresTrOnScatterOnly() && scats) {
        // std::cout << "loc: " << mi->tMax << std::endl;
        Spectrum unbiasd = LocalTr(ray, trans_samp, tMin, mi->tMax);
        // std::cout << "unbiased_trans: " << unbiasd[0] << std::endl;
        // ret *= LocalTr(ray, trans_samp, tMin, mi->tMax);
        ret *= unbiasd;
    }
    if (ff_sampler->RequiresTrOnScatterOnly() && !scats) {
        // std::cout << "whoop" << std::endl;
        // Spectrum unbiasd = LocalTr(ray, trans_samp, tMin, tMax);
        // std::cout << "unbiased_trans: " << biased_transmittance << std::endl;
        ret *= LocalTr(ray, trans_samp, tMin, tMax);
    }

    return ret;
}

BlanchetCorrelatedResults HeterogeneousMedium::Sample_Blanchet(
    const Ray& rWorld, Sampler& ff_samp, Sampler& trans_samp,
    MemoryArena& arena, MediumInteraction* mi, int blanchet_n) {
    ProfilePhase _(Prof::MediumSample);

    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // check bounds
    if (outsideBounds) return Spectrum(1.f);

    bool scats = false;

    BlanchetCorrelatedResults ret = ff_sampler->Sample_Blanchet(
        ray, rWorld, ff_samp, arena, mi, tMin, tMax, scats, blanchet_n);

    // multiply by transmittance if ff sampling method requires it
    if (ff_sampler->RequiresTr()) {
        ret *= LocalTr_Blanchet(ray, trans_samp, tMin, tMax, blanchet_n);
    }

    if (ff_sampler->RequiresTrOnScatterOnly() && scats) {
        ret *= LocalTr_Blanchet(ray, trans_samp, tMin, mi->tMax, blanchet_n);
    } else if (ff_sampler->RequiresTrOnScatterOnly() && !scats) {
        // std::cout << "ah" << std::endl;
        ret *= LocalTr_Blanchet(ray, trans_samp, tMin, tMax, blanchet_n);
    }

    return ret;
}

// // This returns true if the ray is outside the medium bounds
// bool HeterogeneousMedium::TransformToLocalSpace(const Ray& rWorld, Ray& ray,
//                                                 Float& tMin,
//                                                 Float& tMax) const {
//     // transform the ray to medium space
//     ray = WorldToMedium(
//         Ray(rWorld.o, Normalize(rWorld.d), rWorld.tMax * rWorld.d.Length()));

//     if (!mediumBounds.IntersectP(ray, &tMin, &tMax)) {
//         return true;
//     }

//     return false;
// }

#else
Spectrum HeterogeneousMedium::Tr(const Ray& rWorld, Sampler& sampler) const {
    // std::cout << "tr" << std::endl;
    ProfilePhase _(Prof::MediumTr);
    ++nTrCalls;

    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // check bounds
    if (outsideBounds) return Spectrum(1.f);

    // calculate transmittance using the specified estimator
    Spectrum tr = t_est->Tr(ray, sampler, tMin, tMax);

    Medium::nDensityCalls += density->getDensityCalls();
    density->clearDensityCalls();

    return tr;
}

BlanchetCorrelatedResults HeterogeneousMedium::Tr_Blanchet(
    const Ray& rWorld, Sampler& sampler, int blanchet_n) const {
    ProfilePhase _(Prof::MediumTr);
    ++nTrCalls;

    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // check bounds
    if (outsideBounds) {
        return BlanchetCorrelatedResults(1.f);
    }

    // calculate transmittance using the specified estimator
    BlanchetCorrelatedResults tr =
        t_est->Tr_Blanchet(ray, sampler, tMin, tMax, blanchet_n);

    Medium::nDensityCalls += density->getDensityCalls();
    density->clearDensityCalls();

    return tr;
}

BlanchetCorrelatedResults HeterogeneousMedium::LocalTr_Blanchet(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
    int blanchet_n) const {
    ++Medium::nTrCalls;

    BlanchetCorrelatedResults tr =
        t_est->Tr_Blanchet(ray, sampler, tMin, tMax, blanchet_n);

    Medium::nDensityCalls += density->getDensityCalls();
    density->clearDensityCalls();

    return tr;
}

Spectrum HeterogeneousMedium::LocalTr(const Ray& ray, Sampler& sampler,
                                      Float tMin, Float tMax) const {
    ++Medium::nTrCalls;

    Spectrum tr = t_est->Tr(ray, sampler, tMin, tMax);

    Medium::nDensityCalls += density->getDensityCalls();
    density->clearDensityCalls();

    return tr;
}

Spectrum HeterogeneousMedium::Sample(const Ray& rWorld, Sampler& ff_samp,
                                     Sampler& trans_samp, MemoryArena& arena,
                                     MediumInteraction* mi) const {
    ProfilePhase _(Prof::MediumSample);

    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // std::cout << "BOOM" << std::endl;

    // check bounds
    if (outsideBounds) return Spectrum(1.f);

    bool scats = false;

    // return the sampled contribution and medium interaction
    Spectrum ret =
        ff_sampler->Sample(ray, rWorld, ff_samp, arena, mi, tMin, tMax, scats);

    // multiply by transmittance if ff sampling method requires it
    if (ff_sampler->RequiresTr()) {
        ret *= LocalTr(ray, trans_samp, tMin, tMax);
    }

    if (ff_sampler->RequiresTrOnScatterOnly() && scats) {
        // std::cout << "loc: " << mi->tMax << std::endl;
        Spectrum unbiasd = LocalTr(ray, trans_samp, tMin, mi->tMax);
        // std::cout << "unbiased_trans: " << unbiasd[0] << std::endl;
        // ret *= LocalTr(ray, trans_samp, tMin, mi->tMax);
        ret *= unbiasd;
    }
    if (ff_sampler->RequiresTrOnScatterOnly() && !scats) {
        // std::cout << "whoop" << std::endl;
        // Spectrum unbiasd = LocalTr(ray, trans_samp, tMin, tMax);
        // std::cout << "unbiased_trans: " << biased_transmittance <<
        // std::endl;
        ret *= LocalTr(ray, trans_samp, tMin, tMax);
    }

    return ret;
}

BlanchetCorrelatedResults HeterogeneousMedium::Sample_Blanchet(
    const Ray& rWorld, Sampler& ff_samp, Sampler& trans_samp,
    MemoryArena& arena, MediumInteraction* mi, int blanchet_n) const {
    ProfilePhase _(Prof::MediumSample);

    Ray ray;
    Float tMin, tMax;

    // generate a transformed ray and bounds
    bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

    // check bounds
    if (outsideBounds) return Spectrum(1.f);

    bool scats = false;

    BlanchetCorrelatedResults ret = ff_sampler->Sample_Blanchet(
        ray, rWorld, ff_samp, arena, mi, tMin, tMax, scats, blanchet_n);

    // multiply by transmittance if ff sampling method requires it
    if (ff_sampler->RequiresTr()) {
        ret *= LocalTr_Blanchet(ray, trans_samp, tMin, tMax, blanchet_n);
    }

    if (ff_sampler->RequiresTrOnScatterOnly() && scats) {
        ret *= LocalTr_Blanchet(ray, trans_samp, tMin, mi->tMax, blanchet_n);
    } else if (ff_sampler->RequiresTrOnScatterOnly() && !scats) {
        // std::cout << "ah" << std::endl;
        ret *= LocalTr_Blanchet(ray, trans_samp, tMin, tMax, blanchet_n);
    }

    return ret;
}
#endif

// This returns true if the ray is outside the medium bounds
bool HeterogeneousMedium::TransformToLocalSpace(const Ray& rWorld, Ray& ray,
                                                Float& tMin,
                                                Float& tMax) const {
    // ray = rWorld;
    // tMin = 0.0;
    // tMax = ray.tMax;
    // return false;

    // transform the ray to medium space
    ray = WorldToMedium(
        Ray(rWorld.o, Normalize(rWorld.d), rWorld.tMax * rWorld.d.Length()));

    if (!mediumBounds.IntersectP(ray, &tMin, &tMax)) {
        // return false;
        return true;
    }

    return false;
}

}  // namespace pbrt
