#pragma once

#include "densityfunc.h"

namespace pbrt {

class SimpleDiskDensity : public DensityFunction {
  public:
    SimpleDiskDensity(Spectrum dense);
    SimpleDiskDensity(Spectrum dense, Float width, Float depth, Float xpos);

    virtual Spectrum D(const Point3f& p, bool buildState) const;

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

    Float width;
    Float depth;
    Float xpos;
};

}  // namespace pbrt