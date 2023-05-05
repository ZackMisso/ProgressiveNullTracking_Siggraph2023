
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

#ifndef PBRT_CORE_LIGHT_H
#define PBRT_CORE_LIGHT_H

// core/light.h*
#include "blanchet_spectrum.h"
#include "fd_cor.h"
#include "interaction.h"
#include "memory.h"
#include "pbrt.h"
// #include "trans_samplers/trans_sampler.h"

namespace pbrt {

// LightFlags Declarations
enum class LightFlags : int {
    DeltaPosition = 1,
    DeltaDirection = 2,
    Area = 4,
    Infinite = 8
};

inline bool IsDeltaLight(int flags) {
    return flags & (int)LightFlags::DeltaPosition ||
           flags & (int)LightFlags::DeltaDirection;
}

// Light Declarations
class Light {
  public:
    // Light Interface
    virtual ~Light();
    Light(int flags, const Transform &LightToWorld);
    Light(int flags, const Transform &LightToWorld, int nSamples);
#if RENDER_MUTABLE
    Light(int flags, const Transform &LightToWorld,
          MediumInterface &mediumInterface, int nSamples = 1);
#else
    Light(int flags, const Transform &LightToWorld,
          const MediumInterface &mediumInterface, int nSamples = 1);
#endif
    virtual Spectrum Sample_Li(const Interaction &ref, const Point2f &u,
                               Vector3f *wi, Float *pdf,
                               VisibilityTester *vis) const = 0;
    virtual void Sample_Li_ClampGeom(const Interaction &ref, const Point2f &u,
                                     Vector3f *wi, Float *pdf,
                                     VisibilityTester *vis,
                                     DebiasedClampingSpectrum &ret) const {
        ret.setAll(Sample_Li(ref, u, wi, pdf, vis));
    }
    virtual Spectrum Sample_Pos(const Point2f &u, Point3f *p,
                                Float *pdf) const = 0;
    virtual Spectrum Power() const = 0;
    virtual void Preprocess(const Scene &scene) {}
    virtual Spectrum Le(const RayDifferential &r) const;
    virtual Float Pdf_Li(const Interaction &ref, const Vector3f &wi) const = 0;
    virtual Spectrum Sample_Le(const Point2f &u1, const Point2f &u2, Float time,
                               Ray *ray, Normal3f *nLight, Float *pdfPos,
                               Float *pdfDir) const = 0;
    virtual void Pdf_Le(const Ray &ray, const Normal3f &nLight, Float *pdfPos,
                        Float *pdfDir) const = 0;

#if RENDER_MUTABLE
    virtual Spectrum Sample_Li_D(const Ray &ray, Sampler &sampler,
                                 Medium *medium, Vector3f *wi,
                                 VisibilityTester *vis, MediumInteraction *mi,
                                 MemoryArena &arena, Float &t, Float a,
                                 Float b) {
        return Spectrum(1);
    };

    virtual Spectrum Sample_Li_D_NoVis(const Ray &ray, Sampler &sampler,
                                       Medium *medium, Vector3f *wi,
                                       // VisibilityTester* vis,
                                       MediumInteraction *mi, Point3f &pt,
                                       MemoryArena &arena, Float &t, Float a,
                                       Float b) {
        return Spectrum(1);
    };
#else

    virtual Spectrum Sample_Li_D(const Ray &ray, Sampler &sampler,
                                 const Medium *medium, Vector3f *wi,
                                 VisibilityTester *vis, MediumInteraction *mi,
                                 MemoryArena &arena, Float &t, Float a,
                                 Float b) {
        return Spectrum(1);
    };

    virtual Spectrum Sample_Li_D_NoVis(const Ray &ray, Sampler &sampler,
                                       const Medium *medium, Vector3f *wi,
                                       // VisibilityTester* vis,
                                       MediumInteraction *mi, Point3f &pt,
                                       MemoryArena &arena, Float &t, Float a,
                                       Float b) {
        return Spectrum(1);
    };
#endif

    virtual Spectrum Sample_Li(const Interaction &ref, const Point2f &u,
                               Float h, Vector3f *wi, Float *pdf,
                               VisibilityTester *vis) const {
        return Sample_Li(ref, u, wi, pdf, vis);
    }

    virtual void Sample_Le_H(const Point2f &u_pos, Float h, Ray *ray) const {}

    virtual Spectrum Sample_Li_H(const Interaction &ref, const Point2f &u,
                                 Vector3f *wi, Float *pdf,
                                 VisibilityTester *vis) const {
        return Sample_Li(ref, u, wi, pdf, vis);
    }

    virtual Spectrum Sample_Pos(const Point2f &u, Float h, Point3f *p,
                                Float *pdf) const {
        return Sample_Pos(u, p, pdf);
    }

    // Float Pdf_Li(const Interaction &, const Vector3f &, Float h) const;

    virtual Spectrum Sample_Le(const Point2f &u1, const Point2f &u2, Float h,
                               Float time, Ray *ray, Normal3f *nLight,
                               Float *pdfPos, Float *pdfDir) const {
        return Sample_Le(u1, u2, time, ray, nLight, pdfPos, pdfDir);
    }

    // Light Public Data
    const int flags;
    const int nSamples;

#if RENDER_MUTABLE
    MediumInterface mediumInterface;
#else
    const MediumInterface mediumInterface;
#endif

  protected:
    // Light Protected Data
    const Transform LightToWorld, WorldToLight;
};

class VisibilityTester {
  public:
    VisibilityTester() {}
    // VisibilityTester Public Methods
    VisibilityTester(const Interaction &p0, const Interaction &p1)
        : p0(p0), p1(p1), transportMode(TransportMode::Radiance) {}
    const Interaction &P0() const { return p0; }
    const Interaction &P1() const { return p1; }
    void setTransportMode(TransportMode mode) { transportMode = mode; }
    bool Unoccluded(const Scene &scene) const;
    // #if RENDER_MUTABLE
    // Spectrum Tr(Scene &scene,
    //             Sampler &sampler) const;
    // #else
    Spectrum Tr(const Scene &scene, Sampler &sampler) const;
    // #endif
    Spectrum Tr_ManualSig(const Scene &scene, Sampler &sampler,
                          Float sigma_t) const;
    Spectrum Tr(const Scene &scene, Sampler &sampler, uint32_t flags) const;
    BlanchetCorrelatedResults Tr_Blanchet(const Scene &scene, Sampler &sampler,
                                          int blanchet_n) const;
    BlanchetCorrelatedResults Tr_Blanchet(const Scene &scene, Sampler &sampler,
                                          uint32_t flags, int blanchet_n) const;

  private:
    Interaction p0, p1;
    TransportMode transportMode;
};

class AreaLight : public Light {
  public:
// AreaLight Interface
#if RENDER_MUTABLE
    AreaLight(const Transform &LightToWorld, MediumInterface &medium,
              int nSamples);
#else
    AreaLight(const Transform &LightToWorld, const MediumInterface &medium,
              int nSamples);
#endif
    virtual Spectrum L(const Interaction &intr, const Vector3f &w) const = 0;
};

}  // namespace pbrt

#endif  // PBRT_CORE_LIGHT_H
