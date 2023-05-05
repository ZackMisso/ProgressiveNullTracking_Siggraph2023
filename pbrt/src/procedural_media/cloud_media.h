#pragma once

#include "pbrt.h"
#include "texture.h"
#include "paramset.h"

namespace pbrt
{

class Cloud_ProceduralMedia : public Texture<Spectrum>
{
public:
    Cloud_ProceduralMedia(const Bounds3f& medBounds);

    Spectrum Evaluate(const SurfaceInteraction& si) const;
    Spectrum Evaluate(const Point3f& p) const;

    Float evaluateCloud(const Point3f& pt) const;

    // member variables
    const Bounds3f medBounds;

    Float period;
    Float resolution;
    Float hardCodedMax;
    Float hardCodedMin;
    Float distPeriod;
    Float distFactor;
    Float threshStart;
    Float threshSlope;
};

Cloud_ProceduralMedia* CreateCloudProceduralMedia(const TextureParams& tp);

}
