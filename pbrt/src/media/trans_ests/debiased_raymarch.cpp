#include "debiased_raymarch.h"

#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

// manual sig is biased, mutable sig is unbiased currently
Spectrum DebiasedRaymarch::Tr_ManualSig(const Ray& ray, Sampler& sampler,
                                        Float tMin, Float tMax, Float maxD,
                                        Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    // Float step = base_step_size * maxD / sigma_t;
    Float step = 1.0 / (maxD * sig_t);

    Spectrum od = Spectrum(0.0);
    Float t = tMin;

    while (t < tMax) {
        step = std::min(step, tMax - t);

        Float loc = t + sampler.Get1D() * step;
        od += density->D(ray(loc)) * step * sig_t;
        t += step;
    }

    // this is biased
    return Exp(-od);
}

// this is monte carlo + ray marching
Spectrum DebiasedRaymarch::Tr_MutableSig(const Ray& ray, Sampler& sampler,
                                         Float tMin, Float tMax, Float& maxD,
                                         Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    // do biased 1-sample evaluatuation //
    BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

    Float blanch_sample = sampler.Get1D();
    Float r = 0.65;
    Float blanchet_pmf = r;
    Float cmf = blanchet_pmf;
    int blanchet_n = 0;

    while (cmf < blanch_sample) {
        ++blanchet_n;
        blanchet_pmf *= 1.0 - r;
        cmf += blanchet_pmf;
    }

    // canonical test show k = 2 is the best choice
    // in terms of estimating only transmittance for
    // debiased raymarching, but is too costly for
    // scattering media so we use k = 1.

    Float base_step = sig_t / (2.0 * maxD);
    int p1s = pow(2, blanchet_n + 1);
    int ps = pow(2, blanchet_n);
    Float inv_p1s = 1.0 / Float(p1s);
    Float inv_ps = 1.0 / Float(ps);

    Float t = tMin;

    Float rand_val = sampler.Get1D();
    int flip_val = (sampler.Get1D() < 0.5) ? 0 : 1;
    std::vector<Spectrum> ods = std::vector<Spectrum>(p1s, Spectrum(0.0));
    while (t < tMax) {
        Float step = base_step;
        step = std::min(step, tMax - t);

        for (int i = 0; i < p1s; ++i) {
            Float loc = t + (Float(i) + rand_val) * inv_p1s * step;

            // todo: this currently assumes monochrome media
            Spectrum od_contr = density->D(ray(loc)) * step * sig_t;

            ods[i] += od_contr;
            if (i % 2 == flip_val) {
                od.odd += od_contr * inv_ps;
            } else {
                od.even += od_contr * inv_ps;
            }
            od.all += od_contr * inv_p1s;
        }

        t += step;
    }

    for (int i = 0; i < ods.size(); ++i) {
        Spectrum eval = Exp(-ods[i]);
        od.bias = od.bias + 1.0 / Float(i + 1) * (eval - od.bias);
    }

    od.all = Exp(-od.all);
    od.odd = Exp(-od.odd);
    od.even = Exp(-od.even);

    return (od.all - 0.5 * (od.even + od.odd)) / blanchet_pmf + od.bias;
}

}  // namespace pbrt