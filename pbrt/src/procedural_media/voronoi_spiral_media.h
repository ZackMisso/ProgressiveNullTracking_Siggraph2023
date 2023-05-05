#pragma once

#include "pbrt.h"
#include "texture.h"
#include "paramset.h"

namespace pbrt
{

class Voronoi_Spiral_ProceduralMedia : public Texture<Spectrum>
{
public:
    Voronoi_Spiral_ProceduralMedia(const Bounds3f& medBounds);

    Spectrum Evaluate(const SurfaceInteraction& si) const;
    Spectrum Evaluate(const Point3f& p) const;

    const Bounds3f medBounds;
    std::vector<Point3f> points;
    std::vector<Spectrum> colors;
    Spectrum noisePeriod;
    Spectrum noiseAmp;
    Float radius;
    Float zPeriod;
    int levelPoints;
};

Voronoi_Spiral_ProceduralMedia* CreateVoronoiSpiralProceduralMedia(const TextureParams &tp);

}
