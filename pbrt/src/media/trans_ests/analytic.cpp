#include "analytic.h"

#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

Spectrum AnalyticTrans::Tr_ManualSig(const Ray& ray, Sampler& sampler,
                                     Float tMin, Float tMax, Float maxD,
                                     Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    Float od = 0.0;
    Float t = tMin;
    for (int i = 0; i < queries.size(); ++i) {
        Float step = (queries[i].t_end - queries[i].t_start) * (tMax - tMin);
        Float past_od = od;
        od += queries[i].maj * step * sigma_t;
        t += step;
    }

    return Spectrum(exp(-od));
}

Spectrum AnalyticTrans::Tr_MutableSig(const Ray& ray, Sampler& sampler,
                                      Float tMin, Float tMax, Float& maxD,
                                      Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    Float invMaxD = 1.0 / maxD;

    Spectrum dens = density->D(ray((tMax - tMin) / 2.0 + tMin));

    return Exp(-dens * Spectrum(tMax - tMin) * sigma_t);
}

// this is actual ray-marching
BlanchetCorrelatedResults AnalyticTrans::Tr_Blanchet_Macro(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax, Float maxD,
    int blanchet_n) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    // do biased 1-sample evaluatuation //
    BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

    // Float base_step = base_step_size * maxD / sigma_t;
    Float base_step = base_step_size / sigma_t;

    Float dist_samp = sampler.Get1D();
    bool flip = sampler.Get1D() > 0.5;

    Float t = path.tMin;
    while (t < path.tMax) {
        Float step = std::min(base_step, path.tMax - t);

        Float loc = t + dist_samp * step;
        od.bias += density->D(ray(path.unmap(loc))) * step * sigma_t;
        t += step;
    }
    //////////////////////////////////////

    // do correlated bias n+1-sample - avg n-sample evaluation //
    base_step /= pow(2.0, Float(blanchet_n + 1));
    t = path.tMin;
    while (t < path.tMax) {
        Float step = base_step;
        Float prev_t = t;

        if (2.0 * base_step > path.tMax - t) {
            // TODO - resize basestep
            step = (path.tMax - t) / 2.0;
        }

        // only the last step will be smaller, and we want to make sure that
        // t > tMax after the last step, so by increasing t by base_step instead
        // of step this is garaunteed despite any floating point inaccuracies.
        t += 2.0 * base_step;

        // do two steps at a time
        Float left_loc = prev_t + dist_samp * step;
        Float right_loc = (prev_t + step) + dist_samp * step;

        Spectrum left_od_part = density->D(ray(path.unmap(left_loc))) * sigma_t;
        Spectrum right_od_part =
            density->D(ray(path.unmap(right_loc))) * sigma_t;

        if (flip) {
            od.odd += left_od_part * 2.0 * step;
            od.even += right_od_part * 2.0 * step;
        } else {
            od.even += left_od_part * 2.0 * step;
            od.odd += right_od_part * 2.0 * step;
        }

        od.all += left_od_part * step;
        od.all += right_od_part * step;
    }
    /////////////////////////////////////////////////////////////

    od.exponentiate();

    return od;
}

Spectrum AnalyticTrans::Tr(const Ray& ray, Sampler& sampler, Float tMin,
                           Float tMax, bool startOnSurface, bool endOnSurface,
                           bool adjoint, Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    Float od = 0.0;
    Float t = tMin;
    for (int i = 0; i < queries.size(); ++i) {
        Float step = (queries[i].t_end - queries[i].t_start) * (tMax - tMin);
        Float past_od = od;
        od += queries[i].maj * step * sigma_t;
        t += step;
    }

    return Spectrum(
        trans_func->eval(od, startOnSurface, endOnSurface, adjoint));
}

// this is monte carlo + ray marching
BlanchetCorrelatedResults AnalyticTrans::Tr_Blanchet(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
    bool startOnSurface, bool endOnSurface, bool adjoint, int blanchet_n,
    Float maxD) const {
    sampler.BeforeTransmittanceSample();

    // return (BlanchetCorrelatedResults(0.5));

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (BlanchetCorrelatedResults(1.0));
    }

    Float od = 0.0;
    Float t = path.tMin;
    for (int i = 0; i < path.non_empty.size(); ++i) {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step * sigma_t;
    }

    return BlanchetCorrelatedResults(
        trans_func->eval(od, startOnSurface, endOnSurface, adjoint));
}

BlanchetCorrelatedResults AnalyticTrans::Tr_Blanchet_Beta(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax, Float beta,
    Float c, bool startOnSurface, bool endOnSurface, bool adjoint,
    int blanchet_n, Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (Spectrum(1.0));
    }

    Float od = 0.0;
    Float t = path.tMin;
    for (int i = 0; i < path.non_empty.size(); ++i) {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step;
    }

    return BlanchetCorrelatedResults(
        trans_func->eval(od, beta, c, startOnSurface, endOnSurface, adjoint));
}

BlanchetCorrelatedFdDebiasing AnalyticTrans::Tr_Blanchet_FdBeta(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
    bool startOnSurface, bool endOnSurface, bool adjoint, int blanchet_n,
    int davis_n, Float base_h, Float davis_beta, Float davis_c,
    Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    // std::cout << "in here zack" << std::endl;
    // return (BlanchetCorrelatedFdDebiasing(0.5));

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (BlanchetCorrelatedFdDebiasing(1.0));
    }

    Float od = 0.0;
    Float t = path.tMin;
    Float dist = 0.0;
    for (int i = 0; i < path.non_empty.size(); ++i) {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step;
        dist += step;
    }

    BlanchetCorrelatedFdDebiasing tr(Spectrum(1.f));

    Float h1 = base_h * pow(2.0, -Float(davis_n));
    Float h2 = base_h * pow(2.0, -Float(davis_n + 1));

    // BlanchetCorrelatedFdDebiasing tr(trans_func->eval(od, davis_beta,
    // davis_c, startOnSurface, endOnSurface, adjoint));
    //
    // return tr;

    // h1 = 0.0;
    // h2 = 0.0;
    // base_h = 0.0;

    // null
    // tr.bias[0] = trans_func->eval(od, davis_beta, davis_c, startOnSurface,
    // endOnSurface, adjoint); tr.even[0] = trans_func->eval(od, davis_beta,
    // davis_c, startOnSurface, endOnSurface, adjoint); tr.odd[0] =
    // trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface,
    // adjoint); tr.all[0] = trans_func->eval(od, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint);
    //
    // // h
    // tr.bias[1] = trans_func->eval(od + dist * base_h, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[1] = trans_func->eval(od
    // + dist * base_h, davis_beta, davis_c, startOnSurface, endOnSurface,
    // adjoint); tr.odd[1] = trans_func->eval(od + dist * base_h, davis_beta,
    // davis_c, startOnSurface, endOnSurface, adjoint); tr.all[1] =
    // trans_func->eval(od + dist * base_h, davis_beta, davis_c, startOnSurface,
    // endOnSurface, adjoint);
    //
    // tr.bias[2] = trans_func->eval(od - dist * base_h, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[2] = trans_func->eval(od
    // - dist * base_h, davis_beta, davis_c, startOnSurface, endOnSurface,
    // adjoint); tr.odd[2] = trans_func->eval(od - dist * base_h, davis_beta,
    // davis_c, startOnSurface, endOnSurface, adjoint); tr.all[2] =
    // trans_func->eval(od - dist * base_h, davis_beta, davis_c, startOnSurface,
    // endOnSurface, adjoint);
    //
    // // h1
    // tr.bias[3] = trans_func->eval(od + dist * h1, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[3] = trans_func->eval(od
    // + dist * h1, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[3] = trans_func->eval(od + dist * h1, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[3] = trans_func->eval(od +
    // dist * h1, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[4] = trans_func->eval(od - dist * h1, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[4] = trans_func->eval(od
    // - dist * h1, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[4] = trans_func->eval(od - dist * h1, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[4] = trans_func->eval(od -
    // dist * h1, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // // h2
    // tr.bias[5] = trans_func->eval(od + dist * h2, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[5] = trans_func->eval(od
    // + dist * h2, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[5] = trans_func->eval(od + dist * h2, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[5] = trans_func->eval(od +
    // dist * h2, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[6] = trans_func->eval(od - dist * h2, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[6] = trans_func->eval(od
    // - dist * h2, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[6] = trans_func->eval(od - dist * h2, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[6] = trans_func->eval(od -
    // dist * h2, davis_beta, davis_c, startOnSurface, endOnSurface, adjoint);

    // std::cout << "davis beta; " << davis_beta << std::endl;
    // std::cout << "davis c; " << davis_c << std::endl;

    // // // null
    tr.bias[0] = trans_func->eval(od, davis_beta, davis_c, startOnSurface,
                                  endOnSurface, adjoint);
    tr.even[0] = trans_func->eval(od, davis_beta, davis_c, startOnSurface,
                                  endOnSurface, adjoint);
    tr.odd[0] = trans_func->eval(od, davis_beta, davis_c, startOnSurface,
                                 endOnSurface, adjoint);
    tr.all[0] = trans_func->eval(od, davis_beta, davis_c, startOnSurface,
                                 endOnSurface, adjoint);

    // h
    tr.bias[1] = trans_func->eval(od, davis_beta, davis_c + base_h,
                                  startOnSurface, endOnSurface, adjoint);
    tr.even[1] = trans_func->eval(od, davis_beta, davis_c + base_h,
                                  startOnSurface, endOnSurface, adjoint);
    tr.odd[1] = trans_func->eval(od, davis_beta, davis_c + base_h,
                                 startOnSurface, endOnSurface, adjoint);
    tr.all[1] = trans_func->eval(od, davis_beta, davis_c + base_h,
                                 startOnSurface, endOnSurface, adjoint);

    tr.bias[2] = trans_func->eval(od, davis_beta, davis_c - base_h,
                                  startOnSurface, endOnSurface, adjoint);
    tr.even[2] = trans_func->eval(od, davis_beta, davis_c - base_h,
                                  startOnSurface, endOnSurface, adjoint);
    tr.odd[2] = trans_func->eval(od, davis_beta, davis_c - base_h,
                                 startOnSurface, endOnSurface, adjoint);
    tr.all[2] = trans_func->eval(od, davis_beta, davis_c - base_h,
                                 startOnSurface, endOnSurface, adjoint);

    // h1
    tr.bias[3] = trans_func->eval(od, davis_beta, davis_c + h1, startOnSurface,
                                  endOnSurface, adjoint);
    tr.even[3] = trans_func->eval(od, davis_beta, davis_c + h1, startOnSurface,
                                  endOnSurface, adjoint);
    tr.odd[3] = trans_func->eval(od, davis_beta, davis_c + h1, startOnSurface,
                                 endOnSurface, adjoint);
    tr.all[3] = trans_func->eval(od, davis_beta, davis_c + h1, startOnSurface,
                                 endOnSurface, adjoint);

    tr.bias[4] = trans_func->eval(od, davis_beta, davis_c - h1, startOnSurface,
                                  endOnSurface, adjoint);
    tr.even[4] = trans_func->eval(od, davis_beta, davis_c - h1, startOnSurface,
                                  endOnSurface, adjoint);
    tr.odd[4] = trans_func->eval(od, davis_beta, davis_c - h1, startOnSurface,
                                 endOnSurface, adjoint);
    tr.all[4] = trans_func->eval(od, davis_beta, davis_c - h1, startOnSurface,
                                 endOnSurface, adjoint);

    // h2
    tr.bias[5] = trans_func->eval(od, davis_beta, davis_c + h2, startOnSurface,
                                  endOnSurface, adjoint);
    tr.even[5] = trans_func->eval(od, davis_beta, davis_c + h2, startOnSurface,
                                  endOnSurface, adjoint);
    tr.odd[5] = trans_func->eval(od, davis_beta, davis_c + h2, startOnSurface,
                                 endOnSurface, adjoint);
    tr.all[5] = trans_func->eval(od, davis_beta, davis_c + h2, startOnSurface,
                                 endOnSurface, adjoint);

    tr.bias[6] = trans_func->eval(od, davis_beta, davis_c - h2, startOnSurface,
                                  endOnSurface, adjoint);
    tr.even[6] = trans_func->eval(od, davis_beta, davis_c - h2, startOnSurface,
                                  endOnSurface, adjoint);
    tr.odd[6] = trans_func->eval(od, davis_beta, davis_c - h2, startOnSurface,
                                 endOnSurface, adjoint);
    tr.all[6] = trans_func->eval(od, davis_beta, davis_c - h2, startOnSurface,
                                 endOnSurface, adjoint);

    // std::cout << "base_h " << base_h << std::endl;

    // // null
    // tr.bias[0] = trans_func->eval(od, davis_beta, davis_c, startOnSurface,
    // endOnSurface, adjoint); tr.even[0] = trans_func->eval(od, davis_beta,
    // davis_c, startOnSurface, endOnSurface, adjoint); tr.odd[0] =
    // trans_func->eval(od, davis_beta, davis_c, startOnSurface, endOnSurface,
    // adjoint); tr.all[0] = trans_func->eval(od, davis_beta, davis_c,
    // startOnSurface, endOnSurface, adjoint);
    //
    // // h
    // tr.bias[1] = trans_func->eval(od, davis_beta + base_h, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[1] = trans_func->eval(od,
    // davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[1] = trans_func->eval(od, davis_beta + base_h, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[1] = trans_func->eval(od,
    // davis_beta + base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[2] = trans_func->eval(od, davis_beta - base_h, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[2] = trans_func->eval(od,
    // davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[2] = trans_func->eval(od, davis_beta - base_h, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[2] = trans_func->eval(od,
    // davis_beta - base_h, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // // h1
    // tr.bias[3] = trans_func->eval(od, davis_beta + h1, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[3] = trans_func->eval(od,
    // davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[3] = trans_func->eval(od, davis_beta + h1, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[3] = trans_func->eval(od,
    // davis_beta + h1, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[4] = trans_func->eval(od, davis_beta - h1, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[4] = trans_func->eval(od,
    // davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[4] = trans_func->eval(od, davis_beta - h1, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[4] = trans_func->eval(od,
    // davis_beta - h1, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // // h2
    // tr.bias[5] = trans_func->eval(od, davis_beta + h2, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[5] = trans_func->eval(od,
    // davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[5] = trans_func->eval(od, davis_beta + h2, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[5] = trans_func->eval(od,
    // davis_beta + h2, davis_c, startOnSurface, endOnSurface, adjoint);
    //
    // tr.bias[6] = trans_func->eval(od, davis_beta - h2, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.even[6] = trans_func->eval(od,
    // davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);
    // tr.odd[6] = trans_func->eval(od, davis_beta - h2, davis_c,
    // startOnSurface, endOnSurface, adjoint); tr.all[6] = trans_func->eval(od,
    // davis_beta - h2, davis_c, startOnSurface, endOnSurface, adjoint);

    return tr;
}

// this is monte carlo + ray marching
BlanchetCorrelatedResults AnalyticTrans::Tr_Blanchet(const Ray& ray,
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

    // std::cout << "new traversal: " << path.tMin << " " << path.tMax <<
    // std::endl; std::cout << "blanchet_n: " << blanchet_n << std::endl;

    // do biased 1-sample evaluatuation //
    BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);

    // Float base_step = base_step_size * maxD / sigma_t;
    Float base_step = base_step_size / sigma_t;

    // Float dist_samp = sampler.Get1D();
    // bool flip = sampler.Get1D() > 0.5;

    Float t = path.tMin;
    while (t < path.tMax) {
        // Float step = std::min(base_step, tMax - t);
        Float step = base_step;

        // Float bias_loc = t + sampler.Get1D() * step;
        // if (bias_loc < tMax)
        //     od.bias += density->D(ray(bias_loc)) * step * sigma_t;

        for (int i = 0; i < pow(2, blanchet_n + 1); ++i) {
            Float loc = t + sampler.Get1D() * step;

            if (loc < path.tMax) {
                Spectrum od_contr =
                    density->D(ray(path.unmap(loc))) * step * sigma_t;

                // std::cout << "od: " << od_contr << std::endl;

                if (i == 0) od.bias += od_contr;
                if (i < pow(2, blanchet_n)) {
                    od.odd += od_contr / Float(pow(2, blanchet_n));
                    // od.even += od_contr / Float(pow(2, blanchet_n));
                }
                if (i >= pow(2, blanchet_n) && i < pow(2, blanchet_n + 1)) {
                    od.even += od_contr / Float(pow(2, blanchet_n));
                }
                // if (i < pow(2, blanchet_n+1))
                // {
                od.all += od_contr / Float(pow(2, blanchet_n + 1));
                // }
            }
        }

        // Float loc = t + dist_samp * step;
        // od.bias += density->D(ray(loc)) * step * sigma_t;
        t += step;
    }

    // std::cout << "finished" << std::endl;
    //////////////////////////////////////

    // do correlated bias n+1-sample - avg n-sample evaluation //
    // base_step /= pow(2.0, Float(blanchet_n+1));
    // t = tMin;
    // while (t < tMax)
    // {
    //     Float step = base_step;
    //     Float prev_t = t;
    //
    //     if (2.0 * base_step > tMax - t)
    //     {
    //         // TODO - resize basestep
    //         step = (tMax - t) / 2.0;
    //     }
    //
    //     // only the last step will be smaller, and we want to make sure that
    //     // t > tMax after the last step, so by increasing t by base_step
    //     instead
    //     // of step this is garaunteed despite any floating point
    //     inaccuracies. t += 2.0 * base_step;
    //
    //     // do two steps at a time
    //     Float left_loc = prev_t + dist_samp * step;
    //     Float right_loc = (prev_t + step) + dist_samp * step;
    //
    //     Spectrum left_od_part = density->D(ray(left_loc)) * sigma_t;
    //     Spectrum right_od_part = density->D(ray(right_loc)) * sigma_t;
    //
    //     if (flip)
    //     {
    //         od.odd += left_od_part * 2.0 * step;
    //         od.even += right_od_part * 2.0 * step;
    //     }
    //     else
    //     {
    //         od.even += left_od_part * 2.0 * step;
    //         od.odd += right_od_part * 2.0 * step;
    //     }
    //
    //     od.all += left_od_part * step;
    //     od.all += right_od_part * step;
    // }
    /////////////////////////////////////////////////////////////

    od.exponentiate();

    return od;
}

// BlanchetCorrelatedResults RayMarchTrans::Tr_Blanchet_Old
// (
//     const Ray& ray,
//     Sampler& sampler,
//     Float tMin,
//     Float tMax,
//     Float maxD,
//     int blanchet_n) const
// {
//     // return Tr_Blanchet_Macro(ray,
//     //                          sampler,
//     //                          tMin,
//     //                          tMax,
//     //                          maxD,
//     //                          blanchet_n);
//
//     sampler.BeforeTransmittanceSample();
//
//     // do biased 1-sample evaluatuation //
//     BlanchetCorrelatedResults od = BlanchetCorrelatedResults(0.0);
//
//     // Float base_step = base_step_size * maxD / sigma_t;
//     Float base_step = base_step_size / sigma_t;
//
//     Float dist_samp = sampler.Get1D();
//     bool flip = sampler.Get1D() > 0.5;
//
//     Float t = tMin;
//     while (t < tMax)
//     {
//         Float step = std::min(base_step, tMax - t);
//
//         Float loc = t + dist_samp * step;
//         od.bias += density->D(ray(loc)) * step * sigma_t;
//         t += step;
//     }
//     //////////////////////////////////////
//
//     // do correlated bias n+1-sample - avg n-sample evaluation //
//     base_step /= pow(2.0, Float(blanchet_n+1));
//     t = tMin;
//     while (t < tMax)
//     {
//         Float step = base_step;
//         Float prev_t = t;
//
//         if (2.0 * base_step > tMax - t)
//         {
//             // TODO - resize basestep
//             step = (tMax - t) / 2.0;
//         }
//
//         // only the last step will be smaller, and we want to make sure that
//         // t > tMax after the last step, so by increasing t by base_step
//         instead
//         // of step this is garaunteed despite any floating point
//         inaccuracies. t += 2.0 * base_step;
//
//         // do two steps at a time
//         Float left_loc = prev_t + dist_samp * step;
//         Float right_loc = (prev_t + step) + dist_samp * step;
//
//         Spectrum left_od_part = density->D(ray(left_loc)) * sigma_t;
//         Spectrum right_od_part = density->D(ray(right_loc)) * sigma_t;
//
//         if (flip)
//         {
//             od.odd += left_od_part * 2.0 * step;
//             od.even += right_od_part * 2.0 * step;
//         }
//         else
//         {
//             od.even += left_od_part * 2.0 * step;
//             od.odd += right_od_part * 2.0 * step;
//         }
//
//         od.all += left_od_part * step;
//         od.all += right_od_part * step;
//     }
//     /////////////////////////////////////////////////////////////
//
//     od.exponentiate();
//
//     return od;
// }

Spectrum AnalyticTrans::Tr_Beta(const Ray& ray, Sampler& sampler, Float tMin,
                                Float tMax, Float beta, Float c,
                                bool startOnSurface, bool endOnSurface,
                                bool adjoint, Float maxD) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (Spectrum(1.0));
    }

    // bool startOnSurface = getVertex0Type(flags) ==
    // CustomVertexType::VERTEX_SURFACE; bool endOnSurface =
    // getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;

    Float od = 0.0;
    Float t = path.tMin;
    for (int i = 0; i < path.non_empty.size(); ++i) {
        Float step = path.non_empty[i].t_end - path.non_empty[i].t_start;
        Float past_od = od;
        od += path.non_empty[i].maj * step * sigma_t;
    }

    return Spectrum(
        trans_func->eval(od, beta, c, startOnSurface, endOnSurface, adjoint));
}

}  // namespace pbrt
