// #include "heterogeneous_nonexp.h"

// #include "interaction.h"
// #include "samplers/random.h"

// namespace pbrt {

// HeterogeneousNonExpMedium::HeterogeneousNonExpMedium(
//     const Spectrum& sigma_a, const Spectrum& sigma_s, Float g,
//     T_Estimator* t_est, DensityFunction* density, MinorantFunction* minorant,
//     MajorantFunction* majorant, Transmittance_Func* trans_func, FF_Sampler* ff,
//     const Transform& WorldToMedium, const Bounds3f b)
//     : sigma_a(sigma_a),
//       sigma_s(sigma_s),
//       sigma_t(sigma_s + sigma_a),
//       g(g),
//       t_est(t_est),
//       density(density),
//       minorant(minorant),
//       majorant(majorant),
//       trans_func(trans_func),
//       ff_sampler(ff),
//       WorldToMedium(Inverse(WorldToMedium)),
//       mediumBounds(b) {
//     if (!t_est->supportsNonExponential()) {
//         std::cout
//             << "transmittance estimator must support non-exponential media"
//             << std::endl;
//         throw new std::exception();
//     }
//     if (!ff_sampler->supportsNonExponential()) {
//         std::cout << "ff sampling must support non-exponential media"
//                   << std::endl;
//         throw new std::exception();
//     }

//     density->preProcess(WorldToMedium, mediumBounds);

//     t_est->minorant = minorant;
//     t_est->majorant = majorant;
//     t_est->density = density;
//     t_est->sigma_t = sigma_t[0];
//     t_est->trans_func = trans_func;
//     t_est->preProcess();

//     ff_sampler->mediumRef = this;
//     ff_sampler->majorant = majorant;
//     ff_sampler->density = density;
//     ff_sampler->trans_func = trans_func;
//     ff_sampler->sigma_s = sigma_s[0];
//     ff_sampler->sigma_t = sigma_t[0];
//     ff_sampler->g = g;

//     Medium::nTrCalls = 0;
//     Medium::nDensityCalls = 0;

//     t_est->preProcess();
// }

// HeterogeneousNonExpMedium::~HeterogeneousNonExpMedium() {
//     delete density;
//     delete t_est;
//     delete majorant;
//     delete minorant;
//     delete trans_func;
//     delete ff_sampler;
// }

// Spectrum HeterogeneousNonExpMedium::Tr(const Ray& rWorld, Sampler& sampler,
//                                        uint32_t flags,
//                                        TransportMode mode) const {
//     // TODO: adapt this method
//     ProfilePhase _(Prof::MediumTr);
//     ++nTrCalls;

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check inside medium bounds
//     if (outsideBounds) return Spectrum(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     density->clearDensityCalls();

//     // calculate transmittance using the specified estimator
//     Spectrum tr = t_est->Tr(ray, sampler, tMin, tMax, startOnSurface,
//                             endOnSurface, adjoint);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// Spectrum HeterogeneousNonExpMedium::Tr_Beta(const Ray& rWorld, Sampler& sampler,
//                                             Float beta, Float c, uint32_t flags,
//                                             TransportMode mode) const {
//     // TODO: adapt this method
//     ProfilePhase _(Prof::MediumTr);
//     ++nTrCalls;

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check inside medium bounds
//     if (outsideBounds) return Spectrum(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     density->clearDensityCalls();

//     // calculate transmittance using the specified estimator
//     Spectrum tr = t_est->Tr_Beta(ray, sampler, tMin, tMax, beta, c,
//                                  startOnSurface, endOnSurface, adjoint, 1.0);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// BlanchetCorrelatedResults HeterogeneousNonExpMedium::Tr_Blanchet(
//     const Ray& rWorld, Sampler& sampler, uint32_t flags, int blanchet_n,
//     TransportMode mode) const {
//     // TODO: adapt this method
//     ProfilePhase _(Prof::MediumTr);
//     ++nTrCalls;

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check inside medium bounds
//     if (outsideBounds) return Spectrum(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     density->clearDensityCalls();

//     // std::cout << "what the fuf" << std::endl;

//     // calculate transmittance using the specified estimator
//     BlanchetCorrelatedResults tr =
//         t_est->Tr_Blanchet(ray, sampler, tMin, tMax, startOnSurface,
//                            endOnSurface, adjoint, blanchet_n);

//     // BlanchetCorrelatedResults tr = BlanchetCorrelatedResults(t_est->Tr(ray,
//     //                         sampler,
//     //                         tMin,
//     //                         tMax,
//     //                         startOnSurface,
//     //                         endOnSurface,
//     //                         adjoint));

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// BlanchetCorrelatedResults HeterogeneousNonExpMedium::Tr_Blanchet_Beta(
//     const Ray& rWorld, Sampler& sampler, uint32_t flags, Float beta, Float c,
//     int blanchet_n, TransportMode mode) const {
//     // return BlanchetCorrelatedResults(0.5f);
//     // TODO: adapt this method
//     ProfilePhase _(Prof::MediumTr);
//     ++nTrCalls;

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check inside medium bounds
//     if (outsideBounds) return Spectrum(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     density->clearDensityCalls();

//     // std::cout << "beta: " << beta << std::endl;

//     // calculate transmittance using the specified estimator
//     BlanchetCorrelatedResults tr = t_est->Tr_Blanchet_Beta(
//         ray, sampler, tMin, tMax, beta, c, startOnSurface, endOnSurface,
//         adjoint, blanchet_n, 1.0);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// BlanchetCorrelatedFdDebiasing HeterogeneousNonExpMedium::Tr_Blanchet_FdBeta(
//     const Ray& rWorld, Sampler& sampler, int blanchet_n, int davis_n,
//     Float base_h, Float davis_beta, Float davis_c, uint32_t flags,
//     TransportMode mode) const {
//     // return BlanchetCorrelatedResults(0.5f);
//     // TODO: adapt this method
//     ProfilePhase _(Prof::MediumTr);
//     ++nTrCalls;

//     // std::cout << "het davis beta; " << davis_beta << std::endl;
//     // std::cout << "het davis c; " << davis_c << std::endl;

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check inside medium bounds
//     if (outsideBounds) return Spectrum(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     density->clearDensityCalls();

//     // calculate transmittance using the specified estimator
//     BlanchetCorrelatedFdDebiasing tr = t_est->Tr_Blanchet_FdBeta(
//         ray, sampler, tMin, tMax, startOnSurface, endOnSurface, adjoint,
//         blanchet_n, davis_n, base_h, davis_beta, davis_c, 1.0);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// Spectrum HeterogeneousNonExpMedium::LocalTr(const Ray& ray, Sampler& sampler,
//                                             Float tMin, Float tMax,
//                                             uint32_t flags,
//                                             TransportMode mode) const {
//     ++nTrCalls;

//     density->clearDensityCalls();

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     Spectrum tr = t_est->Tr(ray, sampler, tMin, tMax, startOnSurface,
//                             endOnSurface, adjoint);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// BlanchetCorrelatedResults HeterogeneousNonExpMedium::LocalTr_Blanchet_Beta(
//     const Ray& ray, Sampler& sampler, Float beta, Float c, Float tMin,
//     Float tMax, int blanchet_n, uint32_t flags, TransportMode mode) const {
//     ++nTrCalls;

//     density->clearDensityCalls();

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     BlanchetCorrelatedResults tr = t_est->Tr_Blanchet_Beta(
//         ray, sampler, tMin, tMax, beta, c, startOnSurface, endOnSurface,
//         adjoint, blanchet_n, 1.0);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// BlanchetCorrelatedFdDebiasing
// HeterogeneousNonExpMedium::LocalTr_Blanchet_FdBeta(
//     const Ray& ray, Sampler& sampler, Float tMin, Float tMax, int blanchet_n,
//     int davis_n, Float base_h, Float davis_beta, Float davis_c, uint32_t flags,
//     TransportMode mode) const {
//     ++nTrCalls;

//     density->clearDensityCalls();

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     BlanchetCorrelatedFdDebiasing tr = t_est->Tr_Blanchet_FdBeta(
//         ray, sampler, tMin, tMax, startOnSurface, endOnSurface, adjoint,
//         blanchet_n, davis_n, base_h, davis_beta, davis_c, 1.0);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// Spectrum HeterogeneousNonExpMedium::LocalTr_Beta(const Ray& ray,
//                                                  Sampler& sampler, Float beta,
//                                                  Float c, Float tMin,
//                                                  Float tMax, uint32_t flags,
//                                                  TransportMode mode) const {
//     ++nTrCalls;

//     density->clearDensityCalls();

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     Spectrum tr = t_est->Tr_Beta(ray, sampler, tMin, tMax, beta, c,
//                                  startOnSurface, endOnSurface, adjoint, 1.0);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// BlanchetCorrelatedResults HeterogeneousNonExpMedium::LocalTr_Blanchet(
//     const Ray& ray, Sampler& sampler, Float tMin, Float tMax, int blanchet_n,
//     uint32_t flags, TransportMode mode) const {
//     ++nTrCalls;

//     density->clearDensityCalls();

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool endOnSurface =
//         getVertex1Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;

//     // std::cout << "fuf" << std::endl;

//     BlanchetCorrelatedResults tr =
//         t_est->Tr_Blanchet(ray, sampler, tMin, tMax, startOnSurface,
//                            endOnSurface, adjoint, blanchet_n);

//     nDensityCalls += density->getDensityCalls();

//     return tr;
// }

// Spectrum HeterogeneousNonExpMedium::Sample(const Ray& rWorld, Sampler& ff_samp,
//                                            Sampler& trans_samp,
//                                            MemoryArena& arena,
//                                            MediumInteraction* mi,
//                                            uint32_t flags,
//                                            TransportMode mode) const {
//     // TODO: update this
//     ProfilePhase _(Prof::MediumSample);

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check bounds
//     if (outsideBounds) return Spectrum(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;
//     bool scats = false;

//     // return the sampled contribution and medium interaction
//     // Spectrum ret = Spectrum(1.0);
//     // if (sigma_s != Spectrum(0.0)) {
//         // std::cout << "here" << std::endl;
//     Spectrum ret = ff_sampler->Sample(ray, rWorld, ff_samp, arena, mi, tMin,
//                                       tMax, scats, flags, mode);
//     // }

//     // if (scats) std::cout << "this should not happen" << std::endl;

//     CustomVertexType old_type = getVertex0Type(flags);
//     // std::cout << "ack" << std::endl;
//     resetVertex(flags);
//     if (scats) {
//         setVertex1Type(CustomVertexType::VERTEX_MEDIUM, flags);
//     } else {
//         setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//     }
//     setVertex0Type(old_type, flags);

//     // multiply by transmittance if ff sampling method requires it
//     if (ff_sampler->RequiresTr()) {
//         // std::cout << "ahah" << std::endl;
//         ret *= LocalTr(ray, trans_samp, tMin, tMax, flags, mode);
//     }

//     if (ff_sampler->RequiresTrOnScatterOnly() && scats) {
//         // std::cout << "ahah" << std::endl;
//         ret *= LocalTr(ray, trans_samp, tMin, mi->tMax, flags, mode);
//     }

//     if (ff_sampler->RequiresTrOnScatterOnly() && !scats) {
//         // std::cout << "ahah" << std::endl;
//         ret *= LocalTr(ray, trans_samp, tMin, tMax, flags, mode);
//     }

//     return ret;
// }

// Spectrum HeterogeneousNonExpMedium::Sample_Beta(
//     const Ray& rWorld, Sampler& ff_samp, Sampler& trans_samp, Float beta,
//     Float c, MemoryArena& arena, MediumInteraction* mi, uint32_t flags,
//     TransportMode mode) const {
//     // TODO: update this
//     ProfilePhase _(Prof::MediumSample);

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // std::cout << "BOOM" << std::endl;

//     // check bounds
//     if (outsideBounds) return Spectrum(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;
//     bool scats = false;

//     // if (!startOnSurface) std::cout << "Fuck beta" << std::endl;
//     // if (!adjoint) std::cout << "Fuck beta 2" << std::endl;

//     // return the sampled contribution and medium interaction
//     // Spectrum ret = Spectrum(1.0);
//     // if (sigma_s != Spectrum(0.0)) {
//     Spectrum ret =
//         ff_sampler->Sample_Beta(ray, rWorld, ff_samp, arena, mi, tMin, tMax,
//                                 beta, c, scats, flags, mode);
//     // }

//     CustomVertexType old_type = getVertex0Type(flags);
//     resetVertex(flags);
//     if (scats) {
//         setVertex1Type(CustomVertexType::VERTEX_MEDIUM, flags);
//     } else {
//         setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//     }
//     setVertex0Type(old_type, flags);

//     Spectrum tr = Spectrum(1.f);

//     // multiply by transmittance if ff sampling method requires it
//     if (ff_sampler->RequiresTr()) {
//         std::cout << "ask" << std::endl;
//         tr = LocalTr_Beta(ray, trans_samp, beta, c, tMin, tMax, flags, mode);
//     }

//     if (ff_sampler->RequiresTrOnScatterOnly() && scats) {
//         std::cout << "ask" << std::endl;
//         tr =
//             LocalTr_Beta(ray, trans_samp, beta, c, tMin, mi->tMax, flags, mode);
//     }

//     if (ff_sampler->RequiresTrOnScatterOnly() && !scats) {
//         std::cout << "ask" << std::endl;
//         tr = LocalTr_Beta(ray, trans_samp, beta, c, tMin, tMax, flags, mode);
//     }

//     // std::cout << "here right>" << std::endl;

//     // return tr;

//     ret *= tr;

//     return ret;
// }

// BlanchetCorrelatedResults HeterogeneousNonExpMedium::Sample_Blanchet(
//     const Ray& rWorld, Sampler& ff_samp, Sampler& trans_samp,
//     MemoryArena& arena, MediumInteraction* mi, uint32_t flags, int blanchet_n,
//     TransportMode mode) const {
//     // TODO: update this
//     ProfilePhase _(Prof::MediumSample);

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check bounds
//     if (outsideBounds) return BlanchetCorrelatedResults(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;
//     bool scats = false;

//     // return the sampled contribution and medium interaction
//     // BlanchetCorrelatedResults ret = BlanchetCorrelatedResults(1.0);
//     // if (sigma_s != Spectrum(0.0)) {
//     BlanchetCorrelatedResults ret =
//         ff_sampler->Sample_Blanchet(ray, rWorld, ff_samp, arena, mi, tMin, tMax,
//                                     scats, blanchet_n, flags, mode);
//     // }

//     // std::cout << ret.bias << std::endl;
//     CustomVertexType old_type = getVertex0Type(flags);
//     resetVertex(flags);
//     if (scats) {
//         setVertex1Type(CustomVertexType::VERTEX_MEDIUM, flags);
//     } else {
//         setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//     }
//     setVertex0Type(old_type, flags);

//     // multiply by transmittance if ff sampling method requires it
//     if (ff_sampler->RequiresTr()) {
//         ret *= LocalTr_Blanchet(ray, trans_samp, tMin, tMax, blanchet_n, flags,
//                                 mode);
//     }

//     if (ff_sampler->RequiresTrOnScatterOnly() && scats) {
//         ret *= LocalTr_Blanchet(ray, trans_samp, tMin, mi->tMax, blanchet_n,
//                                 flags, mode);
//     }

//     if (ff_sampler->RequiresTrOnScatterOnly() && !scats) {
//         ret *= LocalTr_Blanchet(ray, trans_samp, tMin, tMax, blanchet_n, flags,
//                                 mode);
//     }

//     // std::cout << "END" << std::endl;

//     return ret;
// }

// BlanchetCorrelatedResults HeterogeneousNonExpMedium::Sample_Blanchet_Beta(
//     const Ray& rWorld, Sampler& ff_samp, Sampler& trans_samp, Float beta,
//     Float c, MemoryArena& arena, MediumInteraction* mi, uint32_t flags,
//     int blanchet_n, TransportMode mode) const {
//     // TODO: update this
//     ProfilePhase _(Prof::MediumSample);

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check bounds
//     if (outsideBounds) return BlanchetCorrelatedResults(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;
//     bool scats = false;

//     // if (!startOnSurface) std::cout << "Fuck blanchet beta" << std::endl;
//     // if (!adjoint) std::cout << "Fuck blanchet beta 2" << std::endl;

//     // return the sampled contribution and medium interaction
//     // BlanchetCorrelatedResults ret = BlanchetCorrelatedResults(1.0);
//     // if (sigma_s != Spectrum(0.0)) {
//     BlanchetCorrelatedResults ret = ff_sampler->Sample_Blanchet_Beta(
//         ray, rWorld, ff_samp, beta, c, arena, mi, tMin, tMax, scats, blanchet_n,
//         flags, mode);
//     // }

//     // std::cout << ret.bias << std::endl;
//     CustomVertexType old_type = getVertex0Type(flags);
//     resetVertex(flags);
//     if (scats) {
//         setVertex1Type(CustomVertexType::VERTEX_MEDIUM, flags);
//     } else {
//         setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//     }
//     setVertex0Type(old_type, flags);

//     BlanchetCorrelatedResults tr = BlanchetCorrelatedResults(1.f);

//     // multiply by transmittance if ff sampling method requires it
//     if (ff_sampler->RequiresTr()) {
//         std::cout << "what the fuf" << std::endl;
//         tr = LocalTr_Blanchet_Beta(ray, trans_samp, beta, c, tMin, tMax,
//                                    blanchet_n, flags, mode);
//     }

//     if (ff_sampler->RequiresTrOnScatterOnly() && scats) {
//         std::cout << "what the fuf" << std::endl;
//         tr = LocalTr_Blanchet_Beta(ray, trans_samp, beta, c, tMin, mi->tMax,
//                                    blanchet_n, flags, mode);
//     }

//     if (ff_sampler->RequiresTrOnScatterOnly() && !scats) {
//         std::cout << "what the fuf" << std::endl;
//         tr = LocalTr_Blanchet_Beta(ray, trans_samp, beta, c, tMin, tMax,
//                                    blanchet_n, flags, mode);
//     }

//     // return tr;

//     ret *= tr;

//     // ret *= Spectrum(0.5);

//     // std::cout << "END" << std::endl;

//     return ret;
// }

// // THIS IS CURRENTLY BROKEN : ERROR
// BlanchetCorrelatedFdDebiasing HeterogeneousNonExpMedium::Sample_Blanchet_FdBeta(
//     const Ray& rWorld, Sampler& ff_samp, Sampler& trans_samp,
//     MemoryArena& arena, MediumInteraction* mi, uint32_t flags, int blanchet_n,
//     int davis_n, Float base_h, Float davis_beta, Float davis_c,
//     TransportMode mode) const {
//     // TODO: update this
//     ProfilePhase _(Prof::MediumSample);

//     Ray ray;
//     Float tMin, tMax;

//     // generate a transformed ray and bounds
//     bool outsideBounds = TransformToLocalSpace(rWorld, ray, tMin, tMax);

//     // check bounds
//     if (outsideBounds) return BlanchetCorrelatedFdDebiasing(1.f);

//     bool startOnSurface =
//         getVertex0Type(flags) == CustomVertexType::VERTEX_SURFACE;
//     bool adjoint = mode == TransportMode::Radiance;
//     bool scats = false;

//     // return the sampled contribution and medium interaction
//     BlanchetCorrelatedFdDebiasing ret = ff_sampler->Sample_Blanchet_FdBeta(
//         ray, rWorld, ff_samp, arena, mi, tMin, tMax, scats, blanchet_n, davis_n,
//         base_h, davis_beta, davis_c, flags, mode);

//     // std::cout << ret.bias << std::endl;
//     CustomVertexType old_type = getVertex0Type(flags);
//     resetVertex(flags);
//     if (scats) {
//         setVertex1Type(CustomVertexType::VERTEX_MEDIUM, flags);
//     } else {
//         setVertex1Type(CustomVertexType::VERTEX_SURFACE, flags);
//     }
//     setVertex0Type(old_type, flags);

//     BlanchetCorrelatedFdDebiasing tr = BlanchetCorrelatedFdDebiasing(1.f);

//     // if (scats)
//     // {
//     //     tr = LocalTr_Blanchet_FdBeta
//     //     (
//     //         ray,
//     //         trans_samp,
//     //         tMin,
//     //         mi->tMax,
//     //         blanchet_n,
//     //         davis_n,
//     //         base_h,
//     //         davis_beta,
//     //         davis_c,
//     //         flags,
//     //         mode
//     //     );
//     // }
//     // else
//     // {
//     //     tr = LocalTr_Blanchet_FdBeta
//     //     (
//     //         ray,
//     //         trans_samp,
//     //         tMin,
//     //         tMax,
//     //         blanchet_n,
//     //         davis_n,
//     //         base_h,
//     //         davis_beta,
//     //         davis_c,
//     //         flags,
//     //         mode
//     //     );
//     // }

//     ret *= tr;

//     return ret;
// }

// // This returns true if the ray is outside the medium bounds
// bool HeterogeneousNonExpMedium::TransformToLocalSpace(const Ray& rWorld,
//                                                       Ray& ray, Float& tMin,
//                                                       Float& tMax) const {
//     // transform the ray to medium space
//     ray = WorldToMedium(
//         Ray(rWorld.o, Normalize(rWorld.d), rWorld.tMax * rWorld.d.Length()));

//     if (!mediumBounds.IntersectP(ray, &tMin, &tMax)) {
//         return true;
//     }

//     return false;
// }
// }  // namespace pbrt
