
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

#ifndef PBRT_CORE_INTEGRATOR_H
#define PBRT_CORE_INTEGRATOR_H

// core/integrator.h*
#include "blanchet_spectrum.h"
#include "fd_cor.h"
#include "light.h"
#include "material.h"
#include "pbrt.h"
#include "primitive.h"
#include "reflection.h"
#include "sampler.h"
#include "spectrum.h"

namespace pbrt {

// forwhatever reason I will include this here
static bool is_power_of_2(int val) {
    int i = 1;
    // return true;
    while (i != 0) {
        if (val == i) return true;
        i <<= 1;
    }
    return false;
}

// Integrator Declarations
class Integrator {
  public:
    // Integrator Interface
    virtual ~Integrator();
    virtual void Render(const Scene &scene) = 0;
    virtual void Render_Mutable(Scene &scene) { Render(scene); }
};

Spectrum UniformSampleAllLights(const Interaction &it, const Scene &scene,
                                MemoryArena &arena, Sampler &sampler,
                                Sampler &trans_sampler,
                                const std::vector<int> &nLightSamples,
                                bool handleMedia = false);

Spectrum UniformSampleAllLights(const Interaction &it, const Scene &scene,
                                MemoryArena &arena, Sampler &sampler,
                                Sampler &trans_sampler,
                                bool handleMedia = false);

Spectrum UniformSampleAllLights_NonExp(const Interaction &it,
                                       const Scene &scene, MemoryArena &arena,
                                       Sampler &sampler, Sampler &trans_sampler,
                                       bool handleMedia = false);

Spectrum UniformSampleOneLight(const Interaction &it, const Scene &scene,
                               MemoryArena &arena, Sampler &sampler,
                               Sampler &trans_sampler, bool handleMedia = false,
                               const Distribution1D *lightDistrib = nullptr);

Spectrum UniformSampleOneLight_ManualSig(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, bool handleMedia = false,
    const Distribution1D *lightDistrib = nullptr, Float sigma_t = 1.0);

Spectrum UniformSampleOneLight_NonExp(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler,
    const Distribution1D *lightDistrib = nullptr);

BlanchetCorrelatedResults UniformSampleAllLights_NonExp_Blanchet(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, int blanchet_n);

BlanchetCorrelatedResults UniformSampleOneLight_Blanchet(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, int blanchet_n,
    const Distribution1D *lightDistrib = nullptr);

BlanchetCorrelatedResults UniformSampleAllLights_Blanchet(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, int blanchet_n);

BlanchetCorrelatedResults UniformSampleOneLight_NonExp_Blanchet(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, int blanchet_n,
    const Distribution1D *lightDistrib = nullptr);

Spectrum EstimateDirect(const Interaction &it, const Point2f &uShading,
                        const Light &light, const Point2f &uLight,
                        const Scene &scene, Sampler &sampler,
                        Sampler &trans_sampler, MemoryArena &arena,
                        bool handleMedia = false, bool specular = false);

Spectrum EstimateDirect_ManualSig(const Interaction &it,
                                  const Point2f &uShading, const Light &light,
                                  const Point2f &uLight, const Scene &scene,
                                  Sampler &sampler, Sampler &trans_sampler,
                                  MemoryArena &arena, bool handleMedia = false,
                                  bool specular = false, Float sigma_t = 1.0);

Spectrum EstimateDirect_NonExp(const Interaction &it, const Point2f &uShading,
                               const Light &light, const Point2f &uLight,
                               const Scene &scene, Sampler &sampler,
                               Sampler &trans_sampler, MemoryArena &arena,
                               bool specular = false);

BlanchetCorrelatedResults EstimateDirect_NonExp_Blanchet(
    const Interaction &it, const Point2f &uShading, const Light &light,
    const Point2f &uLight, const Scene &scene, Sampler &sampler,
    Sampler &trans_sampler, MemoryArena &arena, int blanchet_n,
    bool specular = false);

BlanchetCorrelatedResults EstimateDirect_Blanchet(
    const Interaction &it, const Point2f &uShading, const Light &light,
    const Point2f &uLight, const Scene &scene, Sampler &sampler,
    Sampler &trans_sampler, MemoryArena &arena, int blanchet_n,
    bool handleMedia = false, bool specular = false);

std::unique_ptr<Distribution1D> ComputeLightPowerDistribution(
    const Scene &scene);

// SamplerIntegrator Declarations
class SamplerIntegrator : public Integrator {
  public:
    // SamplerIntegrator Public Methods
    SamplerIntegrator(std::shared_ptr<const Camera> camera,
                      std::shared_ptr<Sampler> sampler,
                      const Bounds2i &pixelBounds, long maxSeconds,
                      long maxExtCalls, uint32_t seedOne, uint32_t seedTwo,
                      uint32_t seedThree, std::string trans_sampler,
                      std::string ff_sampler, bool multithreaded)
        : camera(camera),
          sampler(sampler),
          pixelBounds(pixelBounds),
          maxSeconds(maxSeconds),
          maxExtCalls(maxExtCalls),
          seedOne(seedOne),
          seedTwo(seedTwo),
          seedThree(seedThree),
          trans_sampler(trans_sampler),
          ff_sampler(ff_sampler),
          multithreaded(multithreaded) {}

    virtual void Preprocess(const Scene &scene, Sampler &sampler) {}
    virtual void Render(const Scene &scene);
    virtual void Render_Mutable(Scene &scene);
    virtual Spectrum Li(const RayDifferential &ray, const Scene &scene,
                        Sampler &sampler, Sampler &trans_sampler,
                        Sampler &ff_sampler, MemoryArena &arena,
                        int depth = 0) const = 0;
    virtual Spectrum Li_Mutable(const RayDifferential &ray, Scene &scene,
                                Sampler &sampler, Sampler &trans_sampler,
                                Sampler &ff_sampler, MemoryArena &arena,
                                int depth = 0) const {
        return Li(ray, scene, sampler, trans_sampler, ff_sampler, arena, depth);
    }
    // TODO: remove this later when these experiments are no longer needed
    virtual Spectrum Li(const RayDifferential &ray, const Scene &scene,
                        Sampler &sampler, Sampler &trans_sampler,
                        Sampler &ff_sampler, MemoryArena &arena,
                        Point2i pix_loc, int depth = 0) const {
        return Li(ray, scene, sampler, trans_sampler, ff_sampler, arena, depth);
    }
    Spectrum SpecularReflect(const RayDifferential &ray,
                             const SurfaceInteraction &isect,
                             const Scene &scene, Sampler &sampler,
                             Sampler &trans_sampler, Sampler &ff_sampler,
                             MemoryArena &arena, int depth) const;
    Spectrum SpecularTransmit(const RayDifferential &ray,
                              const SurfaceInteraction &isect,
                              const Scene &scene, Sampler &sampler,
                              Sampler &trans_sampler, Sampler &ff_sampler,
                              MemoryArena &arena, int depth) const;

    virtual long getMaxSeconds() const { return maxSeconds; }
    virtual long getMaxExtCalls() const { return maxExtCalls; }

  protected:
    // SamplerIntegrator Protected Data
    std::shared_ptr<const Camera> camera;

    // private:
    // SamplerIntegrator --Private-- Now Protected Data
    std::shared_ptr<Sampler> sampler;
    const Bounds2i pixelBounds;
    const long maxSeconds;
    const long maxExtCalls;
    const std::string trans_sampler;
    const std::string ff_sampler;
    uint32_t seedOne, seedTwo, seedThree;
    bool multithreaded;
};

// Is this even used???
class BlanchetIntegrator : public SamplerIntegrator {
  public:
    BlanchetIntegrator(std::shared_ptr<const Camera> camera,
                       std::shared_ptr<Sampler> sampler,
                       const Bounds2i &pixelBounds, long maxSeconds,
                       long maxExtCalls, uint32_t seedOne, uint32_t seedTwo,
                       uint32_t seedThree, std::string trans_sampler,
                       std::string ff_sampler, bool multithreaded)
        : SamplerIntegrator(camera, sampler, pixelBounds, maxSeconds,
                            maxExtCalls, seedOne, seedTwo, seedThree,
                            trans_sampler, ff_sampler, multithreaded) {}

    virtual Spectrum Li_Bias(const RayDifferential &ray, const Scene &scene,
                             Sampler &sampler, Sampler &trans_sampler,
                             Sampler &ff_sampler, MemoryArena &arena,
                             int blanchet_n, int depth = 0) const = 0;

    void Render(const Scene &scene);
};

}  // namespace pbrt

#endif  // PBRT_CORE_INTEGRATOR_H
