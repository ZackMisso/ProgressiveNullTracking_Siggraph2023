#include "ratio_adap.h"

#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

BlanchetCorrelatedResults RatioTrackingAdaptive::Tr_Blanchet(
    const Ray& ray, Sampler& sampler, Float tMin, Float tMax, Float maxD,
    int blanchet_n) const {
    BlanchetCorrelatedResults tr = BlanchetCorrelatedResults(1.f);

    tr.bias = Tr(ray, sampler, tMin, tMax, maxD);
    tr.odd = tr.bias;
    tr.even = tr.bias;
    tr.all = tr.bias;

    return tr;
}

Spectrum RatioTrackingAdaptive::Tr_MutableSig(const Ray& ray, Sampler& sampler,
                                              Float tMin, Float tMax,
                                              Float& maxD, Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    Float base_maj = maxD;
    Float maj_to_use = base_maj;
    Float invMaj = 1.0 / maj_to_use;

    // Perform ratio tracking to estimate the transmittance value
    Spectrum Tr = Spectrum(1.0);
    Float t = tMin;

    while (true) {
        Float last_t = t;
        t -= std::log(1 - sampler.Get1D()) * invMaj / sig_t;

        if (t >= tMax) {
            Tr *= exp((maj_to_use - base_maj) * (tMax - last_t));
            break;
        }

        Spectrum dens = density->D(ray(t));

        if (dens[0] > maxD) {
            maxD = dens[0];
            majorant->UpdateMajorant(ray(t), maxD);
            return Spectrum(0.0);
        }

        Tr *= (Spectrum(maxD) - dens) * invMaj;
        Tr *= exp((maj_to_use - base_maj) * (t - last_t));

        maj_to_use = std::max(base_maj - dens[0], base_maj * 0.1);
        invMaj = 1.0 / maj_to_use;
    }

    return Tr;
}

Spectrum RatioTrackingAdaptive::Tr_ManualSig(const Ray& ray, Sampler& sampler,
                                             Float tMin, Float tMax, Float maxD,
                                             Float sig_t) const {
    if (majorant->Uses_Macro_Traversal())
        return Tr_Cull(ray, sampler, tMin, tMax, maxD, sig_t);

    sampler.BeforeTransmittanceSample();

    Float base_maj = maxD;
    Float maj_to_use = base_maj;
    Float invMaj = 1.0 / maj_to_use;

    // Perform ratio tracking to estimate the transmittance value
    Spectrum Tr = Spectrum(1.0);
    Float t = tMin;

    while (true) {
        Float last_t = t;
        t -= std::log(1 - sampler.Get1D()) * invMaj / sig_t;
        if (t >= tMax) {
            Tr *= exp((maj_to_use - base_maj) * (tMax - last_t));
            break;
        }

        Spectrum dens = density->D(ray(t));

        Tr *= (Spectrum(maxD) - dens) * invMaj;
        Tr *= exp((maj_to_use - base_maj) * (t - last_t));

        maj_to_use = std::max(base_maj - dens[0], base_maj * 0.1);
        invMaj = 1.0 / maj_to_use;
    }

    return Tr;
}

Spectrum RatioTrackingAdaptive::Tr_Cull(const Ray& ray, Sampler& sampler,
                                        Float tMin, Float tMax, Float maxD,
                                        Float sig_t) const {
    sampler.BeforeTransmittanceSample();

    // TODO: this should eventually be removed

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
