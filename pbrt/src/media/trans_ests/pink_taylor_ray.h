#pragma once

#include "t_est.h"

namespace pbrt {

class PinkTaylorRay : public T_Estimator {
  public:
    PinkTaylorRay() : maj(1.0) {}
    PinkTaylorRay(Float maj) : maj(maj) {}

    virtual Spectrum Tr_ManualSig(const Ray &ray, Sampler &sampler, Float tMin,
                                  Float tMax, Float maxD, Float sig_t) const;

    virtual Spectrum Tr(const Ray& ray, Sampler& sampler, Float tMin,
                        Float tMax, bool startOnSurface, bool endOnSurface,
                        bool adjoint, Float maxD) const;

    Float maj;

    virtual bool supportsNonExponential() const { return true; }

  protected:
    Spectrum pink(Spectrum tau, Float prod_i, Float C) const;
    int determine_tuple_size(Float maj, Float p_z, Float c) const;
    int ncmf(Float maj) const;
    Float expected_order(Float c, Float p_z) const;
    void aggressive_russian_roulette(Sampler &sampler, Float p_z,
                                     std::vector<Float> &weights, int &k) const;
    void aggressive_russian_roulette_pink(Sampler &sampler, Float p_z, Float prod_i,
                                     std::vector<Float> &weights, int &k) const;
    void pivot_array(const std::vector<Spectrum> &samples,
                     std::vector<Spectrum> &pivoted, Float maj, int j) const;
    void elementry_means(const std::vector<Spectrum> &samples,
                         std::vector<Spectrum> &means) const;
};

}  // namespace pbrt
