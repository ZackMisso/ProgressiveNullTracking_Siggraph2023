#include "const_density.h"

namespace pbrt {

ConstantDensity::ConstantDensity(Spectrum dense) : dense(dense) {}

Spectrum ConstantDensity::D(const Point3f& p, bool buildState) const {
    nDensityCalls++;
    return dense;
}

Spectrum ConstantDensity::getMaxDensity() const { return dense; }

Spectrum ConstantDensity::getMinDensity() const { return dense; }

Spectrum ConstantDensity::getMaxDensityInRange(const Point3f& a,
                                               const Point3f& b) const {
    return dense;
}

Spectrum ConstantDensity::getMinDensityInRange(const Point3f& a,
                                               const Point3f& b) const {
    return dense;
}

}  // namespace pbrt
