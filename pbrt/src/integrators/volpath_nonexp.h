// #if defined(_MSC_VER)
// #define NOMINMAX
// #pragma once
// #endif

// #ifndef PBRT_INTEGRATORS_VOLPATH_NONEXP_H
// #define PBRT_INTEGRATORS_VOLPATH_NONEXP_H

// // integrators/volpath.h*
// #include "integrator.h"
// #include "lightdistrib.h"
// #include "pbrt.h"

// namespace pbrt {

// // VolPathIntegrator Declarations
// class VolPathNonExpIntegrator : public SamplerIntegrator {
//   public:
//     // VolPathIntegrator Public Methods
//     VolPathNonExpIntegrator(int maxDepth, std::shared_ptr<const Camera>
//     camera,
//                             std::shared_ptr<Sampler> sampler,
//                             const Bounds2i &pixelBounds, long maxSeconds,
//                             long maxExtCalls, uint32_t seedOne,
//                             uint32_t seedTwo, uint32_t seedThree,
//                             std::string trans_sampler = "independent",
//                             std::string ff_sampler = "independent",
//                             Float rrThreshold = 1,
//                             const std::string &lightSampleStrategy =
//                             "spatial", bool multithreaded = false)
//         : SamplerIntegrator(camera, sampler, pixelBounds, maxSeconds,
//                             maxExtCalls, seedOne, seedTwo, seedThree,
//                             trans_sampler, ff_sampler, multithreaded),
//           maxDepth(maxDepth),
//           rrThreshold(rrThreshold),
//           lightSampleStrategy(lightSampleStrategy) {}

//     void Preprocess(const Scene &scene, Sampler &sampler);

//     Spectrum Li(const RayDifferential &ray, const Scene &scene,
//                 Sampler &sampler, Sampler &trans_sampler, Sampler
//                 &ff_sampler, MemoryArena &arena, int depth) const;

//   private:
//     // VolPathIntegrator Private Data
//     const int maxDepth;
//     const Float rrThreshold;
//     const std::string lightSampleStrategy;
//     std::unique_ptr<LightDistribution> lightDistribution;
// };

// VolPathNonExpIntegrator *CreateVolPathNonExpIntegrator(
//     const ParamSet &params, std::shared_ptr<Sampler> sampler,
//     std::shared_ptr<const Camera> camera);

// }  // namespace pbrt

// #endif  // PBRT_INTEGRATORS_VOLPATH_H
