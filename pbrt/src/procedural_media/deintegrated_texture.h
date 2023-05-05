#pragma once

#include "pbrt.h"
#include "texture.h"
#include "paramset.h"

#if OPENVDB

#include <openvdb/openvdb.h>
#include <openvdb/tools/ChangeBackground.h>

// TODO: rename to something more fitting

namespace pbrt
{

class Deintegrated_Texture_Media : public Texture<Spectrum>
{
public:
    Deintegrated_Texture_Media(const Bounds3f& medBounds,
                               std::string image,
                               bool along_z);
    ~Deintegrated_Texture_Media();

    Spectrum Evaluate(const SurfaceInteraction& si) const;
    Spectrum Evaluate(const Point3f& p) const;
    virtual Spectrum Max() const { return Spectrum(max); }
    virtual Spectrum Min() const { return Spectrum(min); }

    const Bounds3f medBounds;

    // // this is only supported for rgb rendering currently
    openvdb::FloatGrid::Ptr r_grid;
    openvdb::FloatGrid::Ptr g_grid;
    openvdb::FloatGrid::Ptr b_grid;

    Float max;
    Float min;

    double r_dist_min;
    double g_dist_min;
    double b_dist_min;

    double r_dist_total;
    double g_dist_total;
    double b_dist_total;

    int width;
    int height;
    int depth;

    bool along_z;
};

Deintegrated_Texture_Media* CreateDeintegratedTextureMedia(const TextureParams& tp);

}

#endif
