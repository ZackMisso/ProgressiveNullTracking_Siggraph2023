#include "texture_density.h"

#include "paramset.h"

namespace pbrt {

TextureDensityFunction::TextureDensityFunction(
    Spectrum scale, std::shared_ptr<Texture<Spectrum>> texture)
    : scale(scale), Density(texture) {}

Spectrum TextureDensityFunction::D(const Point3f& p, bool buildState) const {
    nDensityCalls++;
    Spectrum ret = Density->Evaluate(p) * scale;

    if (ret[0] > 1.0) ret[0] = 1.0;
    if (ret[1] > 1.0) ret[1] = 1.0;
    if (ret[2] > 1.0) ret[2] = 1.0;

    if (ret[0] < 0.0) ret[0] = 0.0;
    if (ret[1] < 0.0) ret[1] = 0.0;
    if (ret[2] < 0.0) ret[2] = 0.0;

    return ret;
}

void TextureDensityFunction::preProcess(const Transform& medToWorld,
                                        const Bounds3f medBounds) {
    Density->preProcess(medToWorld, medBounds);
}

// TODO: actually calculate real bounds
Spectrum TextureDensityFunction::getMaxDensity() const {
    return Density->Max();
}

Spectrum TextureDensityFunction::getMinDensity() const {
    return Density->Min();
}

Spectrum TextureDensityFunction::getMaxDensityInRange(const Point3f& a,
                                                      const Point3f& b) const {
    return Spectrum(1.0);
}

Spectrum TextureDensityFunction::getMinDensityInRange(const Point3f& a,
                                                      const Point3f& b) const {
    return Spectrum(0.0);
}

}  // namespace pbrt
