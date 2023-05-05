
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#pragma once

#include "pbrt.h"
#include "material.h"
#include "spectrum.h"

namespace pbrt
{

// MetalMaterial Declarations
class MetalCorRoughDebiasedCenterMaterial : public Material
{
public:
    // MetalMaterial Public Methods
    MetalCorRoughDebiasedCenterMaterial
    (
        const std::shared_ptr<Texture<Spectrum>> &eta,
        const std::shared_ptr<Texture<Spectrum>> &k,
        const std::shared_ptr<Texture<Float>> &rough_0,
        const std::shared_ptr<Texture<Float>> &urough_0,
        const std::shared_ptr<Texture<Float>> &vrough_0,
        const std::shared_ptr<Texture<Float>> &bump,
        bool remapRoughness
    );

    void ComputeScatteringFunctions(SurfaceInteraction *si,
                                    MemoryArena &arena,
                                    TransportMode mode,
                                    bool allowMultipleLobes) const;

    virtual void ComputeScatteringFunctions_CorDeb
    (
        SurfaceInteraction_CorDebBSDF *si,
        Float h,
        Float h1,
        Float h2,
        MemoryArena &arena,
        TransportMode mode,
        bool allowMultipleLobes
    ) const;

  private:
    // MetalMaterial Private Data
    std::shared_ptr<Texture<Spectrum>> eta, k;
    std::shared_ptr<Texture<Float>> roughness_0, uRoughness_0, vRoughness_0;
    std::shared_ptr<Texture<Float>> bumpMap;
    bool remapRoughness;
};

MetalCorRoughDebiasedCenterMaterial *CreateMetalCorRoughDebiasedCenterMaterial(const TextureParams &mp);

}  // namespace pbrt
