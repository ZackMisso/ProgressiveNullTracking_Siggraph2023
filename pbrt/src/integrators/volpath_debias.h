
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

#pragma once

// integrators/volpath.h*
#include "integrator.h"
#include "lightdistrib.h"
#include "pbrt.h"

namespace pbrt {

// VolPathIntegrator Declarations
class VolPathDebiasedIntegrator : public SamplerIntegrator {
  public:
    // VolPathIntegrator Public Methods
    VolPathDebiasedIntegrator(
        int maxDepth, std::shared_ptr<const Camera> camera,
        std::shared_ptr<Sampler> sampler, const Bounds2i& pixelBounds,
        long maxSeconds, long maxExtCalls, uint32_t seedOne, uint32_t seedTwo,
        uint32_t seedThree, std::string trans_sampler = "independent",
        std::string ff_sampler = "independent", Float rrThreshold = 1,
        const std::string& lightSampleStrategy = "spatial",
        bool multithreaded = false)
        : SamplerIntegrator(camera, sampler, pixelBounds, maxSeconds,
                            maxExtCalls, seedOne, seedTwo, seedThree,
                            trans_sampler, ff_sampler, multithreaded),
          maxDepth(maxDepth),
          rrThreshold(rrThreshold),
          lightSampleStrategy(lightSampleStrategy) {}
    void Preprocess(const Scene& scene, Sampler& sampler);
    void Render(const Scene& scene);

    BlanchetCorrelatedResults Li(const RayDifferential& ray, const Scene& scene,
                                 Sampler& sampler, Sampler& trans_sampler,
                                 Sampler& ff_sampler, MemoryArena& arena,
                                 int blanchet_n, int depth, int maxBounces,
                                 BlanchetCorrelatedResults beta, Float etaScale,
                                 bool specularBounce) const;

    Spectrum Li(const RayDifferential& ray, const Scene& scene,
                Sampler& sampler, Sampler& trans_sampler, Sampler& ff_sampler,
                MemoryArena& arena, int blanchet_n, Float blanchet_pmf,
                int depth) const;

    Spectrum Li(const RayDifferential& ray, const Scene& scene,
                Sampler& sampler, Sampler& trans_sampler, Sampler& ff_sampler,
                MemoryArena& arena, int depth) const {
        return Spectrum(-1.f);
    }

  private:
    // VolPathIntegrator Private Data
    const int maxDepth;
    const Float rrThreshold;
    const std::string lightSampleStrategy;
    std::unique_ptr<LightDistribution> lightDistribution;
};

VolPathDebiasedIntegrator* CreateVolPathDebiasedIntegrator(
    const ParamSet& params, std::shared_ptr<Sampler> sampler,
    std::shared_ptr<const Camera> camera);

}  // namespace pbrt
