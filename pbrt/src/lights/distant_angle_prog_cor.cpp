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

// lights/distant.cpp*
#include "lights/distant_angle_prog_cor.h"
#include "paramset.h"
#include "sampling.h"
#include "stats.h"

namespace pbrt
{

// DistantLight Method Definitions
DistantAngleProgCorLight::DistantAngleProgCorLight
(
    const Transform &LightToWorld,
    const Spectrum &L,
    const Vector3f &wLight,
    const Vector3f &wHLight
)
    : Light((int)LightFlags::DeltaDirection, LightToWorld),
      L(L),
      wLight(Normalize(LightToWorld(wLight))),
      wHLight(Normalize(LightToWorld(wHLight))) {}

Spectrum DistantAngleProgCorLight::Sample_Li
(
    const Interaction &ref,
    const Point2f &u,
    Vector3f *wi,
    Float *pdf,
    VisibilityTester *vis
) const
{
    ProfilePhase _(Prof::LightSample);

    *wi = wLight;
    *pdf = 1;
    Point3f pOutside = ref.p + wLight * (2 * worldRadius);
    // std::cout << "L: " << L << std::endl;
    *vis =
        VisibilityTester(ref, Interaction(pOutside, ref.time, mediumInterface));
    return L;
}

Spectrum DistantAngleProgCorLight::Sample_Li_H
(
    const Interaction &ref,
    const Point2f &u,
    Vector3f *wi,
    Float *pdf,
    VisibilityTester *vis
) const
{
    ProfilePhase _(Prof::LightSample);

    *wi = wHLight;
    *pdf = 1;
    Point3f pOutside = ref.p + wHLight * (2 * worldRadius);
    *vis =
        VisibilityTester(ref, Interaction(pOutside, ref.time, mediumInterface));
    return L;
}

Spectrum DistantAngleProgCorLight::Sample_Pos
(
    const Point2f& u,
    Point3f* p,
    Float* pdf
) const
{
    std::cout << "ERROR: Distant Light Sample Pos not yet Implemented" << std::endl;

    return Spectrum(1.f);
}

Spectrum DistantAngleProgCorLight::Power() const
{
    return L * Pi * worldRadius * worldRadius;
}

Float DistantAngleProgCorLight::Pdf_Li(const Interaction &, const Vector3f &) const
{
    return 0.f;
}

Float DistantAngleProgCorLight::Pdf_Li_H(const Interaction &, const Vector3f &) const
{
    return 0.f;
}

Spectrum DistantAngleProgCorLight::Sample_Le
(
    const Point2f &u1,
    const Point2f &u2,
    Float time,
    Ray *ray,
    Normal3f *nLight,
    Float *pdfPos,
    Float *pdfDir
) const
{
    ProfilePhase _(Prof::LightSample);
    // Choose point on disk oriented toward infinite light direction
    Vector3f v1, v2;
    CoordinateSystem(wLight, &v1, &v2);
    Point2f cd = ConcentricSampleDisk(u1);
    Point3f pDisk = worldCenter + worldRadius * (cd.x * v1 + cd.y * v2);

    // Set ray origin and direction for infinite light ray
    *ray = Ray(pDisk + worldRadius * wLight, -wLight, Infinity, time);
    *nLight = (Normal3f)ray->d;
    *pdfPos = 1 / (Pi * worldRadius * worldRadius);
    *pdfDir = 1;
    return L;
}

void DistantAngleProgCorLight::Pdf_Le
(
    const Ray &,
    const Normal3f &,
    Float *pdfPos,
    Float *pdfDir
) const
{
    ProfilePhase _(Prof::LightPdf);
    *pdfPos = 1 / (Pi * worldRadius * worldRadius);
    *pdfDir = 0;
}

std::shared_ptr<DistantAngleProgCorLight> CreateDistantAngleProgCorLight
(
    const Transform &light2world,
    const ParamSet &paramSet
)
{
    Spectrum L = paramSet.FindOneSpectrum("L", Spectrum(2.0));
    Spectrum sc = paramSet.FindOneSpectrum("scale", Spectrum(1.0));
    Point3f from = paramSet.FindOnePoint3f("from", Point3f(0, 0, 0));
    Point3f to = paramSet.FindOnePoint3f("to", Point3f(0, 0, -1));
    Float y_rot = paramSet.FindOneFloat("y_rot", 0.0);
    Float x_rot = paramSet.FindOneFloat("x_rot", 0.0); // should be z_rot
    Float h = paramSet.FindOneFloat("h", 1.0);
    Vector3f dir = Vector3f(1.f, 0.f, 0.f);
    Vector3f dir_h = Vector3f(1.f, 0.f, 0.f);

    // std::cout << "H: " << h << std::endl;

    // do two rotations
    dir = RotateZ(x_rot)(dir);
    dir_h = RotateZ(x_rot + h)(dir_h);
    // std::cout << "yea" << std::endl;
    // std::cout << dir << std::endl;
    dir = RotateY(y_rot)(dir);
    dir_h = RotateY(y_rot)(dir_h);
    // std::cout << dir << std::endl;
    // std::cout << "post yea" << std::endl;

    return std::make_shared<DistantAngleProgCorLight>(light2world, L * sc, dir, dir_h);
}

}  // namespace pbrt
