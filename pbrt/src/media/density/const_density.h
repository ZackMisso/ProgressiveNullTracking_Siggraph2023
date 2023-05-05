#pragma once

#include "densityfunc.h"

namespace pbrt {

class ConstantDensity : public DensityFunction {
  public:
    ConstantDensity(Spectrum dense);

    virtual Spectrum D(const Point3f& p, bool buildState = false) const;

    virtual Spectrum getMaxDensity() const;
    virtual Spectrum getMinDensity() const;

    virtual Spectrum getMaxDensityInRange(const Point3f& a,
                                          const Point3f& b) const;

    virtual Spectrum getMinDensityInRange(const Point3f& a,
                                          const Point3f& b) const;

    // doesn't really need it but eh
    virtual bool requiresLocalSpace() const { return true; }

  protected:
    Spectrum dense;
};

}  // namespace pbrt
