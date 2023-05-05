#pragma once

#include "pbrt.h"
#include "texture.h"
#include "paramset.h"

namespace pbrt
{

struct PointDensity
{
    Spectrum density;
    Point3f location;
};

// TODO: add some form of spline interpolation
enum InterpLevel
{
    INTERP_NN,
    INTERP_LI,
    INTERP_BILI,
    INTERP_BARY
};

class Point_Interp_Media : public Texture<Spectrum>
{
public:
    Point_Interp_Media(const Bounds3f& medBound);

    Spectrum Evaluate(const SurfaceInteraction& si) const;
    Spectrum Evaluate(const Point3f& p) const;

// protected:
    const Bounds3f medBounds;
    InterpLevel interp;
    std::vector<PointDensity> densities;
};

Point_Interp_Media* CreatePointInterpMedia(const TextureParams& tp);

}
