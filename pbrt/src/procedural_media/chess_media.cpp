#include "chess_media.h"
#include "interaction.h"
#include "../ImageEditLib/include/imedit/filter.h"
#include "../ImageEditLib/include/imedit/noise.h"
#include "../ImageEditLib/include/imedit/procedural.h"
#include "../ImageEditLib/include/imedit/im_util.h"

namespace pbrt
{

Chess_Media::Chess_Media(const Bounds3f& medBounds,
                         Float hue,
                         Float sat,
                         Float turb_period,
                         Float xtrans,
                         Float ytrans,
                         Float ztrans,
                         Float g_const,
                         Float x_const,
                         Float t_const,
                         Float phase,
                         Float power)
    : medBounds(medBounds),
      hue(hue),
      sat(sat),
      turb_period(turb_period),
      xtrans(xtrans),
      ytrans(ytrans),
      ztrans(ztrans),
      g_const(g_const),
      x_const(x_const),
      t_const(t_const),
      phase(phase),
      power(power)
{
    // Float mesh_min_h = 100.0 / 360.0;
    // Float mesh_max_h = 140.0 / 360.0;
    // Float mesh_min_s = 0.62;
    // Float mesh_max_s = 0.84;
    // Float mesh_min_l = 0.60;
    // Float mesh_max_l = 0.90;
    //
    // Float min_x_period = (medBounds.pMax.x - medBounds.pMin.x) * 0.1;
    // Float min_y_period = (medBounds.pMax.y - medBounds.pMin.y) * 0.1;
    // Float min_z_period = (medBounds.pMax.z - medBounds.pMin.z) * 0.1;
    //
    // Float max_x_period = (medBounds.pMax.x - medBounds.pMin.x) * 0.4;
    // Float max_y_period = (medBounds.pMax.y - medBounds.pMin.y) * 0.4;
    // Float max_z_period = (medBounds.pMax.z - medBounds.pMin.z) * 0.4;
    //
    // unsigned int seed_one = 0x452fb2;
    // unsigned int seed_two = 0x931abc;
    //
    // Float max_trans = 1000000.0;
    //
    // pcg32 rng = pcg32(seed_one, seed_two);
    //
    // x_period = rng.nextDouble() * (max_x_period - min_x_period) + min_x_period;
    // y_period = rng.nextDouble() * (max_y_period - min_y_period) + min_y_period;
    // z_period = rng.nextDouble() * (max_z_period - min_z_period) + min_z_period;
    //
    // std::cout << "X Period: " << x_period << std::endl;
    // std::cout << "Y Period: " << y_period << std::endl;
    // std::cout << "Z Period: " << z_period << std::endl;
    //
    // x_period = 1.2;
    // y_period = 1.2;
    // z_period = 1.2;
    //
    // h = rng.nextDouble() * (mesh_max_h - mesh_min_h) + mesh_min_h;
    // s = rng.nextDouble() * (mesh_max_s - mesh_min_s) + mesh_min_s;
    // l = rng.nextDouble() * (mesh_max_l - mesh_min_l) + mesh_min_l;
    // h = mesh_max_h;
    //
    // xtrans = rng.nextDouble() * 1000000.0;
    // ytrans = rng.nextDouble() * 1000000.0;
    // ztrans = rng.nextDouble() * 1000000.0;
    //
    // xtrans = 0.0;
    // ytrans = 0.0;
    // ztrans = 0.0;
    //
    // // turb_period = 4096.0 / 16.0;
    // turb_period = 4096.0 / 5.0;
}

Chess_Media::~Chess_Media() { }

Spectrum Chess_Media::Evaluate(const SurfaceInteraction& si) const
{
    Float dist = 0.05;
    // add some artificial aliasing for appearances
    Spectrum ret = Spectrum(1.0) - (Evaluate(si.p) +
                                    Evaluate(si.p + Point3f(dist, 0.0, 0.0)) +
                                    Evaluate(si.p + Point3f(-dist, 0.0, 0.0)) +
                                    Evaluate(si.p + Point3f(0.0, dist, 0.0)) +
                                    Evaluate(si.p + Point3f(0.0, -dist, 0.0)) +
                                    Evaluate(si.p + Point3f(0.0, 0.0, dist)) +
                                    Evaluate(si.p + Point3f(0.0, 0.0, -dist))) / 7.0;

    if (ret[0] > 1.0) ret[0] = 1.0;
    if (ret[1] > 1.0) ret[1] = 1.0;
    if (ret[2] > 1.0) ret[2] = 1.0;

    if (ret[0] < 0.0) ret[0] = 0.0;
    if (ret[1] < 0.0) ret[1] = 0.0;
    if (ret[2] < 0.0) ret[2] = 0.0;

    return ret;
}

Spectrum Chess_Media::Evaluate(const Point3f& p) const
{
    Float t = imedit::turbulence(p.x / (medBounds.pMax.x - medBounds.pMin.x) * 4096.0 + xtrans,
                                 p.y / (medBounds.pMax.x - medBounds.pMin.x) * 4096.0 + ytrans,
                                 p.z / (medBounds.pMax.x - medBounds.pMin.x) * 4096.0 + ztrans,
                                 turb_period);

    Float x_val = (p.x - medBounds.pMin.x) / (medBounds.pMax.x - medBounds.pMin.x);

    t = pow(std::fabs(
                std::sin(
                    g_const * (x_const * x_val + t_const * t) - phase
                )
            ), power);

    // imedit::Pixel dark_color;
    // dark_color.r = hue;
    // dark_color.g = sat;
    // dark_color.b = 0.10;
    //
    // imedit::Pixel light_color;
    // light_color.r = hue;
    // light_color.g = sat;
    // light_color.b = 0.46;
    //
    // imedit::hsl_to_rgb(dark_color);
    // imedit::hsl_to_rgb(light_color);
    //
    // Float rgb_dark[3];
    // Float rgb_light[3];
    //
    // rgb_dark[0] = 1.0 - dark_color.r;
    // rgb_dark[1] = 1.0 - dark_color.g;
    // rgb_dark[2] = 1.0 - dark_color.b;
    //
    // rgb_light[0] = 1.0 - light_color.r;
    // rgb_light[1] = 1.0 - light_color.g;
    // rgb_light[2] = 1.0 - light_color.b;

    imedit::Pixel final_color;
    final_color.r = hue;
    final_color.g = sat;
    if (sat == 1.0)
    {
        final_color.b = 0.1 * t + 0.75 * (1.0 - t);
    }
    else
    {
        final_color.b = 0.1 * t + 0.46 * (1.0 - t);
    }

    imedit::hsl_to_rgb(final_color);

    Float rgb[3];
    rgb[0] = 1.0 - final_color.r;
    rgb[1] = 1.0 - final_color.g;
    rgb[2] = 1.0 - final_color.b;

    return Spectrum::FromRGB(rgb);

    // return (Spectrum::FromRGB(rgb_dark) * t + Spectrum::FromRGB(rgb_light) * (1.0 - t));
}

Chess_Media* CreateChessTextureMedia(const TextureParams& tp)
{
    ParamSet params = tp.GetMaterialParams();

    Bounds3f medBounds = Bounds3f(params.FindOnePoint3f("minBounds", Point3f(0.0, 0.0, 0.0)),
                                  params.FindOnePoint3f("maxBounds", Point3f(1.0, 1.0, 1.0)));

    Float hue = params.FindOneFloat("hue", 140.0 / 360.0) - 0.28;
    if (hue < 0.0) hue = 1.0 - hue;
    Float sat = params.FindOneFloat("sat", 1.0);
    Float xtrans = params.FindOneFloat("xtrans", 0.0);
    Float ytrans = params.FindOneFloat("ytrans", 0.0);
    Float ztrans = params.FindOneFloat("ztrans", 0.0);
    Float turb_p = params.FindOneFloat("turb_period", 4096.0 / 5.0);
    Float g_const = params.FindOneFloat("g_const", 1.0);
    Float x_const = params.FindOneFloat("x_const", 4.5);
    Float t_const = params.FindOneFloat("t_const", 7.0);
    Float phase = params.FindOneFloat("phase", 1.0);
    Float power = params.FindOneFloat("power", 4.0);

    Chess_Media* media = new Chess_Media(medBounds,
                                         hue,
                                         sat,
                                         turb_p,
                                         xtrans,
                                         ytrans,
                                         ztrans,
                                         g_const,
                                         x_const,
                                         t_const,
                                         phase,
                                         power);

    return media;
}

}
