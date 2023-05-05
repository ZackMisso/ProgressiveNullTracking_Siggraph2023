#pragma once

// integrators/whitted.h*
#include "integrator.h"
#include "pbrt.h"
#include "scene.h"

namespace pbrt {

// WhittedIntegrator Declarations
class NormalsIntegrator : public SamplerIntegrator {
  public:
    // WhittedIntegrator Public Methods
    NormalsIntegrator(int maxDepth, std::shared_ptr<const Camera> camera,
                      std::shared_ptr<Sampler> sampler,
                      const Bounds2i &pixelBounds, long maxSeconds,
                      long maxExtCalls, uint32_t seedOne, uint32_t seedTwo,
                      uint32_t seedThree,
                      std::string trans_sampler = "independent",
                      std::string ff_sampler = "independent",
                      bool multithreaded = false)
        : SamplerIntegrator(camera, sampler, pixelBounds, maxSeconds,
                            maxExtCalls, seedOne, seedTwo, seedThree,
                            trans_sampler, ff_sampler, multithreaded),
          maxDepth(maxDepth) {}

    Spectrum Li(const RayDifferential &ray, const Scene &scene,
                Sampler &sampler, Sampler &trans_sampler, Sampler &ff_sampler,
                MemoryArena &arena, int depth) const;

  private:
    // WhittedIntegrator Private Data
    const int maxDepth;
};

NormalsIntegrator *CreateNormalsIntegrator(
    const ParamSet &params, std::shared_ptr<Sampler> sampler,
    std::shared_ptr<const Camera> camera);

}  // namespace pbrt
