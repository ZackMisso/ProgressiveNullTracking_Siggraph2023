#include "marble_media.h"

namespace pbrt
{

MarbleProceduralMedia::MarbleProceduralMedia()
{
    // baseTexture = CreateSineTexture();

    baseTexture = CreateSineTexture(AXIS_Y, 100.0, 0.5, 0.0);
}

Spectrum MarbleProceduralMedia::Evaluate(const SurfaceInteraction& si) const
{
    std::cout << "ERROR: Marble Procedural Media Surface Interactions not implemented" << std::endl;
    // TODO
    return Spectrum(1.0);
}

Spectrum MarbleProceduralMedia::Evaluate(const Point3f& p) const
{
    // Float x_period = 5.0;
    // Float y_period = 5.0;
    //

    // if (Noise(0.0, p.y, 0.0) < 0.0) std::cout << " NEG: " << p.y << std::endl;
    // return (0.5 * FBm_yz(p,
    //               Vector3f(0.001, 0.001, 0.001),
    //               Vector3f(0.001, 0.001, 0.001),
    //               0.5,
    //               8) + 0.5);

    // return baseTexture->Evaluate(p);

    Float val = 0.5 * FBm_y(Point3f(p.x * 1.0, p.y * 1.0, p.z * 1.0),
                            Vector3f(0.001, 0.001, 0.001),
                            Vector3f(0.001, 0.001, 0.001),
                            1.0,
                            8) + 0.5;


    // Float val = 0.5 * FBm_yz(Point3f(p.x * 1.0, p.y * 1.0, p.z * 1.0),
    //                          Vector3f(0.001, 0.001, 0.001),
    //                          Vector3f(0.001, 0.001, 0.001),
    //                          1.0,
    //                          8) + 0.5;


    // return Spectrum(val);

    // std::cout << "Val: " << val << std::endl;

    // val = 0.0;

    baseTexture->setPhase(Spectrum(val));

    // return baseTexture->Evaluate(p);

    Spectrum ret = baseTexture->Evaluate(p);

    ret[0] = 0.0;

    return ret;

    // return (0.5 * Noise(0.0, p.y, p.z) + 0.5) * 1.0;

    Float turbPower = 1.0;
    // Float turbSize = 32.0;

    // Spectrum phase;
    // Spectrum period;
    // Spectrum amplitude;

    // std::cout << "what" << std::endl;

    Float y_FBm = turbPower * FBm_y(p,
                                    Vector3f(0.01, 0.01, 0.01),
                                    Vector3f(0.01, 0.01, 0.01),
                                    0.5,
                                    8);

    Float z_FBm = turbPower * FBm_z(p,
                                    Vector3f(0.01, 0.01, 0.01),
                                    Vector3f(0.01, 0.01, 0.01),
                                    0.5,
                                    8);

    // z_FBm = 0.0;
    // y_FBm = 0.0;

    // std::cout << "what" << std::endl;

    baseTexture->setPhase(Spectrum(y_FBm + z_FBm));

    return baseTexture->Evaluate(p);
}

MarbleProceduralMedia* CreateMarbleProceduralMedia()
{
    return new MarbleProceduralMedia();
}

}
