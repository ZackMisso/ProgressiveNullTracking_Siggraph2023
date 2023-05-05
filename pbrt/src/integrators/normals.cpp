// integrators/normals.cpp*
#include "integrators/normals.h"

#include "camera.h"
#include "film.h"
#include "interaction.h"
#include "paramset.h"

namespace pbrt {

// NormalsIntegrator Method Definitions
Spectrum NormalsIntegrator::Li(const RayDifferential &ray, const Scene &scene,
                               Sampler &sampler, Sampler &trans_sampler,
                               Sampler &ff_sampler, MemoryArena &arena,
                               int depth) const {
    Spectrum L(0.);
    // Find closest ray intersection or return background radiance
    SurfaceInteraction isect;
    if (!scene.Intersect(ray, &isect)) {
        return L;
    }

    const Normal3f &n = isect.shading.n;

    L[0] = std::abs(n[0]);
    L[1] = std::abs(n[1]);
    L[2] = std::abs(n[2]);

    return L;
}

NormalsIntegrator *CreateNormalsIntegrator(
    const ParamSet &params, std::shared_ptr<Sampler> sampler,
    std::shared_ptr<const Camera> camera) {
    int maxDepth = params.FindOneInt("maxdepth", 5);
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

    return new NormalsIntegrator(maxDepth, camera, sampler, pixelBounds,
                                 maxSeconds, maxExtCalls, seedOne, seedTwo,
                                 seedThree, trans_sampler, ff_sampler);
}

}  // namespace pbrt
