#include "majorant.h"

namespace pbrt {

ConstBoundedMajorant::ConstBoundedMajorant(const DensityFunction* density)
    : MajorantFunction(density) {
    maxDensity =
        ThreeWiseMax(density->getMaxDensity()[0], density->getMaxDensity()[1],
                     density->getMaxDensity()[2]);

    maxDensity = 1.0;
    // maxDensity = 3.0;
}

Float ConstBoundedMajorant::Maj(const Point3f& pa, const Point3f& pb) const {
    // if (maxDensity != 1.0) std::cout << "WHAT IN BLAIL" << std::endl;
    // std::cout << "SCALE: " << scale << std::endl;
    return maxDensity * scale;
}

}  // namespace pbrt
