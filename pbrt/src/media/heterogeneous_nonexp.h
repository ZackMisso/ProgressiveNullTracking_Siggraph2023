// #pragma once

// TODO: fully convert this later maybe

// #include "pbrt.h"
// #include "medium.h"
// #include "transform.h"
// #include "stats.h"
// #include "trans_ests/t_est.h"
// #include "minorant.h"
// #include "majorant/majorant.h"
// #include "ff_samplers/ff_sampler.h"
// #include "trans_funcs/trans_func.h"

// namespace pbrt
// {

// class HeterogeneousNonExpMedium : public Medium
// {
// public:
//     HeterogeneousNonExpMedium
//     (
//         const Spectrum& sigma_a,
//         const Spectrum& sigma_s,
//         Float g,
//         T_Estimator* t_est,
//         DensityFunction* density,
//         MinorantFunction* minorant,
//         MajorantFunction* majorant,
//         Transmittance_Func* trans_func,
//         FF_Sampler* ff,
//         const Transform& WorldToMedium,
//         const Bounds3f b
//     );

//     ~HeterogeneousNonExpMedium();

//     // temporary clean later
//     void setMajScale(Float scale)
//     {
//         majorant->setScale(scale);
//     }

//     Spectrum Tr
//     (
//         const Ray& ray,
//         Sampler& sampler
//     ) const
//     {
//         std::cout << "this should not be used" << std::endl;
//         std::cout << "BOOP" << std::endl;
//         throw new std::exception();
//     }

//     virtual Spectrum Tr
//     (
//         const Ray &ray,
//         Sampler &sampler,
//         uint32_t flags,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     virtual Spectrum Tr_Beta
//     (
//         const Ray &ray,
//         Sampler &sampler,
//         Float beta,
//         Float c,
//         uint32_t flags,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     virtual BlanchetCorrelatedResults Tr_Blanchet
//     (
//         const Ray &ray,
//         Sampler& sampler,
//         uint32_t flags,
//         int blanchet_n,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     virtual BlanchetCorrelatedResults Tr_Blanchet_Beta
//     (
//         const Ray &ray,
//         Sampler& sampler,
//         uint32_t flags,
//         Float beta, Float c,
//         int blanchet_n,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     virtual BlanchetCorrelatedFdDebiasing Tr_Blanchet_FdBeta
//     (
//         const Ray &ray,
//         Sampler& sampler,
//         int blanchet_n,
//         int davis_n,
//         Float base_h,
//         Float davis_beta,
//         Float davis_c,
//         uint32_t flags,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     Spectrum LocalTr (const Ray& ray,
//                       Sampler& sampler,
//                       Float tMin,
//                       Float tMax) const
//     {
//         std::cout << "this should not be used" << std::endl;
//         std::cout << "BOOP" << std::endl;
//         throw new std::exception();
//     }

//     virtual Spectrum LocalTr_Beta
//     (
//         const Ray &ray,
//         Sampler& sampler,
//         Float beta,
//         Float c,
//         Float tMin,
//         Float tMax,
//         uint32_t flags,
//         TransportMode mode
//     ) const;

//     virtual Spectrum LocalTr
//     (
//         const Ray &ray,
//         Sampler& sampler,
//         Float tMin,
//         Float tMax,
//         uint32_t flags,
//         TransportMode mode
//     ) const;

//     virtual BlanchetCorrelatedResults LocalTr_Blanchet
//     (
//         const Ray &ray,
//         Sampler& sampler,
//         Float tMin,
//         Float tMax,
//         int blanchet_n,
//         uint32_t flags,
//         TransportMode mode
//     ) const;

//     virtual BlanchetCorrelatedResults LocalTr_Blanchet_Beta
//     (
//         const Ray &ray,
//         Sampler& sampler,
//         Float beta,
//         Float c,
//         Float tMin,
//         Float tMax,
//         int blanchet_n,
//         uint32_t flags,
//         TransportMode mode
//     ) const;

//     virtual BlanchetCorrelatedFdDebiasing LocalTr_Blanchet_FdBeta
//     (
//         const Ray &ray,
//         Sampler& sampler,
//         Float tMin,
//         Float tMax,
//         int blanchet_n,
//         int davis_n,
//         Float base_h,
//         Float davis_beta,
//         Float davis_c,
//         uint32_t flags,
//         TransportMode mode
//     ) const;

//     Spectrum Sample
//     (
//         const Ray& rWorld,
//         Sampler& ff_sampler,
//         Sampler& trans_sampler,
//         MemoryArena& arena,
//         MediumInteraction* mi
//     ) const
//     {
//         std::cout << "this should not be used" << std::endl;
//         throw new std::exception();
//     }

//     virtual Spectrum Sample
//     (
//         const Ray &ray,
//         Sampler &ff_sampler,
//         Sampler &trans_sampler,
//         MemoryArena &arena,
//         MediumInteraction *mi,
//         uint32_t flags,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     virtual Spectrum Sample_Beta
//     (
//         const Ray &ray,
//         Sampler &ff_sampler,
//         Sampler &trans_sampler,
//         Float beta,
//         Float c,
//         MemoryArena &arena,
//         MediumInteraction *mi,
//         uint32_t flags,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     virtual BlanchetCorrelatedResults Sample_Blanchet
//     (
//         const Ray &ray,
//         Sampler &ff_sampler,
//         Sampler &trans_sampler,
//         MemoryArena &arena,
//         MediumInteraction *mi,
//         uint32_t flags,
//         int blanchet_n,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     virtual BlanchetCorrelatedResults Sample_Blanchet_Beta
//     (
//         const Ray &ray,
//         Sampler &ff_sampler,
//         Sampler &trans_sampler,
//         Float beta, Float c,
//         MemoryArena &arena,
//         MediumInteraction *mi,
//         uint32_t flags,
//         int blanchet_n,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     virtual BlanchetCorrelatedFdDebiasing Sample_Blanchet_FdBeta
//     (
//         const Ray &ray,
//         Sampler &ff_sampler,
//         Sampler &trans_sampler,
//         MemoryArena &arena,
//         MediumInteraction *mi,
//         uint32_t flags,
//         int blanchet_n,
//         int davis_n,
//         Float base_h,
//         Float davis_beta,
//         Float davis_c,
//         TransportMode mode = TransportMode::Radiance
//     ) const;

//     bool TransformToLocalSpace (const Ray& rWorld,
//                                 Ray& ray,
//                                 Float& tMin,
//                                 Float& tMax) const;

//     virtual void Update_Sigma(Spectrum new_sigma_a, Spectrum new_sigma_s)
//     {
//         sigma_a = new_sigma_a;
//         sigma_s = new_sigma_s;
//         sigma_t = sigma_a + sigma_s;

//         t_est->sigma_t = sigma_t[0];
//         ff_sampler->sigma_s = sigma_s[0];
//         ff_sampler->sigma_t = sigma_t[0];
//     }

//     Float G() const { return g; }
//     Spectrum ScatCoeff(const Point3f& pt) const { return density->D(pt) *
//     sigma_s; }

//     // static long nTrCalls;
//     // static long nDensityCalls;

// private:
//     DensityFunction* density;
//     T_Estimator* t_est;
//     MajorantFunction* majorant;
//     MinorantFunction* minorant;
//     Transmittance_Func* trans_func;
//     FF_Sampler* ff_sampler;

//     const Bounds3f mediumBounds;
//     const Transform WorldToMedium;
//     Spectrum sigma_a, sigma_s, sigma_t;
//     const Float g;
// };

// };
