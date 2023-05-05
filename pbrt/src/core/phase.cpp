#include "phase.h"

#include "stats.h"

namespace pbrt {

// Media Definitions
PhaseFunction::~PhaseFunction() {}

// HenyeyGreenstein Method Definitions
Float HenyeyGreenstein::Sample_p(const Vector3f &wo, Vector3f *wi,
                                 const Point2f &u) const {
    ProfilePhase _(Prof::PhaseFuncSampling);
    // Compute $\cos \theta$ for Henyey--Greenstein sample
    Float cosTheta;
    if (std::abs(g) < 1e-3) cosTheta = 1 - 2 * u[0];
    // else {
    //     Float sqrTerm = (1 - g * g) / (1 - g + 2 * g * u[0]);
    //     cosTheta = (1 + g * g - sqrTerm * sqrTerm) / (2 * g);
    // }
    else  // feign renderer version
    {
        Float numer = -2.f * u[0] * u[0] * (g * g * g + g);
        numer += 2.f * u[0] * (g - 1.f) * (g * g + 1.f);
        numer += (g - 1.f) * (g - 1.f);
        Float denom = (2.f * u[0] - 1.f) * g + 1.f;
        denom *= denom;
        cosTheta = -numer / denom;
    }

    // Compute direction _wi_ for Henyey--Greenstein sample
    Float sinTheta = std::sqrt(std::max((Float)0, 1 - cosTheta * cosTheta));
    Float phi = 2 * Pi * u[1];
    Vector3f v1, v2;
    CoordinateSystem(wo, &v1, &v2);
    *wi = SphericalDirection(sinTheta, cosTheta, phi, v1, v2, -wo);
    return PhaseHG(-cosTheta, g);
}

Float HenyeyGreenstein::p(const Vector3f &wo, const Vector3f &wi) const {
    ProfilePhase _(Prof::PhaseFuncEvaluation);
    return PhaseHG(Dot(wo, wi), g);
}

}  // namespace pbrt
