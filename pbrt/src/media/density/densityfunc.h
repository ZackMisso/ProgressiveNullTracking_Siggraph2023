#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_DENSITYFUNCTION_H
#define PBRT_DENSITYFUNCTION_H

// media/densityfunc.h*
#include <atomic>

#include "pbrt.h"
#include "spectrum.h"
#include "transform.h"

namespace pbrt {

class DensityFunction {
  public:
    virtual ~DensityFunction() {
        nDensityCalls = 0;
        // buildState = false;
    };

    virtual Spectrum D(const Point3f& p, bool buildState = false) const = 0;
    virtual Spectrum D_Debug(const Point3f& p, bool buildState = false) const {
        return Spectrum(0.0);
    }
    virtual Spectrum getMaxDensity() const = 0;
    virtual Spectrum getMinDensity() const = 0;

    virtual void preProcess(const Transform& medToWorld,
                            const Bounds3f medBounds) {}

    virtual Spectrum getMaxDensityInRange(const Point3f& a,
                                          const Point3f& b) const = 0;

    virtual Spectrum getMinDensityInRange(const Point3f& a,
                                          const Point3f& b) const = 0;

    virtual Spectrum getMaxDensityInCube(const Point3i& a,
                                         const Point3i& b) const {
        return Spectrum(0.0);
    }

    virtual Point3i getMinDimensions() const { return Point3i(0, 0, 0); }
    virtual Point3i getMaxDimensions() const { return Point3i(1, 1, 1); }
    virtual Vector3f getDimensionLengths() const {
        Point3i one = getMaxDimensions();
        Point3i two = getMinDimensions();
        return Vector3f(one[0] - two[0], one[1] - two[1], one[2] - two[2]);
    }
    virtual Vector3f getTransFactor() const { return Vector3f(0.0, 0.0, 0.0); }

    virtual bool requiresLocalSpace() const = 0;

    // void startInitialBuildState() { buildState = true; };
    // void endInitialBuildState() { buildState = false; };

    void clearDensityCalls() { nDensityCalls = 0; }
    long getDensityCalls() { return nDensityCalls; }

    static std::atomic<long> nDensityCalls;
    // bool buildState;
};

}  // namespace pbrt

#endif  // PBRT_DENSITYFUNCTION_H
