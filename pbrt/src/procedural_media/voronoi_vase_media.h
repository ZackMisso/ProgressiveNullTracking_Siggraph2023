#pragma once

#include "pbrt.h"
#include "texture.h"
#include "paramset.h"
#include "../textures/sine.h"

namespace pbrt
{

class Voronoi_Vase_ProceduralMedia : public Texture<Spectrum>
{
public:
    Voronoi_Vase_ProceduralMedia(const Bounds3f medBounds);

    Spectrum Evaluate(const SurfaceInteraction& si) const;
    Spectrum Evaluate(const Point3f& p) const;

protected:
    const Bounds3f medBounds;
    Float maxLen;
    int levels;
    int levelPoints;
    Float radius;
    Float zPeriod;

    // cached
    std::vector<Point3f> points;
    std::vector<Spectrum> colors;
};

Voronoi_Vase_ProceduralMedia* CreateVoronoiVaseProceduralMedia(const Transform& medToWorld,
                                                               const Bounds3f medBounds);

}
