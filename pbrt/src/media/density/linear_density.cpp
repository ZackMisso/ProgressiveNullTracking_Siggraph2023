#include "linear_density.h"

namespace pbrt {

LinearDensityFunction::LinearDensityFunction() {
    axis = 1;
    start_pos = -1.0;
    end_pos = 3.0;

    min_density = Spectrum(0.0);
    max_density = Spectrum(1.0);
}

Spectrum LinearDensityFunction::D(const Point3f& p, bool buildState) const {
    if (p[axis] < start_pos) return min_density;
    if (p[axis] > end_pos) return max_density;

    Float proxy = (p[axis] - start_pos) / (end_pos - start_pos);

    return proxy * max_density + (1.0 - proxy) * min_density;
}

Spectrum LinearDensityFunction::getMaxDensity() const { return max_density; }

Spectrum LinearDensityFunction::getMinDensity() const { return min_density; }

Spectrum LinearDensityFunction::getMaxDensityInRange(const Point3f& a,
                                                     const Point3f& b) const {
    if (a.x > b.x) return Spectrum(a.x);
    return Spectrum(b.x);
}

Spectrum LinearDensityFunction::getMinDensityInRange(const Point3f& a,
                                                     const Point3f& b) const {
    if (a.x > b.x) return Spectrum(b.x);
    return Spectrum(a.x);
}

void LinearDensityFunction::setAxis(int param) { axis = param; }
void LinearDensityFunction::setMinDensity(Spectrum param) {
    min_density = param;
}
void LinearDensityFunction::setMaxDensity(Spectrum param) {
    max_density = param;
}
void LinearDensityFunction::setStartPos(Float param) { start_pos = param; }
void LinearDensityFunction::setEndPos(Float param) { end_pos = param; }

}  // namespace pbrt
