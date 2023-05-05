#include "t_est.h"

#include "pbrt.h"
#include "spectrum.h"
#include "trans_samplers/trans_sampler.h"

namespace pbrt {

void T_Estimator::preProcess() {
    // does nothing, is only extended
}

#if RENDER_MUTABLE  // for this version of the codebase, this should always be
                    // true.
Spectrum T_Estimator::Tr(const Ray& ray, Sampler& sampler, Float tMin,
                         Float tMax) {
    return Tr_MutableSig(ray, sampler, tMin, tMax, sigma_t);
#else
Spectrum T_Estimator::Tr(const Ray& ray, Sampler& sampler, Float tMin,
                         Float tMax) const {
#endif

    if (majorant->Uses_Macro_Traversal() && false) {
        std::vector<MacroGridQuery> queries =
            majorant->MacroVoxelTraversal(ray(tMin), ray(tMax));

        Spectrum tr = Spectrum(1.0);

        std::cout << "queries: " << queries.size() << std::endl;

        for (int i = 0; i < queries.size(); ++i) {
            Spectrum segment_tr = Spectrum(1.0);

            if (queries[i].maj > 1e-5) {
                segment_tr =
                    Tr(ray, sampler, queries[i].t_start * (tMax - tMin) + tMin,
                       queries[i].t_end * (tMax - tMin) + tMin, queries[i].maj);
            }

            tr[0] *= segment_tr[0];
            tr[1] *= segment_tr[1];
            tr[2] *= segment_tr[2];
        }

        return tr;
    } else {
        Float maxD = majorant->Maj(ray(tMin), ray(tMax));

        Spectrum tr = Tr(ray, sampler, tMin, tMax, maxD);

        return tr;
    }
}

Spectrum T_Estimator::Tr_ManualSig(const Ray& ray, Sampler& sampler, Float tMin,
                                   Float tMax, Float sig_t) const {
    Float maxD = majorant->Maj(ray(tMin), ray(tMax));

    return Tr_ManualSig(ray, sampler, tMin, tMax, maxD, sig_t);
}

Spectrum T_Estimator::Tr_MutableSig(const Ray& ray, Sampler& sampler,
                                    Float tMin, Float tMax, Float sig_t) {
    std::vector<MacroGridQuery> intervals =
        majorant->MacroVoxelTraversal(ray(tMin), ray(tMax), tMin, tMax);

    Spectrum val = Spectrum(1.0);

    for (int i = 0; i < intervals.size(); ++i) {
        if (intervals[i].maj < 1e-4) continue;
        val *= Tr_MutableSig(ray, sampler, intervals[i].t_start,
                             intervals[i].t_end, intervals[i].maj, sig_t);
    }

    return val;
}

Spectrum T_Estimator::Tr(const Ray& ray, Sampler& sampler, Float tMin,
                         Float tMax, bool startOnSurface, bool endOnSurface,
                         bool adjoint) const {
    if (majorant->Uses_Macro_Traversal() && false) {
        std::vector<MacroGridQuery> queries =
            majorant->MacroVoxelTraversal(ray(tMin), ray(tMax));

        Spectrum tr = Spectrum(1.0);

        std::cout << "queries: " << queries.size() << std::endl;

        for (int i = 0; i < queries.size(); ++i) {
            Spectrum segment_tr = Spectrum(1.0);

            if (queries[i].maj > 1e-5) {
                segment_tr =
                    Tr(ray, sampler, queries[i].t_start * (tMax - tMin) + tMin,
                       queries[i].t_end * (tMax - tMin) + tMin, queries[i].maj);
            }

            tr[0] *= segment_tr[0];
            tr[1] *= segment_tr[1];
            tr[2] *= segment_tr[2];
        }

        return tr;
    } else {
        Float maxD = 5.0;

        std::cout << "ERROR: THIS SHOULD NOT BE RAN CURRENTLY" << std::endl;

        Spectrum tr = Tr(ray, sampler, tMin, tMax, startOnSurface, endOnSurface,
                         adjoint, maxD);

        return tr;
    }
}

BlanchetCorrelatedResults T_Estimator::Tr_Blanchet(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax,
    bool startOnSurface, bool endOnSurface, bool adjoint,
    int blanchet_n) const {
    Float maxD = 5.0;

    BlanchetCorrelatedResults tr =
        Tr_Blanchet(ray, sampler, tMin, tMax, startOnSurface, endOnSurface,
                    adjoint, blanchet_n, maxD);

    return tr;
}

BlanchetCorrelatedResults T_Estimator::Tr_Blanchet(const Ray& ray,
                                                   Sampler& sampler, Float tMin,
                                                   Float tMax,
                                                   int blanchet_n) const {
    // TODO: account for macro traversal maybe
    Float maxD = 0.0;

    if (majorant->Uses_Macro_Traversal() && false) {
        // std::cout << "Pre" << std::endl;
        std::vector<MacroGridQuery> queries =
            majorant->MacroVoxelTraversal(ray(tMin), ray(tMax));
        int start_index = 0;
        int end_index = queries.size() - 1;

        bool start_ch = true;
        bool end_ch = true;

        for (int i = 0; i < queries.size(); ++i) {
            if (queries[i].maj > 0.0) {
                maxD = std::max(queries[i].maj, maxD);
                start_ch = false;
            } else if (start_ch)
                start_index++;

            if (queries[queries.size() - i - 1].maj > 0.0) {
                maxD = std::max(queries[queries.size() - i - 1].maj, maxD);
                end_ch = false;
            } else if (end_ch)
                end_index++;
        }

        if (end_index < start_index)
            return BlanchetCorrelatedResults(Spectrum(1.f));

        // removes empty starting and ending space
        Float new_tMin = queries[start_index].t_start * (tMax - tMin) + tMin;
        Float new_tMax = queries[end_index].t_end * (tMax - tMin) + tMin;

        return Tr_Blanchet(ray, sampler, tMin, tMax, maxD, blanchet_n);
    }

    // majorant controls stepsize, so modify majorant by n to do bias correction
    maxD = 4.0;

    if (!majorant->Uses_Macro_Traversal()) {
        maxD = majorant->Maj(ray(tMin), ray(tMax));
    }

    BlanchetCorrelatedResults tr =
        Tr_Blanchet(ray, sampler, tMin, tMax, maxD, blanchet_n);

    return tr;
}

}  // namespace pbrt
