#pragma once

#include "t_est.h"

namespace pbrt {

class ExpNvidia : public T_Estimator {
  public:
    ExpNvidia() : maj(1.0) {}
    ExpNvidia(Float maj) : maj(maj) {}

    virtual Spectrum Tr_MutableSig(const Ray &ray, Sampler &sampler, Float tMin,
                                   Float tMax, Float &maxD, Float sig_t) const;

    virtual Spectrum Tr_ManualSig(const Ray &ray, Sampler &sampler, Float tMin,
                                  Float tMax, Float maxD, Float sig_t) const;

    Float maj;

    // this is now deprecated
    virtual bool is_null_track() const { return false; }

  protected:
    int determine_tuple_size(Float maj, Float p_z, Float c) const;
    int ncmf(Float maj) const;
    void aggressive_russian_roulette(Sampler &sampler, Float p_z,
                                     std::vector<Float> &weights, int &k) const;
    void pivot_array(const std::vector<Spectrum> &samples,
                     std::vector<Spectrum> &pivoted, Float maj, int j) const;
    void elementry_means(const std::vector<Spectrum> &samples,
                         std::vector<Spectrum> &means) const;
    Float expected_order(Float c, Float p_z) const;
};

}  // namespace pbrt
