#pragma once

#include "pbrt.h"
#include "texture.h"
#include "paramset.h"

namespace pbrt
{

class SineTexture : public Texture<Spectrum>
{
public:
    SineTexture(ActiveTextureAxis activeAxis,
                Float period,
                Float amplitude,
                Float phase = 0.0);
    SineTexture(ActiveTextureAxis axtiveAxis,
                Spectrum period,
                Spectrum amplitude,
                Spectrum phase = Spectrum(0.0));

    Spectrum Evaluate(const SurfaceInteraction& si) const;
    Spectrum Evaluate(const Point3f& p) const;

    void setPeriod(Spectrum param);
    void setAmplitude(Spectrum param);
    void setPhase(Spectrum param);

protected:
    ActiveTextureAxis activeAxis;
    Spectrum period;
    Spectrum amplitude;
    Spectrum phase;
};

SineTexture* CreateSineTexture(const Transform& tex2World);
SineTexture* CreateSineTexture();
SineTexture* CreateSineTexture(ActiveTextureAxis activeAxis,
                               Float period,
                               Float amplitude,
                               Float phase);

// class SineTexture_Float : public Texture<Float>
// {
// public:
//     SineTexture_Float(Float period,
//                       Float amplitude,
//                       Float phase = 0.0);
//     SineTexture_Float(Spectrum period,
//                       Spectrum amplitude,
//                       Spectrum phase = Spectrum(0.0));
//
//     Float Evaluate(const SurfaceInteraction& si) const;
//     Float Evaluate(const Point3f& p) const;
//
// protected:
//     ActiveTextureAxis activeAxis;
//     Spectrum period;
//     Spectrum amplitude;
//     Spectrum phase;
// };

}
