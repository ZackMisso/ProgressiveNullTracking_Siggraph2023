#pragma once

#include "pbrt.h"
#include "../majorant/majorant.h"
#include "../density/densityfunc.h"
#include "../trans_ests/t_est.h"
#include "../trans_funcs/trans_func.h"
#include "sampler.h"
#include "spectrum.h"
#include "phase.h"
#include "medium.h"

namespace pbrt
{

class FF_Sampler
{
public:
    #if RENDER_MUTABLE
    virtual Spectrum Sample (const Ray& ray,
                             const Ray& rWorld,
                             Sampler& sampler,
                             MemoryArena& arena,
                             MediumInteraction* mi,
                             Float tMin,
                             Float tMax,
                             bool& success)
    #else
    virtual Spectrum Sample (const Ray& ray,
                             const Ray& rWorld,
                             Sampler& sampler,
                             MemoryArena& arena,
                             MediumInteraction* mi,
                             Float tMin,
                             Float tMax,
                             bool& success) const
    #endif
    {
        #if RENDER_MUTABLE
        return Sample_MutableSigs(ray,
                                 rWorld,
                                 sampler,
                                 arena,
                                 mi,
                                 tMin,
                                 tMax,
                                 sigma_t,
                                 sigma_s,
                                 success);
        #else
        return Sample_ManualSigs(ray,
                                  rWorld,
                                  sampler,
                                  arena,
                                  mi,
                                  tMin,
                                  tMax,
                                  sigma_t,
                                  sigma_s,
                                  success);
        #endif
    }

    virtual Spectrum Sample_MutableSigs (const Ray& ray,
                                         const Ray& rWorld,
                                         Sampler& sampler,
                                         MemoryArena& arena,
                                         MediumInteraction* mi,
                                         Float tMin,
                                         Float tMax,
                                         Float sig_t,
                                         Float sig_s,
                                         bool& success)
    {
        return Sample_ManualSigs(ray,
                                 rWorld,
                                 sampler,
                                 arena,
                                 mi,
                                 tMin,
                                 tMax,
                                 sigma_t,
                                 sigma_s,
                                 success);
    }

    virtual FD_Cor_Sigma_T_Spectrum Sample_Cor
    (
        const Ray& ray,
        const Ray& rWorld,
        Sampler& sampler,
        MemoryArena& arena,
        MediumInteraction* mi,
        Float tMin,
        Float tMax,
        const FD_Cor_Sigma_T_Spectrum& spec,
        bool& success) const
    {
        std::cout << "this should not be called ff_sampler" << std::endl;
        FD_Cor_Sigma_T_Spectrum res = spec;
        res.setAll(Spectrum(1.f));
        return res;
    }

    virtual FD_DebCor_Sigma_T_Spectrum Sample_DebCor
    (
        const Ray& ray,
        const Ray& rWorld,
        Sampler& sampler,
        MemoryArena& arena,
        MediumInteraction* mi,
        Float tMin,
        Float tMax,
        const FD_DebCor_Sigma_T_Spectrum& spec,
        bool& success) const
    {
        std::cout << "this should not be called ff_sampler" << std::endl;
        FD_DebCor_Sigma_T_Spectrum res = spec;
        res.setAll(Spectrum(1.f));
        return res;
    }

    virtual Spectrum Sample_ManualSigs
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
    ) const = 0;

    virtual Spectrum Sample_ManualSigsDum
    (
        const Ray& ray,
        const Ray& rWorld,
        Sampler& sampler,
        int n,
        int typ,
        MemoryArena& arena,
        MediumInteraction* mi,
        Float tMin,
        Float tMax,
        Float sig_t,
        Float sig_s,
        bool& success
    ) const
    {
        return Sample_ManualSigs(ray, rWorld, sampler, arena,
                                 mi, tMin, tMax, sig_t, sig_s,
                                 success);
    }

    #if RENDER_MUTABLE
    virtual Spectrum Sample (const Ray& ray,
                             const Ray& rWorld,
                             Sampler& sampler,
                             MemoryArena& arena,
                             MediumInteraction* mi,
                             Float tMin,
                             Float tMax,
                             bool& success,
                             uint32_t flags,
                             TransportMode mode)
    #else
    virtual Spectrum Sample (const Ray& ray,
                             const Ray& rWorld,
                             Sampler& sampler,
                             MemoryArena& arena,
                             MediumInteraction* mi,
                             Float tMin,
                             Float tMax,
                             bool& success,
                             uint32_t flags,
                             TransportMode mode) const
    #endif
    {
        return Sample(ray,
                      rWorld,
                      sampler,
                      arena,
                      mi,
                      tMin,
                      tMax,
                      success);
    }

    #if RENDER_MUTABLE
    virtual Spectrum Sample_Beta (const Ray& ray,
                             const Ray& rWorld,
                             Sampler& sampler,
                             MemoryArena& arena,
                             MediumInteraction* mi,
                             Float tMin,
                             Float tMax,
                             Float beta, Float c,
                             bool& success,
                             uint32_t flags,
                             TransportMode mode)
    #else
    virtual Spectrum Sample_Beta (const Ray& ray,
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
    #endif
    {
        return Sample(ray,
                      rWorld,
                      sampler,
                      arena,
                      mi,
                      tMin,
                      tMax,
                      success);
    }

    #if RENDER_MUTABLE
    virtual BlanchetCorrelatedResults Sample_Blanchet
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
    )
    #else
    virtual BlanchetCorrelatedResults Sample_Blanchet
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
    #endif
    {
        return BlanchetCorrelatedResults(Sample(ray,
                                                rWorld,
                                                sampler,
                                                arena,
                                                mi,
                                                tMin,
                                                tMax,
                                                success));
    }

    #if RENDER_MUTABLE
    virtual BlanchetCorrelatedResults Sample_Blanchet_Beta
    (
        const Ray& ray,
        const Ray& rWorld,
        Sampler& sampler,
        Float beta,
        Float c,
        MemoryArena& arena,
        MediumInteraction* mi,
        Float tMin,
        Float tMax,
        bool& success,
        int blanchet_n,
        uint32_t flags,
        TransportMode mode
    )
    #else
    virtual BlanchetCorrelatedResults Sample_Blanchet_Beta
    (
        const Ray& ray,
        const Ray& rWorld,
        Sampler& sampler,
        Float beta,
        Float c,
        MemoryArena& arena,
        MediumInteraction* mi,
        Float tMin,
        Float tMax,
        bool& success,
        int blanchet_n,
        uint32_t flags,
        TransportMode mode
    ) const
    #endif
    {
        return BlanchetCorrelatedResults(Sample(ray,
                                                rWorld,
                                                sampler,
                                                arena,
                                                mi,
                                                tMin,
                                                tMax,
                                                success));
    }

    #if RENDER_MUTABLE
    virtual BlanchetCorrelatedFdDebiasing Sample_Blanchet_FdBeta
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
    )
    #else
    virtual BlanchetCorrelatedFdDebiasing Sample_Blanchet_FdBeta
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
    #endif
    {
        std::cout << "Sample Blanchet Beta not implemented - sampler" << std::endl;
        return BlanchetCorrelatedFdDebiasing(Sample(ray,
                                                rWorld,
                                                sampler,
                                                arena,
                                                mi,
                                                tMin,
                                                tMax,
                                                success));
    }

    #if RENDER_MUTABLE
    virtual BlanchetCorrelatedResults Sample_Blanchet(const Ray& ray,
                                                      const Ray& rWorld,
                                                      Sampler& sampler,
                                                      MemoryArena& arena,
                                                      MediumInteraction* mi,
                                                      Float tMin,
                                                      Float tMax,
                                                      bool& success,
                                                      int blanchet_n)
    #else
    virtual BlanchetCorrelatedResults Sample_Blanchet(const Ray& ray,
                                                      const Ray& rWorld,
                                                      Sampler& sampler,
                                                      MemoryArena& arena,
                                                      MediumInteraction* mi,
                                                      Float tMin,
                                                      Float tMax,
                                                      bool& success,
                                                      int blanchet_n) const
    #endif
    {
        // bool success = false;
        return BlanchetCorrelatedResults(Sample(ray,
                                                rWorld,
                                                sampler,
                                                arena,
                                                mi,
                                                tMin,
                                                tMax,
                                                success));
    }

    #if RENDER_MUTABLE
    virtual BlanchetDifferentialResults Sample_Diff(const Ray& ray,
                                                    const Ray& rWorld,
                                                    Sampler& sampler,
                                                    MemoryArena& arena,
                                                    MediumInteraction* mi,
                                                    Float tMin,
                                                    Float tMax,
                                                    bool& success,
                                                    Float continuous_n)
    #else
    virtual BlanchetDifferentialResults Sample_Diff(const Ray& ray,
                                                    const Ray& rWorld,
                                                    Sampler& sampler,
                                                    MemoryArena& arena,
                                                    MediumInteraction* mi,
                                                    Float tMin,
                                                    Float tMax,
                                                    bool& success,
                                                    Float continuous_n) const
    #endif
    {
        // bool success = false;
        return BlanchetDifferentialResults(Sample(ray,
                                                  rWorld,
                                                  sampler,
                                                  arena,
                                                  mi,
                                                  tMin,
                                                  tMax,
                                                  success));
    }

    #if RENDER_MUTABLE
    virtual Spectrum Sample (const Point3f& light_p,
                             const Ray& ray,
                             const Ray& rWorld,
                             Sampler& sampler,
                             MemoryArena& arena,
                             MediumInteraction* mi,
                             Float tMin,
                             Float tMax)
    #else
    virtual Spectrum Sample (const Point3f& light_p,
                             const Ray& ray,
                             const Ray& rWorld,
                             Sampler& sampler,
                             MemoryArena& arena,
                             MediumInteraction* mi,
                             Float tMin,
                             Float tMax) const
    #endif
    {
        bool success = false;
        return Sample(ray,
                      rWorld,
                      sampler,
                      arena,
                      mi,
                      tMin,
                      tMax,
                      success);
    }

    virtual bool RequiresTr() const = 0;
    virtual bool RequiresLightFirst() const = 0;
    virtual bool RequiresTrOnScatterOnly() const = 0;

    virtual bool supportsNonExponential() const { return false; }

    Medium* mediumRef;
    MajorantFunction* majorant;
    DensityFunction* density;
    Transmittance_Func* trans_func;
    Float sigma_s;
    Float sigma_t;
    Float g;
};

}
