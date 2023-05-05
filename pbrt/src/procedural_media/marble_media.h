#pragma once

#include "pbrt.h"
#include "texture.h"
#include "paramset.h"
#include "../textures/sine.h"

namespace pbrt
{

class MarbleProceduralMedia : public Texture<Spectrum>
{
public:
    MarbleProceduralMedia();

    Spectrum Evaluate(const SurfaceInteraction& si) const;
    Spectrum Evaluate(const Point3f& p) const;

protected:
    SineTexture* baseTexture;
};

MarbleProceduralMedia* CreateMarbleProceduralMedia();

}
