#pragma once

// this file contains the definitions of all of the different
// minorant functions

// TODO: remove this datastructure eventually

#include "density/densityfunc.h"

namespace pbrt {

class MinorantFunction {
  public:
    MinorantFunction(DensityFunction* density);

    virtual ~MinorantFunction() {}

    virtual Float Min(const Point3f& p) const = 0;
};

class ConstBoundedMinorant : public MinorantFunction {
  public:
    ConstBoundedMinorant(DensityFunction* density);

    virtual Float Min(const Point3f& p) const;

    Float minDensity;
};

// class HierarchicalBoundedMinorant : public MinorantFunction
// {
// public:
//     HierarchicalBoundedMinorant(DensityFunction* density);
//
//     virtual Float Min(const Point3f& p) const;
//
//     // TODO: finish tests
// };

}  // namespace pbrt
