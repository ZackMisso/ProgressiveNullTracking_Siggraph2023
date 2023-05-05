// integrators/path_blanchet.cpp*
#include "integrators/path_blanchet.h"

#include "bssrdf.h"
#include "camera.h"
#include "film.h"
#include "interaction.h"
#include "paramset.h"
#include "scene.h"
#include "stats.h"

namespace pbrt {

STAT_PERCENT("Integrator/Zero-radiance paths", zeroRadiancePaths, totalPaths);
STAT_INT_DISTRIBUTION("Integrator/Path length", pathLength);

// PathIntegrator Method Definitions
PathBlanchetIntegrator::PathBlanchetIntegrator(
    int maxDepth, std::shared_ptr<const Camera> camera,
    std::shared_ptr<Sampler> sampler, const Bounds2i &pixelBounds,
    long maxSeconds, long maxExtCalls, uint32_t seedOne, uint32_t seedTwo,
    uint32_t seedThree, std::string trans_sampler, std::string ff_sampler,
    Float rrThreshold, const std::string &lightSampleStrategy,
    bool multithreaded, Float r)
    : SamplerIntegrator(camera, sampler, pixelBounds, maxSeconds, maxExtCalls,
                        seedOne, seedTwo, seedThree, trans_sampler, ff_sampler,
                        multithreaded),
      maxDepth(maxDepth),
      rrThreshold(rrThreshold),
      lightSampleStrategy(lightSampleStrategy),
      r(r) {}

void PathBlanchetIntegrator::Preprocess(const Scene &scene, Sampler &sampler) {
    lightDistribution =
        CreateLightSampleDistribution(lightSampleStrategy, scene);
}

void PathBlanchetIntegrator::accumulate_stuff(BlanchetCorrelatedResults &L,
                                              const Spectrum &stuff,
                                              int blanchet_n, int maxDepth,
                                              int depth) const {
    // if (depth != 0)
    //     std::cout << "max_depth: " << maxDepth << " depth: " << depth <<
    //     std::endl;
    if (depth < maxDepth) L.bias += stuff;
    if (depth < maxDepth + blanchet_n) L.odd += stuff;
    if (depth < maxDepth + blanchet_n + 1) L.all += stuff;
}

BlanchetCorrelatedResults PathBlanchetIntegrator::Li_Blanchet(
    const RayDifferential &r, const Scene &scene, Sampler &sampler,
    Sampler &trans_sampler, Sampler &ff_sampler, MemoryArena &arena,
    int blanchet_n, int depth) const {
    ProfilePhase p(Prof::SamplerIntegratorLi);
    BlanchetCorrelatedResults L(0.f);
    Spectrum beta(1.f);
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
// Find next path vertex and accumulate contribution
#if LOGGING
        VLOG(2) << "Path tracer bounce " << bounces << ", current L = " << L
                << ", beta = " << beta;
#endif

        // Intersect _ray_ with scene and store intersection in _isect_
        SurfaceInteraction isect;
        bool foundIntersection = scene.Intersect(ray, &isect);

        // Possibly add emitted light at intersection
        if (bounces == 0 || specularBounce) {
            // Add emitted light at path vertex or from the environment
            if (foundIntersection) {
                accumulate_stuff(L, beta * isect.Le(-ray.d), blanchet_n,
                                 maxDepth, bounces);
// L += beta * isect.Le(-ray.d);
#if LOGGING
                VLOG(2) << "Added Le -> L = " << L;
#endif
            } else {
                for (const auto &light : scene.infiniteLights)
                    L += beta * light->Le(ray);
#if LOGGING
                VLOG(2) << "Added infinite area lights -> L = " << L;
#endif
            }
        }

        // Terminate path if ray escaped or _maxDepth_ was reached
        if (!foundIntersection || bounces > maxDepth + blanchet_n + 1) break;

        // Compute scattering functions and skip over medium boundaries
        isect.ComputeScatteringFunctions(ray, arena, true);
        if (!isect.bsdf) {
            // #if LOGGING
            //     VLOG(2) << "Skipping intersection due to null bsdf";
            // #endif
            ray = isect.SpawnRay(ray.d);
            bounces--;
            continue;
        }

        const Distribution1D *distrib = lightDistribution->Lookup(isect.p);

        // Sample illumination from lights to find path contribution.
        // (But skip this for perfectly specular BSDFs.)
        if (isect.bsdf->NumComponents(BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) >
            0) {
            ++totalPaths;
            Spectrum Ld =
                beta * UniformSampleOneLight(isect, scene, arena, sampler,
                                             trans_sampler, false, distrib);
            // #if LOGGING
            //     VLOG(2) << "Sampled direct lighting Ld = " << Ld;
            // #endif
            if (Ld.IsBlack()) ++zeroRadiancePaths;
            // #if LOGGING
            //     CHECK_GE(Ld.y(), 0.f);
            // #endif
            accumulate_stuff(L, Ld, blanchet_n, maxDepth, bounces);
            // L += Ld;
        }

        // Sample BSDF to get new path direction
        Vector3f wo = -ray.d, wi;
        Float pdf;
        BxDFType flags;
        Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf,
                                          BSDF_ALL, &flags);
#if LOGGING
        VLOG(2) << "Sampled BSDF, f = " << f << ", pdf = " << pdf;
#endif
        if (f.IsBlack() || pdf == 0.f) break;
        beta *= f * AbsDot(wi, isect.shading.n) / pdf;
#if LOGGING
        VLOG(2) << "Updated beta = " << beta;
        CHECK_GE(beta.y(), 0.f);
        DCHECK(!std::isinf(beta.y()));
#endif
        specularBounce = (flags & BSDF_SPECULAR) != 0;
        if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION)) {
            Float eta = isect.bsdf->eta;
            // Update the term that tracks radiance scaling for refraction
            // depending on whether the ray is entering or leaving the
            // medium.
            etaScale *= (Dot(wo, isect.n) > 0) ? (eta * eta) : 1 / (eta * eta);
        }
        ray = isect.SpawnRay(wi);

        // Account for subsurface scattering, if applicable
        if (isect.bssrdf && (flags & BSDF_TRANSMISSION)) {
            // Importance sample the BSSRDF
            SurfaceInteraction pi;
            Spectrum S = isect.bssrdf->Sample_S(
                scene, sampler.Get1D(), sampler.Get2D(), arena, &pi, &pdf);
#if LOGGING
            DCHECK(!std::isinf(beta.y()));
#endif
            if (S.IsBlack() || pdf == 0) break;
            beta *= S / pdf;

            // Account for the direct subsurface scattering component
            Spectrum direct_sub =
                beta * UniformSampleOneLight(pi, scene, arena, sampler,
                                             trans_sampler, false,
                                             lightDistribution->Lookup(pi.p));

            accumulate_stuff(L, direct_sub, blanchet_n, maxDepth, bounces);

            // L += beta * UniformSampleOneLight(pi,
            //                                   scene,
            //                                   arena,
            //                                   sampler,
            //                                   trans_sampler,
            //                                   false,
            //                                   lightDistribution->Lookup(pi.p));

            // Account for the indirect subsurface scattering component
            Spectrum f = pi.bsdf->Sample_f(pi.wo, &wi, sampler.Get2D(), &pdf,
                                           BSDF_ALL, &flags);
            if (f.IsBlack() || pdf == 0) break;
            beta *= f * AbsDot(wi, pi.shading.n) / pdf;
#if LOGGING
            DCHECK(!std::isinf(beta.y()));
#endif
            specularBounce = (flags & BSDF_SPECULAR) != 0;
            ray = pi.SpawnRay(wi);
        }

        // Possibly terminate the path with Russian roulette.
        // Factor out radiance scaling due to refraction in rrBeta.
        Spectrum rrBeta = beta * etaScale;
        if (rrBeta.MaxComponentValue() < rrThreshold && bounces > 3) {
            Float q = std::max((Float).05, 1 - rrBeta.MaxComponentValue());
            if (sampler.Get1D() < q) break;
            beta /= 1 - q;
#if LOGGING
            DCHECK(!std::isinf(beta.y()));
#endif
        }
    }
    ReportValue(pathLength, bounces);
    return L;
}

Spectrum PathBlanchetIntegrator::Li(const RayDifferential &ray,
                                    const Scene &scene, Sampler &sampler,
                                    Sampler &trans_sampler, Sampler &ff_sampler,
                                    MemoryArena &arena, int depth) const {
    // std::vector<double> cust = std::vector<double>();
    //
    // cust.push_back(0.728396992272);
    // cust.push_back(0.200088430721);
    // cust.push_back(0.0493170728762);
    // cust.push_back(0.0146857930762);
    // cust.push_back(0.00422328504064);
    // cust.push_back(0.00156420197947);
    // cust.push_back(0.000590669522797);
    // cust.push_back(0.000350068020336);
    // cust.push_back(0.000360439551335);
    // cust.push_back(0.000423046939741);
    //
    // // need to sample a blanchet_n
    // Float val = sampler.Get1D();
    // Float pmf = cust[0];
    // Float cmf = pmf;
    // int n = 0;
    //
    // while (cmf < val)
    // {
    //     ++n;
    //     pmf = cust[n];
    //     cmf += pmf;
    // }

    // need to sample a blanchet_n
    Float val = sampler.Get1D();
    Float pmf = r;
    Float cmf = pmf;
    int n = 0;

    while (cmf < val) {
        ++n;
        pmf *= 1.0 - r;
        cmf += pmf;
    }

    BlanchetCorrelatedResults coor_results = Li_Blanchet(
        ray, scene, sampler, trans_sampler, ff_sampler, arena, n, 0);

    // return coor_results.bias;
    // return coor_results.odd;
    // return coor_results.even;
    // return coor_results.all;

    return (coor_results.all - (coor_results.odd)) / pmf + coor_results.bias;
}

PathBlanchetIntegrator *CreatePathBlanchetIntegrator(
    const ParamSet &params, std::shared_ptr<Sampler> sampler,
    std::shared_ptr<const Camera> camera) {
    int maxDepth = params.FindOneInt("maxdepth", 5);
    Float r = Float(params.FindOneInt("r", 5)) / 100.0;
    r = 0.3;
    std::cout << "R: " << r << std::endl;
    std::cout << "MAX DEPTH: " << maxDepth << std::endl;
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
    uint32_t seedOne = uint32_t(params.FindOneInt("seedOne", 0xaf43fab));
    uint32_t seedTwo = uint32_t(params.FindOneInt("seedTwo", 0x41c3d29));
    uint32_t seedThree = uint32_t(params.FindOneInt("seedThree", 0x1cbaf45));

    std::string lightStrategy =
        params.FindOneString("lightsamplestrategy", "spatial");

    return new PathBlanchetIntegrator(
        maxDepth, camera, sampler, pixelBounds, maxSeconds, maxExtCalls,
        seedOne, seedTwo, seedThree, trans_sampler, ff_sampler, rrThreshold,
        lightStrategy, multithreaded, r);
}

}  // namespace pbrt
