#pragma once

#include "densityfunc.h"

namespace pbrt
{

class LinearDensityFunction : public DensityFunction
{
public:
    LinearDensityFunction();

    virtual Spectrum D(const Point3f& p, bool buildState = false) const;

    virtual Spectrum getMaxDensity() const;
    virtual Spectrum getMinDensity() const;

    virtual Spectrum getMaxDensityInRange(const Point3f& a,
                                          const Point3f& b) const;

    virtual Spectrum getMinDensityInRange(const Point3f& a,
                                          const Point3f& b) const;

    virtual bool requiresLocalSpace() const { return false; }

    void setAxis(int param);
    void setMinDensity(Spectrum param);
    void setMaxDensity(Spectrum param);
    void setStartPos(Float param);
    void setEndPos(Float param);

protected:
    int axis;

    Spectrum min_density;
    Spectrum max_density;

    Float start_pos;
    Float end_pos;
};

}
