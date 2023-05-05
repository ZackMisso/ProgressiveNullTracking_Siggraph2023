#include "simple_sphere_density.h"

#include "../../ImageEditLib/include/imedit/noise.h"

namespace pbrt {

SimpleSphereDensity::SimpleSphereDensity(Spectrum dense) : dense(dense) {}

SimpleSphereDensity::SimpleSphereDensity(Spectrum dense, Float xpos, Float ypos,
                                         Float zpos)
    : dense(dense), xpos(xpos), ypos(ypos), zpos(zpos) {}

Spectrum SimpleSphereDensity::D(const Point3f& p, bool buildState) const {
    Float x_tmp = p[0];
    Float y_tmp = p[1];
    Float z_tmp = p[2];

    return dense * (1.0 - std::min(3.5 * ((zpos - z_tmp) * (zpos - z_tmp) +
                                          (ypos - y_tmp) * (ypos - y_tmp) +
                                          (xpos - x_tmp) * (xpos - x_tmp)),
                                   1.0));
}

Spectrum SimpleSphereDensity::getMaxDensity() const { return dense; }

Spectrum SimpleSphereDensity::getMinDensity() const { return dense; }

Spectrum SimpleSphereDensity::getMaxDensityInRange(const Point3f& a,
                                                   const Point3f& b) const {
    return dense;
}

Spectrum SimpleSphereDensity::getMinDensityInRange(const Point3f& a,
                                                   const Point3f& b) const {
    return dense;
}

}  // namespace pbrt
