// integrators/whitted.cpp*
#include "integrators/whitted_blanchet.h"

#include "camera.h"
#include "film.h"
#include "interaction.h"
#include "paramset.h"

namespace pbrt {

void WhittedBlanchetIntegrator::accumulate_stuff(BlanchetCorrelatedResults &L,
                                                 const Spectrum &stuff,
                                                 int blanchet_n,
                                                 int depth) const {
    // std::cout << "depth: " << depth << std::endl;
    if (depth == 0) L.bias += stuff;
    if (depth == blanchet_n) {
        L.odd += stuff;
        L.even += stuff;
    }
    if (depth == blanchet_n + 1) {
        L.all += stuff;
    }

    // std::cout << "bias: " << L.bias << std::endl;
    // std::cout << "all: " << L.all << std::endl;
}

void WhittedBlanchetIntegrator::multiply_stuff(BlanchetCorrelatedResults &L,
                                               const Spectrum &stuff,
                                               int blanchet_n,
                                               int depth) const {
    // std::cout << "depth: " << depth << std::endl;
    if (depth == 0) L.bias *= stuff;
    if (depth == blanchet_n) {
        L.odd *= stuff;
        L.even *= stuff;
    }
    if (depth == blanchet_n + 1) {
        L.all *= stuff;
    }

    // std::cout << "bias: " << L.bias << std::endl;
    // std::cout << "all: " << L.all << std::endl;
}

BlanchetCorrelatedResults WhittedBlanchetIntegrator::SpecularReflect_Blanchet(
    const RayDifferential &ray, const SurfaceInteraction &isect,
    const Scene &scene, Sampler &sampler, Sampler &trans_sampler,
    Sampler &ff_sampler, MemoryArena &arena, int blanchet_n, int depth) const {
    // Compute specular reflection direction _wi_ and BSDF value
    Vector3f wo = isect.wo, wi;
    Float pdf;
    BxDFType type = BxDFType(BSDF_REFLECTION | BSDF_SPECULAR);
    Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf, type);

    // Return contribution of specular reflection
    const Normal3f &ns = isect.shading.n;
    if (pdf > 0.f && !f.IsBlack() && AbsDot(wi, ns) != 0.f) {
        throw new std::exception();
        // Compute ray differential _rd_ for specular reflection
        RayDifferential rd = isect.SpawnRay(wi);
        if (ray.hasDifferentials) {
            rd.hasDifferentials = true;
            rd.rxOrigin = isect.p + isect.dpdx;
            rd.ryOrigin = isect.p + isect.dpdy;
            // Compute differential reflected directions
            Normal3f dndx = isect.shading.dndu * isect.dudx +
                            isect.shading.dndv * isect.dvdx;
            Normal3f dndy = isect.shading.dndu * isect.dudy +
                            isect.shading.dndv * isect.dvdy;
            Vector3f dwodx = -ray.rxDirection - wo,
                     dwody = -ray.ryDirection - wo;
            Float dDNdx = Dot(dwodx, ns) + Dot(wo, dndx);
            Float dDNdy = Dot(dwody, ns) + Dot(wo, dndy);
            rd.rxDirection =
                wi - dwodx + 2.f * Vector3f(Dot(wo, ns) * dndx + dDNdx * ns);
            rd.ryDirection =
                wi - dwody + 2.f * Vector3f(Dot(wo, ns) * dndy + dDNdy * ns);
        }
        std::cout << "CALLING DEPTH: " << depth + 1 << std::endl;
        BlanchetCorrelatedResults L =
            Li_Blanchet(rd, scene, sampler, trans_sampler, ff_sampler, arena,
                        blanchet_n, depth + 1);

        accumulate_stuff(L, f / pdf * AbsDot(wi, ns), blanchet_n, depth);

        return L;
        // return f / pdf *  * AbsDot(wi, ns);
    } else {
        return BlanchetCorrelatedResults(0.f);
    }
}

BlanchetCorrelatedResults WhittedBlanchetIntegrator::Li_Blanchet(
    const RayDifferential &r, const Scene &scene, Sampler &sampler,
    Sampler &trans_sampler, Sampler &ff_sampler, MemoryArena &arena,
    int blanchet_n, int depth) const {
    BlanchetCorrelatedResults L = BlanchetCorrelatedResults(Spectrum(0.f));
    BlanchetCorrelatedResults beta = BlanchetCorrelatedResults(Spectrum(1.f));

    RayDifferential ray(r);
    // std::cout << "DEPTH: " << depth << std::endl;

    for (int bounces = 0; bounces <= blanchet_n + 1; bounces++) {
        // Find closest ray intersection or return background radiance
        SurfaceInteraction isect;
        if (!scene.Intersect(ray, &isect)) {
            break;
            // std::cout << "ACK" << std::endl;
            // throw new std::exception();
            // for (const auto &light : scene.lights) L += light->Le(ray);
            // return L;
        }

        // Compute emitted and reflected light at ray intersection point
        // if (!foundIntersection) break;

        // Initialize common variables for Whitted integrator
        // const Normal3f &n = isect.shading.n;
        // Vector3f wo = isect.wo;

        // Compute scattering functions for surface interaction
        isect.ComputeScatteringFunctions(ray, arena);
        if (!isect.bsdf) {
            // ignore invisible medium boundries
            ray = isect.SpawnRay(ray.d);
            bounces--;
            continue;
        }

        // Compute emitted light if ray hit an area light source
        // TODO: do this maybe
        // accumulate_stuff(L, isect.Le(wo), blanchet_n, depth);
        // L += isect.Le(wo);

        // Add contribution of each light source
        for (const auto &light : scene.lights) {
            Vector3f wi;
            Vector3f wo = -ray.d;
            Float pdf;
            VisibilityTester visibility;
            Spectrum Li = light->Sample_Li(isect, sampler.Get2D(), &wi, &pdf,
                                           &visibility);

            if (Li.IsBlack() || pdf == 0) continue;

            Spectrum f = isect.bsdf->f(wo, wi);
            if (!f.IsBlack() && visibility.Unoccluded(scene)) {
                BlanchetCorrelatedResults tmp = beta;
                if (bounces == 0)
                    L.bias += beta.bias *
                              (f * Li * AbsDot(wi, isect.shading.n) / pdf);
                if (bounces == blanchet_n) {
                    L.odd +=
                        beta.odd * (f * Li * AbsDot(wi, isect.shading.n) / pdf);
                    L.even += beta.even *
                              (f * Li * AbsDot(wi, isect.shading.n) / pdf);
                }
                if (bounces == blanchet_n + 1) {
                    L.all +=
                        beta.all * (f * Li * AbsDot(wi, isect.shading.n) / pdf);
                }
                // accumulate_stuff(L, f * Li * AbsDot(wi, n) / pdf, blanchet_n,
                // depth); L += f * Li * AbsDot(wi, n) / pdf;
            }
        }

        Vector3f wo = -ray.d, wi;
        Float pdf;
        BxDFType flags;
        Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf,
                                          BSDF_ALL, &flags);
        if (f.IsBlack() || pdf == 0.f) break;
        // std::cout << "wooo" << std::endl;
        // std::cout << isect.shading.n << std::endl;
        beta *= f * AbsDot(wi, isect.shading.n) / pdf;
        // TODO: implement support for specular objects
        // #if LOGGING
        //     DCHECK(std::isinf(beta.y()) == false);
        // #endif
        // specularBounce = (flags & BSDF_SPECULAR) != 0;
        // if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION)) {
        //     Float eta = isect.bsdf->eta;
        //     // Update the term that tracks radiance scaling for refraction
        //     // depending on whether the ray is entering or leaving the
        //     // medium.
        //     etaScale *=
        //         (Dot(wo, isect.n) > 0) ? (eta * eta) : 1 / (eta * eta);
        // }
        ray = isect.SpawnRay(wi);

        // throw new std::exception();
        // Trace rays for specular reflection and refraction
        // L += SpecularReflect_Blanchet(ray,
        //                               isect,
        //                               scene,
        //                               sampler,
        //                               trans_sampler,
        //                               ff_sampler,
        //                               arena,
        //                               blanchet_n,
        //                               depth);

        // TODO: implement transmission later
        // L += SpecularTransmit(ray, isect, scene, sampler, trans_sampler,
        // ff_sampler, arena, depth);
    }
    return L;
}

// WhittedIntegrator Method Definitions
Spectrum WhittedBlanchetIntegrator::Li(const RayDifferential &ray,
                                       const Scene &scene, Sampler &sampler,
                                       Sampler &trans_sampler,
                                       Sampler &ff_sampler, MemoryArena &arena,
                                       int blah) const {
    // need to sample a blanchet_n
    Float val = sampler.Get1D();
    // chosen r based off of blanchet paper
    // Float r = 1.0 - pow(2.0, -1.5);
    Float r = 0.5;
    // Float r = 0.2;
    // Float pmf = r * pow(1.0 - r, 0);
    Float pmf = r;
    Float cmf = pmf;
    int n = 0;

    while (cmf < val) {
        ++n;
        pmf *= 1.0 - r;
        cmf += pmf;
    }

    // n = 6;

    BlanchetCorrelatedResults coor_results = Li_Blanchet(
        ray, scene, sampler, trans_sampler, ff_sampler, arena, n, 0);
    // std::cout << coor_results.all[0] << std::endl;
    // std::cout << coor_results.bias[0] << std::endl;
    // std::cout << n << std::endl;
    return coor_results.bias + coor_results.all / pmf;

    return (coor_results.all - (0.5 * (coor_results.even + coor_results.odd))) /
               pmf +
           coor_results.bias;
}

WhittedBlanchetIntegrator *CreateWhittedBlanchetIntegrator(
    const ParamSet &params, std::shared_ptr<Sampler> sampler,
    std::shared_ptr<const Camera> camera) {
    long maxSeconds = (long)params.FindOneInt("maxSeconds", 100000);
    long maxExtCalls = (long)params.FindOneLong("maxExtCalls", -1);
    std::string trans_sampler =
        params.FindOneString("trans_sampler", "independent");
    std::string ff_sampler = params.FindOneString("ff_sampler", "independent");
    bool ff_correlated = params.FindOneBool("ff_correlated", false);
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
    uint32_t seedOne = uint32_t(params.FindOneInt("seedOne", 0xaf43fab));
    uint32_t seedTwo = uint32_t(params.FindOneInt("seedTwo", 0x41c3d29));
    uint32_t seedThree = uint32_t(params.FindOneInt("seedThree", 0x1cbaf45));
    return new WhittedBlanchetIntegrator(
        camera, sampler, pixelBounds, maxSeconds, maxExtCalls, seedOne, seedTwo,
        seedThree, trans_sampler, ff_sampler);
}

}  // namespace pbrt
