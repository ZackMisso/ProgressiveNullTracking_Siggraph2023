#pragma once

#include "pbrt.h"
#include "materials/hair.h"
#include "spectrum.h"

namespace pbrt
{

// are these methods really needed???
static inline Spectrum Min(const Spectrum &a, Float b)
{
    Spectrum result;
    for (int i = 0; i < a.nSamples; ++i)
        result[i] = std::min(a[i], b);
    return result;
}
static inline Spectrum Max(const Spectrum &a, Float b)
{
    Spectrum result;
    for (int i = 0; i < a.nSamples; ++i)
        result[i] = std::max(a[i], b);
    return result;
}
static inline Spectrum Step(Float edge, const Spectrum &x)
{
    Spectrum result;
    for (int i = 0; i < x.nSamples; ++i)
        result[i] = x[i] > edge ? 1.0 : 0.0;
    return result;
}
static inline Spectrum Step(const Spectrum &edge, Float x)
{
    Spectrum result;
    for (int i = 0; i < edge.nSamples; ++i)
        result[i] = x > edge[i] ? 1.0 : 0.0;
    return result;
}
static inline Spectrum Boxcar(const Spectrum &x, Float a, Float b)
{
    Spectrum result;
    for (int i = 0; i < x.nSamples; ++i)
        result[i] = x[i] >= a && x[i] <= b ? 1.0 : 0.0;
    return result;
}
static inline Float Sum(const Spectrum &x)
{
    Float result = x[0];
    for (int i = 1; i < x.nSamples; ++i)
        result += x[i];
    return result;
}
static inline Float Avg(const Spectrum &x)
{
    return Sum(x) / x.nSamples;
}

class Transmittance_Func
{
public:
    enum Mode
    {
        NecSurface,
        NecMedium,
        WeakReciprocal,
        Reciprocal,
    };

    Transmittance_Func() : mode(Reciprocal) { }
    virtual ~Transmittance_Func() { }

    // Mode mode() const { return mode; }
    void setMode(Mode m) { mode = m; }

    inline Spectrum eval(Spectrum tau,
                         bool startOnSurface,
                         bool endOnSurface,
                         bool adjoint) const
    {
        if (mode == NecSurface)
        {
            return !adjoint ? (  endOnSurface ? surfaceSurface(tau) : surfaceMedium(tau))
                            : (startOnSurface ? surfaceSurface(tau) : surfaceMedium(tau));
        }
        else if (mode == NecMedium)
        {
            return !adjoint ? (  endOnSurface ? mediumSurface(tau) : mediumMedium(tau))
                            : (startOnSurface ? mediumSurface(tau) : mediumMedium(tau));
        }
        else if (mode == WeakReciprocal)
        {
            if (startOnSurface && endOnSurface) return surfaceSurface(tau);
            else if (!startOnSurface && !endOnSurface) return mediumMedium(tau);
            else if (!adjoint ^ endOnSurface) return surfaceMedium(tau);
            else return mediumSurface(tau);
        }
        else
        {
            if (startOnSurface && endOnSurface) return surfaceSurface(tau);
            else if (!startOnSurface && !endOnSurface)
            {
                return mediumMedium(tau) / sigmaBar();
            }
            else return mediumSurface(tau);
        }
    }

    inline Spectrum eval(Spectrum tau,
                         Float beta, Float c,
                         bool startOnSurface,
                         bool endOnSurface,
                         bool adjoint) const
    {
        if (mode == NecSurface)
        {
            return !adjoint ? (  endOnSurface ? surfaceSurface(tau, beta, c) : surfaceMedium(tau, beta, c))
                            : (startOnSurface ? surfaceSurface(tau, beta, c) : surfaceMedium(tau, beta, c));
        }
        else if (mode == NecMedium)
        {
            return !adjoint ? (  endOnSurface ? mediumSurface(tau, beta, c) : mediumMedium(tau, beta, c))
                            : (startOnSurface ? mediumSurface(tau, beta, c) : mediumMedium(tau, beta, c));
        }
        else if (mode == WeakReciprocal)
        {
            if (startOnSurface && endOnSurface) return surfaceSurface(tau, beta, c);
            else if (!startOnSurface && !endOnSurface) return mediumMedium(tau, beta, c);
            else if (!adjoint ^ endOnSurface) return surfaceMedium(tau, beta, c);
            else return mediumSurface(tau, beta, c);
        }
        else
        {
            if (startOnSurface && endOnSurface) return surfaceSurface(tau, beta, c);
            else if (!startOnSurface && !endOnSurface)
            {
                return mediumMedium(tau, beta, c) / sigmaBar();
            }
            else return mediumSurface(tau, beta, c);
        }
    }

    inline Float sample(Sampler &sampler, bool startOnSurface) const
    {
        if (mode == NecSurface) return sampleSurface(sampler);
        else if (mode == NecMedium) return sampleMedium(sampler);
        else return startOnSurface ? sampleSurface(sampler) : sampleMedium(sampler);
    }

    inline Float sample(Sampler &sampler, Float bbeta, Float cc, bool startOnSurface) const
    {
        if (mode == NecSurface) return sampleSurface(sampler, bbeta, cc);
        else if (mode == NecMedium) return sampleMedium(sampler, bbeta, cc);
        else return startOnSurface ? sampleSurface(sampler, bbeta, cc) : sampleMedium(sampler, bbeta, cc);
    }

    inline Spectrum surfaceProbability(Spectrum tau, bool startOnSurface) const
    {
        if (mode == NecSurface) return surfaceSurface(tau);
        else if (mode == NecMedium) return mediumSurface(tau);
        else return startOnSurface ? surfaceSurface(tau) : mediumSurface(tau);
    }
    inline Spectrum mediumPdf(Spectrum tau, bool startOnSurface) const
    {
        if (mode == NecSurface) return surfaceMedium(tau);
        else if (mode == NecMedium) return mediumMedium(tau);
        else return startOnSurface ? surfaceMedium(tau) : mediumMedium(tau);
    }

    virtual bool isDirac() const
    {
        return false;
    }

    virtual Float effectiveSigma() const
    {
        if (mode == Reciprocal)
            return sigmaBar();
        return 1.0f;
    }

    virtual bool isDavis() const { return false; }
    virtual Float davisC() const { return 1.0; }
    virtual Float davisBeta() const { return 1.0; }

    virtual Spectrum surfaceSurface(Spectrum tau) const = 0;
    virtual Spectrum surfaceMedium(Spectrum tau) const = 0;
    virtual Spectrum mediumSurface(Spectrum tau) const = 0;
    virtual Spectrum mediumMedium(Spectrum tau) const = 0;

    virtual Spectrum surfaceSurface(Spectrum tau, Float bbeta, Float cc) const
    {
        return surfaceSurface(tau);
    }

    virtual Spectrum surfaceMedium(Spectrum tau, Float bbeta, Float cc) const
    {
        return surfaceMedium(tau);
    }

    virtual Spectrum mediumSurface(Spectrum tau, Float bbeta, Float cc) const
    {
        return mediumSurface(tau);
    }

    virtual Spectrum mediumMedium(Spectrum tau, Float bbeta, Float cc) const
    {
        return mediumMedium(tau);
    }

    virtual Float sigmaBar() const = 0;

    virtual Float sampleSurface(Sampler &sampler) const = 0;
    virtual Float sampleMedium(Sampler &sampler) const = 0;

    virtual Float sampleSurface(Sampler &sampler, Float bbeta, Float cc) const
    {
        return sampleSurface(sampler);
    }

    virtual Float sampleMedium(Sampler &sampler, Float bbeta, Float cc) const
    {
        return sampleMedium(sampler);
    }

    Mode mode;
};

}
