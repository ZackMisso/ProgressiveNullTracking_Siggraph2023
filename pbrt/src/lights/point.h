
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

#ifndef PBRT_LIGHTS_POINT_H
#define PBRT_LIGHTS_POINT_H

// lights/point.h*
#include "pbrt.h"
#include "light.h"
#include "shape.h"

namespace pbrt
{

// PointLight Declarations
class PointLight : public Light {
  public:
    // PointLight Public Methods
    #if RENDER_MUTABLE
    PointLight(const Transform &LightToWorld,
               MediumInterface &mediumInterface, const Spectrum &I)
    #else
    PointLight(const Transform &LightToWorld,
               const MediumInterface &mediumInterface, const Spectrum &I)
    #endif
        : Light((int)LightFlags::DeltaPosition, LightToWorld, mediumInterface),
          pLight(LightToWorld(Point3f(0, 0, 0))),
          I(I) {}
    Spectrum Sample_Li(const Interaction &ref,
                       const Point2f &u,
                       Vector3f *wi,
                       Float *pdf,
                       VisibilityTester *vis) const;
    void Sample_Li_ClampGeom(const Interaction &ref,
                             const Point2f &u,
                             Vector3f *wi,
                             Float *pdf,
                             VisibilityTester *vis,
                             DebiasedClampingSpectrum& ret) const;
    Spectrum Sample_Pos(const Point2f& u,
                        Point3f* p,
                        Float* pdf) const;
    Spectrum Power() const;
    Float Pdf_Li(const Interaction &, const Vector3f &) const;
    Spectrum Sample_Le(const Point2f &u1, const Point2f &u2, Float time,
                       Ray *ray, Normal3f *nLight, Float *pdfPos,
                       Float *pdfDir) const;
    void Pdf_Le(const Ray &, const Normal3f &, Float *pdfPos,
                Float *pdfDir) const;

    #if RENDER_MUTABLE
    virtual Spectrum Sample_Li_D(const Ray& ray,
                                 Sampler& sampler,
                                 Medium* medium,
                                 Vector3f* wi,
                                 VisibilityTester* vis,
                                 MediumInteraction* mi,
                                 MemoryArena& arena,
                                 Float& t,
                                 Float a,
                                 Float b);

    virtual Spectrum Sample_Li_D_NoVis(const Ray& ray,
                                       Sampler& sampler,
                                       Medium* medium,
                                       Vector3f* wi,
                                       // VisibilityTester* vis,
                                       MediumInteraction* mi,
                                       Point3f& pt,
                                       MemoryArena& arena,
                                       Float& t,
                                       Float a,
                                       Float b);
    #else
    virtual Spectrum Sample_Li_D(const Ray& ray,
                                 Sampler& sampler,
                                 const Medium* medium,
                                 Vector3f* wi,
                                 VisibilityTester* vis,
                                 MediumInteraction* mi,
                                 MemoryArena& arena,
                                 Float& t,
                                 Float a,
                                 Float b);

    virtual Spectrum Sample_Li_D_NoVis(const Ray& ray,
                                       Sampler& sampler,
                                       const Medium* medium,
                                       Vector3f* wi,
                                       // VisibilityTester* vis,
                                       MediumInteraction* mi,
                                       Point3f& pt,
                                       MemoryArena& arena,
                                       Float& t,
                                       Float a,
                                       Float b);
    #endif

  private:
    // PointLight Private Data
    const Point3f pLight;
    const Spectrum I;
};

#if RENDER_MUTABLE
std::shared_ptr<PointLight> CreatePointLight(const Transform &light2world,
                                             Medium *medium,
                                             const ParamSet &paramSet);
#else
std::shared_ptr<PointLight> CreatePointLight(const Transform &light2world,
                                             const Medium *medium,
                                             const ParamSet &paramSet);
#endif

}  // namespace pbrt

#endif  // PBRT_LIGHTS_POINT_H
