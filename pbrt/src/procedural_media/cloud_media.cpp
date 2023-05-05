#include "cloud_media.h"

namespace pbrt
{

static int Cloud_NoiseTableSize = 256;

static int Cloud_NoiseTable[512] = {
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

Float cloud_lerp(Float t, Float one, Float two)
{
    return (1.0 - t) * one + t * two;
}

Float cloud_noiseW(Float t)
{
    Float one = t * t * t;
    Float two = one * t;
    return 6.0 * two * t - 15.0 * two + 10.0 * one;
}

Float cloud_gradient(int x, int y, int z, Float dx, Float dy, Float dz) {
    int val = Cloud_NoiseTable[Cloud_NoiseTable[Cloud_NoiseTable[x] + y] + z];

    val &= 15;

    Float u = val < 8 || val == 12 || val == 13 ? dx : dy;
    Float v = val < 4 || val == 12 || val == 13 ? dy : dz;
    return ((val & 1) ? -u : u) + ((val & 2) ? -v : v);
}

Float cloud_perlin(Float x, Float y, Float z)
{
    int xIndex = floor(x);
    int yIndex = floor(y);
    int zIndex = floor(z);

    Float dx = x - xIndex;
    Float dy = y - yIndex;
    Float dz = z - zIndex;

    xIndex = xIndex & Cloud_NoiseTableSize - 1;
    yIndex = yIndex & Cloud_NoiseTableSize - 1;
    zIndex = zIndex & Cloud_NoiseTableSize - 1;

    int xip1 = xIndex + 1;
    int yip1 = yIndex + 1;
    int zip1 = zIndex + 1;

    Float dxm1 = dx - 1;
    Float dym1 = dy - 1;
    Float dzm1 = dz - 1;

    Float g000 = cloud_gradient(xIndex, yIndex, zIndex, dx, dy, dz);
    Float g100 = cloud_gradient(xip1, yIndex, zIndex, dxm1, dy, dz);
    Float g010 = cloud_gradient(xIndex, yip1, zIndex, dx, dym1, dz);
    Float g110 = cloud_gradient(xip1, yip1, zIndex, dxm1, dym1, dz);
    Float g001 = cloud_gradient(xIndex, yIndex, zip1, dx, dy, dzm1);
    Float g101 = cloud_gradient(xip1, yIndex, zip1, dxm1, dy, dzm1);
    Float g011 = cloud_gradient(xIndex, yip1, zip1, dx, dym1, dzm1);
    Float g111 = cloud_gradient(xip1, yip1, zip1, dxm1, dym1, dzm1);


    Float nwx = cloud_noiseW(dx);
    Float nwy = cloud_noiseW(dy);
    Float nwz = cloud_noiseW(dz);

    // trilinear interpolation
    Float x00 = cloud_lerp(nwx, g000, g100);
    Float x10 = cloud_lerp(nwx, g010, g110);
    Float x01 = cloud_lerp(nwx, g001, g101);
    Float x11 = cloud_lerp(nwx, g011, g111);
    Float y0 = cloud_lerp(nwy, x00, x10);
    Float y1 = cloud_lerp(nwy, x01, x11);

    return cloud_lerp(nwz, y0, y1);
}

// #############################################

Float cloud_noise(Float x, Float y, Float z, Float period)
{
    return cloud_perlin(x / period, y / period, z / period);
}

Float cloud_turbulence(Float x, Float y, Float z, Float period)
{
    Float val = 0.0;
    Float scale = period;
    Float totalScale = 0.0;

    while (scale > 1.0)
    {
        val = val + (cloud_noise(x, y, z, scale)) * scale;

        scale /= 2.0;
    }

    return (0.5 + val * 0.5 / period) * 0.5;
}

Cloud_ProceduralMedia::Cloud_ProceduralMedia(const Bounds3f& medBounds)
    : medBounds(medBounds)
{
    // TODO
}

Spectrum Cloud_ProceduralMedia::Evaluate(const SurfaceInteraction& si) const
{
    std::cout << "Cloud media does not support surface interactions" << std::endl;

    return Spectrum(0.0);
}

Float Cloud_ProceduralMedia::evaluateCloud(const Point3f& pt) const
{
    Float half_res = resolution / 2.0;
    Float thresholdValue = threshStart + threshSlope * (abs(pt.z - half_res)) / resolution;

    if (thresholdValue > 1.0) return 0.0;

    thresholdValue = 0.35;

    // turbulence_xyz according to period
    Float turb = cloud_turbulence(pt.x, pt.y, 0.0, period);
    // remap_range_lin
    turb = (turb - hardCodedMin) / (hardCodedMax - hardCodedMin);
    // calculate euclidean_dist according to resolution
    Float euc_dist = (pt.x - half_res) * (pt.x - half_res);
    euc_dist += (pt.y - half_res) * (pt.y - half_res);
    euc_dist += (pt.z - half_res) * (pt.z - half_res);
    // euc_dist = sqrt(euc_dist);
    // euc_dist /= sqrt(half_res * half_res * 2.0);
    euc_dist /= half_res * half_res * 3.0;
    // calculate the exponential of the dist with specified period
    euc_dist = exp(distPeriod * euc_dist);
    // multiply dist by a factor then clamp
    euc_dist *= distFactor;
    if (euc_dist > 1.0) euc_dist = 1.0;
    // multiply in dist to the image
    // turb += 0.3 * abs(cloud_perlin(pt.x, pt.y, pt.z));
    turb += 0.3 * abs(cloud_perlin(pt.x, pt.y, pt.z));
    turb *= euc_dist;
    // threshold and remove
    if (turb > thresholdValue) turb -= thresholdValue;
    else turb = 0.0;

    // return the media
    // return Spectrum(turb);
    // return turb + 0.1 * abs(cloud_perlin(pt.x, pt.y, pt.z));
    return turb;
}

Spectrum Cloud_ProceduralMedia::Evaluate(const Point3f& p) const
{
    if (p.x < medBounds.pMin.x || p.x > medBounds.pMax.x) return Spectrum(0.0);
    if (p.y < medBounds.pMin.x || p.y > medBounds.pMax.y) return Spectrum(0.0);
    if (p.z < medBounds.pMin.x || p.z > medBounds.pMax.z) return Spectrum(0.0);

    Point3f pt = Point3f((p.x - medBounds.pMin.x) / (medBounds.pMax.x - medBounds.pMin.x) * resolution,
                         (p.y - medBounds.pMin.y) / (medBounds.pMax.y - medBounds.pMin.y) * resolution,
                         (p.z - medBounds.pMin.z) / (medBounds.pMax.z - medBounds.pMin.z) * resolution);

    // return Spectrum(evaluateCloud(pt));

    // std::vector<Float> vals = std::vector<Float>();
    // vals.push_back(evaluateCloud(pt));
    // // vals.push_back(evaluateCloud(Point3f(pt.x, pt.z, pt.y)));
    // vals.push_back(evaluateCloud(Point3f(pt.y, pt.z, pt.x)));
    // // vals.push_back(evaluateCloud(Point3f(pt.y, pt.z, pt.x)));
    // vals.push_back(evaluateCloud(Point3f(pt.z, pt.x, pt.y)));
    // // vals.push_back(evaluateCloud(Point3f(pt.z, pt.y, pt.x)));
    // //
    // Float min = vals[0];
    // //
    // // Float avg = vals[0];
    // //
    // for (int i = 1; i < vals.size(); ++i)
    // {
    // //     avg += vals[i];
    //     if (min > vals[i]) min = vals[i];
    // }
    //
    // // return avg / 6.0;
    //
    // if (min > 1.0) std::cout << "AHAHA" << std::endl;
    // //
    // return min;// + 0.1 * abs(cloud_perlin(pt.x, pt.y, pt.z));

    Float half_res = resolution / 2.0;
    Float thresholdValue = threshStart + threshSlope * (abs(pt.z - half_res)) / resolution;

    if (thresholdValue > 1.0) return 0.0;

    thresholdValue = 0.35;

    Float turbOne = cloud_turbulence(pt.x, pt.y, 0.0, period);
    Float turbTwo = cloud_turbulence(pt.y, pt.z, 0.0, period);
    Float turbThree = cloud_turbulence(pt.z, pt.x, 0.0, period);

    Float turb = (turbOne < turbTwo) ?
                     (turbOne < turbThree) ? turbOne
                                           : turbThree
                     : (turbTwo < turbThree) ? turbTwo
                                             : turbThree;

    // if (turbTwo == turb)
    // {
    //     pt = Point3f(pt.y, pt.z, pt.x);
    // }
    // else if (turbThree == turb)
    // {
    //     pt = Point3f(pt.z, pt.x, pt.y);
    // }

    // turbulence_xyz according to period
    // Float turb = cloud_turbulence(pt.x, pt.y, 0.0, period);
    // remap_range_lin
    turb = (turb - hardCodedMin) / (hardCodedMax - hardCodedMin);
    // calculate euclidean_dist according to resolution
    Float euc_dist = (pt.x - half_res) * (pt.x - half_res);
    euc_dist += (pt.y - half_res) * (pt.y - half_res);
    euc_dist += (pt.z - half_res) * (pt.z - half_res);
    // euc_dist = sqrt(euc_dist);
    // euc_dist /= sqrt(half_res * half_res * 3.0);
    euc_dist /= half_res * half_res * 3.0;
    // calculate the exponential of the dist with specified period
    euc_dist = exp(distPeriod * euc_dist);
    // multiply dist by a factor then clamp
    euc_dist *= distFactor;
    if (euc_dist > 1.0) euc_dist = 1.0;
    // multiply in dist to the image
    turb += 0.3 * abs(cloud_perlin(pt.x, pt.y, pt.z));
    turb *= euc_dist;
    // threshold and remove
    if (turb > thresholdValue) turb -= thresholdValue;
    else turb = 0.0;

    return Spectrum(turb);




    // if (min > 1.0) std::cout << "AHAHA" << std::endl;
    //
    // return min;// + 0.1 * abs(cloud_perlin(pt.x, pt.y, pt.z));

    // return Spectrum(evaluateCloud(pt));
    // return (evaluateCloud(pt) + evaluateCloud(Point3f(pt.z, pt.y, pt.x)) + evaluateCloud(Point3f(pt.z, pt.x, pt.y))) / 3.0;

    // Float thresholdValue = threshStart + threshSlope * (abs(pt.z - half_res)) / resolution;
    //
    // // turbulence_xyz according to period
    // Float turb = cloud_turbulence(pt.x, pt.y, 0.0, period);
    // // remap_range_lin
    // turb = (turb - hardCodedMin) / (hardCodedMax - hardCodedMin);
    // // calculate euclidean_dist according to resolution
    // Float euc_dist = (pt.x - half_res) * (pt.x - half_res);
    // euc_dist += (pt.y - half_res) * (pt.y - half_res);
    // // euc_dist += (p.z * resolution - half_res) * (p.z * resolution - half_res);
    // euc_dist = sqrt(euc_dist);
    // euc_dist /= sqrt(half_res * half_res * 2.0);
    // // calculate the exponential of the dist with specified period
    // euc_dist = exp(distPeriod * euc_dist);
    // // multiply dist by a factor then clamp
    // euc_dist *= distFactor;
    // if (euc_dist > 1.0) euc_dist = 1.0;
    // // multiply in dist to the image
    // turb *= euc_dist;
    // // threshold and remove
    // if (turb > thresholdValue) turb -= thresholdValue;
    // else turb = 0.0;
    //
    // // return the media
    // return Spectrum(turb);
}

Cloud_ProceduralMedia* CreateCloudProceduralMedia(const TextureParams& tp)
{
    ParamSet params = tp.GetMaterialParams();

    // Bounds3f medBounds = Bounds3f(params.FindOnePoint3f("minBounds", Point3f(-1.15, 0.4, 0.0)),
    //                               params.FindOnePoint3f("maxBounds", Point3f(0.85, 2.4, 2.0)));

    // Bounds3f medBounds = Bounds3f(params.FindOnePoint3f("minBounds", Point3f(-1.0, -1.0, 0.0)),
    //                               params.FindOnePoint3f("maxBounds", Point3f(1.0, 1.0, 2.0)));

    Bounds3f medBounds = Bounds3f(params.FindOnePoint3f("minBounds", Point3f(-1.0, -1.0, -1.0)),
                                  params.FindOnePoint3f("maxBounds", Point3f(1.0, 1.0, 1.0)));

    Cloud_ProceduralMedia* proc = new Cloud_ProceduralMedia(medBounds);

    proc->period = params.FindOneFloat("period", 16.0);
    proc->resolution = params.FindOneFloat("resolution", 64.0);
    proc->hardCodedMax = params.FindOneFloat("hardCodedMax", 0.445);
    proc->hardCodedMin = params.FindOneFloat("hardCodedMin", 0.0);
    proc->distPeriod = params.FindOneFloat("distPeriod", -3.0);
    proc->distFactor = params.FindOneFloat("distFactor", 1.7);
    proc->threshStart = params.FindOneFloat("threshStart", 0.15);
    proc->threshSlope = params.FindOneFloat("threshSlope", 0.3);

    return proc;
}

}
