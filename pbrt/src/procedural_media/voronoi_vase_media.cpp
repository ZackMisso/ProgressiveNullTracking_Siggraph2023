#include "voronoi_vase_media.h"

namespace pbrt
{

Voronoi_Vase_ProceduralMedia::Voronoi_Vase_ProceduralMedia(const Bounds3f medBounds)
    : medBounds(medBounds)
{
    points = std::vector<Point3f>();
    colors = std::vector<Spectrum>();

    std::vector<Spectrum> levelColors = std::vector<Spectrum>();
    std::vector<Float> amplitudes = std::vector<Float>();
    std::vector<Float> phases = std::vector<Float>();

    std::cout << "MED BOUNDS: " << medBounds << std::endl;

    levels = 6;
    levelPoints = 36;
    radius = 0.25;
    zPeriod = 10.0;
    // fox
    // zPeriod = 2.0;

    Float rgb[3];
    // rgb[0] = 0.1;
    // rgb[1] = 0.90;
    // rgb[2] = 0.90;

    // octo
    rgb[0] = 0.2;
    rgb[1] = 0.2;
    rgb[2] = 0.2;

    // fox
    // rgb[0] = 0.4;
    // rgb[1] = 0.8;
    // rgb[2] = 0.8;

    levelColors.push_back(RGBSpectrum::FromRGB(rgb));

    // rgb[0] = 0.90;
    // rgb[1] = 0.1;
    // rgb[2] = 0.90;

    // octo
    rgb[0] = (255.0 - 20.0) / 255.0;
    rgb[1] = (255.0 - 20.0) / 255.0;
    rgb[2] = (255.0 - 20.0) / 255.0;

    // fox
    // rgb[0] = 0.2;
    // rgb[1] = 0.2;
    // rgb[2] = 0.2;

    levelColors.push_back(RGBSpectrum::FromRGB(rgb));
    // levelColors.push_back(RGBSpectrum::FromRGB(rgb));

    // rgb[0] = 0.90;
    // rgb[1] = 0.90;
    // rgb[2] = 0.1;

    // rgb[0] = (255.0 - 238.0) / 255.0;
    // rgb[1] = (255.0 - 120.0) / 255.0;
    // rgb[2] = (255.0 - 238.0) / 255.0;

    // octo
    rgb[0] = (255.0 - 200.0) / 255.0;
    rgb[1] = (255.0 - 80.0) / 255.0;
    rgb[2] = (255.0 - 98.0) / 255.0;

    // fox
    // rgb[0] = 0.1;
    // rgb[1] = 0.5;
    // rgb[2] = 0.95;


    // rgb[1] = 0.90;
    // rgb[2] = 0.1;

    // levelColors.push_back(RGBSpectrum::FromRGB(rgb));
    //
    // rgb[0] = 0.1;
    // rgb[1] = 0.90;
    // rgb[2] = 0.1;
    //
    // levelColors.push_back(RGBSpectrum::FromRGB(rgb));
    //
    // rgb[0] = 0.90;
    // rgb[1] = 0.1;
    // rgb[2] = 0.1;
    //
    // levelColors.push_back(RGBSpectrum::FromRGB(rgb));
    //
    // rgb[0] = 0.1;
    // rgb[1] = 0.1;
    // rgb[2] = 0.90;

    levelColors.push_back(RGBSpectrum::FromRGB(rgb));
    // levelColors.push_back(RGBSpectrum::FromRGB(rgb));

    // levelColors.push_back(Spectrum(0.0));

    amplitudes.push_back(0.0);
    amplitudes.push_back(0.06);
    // amplitudes.push_back(0.08);
    // amplitudes.push_back(0.10);
    amplitudes.push_back(0.12);

    //////

    //////

    // fox
    // amplitudes.push_back(0.3);
    // amplitudes.push_back(0.3);
    // amplitudes.push_back(0.3);

    // amplitudes.push_back(0.2);
    // amplitudes.push_back(0.2);
    // amplitudes.push_back(0.15);
    // amplitudes.push_back(0.2);

    // fox
    phases.push_back(0.0);
    phases.push_back(6.28 * 2.0 / 6.0);
    phases.push_back(6.28 * 4.0 / 6.0);

    // phases.push_back(0.0);
    // phases.push_back(6.28 * 0.0 / 6.0);
    // phases.push_back(6.28 * 0.0 / 6.0);
    // phases.push_back(6.28 * 3.0 / 6.0);
    // phases.push_back(6.28 * 3.0 / 6.0);

    // phases.push_back(0.0);
    // phases.push_back(0.0);
    // phases.push_back(0.0);
    // phases.push_back(0.0);
    // phases.push_back(6.28 * 3.0 / 6.0);
    // phases.push_back(6.28 * 4.0 / 6.0);
    // phases.push_back(6.28 * 5.0 / 6.0);

    for (int i = 0; i < levelPoints; ++i)
    {
        Float t = Float(i) / Float(levelPoints) * 6.28;
        Float z = 0.0;

        for (int j = 0; j < phases.size(); ++j)
        {
            Float x = 0.5 + radius * cos(t + phases[j]);
            Float y = 0.5 + radius * sin(t + phases[j]);
            // z += amplitudes[j];
            // std::cout << z << std::endl;
            // if (j == 1) z += abs(amplitudes[j] * cos(2.3 * zPeriod * t + phases[j]));
            // else z += abs(amplitudes[j] * cos(zPeriod * t + phases[j]));

            z += abs(amplitudes[j] * cos(zPeriod * t + phases[j]));

            // std::cout << "i: " << i << std::endl;
            // std::cout << "j: " << j << std::endl;
            // std::cout << "z: " << z << std::endl;
            // std::cout << std::endl;

            points.push_back(Point3f(x * (medBounds.pMax.x - medBounds.pMin.x) + medBounds.pMin.x,
                                     y * (medBounds.pMax.y - medBounds.pMin.y) + medBounds.pMin.y,
                                     z * (medBounds.pMax.z - medBounds.pMin.z) + medBounds.pMin.z));

            colors.push_back(levelColors[j]);
        }
    }

    maxLen = medBounds.Diagonal()[medBounds.MaximumExtent()];
    std::cout << "maxLen: " << maxLen << std::endl;
    std::cout << "period: " << 30.0 / maxLen << std::endl;
    //
    // points.push_back(Point3f(0.25, 0.525, 0.5825));
    // points.push_back(Point3f(0.4, 0.7, 0.3375));
    // points.push_back(Point3f(0.4, 0.7, 0.6725));
    // points.push_back(Point3f(0.6, 0.575, 0.475));
    // points.push_back(Point3f(0.475, 0.375, 0.535));
    // points.push_back(Point3f(0.725, 0.5, 0.375));
    // points.push_back(Point3f(0.6, 0.65, 0.7));
    // points.push_back(Point3f(0.7, 0.3, 0.425));

    // points.push_back(Point3f(-100.0, 10.0, 33.0));
    // points.push_back(Point3f(-40.0, 80.0, -65.0));
    // points.push_back(Point3f(-40.0, 80.0, 69.0));
    // points.push_back(Point3f(40.0, 30.0, -10.0));
    // points.push_back(Point3f(-10.0, -50.0, 14.0));
    // points.push_back(Point3f(90.0, 0.0, -50.0));
    // points.push_back(Point3f(40.0, 60.0, 80.0));
    // points.push_back(Point3f(80.0, -80.0, -30.0));

    // for (int i = 0; i < points.size(); ++i)
    // {
    //     points[i].x = points[i].x * (medBounds.pMax.x - medBounds.pMin.x) + medBounds.pMin.x;
    //     points[i].y = points[i].y * (medBounds.pMax.y - medBounds.pMin.y) + medBounds.pMin.y;
    //     points[i].z = points[i].z * (medBounds.pMax.z - medBounds.pMin.z) + medBounds.pMin.z;
    // }
    //
    // Float rgb[3];
    // rgb[0] = 0.1;
    // rgb[1] = 0.90;
    // rgb[2] = 0.90;
    // colors.push_back(RGBSpectrum::FromRGB(rgb));
    // // rgb[0] = 0.0;
    // // rgb[1] = 1.0;
    // // rgb[2] = 0.0;
    // colors.push_back(RGBSpectrum::FromRGB(rgb));
    // // rgb[0] = 0.0;
    // // rgb[1] = 0.0;
    // // rgb[2] = 1.0;
    // colors.push_back(RGBSpectrum::FromRGB(rgb));
    // // rgb[0] = 0.5;
    // // rgb[1] = 0.5;
    // // rgb[2] = 0.5;
    // colors.push_back(RGBSpectrum::FromRGB(rgb));
    // rgb[0] = 0.90;
    // rgb[1] = 0.90;
    // rgb[2] = 0.10;
    // colors.push_back(RGBSpectrum::FromRGB(rgb));
    // // rgb[0] = 1.0;
    // // rgb[1] = 0.0;
    // // rgb[2] = 1.0;
    // colors.push_back(RGBSpectrum::FromRGB(rgb));
    // // rgb[0] = 0.0;
    // // rgb[1] = 1.0;
    // // rgb[2] = 1.0;
    // colors.push_back(RGBSpectrum::FromRGB(rgb));
    // // rgb[0] = 1.0;
    // // rgb[1] = 1.0;
    // // rgb[2] = 1.0;
    // colors.push_back(RGBSpectrum::FromRGB(rgb));
}

Spectrum Voronoi_Vase_ProceduralMedia::Evaluate(const SurfaceInteraction& si) const
{
    std::cout << "ERROR: MAN_VOR_PROC_MEDIA DOES NOT SUPPORT SURFACE INTERACTIONS" << std::endl;

    return Spectrum(1.0);
}

Spectrum Voronoi_Vase_ProceduralMedia::Evaluate(const Point3f& p) const
{
    // std::cout << "here" << std::endl;
    if (p.x > medBounds.pMax.x || p.x < medBounds.pMin.x)
    {
        // std::cout << "x: " << p.x << std::endl;
        return Spectrum(1.0);

        // std::cout << "Out of X" << std::endl;
    }
    if (p.y > medBounds.pMax.y || p.y < medBounds.pMin.y)
    {
        // std::cout << "y: " << p.y << std::endl;
        return Spectrum(1.0);

    }
    if (p.z > medBounds.pMax.z || p.z < medBounds.pMin.z)
    {
        // std::cout << "z: " << p.z << std::endl;
        return Spectrum(1.0);
    }
    // if (p.y > medBounds.pMax.y || p.y < medBounds.pMin.y) std::cout << "Out of y" << std::endl;
    // if (p.z > medBounds.pMax.z || p.z < medBounds.pMin.z) std::cout << "Out of z" << std::endl;

    int index = 0;
    Float dist = (p - points[0]).absSum();

    for (int i = 1; i < points.size(); ++i)
    {
        Float tmpDist = (p - points[i]).absSum();
        if (tmpDist < dist)
        {
            dist = tmpDist;
            index = i;
        }
    }

    Float period = 30.0 / maxLen;

    // return colors[index];

    Float noise[3];
    noise[0] = 0.1 * sin(period * p.x);
    noise[1] = 0.1 * cos(period * p.y);
    noise[2] = 0.1 * sin(period * p.z);

    Spectrum ret = RGBSpectrum::FromRGB(noise) + colors[index];

    return ret;
}

Voronoi_Vase_ProceduralMedia* CreateVoronoiVaseProceduralMedia(const Transform& medToWorld,
                                                               const Bounds3f medBounds)
{
    return new Voronoi_Vase_ProceduralMedia(medBounds);
}

// TODO: create a create method with custom points and colors

}
