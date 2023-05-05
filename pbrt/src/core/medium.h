
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

#ifndef PBRT_CORE_MEDIUM_H
#define PBRT_CORE_MEDIUM_H

// core/medium.h*
#include <atomic>
#include <memory>

#include "blanchet_spectrum.h"
#include "fd_cor.h"
#include "geometry.h"
#include "material.h"
#include "pbrt.h"
#include "phase.h"
#include "spectrum.h"
#include "trans_samplers/trans_sampler.h"

namespace pbrt {

inline std::ostream &operator<<(std::ostream &os, const PhaseFunction &p) {
    os << p.ToString();
    return os;
}

bool GetMediumScatteringProperties(const std::string &name, Spectrum *sigma_a,
                                   Spectrum *sigma_s);

class Medium {
  public:
    // Medium Interface
    virtual ~Medium() {}

#if RENDER_MUTABLE
    virtual Spectrum Tr(const Ray &ray, Sampler &sampler) = 0;

    virtual Spectrum Tr(const Ray &ray, Sampler &sampler, Float sigma_t) {
        return Tr(ray, sampler);
    }

    virtual Spectrum Tr(const Ray &ray, Sampler &sampler, uint32_t flags,
                        TransportMode mode = TransportMode::Radiance) {
        return Tr(ray, sampler);
    }

    virtual BlanchetCorrelatedResults Tr_Blanchet(
        const Ray &ray, Sampler &sampler, uint32_t flags, int blanchet_n,
        TransportMode mode = TransportMode::Radiance) {
        std::cout << "ERROR Med" << std::endl;
        return BlanchetCorrelatedResults(Tr(ray, sampler));
    }

    virtual BlanchetCorrelatedResults Tr_Blanchet(const Ray &ray,
                                                  Sampler &sampler,
                                                  int blanchet_n) {
        std::cout << "ERROR" << std::endl;
        return BlanchetCorrelatedResults(Tr(ray, sampler));
    }

    virtual Spectrum LocalTr(const Ray &ray, Sampler &sampler, Float tMin,
                             Float tMax) {
        return Spectrum(0.0);
    };

    virtual Spectrum LocalTr(const Ray &ray, Sampler &sampler, Float tMin,
                             Float tMax, uint32_t flags) {
        return LocalTr(ray, sampler, tMin, tMax);
    };

    virtual BlanchetCorrelatedResults LocalTr_Blanchet(const Ray &ray,
                                                       Sampler &sampler,
                                                       Float tMin, Float tMax,
                                                       int blanchet_n) {
        std::cout << "ERROR" << std::endl;
        return BlanchetCorrelatedResults(LocalTr(ray, sampler, tMin, tMax));
    }

    virtual Spectrum Sample(const Ray &ray, Sampler &ff_sampler,
                            Sampler &trans_sampler, MemoryArena &arena,
                            MediumInteraction *mi) = 0;

    // non-exponential sampling
    virtual Spectrum Sample(const Ray &ray, Sampler &ff_sampler,
                            Sampler &trans_sampler, MemoryArena &arena,
                            MediumInteraction *mi, uint32_t flags,
                            TransportMode mode = TransportMode::Radiance) {
        return Sample(ray, ff_sampler, trans_sampler, arena, mi);
    }

    virtual BlanchetCorrelatedResults Sample_Blanchet(
        const Ray &ray, Sampler &ff_sampler, Sampler &trans_sampler,
        MemoryArena &arena, MediumInteraction *mi, uint32_t flags,
        int blanchet_n, TransportMode mode = TransportMode::Radiance) {
        return Sample(ray, ff_sampler, trans_sampler, arena, mi);
    }

    virtual BlanchetCorrelatedResults Sample_Blanchet(
        const Ray &ray, Sampler &ff_sampler, Sampler &trans_sampler,
        MemoryArena &arena, MediumInteraction *mi, int blanchet_n) {
        return BlanchetCorrelatedResults(
            Sample(ray, ff_sampler, trans_sampler, arena, mi));
    }

    //// MUT FD FUNCTIONS ////
    virtual Spectrum Sample(const Ray &rWorld, Sampler &ff_sampler,
                            Sampler &trans_sampler, MemoryArena &arena,
                            MediumInteraction *mi, Float sigma_t,
                            Float sigma_s) {
        return Spectrum(1.0);
    }

    virtual Spectrum LocalTr(const Ray &ray, Sampler &sampler, Float tMin,
                             Float tMax, Float sigma_t) {
        return Spectrum(1.0);
    }
#else
    virtual Spectrum Tr(const Ray &ray, Sampler &sampler) const = 0;

    virtual Spectrum Tr(const Ray &ray, Sampler &sampler, Float sigma_t) const {
        return Tr(ray, sampler);
    }

    virtual Spectrum Tr(const Ray &ray, Sampler &sampler, uint32_t flags,
                        TransportMode mode = TransportMode::Radiance) const {
        return Tr(ray, sampler);
    }

    virtual BlanchetCorrelatedResults Tr_Blanchet(
        const Ray &ray, Sampler &sampler, uint32_t flags, int blanchet_n,
        TransportMode mode = TransportMode::Radiance) const {
        std::cout << "ERROR Med" << std::endl;
        return BlanchetCorrelatedResults(Tr(ray, sampler));
    }

    virtual BlanchetCorrelatedResults Tr_Blanchet(const Ray &ray,
                                                  Sampler &sampler,
                                                  int blanchet_n) const {
        std::cout << "ERROR" << std::endl;
        return BlanchetCorrelatedResults(Tr(ray, sampler));
    }

    virtual Spectrum LocalTr(const Ray &ray, Sampler &sampler, Float tMin,
                             Float tMax) const {
        return Spectrum(0.0);
    };

    virtual Spectrum LocalTr(const Ray &ray, Sampler &sampler, Float tMin,
                             Float tMax, uint32_t flags) const {
        return LocalTr(ray, sampler, tMin, tMax);
    };

    virtual BlanchetCorrelatedResults LocalTr_Blanchet(const Ray &ray,
                                                       Sampler &sampler,
                                                       Float tMin, Float tMax,
                                                       int blanchet_n) const {
        std::cout << "ERROR" << std::endl;
        return BlanchetCorrelatedResults(LocalTr(ray, sampler, tMin, tMax));
    }

    virtual Spectrum Sample(const Ray &ray, Sampler &ff_sampler,
                            Sampler &trans_sampler, MemoryArena &arena,
                            MediumInteraction *mi) const = 0;

    // non-exponential sampling
    virtual Spectrum Sample(
        const Ray &ray, Sampler &ff_sampler, Sampler &trans_sampler,
        MemoryArena &arena, MediumInteraction *mi, uint32_t flags,
        TransportMode mode = TransportMode::Radiance) const {
        return Sample(ray, ff_sampler, trans_sampler, arena, mi);
    }

    virtual BlanchetCorrelatedResults Sample_Blanchet(
        const Ray &ray, Sampler &ff_sampler, Sampler &trans_sampler,
        MemoryArena &arena, MediumInteraction *mi, uint32_t flags,
        int blanchet_n, TransportMode mode = TransportMode::Radiance) const {
        return Sample(ray, ff_sampler, trans_sampler, arena, mi);
    }

    virtual BlanchetCorrelatedResults Sample_Blanchet(
        const Ray &ray, Sampler &ff_sampler, Sampler &trans_sampler,
        MemoryArena &arena, MediumInteraction *mi, int blanchet_n) const {
        return BlanchetCorrelatedResults(
            Sample(ray, ff_sampler, trans_sampler, arena, mi));
    }

    //// MUT FD FUNCTIONS ////
    virtual Spectrum Sample(const Ray &rWorld, Sampler &ff_sampler,
                            Sampler &trans_sampler, MemoryArena &arena,
                            MediumInteraction *mi, Float sigma_t,
                            Float sigma_s) const {
        return Spectrum(1.0);
    }

    virtual Spectrum LocalTr(const Ray &ray, Sampler &sampler, Float tMin,
                             Float tMax, Float sigma_t) const {
        return Spectrum(1.0);
    }
#endif

    // TODO: this should be made a required method
    virtual bool TransformToLocalSpace(const Ray &rWorld, Ray &ray, Float &tMin,
                                       Float &tMax) const {
        return false;
    }

    virtual void RenderVerticalSlice(std::string filename, int ps) const {}

    virtual Spectrum Majorant(const Ray &ray) { return Spectrum(0.f); };

    virtual void Update_Sigma(Spectrum new_sigma_a, Spectrum new_sigma_s) = 0;

    virtual Float G() const { return 1.0; }
    virtual Spectrum ScatCoeff(const Point3f &pt) const {
        return Spectrum(1.0);
    }

    virtual Spectrum Sigma_S() const { return Spectrum(1.0); }
    virtual Spectrum Sigma_A() const { return Spectrum(1.0); }
    virtual Spectrum Sigma_T() const { return Spectrum(1.0); }

    static std::atomic<long> nTrCalls;
    static std::atomic<long> nDensityCalls;
};

// MediumInterface Declarations
struct MediumInterface {
    MediumInterface() : inside(nullptr), outside(nullptr) {}
    // MediumInterface Public Methods

#if RENDER_MUTABLE
    MediumInterface(Medium *medium) : inside(medium), outside(medium) {}
    MediumInterface(Medium *inside, Medium *outside)
        : inside(inside), outside(outside) {}
#else
    MediumInterface(const Medium *medium) : inside(medium), outside(medium) {}
    MediumInterface(const Medium *inside, const Medium *outside)
        : inside(inside), outside(outside) {}
#endif

    bool IsMediumTransition() const { return inside != outside; }

#if RENDER_MUTABLE
    Medium *inside, *outside;
#else
    const Medium *inside, *outside;
#endif
};

}  // namespace pbrt

#endif  // PBRT_CORE_MEDIUM_H
