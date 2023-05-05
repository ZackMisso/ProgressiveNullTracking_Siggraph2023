
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

// core/integrator.cpp*
#include "integrator.h"

#include "camera.h"
#include "film.h"
#include "integrator.h"
#include "interaction.h"
#include "media/heterogeneous.h"
#include "parallel.h"
#include "progressreporter.h"
#include "sampler.h"
#include "samplers/halton.h"
#include "samplers/stratified.h"
#include "sampling.h"
#include "scene.h"
#include "stats.h"
#include "trans_samplers/trans_halton.h"
#include "trans_samplers/trans_hammersley.h"
#include "trans_samplers/trans_independent.h"
#include "trans_samplers/trans_latin_stream.h"

namespace pbrt {

STAT_COUNTER("Integrator/Camera rays traced", nCameraRays);

// Integrator Method Definitions
Integrator::~Integrator() {}

// Integrator Utility Functions
Spectrum UniformSampleAllLights(const Interaction &it, const Scene &scene,
                                MemoryArena &arena, Sampler &sampler,
                                Sampler &trans_sampler,
                                const std::vector<int> &nLightSamples,
                                bool handleMedia) {
    ProfilePhase p(Prof::DirectLighting);
    Spectrum L(0.f);
    for (size_t j = 0; j < scene.lights.size(); ++j) {
        // Accumulate contribution of _j_th light to _L_
        const std::shared_ptr<Light> &light = scene.lights[j];
        int nSamples = nLightSamples[j];
        const Point2f *uLightArray = sampler.Get2DArray(nSamples);
        const Point2f *uScatteringArray = sampler.Get2DArray(nSamples);
        if (!uLightArray || !uScatteringArray) {
            // Use a single sample for illumination from _light_
            Point2f uLight = sampler.Get2D();
            Point2f uScattering = sampler.Get2D();
            L += EstimateDirect(it, uScattering, *light, uLight, scene, sampler,
                                trans_sampler, arena, handleMedia);
        } else {
            // Estimate direct lighting using sample arrays
            Spectrum Ld(0.f);
            for (int k = 0; k < nSamples; ++k)
                Ld += EstimateDirect(it, uScatteringArray[k], *light,
                                     uLightArray[k], scene, sampler,
                                     trans_sampler, arena, handleMedia);
            L += Ld / nSamples;
        }
    }
    return L;
}

// Integrator Utility Functions
Spectrum UniformSampleAllLights(const Interaction &it, const Scene &scene,
                                MemoryArena &arena, Sampler &sampler,
                                Sampler &trans_sampler, bool handleMedia) {
    ProfilePhase p(Prof::DirectLighting);
    Spectrum L(0.f);
    for (size_t j = 0; j < scene.lights.size(); ++j) {
        // Accumulate contribution of _j_th light to _L_
        const std::shared_ptr<Light> &light = scene.lights[j];
        // int nSamples = 1;
        // const Point2f *uLightArray = sampler.Get2DArray(nSamples);
        // const Point2f *uScatteringArray = sampler.Get2DArray(nSamples);
        // if (!uLightArray || !uScatteringArray) {
        //     // Use a single sample for illumination from _light_
        Point2f uLight = sampler.Get2D();
        Point2f uScattering = sampler.Get2D();
        L += EstimateDirect(it, uScattering, *light, uLight, scene, sampler,
                            trans_sampler, arena, handleMedia);
        // } else {
        //     // Estimate direct lighting using sample arrays
        //     Spectrum Ld(0.f);
        //     for (int k = 0; k < nSamples; ++k)
        //         Ld += EstimateDirect(it,
        //                              uScatteringArray[k],
        //                              *light,
        //                              uLightArray[k],
        //                              scene,
        //                              sampler,
        //                              trans_sampler,
        //                              arena,
        //                              handleMedia);
        //     L += Ld / nSamples;
        // }
    }
    return L;
}

Spectrum UniformSampleAllLights_NonExp(const Interaction &it,
                                       const Scene &scene, MemoryArena &arena,
                                       Sampler &sampler, Sampler &trans_sampler,
                                       bool handleMedia) {
    ProfilePhase p(Prof::DirectLighting);
    Spectrum L(0.f);
    for (size_t j = 0; j < scene.lights.size(); ++j) {
        // Accumulate contribution of _j_th light to _L_
        const std::shared_ptr<Light> &light = scene.lights[j];
        int nSamples = 1;

        // Estimate direct lighting using sample arrays
        Spectrum Ld(0.f);
        for (int k = 0; k < nSamples; ++k) {
            Point2f uScattering = sampler.Get2D();
            Point2f uLight = sampler.Get2D();

            Ld += EstimateDirect_NonExp(it, uScattering, *light, uLight, scene,
                                        sampler, trans_sampler, arena);
        }
        L += Ld / nSamples;
    }
    return L;
}

// void UniformSampleAllLights_LightLiH(const Interaction &it, const Scene
// &scene,
//                                      MemoryArena &arena, Sampler &sampler,
//                                      Sampler &trans_sampler, Spectrum &Li,
//                                      Spectrum &Lh, bool handleMedia) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Spectrum L(0.f);
//     for (size_t j = 0; j < scene.lights.size(); ++j) {
//         const std::shared_ptr<Light> &light = scene.lights[j];
//         Point2f uLight = sampler.Get2D();
//         Point2f uScattering = sampler.Get2D();
//         EstimateDirect_LiH(it, uScattering, *light, uLight, scene, sampler,
//                            trans_sampler, Li, Lh, arena, handleMedia);
//         // std::cout << "LI: " << Li << std::endl;
//         // // Accumulate contribution of _j_th light to _L_
//         // const std::shared_ptr<Light> &light = scene.lights[j];
//         // int nSamples = nLightSamples[j];
//         // const Point2f *uLightArray = sampler.Get2DArray(nSamples);
//         // const Point2f *uScatteringArray = sampler.Get2DArray(nSamples);
//         // if (!uLightArray || !uScatteringArray)
//         // {
//         //     // Use a single sample for illumination from _light_
//         //     Point2f uLight = sampler.Get2D();
//         //     Point2f uScattering = sampler.Get2D();
//         //     EstimateDirect_LiH
//         //     (
//         //         it,
//         //         uScattering,
//         //         *light,
//         //         uLight,
//         //         scene,
//         //         sampler,
//         //         trans_sampler,
//         //         arena,
//         //         handleMedia
//         //     );
//         // } else {
//         //     // Estimate direct lighting using sample arrays
//         //     Spectrum Ld(0.f);
//         //
//         //     for (int k = 0; k < nSamples; ++k)
//         //     {
//         //         Ld += EstimateDirect_LiH
//         //         (
//         //             it,
//         //             uScatteringArray[k],
//         //             *light,
//         //             uLightArray[k],
//         //             scene,
//         //             sampler,
//         //             trans_sampler,
//         //             arena,
//         //             handleMedia
//         //         );
//         //     }
//         //
//         //     L += Ld / nSamples;
//         // }
//     }
//     // return L;
// }

Spectrum UniformSampleOneLight(const Interaction &it, const Scene &scene,
                               MemoryArena &arena, Sampler &sampler,
                               Sampler &trans_sampler, bool handleMedia,
                               const Distribution1D *lightDistrib) {
    ProfilePhase p(Prof::DirectLighting);
    // Randomly choose a single light to sample, _light_
    int nLights = int(scene.lights.size());

    if (nLights == 0) return Spectrum(0.f);
    int lightNum;
    Float lightPdf;
    if (lightDistrib) {
        lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
        if (lightPdf == 0) return Spectrum(0.f);
    } else {
        lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
        lightPdf = Float(1) / nLights;
    }

    const std::shared_ptr<Light> &light = scene.lights[lightNum];

    Point2f uLight = sampler.Get2D();
    Point2f uScattering = sampler.Get2D();

    return EstimateDirect(it, uScattering, *light, uLight, scene, sampler,
                          trans_sampler, arena, handleMedia) /
           lightPdf;
}

// Spectrum UniformSampleOneLight_Dum(const Interaction &it, const Scene &scene,
//                                    int n, int typ, MemoryArena &arena,
//                                    Sampler &sampler, Sampler &trans_sampler,
//                                    bool handleMedia,
//                                    const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     if (nLights == 0) return Spectrum(0.f);
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) return Spectrum(0.f);
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     return EstimateDirect_Dum(it, uScattering, n, typ, *light, uLight, scene,
//                               sampler, trans_sampler, arena, handleMedia) /
//            lightPdf;
// }

// void UniformSampleOneLight_CorP(const Interaction &it, const Scene &scene,
//                                 MemoryArena &arena, Sampler &sampler,
//                                 Sampler &trans_sampler, FD_Cor_Spectrum &val,
//                                 bool handleMedia,
//                                 const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     // std::cout << "here" << std::endl;

//     if (nLights == 0) {
//         val.setAll(Spectrum(0.f));
//         return;
//     }
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             val.setAll(Spectrum(0.f));
//             return;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     // std::cout << "da fufk" << std::endl;

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     EstimateDirect_CorP(it, uScattering, *light, uLight, scene, sampler,
//                         trans_sampler, val, arena, handleMedia);

//     val /= lightPdf;
// }

// void UniformSampleOneLight_CorP_Deb(const Interaction &it, const Scene
// &scene,
//                                     MemoryArena &arena, Sampler &sampler,
//                                     Sampler &trans_sampler,
//                                     FD_Cor_Deb_Spectrum &val, bool
//                                     handleMedia, const Distribution1D
//                                     *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     // std::cout << "here" << std::endl;

//     if (nLights == 0) {
//         val.setAll(Spectrum(0.f));
//         return;
//     }
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             val.setAll(Spectrum(0.f));
//             return;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     EstimateDirect_CorP_Deb(it, uScattering, *light, uLight, scene, sampler,
//                             trans_sampler, val, arena, handleMedia);

//     val /= lightPdf;
// }

// void UniformSampleOneLight_CorP_Deb_Cent(
//     const Interaction &it, const Scene &scene, MemoryArena &arena,
//     Sampler &sampler, Sampler &trans_sampler, FD_Cor_Deb_Cent_Spectrum &val,
//     bool handleMedia, const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     if (nLights == 0) {
//         val.setAll(Spectrum(0.f));
//         return;
//     }
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             val.setAll(Spectrum(0.f));
//             return;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     EstimateDirect_CorP_Deb_Cent(it, uScattering, *light, uLight, scene,
//                                  sampler, trans_sampler, val, arena,
//                                  handleMedia);

//     val /= lightPdf;
// }

// void UniformSampleOneLight_FD_Rough(const Interaction &it, const Scene
// &scene,
//                                     MemoryArena &arena, Sampler &sampler,
//                                     Sampler &trans_sampler, Spectrum
//                                     &light_0, Spectrum &light_h, bool
//                                     handleMedia, const Distribution1D
//                                     *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     if (nLights == 0) {
//         light_0 = Spectrum(0.f);
//         light_h = Spectrum(0.f);
//         return;
//     }
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             light_0 = Spectrum(0.f);
//             light_h = Spectrum(0.f);
//             return;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     EstimateDirect_FD_Rough(it, uScattering, *light, uLight, scene, sampler,
//                             trans_sampler, light_0, light_h, arena,
//                             handleMedia);

//     light_0 /= lightPdf;
//     light_h /= lightPdf;
// }

// void UniformSampleOneLight_FD_Rough_Debiased(
//     const Interaction &it, const Scene &scene, MemoryArena &arena,
//     Sampler &sampler, Sampler &trans_sampler, Spectrum &light_0,
//     Spectrum &light_h, Spectrum &light_h1, Spectrum &light_h2, bool
//     handleMedia, const Distribution1D *lightDistrib) { ProfilePhase
//     p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     if (nLights == 0) {
//         light_0 = Spectrum(0.f);
//         light_h = Spectrum(0.f);
//         light_h1 = Spectrum(0.f);
//         light_h2 = Spectrum(0.f);
//         return;
//     }
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             light_0 = Spectrum(0.f);
//             light_h = Spectrum(0.f);
//             light_h1 = Spectrum(0.f);
//             light_h2 = Spectrum(0.f);
//             return;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     EstimateDirect_FD_Rough_Debiased(it, uScattering, *light, uLight, scene,
//                                      sampler, trans_sampler, light_0,
//                                      light_h, light_h1, light_h2, arena,
//                                      handleMedia);

//     light_0 /= lightPdf;
//     light_h /= lightPdf;
//     light_h1 /= lightPdf;
//     light_h2 /= lightPdf;
// }

// void UniformSampleOneLight_FD_Rough_Debiased_Center(
//     const Interaction &it, const Scene &scene, MemoryArena &arena,
//     Sampler &sampler, Sampler &trans_sampler, FD_Cor_Deb_Cent_Spectrum &beta,
//     bool handleMedia, const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     if (nLights == 0) {
//         beta.setAll(Spectrum(0.f));
//         return;
//     }
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             beta.setAll(Spectrum(0.f));
//             return;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     EstimateDirect_FD_Rough_Debiased_Center(it, uScattering, *light, uLight,
//                                             scene, sampler, trans_sampler,
//                                             beta, arena, handleMedia);

//     beta /= lightPdf;
// }

// FD_Cor_Sigma_T_Spectrum UniformSampleOneLight_Cor(
//     const Interaction &it, const Scene &scene, MemoryArena &arena,
//     Sampler &sampler, Sampler &trans_sampler,
//     const FD_Cor_Sigma_T_Spectrum &spec, bool handleMedia,
//     const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());
//     if (nLights == 0) {
//         FD_Cor_Sigma_T_Spectrum val = spec;
//         val.setAll(Spectrum(0.f));
//         return val;
//     }
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             FD_Cor_Sigma_T_Spectrum val = spec;
//             val.setAll(Spectrum(0.f));
//             return val;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     return EstimateDirect_Cor(it, uScattering, *light, uLight, scene,
//     sampler,
//                               trans_sampler, arena, spec, handleMedia) /
//            lightPdf;
// }

// FD_DebCor_Sigma_T_Spectrum UniformSampleOneLight_DebCor(
//     const Interaction &it, const Scene &scene, MemoryArena &arena,
//     Sampler &sampler, Sampler &trans_sampler,
//     const FD_DebCor_Sigma_T_Spectrum &spec, bool handleMedia,
//     const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());
//     if (nLights == 0) {
//         FD_DebCor_Sigma_T_Spectrum val = spec;
//         val.setAll(Spectrum(0.f));
//         return val;
//     }
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             FD_DebCor_Sigma_T_Spectrum val = spec;
//             val.setAll(Spectrum(0.f));
//             return val;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     return EstimateDirect_DebCor(it, uScattering, *light, uLight, scene,
//                                  sampler, trans_sampler, arena, spec,
//                                  handleMedia) /
//            lightPdf;
// }

Spectrum UniformSampleOneLight_ManualSig(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, bool handleMedia,
    const Distribution1D *lightDistrib, Float sigma_t) {
    ProfilePhase p(Prof::DirectLighting);
    // Randomly choose a single light to sample, _light_
    int nLights = int(scene.lights.size());

    // std::cout << "uni samp one light: " << sigma_t << std::endl;

    if (nLights == 0) return Spectrum(0.f);
    int lightNum;
    Float lightPdf;
    if (lightDistrib) {
        lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
        if (lightPdf == 0) return Spectrum(0.f);
    } else {
        lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
        lightPdf = Float(1) / nLights;
    }

    const std::shared_ptr<Light> &light = scene.lights[lightNum];

    Point2f uLight = sampler.Get2D();
    Point2f uScattering = sampler.Get2D();

    return EstimateDirect_ManualSig(it, uScattering, *light, uLight, scene,
                                    sampler, trans_sampler, arena, handleMedia,
                                    false, sigma_t) /
           lightPdf;
}

// void UniformSampleOneLightClampGeom(const Interaction &it, const Scene
// &scene,
//                                     MemoryArena &arena, Sampler &sampler,
//                                     Sampler &trans_sampler,
//                                     DebiasedClampingSpectrum &ret,
//                                     bool handleMedia,
//                                     const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());
//     if (nLights == 0) {
//         ret.setAll(0.f);
//         return;
//     }

//     int lightNum;
//     Float lightPdf;

//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) {
//             ret.setAll(0.f);
//             return;
//         }
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     EstimateDirectClampGeom(it, uScattering, *light, uLight, scene, sampler,
//                             trans_sampler, arena, ret, handleMedia);

//     ret /= lightPdf;
// }

Spectrum UniformSampleOneLight_NonExp(const Interaction &it, const Scene &scene,
                                      MemoryArena &arena, Sampler &sampler,
                                      Sampler &trans_sampler,
                                      const Distribution1D *lightDistrib) {
    ProfilePhase p(Prof::DirectLighting);
    // Randomly choose a single light to sample, _light_
    int nLights = int(scene.lights.size());

    if (nLights == 0) return Spectrum(0.f);
    int lightNum;
    Float lightPdf;
    if (lightDistrib) {
        lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
        if (lightPdf == 0) return Spectrum(0.f);
    } else {
        lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
        lightPdf = Float(1) / nLights;
    }

    const std::shared_ptr<Light> &light = scene.lights[lightNum];

    Point2f uLight = sampler.Get2D();
    Point2f uScattering = sampler.Get2D();

    return EstimateDirect_NonExp(it, uScattering, *light, uLight, scene,
                                 sampler, trans_sampler, arena) /
           lightPdf;
}

// Spectrum UniformSampleOneLight_NonExp_Beta(const Interaction &it,
//                                            const Scene &scene,
//                                            MemoryArena &arena, Float beta,
//                                            Float c, Sampler &sampler,
//                                            Sampler &trans_sampler,
//                                            const Distribution1D
//                                            *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     if (nLights == 0) return Spectrum(0.f);
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) return Spectrum(0.f);
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     return EstimateDirect_NonExp_Beta(it, uScattering, *light, uLight, beta,
//     c,
//                                       scene, sampler, trans_sampler, arena) /
//            lightPdf;
// }

BlanchetCorrelatedResults UniformSampleAllLights_NonExp_Blanchet(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, int blanchet_n) {
    ProfilePhase p(Prof::DirectLighting);

    BlanchetCorrelatedResults L(0.f);

    for (size_t j = 0; j < scene.lights.size(); ++j) {
        // Accumulate contribution of _j_th light to _L_
        const std::shared_ptr<Light> &light = scene.lights[j];
        int nSamples = 1;

        BlanchetCorrelatedResults Ld(0.f);
        for (int k = 0; k < nSamples; ++k) {
            Point2f uScattering = sampler.Get2D();
            Point2f uLight = sampler.Get2D();

            Ld += EstimateDirect_NonExp_Blanchet(
                it, uScattering, *light, uLight, scene, sampler, trans_sampler,
                arena, blanchet_n);
        }
        L += Ld / nSamples;
    }

    return L;
}

BlanchetCorrelatedResults UniformSampleOneLight_NonExp_Blanchet(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, int blanchet_n,
    const Distribution1D *lightDistrib) {
    ProfilePhase p(Prof::DirectLighting);
    // Randomly choose a single light to sample, _light_
    int nLights = int(scene.lights.size());

    if (nLights == 0) return Spectrum(0.f);
    int lightNum;
    Float lightPdf;
    if (lightDistrib) {
        lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
        if (lightPdf == 0) return Spectrum(0.f);
    } else {
        lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
        lightPdf = Float(1) / nLights;
    }

    const std::shared_ptr<Light> &light = scene.lights[lightNum];

    Point2f uLight = sampler.Get2D();
    Point2f uScattering = sampler.Get2D();

    return EstimateDirect_NonExp_Blanchet(it, uScattering, *light, uLight,
                                          scene, sampler, trans_sampler, arena,
                                          blanchet_n) /
           lightPdf;
}

BlanchetCorrelatedResults UniformSampleOneLight_Blanchet(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, int blanchet_n,
    const Distribution1D *lightDistrib) {
    ProfilePhase p(Prof::DirectLighting);
    // Randomly choose a single light to sample, _light_
    int nLights = int(scene.lights.size());

    if (nLights == 0) return Spectrum(0.f);
    int lightNum;
    Float lightPdf;
    if (lightDistrib) {
        lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
        if (lightPdf == 0) return Spectrum(0.f);
    } else {
        lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
        lightPdf = Float(1) / nLights;
    }

    const std::shared_ptr<Light> &light = scene.lights[lightNum];

    Point2f uLight = sampler.Get2D();
    Point2f uScattering = sampler.Get2D();

    return EstimateDirect_Blanchet(it, uScattering, *light, uLight, scene,
                                   sampler, trans_sampler, arena, blanchet_n,
                                   true) /
           lightPdf;
}

// Integrator Utility Functions
BlanchetCorrelatedResults UniformSampleAllLights_Blanchet(
    const Interaction &it, const Scene &scene, MemoryArena &arena,
    Sampler &sampler, Sampler &trans_sampler, int blanchet_n) {
    ProfilePhase p(Prof::DirectLighting);
    BlanchetCorrelatedResults L(0.f);
    for (size_t j = 0; j < scene.lights.size(); ++j) {
        // Accumulate contribution of _j_th light to _L_
        const std::shared_ptr<Light> &light = scene.lights[j];

        Point2f uLight = sampler.Get2D();
        Point2f uScattering = sampler.Get2D();
        L += EstimateDirect_Blanchet(it, uScattering, *light, uLight, scene,
                                     sampler, trans_sampler, arena, blanchet_n,
                                     true);
    }
    return L;
}

// BlanchetCorrelatedResults UniformSampleOneLight_NonExp_Blanchet_Beta(
//     const Interaction &it, const Scene &scene, MemoryArena &arena, Float
//     beta, Float c, Sampler &sampler, Sampler &trans_sampler, int blanchet_n,
//     const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     if (nLights == 0) return Spectrum(0.f);
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) return Spectrum(0.f);
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     return EstimateDirect_NonExp_Blanchet_Beta(
//                it, uScattering, *light, uLight, beta, c, scene, sampler,
//                trans_sampler, arena, blanchet_n) /
//            lightPdf;
// }

// BlanchetCorrelatedFdDebiasing UniformSampleOneLight_NonExp_Blanchet_FdBeta(
//     const Interaction &it, const Scene &scene, MemoryArena &arena,
//     Sampler &sampler, Sampler &trans_sampler, int blanchet_n, int davis_n,
//     Float base_h, Float davis_beta, Float davis_c,
//     const Distribution1D *lightDistrib) {
//     ProfilePhase p(Prof::DirectLighting);
//     // Randomly choose a single light to sample, _light_
//     int nLights = int(scene.lights.size());

//     if (nLights == 0) return Spectrum(0.f);
//     int lightNum;
//     Float lightPdf;
//     if (lightDistrib) {
//         lightNum = lightDistrib->SampleDiscrete(sampler.Get1D(), &lightPdf);
//         if (lightPdf == 0) return Spectrum(0.f);
//     } else {
//         lightNum = std::min((int)(sampler.Get1D() * nLights), nLights - 1);
//         lightPdf = Float(1) / nLights;
//     }

//     const std::shared_ptr<Light> &light = scene.lights[lightNum];

//     Point2f uLight = sampler.Get2D();
//     Point2f uScattering = sampler.Get2D();

//     return EstimateDirect_NonExp_Blanchet_FdBeta(
//                it, uScattering, *light, uLight, scene, sampler,
//                trans_sampler, arena, blanchet_n, davis_n, base_h, davis_beta,
//                davis_c) /
//            lightPdf;
// }

Spectrum EstimateDirect(const Interaction &it, const Point2f &uScattering,
                        const Light &light, const Point2f &uLight,
                        const Scene &scene, Sampler &sampler,
                        Sampler &trans_sampler, MemoryArena &arena,
                        bool handleMedia, bool specular) {
    BxDFType bsdfFlags =
        specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    Spectrum Ld(0.f);
    // Sample light source with multiple importance sampling
    Vector3f wi;
    Float lightPdf = 0, scatteringPdf = 0;
    VisibilityTester visibility;

    Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

    if (lightPdf > 0 && !Li.IsBlack()) {
        // Compute BSDF or phase function's value for light sample
        Spectrum f;
        if (it.IsSurfaceInteraction()) {
            // Evaluate BSDF for light sampling strategy
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

            f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
                AbsDot(wi, isect.shading.n);

            scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
        } else {
            // Evaluate phase function for light sampling strategy
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->p(mi.wo, wi);

            f = Spectrum(p);
            scatteringPdf = p;
        }
        if (!f.IsBlack()) {
            // Compute effect of visibility for light source sample
            if (handleMedia) {
                Li *= visibility.Tr(scene, trans_sampler);
            } else {
                if (!visibility.Unoccluded(scene)) {
                    Li = Spectrum(0.f);
                }
            }

            // Add light's contribution to reflected radiance
            if (!Li.IsBlack()) {
                if (IsDeltaLight(light.flags)) {
                    Ld += f * Li / lightPdf;
                } else {
                    Float weight =
                        PowerHeuristic(1, lightPdf, 1, scatteringPdf);

                    Ld += f * Li * weight / lightPdf;
                }
            }
        }
    }

    // Sample BSDF with multiple importance sampling
    if (!IsDeltaLight(light.flags)) {
        Spectrum f;
        bool sampledSpecular = false;
        if (it.IsSurfaceInteraction()) {
            // Sample scattered direction for surface interactions
            BxDFType sampledType;
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering, &scatteringPdf,
                                     bsdfFlags, &sampledType);
            f *= AbsDot(wi, isect.shading.n);
            sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
        } else {
            // Sample scattered direction for medium interactions
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
            f = Spectrum(p);
            scatteringPdf = p;
        }
        // #if LOGGING
        //         VLOG(2) << "  BSDF / phase sampling f: " << f
        //                 << ", scatteringPdf: " << scatteringPdf;
        // #endif
        if (!f.IsBlack() && scatteringPdf > 0) {
            // Account for light contributions along sampled direction _wi_
            Float weight = 1;
            if (!sampledSpecular) {
                lightPdf = light.Pdf_Li(it, wi);
                if (lightPdf == 0) return Ld;
                weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
            }

            // Find intersection and compute transmittance
            SurfaceInteraction lightIsect;
            Ray ray = it.SpawnRay(wi);
            Spectrum Tr(1.f);
            bool foundSurfaceInteraction =
                handleMedia ? scene.IntersectTr(ray, sampler, trans_sampler,
                                                &lightIsect, &Tr)
                            : scene.Intersect(ray, &lightIsect);
            // std::cout << "TR POST: " << Tr << std::endl;

            // Add light contribution from material sampling
            Spectrum Li(0.f);
            if (foundSurfaceInteraction) {
                if (lightIsect.primitive->GetAreaLight() == &light)
                    Li = lightIsect.Le(-wi);
            } else
                Li = light.Le(ray);
            if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
        }
    }

    return Ld;
}

// void EstimateDirect_LiH(const Interaction &it, const Point2f &uScattering,
//                         const Light &light, const Point2f &uLight,
//                         const Scene &scene, Sampler &sampler,
//                         Sampler &trans_sampler, Spectrum &Li, Spectrum &Lh,
//                         MemoryArena &arena, bool handleMedia, bool specular)
//                         {
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     Spectrum Ld(0.f);
//     Spectrum Ld_h(0.f);
//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Vector3f wi_h;
//     Float lightPdf = 0;
//     Float lightPdf_h = 0;
//     Float scatteringPdf = 0;
//     Float scatteringPdf_h = 0;

//     VisibilityTester visibility;
//     VisibilityTester visibility_h;

//     Spectrum light_contr =
//         light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);
//     Spectrum light_contr_h =
//         light.Sample_Li_H(it, uLight, &wi_h, &lightPdf_h, &visibility_h);

//     // std::cout << "light_contr: " << light_contr << std::endl;
//     // std::cout << "light pdf: " << lightPdf << std::endl;

//     if (lightPdf > 0 && !light_contr.IsBlack()) {
//         // std::cout << "insied here" << std::endl;
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         Spectrum f_h;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);
//             f_h = isect.bsdf->f(isect.wo, wi_h, bsdfFlags) *
//                   AbsDot(wi_h, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//             scatteringPdf_h = isect.bsdf->Pdf(isect.wo, wi_h, bsdfFlags);
//         } else {
//             std::cout << "ERROR: this is not currently supported" <<
//             std::endl;
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
//         if (!f.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 light_contr *= visibility.Tr(scene, trans_sampler);
//             } else {
//                 if (!visibility.Unoccluded(scene)) {
//                     light_contr = Spectrum(0.f);
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!light_contr.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += f * light_contr / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += f * light_contr * weight / lightPdf;
//                 }
//             }
//         }
//         if (!f_h.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 light_contr_h *= visibility.Tr(scene, trans_sampler);
//             } else {
//                 if (!visibility_h.Unoccluded(scene)) {
//                     light_contr_h = Spectrum(0.f);
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!light_contr_h.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld_h += f_h * light_contr_h / lightPdf_h;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf_h, 1, scatteringPdf_h);

//                     Ld_h += f * light_contr_h * weight / lightPdf_h;
//                 }
//             }
//         }
//     }

//     // std::cout << "LD: " << Ld << std::endl;

//     Li += Ld;
//     Lh += Ld_h;

//     // Sample BSDF with multiple importance sampling
//     // this remains unchanged for now: TODO: edit this in the future
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         Spectrum f_h;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);
//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;

//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) {
//                     return;
//                 }
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             Spectrum Tr(1.f);
//             bool foundSurfaceInteraction =
//                 handleMedia ? scene.IntersectTr(ray, sampler, trans_sampler,
//                                                 &lightIsect, &Tr)
//                             : scene.Intersect(ray, &lightIsect);
//             // std::cout << "TR POST: " << Tr << std::endl;

//             // Add light contribution from material sampling
//             Spectrum light_contr(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     light_contr = lightIsect.Le(-wi);
//             } else
//                 light_contr = light.Le(ray);
//             if (!light_contr.IsBlack()) {
//                 Li += f * light_contr * Tr * weight / scatteringPdf;
//                 Lh += f * light_contr * Tr * weight / scatteringPdf;
//             }
//         }
//     }
// }

// Spectrum EstimateDirect_Dum(const Interaction &it, const Point2f
// &uScattering,
//                             int n, int typ, const Light &light,
//                             const Point2f &uLight, const Scene &scene,
//                             Sampler &sampler, Sampler &trans_sampler,
//                             MemoryArena &arena, bool handleMedia,
//                             bool specular) {
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     Spectrum Ld(0.f);
//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
//         if (!f.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 Li *= visibility.Tr_Dum(scene, trans_sampler, n, typ);
//             } else {
//                 if (!visibility.Unoccluded(scene)) {
//                     Li = Spectrum(0.f);
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += f * Li / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += f * Li * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);
//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
// #if LOGGING
//         VLOG(2) << "  BSDF / phase sampling f: " << f
//                 << ", scatteringPdf: " << scatteringPdf;
// #endif
//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) return Ld;
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             Spectrum Tr(1.f);
//             bool foundSurfaceInteraction =
//                 handleMedia ? scene.IntersectTr_Dum(ray, sampler,
//                 trans_sampler,
//                                                     n, typ, &lightIsect, &Tr)
//                             : scene.Intersect(ray, &lightIsect);
//             // std::cout << "TR POST: " << Tr << std::endl;

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);
//             if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
//         }
//     }

//     return Ld;
// }

// void EstimateDirect_CorP(const Interaction &it, const Point2f &uScattering,
//                          const Light &light, const Point2f &uLight,
//                          const Scene &scene, Sampler &sampler,
//                          Sampler &trans_sampler, FD_Cor_Spectrum &val,
//                          MemoryArena &arena, bool handleMedia, bool specular)
//                          {
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);

//     FD_Cor_Spectrum Ld = val;
//     Ld.setAll(Spectrum(0.f));
//     // Sample light source with multiple importance sampling
//     Vector3f wi_0;
//     Vector3f wi_h;

//     Float lightPdf_0 = 0;
//     Float lightPdf_h = 0;
//     Float scatteringPdf_0 = 0;
//     Float scatteringPdf_h = 0;

//     VisibilityTester visibility_0;
//     VisibilityTester visibility_h;

//     Spectrum Li_0 =
//         light.Sample_Li(it, uLight, 0.0, &wi_0, &lightPdf_0, &visibility_0);
//     Spectrum Li_H =
//         light.Sample_Li(it, uLight, val.h, &wi_h, &lightPdf_h,
//         &visibility_h);

//     // std::cout << "h: " << val.h << std::endl;
//     // std::cout << "li0: " << Li_0 << std::endl;
//     // std::cout << "lih: " << Li_H << std::endl;
//     // std::cout << std::endl;

//     if (lightPdf_0 > 0 && !Li_0.IsBlack()) {
//         // std::cout << "fuf" << std::endl;
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f_0;
//         Spectrum f_h;

//         // std::cout << "faf" << std::endl;

//         if (it.IsSurfaceInteraction()) {
//             // std::cout << "wat" << std::endl;
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f_0 = isect.bsdf->f(isect.wo, wi_0, bsdfFlags) *
//                   AbsDot(wi_0, isect.shading.n);

//             f_h = isect.bsdf->f(isect.wo, wi_h, bsdfFlags) *
//                   AbsDot(wi_h, isect.shading.n);

//             scatteringPdf_0 = isect.bsdf->Pdf(isect.wo, wi_0, bsdfFlags);
//             scatteringPdf_h = isect.bsdf->Pdf(isect.wo, wi_h, bsdfFlags);

//             // std::cout << "computed surff" << std::endl;
//         } else {
//             // std::cout << "feck" << std::endl;
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;

//             // std::cout << "prephase" << std::endl;
//             Float p_0 = mi.phase->p(mi.wo, wi_0);
//             Float p_h = mi.phase->p(mi.wo, wi_h);
//             // std::cout << "postphase" << std::endl;

//             f_0 = Spectrum(p_0);
//             f_h = Spectrum(p_h);

//             scatteringPdf_0 = p_0;
//             scatteringPdf_h = p_h;
//         }
//         // TODO: change these checks to apply to each term individually
//         //       for the current tests it doesn't matter, since there will
//         not
//         //       be cases where the terms are black.
//         if (!f_0.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 // std::cout << "handling media" << std::endl;
//                 Li_0 *= visibility_0.Tr(scene, trans_sampler);
//                 Li_H *= visibility_h.Tr(scene, trans_sampler);
//             } else {
//                 if (!visibility_0.Unoccluded(scene)) {
//                     Li_0 = Spectrum(0.f);
//                 }
//                 if (!visibility_h.Unoccluded(scene)) {
//                     Li_H = Spectrum(0.f);
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li_0.IsBlack()) {
//                 // std::cout << "blep" << std::endl;
//                 if (IsDeltaLight(light.flags)) {
//                     Ld.L_0 += f_0 * Li_0 / lightPdf_0;
//                     Ld.L_H += f_h * Li_H / lightPdf_h;
//                 } else {
//                     Float weight_0 =
//                         PowerHeuristic(1, lightPdf_0, 1, scatteringPdf_0);
//                     Float weight_h =
//                         PowerHeuristic(1, lightPdf_h, 1, scatteringPdf_h);

//                     // Ld += f * Li * weight / lightPdf;

//                     Ld.L_0 += f_0 * Li_0 * weight_0 / lightPdf_0;
//                     Ld.L_H += f_h * Li_H * weight_h / lightPdf_h;
//                 }
//             }
//         }
//     }

//     // std::cout << "end" << std::endl;

//     val = Ld;

//     // TODO: support this later
//     // // Sample BSDF with multiple importance sampling
//     // if (!IsDeltaLight(light.flags))
//     // {
//     //     Spectrum f;
//     //     bool sampledSpecular = false;
//     //     if (it.IsSurfaceInteraction()) {
//     //         // Sample scattered direction for surface interactions
//     //         BxDFType sampledType;
//     //         const SurfaceInteraction &isect = (const SurfaceInteraction
//     &)it;
//     //         f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//     //         &scatteringPdf,
//     //                                  bsdfFlags, &sampledType);
//     //         f *= AbsDot(wi, isect.shading.n);
//     //         sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//     //     } else {
//     //         // Sample scattered direction for medium interactions
//     //         const MediumInteraction &mi = (const MediumInteraction &)it;
//     //         Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//     //         f = Spectrum(p);
//     //         scatteringPdf = p;
//     //     }
//     //     #if LOGGING
//     //         VLOG(2) << "  BSDF / phase sampling f: " << f << ",
//     //         scatteringPdf: " <<
//     //             scatteringPdf;
//     //     #endif
//     //     if (!f.IsBlack() && scatteringPdf > 0) {
//     //         // Account for light contributions along sampled direction
//     _wi_
//     //         Float weight = 1;
//     //         if (!sampledSpecular) {
//     //             lightPdf = light.Pdf_Li(it, wi);
//     //             if (lightPdf == 0) return Ld;
//     //             weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//     //         }
//     //
//     //         // Find intersection and compute transmittance
//     //         SurfaceInteraction lightIsect;
//     //         Ray ray = it.SpawnRay(wi);
//     //         Spectrum Tr(1.f);
//     //         bool foundSurfaceInteraction =
//     //             handleMedia ? scene.IntersectTr(ray,
//     //                                             sampler,
//     //                                             trans_sampler,
//     //                                             &lightIsect,
//     //                                             &Tr)
//     //                         : scene.Intersect(ray, &lightIsect);
//     //         // std::cout << "TR POST: " << Tr << std::endl;
//     //
//     //         // Add light contribution from material sampling
//     //         Spectrum Li(0.f);
//     //         if (foundSurfaceInteraction) {
//     //             if (lightIsect.primitive->GetAreaLight() == &light)
//     //                 Li = lightIsect.Le(-wi);
//     //         } else
//     //             Li = light.Le(ray);
//     //         if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
//     //     }
//     // }

//     // return Ld;
// }

// void EstimateDirect_CorP_Deb(const Interaction &it, const Point2f
// &uScattering,
//                              const Light &light, const Point2f &uLight,
//                              const Scene &scene, Sampler &sampler,
//                              Sampler &trans_sampler, FD_Cor_Deb_Spectrum
//                              &val, MemoryArena &arena, bool handleMedia, bool
//                              specular) {
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);

//     FD_Cor_Deb_Spectrum Ld = val;
//     Ld.setAll(Spectrum(0.f));
//     // Sample light source with multiple importance sampling
//     Vector3f wi_0;
//     Vector3f wi_h;
//     Vector3f wi_h1;
//     Vector3f wi_h2;

//     Float lightPdf_0 = 0;
//     Float lightPdf_h = 0;
//     Float lightPdf_h1 = 0;
//     Float lightPdf_h2 = 0;
//     Float scatteringPdf_0 = 0;
//     Float scatteringPdf_h = 0;
//     Float scatteringPdf_h1 = 0;
//     Float scatteringPdf_h2 = 0;

//     VisibilityTester visibility_0;
//     VisibilityTester visibility_h;
//     VisibilityTester visibility_h1;
//     VisibilityTester visibility_h2;

//     Spectrum Li_0 =
//         light.Sample_Li(it, uLight, 0.0, &wi_0, &lightPdf_0, &visibility_0);
//     Spectrum Li_H =
//         light.Sample_Li(it, uLight, val.h, &wi_h, &lightPdf_h,
//         &visibility_h);
//     Spectrum Li_H1 = light.Sample_Li(it, uLight, val.h1, &wi_h1,
//     &lightPdf_h1,
//                                      &visibility_h1);
//     Spectrum Li_H2 = light.Sample_Li(it, uLight, val.h2, &wi_h2,
//     &lightPdf_h2,
//                                      &visibility_h2);

//     if (lightPdf_0 > 0 && !Li_0.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f_0;
//         Spectrum f_h;
//         Spectrum f_h1;
//         Spectrum f_h2;

//         if (it.IsSurfaceInteraction()) {
//             // std::cout << "wat" << std::endl;
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f_0 = isect.bsdf->f(isect.wo, wi_0, bsdfFlags) *
//                   AbsDot(wi_0, isect.shading.n);

//             f_h = isect.bsdf->f(isect.wo, wi_h, bsdfFlags) *
//                   AbsDot(wi_h, isect.shading.n);

//             f_h1 = isect.bsdf->f(isect.wo, wi_h1, bsdfFlags) *
//                    AbsDot(wi_h1, isect.shading.n);

//             f_h2 = isect.bsdf->f(isect.wo, wi_h2, bsdfFlags) *
//                    AbsDot(wi_h2, isect.shading.n);

//             scatteringPdf_0 = isect.bsdf->Pdf(isect.wo, wi_0, bsdfFlags);
//             scatteringPdf_h = isect.bsdf->Pdf(isect.wo, wi_h, bsdfFlags);
//             scatteringPdf_h1 = isect.bsdf->Pdf(isect.wo, wi_h1, bsdfFlags);
//             scatteringPdf_h2 = isect.bsdf->Pdf(isect.wo, wi_h2, bsdfFlags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;

//             // std::cout << "prephase" << std::endl;
//             Float p_0 = mi.phase->p(mi.wo, wi_0);
//             Float p_h = mi.phase->p(mi.wo, wi_h);
//             Float p_h1 = mi.phase->p(mi.wo, wi_h1);
//             Float p_h2 = mi.phase->p(mi.wo, wi_h2);

//             f_0 = Spectrum(p_0);
//             f_h = Spectrum(p_h);
//             f_h1 = Spectrum(p_h1);
//             f_h2 = Spectrum(p_h2);

//             scatteringPdf_0 = p_0;
//             scatteringPdf_h = p_h;
//             scatteringPdf_h1 = p_h1;
//             scatteringPdf_h2 = p_h2;
//         }
//         // TODO: change these checks to apply to each term individually
//         //       for the current tests it doesn't matter, since there will
//         not
//         //       be cases where the terms are black.
//         if (!f_0.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 // std::cout << "handling media" << std::endl;
//                 Li_0 *= visibility_0.Tr(scene, trans_sampler);
//                 Li_H *= visibility_h.Tr(scene, trans_sampler);
//                 Li_H1 *= visibility_h1.Tr(scene, trans_sampler);
//                 Li_H2 *= visibility_h2.Tr(scene, trans_sampler);
//             } else {
//                 if (!visibility_0.Unoccluded(scene)) {
//                     Li_0 = Spectrum(0.f);
//                 }
//                 if (!visibility_h.Unoccluded(scene)) {
//                     Li_H = Spectrum(0.f);
//                 }
//                 if (!visibility_h1.Unoccluded(scene)) {
//                     Li_H1 = Spectrum(0.f);
//                 }
//                 if (!visibility_h2.Unoccluded(scene)) {
//                     Li_H2 = Spectrum(0.f);
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li_0.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld.L_0 += f_0 * Li_0 / lightPdf_0;
//                 } else {
//                     Float weight_0 =
//                         PowerHeuristic(1, lightPdf_0, 1, scatteringPdf_0);
//                     Ld.L_0 += f_0 * Li_0 * weight_0 / lightPdf_0;
//                 }
//             }

//             if (!Li_H.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld.L_H += f_h * Li_H / lightPdf_h;
//                 } else {
//                     Float weight_h =
//                         PowerHeuristic(1, lightPdf_h, 1, scatteringPdf_h);
//                     Ld.L_H += f_h * Li_H * weight_h / lightPdf_h;
//                 }
//             }

//             if (!Li_H1.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld.L_H1 += f_h1 * Li_H1 / lightPdf_h1;
//                 } else {
//                     Float weight_h1 =
//                         PowerHeuristic(1, lightPdf_h1, 1, scatteringPdf_h1);
//                     Ld.L_H1 += f_h1 * Li_H1 * weight_h1 / lightPdf_h1;
//                 }
//             }

//             if (!Li_H2.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld.L_H2 += f_h2 * Li_H2 / lightPdf_h2;
//                 } else {
//                     Float weight_h2 =
//                         PowerHeuristic(1, lightPdf_h2, 1, scatteringPdf_h2);
//                     Ld.L_H2 += f_h2 * Li_H2 * weight_h2 / lightPdf_h2;
//                 }
//             }
//         }
//     }

//     val = Ld;

//     // TODO: support this later
//     // // Sample BSDF with multiple importance sampling
//     // if (!IsDeltaLight(light.flags))
//     // {
//     //     Spectrum f;
//     //     bool sampledSpecular = false;
//     //     if (it.IsSurfaceInteraction()) {
//     //         // Sample scattered direction for surface interactions
//     //         BxDFType sampledType;
//     //         const SurfaceInteraction &isect = (const SurfaceInteraction
//     &)it;
//     //         f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//     //         &scatteringPdf,
//     //                                  bsdfFlags, &sampledType);
//     //         f *= AbsDot(wi, isect.shading.n);
//     //         sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//     //     } else {
//     //         // Sample scattered direction for medium interactions
//     //         const MediumInteraction &mi = (const MediumInteraction &)it;
//     //         Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//     //         f = Spectrum(p);
//     //         scatteringPdf = p;
//     //     }
//     //     #if LOGGING
//     //         VLOG(2) << "  BSDF / phase sampling f: " << f << ",
//     //         scatteringPdf: " <<
//     //             scatteringPdf;
//     //     #endif
//     //     if (!f.IsBlack() && scatteringPdf > 0) {
//     //         // Account for light contributions along sampled direction
//     _wi_
//     //         Float weight = 1;
//     //         if (!sampledSpecular) {
//     //             lightPdf = light.Pdf_Li(it, wi);
//     //             if (lightPdf == 0) return Ld;
//     //             weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//     //         }
//     //
//     //         // Find intersection and compute transmittance
//     //         SurfaceInteraction lightIsect;
//     //         Ray ray = it.SpawnRay(wi);
//     //         Spectrum Tr(1.f);
//     //         bool foundSurfaceInteraction =
//     //             handleMedia ? scene.IntersectTr(ray,
//     //                                             sampler,
//     //                                             trans_sampler,
//     //                                             &lightIsect,
//     //                                             &Tr)
//     //                         : scene.Intersect(ray, &lightIsect);
//     //         // std::cout << "TR POST: " << Tr << std::endl;
//     //
//     //         // Add light contribution from material sampling
//     //         Spectrum Li(0.f);
//     //         if (foundSurfaceInteraction) {
//     //             if (lightIsect.primitive->GetAreaLight() == &light)
//     //                 Li = lightIsect.Le(-wi);
//     //         } else
//     //             Li = light.Le(ray);
//     //         if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
//     //     }
//     // }

//     // return Ld;
// }

// void EstimateDirect_CorP_Deb_Cent(
//     const Interaction &it, const Point2f &uScattering, const Light &light,
//     const Point2f &uLight, const Scene &scene, Sampler &sampler,
//     Sampler &trans_sampler, FD_Cor_Deb_Cent_Spectrum &val, MemoryArena
//     &arena, bool handleMedia, bool specular) {
//     // this method assumes a scene with a single point light
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);

//     FD_Cor_Deb_Cent_Spectrum Ld = val;
//     Ld.setAll(Spectrum(0.f));
//     // Sample light source with multiple importance sampling
//     Vector3f wi_h00;
//     Vector3f wi_h01;
//     Vector3f wi_h10;
//     Vector3f wi_h11;
//     Vector3f wi_h20;
//     Vector3f wi_h21;

//     Float lightPdf_h00 = 0;
//     Float lightPdf_h01 = 0;
//     Float lightPdf_h10 = 0;
//     Float lightPdf_h11 = 0;
//     Float lightPdf_h20 = 0;
//     Float lightPdf_h21 = 0;

//     Float scatteringPdf_h00 = 0;
//     Float scatteringPdf_h01 = 0;
//     Float scatteringPdf_h10 = 0;
//     Float scatteringPdf_h11 = 0;
//     Float scatteringPdf_h20 = 0;
//     Float scatteringPdf_h21 = 0;

//     VisibilityTester visibility_h00;
//     VisibilityTester visibility_h01;
//     VisibilityTester visibility_h10;
//     VisibilityTester visibility_h11;
//     VisibilityTester visibility_h20;
//     VisibilityTester visibility_h21;

//     Spectrum Li_H00 = light.Sample_Li(it, uLight, -val.h, &wi_h00,
//                                       &lightPdf_h00, &visibility_h00);
//     Spectrum Li_H01 = light.Sample_Li(it, uLight, val.h, &wi_h01,
//     &lightPdf_h01,
//                                       &visibility_h01);
//     Spectrum Li_H10 = light.Sample_Li(it, uLight, -val.h1, &wi_h10,
//                                       &lightPdf_h10, &visibility_h10);
//     Spectrum Li_H11 = light.Sample_Li(it, uLight, val.h1, &wi_h11,
//                                       &lightPdf_h11, &visibility_h11);
//     Spectrum Li_H20 = light.Sample_Li(it, uLight, -val.h2, &wi_h20,
//                                       &lightPdf_h20, &visibility_h20);
//     Spectrum Li_H21 = light.Sample_Li(it, uLight, val.h2, &wi_h21,
//                                       &lightPdf_h21, &visibility_h21);

//     Spectrum f_h00;
//     Spectrum f_h01;
//     Spectrum f_h10;
//     Spectrum f_h11;
//     Spectrum f_h20;
//     Spectrum f_h21;

//     const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//     f_h00 = isect.bsdf->f(isect.wo, wi_h00, bsdfFlags) *
//             AbsDot(wi_h00, isect.shading.n);
//     f_h01 = isect.bsdf->f(isect.wo, wi_h01, bsdfFlags) *
//             AbsDot(wi_h01, isect.shading.n);
//     f_h10 = isect.bsdf->f(isect.wo, wi_h10, bsdfFlags) *
//             AbsDot(wi_h10, isect.shading.n);
//     f_h11 = isect.bsdf->f(isect.wo, wi_h11, bsdfFlags) *
//             AbsDot(wi_h11, isect.shading.n);
//     f_h20 = isect.bsdf->f(isect.wo, wi_h20, bsdfFlags) *
//             AbsDot(wi_h20, isect.shading.n);
//     f_h21 = isect.bsdf->f(isect.wo, wi_h21, bsdfFlags) *
//             AbsDot(wi_h21, isect.shading.n);

//     scatteringPdf_h00 = isect.bsdf->Pdf(isect.wo, wi_h00, bsdfFlags);
//     scatteringPdf_h01 = isect.bsdf->Pdf(isect.wo, wi_h01, bsdfFlags);
//     scatteringPdf_h10 = isect.bsdf->Pdf(isect.wo, wi_h10, bsdfFlags);
//     scatteringPdf_h11 = isect.bsdf->Pdf(isect.wo, wi_h11, bsdfFlags);
//     scatteringPdf_h20 = isect.bsdf->Pdf(isect.wo, wi_h20, bsdfFlags);
//     scatteringPdf_h21 = isect.bsdf->Pdf(isect.wo, wi_h21, bsdfFlags);

//     if (!visibility_h00.Unoccluded(scene)) Li_H00 = Spectrum(0.f);
//     if (!visibility_h01.Unoccluded(scene)) Li_H01 = Spectrum(0.f);
//     if (!visibility_h10.Unoccluded(scene)) Li_H10 = Spectrum(0.f);
//     if (!visibility_h11.Unoccluded(scene)) Li_H11 = Spectrum(0.f);
//     if (!visibility_h20.Unoccluded(scene)) Li_H20 = Spectrum(0.f);
//     if (!visibility_h21.Unoccluded(scene)) Li_H21 = Spectrum(0.f);

//     Ld.L_H00 += f_h00 * Li_H00 / lightPdf_h00;
//     Ld.L_H01 += f_h01 * Li_H01 / lightPdf_h01;
//     Ld.L_H10 += f_h10 * Li_H10 / lightPdf_h10;
//     Ld.L_H11 += f_h11 * Li_H11 / lightPdf_h11;
//     Ld.L_H20 += f_h20 * Li_H20 / lightPdf_h20;
//     Ld.L_H21 += f_h21 * Li_H21 / lightPdf_h21;

//     val = Ld;

//     // if (lightPdf_0 > 0 && !Li_0.IsBlack())
//     // {
//     //     // Compute BSDF or phase function's value for light sample
//     //     Spectrum f_0;
//     //     Spectrum f_h;
//     //     Spectrum f_h1;
//     //     Spectrum f_h2;
//     //
//     //     if (it.IsSurfaceInteraction())
//     //     {
//     //         // std::cout << "wat" << std::endl;
//     //         // Evaluate BSDF for light sampling strategy
//     //         const SurfaceInteraction &isect = (const SurfaceInteraction
//     &)it;
//     //
//     //         f_0 = isect.bsdf->f(isect.wo, wi_0, bsdfFlags) *
//     //               AbsDot(wi_0, isect.shading.n);
//     //
//     //         f_h = isect.bsdf->f(isect.wo, wi_h, bsdfFlags) *
//     //               AbsDot(wi_h, isect.shading.n);
//     //
//     //         f_h1 = isect.bsdf->f(isect.wo, wi_h1, bsdfFlags) *
//     //                AbsDot(wi_h1, isect.shading.n);
//     //
//     //         f_h2 = isect.bsdf->f(isect.wo, wi_h2, bsdfFlags) *
//     //                AbsDot(wi_h2, isect.shading.n);
//     //
//     //         scatteringPdf_0 = isect.bsdf->Pdf(isect.wo, wi_0, bsdfFlags);
//     //         scatteringPdf_h = isect.bsdf->Pdf(isect.wo, wi_h, bsdfFlags);
//     //         scatteringPdf_h1 = isect.bsdf->Pdf(isect.wo, wi_h1,
//     bsdfFlags);
//     //         scatteringPdf_h2 = isect.bsdf->Pdf(isect.wo, wi_h2,
//     bsdfFlags);
//     //     } else {
//     //         // Evaluate phase function for light sampling strategy
//     //         const MediumInteraction &mi = (const MediumInteraction &)it;
//     //
//     //         // std::cout << "prephase" << std::endl;
//     //         Float p_0 = mi.phase->p(mi.wo, wi_0);
//     //         Float p_h = mi.phase->p(mi.wo, wi_h);
//     //         Float p_h1 = mi.phase->p(mi.wo, wi_h1);
//     //         Float p_h2 = mi.phase->p(mi.wo, wi_h2);
//     //
//     //         f_0 = Spectrum(p_0);
//     //         f_h = Spectrum(p_h);
//     //         f_h1 = Spectrum(p_h1);
//     //         f_h2 = Spectrum(p_h2);
//     //
//     //         scatteringPdf_0 = p_0;
//     //         scatteringPdf_h = p_h;
//     //         scatteringPdf_h1 = p_h1;
//     //         scatteringPdf_h2 = p_h2;
//     //     }
//     //     // TODO: change these checks to apply to each term individually
//     //     //       for the current tests it doesn't matter, since there will
//     //     not
//     //     //       be cases where the terms are black.
//     //     if (!f_0.IsBlack())
//     //     {
//     //         // Compute effect of visibility for light source sample
//     //         if (handleMedia)
//     //         {
//     //             // std::cout << "handling media" << std::endl;
//     //             Li_0 *= visibility_0.Tr(scene, trans_sampler);
//     //             Li_H *= visibility_h.Tr(scene, trans_sampler);
//     //             Li_H1 *= visibility_h1.Tr(scene, trans_sampler);
//     //             Li_H2 *= visibility_h2.Tr(scene, trans_sampler);
//     //         }
//     //         else
//     //         {
//     //             if (!visibility_0.Unoccluded(scene))
//     //             {
//     //                 Li_0 = Spectrum(0.f);
//     //             }
//     //             if (!visibility_h.Unoccluded(scene))
//     //             {
//     //                 Li_H = Spectrum(0.f);
//     //             }
//     //             if (!visibility_h1.Unoccluded(scene))
//     //             {
//     //                 Li_H1 = Spectrum(0.f);
//     //             }
//     //             if (!visibility_h2.Unoccluded(scene))
//     //             {
//     //                 Li_H2 = Spectrum(0.f);
//     //             }
//     //         }
//     //
//     //         // Add light's contribution to reflected radiance
//     //         if (!Li_0.IsBlack())
//     //         {
//     //             if (IsDeltaLight(light.flags))
//     //             {
//     //                 Ld.L_0 += f_0 * Li_0 / lightPdf_0;
//     //             }
//     //             else
//     //             {
//     //                 Float weight_0 = PowerHeuristic(1, lightPdf_0, 1,
//     //                 scatteringPdf_0); Ld.L_0 += f_0 * Li_0 * weight_0 /
//     //                 lightPdf_0;
//     //             }
//     //         }
//     //
//     //         if (!Li_H.IsBlack())
//     //         {
//     //             if (IsDeltaLight(light.flags))
//     //             {
//     //                 Ld.L_H += f_h * Li_H / lightPdf_h;
//     //             }
//     //             else
//     //             {
//     //                 Float weight_h = PowerHeuristic(1, lightPdf_h, 1,
//     //                 scatteringPdf_h); Ld.L_H += f_h * Li_H * weight_h /
//     //                 lightPdf_h;
//     //             }
//     //         }
//     //
//     //         if (!Li_H1.IsBlack())
//     //         {
//     //             if (IsDeltaLight(light.flags))
//     //             {
//     //                 Ld.L_H1 += f_h1 * Li_H1 / lightPdf_h1;
//     //             }
//     //             else
//     //             {
//     //                 Float weight_h1 = PowerHeuristic(1, lightPdf_h1, 1,
//     //                 scatteringPdf_h1); Ld.L_H1 += f_h1 * Li_H1 * weight_h1
//     /
//     //                 lightPdf_h1;
//     //             }
//     //         }
//     //
//     //         if (!Li_H2.IsBlack())
//     //         {
//     //             if (IsDeltaLight(light.flags))
//     //             {
//     //                 Ld.L_H2 += f_h2 * Li_H2 / lightPdf_h2;
//     //             }
//     //             else
//     //             {
//     //                 Float weight_h2 = PowerHeuristic(1, lightPdf_h2, 1,
//     //                 scatteringPdf_h2); Ld.L_H2 += f_h2 * Li_H2 * weight_h2
//     /
//     //                 lightPdf_h2;
//     //             }
//     //         }
//     //     }
//     // }
//     //
//     // val = Ld;

//     // TODO: support this later
//     // // Sample BSDF with multiple importance sampling
//     // if (!IsDeltaLight(light.flags))
//     // {
//     //     Spectrum f;
//     //     bool sampledSpecular = false;
//     //     if (it.IsSurfaceInteraction()) {
//     //         // Sample scattered direction for surface interactions
//     //         BxDFType sampledType;
//     //         const SurfaceInteraction &isect = (const SurfaceInteraction
//     &)it;
//     //         f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//     //         &scatteringPdf,
//     //                                  bsdfFlags, &sampledType);
//     //         f *= AbsDot(wi, isect.shading.n);
//     //         sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//     //     } else {
//     //         // Sample scattered direction for medium interactions
//     //         const MediumInteraction &mi = (const MediumInteraction &)it;
//     //         Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//     //         f = Spectrum(p);
//     //         scatteringPdf = p;
//     //     }
//     //     #if LOGGING
//     //         VLOG(2) << "  BSDF / phase sampling f: " << f << ",
//     //         scatteringPdf: " <<
//     //             scatteringPdf;
//     //     #endif
//     //     if (!f.IsBlack() && scatteringPdf > 0) {
//     //         // Account for light contributions along sampled direction
//     _wi_
//     //         Float weight = 1;
//     //         if (!sampledSpecular) {
//     //             lightPdf = light.Pdf_Li(it, wi);
//     //             if (lightPdf == 0) return Ld;
//     //             weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//     //         }
//     //
//     //         // Find intersection and compute transmittance
//     //         SurfaceInteraction lightIsect;
//     //         Ray ray = it.SpawnRay(wi);
//     //         Spectrum Tr(1.f);
//     //         bool foundSurfaceInteraction =
//     //             handleMedia ? scene.IntersectTr(ray,
//     //                                             sampler,
//     //                                             trans_sampler,
//     //                                             &lightIsect,
//     //                                             &Tr)
//     //                         : scene.Intersect(ray, &lightIsect);
//     //         // std::cout << "TR POST: " << Tr << std::endl;
//     //
//     //         // Add light contribution from material sampling
//     //         Spectrum Li(0.f);
//     //         if (foundSurfaceInteraction) {
//     //             if (lightIsect.primitive->GetAreaLight() == &light)
//     //                 Li = lightIsect.Le(-wi);
//     //         } else
//     //             Li = light.Le(ray);
//     //         if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
//     //     }
//     // }

//     // return Ld;
// }

// void EstimateDirect_FD_Rough(const Interaction &it, const Point2f
// &uScattering,
//                              const Light &light, const Point2f &uLight,
//                              const Scene &scene, Sampler &sampler,
//                              Sampler &trans_sampler, Spectrum &light_0,
//                              Spectrum &light_h, MemoryArena &arena,
//                              bool handleMedia, bool specular) {
//     // std::cout << "inside estimate direct" << std::endl;
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     Spectrum Ld_0(0.f);
//     Spectrum Ld_H(0.f);
//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f_0;
//         Spectrum f_h;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction_CorBSDF &isect =
//                 (const SurfaceInteraction_CorBSDF &)it;

//             // std::cout << "about to evaluate bsdfs" << std::endl;

//             f_0 = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                   AbsDot(wi, isect.shading.n);

//             // std::cout << "evaluating second bsdf" << std::endl;

//             if (isect.bsdf_h) {
//                 f_h = isect.bsdf_h->f(isect.wo, wi, bsdfFlags) *
//                       AbsDot(wi, isect.shading.n);
//             } else {
//                 f_h = f_0;
//             }

//             // std::cout << "post evaluating bsdfs" << std::endl;

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//         } else {
//             light_0 = Spectrum(0.f);
//             light_h = Spectrum(0.f);
//             return;
//         }
//         if (!f_0.IsBlack()) {
//             if (!visibility.Unoccluded(scene)) {
//                 Li = Spectrum(0.f);
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld_0 += f_0 * Li / lightPdf;
//                     Ld_H += f_h * Li / lightPdf;
//                 } else {
//                     // Float weight = PowerHeuristic(1, lightPdf, 1,
//                     // scatteringPdf);

//                     // Ld += f * Li * weight / lightPdf;
//                     Ld_0 += f_0 * Li / lightPdf;
//                     Ld_H += f_h * Li / lightPdf;
//                 }
//             }
//         }
//     }

//     light_0 = Ld_0;
//     light_h = Ld_H;

//     // // Sample BSDF with multiple importance sampling
//     // if (!IsDeltaLight(light.flags))
//     // {
//     //     Spectrum f;
//     //     bool sampledSpecular = false;
//     //     if (it.IsSurfaceInteraction()) {
//     //         // Sample scattered direction for surface interactions
//     //         BxDFType sampledType;
//     //         const SurfaceInteraction &isect = (const SurfaceInteraction
//     &)it;
//     //         f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//     //         &scatteringPdf,
//     //                                  bsdfFlags, &sampledType);
//     //         f *= AbsDot(wi, isect.shading.n);
//     //         sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//     //     } else {
//     //         // Sample scattered direction for medium interactions
//     //         const MediumInteraction &mi = (const MediumInteraction &)it;
//     //         Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//     //         f = Spectrum(p);
//     //         scatteringPdf = p;
//     //     }
//     //     #if LOGGING
//     //         VLOG(2) << "  BSDF / phase sampling f: " << f << ",
//     //         scatteringPdf: " <<
//     //             scatteringPdf;
//     //     #endif
//     //     if (!f.IsBlack() && scatteringPdf > 0) {
//     //         // Account for light contributions along sampled direction
//     _wi_
//     //         Float weight = 1;
//     //         if (!sampledSpecular) {
//     //             lightPdf = light.Pdf_Li(it, wi);
//     //             if (lightPdf == 0) return Ld;
//     //             weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//     //         }
//     //
//     //         // Find intersection and compute transmittance
//     //         SurfaceInteraction lightIsect;
//     //         Ray ray = it.SpawnRay(wi);
//     //         Spectrum Tr(1.f);
//     //         bool foundSurfaceInteraction =
//     //             handleMedia ? scene.IntersectTr(ray,
//     //                                             sampler,
//     //                                             trans_sampler,
//     //                                             &lightIsect,
//     //                                             &Tr)
//     //                         : scene.Intersect(ray, &lightIsect);
//     //         // std::cout << "TR POST: " << Tr << std::endl;
//     //
//     //         // Add light contribution from material sampling
//     //         Spectrum Li(0.f);
//     //         if (foundSurfaceInteraction) {
//     //             if (lightIsect.primitive->GetAreaLight() == &light)
//     //                 Li = lightIsect.Le(-wi);
//     //         } else
//     //             Li = light.Le(ray);
//     //         if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
//     //     }
//     // }

//     // return Ld;
// }

// void EstimateDirect_FD_Rough_Debiased(const Interaction &it,
//                                       const Point2f &uScattering,
//                                       const Light &light, const Point2f
//                                       &uLight, const Scene &scene, Sampler
//                                       &sampler, Sampler &trans_sampler,
//                                       Spectrum &light_0, Spectrum &light_h,
//                                       Spectrum &light_h1, Spectrum &light_h2,
//                                       MemoryArena &arena, bool handleMedia,
//                                       bool specular) {
//     // std::cout << "inside estimate direct" << std::endl;
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     Spectrum Ld_0(0.f);
//     Spectrum Ld_H(0.f);
//     Spectrum Ld_H1(0.f);
//     Spectrum Ld_H2(0.f);

//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f_0;
//         Spectrum f_h;
//         Spectrum f_h1;
//         Spectrum f_h2;

//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction_CorDebBSDF &isect =
//                 (const SurfaceInteraction_CorDebBSDF &)it;

//             f_0 = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                   AbsDot(wi, isect.shading.n);

//             if (isect.bsdf_h) {
//                 f_h = isect.bsdf_h->f(isect.wo, wi, bsdfFlags) *
//                       AbsDot(wi, isect.shading.n);
//             } else {
//                 f_h = f_0;
//             }

//             if (isect.bsdf_h1) {
//                 f_h1 = isect.bsdf_h1->f(isect.wo, wi, bsdfFlags) *
//                        AbsDot(wi, isect.shading.n);
//             } else {
//                 f_h1 = f_0;
//             }

//             if (isect.bsdf_h2) {
//                 f_h2 = isect.bsdf_h2->f(isect.wo, wi, bsdfFlags) *
//                        AbsDot(wi, isect.shading.n);
//             } else {
//                 f_h2 = f_0;
//             }

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//         } else {
//             light_0 = Spectrum(0.f);
//             light_h = Spectrum(0.f);
//             light_h1 = Spectrum(0.f);
//             light_h2 = Spectrum(0.f);
//             return;
//         }
//         if (!f_0.IsBlack()) {
//             if (!visibility.Unoccluded(scene)) {
//                 Li = Spectrum(0.f);
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld_0 += f_0 * Li / lightPdf;
//                     Ld_H += f_h * Li / lightPdf;
//                     Ld_H1 += f_h1 * Li / lightPdf;
//                     Ld_H2 += f_h2 * Li / lightPdf;
//                 } else {
//                     Ld_0 += f_0 * Li / lightPdf;
//                     Ld_H += f_h * Li / lightPdf;
//                     Ld_H1 += f_h1 * Li / lightPdf;
//                     Ld_H2 += f_h2 * Li / lightPdf;
//                 }
//             }
//         }
//     }

//     // // Sample BSDF with multiple importance sampling
//     // if (!IsDeltaLight(light.flags)) {
//     //     Spectrum f;
//     //     Spectrum f_h;
//     //     Spectrum f_h1;
//     //     Spectrum f_h2;
//     //     bool sampledSpecular = false;
//     //     if (it.IsSurfaceInteraction()) {
//     //         // Sample scattered direction for surface interactions
//     //         BxDFType sampledType;
//     //         const SurfaceInteraction_CorDebBSDF &isect =
//     //             (const SurfaceInteraction_CorDebBSDF &)it;
//     //         f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//     //         &scatteringPdf,
//     //                                  bsdfFlags, &sampledType);
//     //         f *= AbsDot(wi, isect.shading.n);
//     //         sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//     //
//     //         if (isect.bsdf_h) {
//     //             f_h = isect.bsdf_h->f(isect.wo, wi, bsdfFlags) *
//     //                   AbsDot(wi, isect.shading.n);
//     //         } else {
//     //             f_h = f;
//     //         }
//     //
//     //         if (isect.bsdf_h1) {
//     //             f_h1 = isect.bsdf_h1->f(isect.wo, wi, bsdfFlags) *
//     //                    AbsDot(wi, isect.shading.n);
//     //         } else {
//     //             f_h1 = f;
//     //         }
//     //
//     //         if (isect.bsdf_h2) {
//     //             f_h2 = isect.bsdf_h2->f(isect.wo, wi, bsdfFlags) *
//     //                    AbsDot(wi, isect.shading.n);
//     //         } else {
//     //             f_h2 = f;
//     //         }
//     //     }
//     //     if (!f.IsBlack() && scatteringPdf > 0) {
//     //         // Account for light contributions along sampled direction
//     _wi_
//     //         Float weight = 1;
//     //         if (!sampledSpecular) {
//     //             lightPdf = light.Pdf_Li(it, wi);
//     //             if (lightPdf == 0) {
//     //                 light_0 = Ld_0;
//     //                 light_h = Ld_H;
//     //                 light_h1 = Ld_H1;
//     //                 light_h2 = Ld_H2;
//     //
//     //                 return;
//     //             }
//     //             weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//     //         }
//     //
//     //         // Find intersection and compute transmittance
//     //         SurfaceInteraction lightIsect;
//     //         Ray ray = it.SpawnRay(wi);
//     //         bool foundSurfaceInteraction = scene.Intersect(ray,
//     &lightIsect);
//     //
//     //         // Add light contribution from material sampling
//     //         Spectrum Li(0.f);
//     //         if (foundSurfaceInteraction) {
//     //             if (lightIsect.primitive->GetAreaLight() == &light)
//     //                 Li = lightIsect.Le(-wi);
//     //         } else
//     //             Li = light.Le(ray);
//     //         if (!Li.IsBlack()) {
//     //             Ld_0 += f * Li * weight / scatteringPdf;
//     //             Ld_H += f_h * Li * weight / scatteringPdf;
//     //             Ld_H1 += f_h1 * Li * weight / scatteringPdf;
//     //             Ld_H2 += f_h2 * Li * weight / scatteringPdf;
//     //         }
//     //     }
//     // }

//     light_0 = Ld_0;
//     light_h = Ld_H;
//     light_h1 = Ld_H1;
//     light_h2 = Ld_H2;
// }

// void EstimateDirect_FD_Rough_Debiased_Center(
//     const Interaction &it, const Point2f &uScattering, const Light &light,
//     const Point2f &uLight, const Scene &scene, Sampler &sampler,
//     Sampler &trans_sampler, FD_Cor_Deb_Cent_Spectrum &beta, MemoryArena
//     &arena, bool handleMedia, bool specular) {
//     // std::cout << "inside estimate direct" << std::endl;
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     FD_Cor_Deb_Cent_Spectrum Ld(Spectrum(0.f));

//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f_0;
//         Spectrum f_h00;
//         Spectrum f_h01;
//         Spectrum f_h10;
//         Spectrum f_h11;
//         Spectrum f_h20;
//         Spectrum f_h21;

//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction_CorDebBSDF &isect =
//                 (const SurfaceInteraction_CorDebBSDF &)it;

//             f_0 = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                   AbsDot(wi, isect.shading.n);

//             if (isect.bsdf_h00) {
//                 f_h00 = isect.bsdf_h00->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h01 = isect.bsdf_h01->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h10 = isect.bsdf_h10->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h11 = isect.bsdf_h11->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h20 = isect.bsdf_h20->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h21 = isect.bsdf_h21->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//             } else {
//                 f_h00 = f_0;
//                 f_h01 = f_0;
//                 f_h10 = f_0;
//                 f_h11 = f_0;
//                 f_h20 = f_0;
//                 f_h21 = f_0;
//             }

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//         } else {
//             beta.setAll(Spectrum(0.f));
//             return;
//         }
//         if (!f_0.IsBlack()) {
//             if (!visibility.Unoccluded(scene)) {
//                 Li = Spectrum(0.f);
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld.L_0 += f_0 * Li / lightPdf;
//                     Ld.L_H00 += f_h00 * Li / lightPdf;
//                     Ld.L_H01 += f_h01 * Li / lightPdf;
//                     Ld.L_H10 += f_h10 * Li / lightPdf;
//                     Ld.L_H11 += f_h11 * Li / lightPdf;
//                     Ld.L_H20 += f_h20 * Li / lightPdf;
//                     Ld.L_H21 += f_h21 * Li / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld.L_0 += f_0 * Li * weight / lightPdf;
//                     Ld.L_H00 += f_h00 * Li * weight / lightPdf;
//                     Ld.L_H01 += f_h01 * Li * weight / lightPdf;
//                     Ld.L_H10 += f_h10 * Li * weight / lightPdf;
//                     Ld.L_H11 += f_h11 * Li * weight / lightPdf;
//                     Ld.L_H20 += f_h20 * Li * weight / lightPdf;
//                     Ld.L_H21 += f_h21 * Li * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         Spectrum f_h00;
//         Spectrum f_h01;
//         Spectrum f_h10;
//         Spectrum f_h11;
//         Spectrum f_h20;
//         Spectrum f_h21;

//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction_CorDebBSDF &isect =
//                 (const SurfaceInteraction_CorDebBSDF &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);
//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;

//             if (isect.bsdf_h00) {
//                 f_h00 = isect.bsdf_h00->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h01 = isect.bsdf_h01->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h10 = isect.bsdf_h10->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h11 = isect.bsdf_h11->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h20 = isect.bsdf_h20->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//                 f_h21 = isect.bsdf_h21->f(isect.wo, wi, bsdfFlags) *
//                         AbsDot(wi, isect.shading.n);
//             } else {
//                 f_h00 = f;
//                 f_h01 = f;
//                 f_h10 = f;
//                 f_h11 = f;
//                 f_h20 = f;
//                 f_h21 = f;
//             }
//         }
//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) {
//                     beta = Ld;
//                     return;
//                 }
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             bool foundSurfaceInteraction = scene.Intersect(ray, &lightIsect);

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);
//             if (!Li.IsBlack()) {
//                 Ld.L_0 += f * Li * weight / scatteringPdf;
//                 Ld.L_H00 += f_h00 * Li * weight / scatteringPdf;
//                 Ld.L_H01 += f_h01 * Li * weight / scatteringPdf;
//                 Ld.L_H10 += f_h10 * Li * weight / scatteringPdf;
//                 Ld.L_H11 += f_h11 * Li * weight / scatteringPdf;
//                 Ld.L_H20 += f_h20 * Li * weight / scatteringPdf;
//                 Ld.L_H21 += f_h21 * Li * weight / scatteringPdf;
//             }
//         }
//     }

//     beta = Ld;
// }

// FD_Cor_Sigma_T_Spectrum EstimateDirect_Cor(
//     const Interaction &it, const Point2f &uScattering, const Light &light,
//     const Point2f &uLight, const Scene &scene, Sampler &sampler,
//     Sampler &trans_sampler, MemoryArena &arena,
//     const FD_Cor_Sigma_T_Spectrum &spec, bool handleMedia, bool specular) {
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);

//     FD_Cor_Sigma_T_Spectrum Ld = spec;
//     Ld.setAll(Spectrum(0.f));

//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     Spectrum Li_li = light.Sample_Li(it, uLight, &wi, &lightPdf,
//     &visibility); FD_Cor_Sigma_T_Spectrum Li = spec; Li.setAll(Li_li);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
//         if (!f.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 Li *= visibility.Tr_Cor(scene, trans_sampler, spec);
//             } else {
//                 if (!visibility.Unoccluded(scene)) {
//                     Li.setAll(Spectrum(0.f));
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += Li * f / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += Li * f * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);
//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
// #if LOGGING
//         VLOG(2) << "  BSDF / phase sampling f: " << f
//                 << ", scatteringPdf: " << scatteringPdf;
// #endif
//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) return Ld;
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             FD_Cor_Sigma_T_Spectrum Tr = spec;
//             Tr.setAll(Spectrum(1.f));
//             bool foundSurfaceInteraction =
//                 handleMedia ? scene.IntersectTr_Cor(ray, sampler,
//                 trans_sampler,
//                                                     &lightIsect, &Tr)
//                             : scene.Intersect(ray, &lightIsect);

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);
//             if (!Li.IsBlack()) Ld += Tr * Li * f * weight / scatteringPdf;
//         }
//     }

//     return Ld;
// }

// FD_DebCor_Sigma_T_Spectrum EstimateDirect_DebCor(
//     const Interaction &it, const Point2f &uScattering, const Light &light,
//     const Point2f &uLight, const Scene &scene, Sampler &sampler,
//     Sampler &trans_sampler, MemoryArena &arena,
//     const FD_DebCor_Sigma_T_Spectrum &spec, bool handleMedia, bool specular)
//     { BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);

//     FD_DebCor_Sigma_T_Spectrum Ld = spec;
//     Ld.setAll(Spectrum(0.f));

//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     Spectrum Li_li = light.Sample_Li(it, uLight, &wi, &lightPdf,
//     &visibility); FD_DebCor_Sigma_T_Spectrum Li = spec; Li.setAll(Li_li);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
//         if (!f.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 Li *= visibility.Tr_DebCor(scene, trans_sampler, spec);
//             } else {
//                 if (!visibility.Unoccluded(scene)) {
//                     Li.setAll(Spectrum(0.f));
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += Li * f / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += Li * f * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);
//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
// #if LOGGING
//         VLOG(2) << "  BSDF / phase sampling f: " << f
//                 << ", scatteringPdf: " << scatteringPdf;
// #endif
//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) return Ld;
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             FD_DebCor_Sigma_T_Spectrum Tr = spec;
//             Tr.setAll(Spectrum(1.f));
//             bool foundSurfaceInteraction =
//                 handleMedia ? scene.IntersectTr_DebCor(
//                                   ray, sampler, trans_sampler, &lightIsect,
//                                   &Tr)
//                             : scene.Intersect(ray, &lightIsect);

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);
//             if (!Li.IsBlack()) Ld += Tr * Li * f * weight / scatteringPdf;
//         }
//     }

//     return Ld;
// }

Spectrum EstimateDirect_ManualSig(const Interaction &it,
                                  const Point2f &uScattering,
                                  const Light &light, const Point2f &uLight,
                                  const Scene &scene, Sampler &sampler,
                                  Sampler &trans_sampler, MemoryArena &arena,
                                  bool handleMedia, bool specular,
                                  Float sigma_t) {
    BxDFType bsdfFlags =
        specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    Spectrum Ld(0.f);
    // Sample light source with multiple importance sampling
    Vector3f wi;
    Float lightPdf = 0, scatteringPdf = 0;
    VisibilityTester visibility;

    // std::cout << "est dir: " << sigma_t << std::endl;

    Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

    if (lightPdf > 0 && !Li.IsBlack()) {
        // Compute BSDF or phase function's value for light sample
        Spectrum f;
        if (it.IsSurfaceInteraction()) {
            // Evaluate BSDF for light sampling strategy
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

            f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
                AbsDot(wi, isect.shading.n);

            scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
        } else {
            // Evaluate phase function for light sampling strategy
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->p(mi.wo, wi);

            f = Spectrum(p);
            scatteringPdf = p;
        }
        if (!f.IsBlack()) {
            // Compute effect of visibility for light source sample
            if (handleMedia) {
                // std::cout << "pre" << std::endl;
                Li *= visibility.Tr_ManualSig(scene, trans_sampler, sigma_t);
                // std::cout << "post" << std::endl;
            } else {
                if (!visibility.Unoccluded(scene)) {
                    Li = Spectrum(0.f);
                }
            }

            // Add light's contribution to reflected radiance
            if (!Li.IsBlack()) {
                if (IsDeltaLight(light.flags)) {
                    Ld += f * Li / lightPdf;
                } else {
                    Float weight =
                        PowerHeuristic(1, lightPdf, 1, scatteringPdf);

                    Ld += f * Li * weight / lightPdf;
                }
            }
        }
    }

    // Sample BSDF with multiple importance sampling
    if (!IsDeltaLight(light.flags)) {
        Spectrum f;
        bool sampledSpecular = false;
        if (it.IsSurfaceInteraction()) {
            // Sample scattered direction for surface interactions
            BxDFType sampledType;
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering, &scatteringPdf,
                                     bsdfFlags, &sampledType);
            f *= AbsDot(wi, isect.shading.n);
            sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
        } else {
            // Sample scattered direction for medium interactions
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
            f = Spectrum(p);
            scatteringPdf = p;
        }
#if LOGGING
        VLOG(2) << "  BSDF / phase sampling f: " << f
                << ", scatteringPdf: " << scatteringPdf;
#endif
        if (!f.IsBlack() && scatteringPdf > 0) {
            // Account for light contributions along sampled direction _wi_
            Float weight = 1;
            if (!sampledSpecular) {
                lightPdf = light.Pdf_Li(it, wi);
                if (lightPdf == 0) return Ld;
                weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
            }

            // Find intersection and compute transmittance
            SurfaceInteraction lightIsect;
            Ray ray = it.SpawnRay(wi);
            Spectrum Tr(1.f);
            bool foundSurfaceInteraction =
                handleMedia
                    ? scene.IntersectTr_ManualSig(ray, sampler, trans_sampler,
                                                  &lightIsect, &Tr, sigma_t)
                    : scene.Intersect(ray, &lightIsect);
            // std::cout << "TR POST: " << Tr << std::endl;

            // Add light contribution from material sampling
            Spectrum Li(0.f);
            if (foundSurfaceInteraction) {
                if (lightIsect.primitive->GetAreaLight() == &light)
                    Li = lightIsect.Le(-wi);
            } else
                Li = light.Le(ray);
            if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
        }
    }

    return Ld;
}

// void EstimateDirectClampGeom(const Interaction &it, const Point2f
// &uScattering,
//                              const Light &light, const Point2f &uLight,
//                              const Scene &scene, Sampler &sampler,
//                              Sampler &trans_sampler, MemoryArena &arena,
//                              DebiasedClampingSpectrum &ret, bool handleMedia,
//                              bool specular) {
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);

//     DebiasedClampingSpectrum Ld = ret;

//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     // need to have the same k set as the return
//     DebiasedClampingSpectrum Li = ret;
//     Li.setAll(Spectrum(0.f));

//     light.Sample_Li_ClampGeom(it, uLight, &wi, &lightPdf, &visibility, Li);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
//         if (!f.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 Li *= visibility.Tr(scene, trans_sampler);
//             } else {
//                 if (!visibility.Unoccluded(scene)) {
//                     Li.setAll(Spectrum(0.f));
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += Li * f / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += Li * f * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);
//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
// #if LOGGING
//         VLOG(2) << "  BSDF / phase sampling f: " << f
//                 << ", scatteringPdf: " << scatteringPdf;
// #endif
//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // THIS SHOULD NOT BE EVER CALLED
//             std::cout << "Zack this is going to break cause you are a dumb
//             dumb"
//                       << std::endl;
//             assert(false);
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) {
//                     ret = Ld;
//                     return;
//                 }
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             Spectrum Tr(1.f);
//             bool foundSurfaceInteraction =
//                 handleMedia ? scene.IntersectTr(ray, sampler, trans_sampler,
//                                                 &lightIsect, &Tr)
//                             : scene.Intersect(ray, &lightIsect);

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);
//             if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
//         }
//     }

//     ret = Ld;
// }

Spectrum EstimateDirect_NonExp(const Interaction &it,
                               const Point2f &uScattering, const Light &light,
                               const Point2f &uLight, const Scene &scene,
                               Sampler &sampler, Sampler &trans_sampler,
                               MemoryArena &arena, bool specular) {
    BxDFType bsdfFlags =
        specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    Spectrum Ld(0.f);
    // Sample light source with multiple importance sampling
    Vector3f wi;
    Float lightPdf = 0, scatteringPdf = 0;
    VisibilityTester visibility;

    Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

    uint32_t flags = 0;
    resetVertex(flags);

    if (lightPdf > 0 && !Li.IsBlack()) {
        // Compute BSDF or phase function's value for light sample
        Spectrum f;
        if (it.IsSurfaceInteraction()) {
            // Evaluate BSDF for light sampling strategy
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

            f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
                AbsDot(wi, isect.shading.n);

            scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);

            // set the first vertex to be surface and lights are always surfaces
            // std::cout << "Setting flags" << std::endl;
            setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
            setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
        } else {
            // Evaluate phase function for light sampling strategy
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->p(mi.wo, wi);

            f = Spectrum(p);
            scatteringPdf = p;

            // set the first vertex to medium and lights are always surfaces
            setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
            setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
        }
        if (!f.IsBlack()) {
            // Compute effect of visibility for light source sample
            Li *= visibility.Tr(scene, trans_sampler, flags);

            // Add light's contribution to reflected radiance
            if (!Li.IsBlack()) {
                if (IsDeltaLight(light.flags)) {
                    Ld += f * Li / lightPdf;
                } else {
                    Float weight =
                        PowerHeuristic(1, lightPdf, 1, scatteringPdf);

                    Ld += f * Li * weight / lightPdf;
                }
            }
        }
    }

    // Sample BSDF with multiple importance sampling
    if (!IsDeltaLight(light.flags)) {
        Spectrum f;
        bool sampledSpecular = false;
        if (it.IsSurfaceInteraction()) {
            // Sample scattered direction for surface interactions
            BxDFType sampledType;
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering, &scatteringPdf,
                                     bsdfFlags, &sampledType);

            f *= AbsDot(wi, isect.shading.n);
            sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;

            setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
            setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
        } else {
            // Sample scattered direction for medium interactions
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
            f = Spectrum(p);
            scatteringPdf = p;

            setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
            setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
        }

        if (!f.IsBlack() && scatteringPdf > 0) {
            // Account for light contributions along sampled direction _wi_
            Float weight = 1;
            if (!sampledSpecular) {
                lightPdf = light.Pdf_Li(it, wi);
                if (lightPdf == 0) return Ld;
                weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
            }

            // Find intersection and compute transmittance
            SurfaceInteraction lightIsect;
            Ray ray = it.SpawnRay(wi);
            Spectrum Tr(1.f);
            // std::cout << "ack!" << std::endl;
            bool foundSurfaceInteraction = scene.IntersectTr(
                ray, sampler, trans_sampler, &lightIsect, &Tr, flags);

            // Add light contribution from material sampling
            Spectrum Li(0.f);
            if (foundSurfaceInteraction) {
                if (lightIsect.primitive->GetAreaLight() == &light)
                    Li = lightIsect.Le(-wi);
            } else
                Li = light.Le(ray);

            if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
        }
    }
    return Ld;
}

// Spectrum EstimateDirect_NonExp_Beta(const Interaction &it,
//                                     const Point2f &uScattering,
//                                     const Light &light, const Point2f
//                                     &uLight, Float beta, Float c, const Scene
//                                     &scene, Sampler &sampler, Sampler
//                                     &trans_sampler, MemoryArena &arena, bool
//                                     specular) {
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     Spectrum Ld(0.f);
//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

//     uint32_t flags = 0;
//     resetVertex(flags);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);

//             // set the first vertex to be surface and lights are always
//             surfaces
//             // std::cout << "Setting flags" << std::endl;
//             setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;

//             // set the first vertex to medium and lights are always surfaces
//             setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         }
//         if (!f.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             Li *= visibility.Tr_Beta(scene, trans_sampler, beta, c, flags);

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += f * Li / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += f * Li * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);

//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;

//             setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;

//             setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         }

//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) return Ld;
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             Spectrum Tr(1.f);
//             // std::cout << "ack!" << std::endl;
//             bool foundSurfaceInteraction = scene.IntersectTr_Beta(
//                 ray, sampler, trans_sampler, beta, c, &lightIsect, &Tr,
//                 flags);

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);

//             if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
//         }
//     }
//     return Ld;
// }

BlanchetCorrelatedResults EstimateDirect_NonExp_Blanchet(
    const Interaction &it, const Point2f &uScattering, const Light &light,
    const Point2f &uLight, const Scene &scene, Sampler &sampler,
    Sampler &trans_sampler, MemoryArena &arena, int blanchet_n, bool specular) {
    // TODO
    // throw new std::exception();

    BxDFType bsdfFlags =
        specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    BlanchetCorrelatedResults Ld(0.f);
    // Sample light source with multiple importance sampling
    Vector3f wi;
    Float lightPdf = 0, scatteringPdf = 0;
    VisibilityTester visibility;

    BlanchetCorrelatedResults Li = BlanchetCorrelatedResults(
        light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility));

    uint32_t flags = 0;
    resetVertex(flags);

    if (lightPdf > 0 && !Li.IsBlack()) {
        // Compute BSDF or phase function's value for light sample
        Spectrum f;
        if (it.IsSurfaceInteraction()) {
            // Evaluate BSDF for light sampling strategy
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

            f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
                AbsDot(wi, isect.shading.n);

            scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);

            // set the first vertex to be surface and lights are always surfaces
            // std::cout << "Setting flags" << std::endl;
            setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
            setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
        } else {
            // Evaluate phase function for light sampling strategy
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->p(mi.wo, wi);

            f = Spectrum(p);
            scatteringPdf = p;

            // set the first vertex to medium and lights are always surfaces
            setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
            setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
        }
        if (!f.IsBlack()) {
            // Compute effect of visibility for light source sample
            Li *=
                visibility.Tr_Blanchet(scene, trans_sampler, flags, blanchet_n);

            // Add light's contribution to reflected radiance
            if (!Li.IsBlack()) {
                if (IsDeltaLight(light.flags)) {
                    Ld += Li * f / lightPdf;
                } else {
                    Float weight =
                        PowerHeuristic(1, lightPdf, 1, scatteringPdf);

                    Ld += Li * f * weight / lightPdf;
                }
            }
        }
    }

    // Sample BSDF with multiple importance sampling
    if (!IsDeltaLight(light.flags)) {
        Spectrum f;
        bool sampledSpecular = false;
        if (it.IsSurfaceInteraction()) {
            // Sample scattered direction for surface interactions
            BxDFType sampledType;
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering, &scatteringPdf,
                                     bsdfFlags, &sampledType);

            f *= AbsDot(wi, isect.shading.n);
            sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;

            setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
            setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
        } else {
            // Sample scattered direction for medium interactions
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
            f = Spectrum(p);
            scatteringPdf = p;

            setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
            setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
        }

        if (!f.IsBlack() && scatteringPdf > 0) {
            // Account for light contributions along sampled direction _wi_
            Float weight = 1;
            if (!sampledSpecular) {
                lightPdf = light.Pdf_Li(it, wi);
                if (lightPdf == 0) return Ld;
                weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
            }

            // Find intersection and compute transmittance
            SurfaceInteraction lightIsect;
            Ray ray = it.SpawnRay(wi);
            BlanchetCorrelatedResults Tr(1.f);
            // std::cout << "ack!" << std::endl;
            bool foundSurfaceInteraction =
                scene.IntersectTr_Blanchet(ray, sampler, trans_sampler,
                                           &lightIsect, &Tr, flags, blanchet_n);

            // Add light contribution from material sampling
            Spectrum Li(0.f);
            if (foundSurfaceInteraction) {
                if (lightIsect.primitive->GetAreaLight() == &light)
                    Li = lightIsect.Le(-wi);
            } else
                Li = light.Le(ray);

            if (!Li.IsBlack()) Ld += Tr * f * Li * weight / scatteringPdf;
        }
    }
    return Ld;
}

// BlanchetCorrelatedResults EstimateDirect_NonExp_Blanchet_Beta(
//     const Interaction &it, const Point2f &uScattering, const Light &light,
//     const Point2f &uLight, Float beta, Float c, const Scene &scene,
//     Sampler &sampler, Sampler &trans_sampler, MemoryArena &arena,
//     int blanchet_n, bool specular) {
//     // TODO
//     // throw new std::exception();

//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     BlanchetCorrelatedResults Ld(0.f);
//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     BlanchetCorrelatedResults Li = BlanchetCorrelatedResults(
//         light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility));

//     uint32_t flags = 0;
//     resetVertex(flags);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);

//             // set the first vertex to be surface and lights are always
//             surfaces
//             // std::cout << "Setting flags" << std::endl;
//             setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;

//             // set the first vertex to medium and lights are always surfaces
//             setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         }
//         if (!f.IsBlack()) {
//             if (getVertex0Type(flags) == CustomVertexType::VERTEX_MEDIUM &&
//                 getVertex1Type(flags) == CustomVertexType::VERTEX_MEDIUM) {
//                 std::cout << "WHAT THE FUFK" << std::endl;
//             }
//             // std::cout << "pre vis" << std::endl;
//             // Compute effect of visibility for light source sample
//             Li *= visibility.Tr_Blanchet_Beta(scene, trans_sampler, beta, c,
//                                               flags, blanchet_n);

//             // std::cout << "post vis" << std::endl;
//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += Li * f / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += Li * f * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);

//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;

//             setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;

//             setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         }

//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) return Ld;
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             BlanchetCorrelatedResults Tr(1.f);
//             // std::cout << "ack!" << std::endl;

//             if (getVertex0Type(flags) == CustomVertexType::VERTEX_MEDIUM &&
//                 getVertex1Type(flags) == CustomVertexType::VERTEX_MEDIUM) {
//                 std::cout << "WHAT THE FUFK" << std::endl;
//             }

//             // std::cout << "pre inter" << std::endl;

//             bool foundSurfaceInteraction = scene.IntersectTr_Blanchet_Beta(
//                 ray, sampler, trans_sampler, beta, c, &lightIsect, &Tr,
//                 flags, blanchet_n);

//             // std::cout << "post inter" << std::endl;

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);

//             if (!Li.IsBlack()) Ld += Tr * f * Li * weight / scatteringPdf;
//         }
//     }
//     return Ld;
// }

// BlanchetCorrelatedFdDebiasing EstimateDirect_NonExp_Blanchet_FdBeta(
//     const Interaction &it, const Point2f &uScattering, const Light &light,
//     const Point2f &uLight, const Scene &scene, Sampler &sampler,
//     Sampler &trans_sampler, MemoryArena &arena, int blanchet_n, int davis_n,
//     Float base_h, Float davis_beta, Float davis_c, bool specular) {
//     // TODO: this needs to be finished
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     BlanchetCorrelatedFdDebiasing Ld(0.f);
//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     BlanchetCorrelatedFdDebiasing Li = BlanchetCorrelatedFdDebiasing(
//         light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility));

//     uint32_t flags = 0;
//     resetVertex(flags);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);

//             // set the first vertex to be surface and lights are always
//             surfaces
//             // std::cout << "Setting flags" << std::endl;
//             setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;

//             // set the first vertex to medium and lights are always surfaces
//             setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         }
//         if (!f.IsBlack()) {
//             if (getVertex0Type(flags) == CustomVertexType::VERTEX_MEDIUM &&
//                 getVertex1Type(flags) == CustomVertexType::VERTEX_MEDIUM) {
//                 // std::cout << "WHAT THE FUFK" << std::endl;
//             }
//             // Compute effect of visibility for light source sample
//             Li *= visibility.Tr_Blanchet_FdBeta(scene, trans_sampler,
//                                                 blanchet_n, davis_n, base_h,
//                                                 davis_beta, davis_c, flags);

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += Li * f / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += Li * f * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         // std::cout << "here" << std::endl;
//         Spectrum f;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);

//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;

//             setVertex0Type(CustomVertexType::VERTEX_SURFACE, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;

//             setVertex0Type(CustomVertexType::VERTEX_MEDIUM, flags);
//             setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//         }

//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) return Ld;
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             BlanchetCorrelatedFdDebiasing Tr(1.f);

//             bool foundSurfaceInteraction = scene.IntersectTr_Blanchet_FdBeta(
//                 ray, sampler, trans_sampler, &lightIsect, &Tr, flags,
//                 blanchet_n, davis_n, base_h, davis_beta, davis_c);

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);

//             if (!Li.IsBlack()) Ld += Tr * f * Li * weight / scatteringPdf;
//         }
//     }

//     return Ld;
// }

// Spectrum EstimateDirect_Bias(const Interaction &it, const Point2f
// &uScattering,
//                              const Light &light, const Point2f &uLight,
//                              const Scene &scene, Sampler &sampler,
//                              Sampler &trans_sampler, MemoryArena &arena,
//                              int blanchet_n, bool handleMedia, bool specular)
//                              {
//     BxDFType bsdfFlags =
//         specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
//     Spectrum Ld(0.f);
//     // Sample light source with multiple importance sampling
//     Vector3f wi;
//     Float lightPdf = 0, scatteringPdf = 0;
//     VisibilityTester visibility;

//     Spectrum Li = light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility);

//     if (lightPdf > 0 && !Li.IsBlack()) {
//         // Compute BSDF or phase function's value for light sample
//         Spectrum f;
//         if (it.IsSurfaceInteraction()) {
//             // Evaluate BSDF for light sampling strategy
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

//             f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
//                 AbsDot(wi, isect.shading.n);

//             scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
//         } else {
//             // Evaluate phase function for light sampling strategy
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->p(mi.wo, wi);

//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
//         if (!f.IsBlack()) {
//             // Compute effect of visibility for light source sample
//             if (handleMedia) {
//                 Li *= visibility.Tr_Bias(scene, trans_sampler, blanchet_n);
//             } else {
//                 if (!visibility.Unoccluded(scene)) {
//                     Li = Spectrum(0.f);
//                 }
//             }

//             // Add light's contribution to reflected radiance
//             if (!Li.IsBlack()) {
//                 if (IsDeltaLight(light.flags)) {
//                     Ld += f * Li / lightPdf;
//                 } else {
//                     Float weight =
//                         PowerHeuristic(1, lightPdf, 1, scatteringPdf);

//                     Ld += f * Li * weight / lightPdf;
//                 }
//             }
//         }
//     }

//     // Sample BSDF with multiple importance sampling
//     if (!IsDeltaLight(light.flags)) {
//         Spectrum f;
//         bool sampledSpecular = false;
//         if (it.IsSurfaceInteraction()) {
//             // Sample scattered direction for surface interactions
//             BxDFType sampledType;
//             const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
//             f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering,
//             &scatteringPdf,
//                                      bsdfFlags, &sampledType);
//             f *= AbsDot(wi, isect.shading.n);
//             sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
//         } else {
//             // Sample scattered direction for medium interactions
//             const MediumInteraction &mi = (const MediumInteraction &)it;
//             Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
//             f = Spectrum(p);
//             scatteringPdf = p;
//         }
// #if LOGGING
//         VLOG(2) << "  BSDF / phase sampling f: " << f
//                 << ", scatteringPdf: " << scatteringPdf;
// #endif
//         if (!f.IsBlack() && scatteringPdf > 0) {
//             // Account for light contributions along sampled direction _wi_
//             Float weight = 1;
//             if (!sampledSpecular) {
//                 lightPdf = light.Pdf_Li(it, wi);
//                 if (lightPdf == 0) return Ld;
//                 weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
//             }

//             // Find intersection and compute transmittance
//             SurfaceInteraction lightIsect;
//             Ray ray = it.SpawnRay(wi);
//             Spectrum Tr(1.f);
//             bool foundSurfaceInteraction =
//                 handleMedia
//                     ? scene.IntersectTr_Bias(ray, sampler, trans_sampler,
//                                              &lightIsect, &Tr, blanchet_n)
//                     : scene.Intersect(ray, &lightIsect);
//             // std::cout << "TR POST: " << Tr << std::endl;

//             // Add light contribution from material sampling
//             Spectrum Li(0.f);
//             if (foundSurfaceInteraction) {
//                 if (lightIsect.primitive->GetAreaLight() == &light)
//                     Li = lightIsect.Le(-wi);
//             } else
//                 Li = light.Le(ray);
//             if (!Li.IsBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
//         }
//     }
//     return Ld;
// }

BlanchetCorrelatedResults EstimateDirect_Blanchet(
    const Interaction &it, const Point2f &uScattering, const Light &light,
    const Point2f &uLight, const Scene &scene, Sampler &sampler,
    Sampler &trans_sampler, MemoryArena &arena, int blanchet_n,
    bool handleMedia, bool specular) {
    BxDFType bsdfFlags =
        specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    BlanchetCorrelatedResults Ld_Cor(0.f);
    // Sample light source with multiple importance sampling
    Vector3f wi;
    Float lightPdf = 0, scatteringPdf = 0;
    VisibilityTester visibility;

    BlanchetCorrelatedResults Li_Cor = BlanchetCorrelatedResults(
        light.Sample_Li(it, uLight, &wi, &lightPdf, &visibility));

    if (lightPdf > 0 && !Li_Cor.IsBlack()) {
        // Compute BSDF or phase function's value for light sample
        Spectrum f;
        if (it.IsSurfaceInteraction()) {
            // Evaluate BSDF for light sampling strategy
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;

            f = isect.bsdf->f(isect.wo, wi, bsdfFlags) *
                AbsDot(wi, isect.shading.n);

            scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
        } else {
            // Evaluate phase function for light sampling strategy
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->p(mi.wo, wi);

            f = Spectrum(p);
            scatteringPdf = p;
        }
        if (!f.IsBlack()) {
            // Compute effect of visibility for light source sample
            if (handleMedia) {
                Li_Cor *=
                    visibility.Tr_Blanchet(scene, trans_sampler, blanchet_n);
            } else {
                if (!visibility.Unoccluded(scene)) {
                    Li_Cor = BlanchetCorrelatedResults(0.f);
                }
            }

            // Add light's contribution to reflected radiance
            if (!Li_Cor.IsBlack()) {
                if (IsDeltaLight(light.flags)) {
                    Ld_Cor += Li_Cor * f / lightPdf;
                } else {
                    Float weight =
                        PowerHeuristic(1, lightPdf, 1, scatteringPdf);

                    Ld_Cor += Li_Cor * f * weight / lightPdf;
                }
            }
        }
    }

    // Sample BSDF with multiple importance sampling
    if (!IsDeltaLight(light.flags)) {
        Spectrum f;
        bool sampledSpecular = false;
        if (it.IsSurfaceInteraction()) {
            // Sample scattered direction for surface interactions
            BxDFType sampledType;
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->Sample_f(isect.wo, &wi, uScattering, &scatteringPdf,
                                     bsdfFlags, &sampledType);
            f *= AbsDot(wi, isect.shading.n);
            sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
        } else {
            // Sample scattered direction for medium interactions
            const MediumInteraction &mi = (const MediumInteraction &)it;
            Float p = mi.phase->Sample_p(mi.wo, &wi, uScattering);
            f = Spectrum(p);
            scatteringPdf = p;
        }
#if LOGGING
        VLOG(2) << "  BSDF / phase sampling f: " << f
                << ", scatteringPdf: " << scatteringPdf;
#endif
        if (!f.IsBlack() && scatteringPdf > 0) {
            // Account for light contributions along sampled direction _wi_
            Float weight = 1;
            if (!sampledSpecular) {
                lightPdf = light.Pdf_Li(it, wi);
                if (lightPdf == 0) return Ld_Cor;
                weight = PowerHeuristic(1, scatteringPdf, 1, lightPdf);
            }

            // Find intersection and compute transmittance
            SurfaceInteraction lightIsect;
            Ray ray = it.SpawnRay(wi);
            BlanchetCorrelatedResults Tr_Cor(1.f);
            bool foundSurfaceInteraction =
                handleMedia ? scene.IntersectTr_Blanchet(
                                  ray, sampler, trans_sampler, &lightIsect,
                                  &Tr_Cor, blanchet_n)
                            : scene.Intersect(ray, &lightIsect);
            // std::cout << "TR POST: " << Tr << std::endl;

            // Add light contribution from material sampling
            Spectrum Li(0.f);
            if (foundSurfaceInteraction) {
                if (lightIsect.primitive->GetAreaLight() == &light)
                    Li = lightIsect.Le(-wi);
            } else
                Li = light.Le(ray);
            if (!Li.IsBlack())
                Ld_Cor += Tr_Cor * f * Li * weight / scatteringPdf;
        }
    }

    return Ld_Cor;
}

std::unique_ptr<Distribution1D> ComputeLightPowerDistribution(
    const Scene &scene) {
    if (scene.lights.empty()) return nullptr;
    std::vector<Float> lightPower;
    for (const auto &light : scene.lights)
        lightPower.push_back(light->Power().y());
    return std::unique_ptr<Distribution1D>(
        new Distribution1D(&lightPower[0], lightPower.size()));
}

// SamplerIntegrator Method Definitions
void SamplerIntegrator::Render(const Scene &scene) {
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

    std::cout << "NTILES X: " << nTiles[0] << std::endl;
    std::cout << "NTILES Y: " << nTiles[1] << std::endl;
    std::cout << "TileSize: " << tileSize << std::endl;

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
        // std::cout << HeterogeneousMedium::nDensityCalls << std::endl;
        if (!reporter.continueRun(maxExtCalls)) {
            finished_location = i;
            i = sampler->samplesPerPixel;
        }
        {
            // std::cout << "starting pixel sample: " << i << " / " <<
            // sampler->samplesPerPixel << std::endl;
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

                        // debugging
                        // if (pixel.x < 504 || pixel.x > 516 || pixel.y < 685
                        // ||
                        //     pixel.y > 693)
                        //     continue;

                        // exp
                        // if (pixel.x != 384 || pixel.y != 151) continue;
                        // 0.25
                        // if (pixel.x != 328 || pixel.y != 224) continue;
                        // 0.50
                        // if (pixel.x != 345 || pixel.y != 328) continue;
                        // 0.75
                        // if (pixel.x != 263 || pixel.y != 389) continue;

                        tileSampler->StartPixel(pixel);
                        transSampler->StartPixel(pixel, sampleExtent.x,
                                                 sampleExtent.y, 1);
                        ffSampler->StartPixel(pixel, sampleExtent.x,
                                              sampleExtent.y, 2);

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
                        ++nCameraRays;

                        // Evaluate radiance along camera ray
                        Spectrum L(0.f);

                        if (rayWeight > 0)
                            L = Li(ray, scene, *tileSampler, *transSampler,
                                   *ffSampler, arena, pixel);

                        //                         // Issue warning if
                        //                         unexpected radiance value
                        //                         returned if (L.HasNaNs()) {
                        //                             // #if LOGGING
                        //                             //     VLOG(ERROR) <<
                        //                             StringPrintf(
                        //                             //         "Not-a-number
                        //                             radiance value returned
                        //                             "
                        //                             //         "for pixel
                        //                             (%d, %d), sample %d.
                        //                             Setting
                        //                             //         to black.",
                        //                             pixel.x, pixel.y,
                        //                             //
                        //                             (int)tileSamplers[instanceIndex]->CurrentSampleNumber());
                        //                             // #endif
                        //                             L = Spectrum(0.f);
                        //                         } else if (L.y() < -1e-5) {
                        // #if LOGGING
                        //                         // LOG(ERROR) <<
                        //                         StringPrintf(
                        //                         //     "Negative luminance
                        //                         value, %f, returned "
                        //                         //     "for pixel (%d, %d),
                        //                         sample %d. Setting to
                        //                         //     black.", L.y(),
                        //                         pixel.x, pixel.y,
                        //                         //
                        //                         (int)tileSampler->CurrentSampleNumber());
                        // #endif
                        //                             // std::cout << "here
                        //                             with a question mark" <<
                        //                             // std::endl; std::cout
                        //                             << L[0] << std::endl; L =
                        //                             // Spectrum(0.f); // THIS
                        //                             WAS THE BUG
                        //                         } else if (std::isinf(L.y()))
                        //                         {
                        //                             std::cout << "ERROR: INF"
                        //                             << std::endl;
                        //                             // #if LOGGING
                        //                             //     LOG(ERROR) <<
                        //                             StringPrintf(
                        //                             //     "Infinite
                        //                             luminance value returned
                        //                             "
                        //                             //     "for pixel (%d,
                        //                             %d), sample %d. Setting
                        //                             to
                        //                             //     black.", pixel.x,
                        //                             pixel.y,
                        //                             //
                        //                             (int)tileSamplers[instanceIndex]->CurrentSampleNumber());
                        //                             // #endif
                        //                             L = Spectrum(0.f);
                        //                         }

                        // Add camera ray's contribution to image
                        filmTile->AddSample(cameraSample.pFilm, L, rayWeight);

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

        if (Globals::WRITE_INCREMENTS) {
            if (reporter.continueRun(maxExtCalls)) {
                if (is_power_of_2(i)) {
                    long currentTime = reporter.getCurrentTime();
                    camera->film->WriteImage(currentTime, i);
                }
            }
        }
    }
    reporter.Done();

    // Save final image after rendering
    long currentTime = reporter.getCurrentTime();
    camera->film->WriteImage(currentTime, -1, finished_location);
}

// SamplerIntegrator Method Definitions
void SamplerIntegrator::Render_Mutable(Scene &scene) {
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
    tileSize = 16;  // laptop
    // tileSize = 1;  // workstation

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
    int buffer = 0;

    while (i < sampler->samplesPerPixel + buffer) {
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

                        // exp
                        // if (pixel.x != 117 || pixel.y != 144) continue;

                        tileSampler->StartPixel(pixel);
                        transSampler->StartPixel(pixel, sampleExtent.x,
                                                 sampleExtent.y, 1);
                        ffSampler->StartPixel(pixel, sampleExtent.x,
                                              sampleExtent.y, 2);

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
                        ++nCameraRays;

                        // Evaluate radiance along camera ray
                        Spectrum L(0.f);

                        if (rayWeight > 0)
                            L = Li_Mutable(ray, scene, *tileSampler,
                                           *transSampler, *ffSampler, arena);

                        // Add camera ray's contribution to image
                        if (i >= buffer) {
                            filmTile->AddSample(cameraSample.pFilm, L,
                                                rayWeight);
                        }

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

        if (Globals::WRITE_INCREMENTS) {
            if (reporter.continueRun(maxExtCalls)) {
                if (is_power_of_2(i - buffer)) {
                    long currentTime = reporter.getCurrentTime();
                    camera->film->WriteImage(currentTime, i - buffer);
                }
            }
        }
    }
    reporter.Done();

    // Save final image after rendering
    long currentTime = reporter.getCurrentTime();
    camera->film->WriteImage(currentTime, -1, finished_location);
}

Spectrum SamplerIntegrator::SpecularReflect(
    const RayDifferential &ray, const SurfaceInteraction &isect,
    const Scene &scene, Sampler &sampler, Sampler &trans_sampler,
    Sampler &ff_sampler, MemoryArena &arena, int depth) const {
    // Compute specular reflection direction _wi_ and BSDF value
    Vector3f wo = isect.wo, wi;
    Float pdf;
    BxDFType type = BxDFType(BSDF_REFLECTION | BSDF_SPECULAR);
    Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf, type);

    // Return contribution of specular reflection
    const Normal3f &ns = isect.shading.n;
    if (pdf > 0.f && !f.IsBlack() && AbsDot(wi, ns) != 0.f) {
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
        return f / pdf *
               Li(rd, scene, sampler, trans_sampler, ff_sampler, arena,
                  depth + 1) *
               AbsDot(wi, ns);
    } else
        return Spectrum(0.f);
}

Spectrum SamplerIntegrator::SpecularTransmit(
    const RayDifferential &ray, const SurfaceInteraction &isect,
    const Scene &scene, Sampler &sampler, Sampler &trans_sampler,
    Sampler &ff_sampler, MemoryArena &arena, int depth) const {
    Vector3f wo = isect.wo, wi;
    Float pdf;
    const Point3f &p = isect.p;
    const Normal3f &ns = isect.shading.n;
    const BSDF &bsdf = *isect.bsdf;
    Spectrum f = bsdf.Sample_f(wo, &wi, sampler.Get2D(), &pdf,
                               BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));
    Spectrum L = Spectrum(0.f);
    if (pdf > 0.f && !f.IsBlack() && AbsDot(wi, ns) != 0.f) {
        // Compute ray differential _rd_ for specular transmission
        RayDifferential rd = isect.SpawnRay(wi);
        if (ray.hasDifferentials) {
            rd.hasDifferentials = true;
            rd.rxOrigin = p + isect.dpdx;
            rd.ryOrigin = p + isect.dpdy;

            Float eta = bsdf.eta;
            Vector3f w = -wo;
            if (Dot(wo, ns) < 0) eta = 1.f / eta;

            Normal3f dndx = isect.shading.dndu * isect.dudx +
                            isect.shading.dndv * isect.dvdx;
            Normal3f dndy = isect.shading.dndu * isect.dudy +
                            isect.shading.dndv * isect.dvdy;

            Vector3f dwodx = -ray.rxDirection - wo,
                     dwody = -ray.ryDirection - wo;
            Float dDNdx = Dot(dwodx, ns) + Dot(wo, dndx);
            Float dDNdy = Dot(dwody, ns) + Dot(wo, dndy);

            Float mu = eta * Dot(w, ns) - Dot(wi, ns);
            Float dmudx =
                (eta - (eta * eta * Dot(w, ns)) / Dot(wi, ns)) * dDNdx;
            Float dmudy =
                (eta - (eta * eta * Dot(w, ns)) / Dot(wi, ns)) * dDNdy;

            rd.rxDirection =
                wi + eta * dwodx - Vector3f(mu * dndx + dmudx * ns);
            rd.ryDirection =
                wi + eta * dwody - Vector3f(mu * dndy + dmudy * ns);
        }

        L = f * AbsDot(wi, ns) / pdf *
            Li(rd, scene, sampler, trans_sampler, ff_sampler, arena, depth + 1);
    }
    return L;
}

// BlanchetIntegrator Method Definitions
void BlanchetIntegrator::Render(const Scene &scene) {
    Preprocess(scene, *sampler);

    bool bias = false;

    // Render image tiles in parallel

    // Compute number of tiles, _nTiles_, to use for parallel rendering
    Bounds2i sampleBounds = camera->film->GetSampleBounds();
    Vector2i sampleExtent = sampleBounds.Diagonal();

    int tileSize;
    if (multithreaded) {
        tileSize = 16;
    } else {
        tileSize = 1;
    }

    Point2i nTiles((sampleExtent.x + tileSize - 1) / tileSize,
                   (sampleExtent.y + tileSize - 1) / tileSize);

    // std::cout << "NTILES X: " << nTiles[0] << std::endl;
    // std::cout << "NTILES Y: " << nTiles[1] << std::endl;
    // std::cout << "TileSize: " << tileSize << std::endl;

    std::unique_ptr<Sampler> tileSampler;
    std::unique_ptr<Sampler> transSampler;
    std::unique_ptr<Sampler> ffSampler;

    // int seedOne = 1;
    // int seedTwo = 2;
    // int seedThree = 3;

    // if (!multithreaded) {
    //     seedOne += 0xaf43fab;
    //     seedTwo += 0x41c3d29;
    //     seedThree += 0x1cbaf45;

    //     seedOne += 0x97acb32;
    //     seedTwo += 0xacdc439;
    //     seedThree += 0x6969420;
    // }

    // seedOne += 0x97acb32;
    // seedTwo += 0xacdc439;
    // seedThree += 0x6969420;

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
                        ffSampler->StartPixel(pixel, nTiles.x, nTiles.y, 2);

                        // from here on out blanchet should be applied

                        // sample an n using tileSampler

                        Float val = tileSampler->Get1D();
                        // chosen r based off of blanchet paper
                        Float r = 1.0 - pow(2.0, -1.5);
                        // Float pmf = r * pow(1.0 - r, 0);
                        Float pmf = r;
                        Float cmf = pmf;
                        int n = 0;

                        while (cmf < val) {
                            ++n;
                            pmf *= 1.0 - r;
                            cmf += pmf;
                        }

                        Spectrum all = Spectrum(0.f);
                        Spectrum even = Spectrum(0.f);
                        Spectrum odd = Spectrum(0.f);
                        // Spectrum biased = Spectrum(0.f);

                        /*int blanchet_samps = pow(2.0, n+1);

                        for (int itr = 0; itr < blanchet_samps; ++itr)
                        {
                            // Initialize _CameraSample_ for current sample
                            CameraSample cameraSample =
                        tileSampler->GetCameraSample(pixel);

                            // Generate camera ray for current sample
                            RayDifferential ray;
                            Float rayWeight =
                        camera->GenerateRayDifferential(cameraSample, &ray);
                            ray.ScaleDifferentials(1.0 /
                        std::sqrt((Float)tileSampler->samplesPerPixel));
                            ++nCameraRays;

                            // Evaluate radiance along camera ray
                            Spectrum L(0.f);
                            if (rayWeight > 0) L = Li(ray,
                                                      scene,
                                                      *tileSampler,
                                                      *transSampler,
                                                      *ffSampler,
                                                      arena);

                            if (itr % 2 == 0) even += L;
                            else odd += L;

                            //std::cout << odd[0] << " " << even[0] <<
                        std::endl; all += L;
                        }

                        even /= Float(blanchet_samps >> 1);
                        odd /= Float(blanchet_samps >> 1);
                        all /= Float(blanchet_samps);*/

                        Spectrum biased_one(0.f);
                        Spectrum biased_two(0.f);
                        Spectrum biased(0.f);

                        Float one_weight = 0.f;
                        Float two_weight = 0.f;
                        Float bias_weight = 0.f;

                        CameraSample cameraSample =
                            tileSampler->GetCameraSample(pixel);

                        // Generate camera ray for current sample
                        RayDifferential ray;
                        Float ray_weight =
                            camera->GenerateRayDifferential(cameraSample, &ray);
                        ray.ScaleDifferentials(
                            1.0 /
                            std::sqrt((Float)tileSampler->samplesPerPixel));
                        ++nCameraRays;

                        // n+1 biased
                        {
                            // Initialize _CameraSample_ for current sample
                            // CameraSample cameraSample =
                            // tileSampler->GetCameraSample(pixel);

                            // Generate camera ray for current sample
                            // RayDifferential ray;
                            // Float one_weight =
                            // camera->GenerateRayDifferential(cameraSample,
                            // &ray); ray.ScaleDifferentials(1.0 /
                            // std::sqrt((Float)tileSampler->samplesPerPixel));
                            //++nCameraRays;

                            // Evaluate radiance along camera ray
                            if (ray_weight > 0)
                                biased_one = Li_Bias(ray, scene, *tileSampler,
                                                     *transSampler, *ffSampler,
                                                     arena, n + 1);
                        }
                        // n biased
                        {
                            // Initialize _CameraSample_ for current sample
                            // CameraSample cameraSample =
                            // tileSampler->GetCameraSample(pixel);
                            //
                            // // Generate camera ray for current sample
                            // RayDifferential ray;
                            // Float two_weight =
                            // camera->GenerateRayDifferential(cameraSample,
                            // &ray); ray.ScaleDifferentials(1.0 /
                            // std::sqrt((Float)tileSampler->samplesPerPixel));
                            // ++nCameraRays;

                            // Evaluate radiance along camera ray
                            if (ray_weight > 0)
                                biased_two = Li_Bias(ray, scene, *tileSampler,
                                                     *transSampler, *ffSampler,
                                                     arena, n);
                        }

                        // 1 biased
                        {
                            // // Initialize _CameraSample_ for current sample
                            // CameraSample cameraSample =
                            // tileSampler->GetCameraSample(pixel);
                            //
                            // // Generate camera ray for current sample
                            // RayDifferential ray;
                            // Float rayWeight =
                            // camera->GenerateRayDifferential(cameraSample,
                            // &ray); ray.ScaleDifferentials(1.0 /
                            // std::sqrt((Float)tileSampler->samplesPerPixel));
                            // ++nCameraRays;

                            // Evaluate radiance along camera ray
                            // Spectrum biased(0.f);
                            if (ray_weight > 0 && !bias)
                                biased = Li(ray, scene, *tileSampler,
                                            *transSampler, *ffSampler, arena);
                        }

                        /*// Initialize _CameraSample_ for current sample
                        CameraSample cameraSample =
                        tileSampler->GetCameraSample(pixel);

                        // Generate camera ray for current sample
                        RayDifferential ray;
                        Float rayWeight =
                        camera->GenerateRayDifferential(cameraSample, &ray);
                        ray.ScaleDifferentials(1.0 /
                        std::sqrt((Float)tileSampler->samplesPerPixel));
                        ++nCameraRays;

                        // Evaluate radiance along camera ray
                        Spectrum biased(0.f);
                        if (rayWeight > 0) biased = Li(ray,
                                                       scene,
                                                       *tileSampler,
                                                       *transSampler,
                                                       *ffSampler,
                                                       arena);*/

                        Spectrum L = Spectrum(0.f);
                        if (bias)
                            L = (biased_one - biased_two) / pmf;
                        else
                            L = (biased_one - biased_two) / pmf + biased;

                        /*// Initialize _CameraSample_ for current sample
                        CameraSample cameraSample =
                        tileSampler->GetCameraSample(pixel);

                        // Generate camera ray for current sample
                        RayDifferential ray;
                        Float rayWeight =
                        camera->GenerateRayDifferential(cameraSample, &ray);
                        ray.ScaleDifferentials(1.0 /
                        std::sqrt((Float)tileSampler->samplesPerPixel));
                        ++nCameraRays;

                        // Evaluate radiance along camera ray
                        Spectrum L(0.f);
                        if (rayWeight > 0) L = Li(ray,
                                                  scene,
                                                  *tileSampler,
                                                  *transSampler,
                                                  *ffSampler,
                                                  arena);*/
                        // else
                        // {
                        //     std::cout << "Ray Weight < 0" << std::endl;
                        // }

                        // Issue warning if unexpected radiance value returned
                        if (L.HasNaNs()) {
                            // #if LOGGING
                            //     VLOG(ERROR) << StringPrintf(
                            //         "Not-a-number radiance value returned "
                            //         "for pixel (%d, %d), sample %d. Setting
                            //         to black.", pixel.x, pixel.y,
                            //         (int)tileSamplers[instanceIndex]->CurrentSampleNumber());
                            // #endif
                            L = Spectrum(0.f);
                        } else if (L.y() < -1e-5) {
#if LOGGING
                        // LOG(ERROR) << StringPrintf(
                        //     "Negative luminance value, %f, returned "
                        //     "for pixel (%d, %d), sample %d. Setting to
                        //     black.", L.y(), pixel.x, pixel.y,
                        //     (int)tileSampler->CurrentSampleNumber());
#endif
                            // std::cout << "here with a question mark" <<
                            // std::endl; std::cout << L[0] << std::endl; L =
                            // Spectrum(0.f); // THIS WAS THE BUG
                        } else if (std::isinf(L.y())) {
                            std::cout << "ERROR: INF" << std::endl;
                            // #if LOGGING
                            //     LOG(ERROR) << StringPrintf(
                            //     "Infinite luminance value returned "
                            //     "for pixel (%d, %d), sample %d. Setting to
                            //     black.", pixel.x, pixel.y,
                            //     (int)tileSamplers[instanceIndex]->CurrentSampleNumber());
                            // #endif
                            L = Spectrum(0.f);
                        }
#if LOGGING
                        VLOG(1) << "Camera sample: " << cameraSample
                                << " -> ray: " << ray << " -> L = " << L;
#endif

                        // Add camera ray's contribution to image
                        filmTile->AddSample(cameraSample.pFilm, L, ray_weight);

                        // Free _MemoryArena_ memory from computing image sample
                        // value
                        arena.Reset();
                    }
#if LOGGING
                    LOG(INFO) << "Finished image tile " << tileBounds;
#endif

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
        // if (reporter.continueRun(maxExtCalls)) {
        //     // TODO: uncomment for incremental sample prints
        //     if (i % 1000 == 0)
        //     {
        //         long currentTime = reporter.getCurrentTime();
        //         camera->film->WriteImage(currentTime, i);
        //     }
        // }
    }
    reporter.Done();

#if LOGGING
    LOG(INFO) << "Rendering finished";
#endif

    // Save final image after rendering
    long currentTime = reporter.getCurrentTime();
    camera->film->WriteImage(currentTime, -1, finished_location);
}

}  // namespace pbrt
