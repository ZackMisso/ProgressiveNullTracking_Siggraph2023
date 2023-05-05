#pragma once

#include "t_est.h"

namespace pbrt
{

class Pseries_Cumulative : public T_Estimator
{
public:
    virtual Spectrum Tr_ManualSig(const Ray& ray,
                                  Sampler& sampler,
                                  Float tMin,
                                  Float tMax,
                                  Float maxD,
                                  Float sig_t) const;
};

}