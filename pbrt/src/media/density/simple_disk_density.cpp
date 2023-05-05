#include "simple_disk_density.h"

#include "../../ImageEditLib/include/imedit/noise.h"

namespace pbrt {

SimpleDiskDensity::SimpleDiskDensity(Spectrum dense) : dense(dense) {}

SimpleDiskDensity::SimpleDiskDensity(Spectrum dense, Float width, Float depth,
                                     Float xpos)
    : dense(dense), width(width), depth(depth), xpos(xpos) {}

Spectrum SimpleDiskDensity::D(const Point3f& p, bool buildState) const {
    Float x_tmp = p[0];
    Float y_tmp = p[1];
    Float z_tmp = p[2];

    if (buildState) {
        if (std::abs(x_tmp - 0.4) < width) {
            // std::cout << "IN HERE" << std::endl;
            return dense *
                   (1.0 - std::min(depth * ((0.0 - z_tmp) * (0.0 - z_tmp) +
                                            (0.0 - y_tmp) * (0.0 - y_tmp)),
                                   1.0));
        }
    } else {
        // std::cout << "IN HERE" << std::endl;
        if (std::abs(x_tmp - xpos) < width)
            return dense *
                   (1.0 - std::min(depth * ((0.0 - z_tmp) * (0.0 - z_tmp) +
                                            (0.0 - y_tmp) * (0.0 - y_tmp)),
                                   1.0));
    }
    return 0.0;
}

Spectrum SimpleDiskDensity::getMaxDensity() const { return dense; }

Spectrum SimpleDiskDensity::getMinDensity() const { return dense; }

Spectrum SimpleDiskDensity::getMaxDensityInRange(const Point3f& a,
                                                 const Point3f& b) const {
    return dense;
}

Spectrum SimpleDiskDensity::getMinDensityInRange(const Point3f& a,
                                                 const Point3f& b) const {
    return dense;
}

}  // namespace pbrt
