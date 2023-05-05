#pragma once

#include "pbrt.h"
#include "integrator.h"
#include "camera.h"
#include "film.h"

namespace pbrt
{

class Vol_SPPMIntegrator : public Integrator
{
public:
    Vol_SPPMIntegrator(std::shared_ptr<const Camera>& camera,
                       int nIterations,
                       int photonsPerIteration,
                       int maxDepth,
                       Float initialSearchRadius,
                       int writeFrequency)
        : camera(camera),
          initialSearchRadius(initialSearchRadius),
          nIterations(nIterations),
          maxDepth(maxDepth),
          photonsPerIteration(photonsPerIteration > 0
                                  ? photonsPerIteration
                                  : camera->film->croppedPixelBounds.Area()),
          writeFrequency(writeFrequency) { }

    void Render(const Scene& scene);

protected:
    std::shared_ptr<const Camera> camera;
    const Float initialSearchRadius;
    const int nIterations;
    const int maxDepth;
    const int photonsPerIteration;
    const int writeFrequency;
};

Integrator* CreateVolSPPMIntegrator(const ParamSet& params,
                                    std::shared_ptr<const Camera> camera);

}
