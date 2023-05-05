
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


// lights/point.cpp*
#include "lights/point.h"
#include "scene.h"
#include "paramset.h"
#include "sampling.h"
#include "stats.h"
#include "sampler.h"

namespace pbrt {

// PointLight Method Definitions
Spectrum PointLight::Sample_Li(const Interaction& ref,
                               const Point2f& u,
                               Vector3f* wi,
                               Float* pdf,
                               VisibilityTester* vis) const
{
    ProfilePhase _(Prof::LightSample);
    *wi = Normalize(pLight - ref.p);
    *pdf = 1.f;
    *vis = VisibilityTester(ref, Interaction(pLight, ref.time, mediumInterface));
    return I / DistanceSquared(pLight, ref.p);
}

void PointLight::Sample_Li_ClampGeom(const Interaction &ref,
                                     const Point2f &u,
                                     Vector3f *wi,
                                     Float *pdf,
                                     VisibilityTester *vis,
                                     DebiasedClampingSpectrum& ret) const
{
    // ret.setAll(Sample_Li(ref, u, wi, pdf, vis));
    // return;

    ProfilePhase _(Prof::LightSample);
    *wi = Normalize(pLight - ref.p);
    *pdf = 1.f;
    *vis = VisibilityTester(ref, Interaction(pLight, ref.time, mediumInterface));

    ret.setAll(I / DistanceSquared(pLight, ref.p));
    return;

    Float geom = 1.0 / DistanceSquared(pLight, ref.p);
    Float base = 4.0; // TODO: make this a property of debiased clamping spectrum

    Float base_k = base * pow(2.0, double(ret.k));
    Float base_kp1 = base * pow(2.0, double(ret.k + 1));

    Float dist = std::min(geom, base);
    Float dist_k = std::min(geom, base_k);
    Float dist_kp1 = std::min(geom, base_kp1);

    ret.biased = dist * I;
    ret.top = dist_kp1 * I;
    ret.bot = dist_k * I;
}

Spectrum PointLight::Sample_Pos(const Point2f& u,
                                Point3f* p,
                                Float* pdf) const
{
    ProfilePhase _(Prof::LightSample);
    *p = pLight;
    *pdf = 1.f;
    return I;
}

Spectrum PointLight::Power() const { return 4 * Pi * I; }

Float PointLight::Pdf_Li(const Interaction &, const Vector3f &) const {
    return 0;
}

Spectrum PointLight::Sample_Le(const Point2f &u1,
                               const Point2f &u2,
                               Float time,
                               Ray *ray,
                               Normal3f *nLight,
                               Float *pdfPos,
                               Float *pdfDir) const
{
    ProfilePhase _(Prof::LightSample);
    *ray = Ray(pLight,
               UniformSampleSphere(u1),
               Infinity,
               time,
               mediumInterface.inside);
    *nLight = (Normal3f)ray->d;
    *pdfPos = 1;
    *pdfDir = UniformSpherePdf();
    return I;
}

void PointLight::Pdf_Le(const Ray &, const Normal3f &, Float *pdfPos,
                        Float *pdfDir) const {
    ProfilePhase _(Prof::LightPdf);
    *pdfPos = 0;
    *pdfDir = UniformSpherePdf();
}

#if RENDER_MUTABLE
Spectrum PointLight::Sample_Li_D(const Ray& ray,
                                 Sampler& sampler,
                                 Medium* medium,
                                 Vector3f* wi,
                                 VisibilityTester* vis,
                                 MediumInteraction* mi,
                                 MemoryArena& arena,
                                 Float& t,
                                 Float a,
                                 Float b)
#else
Spectrum PointLight::Sample_Li_D(const Ray& ray,
                                 Sampler& sampler,
                                 const Medium* medium,
                                 Vector3f* wi,
                                 VisibilityTester* vis,
                                 MediumInteraction* mi,
                                 MemoryArena& arena,
                                 Float& t,
                                 Float a,
                                 Float b)
#endif
{
    sampler.BeforeFreeFlightSample();

    // we now have a and b, now we need to find c
    Vector3f rayD = ray.d;
    Vector3f toLight = pLight - ray.o;

    Float c = Dot(rayD, toLight) / rayD.Length();

    *wi = Normalize(pLight - ray(c));

    a -= c;
    b -= c;

    Float D = (ray(c) - pLight).Length();

    Float thetaA = atan(a / D);
    Float thetaB = atan(b / D);

    Float e = sampler.Get1D();
    t = D * tan(e * (thetaB - thetaA) + thetaA) + c;
    Float pdf = D / (thetaB - thetaA);

    PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(medium->G());
    *mi = MediumInteraction(ray(t),
                            -ray.d,
                            ray.time,
                            medium,
                            phase);

    *vis = VisibilityTester(*mi, Interaction(pLight, mi->time, mediumInterface));

    return I / pdf;
}

#if RENDER_MUTABLE
Spectrum PointLight::Sample_Li_D_NoVis(const Ray& ray,
                                       Sampler& sampler,
                                       Medium* medium,
                                       Vector3f* wi,
                                       MediumInteraction* mi,
                                       Point3f& pt,
                                       MemoryArena& arena,
                                       Float& t,
                                       Float a,
                                       Float b)
#else
Spectrum PointLight::Sample_Li_D_NoVis(const Ray& ray,
                                       Sampler& sampler,
                                       const Medium* medium,
                                       Vector3f* wi,
                                       MediumInteraction* mi,
                                       Point3f& pt,
                                       MemoryArena& arena,
                                       Float& t,
                                       Float a,
                                       Float b)
#endif
{
    sampler.BeforeFreeFlightSample();
    // we now have a and b, now we need to find c
    Vector3f rayD = ray.d;
    Vector3f toLight = pLight - ray.o;

    Float c = Dot(rayD, toLight) / rayD.Length();

    *wi = Normalize(pLight - ray(c));

    a -= c;
    b -= c;

    Float D = (ray(c) - pLight).Length();

    Float thetaA = atan(a / D);
    Float thetaB = atan(b / D);

    Float e = sampler.Get1D();
    t = D * tan(e * (thetaB - thetaA) + thetaA) + c;
    Float pdf = D / (thetaB - thetaA);

    PhaseFunction *phase = ARENA_ALLOC(arena, HenyeyGreenstein)(medium->G());
    *mi = MediumInteraction(ray(t),
                            -ray.d,
                            ray.time,
                            medium,
                            phase);

    pt = pLight;
    //
    // *vis = VisibilityTester(*mi, Interaction(pLight, mi->time, mediumInterface));

    return I / pdf;
}

#if RENDER_MUTABLE
std::shared_ptr<PointLight> CreatePointLight(const Transform &light2world,
                                             Medium *medium,
                                             const ParamSet &paramSet) {
#else
std::shared_ptr<PointLight> CreatePointLight(const Transform &light2world,
                                             const Medium *medium,
                                             const ParamSet &paramSet) {
#endif
    Spectrum I = paramSet.FindOneSpectrum("I", Spectrum(1.0));
    Spectrum sc = paramSet.FindOneSpectrum("scale", Spectrum(1.0));
    Point3f P = paramSet.FindOnePoint3f("from", Point3f(0, 0, 0));
    Transform l2w = Translate(Vector3f(P.x, P.y, P.z)) * light2world;

    #if RENDER_MUTABLE
    MediumInterface med_tmp = MediumInterface(medium);
    #else
    const MediumInterface med_tmp = MediumInterface(medium);
    #endif

    return std::make_shared<PointLight>(l2w, med_tmp, I * sc);
}

}  // namespace pbrt
