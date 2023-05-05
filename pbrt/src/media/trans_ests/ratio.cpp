#include "ratio.h"

#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

BlanchetCorrelatedResults RatioTracking::Tr_Blanchet(const Ray& ray,
                                                     Sampler& sampler,
                                                     Float tMin, Float tMax,
                                                     Float maxD,
                                                     int blanchet_n) const {
    BlanchetCorrelatedResults tr = BlanchetCorrelatedResults(1.f);

    tr.bias = Tr(ray, sampler, tMin, tMax, maxD);
    tr.odd = tr.bias;
    tr.even = tr.bias;
    tr.all = tr.bias;

    return tr;
}

Spectrum RatioTracking::Tr_ManualSig(const Ray& ray, Sampler& sampler,
                                     Float tMin, Float tMax, Float maxD,
                                     Float sig_t) const {
    if (majorant->Uses_Macro_Traversal())  // TODO: uncomment
        return Tr_Cull(ray, sampler, tMin, tMax, maxD, sig_t);

    sampler.BeforeTransmittanceSample();

    Float invMaxD = 1.0 / maxD;

    // Perform ratio tracking to estimate the transmittance value
    Spectrum Tr = Spectrum(1.0);
    Float t = tMin;

    while (true) {
        t -= std::log(1 - sampler.Get1D()) * invMaxD / sig_t;
        if (t >= tMax) break;

        Spectrum dens = density->D(ray(t));

        Tr *= (Spectrum(maxD) - dens) * invMaxD;
    }

    return Tr;
}

Spectrum RatioTracking::Tr_MutableSig(const Ray& ray, Sampler& sampler,
                                      Float tMin, Float tMax, Float& maxD,
                                      Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    Float invMaxD = 1.0 / maxD;
    // std::cout << "maj: " << maxD << std::endl;

    // Perform ratio tracking to estimate the transmittance value
    Spectrum Tr = Spectrum(1.0);
    Float t = tMin;

    while (true) {
        t -= std::log(1 - sampler.Get1D()) * invMaxD / sig_t;
        if (t >= tMax) break;

        // for all scenes except for figure_3, our medium density is represented
        // as monochromatic so the sigma_t will always cancel within the ratio.
        // For figure_3 we render a colored class medium where we scale the
        // density everywhere by sigma_a since we set sigma_s to zero. This is
        // incorperated here by treating it as a special case.
        Spectrum dens;
        if (Globals::MONO_DENSE_SPEC_ALBEDO) {
            dens = density->D(ray(t)) * sigma_t_spec;

            for (int i = 0; i < 3; ++i) {
                if (dens[i] > maxD) {
                    if (!Globals::NO_CLAMP) {
                        Tr[i] = 0.0;
                    }

                    // update majorant grid if applicable
                    majorant->UpdateMajorant(ray(t), dens[i]);
                }
            }
        } else {
            dens = density->D(ray(t));

            if (dens[0] > maxD) {
                if (!Globals::NO_CLAMP) {
                    Tr = 0.0;
                }

                // update majorant grid if applicable
                majorant->UpdateMajorant(ray(t), dens[0]);
            }

            // std::cout << "WRONG" << std::endl;
        }

        Tr *= (Spectrum(maxD) - dens) * invMaxD;
        // Tr *= (Spectrum(maxD * sigma_t) - dens) * invMaxD;
    }

    // std::cout << "TR: " << Tr[0] << ", " << Tr[1] << ", " << Tr[2] <<
    // std::endl;

    return Tr;
}

// Spectrum RatioTracking::Tr_MutableSig(const Ray& ray, Sampler& sampler,
//                                       Float tMin, Float tMax, Float& maxD,
//                                       Float sig_t) const {
//     sampler.BeforeTransmittanceSample();

//     Float invMaxD = 1.0 / maxD;
//     // std::cout << "maj: " << maxD << std::endl;

//     // Perform ratio tracking to estimate the transmittance value
//     Spectrum Tr = Spectrum(1.0);
//     Float t = tMin;

//     while (true) {
//         t -= std::log(1 - sampler.Get1D()) * invMaxD / sig_t;
//         if (t >= tMax) break;

//         Spectrum dens = density->D(ray(t));

//         // std::cout << "dense; " << dens[0] << std::endl;
//         // std::cout << "maj: " << maxD << std::endl;
//         // std::cout << std::endl;

// #if NO_CLAMP
//         if (dens[0] > maxD) {
//             // std::cout << "NON-BOUNDING RATIO" << std::endl;
//             maxD = dens[0];
//             majorant->UpdateMajorant(ray(t), maxD);
//         }
// #else
//         if (dens[0] > maxD) {
//             // std::cout << "updating majorant" << std::endl;
//             // std::cout << "NON-BOUNDING RATIO" << std::endl;
//             maxD = dens[0];
//             majorant->UpdateMajorant(ray(t), maxD);
//             return Spectrum(0.0);
//         }
// #endif

//         Tr *= (Spectrum(maxD) - dens) * invMaxD;
//     }

//     return Tr;
// }

Spectrum RatioTracking::Tr_Cull(const Ray& ray, Sampler& sampler, Float tMin,
                                Float tMax, Float maxD, Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    Point3f a = ray(tMin);
    Point3f b = ray(tMax);

    std::vector<MacroGridQuery> queries = majorant->MacroVoxelTraversal(a, b);

    MacroGridFunction path = MacroGridFunction(queries, tMin, tMax);

    if (path.tMin >= path.tMax) {
        return (Spectrum(1.0));
    }

    if (maj > 0.0) maxD = maj;

    Float invMaxD = 1.0 / maxD;

    // Perform ratio tracking to estimate the transmittance value
    Spectrum Tr = Spectrum(1.0);
    Float t = path.tMin;

    while (true) {
        t -= std::log(1 - sampler.Get1D()) * invMaxD / sig_t;
        if (t >= path.tMax) break;

        Spectrum dens = density->D(ray(path.unmap(t)));

        if (dens[0] > maxD) {
            std::cout << "what  ze fuk" << std::endl;
            std::cout << "maxD: " << maxD << std::endl;
        }

        Tr *= (Spectrum(maxD) - dens) * invMaxD;
    }

    return Tr;
}

}  // namespace pbrt
