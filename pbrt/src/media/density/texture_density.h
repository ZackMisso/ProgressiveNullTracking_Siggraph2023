#pragma once

#include "densityfunc.h"
#include "texture.h"

namespace pbrt {

class TextureDensityFunction : public DensityFunction {
  public:
    TextureDensityFunction(Spectrum scale,
                           std::shared_ptr<Texture<Spectrum>> texture);

    virtual Spectrum D(const Point3f& p, bool buildState = false) const;

    virtual void preProcess(const Transform& medToWorld,
                            const Bounds3f medBounds);

    virtual Spectrum getMaxDensity() const;
    virtual Spectrum getMinDensity() const;

    virtual Spectrum getMaxDensityInRange(const Point3f& a,
                                          const Point3f& b) const;

    virtual Spectrum getMinDensityInRange(const Point3f& a,
                                          const Point3f& b) const;

    virtual bool requiresLocalSpace() const { return true; }

  protected:
    std::shared_ptr<Texture<Spectrum>> Density;
    Spectrum scale;

    // bool processTrueBounds;
};

}  // namespace pbrt
