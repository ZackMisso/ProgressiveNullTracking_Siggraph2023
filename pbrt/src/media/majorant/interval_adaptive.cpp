#include "majorant.h"

namespace pbrt {

IntervalAdaptiveMajorant::IntervalAdaptiveMajorant(
    const DensityFunction* density, int dim)
    : MajorantFunction(density), dim(dim) {
    initialize(density);
    delta = 0.2;
}

IntervalAdaptiveMajorant::~IntervalAdaptiveMajorant() { delete[] majGrid; }

std::vector<MacroGridQuery> IntervalAdaptiveMajorant::MacroVoxelTraversal(
    const Point3f& pa, const Point3f& pb, Float baseTMin,
    Float baseTMax) const {
    std::vector<MacroGridQuery> queries = std::vector<MacroGridQuery>();

    Float dx = 1.0 / Float(dim);
    Float dy = 1.0 / Float(dim);
    Float dz = 1.0 / Float(dim);

    int a_ix = floor((pa[0] - (-0.5)) / dx);
    int a_iy = floor((pa[2] - (-0.5)) / dy);
    int a_iz = floor((pa[1] - (-0.5)) / dz);

    int b_ix = floor((pb[0] - (-0.5)) / dx);
    int b_iy = floor((pb[2] - (-0.5)) / dy);
    int b_iz = floor((pb[1] - (-0.5)) / dz);

    Float a_vx = (pa[0] - (-0.5)) / dx;
    Float a_vy = (pa[2] - (-0.5)) / dy;
    Float a_vz = (pa[1] - (-0.5)) / dz;

    Float b_vx = (pb[0] - (-0.5)) / dx;
    Float b_vy = (pb[2] - (-0.5)) / dy;
    Float b_vz = (pb[1] - (-0.5)) / dz;

    std::vector<Float> t_vals = std::vector<Float>();

    t_vals.push_back(0.0);

    int cnt = 0;
    while (a_ix != b_ix) {
        if (cnt != 0) a_ix = a_ix + (b_ix - a_ix) / (std::abs(b_ix - a_ix));
        Float index = Float(a_ix);
        Float t = (index - a_vx) / (b_vx - a_vx);
        cnt++;

        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    cnt = 0;
    while (a_iy != b_iy) {
        if (cnt != 0) a_iy = a_iy + (b_iy - a_iy) / (std::abs(b_iy - a_iy));
        Float index = Float(a_iy);
        Float t = (index - a_vy) / (b_vy - a_vy);
        cnt++;

        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    cnt = 0;
    while (a_iz != b_iz) {
        if (cnt != 0) a_iz = a_iz + (b_iz - a_iz) / (std::abs(b_iz - a_iz));
        Float index = Float(a_iz);
        Float t = (index - a_vz) / (b_vz - a_vz);
        cnt++;

        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    t_vals.push_back(1.0);

    // I wish i did not have to sort but oh well... improve this later
    std::sort(t_vals.begin(), t_vals.end());

    for (int pos = 0; pos < t_vals.size() - 1; ++pos) {
        if (t_vals[pos] >= 1.0) continue;
        if (t_vals[pos] == t_vals[pos + 1]) continue;

        Point3f a = Point3f((pb[0] - pa[0]) * t_vals[pos] + pa[0],
                            (pb[1] - pa[1]) * t_vals[pos] + pa[1],
                            (pb[2] - pa[2]) * t_vals[pos] + pa[2]);

        Point3f b = Point3f((pb[0] - pa[0]) * t_vals[pos + 1] + pa[0],
                            (pb[1] - pa[1]) * t_vals[pos + 1] + pa[1],
                            (pb[2] - pa[2]) * t_vals[pos + 1] + pa[2]);

        Point3f c = (a + b) / 2.0;

        Float majorant = Maj(c);

        queries.push_back(MacroGridQuery(
            a, b, majorant, t_vals[pos] * (baseTMax - baseTMin) + baseTMin,
            t_vals[pos + 1] * (baseTMax - baseTMin) + baseTMin));
    }

    return queries;
}

// extremely inefficient
Float IntervalAdaptiveMajorant::Maj(const Point3f& p) const {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    // if (!(x >= 0 && x < dim && y >= 0 && y < dim && z >= 0 && z < dim))
    //     std::cout << "out of bounds: x: " << x << " y: " << p[1] << " z: " <<
    //     z
    //               << std::endl;

    return getMajVoxel(x, y, z);
}

Float IntervalAdaptiveMajorant::Maj(const Point3f& pa,
                                    const Point3f& pb) const {
    // std::cout << "THIS SHOULD NOT BE CALLED!" << std::endl;
    return 1.0;
}

void IntervalAdaptiveMajorant::setMajVoxel(int x, int y, int z, Float maj) {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;
    std::cout << "OLD: " << majGrid[z * dim * dim + y * dim + x]
              << " NEW: " << maj << std::endl;
    // if (x >= 0 && x < dim && y >= 0 && y < dim && z >= 0 && z < dim)
    majGrid[z * dim * dim + y * dim + x] = maj;
}

Float IntervalAdaptiveMajorant::getMajVoxel(int x, int y, int z) const {
    // return 3.0;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;
    // if (x >= 0 && x < dim && y >= 0 && y < dim && z >= 0 && z < dim)
    return majGrid[z * dim * dim + y * dim + x];
    // std::cout << "out of bounds: x: " << x << " y: " << y << " z: " << z
    //           << std::endl;
    // return 3.0;
}

void IntervalAdaptiveMajorant::initialize(const DensityFunction* dense) {
    majGrid = new Float[dim * dim * dim];

    for (int x = 0; x < dim; ++x) {
        for (int y = 0; y < dim; ++y) {
            for (int z = 0; z < dim; ++z) {
                setMajVoxel(x, y, z, 0.01);
                // setMajVoxel(x, y, z, 3.0);
            }
        }
    }
}

void IntervalAdaptiveMajorant::UpdateMajorant(const Point3f& p, Float density) {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    // if (!(x >= 0 && x < dim && y >= 0 && y < dim && z >= 0 && z < dim))
    //     std::cout << "out of bounds: x: " << x << " y: " << p[1] << " z: " <<
    //     z
    //               << std::endl;

    // uncomment for truly adaptive behavior
    Float maxVal = getMajVoxel(x, y, z);
    if (density > maxVal) setMajVoxel(x, y, z, density + delta);
}

}  // namespace pbrt
