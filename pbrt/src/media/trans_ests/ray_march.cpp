#include "ray_march.h"

#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

// FD_Cor_Sigma_T_Spectrum Spectrum RayMarchTrans::Tr_Cor
// (
//     const Ray& ray,
//     Sampler& sampler,
//     Float tMin,
//     Float tMax,
//     Float maxD,
//     const FD_Cor_Sigma_T_Spectrum& spec
// ) const
// {
//     FD_Cor_Sigma_T_Spectrum tr = spec;
//     tr.setAll(Spectrum(0.f));
//
//     Spectrum od = Spectrum(0.0);
//     Float t = tMin;
//
//     while (t < tMax)
//     {
//         step = std::min(step, tMax - t);
//
//         Float loc = t + sampler.Get1D() * step;
//         od += density->D(ray(loc)) * step * sig_t;
//         t += step;
//     }
//
//     // this is biased
//     return Exp(-od);
// }

Spectrum RayMarchTrans::Tr_ManualSig(const Ray& ray, Sampler& sampler,
                                     Float tMin, Float tMax, Float maxD,
                                     Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    // Float step = base_step_size * maxD / sigma_t;
    Float step = base_step_size / sig_t;

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

// // this is actual ray-marching
// // TODO: delete this
// BlanchetCorrelatedResults RayMarchTrans::Tr_Blanchet_Macro(
//     const Ray& ray, Sampler& sampler, Float tMin, Float tMax, Float maxD,
//     int blanchet_n) const {
//     sampler.BeforeTransmittanceSample();

//     // std::cout << "in here" << std::endl;

//     Point3f a = ray(tMin);
//     Point3f b = ray(tMax);

//     std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a,
//     b);

//     MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

//     // do biased 1-sample evaluatuation //
//     BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

//     // Float base_step = base_step_size * maxD / sigma_t;
//     Float base_step = base_step_size / sigma_t;

//     Float dist_samp = sampler.Get1D();
//     bool flip = sampler.Get1D() > 0.5;

//     Float t = path.tMin;
//     while (t < path.tMax) {
//         Float step = std::min(base_step, path.tMax - t);

//         Float loc = t + dist_samp * step;
//         od.bias += density->D(ray(path.unmap(loc))) * step * sigma_t;
//         t += step;
//     }
//     //////////////////////////////////////

//     // do correlated bias n+1-sample - avg n-sample evaluation //
//     base_step /= pow(2.0, Float(blanchet_n + 1));
//     t = path.tMin;
//     while (t < path.tMax) {
//         Float step = base_step;
//         Float prev_t = t;

//         if (2.0 * base_step > path.tMax - t) {
//             // TODO - resize basestep
//             step = (path.tMax - t) / 2.0;
//         }

//         // only the last step will be smaller, and we want to make sure that
//         // t > tMax after the last step, so by increasing t by base_step
//         instead
//         // of step this is garaunteed despite any floating point
//         inaccuracies. t += 2.0 * base_step;

//         // do two steps at a time
//         Float left_loc = prev_t + dist_samp * step;
//         Float right_loc = (prev_t + step) + dist_samp * step;

//         Spectrum left_od_part = density->D(ray(path.unmap(left_loc))) *
//         sigma_t; Spectrum right_od_part =
//             density->D(ray(path.unmap(right_loc))) * sigma_t;

//         if (flip) {
//             od.odd += left_od_part * 2.0 * step;
//             od.even += right_od_part * 2.0 * step;
//         } else {
//             od.even += left_od_part * 2.0 * step;
//             od.odd += right_od_part * 2.0 * step;
//         }

//         od.all += left_od_part * step;
//         od.all += right_od_part * step;
//     }
//     /////////////////////////////////////////////////////////////

//     od.exponentiate();

//     return od;
// }

// TODO: make unbiased method
Spectrum RayMarchTrans::Tr(const Ray& ray, Sampler& sampler, Float tMin,
                           Float tMax, bool startOnSurface, bool endOnSurface,
                           bool adjoint, Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Float step = base_step_size / sigma_t;

    Spectrum od = Spectrum(0.0);
    Float t = tMin;

    std::cout << "ray march" << std::endl;

    while (t < tMax) {
        step = std::min(step, tMax - t);

        Float loc = t + sampler.Get1D() * step;
        od += density->D(ray(loc)) * step * sigma_t;
        t += step;
    }

    // std::cout << "od: " << od << std::endl;
    // std::cout << "tr: " << trans_func->eval(od, startOnSurface, endOnSurface,
    // adjoint) << std::endl;

    return trans_func->eval(od, startOnSurface, endOnSurface, adjoint);
}

// this is monte carlo + ray marching
BlanchetCorrelatedResults RayMarchTrans::Tr_Blanchet(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
    bool startOnSurface, bool endOnSurface, bool adjoint, int blanchet_n,
    Float maxD) const {
    sampler.BeforeTransmittanceSample();

    // std::cout << "what the fuck" << std::endl;

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    // density->clearDensityCalls();

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMax <= path.tMin) return BlanchetCorrelatedResults(1.0);

    // std::cout << "dense calls: " << density->getDensityCalls() << std::endl;

    // do biased 1-sample evaluatuation //
    BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

    Float base_step = base_step_size / 2.0;  // k = 1
    int p1s = pow(2, blanchet_n + 1);
    int ps = pow(2, blanchet_n);
    Float inv_p1s = 1.0 / Float(p1s);
    Float inv_ps = 1.0 / Float(ps);

    Float t = path.tMin;

    // TODO: can decrease cost substantially by choosing to not evaluate bias
    // an attempt at an adaptive method
    // if (path.tMax - path.tMin < 1.0)
    //   base_step = path.tMax - path.tMin;
    // else
    //   base_step = (path.tMax - path.tMin) / 8.0;

    Float rand_val = sampler.Get1D();
    int flip_val = (sampler.Get1D() < 0.5) ? 0 : 1;
    std::vector<Spectrum> ods = std::vector<Spectrum>(p1s, Spectrum(0.0));
    while (t < path.tMax) {
        Float step = base_step;
        step = std::min(step, path.tMax - t);

        for (int i = 0; i < p1s; ++i) {
            Float loc = t + (Float(i) + rand_val) * inv_p1s * step;

            Spectrum od_contr =
                density->D(ray(path.unmap(loc))) * step * sigma_t;

            ods[i] += od_contr;
            if (i % 2 == flip_val) {
                od.odd += od_contr * inv_ps;
            } else {
                od.even += od_contr * inv_ps;
            }
            od.all += od_contr * inv_p1s;

            // Float loc = t + sampler.Get1D() * step;
            //
            // if (loc < path.tMax) {
            //     Spectrum od_contr =
            //         density->D(ray(path.unmap(loc))) * step * sigma_t;
            //
            //     ods[i] += od_contr;
            //     if (i < ps) {
            //         od.odd += od_contr * inv_ps;
            //     } else {
            //         od.even += od_contr * inv_ps;
            //     }
            //
            //     od.all += od_contr * inv_p1s;
            // }
            // else
            // {
            //     std::cout << "WHAT THE HECK" <<  std::endl;
            // }
        }

        t += step;
    }

    // std::cout << "dense calls: " << density->getDensityCalls() << std::endl;

    for (int i = 0; i < ods.size(); ++i) {
        Spectrum eval =
            trans_func->eval(ods[i], startOnSurface, endOnSurface, adjoint);
        od.bias = od.bias + 1.0 / Float(i + 1) * (eval - od.bias);
    }

    od.all = trans_func->eval(od.all, startOnSurface, endOnSurface, adjoint);
    od.odd = trans_func->eval(od.odd, startOnSurface, endOnSurface, adjoint);
    od.even = trans_func->eval(od.even, startOnSurface, endOnSurface, adjoint);

    // return BlanchetCorrelatedResults(p1s);

    return od;
}

BlanchetCorrelatedResults RayMarchTrans::Tr_Blanchet_Beta(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax, Float beta,
    Float c, bool startOnSurface, bool endOnSurface, bool adjoint,
    int blanchet_n, Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMax <= path.tMin) return BlanchetCorrelatedResults(1.0);

    // do biased 1-sample evaluatuation //
    BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

    Float base_step = base_step_size / sigma_t;

    Float t = path.tMin;
    // Float samp = sampler.Get1D();

    // int base_n = 4;
    while (t < path.tMax) {
        Float step = base_step;
        step = std::min(0.1, (path.tMax - path.tMin) / 10.0);
        step = std::min(step, path.tMax - t);

        for (int i = 0; i < pow(2, blanchet_n + 1); ++i) {
            Float loc = t + sampler.Get1D() * step;

            if (loc < path.tMax) {
                Spectrum od_contr =
                    density->D(ray(path.unmap(loc))) * step * sigma_t;

                if (i == 0) od.bias += od_contr;
                if (i < pow(2, blanchet_n)) {
                    od.odd += od_contr / Float(pow(2, blanchet_n));
                } else {
                    od.even += od_contr / Float(pow(2, blanchet_n));
                }

                od.all += od_contr / Float(pow(2, blanchet_n + 1));
            }
        }

        t += step;
    }

    od.bias = trans_func->eval(od.bias, beta, c, startOnSurface, endOnSurface,
                               adjoint);
    od.all = trans_func->eval(od.all, beta, c, startOnSurface, endOnSurface,
                              adjoint);
    od.odd = trans_func->eval(od.odd, beta, c, startOnSurface, endOnSurface,
                              adjoint);
    od.even = trans_func->eval(od.even, beta, c, startOnSurface, endOnSurface,
                               adjoint);

    return od;
}

BlanchetCorrelatedFdDebiasing RayMarchTrans::Tr_Blanchet_FdBeta(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
    bool startOnSurface, bool endOnSurface, bool adjoint, int blanchet_n,
    int davis_n, Float base_h, Float davis_beta, Float davis_c,
    Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMax <= path.tMin) return BlanchetCorrelatedFdDebiasing(1.0);

    // do biased 1-sample evaluatuation //
    BlanchetCorrelatedFdDebiasing od = BlanchetCorrelatedFdDebiasing(0.0);

    Float base_step = base_step_size / sigma_t;

    Float t = path.tMin;

    Spectrum all = Spectrum(0.f);
    Spectrum even = Spectrum(0.f);
    Spectrum odd = Spectrum(0.f);
    Spectrum bias = Spectrum(0.f);

    while (t < path.tMax) {
        Float step = base_step;
        step = std::min(0.1, (path.tMax - path.tMin) / 10.0);
        step = std::min(step, path.tMax - t);

        for (int i = 0; i < pow(2, blanchet_n + 1); ++i) {
            Float loc = t + sampler.Get1D() * step;

            if (loc < path.tMax) {
                Spectrum od_contr =
                    density->D(ray(path.unmap(loc))) * step * sigma_t;

                if (i == 0) bias += od_contr;
                if (i < pow(2, blanchet_n)) {
                    odd += od_contr / Float(pow(2, blanchet_n));
                } else {
                    even += od_contr / Float(pow(2, blanchet_n));
                }

                all += od_contr / Float(pow(2, blanchet_n + 1));
            }
        }

        t += step;
    }

    Float h1 = base_h * pow(2.0, -Float(davis_n));
    Float h2 = base_h * pow(2.0, -Float(davis_n + 1));

    // null
    od.bias[0] = trans_func->eval(bias, davis_beta, davis_c, startOnSurface,
                                  endOnSurface, adjoint);
    od.even[0] = trans_func->eval(even, davis_beta, davis_c, startOnSurface,
                                  endOnSurface, adjoint);
    od.odd[0] = trans_func->eval(odd, davis_beta, davis_c, startOnSurface,
                                 endOnSurface, adjoint);
    od.all[0] = trans_func->eval(all, davis_beta, davis_c, startOnSurface,
                                 endOnSurface, adjoint);

    // h
    od.bias[1] = trans_func->eval(bias, davis_beta, davis_c + base_h,
                                  startOnSurface, endOnSurface, adjoint);
    od.even[1] = trans_func->eval(even, davis_beta, davis_c + base_h,
                                  startOnSurface, endOnSurface, adjoint);
    od.odd[1] = trans_func->eval(odd, davis_beta, davis_c + base_h,
                                 startOnSurface, endOnSurface, adjoint);
    od.all[1] = trans_func->eval(all, davis_beta, davis_c + base_h,
                                 startOnSurface, endOnSurface, adjoint);

    od.bias[2] = trans_func->eval(bias, davis_beta, davis_c - base_h,
                                  startOnSurface, endOnSurface, adjoint);
    od.even[2] = trans_func->eval(even, davis_beta, davis_c - base_h,
                                  startOnSurface, endOnSurface, adjoint);
    od.odd[2] = trans_func->eval(odd, davis_beta, davis_c - base_h,
                                 startOnSurface, endOnSurface, adjoint);
    od.all[2] = trans_func->eval(all, davis_beta, davis_c - base_h,
                                 startOnSurface, endOnSurface, adjoint);

    // h1
    od.bias[3] = trans_func->eval(bias, davis_beta, davis_c + h1,
                                  startOnSurface, endOnSurface, adjoint);
    od.even[3] = trans_func->eval(even, davis_beta, davis_c + h1,
                                  startOnSurface, endOnSurface, adjoint);
    od.odd[3] = trans_func->eval(odd, davis_beta, davis_c + h1, startOnSurface,
                                 endOnSurface, adjoint);
    od.all[3] = trans_func->eval(all, davis_beta, davis_c + h1, startOnSurface,
                                 endOnSurface, adjoint);

    od.bias[4] = trans_func->eval(bias, davis_beta, davis_c - h1,
                                  startOnSurface, endOnSurface, adjoint);
    od.even[4] = trans_func->eval(even, davis_beta, davis_c - h1,
                                  startOnSurface, endOnSurface, adjoint);
    od.odd[4] = trans_func->eval(odd, davis_beta, davis_c - h1, startOnSurface,
                                 endOnSurface, adjoint);
    od.all[4] = trans_func->eval(all, davis_beta, davis_c - h1, startOnSurface,
                                 endOnSurface, adjoint);

    // h2
    od.bias[5] = trans_func->eval(bias, davis_beta, davis_c + h2,
                                  startOnSurface, endOnSurface, adjoint);
    od.even[5] = trans_func->eval(even, davis_beta, davis_c + h2,
                                  startOnSurface, endOnSurface, adjoint);
    od.odd[5] = trans_func->eval(odd, davis_beta, davis_c + h2, startOnSurface,
                                 endOnSurface, adjoint);
    od.all[5] = trans_func->eval(all, davis_beta, davis_c + h2, startOnSurface,
                                 endOnSurface, adjoint);

    od.bias[6] = trans_func->eval(bias, davis_beta, davis_c - h2,
                                  startOnSurface, endOnSurface, adjoint);
    od.even[6] = trans_func->eval(even, davis_beta, davis_c - h2,
                                  startOnSurface, endOnSurface, adjoint);
    od.odd[6] = trans_func->eval(odd, davis_beta, davis_c - h2, startOnSurface,
                                 endOnSurface, adjoint);
    od.all[6] = trans_func->eval(all, davis_beta, davis_c - h2, startOnSurface,
                                 endOnSurface, adjoint);

    return od;
}

// // this is monte carlo + ray marching
// BlanchetCorrelatedResults RayMarchTrans::Tr_Blanchet(const Ray& ray,
//                                                      Sampler& sampler,
//                                                      Float tMin, Float tMax,
//                                                      Float maxD,
//                                                      int blanchet_n) const {
//     // throw new std::exception();
//     // return Tr_Blanchet_Macro(ray,
//     //                          sampler,
//     //                          tMin,
//     //                          tMax,
//     //                          maxD,
//     //                          blanchet_n);

//     // std::cout << "here: " << std::endl;

//     sampler.BeforeTransmittanceSample();

//     Point3f a = ray(tMin);
//     Point3f b = ray(tMax);

//     std::vector<MacroGridQuery> queries;

//     if (majorant->Uses_Macro_Traversal()) {
//         queries = majorant->MacroVoxelTraversal(a, b);
//     } else {
//         queries = std::vector<MacroGridQuery>();
//         queries.push_back(MacroGridQuery(a, b, 1.0, tMin, tMax));
//     }

//     MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

//     if (path.tMin >= path.tMax) {
//         return (Spectrum(1.0));
//     }

//     // Float debias_prob = 0.1;
//     Float debias_prob = 0.1;

//     // std::cout << "new traversal: " << path.tMin << " " << path.tMax <<
//     // std::endl; std::cout << "blanchet_n: " << blanchet_n << std::endl;

//     // do biased 1-sample evaluatuation //
//     BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

//     // Float base_step = base_step_size * maxD / sigma_t;
//     // Float base_step = base_step_size / sigma_t;
//     Float base_step = base_step_size / 2.0;  // k = 1
//     // std::cout << "base_step: " << base_step << std::endl;

//     // the most practical estimator for multiple scattering would
//     // be to use this when the number of mean free paths are high
//     // and switch to ratio tracking when the mean free paths are
//     // low.
//     // if (path.tMax - path.tMin < 2.0)
//     //     base_step = path.tMax - path.tMin;
//     // else
//     //     base_step = 1.0;

//     if (path.tMax - path.tMin < 2.0) {
//         Spectrum Tr = Spectrum(1.0);
//         Float t = path.tMin;

//         while (true) {
//             t -= std::log(1 - sampler.Get1D()) * 1.0 / sigma_t;
//             if (t >= path.tMax) break;

//             Spectrum dens = density->D(ray(path.unmap(t)));

//             Tr *= (Spectrum(1.0) - dens) * 1.0;
//         }

//         od.bias = Tr;
//         od.odd = Tr;
//         od.even = Tr;
//         od.all = Tr;

//         return od;
//     }

//     base_step = 1.0;

//     // std::cout << path.tMax - path.tMin << std::endl;

//     Float debias_val = sampler.Get1D();

//     if (debias_prob > debias_val) {
//         int p1s = pow(2, blanchet_n + 1);
//         int ps = pow(2, blanchet_n);
//         Float inv_p1s = 1.0 / Float(p1s);
//         Float inv_ps = 1.0 / Float(ps);

//         Float t = path.tMin;

//         Float rand_val = sampler.Get1D();
//         int flip_val = (sampler.Get1D() < 0.5) ? 0 : 1;
//         std::vector<Spectrum> ods = std::vector<Spectrum>(p1s,
//         Spectrum(0.0)); while (t < path.tMax) {
//             Float step = base_step;
//             step = std::min(step, path.tMax - t);

//             for (int i = 0; i < p1s; ++i) {
//                 Float loc = t + (Float(i) + rand_val) * inv_p1s * step;

//                 Spectrum od_contr =
//                     density->D(ray(path.unmap(loc))) * step * sigma_t;

//                 ods[i] += od_contr;
//                 if (i % 2 == flip_val) {
//                     od.odd += od_contr * inv_ps;
//                 } else {
//                     od.even += od_contr * inv_ps;
//                 }
//                 od.all += od_contr * inv_p1s;
//             }

//             t += step;
//         }

//         for (int i = 0; i < ods.size(); ++i) {
//             Spectrum eval = Exp(-ods[i]);
//             od.bias = od.bias + 1.0 / Float(i + 1) * (eval - od.bias);
//         }

//         od.all = Exp(-od.all) / (debias_prob);
//         od.odd = Exp(-od.odd) / (debias_prob);
//         od.even = Exp(-od.even) / (debias_prob);
//     } else {
//         Float t = path.tMin;
//         Float rand_val = sampler.Get1D();

//         while (t < path.tMax) {
//             Float step = base_step;
//             step = std::min(step, path.tMax - t);

//             Float loc = t + rand_val * step;

//             Spectrum od_contr =
//                 density->D(ray(path.unmap(loc))) * step * sigma_t;

//             od.bias += od_contr;
//             od.odd += od_contr;
//             od.even += od_contr;
//             od.all += od_contr;

//             t += step;
//         }
//         od.exponentiate();
//         // od.bias /= (1.0 - debias_prob);
//         // od.odd /= (1.0 - debias_prob);
//         // od.even /= (1.0 - debias_prob);
//         // od.all /= (1.0 - debias_prob);
//     }

//     return od;
// }

// // this is monte carlo + ray marching
// BlanchetCorrelatedResults RayMarchTrans::Tr_Blanchet(const Ray& ray,
//                                                      Sampler& sampler,
//                                                      Float tMin, Float tMax,
//                                                      Float maxD,
//                                                      int blanchet_n) const {
//     // throw new std::exception();
//     // return Tr_Blanchet_Macro(ray,
//     //                          sampler,
//     //                          tMin,
//     //                          tMax,
//     //                          maxD,
//     //                          blanchet_n);

//     // std::cout << "here: " << std::endl;

//     sampler.BeforeTransmittanceSample();

//     Point3f a = ray(tMin);
//     Point3f b = ray(tMax);

//     std::vector<MacroGridQuery> queries;

//     if (majorant->Uses_Macro_Traversal()) {
//         queries = majorant->MacroVoxelTraversal(a, b);
//     } else {
//         queries = std::vector<MacroGridQuery>();
//         queries.push_back(MacroGridQuery(a, b, 1.0, tMin, tMax));
//     }

//     MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

//     if (path.tMin >= path.tMax) {
//         return (Spectrum(1.0));
//     }

//     // Float debias_prob = 0.1;
//     Float debias_prob = 0.1;

//     // std::cout << "new traversal: " << path.tMin << " " << path.tMax <<
//     // std::endl; std::cout << "blanchet_n: " << blanchet_n << std::endl;

//     // do biased 1-sample evaluatuation //
//     BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

//     // Float base_step = base_step_size * maxD / sigma_t;
//     // Float base_step = base_step_size / sigma_t;
//     Float base_step = base_step_size / 2.0;  // k = 1
//     // std::cout << "base_step: " << base_step << std::endl;

//     // the most practical estimator for multiple scattering would
//     // be to use this when the number of mean free paths are high
//     // and switch to ratio tracking when the mean free paths are
//     // low.
//     // if (path.tMax - path.tMin < 2.0)
//     //     base_step = path.tMax - path.tMin;
//     // else
//     //     base_step = 1.0;

//     if (path.tMax - path.tMin < 1.0)
//         base_step = path.tMax - path.tMin;
//     else
//         base_step = 1.0;

//     // std::cout << path.tMax - path.tMin << std::endl;

//     Float debias_val = sampler.Get1D();

//     if (debias_prob > debias_val) {
//         int p1s = pow(2, blanchet_n + 1);
//         int ps = pow(2, blanchet_n);
//         Float inv_p1s = 1.0 / Float(p1s);
//         Float inv_ps = 1.0 / Float(ps);

//         Float t = path.tMin;

//         Float rand_val = sampler.Get1D();
//         int flip_val = (sampler.Get1D() < 0.5) ? 0 : 1;
//         std::vector<Spectrum> ods = std::vector<Spectrum>(p1s,
//         Spectrum(0.0)); while (t < path.tMax) {
//             Float step = base_step;
//             step = std::min(step, path.tMax - t);

//             for (int i = 0; i < p1s; ++i) {
//                 Float loc = t + (Float(i) + rand_val) * inv_p1s * step;

//                 Spectrum od_contr =
//                     density->D(ray(path.unmap(loc))) * step * sigma_t;

//                 ods[i] += od_contr;
//                 if (i % 2 == flip_val) {
//                     od.odd += od_contr * inv_ps;
//                 } else {
//                     od.even += od_contr * inv_ps;
//                 }
//                 od.all += od_contr * inv_p1s;
//             }

//             t += step;
//         }

//         for (int i = 0; i < ods.size(); ++i) {
//             Spectrum eval = Exp(-ods[i]);
//             od.bias = od.bias + 1.0 / Float(i + 1) * (eval - od.bias);
//         }

//         od.all = Exp(-od.all) / (debias_prob);
//         od.odd = Exp(-od.odd) / (debias_prob);
//         od.even = Exp(-od.even) / (debias_prob);
//     } else {
//         Float t = path.tMin;
//         Float rand_val = sampler.Get1D();

//         while (t < path.tMax) {
//             Float step = base_step;
//             step = std::min(step, path.tMax - t);

//             Float loc = t + rand_val * step;

//             Spectrum od_contr =
//                 density->D(ray(path.unmap(loc))) * step * sigma_t;

//             od.bias += od_contr;
//             od.odd += od_contr;
//             od.even += od_contr;
//             od.all += od_contr;

//             t += step;
//         }
//         od.exponentiate();
//         // od.bias /= (1.0 - debias_prob);
//         // od.odd /= (1.0 - debias_prob);
//         // od.even /= (1.0 - debias_prob);
//         // od.all /= (1.0 - debias_prob);
//     }

//     return od;
// }

// PRE MICRO OPTIMIZATIONS BlanchetCorrelatedResults
BlanchetCorrelatedResults RayMarchTrans::Tr_Blanchet(const Ray& ray,
                                                     Sampler& sampler,
                                                     Float tMin, Float tMax,
                                                     Float maxD,
                                                     int blanchet_n) const {
    // throw new std::exception();
    // return Tr_Blanchet_Macro(ray,
    //                          sampler,
    //                          tMin,
    //                          tMax,
    //                          maxD,
    //                          blanchet_n);

    // std::cout << "here: " << std::endl;

    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries;

    if (majorant->Uses_Macro_Traversal()) {
        queries = majorant->MacroVoxelTraversal(a, b);
    } else {
        queries = std::vector<MacroGridQuery>();
        queries.push_back(MacroGridQuery(a, b, 1.0, tMin, tMax));
    }

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (Spectrum(1.0));
    }

    // std::cout << "new traversal: " << path.tMin << " " << path.tMax <<
    // std::endl; std::cout << "blanchet_n: " << blanchet_n << std::endl;

    // do biased 1-sample evaluatuation //
    BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

    // Float base_step = base_step_size * maxD / sigma_t;
    // Float base_step = base_step_size / sigma_t;
    // Float base_step = base_step_size / 2.0;  // k = 1
    Float base_step = base_step_size / 1.0;  // k = 1
    // std::cout << "base_step: " << base_step << std::endl;

    int p1s = pow(2, blanchet_n + 1);
    int ps = pow(2, blanchet_n);
    Float inv_p1s = 1.0 / Float(p1s);
    Float inv_ps = 1.0 / Float(ps);

    Float t = path.tMin;

    Float rand_val = sampler.Get1D();
    int flip_val = (sampler.Get1D() < 0.5) ? 0 : 1;
    std::vector<Spectrum> ods = std::vector<Spectrum>(p1s, Spectrum(0.0));
    while (t < path.tMax) {
        Float step = base_step;
        step = std::min(step, path.tMax - t);

        for (int i = 0; i < p1s; ++i) {
            Float loc = t + (Float(i) + rand_val) * inv_p1s * step;

            Spectrum od_contr =
                density->D(ray(path.unmap(loc))) * step * sigma_t;

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

    return od;
}

}  // namespace pbrt
