#pragma once

#include "densityfunc.h"

namespace pbrt {

class ProceduralSphereDensity : public DensityFunction {
  public:
    ProceduralSphereDensity(Spectrum dense);
    ProceduralSphereDensity(Spectrum dense, Float xpos, Float ypos, Float zpos,
                            Float falloff, Float cutoff_dense);

    virtual Spectrum D(const Point3f& p, bool buildState = false) const;
    virtual Spectrum proc_D(const Point3f& p, bool buildState = false) const;

    virtual Spectrum getMaxDensity() const;
    virtual Spectrum getMinDensity() const;

    virtual Spectrum getMaxDensityInRange(const Point3f& a,
                                          const Point3f& b) const;

    virtual Spectrum getMinDensityInRange(const Point3f& a,
                                          const Point3f& b) const;

    // doesn't really need it but eh
    virtual bool requiresLocalSpace() const { return true; }

    // void setBox_Min_X(Float param) { box_min_x = param; }
    // void setBox_Min_Y(Float param) { box_min_y = param; }
    // void setBox_Min_Z(Float param) { box_min_z = param; }
    // void setBox_Max_X(Float param) { box_max_x = param; }
    // void setBox_Max_Y(Float param) { box_max_y = param; }
    // void setBox_Max_Z(Float param) { box_max_z = param; }

    // Float getBox_Min_X() const { return box_min_x; }
    // Float getBox_Min_Y() const { return box_min_y; }
    // Float getBox_Min_Z() const { return box_min_z; }
    // Float getBox_Max_X() const { return box_max_x; }
    // Float getBox_Max_Y() const { return box_max_y; }
    // Float getBox_Max_Z() const { return box_max_z; }

  protected:
    Spectrum dense;

    Float xpos;
    Float ypos;
    Float zpos;
    Float falloff;
    Float cutoff_dense;

    // Float box_min_x;
    // Float box_min_y;
    // Float box_min_z;

    // Float box_max_x;
    // Float box_max_y;
    // Float box_max_z;

    // Float xScaleFactor;
    // Float yScaleFactor;
    // Float zScaleFactor;

    // Float xTransFactor;
    // Float yTransFactor;
    // Float zTransFactor;
};

}  // namespace pbrt