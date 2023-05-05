#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_HETEROGENEOUSMEDIA_H
#define PBRT_HETEROGENEOUSMEDIA_H

// media/heterogeneous.h*
#include <atomic>

#include "ff_samplers/ff_sampler.h"
#include "majorant/majorant.h"
#include "medium.h"
#include "minorant.h"
#include "pbrt.h"
#include "stats.h"
#include "trans_ests/t_est.h"
#include "transform.h"

namespace pbrt {

// TODO: uncomment this after full integration
// STAT_MEMORY_COUNTER("Memory/Volume density", densityBytes);

class HeterogeneousMedium : public Medium {
  public:
    HeterogeneousMedium(const Spectrum& sigma_a, const Spectrum& sigma_s,
                        Float g, T_Estimator* t_est, DensityFunction* density,
                        MinorantFunction* minorant, MajorantFunction* majorant,
                        FF_Sampler* ff, const Transform& WorldToMedium,
                        const Bounds3f b);

    ~HeterogeneousMedium();

    // temporary clean later
    void setMajScale(Float scale);

#if RENDER_MUTABLE
    Spectrum Tr(const Ray& ray, Sampler& sampler);

    BlanchetCorrelatedResults Tr_Blanchet(const Ray& ray, Sampler& sampler,
                                          int blanchet_n);

    Spectrum LocalTr(const Ray& ray, Sampler& sampler, Float tMin, Float tMax);

    BlanchetCorrelatedResults LocalTr_Blanchet(const Ray& ray, Sampler& sampler,
                                               Float tMin, Float tMax,
                                               int blanchet_n);

    Spectrum Majorant(const Ray& ray);

    Spectrum Sample(const Ray& rWorld, Sampler& ff_sampler,
                    Sampler& trans_sampler, MemoryArena& arena,
                    MediumInteraction* mi);

    BlanchetCorrelatedResults Sample_Blanchet(
        const Ray& ray, Sampler& ff_sampler, Sampler& trans_sampler,
        MemoryArena& arena, MediumInteraction* mi, int blanchet_n);

#else
    Spectrum Tr(const Ray& ray, Sampler& sampler) const;

    BlanchetCorrelatedResults Tr_Blanchet(const Ray& ray, Sampler& sampler,
                                          int blanchet_n) const;

    Spectrum LocalTr(const Ray& ray, Sampler& sampler, Float tMin,
                     Float tMax) const;

    BlanchetCorrelatedResults LocalTr_Blanchet(const Ray& ray, Sampler& sampler,
                                               Float tMin, Float tMax,
                                               int blanchet_n) const;

    Spectrum Sample(const Ray& rWorld, Sampler& ff_sampler,
                    Sampler& trans_sampler, MemoryArena& arena,
                    MediumInteraction* mi) const;

    BlanchetCorrelatedResults Sample_Blanchet(
        const Ray& ray, Sampler& ff_sampler, Sampler& trans_sampler,
        MemoryArena& arena, MediumInteraction* mi, int blanchet_n) const;
#endif

    bool TransformToLocalSpace(const Ray& rWorld, Ray& ray, Float& tMin,
                               Float& tMax) const;

    virtual void RenderVerticalSlice(std::string filename, int ps) const {
        majorant->RenderVerticalSlice(filename, ps);
    }

    Float G() const { return g; }
    Spectrum ScatCoeff(const Point3f& pt) const {
        return density->D(pt) * sigma_s;
    }

    virtual void Update_Sigma(Spectrum new_sigma_a, Spectrum new_sigma_s) {
        sigma_a = new_sigma_a;
        sigma_s = new_sigma_s;
        sigma_t = sigma_a + sigma_s;

        t_est->sigma_t = sigma_t[0];
        ff_sampler->sigma_s = sigma_s[0];
        ff_sampler->sigma_t = sigma_t[0];
    }

    virtual Spectrum Sigma_S() const { return sigma_s; }
    virtual Spectrum Sigma_A() const { return sigma_a; }
    virtual Spectrum Sigma_T() const { return sigma_t; }

  private:
    DensityFunction* density;
    T_Estimator* t_est;
    MajorantFunction* majorant;
    MinorantFunction* minorant;
    FF_Sampler* ff_sampler;

    const Bounds3f mediumBounds;
    const Transform WorldToMedium;
    Spectrum sigma_a, sigma_s, sigma_t;
    const Float g;
};

};  // namespace pbrt

#endif  // PBRT_HETEROGENEOUS_MEDIA_H
