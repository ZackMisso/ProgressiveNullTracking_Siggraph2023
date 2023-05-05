
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

#ifndef PBRT_CORE_INTERACTION_H
#define PBRT_CORE_INTERACTION_H

// core/interaction.h*
#include "pbrt.h"
#include "geometry.h"
#include "transform.h"
#include "medium.h"
#include "material.h"

namespace pbrt {

// Interaction Declarations
struct Interaction
{
    // Interaction Public Methods
    Interaction() : time(0) {}
    Interaction(const Point3f &p, const Normal3f &n, const Vector3f &pError,
                const Vector3f &wo, Float time,
                const MediumInterface &mediumInterface)
        : p(p),
          time(time),
          pError(pError),
          wo(Normalize(wo)),
          n(n),
          mediumInterface(mediumInterface) {}
    bool IsSurfaceInteraction() const { return n != Normal3f(); }
    Ray SpawnRay(const Vector3f &d) const {
        Point3f o = OffsetRayOrigin(p, pError, n, d);
        return Ray(o, d, Infinity, time, GetMedium(d));
    }
    Ray SpawnRayTo(const Point3f &p2) const {
        Point3f origin = OffsetRayOrigin(p, pError, n, p2 - p);
        Vector3f d = p2 - p;
        return Ray(origin, d, 1 - ShadowEpsilon, time, GetMedium(d));
    }
    Ray SpawnRayTo(const Interaction &it) const {
        Point3f origin = OffsetRayOrigin(p, pError, n, it.p - p);
        Point3f target = OffsetRayOrigin(it.p, it.pError, it.n, origin - it.p);
        Vector3f d = target - origin;
        return Ray(origin, d, 1 - ShadowEpsilon, time, GetMedium(d));
    }
    Interaction(const Point3f &p, const Vector3f &wo, Float time,
                const MediumInterface &mediumInterface)
        : p(p), time(time), wo(wo), mediumInterface(mediumInterface) {}
    Interaction(const Point3f &p, Float time,
                const MediumInterface &mediumInterface)
        : p(p), time(time), mediumInterface(mediumInterface) {}
    bool IsMediumInteraction() const { return !IsSurfaceInteraction(); }

    #if RENDER_MUTABLE
        Medium *GetMedium(const Vector3f &w) const {
            return Dot(w, n) > 0 ? mediumInterface.outside : mediumInterface.inside;
        }
        Medium *GetMedium() const
        {
            #if LOGGING
                CHECK_EQ(mediumInterface.inside, mediumInterface.outside);
            #endif
            return mediumInterface.inside;
        }
    #else
        const Medium *GetMedium(const Vector3f &w) const {
            return Dot(w, n) > 0 ? mediumInterface.outside : mediumInterface.inside;
        }
        const Medium *GetMedium() const
        {
            #if LOGGING
                CHECK_EQ(mediumInterface.inside, mediumInterface.outside);
            #endif
            return mediumInterface.inside;
        }
    #endif

    // Interaction Public Data
    Point3f p;
    Float time;
    Vector3f pError;
    Vector3f wo;
    Normal3f n;
    MediumInterface mediumInterface;
};

class MediumInteraction : public Interaction {
  public:
    // MediumInteraction Public Methods
    MediumInteraction() : phase(nullptr) {}

    #if RENDER_MUTABLE
        MediumInteraction(const Point3f &p,
                        const Vector3f &wo,
                        Float time,
                        Medium *medium,
                        const PhaseFunction *phase)
            : Interaction(p, wo, time, medium), phase(phase) {}
        MediumInteraction(const Point3f &p,
                        const Vector3f &wo,
                        Float time,
                        Medium *medium,
                        const PhaseFunction *phase,
                        const Ray ray,
                        const Float tMax,
                        const Float tMin)
            : Interaction(p, wo, time, medium), phase(phase), tMax(tMax), tMin(tMin), ray(ray) {}
    #else
        MediumInteraction(const Point3f &p,
                      const Vector3f &wo,
                      Float time,
                      const Medium *medium,
                      const PhaseFunction *phase)
        : Interaction(p, wo, time, medium), phase(phase) {}
    MediumInteraction(const Point3f &p,
                      const Vector3f &wo,
                      Float time,
                      const Medium *medium,
                      const PhaseFunction *phase,
                      const Ray ray,
                      const Float tMax,
                      const Float tMin)
        : Interaction(p, wo, time, medium), phase(phase), tMax(tMax), tMin(tMin), ray(ray) {}
    #endif
    bool IsValid() const { return phase != nullptr; }

    // MediumInteraction Public Data
    // TODO: this is slow
    const PhaseFunction *phase;
    Ray ray;
    Float tMax;
    Float tMin;
};

// SurfaceInteraction Declarations
class SurfaceInteraction : public Interaction
{
  public:
    // SurfaceInteraction Public Methods
    SurfaceInteraction() {}
    SurfaceInteraction(const Point3f &p, const Vector3f &pError,
                       const Point2f &uv, const Vector3f &wo,
                       const Vector3f &dpdu, const Vector3f &dpdv,
                       const Normal3f &dndu, const Normal3f &dndv, Float time,
                       const Shape *sh,
                       int faceIndex = 0);
    void SetShadingGeometry(const Vector3f &dpdu, const Vector3f &dpdv,
                            const Normal3f &dndu, const Normal3f &dndv,
                            bool orientationIsAuthoritative);

    void ComputeScatteringFunctions(const RayDifferential &ray,
                                    MemoryArena &arena,
                                    bool allowMultipleLobes = false,
                                    TransportMode mode = TransportMode::Radiance);

    void ComputeScatteringFunctions(const RayDifferential &ray,
                                    MemoryArena &arena,
                                    int depth,
                                    bool allowMultipleLobes = false,
                                    TransportMode mode = TransportMode::Radiance);

    void ComputeDifferentials(const RayDifferential &r) const;
    Spectrum Le(const Vector3f &w) const;

    // SurfaceInteraction Public Data
    Point2f uv;
    Vector3f dpdu, dpdv;
    Normal3f dndu, dndv;
    const Shape *shape = nullptr;
    struct {
        Normal3f n;
        Vector3f dpdu, dpdv;
        Normal3f dndu, dndv;
    } shading;
    const Primitive *primitive = nullptr;
    BSDF *bsdf = nullptr;
    BSSRDF *bssrdf = nullptr;
    mutable Vector3f dpdx, dpdy;
    mutable Float dudx = 0, dvdx = 0, dudy = 0, dvdy = 0;

    // Added after book publication. Shapes can optionally provide a face
    // index with an intersection point for use in Ptex texture lookups.
    // If Ptex isn't being used, then this value is ignored.
    int faceIndex = 0;
};

// this is used when correlating two evaluations of BSDF roughness for
// finite differences.
//
// SurfaceInteraction_CorBSDF Declarations

class SurfaceInteraction_CorBSDF : public SurfaceInteraction
{
public:
    SurfaceInteraction_CorBSDF() { }

    SurfaceInteraction_CorBSDF
    (
        const Point3f &p,
        const Vector3f &pError,
        const Point2f &uv,
        const Vector3f &wo,
        const Vector3f &dpdu,
        const Vector3f &dpdv,
        const Normal3f &dndu,
        const Normal3f &dndv,
        Float time,
        const Shape *sh,
        int faceIndex = 0
    )
        : SurfaceInteraction(p,
                             pError,
                             uv,
                             wo,
                             dpdu,
                             dpdv,
                             dndu,
                             dndv,
                             time,
                             sh,
                             faceIndex) { }

    void ComputeScatteringFunctions(const RayDifferential &ray,
                                    MemoryArena &arena,
                                    bool allowMultipleLobes = false,
                                    TransportMode mode = TransportMode::Radiance);

    BSDF* bsdf_h = nullptr;
};

class SurfaceInteraction_CorDebBSDF : public SurfaceInteraction
{
public:
    SurfaceInteraction_CorDebBSDF() { }

    SurfaceInteraction_CorDebBSDF
    (
        const Point3f &p,
        const Vector3f &pError,
        const Point2f &uv,
        const Vector3f &wo,
        const Vector3f &dpdu,
        const Vector3f &dpdv,
        const Normal3f &dndu,
        const Normal3f &dndv,
        Float time,
        const Shape *sh,
        int faceIndex = 0
    )
        : SurfaceInteraction(p,
                             pError,
                             uv,
                             wo,
                             dpdu,
                             dpdv,
                             dndu,
                             dndv,
                             time,
                             sh,
                             faceIndex) { }

    void ComputeScatteringFunctions(const RayDifferential &ray,
                                    Float h,
                                    Float h1,
                                    Float h2,
                                    MemoryArena &arena,
                                    bool allowMultipleLobes = false,
                                    TransportMode mode = TransportMode::Radiance);

    BSDF* bsdf_h = nullptr;
    BSDF* bsdf_h1 = nullptr;
    BSDF* bsdf_h2 = nullptr;

    BSDF* bsdf_h00 = nullptr;
    BSDF* bsdf_h01 = nullptr;
    BSDF* bsdf_h10 = nullptr;
    BSDF* bsdf_h11 = nullptr;
    BSDF* bsdf_h20 = nullptr;
    BSDF* bsdf_h21 = nullptr;
};

// A vertex type to indicate what are the end vertices of a ray
enum class CustomVertexType
{
	VERTEX_SURFACE,
	VERTEX_MEDIUM,
	VERTEX_UNDEFINED
};

// code for the vertex types (bit representations)
// surface - 01
// medium  - 10
// unknown - 11
inline CustomVertexType getVertexType(int code)
{
	switch (code)
	{
	case 1: return CustomVertexType::VERTEX_SURFACE;
	case 2: return CustomVertexType::VERTEX_MEDIUM;
	case 3:
	default: return CustomVertexType::VERTEX_UNDEFINED;
	}
}

inline uint32_t getVertexCode(CustomVertexType type)
{
	switch (type)
	{
	case pbrt::CustomVertexType::VERTEX_SURFACE: return 1;
	case pbrt::CustomVertexType::VERTEX_MEDIUM: return 2;
	case pbrt::CustomVertexType::VERTEX_UNDEFINED:
	default: return 3;
	}
}

inline CustomVertexType getVertex0Type(const uint32_t flags)
{
	int v0Flags = flags & 0x00000003;
	return getVertexType(v0Flags);
}

inline CustomVertexType getVertex1Type(const uint32_t flags)
{
	int v1Flags = (flags & 0x0000000C) >> 2;
	return getVertexType(v1Flags);
}

inline void setVertex0Type(CustomVertexType type, uint32_t& flags)
{
	int code = getVertexCode(type);
	flags |= code;
}

inline void setVertex1Type(CustomVertexType type, uint32_t& flags)
{
	int code = getVertexCode(type) << 2;
	flags |= code;
}

inline void setVertex0Camera(uint32_t& flags)
{
    // set the bit at location = 5
    int code = 1 << 4;
    flags |= code;
}

inline void resetVertex(uint32_t& flags)
{
    flags = 0;
}

inline bool isVertex0Camera(const uint32_t& flags)
{
    // check if the 5th bit is 1
    return ((flags & 0x00000010) >> 4);
}

}  // namespace pbrt

#endif  // PBRT_CORE_INTERACTION_H
