#include "minorant.h"

namespace pbrt
{

MinorantFunction::MinorantFunction(DensityFunction* density)
{
    // does nothing
}

ConstBoundedMinorant::ConstBoundedMinorant(DensityFunction* density) : MinorantFunction(density)
{
    minDensity = ThreeWiseMin(density->getMinDensity()[0],
                              density->getMinDensity()[1],
                              density->getMinDensity()[2]);
}

Float ConstBoundedMinorant::Min(const Point3f& p) const
{
    return minDensity;
}

// HierarchicalBoundedMinorant::HierarchicalBoundedMinorant(DensityFunction* density) : MinorantFunction(density)
// {
//     // TODO: initialize hierarchical structure here
// }
//
// Float HierarchicalBoundedMinorant::Min(const Point3f& p) const
// {
//     // TODO
//     return 0.0;
// }

}
