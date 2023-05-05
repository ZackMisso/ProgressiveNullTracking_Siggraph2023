#pragma once

#include "pbrt.h"
#include "texture.h"
#include "paramset.h"

// TODO: this is a procedural version of deintegrated texture

namespace pbrt
{

class Chess_Media : public Texture<Spectrum>
{
public:
    Chess_Media(const Bounds3f& medBounds,
                Float hue,
                Float sat,
                Float turb_period,
                Float xtrans,
                Float ytrans,
                Float ztrans,
                Float g_const,
                Float x_const,
                Float t_const,
                Float phase,
                Float power);

    ~Chess_Media();

    Spectrum Evaluate(const SurfaceInteraction& si) const;
    Spectrum Evaluate(const Point3f& p) const;

    const Bounds3f medBounds;

    Float hue;
    Float sat;

    Float xtrans;
    Float ytrans;
    Float ztrans;

    Float g_const;
    Float x_const;
    Float t_const;
    Float phase;
    Float power;

    Float turb_period;
};

Chess_Media* CreateChessTextureMedia(const TextureParams& tp);

}
