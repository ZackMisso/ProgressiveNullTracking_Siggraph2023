
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

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_TEXTURES_MARBLE_H
#define PBRT_TEXTURES_MARBLE_H

// textures/marble.h*
#include "pbrt.h"
#include "texture.h"
#include "paramset.h"

namespace pbrt {

// MarbleTexture Declarations
class MarbleTexture : public Texture<Spectrum> {
  public:
    // MarbleTexture Public Methods
    MarbleTexture(std::unique_ptr<TextureMapping3D> mapping, int octaves,
                  Float omega, Float scale, Float variation)
        : mapping(std::move(mapping)),
          octaves(octaves),
          omega(omega),
          scale(scale),
          variation(variation) {}

    Spectrum Evaluate(const SurfaceInteraction &si) const {
        Vector3f dpdx, dpdy;
        Point3f p = mapping->Map(si, &dpdx, &dpdy);
        p *= scale;
        Float marble =
            p.y +
            variation * FBm_xyz(p, scale * dpdx, scale * dpdy, omega, octaves);
        Float t = .5f + .5f * std::sin(marble);
        // Evaluate marble spline at _t_
        static Float c[][3] = {
            {.58f, .58f, .6f}, {.58f, .58f, .6f}, {.58f, .58f, .6f},
            {.5f, .5f, .5f},   {.6f, .59f, .58f}, {.58f, .58f, .6f},
            {.58f, .58f, .6f}, {.2f, .2f, .33f},  {.58f, .58f, .6f},
        };
#define NC sizeof(c) / sizeof(c[0])
#define NSEG (NC - 3)
        int first = std::floor(t * NSEG);
        t = (t * NSEG - first);
        Spectrum c0 = Spectrum::FromRGB(c[first]);
        Spectrum c1 = Spectrum::FromRGB(c[first + 1]);
        Spectrum c2 = Spectrum::FromRGB(c[first + 2]);
        Spectrum c3 = Spectrum::FromRGB(c[first + 3]);
        // Bezier spline evaluated with de Castilejau's algorithm
        Spectrum s0 = (1.f - t) * c0 + t * c1;
        Spectrum s1 = (1.f - t) * c1 + t * c2;
        Spectrum s2 = (1.f - t) * c2 + t * c3;
        s0 = (1.f - t) * s0 + t * s1;
        s1 = (1.f - t) * s1 + t * s2;
        // Extra scale of 1.5 to increase variation among colors
        return 1.5f * ((1.f - t) * s0 + t * s1);
    }

    Spectrum Evaluate(const Point3f& pi) const
    {
        Point3f p = mapping->Map(pi);
        p *= scale;
        // std::cout << "BEFORE FBM" << std::endl;
        Float marbleY = 0.0 * p.y + variation * FBm_y(p,
                                                Vector3f(0.001, 0.001, 0.001),
                                                Vector3f(0.001, 0.001, 0.001),
                                                omega,
                                                octaves);

         Float marbleX = 0.0 * p.x + variation * FBm_x(p,
                                                 Vector3f(0.001, 0.001, 0.001),
                                                 Vector3f(0.001, 0.001, 0.001),
                                                 omega,
                                                 octaves);

        Float marbleZ = 2.0 * std::sin(10.0 * p.z) + variation * FBm_y(p,
                                                Vector3f(0.001, 0.001, 0.001),
                                                Vector3f(0.001, 0.001, 0.001),
                                                omega,
                                                octaves);

        marbleZ += 1.0 * Turbulence_y(p,
                                      Vector3f(0.001, 0.001, 0.001),
                                      Vector3f(0.001, 0.001, 0.001),
                                      omega,
                                      octaves);

        // std::cout << "AFTER FBM" << std::endl;
        Float tx = .5f + .5f * std::sin(marbleZ / 1.0);
        Float ty = .5f + .5f * std::sin(marbleY / 1.0);
        // Evaluate marble spline at _t_
        // static Float c[][3] = {
        //     {.58f, .58f, .6f}, {.58f, .58f, .6f}, {.58f, .58f, .6f},
        //     {.5f, .5f, .5f},   {.6f, .59f, .58f}, {.58f, .58f, .6f},
        //     {.58f, .58f, .6f}, {.2f, .2f, .33f},  {.58f, .58f, .6f},
        // };

        // static Float c[][3] = {
        //     {.58f, .58f, .6f}, {.8f, .8f, .8f}, {.58f, .58f, .6f},
        //     {.4f, .4f, .4f},   {.6f, .59f, .58f}, {.8f, .8f, .8f},
        //     {.3f, .3f, .3f}, {.2f, .2f, .33f},  {.6f, .6f, .6f},
        // };

        // static Float c[][3] = {
        //     {.8f, .8f, .8f}, {.8f, .8f, .8f}, {.0f, .0f, .0f},
        //     {.0f, .0f, .0f},   {.8f, .8f, .8f}, {.8f, .8f, .8f},
        //     {.0f, .0f, .0f}, {.2f, .2f, .2f},  {.4f, .4f, .4f},
        // };

        static Float c[][3] = {
            {.8f, .8f, .8f}, {.7f, .7f, .7f}, {.6f, .6f, .6f},
            {.5f, .5f, .5f},   {.4f, .4f, .4f}, {.3f, .3f, .3f},
            {.2f, .2f, .2f}, {.1f, .1f, .1f},  {.0f, .0f, .0f},
        };

        // static Float c[][3] = {
        //     {.8f, .8f, .0f}, {.8f, .8f, .0f}, {.08f, .08f, .0f},
        //     {.0f, .8f, .8f}, {.0f, .8f, .8f}, {.0f, .8f, .8f},
        //     {.8f, .0f, .8f}, {.8f, .0f, .8f}, {.8f, .0f, .8f},
        // };

        // static Float c[][3] = {
        //     {.58f, .58f, .6f}, {.8f, .0f, .0f}, {.58f, .58f, .6f},
        //     {.0f, .4f, .0f},   {.6f, .59f, .58f}, {.0f, .0f, .8f},
        //     {.3f, .3f, .3f}, {.2f, .2f, .33f},  {.6f, .6f, .0f},
        // };
#define NC sizeof(c) / sizeof(c[0])
#define NSEG (NC - 3)
        int firstX = std::floor(tx * NSEG);
        int firstY = std::floor(ty * NSEG);
        // std::cout << "first: " << first << std::endl;
        // std::cout << "t: " << t << std::endl;
        // std::cout << "marble: " << marble << std::endl;
        tx = (tx * NSEG - firstX);
        ty = (ty * NSEG - firstY);
        Spectrum c0x = Spectrum::FromRGB(c[firstX]);
        Spectrum c1x = Spectrum::FromRGB(c[firstX + 1]);
        Spectrum c2x = Spectrum::FromRGB(c[firstX + 2]);
        Spectrum c3x = Spectrum::FromRGB(c[firstX + 3]);
        Spectrum c0y = Spectrum::FromRGB(c[firstY]);
        Spectrum c1y = Spectrum::FromRGB(c[firstY + 1]);
        Spectrum c2y = Spectrum::FromRGB(c[firstY + 2]);
        Spectrum c3y = Spectrum::FromRGB(c[firstY + 3]);
        // std::cout << "what" << std::endl;
        // Bezier spline evaluated with de Castilejau's algorithm
        Spectrum s0x = (1.f - tx) * c0x + tx * c1x;
        Spectrum s1x = (1.f - tx) * c1x + tx * c2x;
        Spectrum s2x = (1.f - tx) * c2x + tx * c3x;
        s0x = (1.f - tx) * s0x + tx * s1x;
        s1x = (1.f - tx) * s1x + tx * s2x;

        Spectrum s0y = (1.f - ty) * c0y + ty * c1y;
        Spectrum s1y = (1.f - ty) * c1y + ty * c2y;
        Spectrum s2y = (1.f - ty) * c2y + ty * c3y;
        s0y = (1.f - ty) * s0y + ty * s1y;
        s1y = (1.f - ty) * s1y + ty * s2y;
        // std::cout << "POST" << std::endl;
        // Extra scale of 1.5 to increase variation among colors
        Spectrum rgb = 1.5 * ((1.f - tx) * s0x + tx * s1x);
        // rgb += ((1.f - ty) * s0y + ty * s1y);
        // return rgb / 2.0;

        return rgb;

        // return rgb / 2.0;
        // return 1.5f * ((1.f - t) * s0 + t * s1);
    }

  private:
    // MarbleTexture Private Data
    std::unique_ptr<TextureMapping3D> mapping;
    const int octaves;
    const Float omega, scale, variation;
};

Texture<Float> *CreateMarbleFloatTexture(const Transform &tex2world,
                                         const TextureParams &tp);
MarbleTexture *CreateMarbleSpectrumTexture(const Transform &tex2world,
                                           const TextureParams &tp);
MarbleTexture *CreateMarbleSpectrumTexture(const Transform &tex2world);

}  // namespace pbrt

#endif  // PBRT_TEXTURES_MARBLE_H
