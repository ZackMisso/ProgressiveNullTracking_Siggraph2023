// // integrators/volpath.cpp*
// #include "integrators/volpath_nonexp_debias.h"

// #include "blanchet_spectrum.h"
// #include "bssrdf.h"
// #include "camera.h"
// #include "film.h"
// #include "interaction.h"
// #include "media/heterogeneous.h"
// #include "paramset.h"
// #include "progressreporter.h"
// #include "scene.h"
// #include "stats.h"
// #include "trans_samplers/trans_halton.h"
// #include "trans_samplers/trans_hammersley.h"
// #include "trans_samplers/trans_independent.h"
// #include "trans_samplers/trans_latin_stream.h"

// namespace pbrt {

// STAT_INT_DISTRIBUTION("Integrator/Path length", pathLength);
// STAT_COUNTER("Integrator/Volume interactions", volumeInteractions);
// STAT_COUNTER("Integrator/Surface interactions", surfaceInteractions);

// // VolPathIntegrator Method Definitions
// void VolPathNonExpDebiasIntegrator::Preprocess(const Scene& scene,
//                                                Sampler& sampler) {
//     lightDistribution =
//         CreateLightSampleDistribution(lightSampleStrategy, scene);
// }

// BlanchetCorrelatedResults VolPathNonExpDebiasIntegrator::Li(
//     const RayDifferential& r, const Scene& scene, Sampler& sampler,
//     Sampler& trans_sampler, Sampler& ff_sampler, MemoryArena& arena,
//     int blanchet_n, int depth, int maxBounces, BlanchetCorrelatedResults
//     beta, Float etaScale, bool specularBounce) const { ProfilePhase
//     p(Prof::SamplerIntegratorLi); BlanchetCorrelatedResults L(0.f); beta =
//     BlanchetCorrelatedResults(1.f); RayDifferential ray(r); specularBounce =
//     false; int bounces;

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
//     etaScale = 1;

//     for (bounces = 0;; ++bounces) {
//         // Intersect _ray_ with scene and store intersection in _isect_
//         SurfaceInteraction isect;
//         bool foundIntersection = scene.Intersect(ray, &isect);

//         // Sample the participating medium, if present
//         MediumInteraction mi;
//         uint32_t tempFlags = vflags;
//         setVertex1Type(CustomVertexType::VERTEX_MEDIUM, tempFlags);
//         if (ray.medium) {
//             // std::cout << "sampling " << std::endl;
//             beta *= ray.medium->Sample_Blanchet(
//                 ray, ff_sampler, trans_sampler, arena, &mi, tempFlags,
//                 blanchet_n, TransportMode::Radiance);
//             // std::cout << "finished sampling " << std::endl;
//         }

//         if (beta.IsBlack()) break;

//         // Handle an interaction with a medium or a surface
//         if (mi.IsValid()) {
//             // Terminate path if ray escaped or _maxDepth_ was reached
//             if (bounces >= maxDepth) break;

//             ++volumeInteractions;
//             // Handle scattering at point in medium for volumetric path
//             tracer const Distribution1D* lightDistrib =
//                 lightDistribution->Lookup(mi.p);
//             L += beta *
//                  UniformSampleAllLights_NonExp_Blanchet(
//                      mi, scene, arena, sampler, trans_sampler, blanchet_n);

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
//                     for (const auto& light : scene.infiniteLights)
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
//             const Distribution1D* lightDistrib =
//                 lightDistribution->Lookup(isect.p);

//             // std::cout << "Sampling one light" << std::endl;
//             // Spectrum lt_samp = UniformSampleOneLight_NonExp(isect,
//             //                                          scene,
//             //                                          arena,
//             //                                          sampler,
//             //                                          trans_sampler,
//             //                                          lightDistrib);

//             // std::cout << "light: " << lt_samp << std::endl;

//             L += beta *
//                  UniformSampleAllLights_NonExp_Blanchet(
//                      isect, scene, arena, sampler, trans_sampler,
//                      blanchet_n);

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
//                 L += beta *
//                      UniformSampleAllLights_NonExp_Blanchet(
//                          pi, scene, arena, sampler, trans_sampler,
//                          blanchet_n);

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
//         Spectrum rrBeta = beta.bias * etaScale;

// #if NO_RUSSIAN_ROULETTE
//         continue;
// #endif

//         if (rrBeta.MaxComponentValue() < rrThreshold && bounces > 3) {
//             Float q = std::max((Float).05, 1 - rrBeta.MaxComponentValue());
//             if (sampler.Get1D() < q) break;
//             beta /= 1 - q;
//         }
//     }

//     ReportValue(pathLength, bounces);
//     return L;
// }

// Spectrum VolPathNonExpDebiasIntegrator::Li(
//     const RayDifferential& ray, const Scene& scene, Sampler& sampler,
//     Sampler& trans_sampler, Sampler& ff_sampler, MemoryArena& arena,
//     int blanchet_n, Float blanchet_pmf, int depth) const {
//     // std::cout << "First run" << std::endl;
//     BlanchetCorrelatedResults coor_results =
//         Li(ray, scene, sampler, trans_sampler, ff_sampler, arena, blanchet_n,
//            maxDepth, maxDepth, Spectrum(1.0), 1.0, false);

//     // return coor_results.bias;
//     // return coor_results.even;
//     // return coor_results.all;

//     // return coor_results.all;

//     return (coor_results.all - (0.5 * (coor_results.even +
//     coor_results.odd))) /
//                blanchet_pmf +
//            coor_results.bias;
// }

// void VolPathNonExpDebiasIntegrator::Render(const Scene& scene) {
//     // std::cout << "INSIDE RENDER" << std::endl;
//     Preprocess(scene, *sampler);
//     // Render image tiles in parallel

//     // Compute number of tiles, _nTiles_, to use for parallel rendering
//     Bounds2i sampleBounds = camera->film->GetSampleBounds();
//     Vector2i sampleExtent = sampleBounds.Diagonal();

//     int tileSize;
//     if (multithreaded) {
//         tileSize = 16;
//     } else {
//         tileSize = 16;
//     }
//     tileSize = 16;

//     Point2i nTiles((sampleExtent.x + tileSize - 1) / tileSize,
//                    (sampleExtent.y + tileSize - 1) / tileSize);

//     std::cout << "NTILES X: " << nTiles[0] << std::endl;
//     std::cout << "NTILES Y: " << nTiles[1] << std::endl;
//     std::cout << "TileSize: " << tileSize << std::endl;

//     std::unique_ptr<Sampler> tileSampler;
//     std::unique_ptr<Sampler> transSampler;
//     std::unique_ptr<Sampler> ffSampler;

//     if (!multithreaded) {
//         std::cout << "MULT OFF" << std::endl;
//         // seedOne += 0xaf43fab;
//         // seedTwo += 0x41c3d29;
//         // seedThree += 0x1cbaf45;

//         // seedOne += 0x97acb32;
//         // seedTwo += 0xacdc439;
//         // seedThree += 0x6969420;
//     }

//     // seedOne += 0x47dcb32;
//     // seedTwo += 0x2cdc4a9;
//     // seedThree += 0x4206969;
//     //
//     // seedOne += 0x47dcb32;
//     // seedTwo += 0x2cdc4a9;
//     // seedThree += 0x4206969;
//     //
//     // seedOne += 0x47dcb32;
//     // seedTwo += 0x2cdc4a9;
//     // seedThree += 0x4206969;

//     tileSampler = sampler->Clone(seedOne);

//     // TODO: make the other sampling methods work with this
//     if (trans_sampler == "independent") {
//         transSampler = std::unique_ptr<Sampler>(
//             new Trans_Independent(sampler->samplesPerPixel, seedTwo));
//     } else if (trans_sampler == "stream") {
//         transSampler =
//             std::unique_ptr<Sampler>(new Trans_LatinHyperCubes_Stream(
//                 sampler->samplesPerPixel, seedTwo));
//     } else {
//         std::cout << "Warning: Not Supported Transmittance Sampler,
//         Defaulting "
//                      "to Independent"
//                   << std::endl;
//         transSampler = std::unique_ptr<Sampler>(
//             new Trans_Independent(sampler->samplesPerPixel, seedTwo));
//     }

//     if (ff_sampler == "independent") {
//         ffSampler = std::unique_ptr<Sampler>(
//             new Trans_Independent(sampler->samplesPerPixel, seedThree));

//     } else if (ff_sampler == "stream") {
//         ffSampler = std::unique_ptr<Sampler>(new
//         Trans_LatinHyperCubes_Stream(
//             sampler->samplesPerPixel, seedThree));

//     } else {
//         std::cout << "Warning: Not Supported Free Flight Sampler, Defaulting
//         "
//                      "to Independent"
//                   << std::endl;
//         ffSampler = std::unique_ptr<Sampler>(
//             new Trans_Independent(sampler->samplesPerPixel, seedThree));
//     }

//     uint64_t totalWork;

//     if (maxExtCalls > 0) {
//         totalWork = maxExtCalls;
//     } else {
//         totalWork = nTiles.x * nTiles.y * sampler->samplesPerPixel;
//     }

//     ProgressReporter reporter(totalWork, "Rendering", getMaxSeconds());

//     int i = 0;
//     int finished_location = -1;

//     while (i < sampler->samplesPerPixel) {
//         // std::cout << HeterogeneousMedium::nDensityCalls << std::endl;
//         if (!reporter.continueRun(maxExtCalls)) {
//             finished_location = i;
//             i = sampler->samplesPerPixel;
//         }
//         {
//             // choose an n
//             // need to sample a blanchet_n
//             Float val = tileSampler->Get1D();
//             // Float pmf = r * pow(1.0 - r, 0);
//             Float r = 0.65;
//             Float blanchet_pmf = r;
//             Float cmf = blanchet_pmf;
//             int blanchet_n = 0;

//             while (cmf < val) {
//                 ++blanchet_n;
//                 blanchet_pmf *= 1.0 - r;
//                 cmf += blanchet_pmf;
//             }

//             // std::cout << "blanchet_n: " << blanchet_n << std::endl;

//             ParallelFor2D(
//                 [&](Point2i tile) {
//                     // Render section of image corresponding to _tile_

//                     // stop running if the time is off
//                     if (!reporter.continueRun(maxExtCalls)) return;

//                     // Allocate _MemoryArena_ for tile
//                     MemoryArena arena;

//                     // Get sampler instance for tile
//                     int instanceIndex = tile.y * nTiles.x + tile.x;

//                     // Compute sample bounds for tile
//                     int x0 = sampleBounds.pMin.x + tile.x * tileSize;
//                     int x1 = std::min(x0 + tileSize, sampleBounds.pMax.x);
//                     int y0 = sampleBounds.pMin.y + tile.y * tileSize;
//                     int y1 = std::min(y0 + tileSize, sampleBounds.pMax.y);

//                     Bounds2i tileBounds(Point2i(x0, y0), Point2i(x1, y1));

// #if LOGGING
//                     LOG(INFO) << "Starting image tile " << tileBounds;
// #endif

//                     // Get _FilmTile_ for tile
//                     std::unique_ptr<FilmTile> filmTile =
//                         camera->film->GetFilmTile(tileBounds);

//                     // Loop over pixels in tile to render them
//                     for (Point2i pixel : tileBounds) {
//                         // Do this check after the StartPixel() call; this
//                         keeps
//                         // the usage of RNG values from (most) Samplers that
//                         use
//                         // RNGs consistent, which improves reproducability /
//                         // debugging.
//                         if (!InsideExclusive(pixel, pixelBounds)) continue;

//                         // debug 224, 296
//                         // if (pixel.x < 220 || pixel.x > 232 ||
//                         //     pixel.y < 288 || pixel.y > 300) continue;

//                         // exp
//                         // if (pixel.x != 384 || pixel.y != 151) continue;
//                         // 0.25
//                         // if (pixel.x != 328 || pixel.y != 224) continue;
//                         // 0.50
//                         // if (pixel.x != 345 || pixel.y != 328) continue;
//                         // 0.75
//                         if (pixel.x != 263 || pixel.y != 389) continue;

//                         // std::cout << "Pixel: (" << pixel[0] << ", " <<
//                         // pixel[1] << ")" << std::endl;

//                         tileSampler->StartPixel(pixel);
//                         transSampler->StartPixel(pixel, sampleExtent.x,
//                                                  sampleExtent.y, 1);
//                         ffSampler->StartPixel(pixel, sampleExtent.x,
//                                                  sampleExtent.y, 2);

//                         // Initialize _CameraSample_ for current sample
//                         CameraSample cameraSample =
//                             tileSampler->GetCameraSample(pixel);

//                         // Generate camera ray for current sample
//                         RayDifferential ray;
//                         Float rayWeight =
//                             camera->GenerateRayDifferential(cameraSample,
//                             &ray);
//                         ray.ScaleDifferentials(
//                             1.0 /
//                             std::sqrt((Float)tileSampler->samplesPerPixel));
//                         // ++nCameraRays;

//                         // Evaluate radiance along camera ray
//                         Spectrum L(0.f);
//                         // std::cout << pixel.x << std::endl;
//                         if (rayWeight > 0)
//                             L = Li(ray, scene, *tileSampler, *transSampler,
//                                    *ffSampler, arena, blanchet_n,
//                                    blanchet_pmf, 0);
//                         // std::cout << "OUT" << std::endl;

//                         // if (rayWeight > 0) L = Li(ray,
//                         //                           scene,
//                         //                           *tileSampler,
//                         //                           *transSampler,
//                         //                           *ffSampler,
//                         //                           arena);
//                         //
//                         // else
//                         // {
//                         //     std::cout << "Ray Weight < 0" << std::endl;
//                         // }

//                         // Issue warning if unexpected radiance value
//                         returned if (L.HasNaNs()) {
//                             // #if LOGGING
//                             //     VLOG(ERROR) << StringPrintf(
//                             //         "Not-a-number radiance value returned
//                             "
//                             //         "for pixel (%d, %d), sample %d.
//                             Setting
//                             //         to black.", pixel.x, pixel.y,
//                             //
//                             (int)tileSamplers[instanceIndex]->CurrentSampleNumber());
//                             // #endif
//                             L = Spectrum(0.f);
//                         } else if (L.y() < -1e-5) {
// #if LOGGING
//                         // LOG(ERROR) << StringPrintf(
//                         //     "Negative luminance value, %f, returned "
//                         //     "for pixel (%d, %d), sample %d. Setting to
//                         //     black.", L.y(), pixel.x, pixel.y,
//                         //     (int)tileSampler->CurrentSampleNumber());
// #endif
//                             // std::cout << "here with a question mark" <<
//                             // std::endl; std::cout << L[0] << std::endl; L =
//                             // Spectrum(0.f); // THIS WAS THE BUG
//                         } else if (std::isinf(L.y())) {
//                             std::cout << "ERROR: INF" << std::endl;
//                             // #if LOGGING
//                             //     LOG(ERROR) << StringPrintf(
//                             //     "Infinite luminance value returned "
//                             //     "for pixel (%d, %d), sample %d. Setting to
//                             //     black.", pixel.x, pixel.y,
//                             //
//                             (int)tileSamplers[instanceIndex]->CurrentSampleNumber());
//                             // #endif
//                             L = Spectrum(0.f);
//                         }
// #if LOGGING
//                         VLOG(1) << "Camera sample: " << cameraSample
//                                 << " -> ray: " << ray << " -> L = " << L;
// #endif

//                         // Add camera ray's contribution to image
//                         filmTile->AddSample(cameraSample.pFilm, L,
//                         rayWeight);

//                         // Free _MemoryArena_ memory from computing image
//                         sample
//                         // value
//                         arena.Reset();
//                     }
// #if LOGGING
//                     LOG(INFO) << "Finished image tile " << tileBounds;
// #endif

//                     // Merge image tile into _Film_
//                     if (reporter.continueRun(maxExtCalls)) {
//                         camera->film->MergeFilmTile(std::move(filmTile));
//                         if (maxExtCalls > 0) {
//                             reporter.UpdateExact(
//                                 HeterogeneousMedium::nDensityCalls);
//                         } else {
//                             reporter.Update();
//                         }
//                     }
//                 },
//                 nTiles);
//         }

//         tileSampler->StartNextSample();
//         transSampler->StartNextSample();
//         ffSampler->StartNextSample();
//         i++;

//         if (reporter.continueRun(maxExtCalls)) {
//             if (is_power_of_2(i)) {
//                 long currentTime = reporter.getCurrentTime();
//                 camera->film->WriteImage(currentTime, i);
//             }
//         }
//     }
//     reporter.Done();

// #if LOGGING
//     LOG(INFO) << "Rendering finished";
// #endif

//     // Save final image after rendering
//     long currentTime = reporter.getCurrentTime();
//     camera->film->WriteImage(currentTime, -1, finished_location);
// }

// VolPathNonExpDebiasIntegrator* CreateVolPathNonExpDebiasIntegrator(
//     const ParamSet& params, std::shared_ptr<Sampler> sampler,
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
//     const int* pb = params.FindInt("pixelbounds", &np);
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
//     return new VolPathNonExpDebiasIntegrator(
//         maxDepth, camera, sampler, pixelBounds, maxSeconds, maxExtCalls,
//         seedOne, seedTwo, seedThree, trans_sampler, ff_sampler, rrThreshold,
//         lightStrategy, multithreaded);
// }

// }  // namespace pbrt
