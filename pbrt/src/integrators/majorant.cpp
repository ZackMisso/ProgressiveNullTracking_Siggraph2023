
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

// integrators/volpath.cpp*
#include "integrators/majorant.h"

#include "bssrdf.h"
#include "camera.h"
#include "film.h"
#include "interaction.h"
#include "media/heterogeneous.h"
#include "paramset.h"
#include "progressreporter.h"
#include "scene.h"
#include "stats.h"
#include "trans_samplers/trans_halton.h"
#include "trans_samplers/trans_hammersley.h"
#include "trans_samplers/trans_independent.h"
#include "trans_samplers/trans_latin_stream.h"

namespace pbrt {

STAT_INT_DISTRIBUTION("Integrator/Path length", pathLength);
STAT_COUNTER("Integrator/Volume interactions", volumeInteractions);
STAT_COUNTER("Integrator/Surface interactions", surfaceInteractions);

// MajorantIntegrator Method Definitions
void MajorantIntegrator::Preprocess(const Scene &scene, Sampler &sampler) {
    lightDistribution =
        CreateLightSampleDistribution(lightSampleStrategy, scene);
}

Spectrum MajorantIntegrator::Li(const RayDifferential &r, const Scene &scene,
                                Sampler &sampler, Sampler &trans_sampler,
                                Sampler &ff_sampler, MemoryArena &arena,
                                int depth) const {
    ProfilePhase p(Prof::SamplerIntegratorLi);
    Spectrum L(0.f), beta(1.f);
    RayDifferential ray(r);
    bool specularBounce = false;
    int bounces;
    // Added after book publication: etaScale tracks the accumulated effect
    // of radiance scaling due to rays passing through refractive
    // boundaries (see the derivation on p. 527 of the third edition). We
    // track this value in order to remove it from beta when we apply
    // Russian roulette; this is worthwhile, since it lets us sometimes
    // avoid terminating refracted rays that are about to be refracted back
    // out of a medium and thus have their beta value increased.
    Float etaScale = 1;

    for (bounces = 0;; ++bounces) {
        // Intersect _ray_ with scene and store intersection in _isect_
        SurfaceInteraction isect;
        bool foundIntersection = scene.Intersect(ray, &isect);

        // Sample the participating medium, if present
        MediumInteraction mi;
        if (ray.medium) {
            // ray.medium->InitializeSamplers(&trans_sampler, &ff_sampler);
            beta *=
                ray.medium->Sample(ray, ff_sampler, trans_sampler, arena, &mi);

            // return ray.medium->Majorant(ray);

            // TODO: figure out how to implement this for eqi
            // if (ray.medium->FFSampleRequiresTrCall() && mi.IsValid())
            // {
            //     beta *= ray.medium->LocalTr(mi.ray, sampler, mi.tMin,
            //     mi.tMax);
            // }
        }
        if (beta.IsBlack()) break;  // this

        // Handle an interaction with a medium or a surface
        if (mi.IsValid()) {
            // Terminate path if ray escaped or _maxDepth_ was reached
            if (bounces >= maxDepth) break;

            ++volumeInteractions;
            // Handle scattering at point in medium for volumetric path tracer
            const Distribution1D *lightDistrib =
                lightDistribution->Lookup(mi.p);
            // L +=
            //     beta * UniformSampleOneLight(mi, scene, arena, sampler,
            //                                  trans_sampler, true,
            //                                  lightDistrib);

            L += beta * UniformSampleAllLights(mi, scene, arena, sampler,
                                               trans_sampler, true);

            Vector3f wo = -ray.d, wi;
            mi.phase->Sample_p(wo, &wi, sampler.Get2D());
            ray = mi.SpawnRay(wi);
            specularBounce = false;
        } else {
            ++surfaceInteractions;
            // Handle scattering at point on surface for volumetric path tracer

            // Possibly add emitted light at intersection
            if (bounces == 0 || specularBounce) {
                // Add emitted light at path vertex or from the environment
                if (foundIntersection)
                    L += beta * isect.Le(-ray.d);
                else
                    for (const auto &light : scene.infiniteLights)
                        L += beta * light->Le(ray);
            }

            // std::cout << "wooo: " << L << std::endl;

            // Terminate path if ray escaped or _maxDepth_ was reached
            if (!foundIntersection || bounces >= maxDepth) break;

            // std::cout << "wooo2: " << L << std::endl;

            // Compute scattering functions and skip over medium boundaries
            isect.ComputeScatteringFunctions(ray, arena, true);
            if (!isect.bsdf) {
                ray = isect.SpawnRay(ray.d);
                bounces--;
                // std::cout << "wooo3" << std::endl;
                continue;
            }

            // std::cout << "beta: " << beta << std::endl;

            // std::cout << "wooo" << std::endl;

            // Sample illumination from lights to find attenuated path
            // contribution
            const Distribution1D *lightDistrib =
                lightDistribution->Lookup(isect.p);
            // L +=
            //     beta * UniformSampleOneLight(isect, scene, arena, sampler,
            //                                  trans_sampler, true,
            //                                  lightDistrib);

            L += beta * UniformSampleAllLights(isect, scene, arena, sampler,
                                               trans_sampler, true);

            // std::cout << "awooop: " << L << std::endl;

            // Sample BSDF to get new path direction
            Vector3f wo = -ray.d, wi;
            Float pdf;
            BxDFType flags;
            Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf,
                                              BSDF_ALL, &flags);
            if (f.IsBlack() || pdf == 0.f) break;
            // std::cout << "wooo" << std::endl;
            // std::cout << isect.shading.n << std::endl;
            beta *= f * AbsDot(wi, isect.shading.n) / pdf;
#if LOGGING
            DCHECK(std::isinf(beta.y()) == false);
#endif
            specularBounce = (flags & BSDF_SPECULAR) != 0;
            if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION)) {
                Float eta = isect.bsdf->eta;
                // Update the term that tracks radiance scaling for refraction
                // depending on whether the ray is entering or leaving the
                // medium.
                etaScale *=
                    (Dot(wo, isect.n) > 0) ? (eta * eta) : 1 / (eta * eta);
            }
            ray = isect.SpawnRay(wi);

            // Account for attenuated subsurface scattering, if applicable
            if (isect.bssrdf && (flags & BSDF_TRANSMISSION)) {
                // std::cout << "bssrdf fuff" << std::endl;
                // std::cout << "wooo 4" << std::endl;
                // Importance sample the BSSRDF
                SurfaceInteraction pi;
                Spectrum S = isect.bssrdf->Sample_S(
                    scene, sampler.Get1D(), sampler.Get2D(), arena, &pi, &pdf);
#if LOGGING
                DCHECK(std::isinf(beta.y()) == false);
#endif
                if (S.IsBlack() || pdf == 0) break;
                beta *= S / pdf;

                // Account for the attenuated direct subsurface scattering
                // component
                // L += beta * UniformSampleOneLight(
                //                 pi, scene, arena, sampler, trans_sampler,
                //                 true, lightDistribution->Lookup(pi.p));

                L += beta * UniformSampleAllLights(pi, scene, arena, sampler,
                                                   trans_sampler, true);

                // Account for the indirect subsurface scattering component
                Spectrum f = pi.bsdf->Sample_f(pi.wo, &wi, sampler.Get2D(),
                                               &pdf, BSDF_ALL, &flags);
                if (f.IsBlack() || pdf == 0) break;
                beta *= f * AbsDot(wi, pi.shading.n) / pdf;
#if LOGGING
                DCHECK(std::isinf(beta.y()) == false);
#endif
                specularBounce = (flags & BSDF_SPECULAR) != 0;
                ray = pi.SpawnRay(wi);
            }
        }

        // Possibly terminate the path with Russian roulette
        // Factor out radiance scaling due to refraction in rrBeta.
        Spectrum rrBeta = beta * etaScale;
        if (rrBeta.MaxComponentValue() < rrThreshold && bounces > 3) {
            Float q = std::max((Float).05, 1 - rrBeta.MaxComponentValue());
            if (sampler.Get1D() < q) break;
            beta /= 1 - q;
#if LOGGING
            DCHECK(std::isinf(beta.y()) == false);
#endif
        }
    }

    ReportValue(pathLength, bounces);
    return L;
}

void MajorantIntegrator::Render_Mutable(Scene &scene) {
    // std::cout << "STARTING HERE" << std::endl;
    // return;
    Preprocess(scene, *sampler);
    // Render image tiles in parallel

    // Compute number of tiles, _nTiles_, to use for parallel rendering
    Bounds2i sampleBounds = camera->film->GetSampleBounds();
    Vector2i sampleExtent = sampleBounds.Diagonal();

    int tileSize;
    if (multithreaded) {
        tileSize = 16;
    } else {
        tileSize = 16;
    }
    // hardcode to 1 for performance comparisons
    tileSize = 16;

    Point2i nTiles((sampleExtent.x + tileSize - 1) / tileSize,
                   (sampleExtent.y + tileSize - 1) / tileSize);

    std::unique_ptr<Sampler> tileSampler;
    std::unique_ptr<Sampler> transSampler;
    std::unique_ptr<Sampler> ffSampler;

    tileSampler = sampler->Clone(seedOne);

    // TODO: make the other sampling methods work with this
    if (trans_sampler == "independent")
        transSampler = std::unique_ptr<Sampler>(
            new Trans_Independent(sampler->samplesPerPixel, seedTwo));
    else if (trans_sampler == "stream")
        transSampler =
            std::unique_ptr<Sampler>(new Trans_LatinHyperCubes_Stream(
                sampler->samplesPerPixel, seedTwo));
    else
        transSampler = std::unique_ptr<Sampler>(
            new Trans_Independent(sampler->samplesPerPixel, seedTwo));

    if (ff_sampler == "independent") {
        ffSampler = std::unique_ptr<Sampler>(
            new Trans_Independent(sampler->samplesPerPixel, seedThree));
    } else if (ff_sampler == "stream") {
        ffSampler = std::unique_ptr<Sampler>(new Trans_LatinHyperCubes_Stream(
            sampler->samplesPerPixel, seedThree));
    } else {
        std::cout << "Warning: Not Supported Free Flight Sampler, Defaulting "
                     "to Independent"
                  << std::endl;
        ffSampler = std::unique_ptr<Sampler>(
            new Trans_Independent(sampler->samplesPerPixel, seedThree));
    }

    uint64_t totalWork;

    if (maxExtCalls > 0) {
        totalWork = maxExtCalls;
    } else {
        totalWork = nTiles.x * nTiles.y * sampler->samplesPerPixel;
    }

    ProgressReporter reporter(totalWork, "Rendering", getMaxSeconds());

    int i = 0;
    int finished_location = -1;

    while (i < sampler->samplesPerPixel) {
        if (!reporter.continueRun(maxExtCalls)) {
            finished_location = i;
            i = sampler->samplesPerPixel;
        }
        {
            ParallelFor2D(
                [&](Point2i tile) {
                    // Render section of image corresponding to _tile_

                    // stop running if the time is off
                    if (!reporter.continueRun(maxExtCalls)) return;

                    // Allocate _MemoryArena_ for tile
                    MemoryArena arena;

                    // Get sampler instance for tile
                    int instanceIndex = tile.y * nTiles.x + tile.x;

                    // Compute sample bounds for tile
                    int x0 = sampleBounds.pMin.x + tile.x * tileSize;
                    int x1 = std::min(x0 + tileSize, sampleBounds.pMax.x);
                    int y0 = sampleBounds.pMin.y + tile.y * tileSize;
                    int y1 = std::min(y0 + tileSize, sampleBounds.pMax.y);

                    Bounds2i tileBounds(Point2i(x0, y0), Point2i(x1, y1));

                    // Get _FilmTile_ for tile
                    std::unique_ptr<FilmTile> filmTile =
                        camera->film->GetFilmTile(tileBounds);

                    // Loop over pixels in tile to render them
                    for (Point2i pixel : tileBounds) {
                        // Do this check after the StartPixel() call; this keeps
                        // the usage of RNG values from (most) Samplers that use
                        // RNGs consistent, which improves reproducability /
                        // debugging.
                        if (!InsideExclusive(pixel, pixelBounds)) continue;

                        tileSampler->StartPixel(pixel);
                        transSampler->StartPixel(pixel, sampleExtent.x,
                                                 sampleExtent.y, 1);
                        ffSampler->StartPixel(pixel, sampleExtent.x,
                                              sampleExtent.y, 2);

                        // if (i == 0 && pixel.x == 0 && pixel.y == 0) {
                        //     std::cout << "mem" << std::endl;
                        //     // MemoryArena arena2;
                        //     std::cout << "cam" << std::endl;
                        //     CameraSample cameraSample =
                        //         tileSampler->GetCameraSample(
                        //             Point2i(nTiles.x, nTiles.y));
                        //     std::cout << "ray" << std::endl;
                        //     RayDifferential ray;
                        //     Float rayWeight =
                        //     camera->GenerateRayDifferential(
                        //         cameraSample, &ray);

                        //     std::cout << "int" << std::endl;
                        //     // HACK TO PRINT OUT VERTICAL SLICE
                        //     SurfaceInteraction isect;
                        //     bool foundIntersection =
                        //         scene.Intersect(ray, &isect);

                        //     std::cout << "isect" << std::endl;
                        //     isect.ComputeScatteringFunctions(ray, arena,
                        //     true); if (!isect.bsdf) {
                        //         ray = isect.SpawnRay(ray.d);
                        //         // bounces--;
                        //         std::cout << "whiio" << std::endl;
                        //         if (ray.medium) {
                        //             std::cout << "I: " << i << std::endl;
                        //             ray.medium->RenderVerticalSlice(
                        //                 camera->film->filename, i);
                        //         }
                        //     }
                        // }

                        // Initialize _CameraSample_ for current sample
                        CameraSample cameraSample =
                            tileSampler->GetCameraSample(pixel);

                        // Generate camera ray for current sample
                        RayDifferential ray;
                        Float rayWeight =
                            camera->GenerateRayDifferential(cameraSample, &ray);
                        ray.ScaleDifferentials(
                            1.0 /
                            std::sqrt((Float)tileSampler->samplesPerPixel));
                        // ++nCameraRays;

                        // Evaluate radiance along camera ray
                        Spectrum L(0.f);

                        // std::cout << "calling LI_mutable" << std::endl;

                        if (rayWeight > 0)
                            L += Li_Mutable(ray, scene, *tileSampler,
                                            *transSampler, *ffSampler, arena, 0,
                                            false);

                        // Add camera ray's contribution to image
                        filmTile->AddSample(cameraSample.pFilm, L, rayWeight);

                        // actual majorant
                        // if (is_power_of_2(i)) {
                        //     L = Li_Mutable(ray, scene, *tileSampler,
                        //                    *transSampler, *ffSampler, arena,
                        //                    0, true);
                        //     filmTile->SetSample(cameraSample.pFilm, L,
                        //                         rayWeight);
                        // }

                        // Free _MemoryArena_ memory from computing image sample
                        // value
                        arena.Reset();
                    }

                    // Merge image tile into _Film_
                    if (reporter.continueRun(maxExtCalls)) {
                        camera->film->MergeFilmTile(std::move(filmTile));
                        if (maxExtCalls > 0) {
                            reporter.UpdateExact(
                                HeterogeneousMedium::nDensityCalls);
                        } else {
                            reporter.Update();
                        }
                    }
                },
                nTiles);
        }

        tileSampler->StartNextSample();
        transSampler->StartNextSample();
        ffSampler->StartNextSample();
        i++;

        // if write increments is turned off, we still want to write
        // the slices at the end
        if (Globals::WRITE_INCREMENTS || i == sampler->samplesPerPixel) {
            if (reporter.continueRun(maxExtCalls)) {
                if (is_power_of_2(i)) {
                    long currentTime = reporter.getCurrentTime();
                    camera->film->WriteImage(currentTime, i);

                    MemoryArena arena;

                    CameraSample cameraSample = tileSampler->GetCameraSample(
                        Point2i(nTiles.x, nTiles.y));

                    RayDifferential ray;
                    Float rayWeight =
                        camera->GenerateRayDifferential(cameraSample, &ray);

                    // HACK TO PRINT OUT VERTICAL SLICE
                    SurfaceInteraction isect;
                    bool foundIntersection = scene.Intersect(ray, &isect);

                    isect.ComputeScatteringFunctions(ray, arena, true);
                    if (!isect.bsdf) {
                        ray = isect.SpawnRay(ray.d);
                        // bounces--;

                        if (ray.medium) {
                            ray.medium->RenderVerticalSlice(
                                camera->film->filename, i);
                        }
                    }
                }
            }
        }
    }
    reporter.Done();

    // Save final image after rendering
    long currentTime = reporter.getCurrentTime();
    camera->film->WriteImage(currentTime, -1, finished_location);
}

Spectrum MajorantIntegrator::Li_Mutable(const RayDifferential &r, Scene &scene,
                                        Sampler &sampler,
                                        Sampler &trans_sampler,
                                        Sampler &ff_sampler, MemoryArena &arena,
                                        int depth, bool is_render) const {
    ProfilePhase p(Prof::SamplerIntegratorLi);
    Spectrum L(0.f), beta(1.f);
    RayDifferential ray(r);
    bool specularBounce = false;
    int bounces;
    Float etaScale = 1;

    for (bounces = 0;; ++bounces) {
        // Intersect _ray_ with scene and store intersection in _isect_
        SurfaceInteraction isect;
        bool foundIntersection = scene.Intersect(ray, &isect);

        // Sample the participating medium, if present
        MediumInteraction mi;
        if (ray.medium) {
            beta *=
                ray.medium->Sample(ray, ff_sampler, trans_sampler, arena, &mi);

            if (is_render) return ray.medium->Majorant(ray);
        }
        if (beta.IsBlack()) break;  // this

        // Handle an interaction with a medium or a surface
        if (mi.IsValid()) {
            // Terminate path if ray escaped or _maxDepth_ was reached
            if (bounces >= maxDepth) break;

            ++volumeInteractions;
            // Handle scattering at point in medium for volumetric path tracer
            const Distribution1D *lightDistrib =
                lightDistribution->Lookup(mi.p);

            L += beta * UniformSampleAllLights(mi, scene, arena, sampler,
                                               trans_sampler, true);

            Vector3f wo = -ray.d, wi;
            mi.phase->Sample_p(wo, &wi, sampler.Get2D());
            ray = mi.SpawnRay(wi);
            specularBounce = false;
        } else {
            ++surfaceInteractions;
            // Handle scattering at point on surface for volumetric path tracer

            // Possibly add emitted light at intersection
            if (bounces == 0 || specularBounce) {
                // Add emitted light at path vertex or from the environment
                if (foundIntersection)
                    L += beta * isect.Le(-ray.d);
                else
                    for (const auto &light : scene.infiniteLights)
                        L += beta * light->Le(ray);
            }

            // Terminate path if ray escaped or _maxDepth_ was reached
            if (!foundIntersection || bounces >= maxDepth) break;

            // Compute scattering functions and skip over medium boundaries
            isect.ComputeScatteringFunctions(ray, arena, true);
            if (!isect.bsdf) {
                ray = isect.SpawnRay(ray.d);
                bounces--;
                continue;
            }

            // Sample illumination from lights to find attenuated path
            // contribution
            const Distribution1D *lightDistrib =
                lightDistribution->Lookup(isect.p);

            L += beta * UniformSampleAllLights(isect, scene, arena, sampler,
                                               trans_sampler, true);

            // Sample BSDF to get new path direction
            Vector3f wo = -ray.d, wi;
            Float pdf;
            BxDFType flags;
            Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf,
                                              BSDF_ALL, &flags);
            if (f.IsBlack() || pdf == 0.f) break;

            beta *= f * AbsDot(wi, isect.shading.n) / pdf;

            specularBounce = (flags & BSDF_SPECULAR) != 0;
            if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION)) {
                Float eta = isect.bsdf->eta;
                // Update the term that tracks radiance scaling for refraction
                // depending on whether the ray is entering or leaving the
                // medium.
                etaScale *=
                    (Dot(wo, isect.n) > 0) ? (eta * eta) : 1 / (eta * eta);
            }
            ray = isect.SpawnRay(wi);

            // Account for attenuated subsurface scattering, if applicable
            if (isect.bssrdf && (flags & BSDF_TRANSMISSION)) {
                // Importance sample the BSSRDF
                SurfaceInteraction pi;
                Spectrum S = isect.bssrdf->Sample_S(
                    scene, sampler.Get1D(), sampler.Get2D(), arena, &pi, &pdf);

                if (S.IsBlack() || pdf == 0) break;
                beta *= S / pdf;

                L += beta * UniformSampleAllLights(pi, scene, arena, sampler,
                                                   trans_sampler, true);

                // Account for the indirect subsurface scattering component
                Spectrum f = pi.bsdf->Sample_f(pi.wo, &wi, sampler.Get2D(),
                                               &pdf, BSDF_ALL, &flags);
                if (f.IsBlack() || pdf == 0) break;
                beta *= f * AbsDot(wi, pi.shading.n) / pdf;

                specularBounce = (flags & BSDF_SPECULAR) != 0;
                ray = pi.SpawnRay(wi);
            }
        }

        // Possibly terminate the path with Russian roulette
        // Factor out radiance scaling due to refraction in rrBeta.
        Spectrum rrBeta = beta * etaScale;
        if (rrBeta.MaxComponentValue() < rrThreshold && bounces > 3) {
            Float q = std::max((Float).05, 1 - rrBeta.MaxComponentValue());
            if (sampler.Get1D() < q) break;
            beta /= 1 - q;
        }
    }

    ReportValue(pathLength, bounces);
    return L;
}

MajorantIntegrator *CreateMajorantIntegrator(
    const ParamSet &params, std::shared_ptr<Sampler> sampler,
    std::shared_ptr<const Camera> camera) {
    int maxDepth = params.FindOneInt("maxdepth", 5);
    long maxSeconds = (long)params.FindOneInt("maxSeconds", 100000);
    long maxExtCalls = (long)params.FindOneLong("maxExtCalls", -1);
    std::string trans_sampler =
        params.FindOneString("trans_sampler", "independent");
    std::string ff_sampler = params.FindOneString("ff_sampler", "independent");
    bool ff_correlated = params.FindOneBool("ff_correlated", false);
    bool multithreaded = params.FindOneBool("multithreaded", false);
    int np;
    const int *pb = params.FindInt("pixelbounds", &np);
    Bounds2i pixelBounds = camera->film->GetSampleBounds();
    if (pb) {
        if (np != 4)
            Error("Expected four values for \"pixelbounds\" parameter. Got %d.",
                  np);
        else {
            pixelBounds = Intersect(pixelBounds,
                                    Bounds2i{{pb[0], pb[2]}, {pb[1], pb[3]}});
            if (pixelBounds.Area() == 0)
                Error("Degenerate \"pixelbounds\" specified.");
        }
    }
    Float rrThreshold = params.FindOneFloat("rrthreshold", 1.);
    // uint32_t seedOne = uint32_t(params.FindOneInt("seedOne", 0xaf43fab));
    // uint32_t seedTwo = uint32_t(params.FindOneInt("seedTwo", 0x41c3d29));
    // uint32_t seedThree = uint32_t(params.FindOneInt("seedThree", 0x1cbaf45));

    uint32_t seedOne = uint32_t(params.FindOneInt("seedOne", 0xb7f355));
    uint32_t seedTwo = uint32_t(params.FindOneInt("seedTwo", 0x291929));
    uint32_t seedThree = uint32_t(params.FindOneInt("seedThree", 0xaaacba));
    // std::string lightStrategy =
    //     params.FindOneString("lightsamplestrategy", "spatial");
    std::string lightStrategy =
        params.FindOneString("lightsamplestrategy", "uniform");
    return new MajorantIntegrator(maxDepth, camera, sampler, pixelBounds,
                                  maxSeconds, maxExtCalls, seedOne, seedTwo,
                                  seedThree, trans_sampler, ff_sampler,
                                  rrThreshold, lightStrategy, multithreaded);
}

}  // namespace pbrt
