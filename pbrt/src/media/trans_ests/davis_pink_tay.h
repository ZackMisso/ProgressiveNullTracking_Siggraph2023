#pragma once

#include "t_est.h"

namespace pbrt
{

class Davis_Pink_Taylor : public T_Estimator
{
public:
    Davis_Pink_Taylor(Float heckinfeckin) : T_Estimator(), heckinfeckin(heckinfeckin) { }

    Float compute_alpha(Float tau, Float c) const;
    Float model(Float tau, Float c) const;
    Float bk_expected_order(Float c, Float p_z) const;
    int ncmf(Float maj) const;
    int determine_tuple_size(Float maj, Float p_z, Float c) const;

    void aggressive_russian_roulette
    (
        Sampler& sampler,
        Float p_z,
        std::vector<Float>& weights,
        Float davis_c,
        int& k
    ) const;

    void pivot_array
    (
        const std::vector<Float>& samples,
        std::vector<Float>& pivoted,
        int j
    ) const;

    void elementry_means
    (
        const std::vector<Float>& samples,
        std::vector<Float>& means
    ) const;

    virtual Spectrum Tr_ManualSig
    (
        const Ray& ray,
        Sampler& sampler,
        Float tMin,
        Float tMax,
        Float maxD,
        Float sig_t
    ) const;

    virtual Spectrum Tr
    (
        const Ray& ray,
        Sampler& sampler,
        Float tMin,
        Float tMax,
        bool startOnSurface,
        bool endOnSurface,
        bool adjoint,
        Float maxD
    ) const;

    // virtual Spectrum Tr_Beta(const Ray& ray,
    //                     Sampler& sampler,
    //                     Float tMin,
    //                     Float tMax,
    //                     Float beta,
    //                     Float c,
    //                     bool startOnSurface,
    //                     bool endOnSurface,
    //                     bool adjoint,
    //                     Float maxD) const;

    virtual bool supportsNonExponential() const { return true; }

    Float heckinfeckin;
};

}
