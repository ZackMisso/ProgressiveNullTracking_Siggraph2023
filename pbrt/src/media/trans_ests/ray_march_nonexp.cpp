#include "ray_march_nonexp.h"
#include "spectrum.h"
#include "sampler.h"

// TODO: remove this class, or rename it to biased raymarching

namespace pbrt
{

Spectrum RayMarchNonExpTrans::Tr_ManualSig(const Ray& ray,
                                           Sampler& sampler,
                                           Float tMin,
                                           Float tMax,
                                           Float maxD,
                                           Float sig_t) const
{
    sampler.BeforeTransmittanceSample();

    Float step = base_step_size / sig_t;

    Spectrum od = Spectrum(0.0);
    Float t = tMin;

    while (t < tMax)
    {
        step = std::min(step, tMax - t);

        Float loc = t + sampler.Get1D() * step;
        od += density->D(ray(loc)) * step * sig_t;
        t += step;
    }

    // std::cout << "here" << std::endl;

    // this is biased
    return Exp(-od);
}

Spectrum RayMarchNonExpTrans::Tr(const Ray& ray,
                                 Sampler& sampler,
                                 Float tMin,
                                 Float tMax,
                                 bool startOnSurface,
                                 bool endOnSurface,
                                 bool adjoint,
                                 Float maxD) const
{
    sampler.BeforeTransmittanceSample();

    Float step = base_step_size / sigma_t;

    Spectrum od = Spectrum(0.0);
    Float t = tMin;

    while (t < tMax)
    {
        step = std::min(step, tMax - t);

        Float loc = t + sampler.Get1D() * step;
        od += density->D(ray(loc)) * step * sigma_t;
        t += step;
    }

    std::cout << "here" << std::endl;

    return trans_func->eval(od, startOnSurface, endOnSurface, adjoint);
}

}
