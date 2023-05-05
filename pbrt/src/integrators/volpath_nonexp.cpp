// // integrators/volpath.cpp*
// #include "integrators/volpath_nonexp.h"

// #include "bssrdf.h"
// #include "camera.h"
// #include "film.h"
// #include "interaction.h"
// #include "paramset.h"
// #include "scene.h"
// #include "stats.h"
// namespace pbrt {

// STAT_INT_DISTRIBUTION("Integrator/Path length", pathLength);
// STAT_COUNTER("Integrator/Volume interactions", volumeInteractions);
// STAT_COUNTER("Integrator/Surface interactions", surfaceInteractions);

// // VolPathIntegrator Method Definitions
// void VolPathNonExpIntegrator::Preprocess(const Scene &scene, Sampler
// &sampler) {
//     lightDistribution =
//         CreateLightSampleDistribution(lightSampleStrategy, scene);
// }

// Spectrum VolPathNonExpIntegrator::Li(const RayDifferential &r,
//                                      const Scene &scene, Sampler &sampler,
//                                      Sampler &trans_sampler,
//                                      Sampler &ff_sampler, MemoryArena &arena,
//                                      int depth) const {
//     ProfilePhase p(Prof::SamplerIntegratorLi);
//     Spectrum L(0.f), beta(1.f);
//     RayDifferential ray(r);
//     bool specularBounce = false;
//     int bounces;

//     // Start setting the flags
//     uint32_t vflags = 0;
//     resetVertex(vflags);
//     setVertex0Type(CustomVertexType::VERTEX_SURFACE, vflags);
//     setVertex0Camera(vflags);

//     // Added after book publication: etaScale tracks the accumulated effect
//     // of radiance scaling due to rays passing through refractive
//     // boundaries (see the derivation on p. 527 of the third edition). We
//     // track this value in order to remove it from beta when we apply
//     // Russian roulette; this is worthwhile, since it lets us sometimes
//     // avoid terminating refracted rays that are about to be refracted back
//     // out of a medium and thus have their beta value increased.
//     Float etaScale = 1;

//     for (bounces = 0;; ++bounces) {
//         // Intersect _ray_ with scene and store intersection in _isect_
//         SurfaceInteraction isect;
//         bool foundIntersection = scene.Intersect(ray, &isect);

//         // Sample the participating medium, if present
//         MediumInteraction mi;
//         uint32_t tempFlags = vflags;
//         setVertex1Type(CustomVertexType::VERTEX_MEDIUM, tempFlags);
//         if (ray.medium) {
//             beta *= ray.medium->Sample(ray, ff_sampler, trans_sampler, arena,
//                                        &mi, tempFlags,
//                                        TransportMode::Radiance);
//         }

//         if (beta.IsBlack()) break;

//         // Handle an interaction with a medium or a surface
//         if (mi.IsValid()) {
//             // Terminate path if ray escaped or _maxDepth_ was reached
//             if (bounces >= maxDepth) break;

//             ++volumeInteractions;
//             // Handle scattering at point in medium for volumetric path
//             tracer const Distribution1D *lightDistrib =
//                 lightDistribution->Lookup(mi.p);
//             // L += beta * UniformSampleOneLight_NonExp(mi,
//             //                                          scene,
//             //                                          arena,
//             //                                          sampler,
//             //                                          trans_sampler,
//             //                                          lightDistrib);

//             L += beta * UniformSampleAllLights_NonExp(mi, scene, arena,
//             sampler,
//                                                       trans_sampler, true);

//             // std::cout << "media" << std::endl;
//             // throw new std::exception();

//             // update the segment type
//             resetVertex(vflags);
//             setVertex0Type(CustomVertexType::VERTEX_MEDIUM, vflags);
//             Vector3f wo = -ray.d, wi;
//             mi.phase->Sample_p(wo, &wi, sampler.Get2D());
//             ray = mi.SpawnRay(wi);
//             specularBounce = false;
//         } else {
//             ++surfaceInteractions;
//             // Handle scattering at point on surface for volumetric path
//             tracer resetVertex(vflags);
//             setVertex0Type(CustomVertexType::VERTEX_SURFACE, vflags);

//             // Possibly add emitted light at intersection
//             if (bounces == 0 || specularBounce) {
//                 // Add emitted light at path vertex or from the environment
//                 if (foundIntersection)
//                     L += beta * isect.Le(-ray.d);
//                 else
//                     for (const auto &light : scene.infiniteLights)
//                         L += beta * light->Le(ray);
//             }

//             // Terminate path if ray escaped or _maxDepth_ was reached
//             if (!foundIntersection || bounces >= maxDepth) break;

//             // Compute scattering functions and skip over medium boundaries
//             isect.ComputeScatteringFunctions(ray, arena, true);

//             if (!isect.bsdf)  // null bsdf
//             {
//                 ray = isect.SpawnRay(ray.d);
//                 bounces--;
//                 if (bounces == -1) {
//                     setVertex0Camera(vflags);  // camera segment
//                 }
//                 continue;
//             }

//             // Sample illumination from lights to find attenuated path
//             // contribution
//             const Distribution1D *lightDistrib =
//                 lightDistribution->Lookup(isect.p);

//             // std::cout << "Sampling one light" << std::endl;
//             // Spectrum lt_samp = UniformSampleOneLight_NonExp(isect,
//             //                                          scene,
//             //                                          arena,
//             //                                          sampler,
//             //                                          trans_sampler,
//             //                                          lightDistrib);

//             // std::cout << "light: " << lt_samp << std::endl;

//             L += beta * UniformSampleAllLights_NonExp(
//                             isect, scene, arena, sampler, trans_sampler,
//                             true);

//             // L += beta * UniformSampleOneLight_NonExp(isect,
//             //                                          scene,
//             //                                          arena,
//             //                                          sampler,
//             //                                          trans_sampler,
//             //                                          lightDistrib);

//             // throw new std::exception();

//             // Sample BSDF to get new path direction
//             Vector3f wo = -ray.d, wi;
//             Float pdf;
//             BxDFType flags;
//             Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf,
//                                               BSDF_ALL, &flags);

//             if (f.IsBlack() || pdf == 0.f) break;

//             beta *= f * AbsDot(wi, isect.shading.n) / pdf;

//             specularBounce = (flags & BSDF_SPECULAR) != 0;

//             if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION)) {
//                 Float eta = isect.bsdf->eta;
//                 // Update the term that tracks radiance scaling for
//                 refraction
//                 // depending on whether the ray is entering or leaving the
//                 // medium.
//                 etaScale *=
//                     (Dot(wo, isect.n) > 0) ? (eta * eta) : 1 / (eta * eta);
//             }
//             ray = isect.SpawnRay(wi);

//             // Account for attenuated subsurface scattering, if applicable
//             if (isect.bssrdf && (flags & BSDF_TRANSMISSION)) {
//                 // Importance sample the BSSRDF
//                 SurfaceInteraction pi;
//                 Spectrum S = isect.bssrdf->Sample_S(
//                     scene, sampler.Get1D(), sampler.Get2D(), arena, &pi,
//                     &pdf);

//                 if (S.IsBlack() || pdf == 0) break;
//                 beta *= S / pdf;

//                 // Account for the attenuated direct subsurface scattering
//                 // component

//                 L += beta * UniformSampleAllLights_NonExp(
//                                 pi, scene, arena, sampler, trans_sampler,
//                                 true);
//                 // L += beta *
//                 //      UniformSampleOneLight_NonExp(pi,
//                 //                                   scene,
//                 //                                   arena,
//                 //                                   sampler,
//                 //                                   trans_sampler,
//                 // lightDistribution->Lookup(pi.p));

//                 // Account for the indirect subsurface scattering component
//                 Spectrum f = pi.bsdf->Sample_f(pi.wo, &wi, sampler.Get2D(),
//                                                &pdf, BSDF_ALL, &flags);

//                 if (f.IsBlack() || pdf == 0) break;
//                 beta *= f * AbsDot(wi, pi.shading.n) / pdf;

//                 specularBounce = (flags & BSDF_SPECULAR) != 0;
//                 ray = pi.SpawnRay(wi);

//                 vflags = 0;
//                 setVertex0Type(CustomVertexType::VERTEX_SURFACE, vflags);
//             }
//         }

//         // Possibly terminate the path with Russian roulette
//         // Factor out radiance scaling due to refraction in rrBeta.
//         Spectrum rrBeta = beta * etaScale;

//         #if NO_RUSSIAN_ROULETTE
//             continue;
//         #endif

//         if (rrBeta.MaxComponentValue() < rrThreshold && bounces > 3) {
//             Float q = std::max((Float).05, 1 - rrBeta.MaxComponentValue());
//             if (sampler.Get1D() < q) break;
//             beta /= 1 - q;
//         }
//     }

//     ReportValue(pathLength, bounces);
//     return L;
// }

// VolPathNonExpIntegrator *CreateVolPathNonExpIntegrator(
//     const ParamSet &params, std::shared_ptr<Sampler> sampler,
//     std::shared_ptr<const Camera> camera) {
//     int maxDepth = params.FindOneInt("maxdepth", 5);
//     long maxSeconds = (long)params.FindOneInt("maxSeconds", 100000);
//     long maxExtCalls = (long)params.FindOneLong("maxExtCalls", -1);
//     std::string trans_sampler =
//         params.FindOneString("trans_sampler", "independent");
//     std::string ff_sampler = params.FindOneString("ff_sampler",
//     "independent"); bool ff_correlated = params.FindOneBool("ff_correlated",
//     false); bool multithreaded = params.FindOneBool("multithreaded", false);
//     int np;
//     const int *pb = params.FindInt("pixelbounds", &np);
//     Bounds2i pixelBounds = camera->film->GetSampleBounds();
//     if (pb) {
//         if (np != 4)
//             Error("Expected four values for \"pixelbounds\" parameter. Got
//             %d.",
//                   np);
//         else {
//             pixelBounds = Intersect(pixelBounds,
//                                     Bounds2i{{pb[0], pb[2]}, {pb[1],
//                                     pb[3]}});
//             if (pixelBounds.Area() == 0)
//                 Error("Degenerate \"pixelbounds\" specified.");
//         }
//     }
//     Float rrThreshold = params.FindOneFloat("rrthreshold", 1.);
//     uint32_t seedOne = uint32_t(params.FindOneInt("seedOne", 0xaf43fab));
//     uint32_t seedTwo = uint32_t(params.FindOneInt("seedTwo", 0x41c3d29));
//     uint32_t seedThree = uint32_t(params.FindOneInt("seedThree", 0x1cbaf45));
//     // std::string lightStrategy =
//     //     params.FindOneString("lightsamplestrategy", "spatial");
//     std::string lightStrategy =
//         params.FindOneString("lightsamplestrategy", "uniform");
//     return new VolPathNonExpIntegrator(
//         maxDepth, camera, sampler, pixelBounds, maxSeconds, maxExtCalls,
//         seedOne, seedTwo, seedThree, trans_sampler, ff_sampler, rrThreshold,
//         lightStrategy, multithreaded);
// }

// }  // namespace pbrt
