#include "majorant.h"

namespace pbrt {

ConstAdaptiveMajorant::ConstAdaptiveMajorant(const DensityFunction* density)
    : MajorantFunction(density) {
    maxDensity = 0.01;
}

Float ConstAdaptiveMajorant::Maj(const Point3f& pa, const Point3f& pb) const {
    return maxDensity;
}

void ConstAdaptiveMajorant::UpdateMajorant(const Point3f& p, Float density) {
    if (maxDensity < density) {
        maxDensity = density;
    }
}

}  // namespace pbrt
