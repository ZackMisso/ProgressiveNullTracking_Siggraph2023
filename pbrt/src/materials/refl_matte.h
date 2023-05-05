#pragma once

#include "pbrt.h"
#include "material.h"

namespace pbrt
{

class ReflectiveMatteMaterial : public Material
{
public:
    ReflectiveMatteMaterial(const std::shared_ptr<Texture<Spectrum>>& Kd,
                            const std::shared_ptr<Texture<Spectrum>>& Kr)
        : Kd(Kd), Kr(Kr) { }

    void ComputeScatteringFunctions(SurfaceInteraction* si,
                                    MemoryArena& arena,
                                    TransportMode mode,
                                    bool allowMultipleLobes) const;

private:
    std::shared_ptr<Texture<Spectrum>> Kd;
    std::shared_ptr<Texture<Spectrum>> Kr;
};

ReflectiveMatteMaterial* CreateReflectiveMatteMaterial(const TextureParams& mp);

}
