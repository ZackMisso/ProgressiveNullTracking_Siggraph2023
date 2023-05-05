#include "sine.h"

namespace pbrt
{

SineTexture::SineTexture(ActiveTextureAxis activeAxis,
                         Float period,
                         Float amplitude,
                         Float phase)
    : activeAxis(activeAxis),
      period(Spectrum(period)),
      amplitude(Spectrum(amplitude)),
      phase(Spectrum(phase))
{
    // does nothing
}

SineTexture::SineTexture(ActiveTextureAxis activeAxis,
                         Spectrum period,
                         Spectrum amplitude,
                         Spectrum phase)
    : activeAxis(activeAxis),
      period(period),
      amplitude(amplitude),
      phase(phase)
{
    // does nothing
}

Spectrum SineTexture::Evaluate(const SurfaceInteraction& si) const
{
    std::cout << "Surface Interactions for Sine Textures is not supported yet" << std::endl;
    // TODO
    Spectrum(1.f);
}

Spectrum SineTexture::Evaluate(const Point3f& p) const
{
    switch (activeAxis)
    {
        case AXIS_X:
        {
            Float val = amplitude[0] * sin(period[0] * p.x + phase[0]) + amplitude[0];
            return Spectrum(abs(val));
        }
        case AXIS_Y:
        {
            Float val = amplitude[1] * sin(period[1] * p.y + phase[1]) + amplitude[1];
            return Spectrum(abs(val));
        }
        case AXIS_Z:
        {
            Float val = amplitude[2] * sin(period[2] * p.z + phase[2]) + amplitude[2];
            return Spectrum(abs(val));
        }
        case AXIS_XY:
        {
            Float val = amplitude[0] * sin(period[0] * p.x + period[1] * p.y + phase[0] + phase[1]) + amplitude[0];
            return Spectrum(abs(val));
        }
        case AXIS_XZ:
        {
            Float val = amplitude[0] * sin(period[0] * p.x + period[2] * p.z + phase[0] + phase[2]) + amplitude[0];
            return Spectrum(abs(val));
        }
        case AXIS_YZ:
        {
            // Float val = amplitude[1] * sin(period[2] * p.z + period[1] * p.y + phase[2] + phase[1]) + amplitude[1];
            // return Spectrum(abs(val));

            // std::cout << "phase: " << phase[0] << std::endl;
            // std::cout << "y: " << 10.0 * p.y << std::endl;
            // std::cout << "z: " << 10.0 * p.z << std::endl;

            Float val = amplitude[0] * sin(period[0] * (10.0 * p.z + 10.0 * p.y + phase[0])) + amplitude[0];
            return Spectrum(val);
        }
        case AXIS_XYZ:
        {
            Float val = amplitude[0] * sin(period[2] * p.z + period[1] * p.y + period[0] * p.x + phase[2] + phase[1] + phase[0]) + amplitude[0];
            return Spectrum(abs(val));
        }
        // case AXIS_ALL_SEPARATE:
        // {
        //     Float rgb[3];
        //
        //     rgb[0] = amplitude[0] * sin(period[0] * p.x + phase[0]);
        //     rgb[1] = amplitude[1] * sin(period[1] * p.y + phase[1]);
        //     rgb[2] = amplitude[2] * sin(period[2] * p.z + phase[2]);
        //
        //     return RGBSpectrum::FromRGB(rgb);
        // }
        default:
        {
            Float rgb[3];

            rgb[0] = abs(amplitude[0] * sin(period[0] * p.x + phase[0]));
            rgb[1] = abs(amplitude[1] * sin(period[1] * p.y + phase[1]));
            rgb[2] = abs(amplitude[2] * sin(period[2] * p.z + phase[2]));

            return RGBSpectrum::FromRGB(rgb);
        }
    }
}

void SineTexture::setPeriod(Spectrum param) { period = param; }
void SineTexture::setAmplitude(Spectrum param) { amplitude = param; }
void SineTexture::setPhase(Spectrum param) { phase = param; }

SineTexture* CreateSineTexture(const Transform& tex2World)
{
    // TODO: fully implement
    // return new SineTexture(AXIS_YZ, 20.0, 1.0, 0.0);
    return new SineTexture(AXIS_YZ, 5.0, 0.5, 0.0);
}

SineTexture* CreateSineTexture()
{
    // TODO: fully implement
    // return new SineTexture(AXIS_YZ, 20.0, 1.0, 0.0);
    return new SineTexture(AXIS_YZ, 5.0, 0.5, 0.0);
}

SineTexture* CreateSineTexture(ActiveTextureAxis activeAxis,
                               Float period,
                               Float amplitude,
                               Float phase)
{
    return new SineTexture(activeAxis, period, amplitude, phase);
}

}
