#include "voronoi_spiral_media.h"

namespace pbrt
{

Voronoi_Spiral_ProceduralMedia::Voronoi_Spiral_ProceduralMedia(const Bounds3f& medBounds)
    : medBounds(medBounds)
{
    // does nothing
}

Spectrum Voronoi_Spiral_ProceduralMedia::Evaluate(const SurfaceInteraction& si) const
{
    std::cout << "ERROR: MAN_VOR_PROC_MEDIA DOES NOT SUPPORT SURFACE INTERACTIONS" << std::endl;

    return Spectrum(1.0);
}

static int Spiral_NoiseTableSize = 256;

static int Spiral_NoiseTable[512] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
    36, 103, 30, 69, 142,
    // Remainder of the noise permutation table
    8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62,
    94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
    20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77,
    146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55,
    46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76,
    132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100,
    109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147,
    118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28,
    42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101,
    155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232,
    178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12,
    191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31,
    181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66,
    215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194,
    233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6,
    148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
    57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
    165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60,
    211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
    63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135,
    130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
    250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
    227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2,
    44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
    98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251,
    34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249,
    14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115,
    121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72,
    243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

Float spiral_lerp(Float t, Float one, Float two)
{
    return (1.0 - t) * one + t * two;
}

Float spiral_noiseW(Float t)
{
    Float one = t * t * t;
    Float two = one * t;
    return 6.0 * two * t - 15.0 * two + 10.0 * one;
}

Float spiral_gradient(int x, int y, int z, Float dx, Float dy, Float dz) {
    int val = Spiral_NoiseTable[Spiral_NoiseTable[Spiral_NoiseTable[x] + y] + z];

    val &= 15;

    Float u = val < 8 || val == 12 || val == 13 ? dx : dy;
    Float v = val < 4 || val == 12 || val == 13 ? dy : dz;
    return ((val & 1) ? -u : u) + ((val & 2) ? -v : v);
}

Float spiral_perlin(Float x, Float y, Float z)
{
    int xIndex = floor(x);
    int yIndex = floor(y);
    int zIndex = floor(z);

    Float dx = x - xIndex;
    Float dy = y - yIndex;
    Float dz = z - zIndex;

    xIndex = xIndex & Spiral_NoiseTableSize - 1;
    yIndex = yIndex & Spiral_NoiseTableSize - 1;
    zIndex = zIndex & Spiral_NoiseTableSize - 1;

    int xip1 = xIndex + 1;
    int yip1 = yIndex + 1;
    int zip1 = zIndex + 1;

    Float dxm1 = dx - 1;
    Float dym1 = dy - 1;
    Float dzm1 = dz - 1;

    Float g000 = spiral_gradient(xIndex, yIndex, zIndex, dx, dy, dz);
    Float g100 = spiral_gradient(xip1, yIndex, zIndex, dxm1, dy, dz);
    Float g010 = spiral_gradient(xIndex, yip1, zIndex, dx, dym1, dz);
    Float g110 = spiral_gradient(xip1, yip1, zIndex, dxm1, dym1, dz);
    Float g001 = spiral_gradient(xIndex, yIndex, zip1, dx, dy, dzm1);
    Float g101 = spiral_gradient(xip1, yIndex, zip1, dxm1, dy, dzm1);
    Float g011 = spiral_gradient(xIndex, yip1, zip1, dx, dym1, dzm1);
    Float g111 = spiral_gradient(xip1, yip1, zip1, dxm1, dym1, dzm1);

    Float nwx = spiral_noiseW(dx);
    Float nwy = spiral_noiseW(dy);
    Float nwz = spiral_noiseW(dz);

    // trilinear interpolation
    Float x00 = spiral_lerp(nwx, g000, g100);
    Float x10 = spiral_lerp(nwx, g010, g110);
    Float x01 = spiral_lerp(nwx, g001, g101);
    Float x11 = spiral_lerp(nwx, g011, g111);
    Float y0 = spiral_lerp(nwy, x00, x10);
    Float y1 = spiral_lerp(nwy, x01, x11);

    return spiral_lerp(nwz, y0, y1);
}

// #############################################

Float spiral_noise(Float x, Float y, Float z, Float period)
{
    return spiral_perlin(x / period, y / period, z / period);
}

Float spiral_turbulence(Float x, Float y, Float z, Float period)
{
    Float val = 0.0;
    Float scale = period;
    Float totalScale = 0.0;

    while (scale > 1.0)
    {
        val = val + (spiral_noise(x, y, z, scale)) * scale;

        scale /= 2.0;
    }

    return (0.5 + val * 0.5 / period) * 0.5;
}

Spectrum Voronoi_Spiral_ProceduralMedia::Evaluate(const Point3f& p) const
{
    if (!Inside(p, medBounds))
    {
        return 1.0;
    }

    // Old Way - no interpolation
    // int index = 0;
    // Float dist = (p - points[0]).absSum();
    //
    // for (int i = 1; i < points.size(); ++i)
    // {
    //     Float tmpDist = (p - points[i]).absSum();
    //     if (tmpDist < dist)
    //     {
    //         dist = tmpDist;
    //         index = i;
    //     }
    // }

    // two vertex interpolation
    int indexOne = 0;
    int indexTwo = points.size() - 1;
    Float distOne = (p - points[0]).absSum();
    Float distTwo = (p - points[points.size() - 1]).absSum();

    if (distOne < distTwo)
    {
        Float tmp = distTwo;
        int tmpInd = indexTwo;
        indexTwo = indexOne;
        distTwo = distOne;
        indexOne = tmpInd;
        distOne = tmp;
    }

    for (int i = 1; i < points.size() - 1; ++i)
    {
        Float tmpDist = (p - points[i]).absSum();
        int tmpIndex = i;
        if (tmpDist < distOne)
        {
            Float tmp = distOne;
            distOne = tmpDist;
            tmpDist = tmp;
            tmpIndex = indexOne;
            indexOne = i;
        }
        if (tmpDist < distTwo)
        {
            distTwo = tmpDist;
            indexTwo = i;
        }
    }

    Float t = distOne / (distOne + distTwo);

    Float noise[3];
    noise[0] = noiseAmp[0] * sin(noisePeriod[0] * p.x);
    noise[1] = noiseAmp[1] * cos(noisePeriod[1] * p.y);
    noise[2] = noiseAmp[2] * sin(noisePeriod[2] * p.z);

    return colors[indexOne] * (1.0 - t) + colors[indexTwo] * t + RGBSpectrum::FromRGB(noise);

    //
    // noise[0] = 0.0;
    // noise[1] = 0.0;
    // noise[2] = 0.0;
    // // noise[0] = noiseAmp[0] * sin(noisePeriod[0] * p.x);
    // // noise[1] = noiseAmp[1] * cos(noisePeriod[1] * p.y);
    // // noise[2] = noiseAmp[2] * sin(noisePeriod[2] * p.z);
    //
    // // noise[0] = noiseAmp[0] * fox_noise(p.x, 0.0, 0.0, 20.0);
    // // noise[1] = noiseAmp[1] * fox_noise(0.0, p.y, 0.0, 20.0);
    // // noise[2] = noiseAmp[2] * fox_noise(0.0, 0.0, p.z, 20.0);
    //
    // Spectrum ret = RGBSpectrum::FromRGB(noise) + colors[index];
    //
    // return ret;
}

Voronoi_Spiral_ProceduralMedia* CreateVoronoiSpiralProceduralMedia(const TextureParams &tp)
{
    ParamSet params = tp.GetMaterialParams();

    Bounds3f medBounds = Bounds3f(params.FindOnePoint3f("minBounds", Point3f(0.0, 0.0, 0.0)),
                                  params.FindOnePoint3f("maxBounds", Point3f(1.0, 1.0, 1.0)));

    Voronoi_Spiral_ProceduralMedia* proc = new Voronoi_Spiral_ProceduralMedia(medBounds);

    proc->points = std::vector<Point3f>();
    proc->colors = std::vector<Spectrum>();

    int numLCol = 0;
    const Spectrum* levelColors = params.FindSpectrum("levelColors", &numLCol);

    int numAm = 0;
    const Float* amplitudes = params.FindFloat("amplitudes", &numAm);

    int numPh = 0;
    const Float* phases = params.FindFloat("phases", &numPh);

    int numPer;
    const Float* zPer = params.FindFloat("zPeriod", &numPer);

    if (numAm == numPh && numPer == numAm)
    {
        proc->levelPoints = params.FindOneInt("levelPoints", 36);
        proc->radius = params.FindOneFloat("radius", 0.25);

        for (int i = 0; i < proc->levelPoints; ++i)
        {
            Float t = Float(i) / Float(proc->levelPoints) * 6.28;
            Float z = 0.0;

            for (int j = 0; j < numPh; ++j)
            {
                Float x = 0.5 + proc->radius * cos(t + phases[j]);
                Float y = 0.5 + proc->radius * sin(t + phases[j]);

                z += abs(amplitudes[j] * cos(zPer[j] * t + phases[j]));

                proc->points.push_back(Point3f(x * (medBounds.pMax.x - medBounds.pMin.x) + medBounds.pMin.x,
                                         y * (medBounds.pMax.y - medBounds.pMin.y) + medBounds.pMin.y,
                                         z * (medBounds.pMax.z - medBounds.pMin.z) + medBounds.pMin.z));

                proc->colors.push_back(levelColors[j]);
            }
        }

        proc->noisePeriod = params.FindOneSpectrum("noisePeriod", Spectrum(0.0));
        proc->noiseAmp = params.FindOneSpectrum("noiseAmp", Spectrum(0.1));

        return proc;
    }

    return nullptr;
}

}
