#include "majorant.h"

namespace pbrt {

IntervalAdaptiveDensityMajorant::IntervalAdaptiveDensityMajorant(
    const DensityFunction* density, int dim)
    : MajorantFunction(density), dim(dim) {
    initialize(density);
    delta = 0.1;   // for sphere medium
    delta = 0.05;  // for disk medium
    // delta = 1.0;
}

IntervalAdaptiveDensityMajorant::~IntervalAdaptiveDensityMajorant() {
    delete[] majGrid;
}

std::vector<MacroGridQuery>
IntervalAdaptiveDensityMajorant::MacroVoxelTraversal(const Point3f& pa,
                                                     const Point3f& pb,
                                                     Float baseTMin,
                                                     Float baseTMax) const {
    std::vector<MacroGridQuery> queries = std::vector<MacroGridQuery>();

    // std::cout << "START" << std::endl;

    Float dx = 1.0 / Float(dim);
    Float dy = 1.0 / Float(dim);
    Float dz = 1.0 / Float(dim);

    int a_ix = std::floor((pa[0] - (-0.5)) / dx);
    int a_iy = std::floor((pa[1] - (-0.5)) / dy);
    int a_iz = std::floor((pa[2] - (-0.5)) / dz);

    int b_ix = std::floor((pb[0] - (-0.5)) / dx);
    int b_iy = std::floor((pb[1] - (-0.5)) / dy);
    int b_iz = std::floor((pb[2] - (-0.5)) / dz);

    // Float a_vx = (pa[0] - (-0.5)) / dx;
    // Float a_vy = (pa[1] - (-0.5)) / dy;
    // Float a_vz = (pa[2] - (-0.5)) / dz;

    // Float b_vx = (pb[0] - (-0.5)) / dx;
    // Float b_vy = (pb[1] - (-0.5)) / dy;
    // Float b_vz = (pb[2] - (-0.5)) / dz;

    Float a_vx = (pa[0] - (-0.5)) / dx;
    Float a_vy = (pa[1] - (-0.5)) / dy;
    Float a_vz = (pa[2] - (-0.5)) / dz;

    Float b_vx = (pb[0] - (-0.5)) / dx;
    Float b_vy = (pb[1] - (-0.5)) / dy;
    Float b_vz = (pb[2] - (-0.5)) / dz;

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

    // std::cout << "START" << std::endl;

    // std::cout << "A: " << pa << std::endl;
    // std::cout << "B: " << pb << std::endl;

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

        // std::cout << "A: " << a << std::endl;
        // std::cout << "B: " << b << std::endl;
        // std::cout << "C: " << c << std::endl;

        Float majorant = Maj(c);

        int x = floor((c[0] - (-0.5)) * Float(dim));
        int y = floor((c[1] - (-0.5)) * Float(dim));
        int z = floor((c[2] - (-0.5)) * Float(dim));

        queries.push_back(
            MacroGridQuery(a, b, Point3i(x, y, z), majorant,
                           t_vals[pos] * (baseTMax - baseTMin) + baseTMin,
                           t_vals[pos + 1] * (baseTMax - baseTMin) + baseTMin));
    }

    // std::cout << "END" << std::endl;

    return queries;
}

std::vector<MacroGridQuery>
IntervalAdaptiveDensityMajorant::MacroVoxelTraversal_DEBUG(
    const Point3f& pa, const Point3f& pb, Float baseTMin,
    Float baseTMax) const {
    std::vector<MacroGridQuery> queries = std::vector<MacroGridQuery>();

    // std::cout << "START" << std::endl;

    Float dx = 1.0 / Float(dim);
    Float dy = 1.0 / Float(dim);
    Float dz = 1.0 / Float(dim);

    int a_ix = std::floor((pa[0] - (-0.5)) / dx);
    int a_iy = std::floor((pa[1] - (-0.5)) / dy);
    int a_iz = std::floor((pa[2] - (-0.5)) / dz);

    int b_ix = std::floor((pb[0] - (-0.5)) / dx);
    int b_iy = std::floor((pb[1] - (-0.5)) / dy);
    int b_iz = std::floor((pb[2] - (-0.5)) / dz);

    std::cout << "Ax: " << a_ix << std::endl;
    std::cout << "Ay: " << a_iy << std::endl;
    std::cout << "Az: " << a_iz << std::endl;

    std::cout << "Bx: " << b_ix << std::endl;
    std::cout << "By: " << b_iy << std::endl;
    std::cout << "Bz: " << b_iz << std::endl;

    // Float a_vx = (pa[0] - (-0.5)) / dx;
    // Float a_vy = (pa[1] - (-0.5)) / dy;
    // Float a_vz = (pa[2] - (-0.5)) / dz;

    // Float b_vx = (pb[0] - (-0.5)) / dx;
    // Float b_vy = (pb[1] - (-0.5)) / dy;
    // Float b_vz = (pb[2] - (-0.5)) / dz;

    Float a_vx = (pa[0] - (-0.5)) / dx;
    Float a_vy = (pa[1] - (-0.5)) / dy;
    Float a_vz = (pa[2] - (-0.5)) / dz;

    Float b_vx = (pb[0] - (-0.5)) / dx;
    Float b_vy = (pb[1] - (-0.5)) / dy;
    Float b_vz = (pb[2] - (-0.5)) / dz;

    std::cout << "Avx: " << a_vx << std::endl;
    std::cout << "Avy: " << a_vy << std::endl;
    std::cout << "Avz: " << a_vz << std::endl;

    std::cout << "Bvx: " << b_vx << std::endl;
    std::cout << "Bvy: " << b_vy << std::endl;
    std::cout << "Bvz: " << b_vz << std::endl;

    std::vector<Float> t_vals = std::vector<Float>();

    t_vals.push_back(0.0);

    std::cout << "starting x traversal: " << std::endl;
    std::cout << "aix; " << a_ix << std::endl;
    std::cout << "bix; " << b_ix << std::endl;
    while (a_ix != b_ix) {
        std::cout << "aix before: " << a_ix << std::endl;
        std::cout << "aix after: " << a_ix << std::endl;
        Float index = Float(a_ix);
        Float t = (index - a_vx) / (b_vx - a_vx);
        a_ix = a_ix + (b_ix - a_ix) / (std::abs(b_ix - a_ix));
        std::cout << "t: " << t << std::endl;
        std::cout << "PT: " << (pb[0] - pa[0]) * t + pa[0] << std::endl;

        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    std::cout << "starting y traversal: " << std::endl;
    std::cout << "aiy; " << a_iy << std::endl;
    std::cout << "biy; " << b_iy << std::endl;
    while (a_iy != b_iy) {
        std::cout << "aiy before: " << a_iy << std::endl;
        std::cout << "aiy after: " << a_iy << std::endl;
        Float index = Float(a_iy);
        Float t = (index - a_vy) / (b_vy - a_vy);
        a_iy = a_iy + (b_iy - a_iy) / (std::abs(b_iy - a_iy));
        std::cout << "t: " << t << std::endl;
        std::cout << "PT: " << (pb[1] - pa[1]) * t + pa[1] << std::endl;

        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    std::cout << "starting z traversal: " << std::endl;
    while (a_iz != b_iz) {
        Float index = Float(a_iz);
        Float t = (index - a_vz) / (b_vz - a_vz);
        a_iz = a_iz + (b_iz - a_iz) / (std::abs(b_iz - a_iz));
        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    t_vals.push_back(1.0);

    // I wish i did not have to sort but oh well... improve this later
    std::sort(t_vals.begin(), t_vals.end());

    // std::cout << "START" << std::endl;

    // std::cout << "A: " << pa << std::endl;
    // std::cout << "B: " << pb << std::endl;

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

        std::cout << "pos: " << pos << std::endl;
        std::cout << "t[pos]: " << t_vals[pos] << std::endl;
        std::cout << "t[pos+1]: " << t_vals[pos + 1] << std::endl;
        debug(a);
        debug(b);
        debug(c);
        std::cout << "Full a: " << a << std::endl;
        std::cout << "Full b: " << b << std::endl;
        std::cout << "Full c: " << c << std::endl;

        // std::cout << "A: " << a << std::endl;
        // std::cout << "B: " << b << std::endl;
        // std::cout << "C: " << c << std::endl;

        Float majorant = Maj(c);

        int x = floor((c[0] - (-0.5)) * Float(dim));
        int y = floor((c[1] - (-0.5)) * Float(dim));
        int z = floor((c[2] - (-0.5)) * Float(dim));

        queries.push_back(
            MacroGridQuery(a, b, Point3i(x, y, z), majorant,
                           t_vals[pos] * (baseTMax - baseTMin) + baseTMin,
                           t_vals[pos + 1] * (baseTMax - baseTMin) + baseTMin));
    }

    // std::cout << "END" << std::endl;

    return queries;
}

// extremely inefficient
Float IntervalAdaptiveDensityMajorant::Maj(const Point3f& p) const {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    // std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

    return getMajVoxel(x, y, z);
}

void IntervalAdaptiveDensityMajorant::debug(Point3f p) const {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

    // return getMajVoxel(x, y, z);
}

Float IntervalAdaptiveDensityMajorant::Maj(const Point3f& pa,
                                           const Point3f& pb) const {
    std::cout << "THIS SHOULD NOT BE CALLED!" << std::endl;
    return 1.0;
}

void IntervalAdaptiveDensityMajorant::setMajVoxel(int x, int y, int z,
                                                  Float maj) {
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

Float IntervalAdaptiveDensityMajorant::getMajVoxel(int x, int y, int z) const {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;
    // if (x >= 0 && x < dim && y >= 0 && y < dim && z >= 0 && z < dim)
    return majGrid[z * dim * dim + y * dim + x];
}

void IntervalAdaptiveDensityMajorant::initialize(const DensityFunction* dense) {
    majGrid = new Float[dim * dim * dim];

    if (dense == nullptr) return;  // debug logic

    // hack for visualization purposes
    // dense->startInitialBuildState();

    // std::cout << "starting preprocess" << std::endl;

    for (int x = 0; x < dim; ++x) {
        for (int y = 0; y < dim; ++y) {
            for (int z = 0; z < dim; ++z) {
                Float min_x = -0.5;
                Float max_x = 0.5;
                Float min_y = -0.5;
                Float max_y = 0.5;
                Float min_z = -0.5;
                Float max_z = 0.5;

                Float dense_val = 0.0;

                for (int j = 0; j < 5; ++j) {
                    for (int i = 0; i < 5; ++i) {
                        for (int k = 0; k < 5; ++k) {
                            Float jj = 0.1 + j * 0.2;
                            Float ii = 0.1 + i * 0.2;
                            Float kk = 0.1 + k * 0.2;

                            // std::cout << (Float(x) + jj) / Float(dim) *
                            //                              (max_x - min_x) +
                            //                          min_x << std::endl;

                            dense_val +=
                                dense->D(Point3f((Float(x) + jj) / Float(dim) *
                                                         (max_x - min_x) +
                                                     min_x,
                                                 (Float(y) + ii) / Float(dim) *
                                                         (max_y - min_y) +
                                                     min_y,
                                                 (Float(z) + kk) / Float(dim) *
                                                         (max_z - min_z) +
                                                     min_z),
                                         true)[0];
                        }
                    }
                }
                // Float dense_val = dense->D(
                //     Point3f(
                //         (Float(x) + 0.5) / Float(dim) * (max_x - min_x) +
                //         min_x, (Float(y) + 0.5) / Float(dim) * (max_y -
                //         min_y) + min_y, (Float(z) + 0.5) / Float(dim) *
                //         (max_z - min_z) +
                //             min_z),
                //     true)[0];
                // Float dense_val = dense->D(
                //     Point3f(
                //         (Float(x) + 0.5) / Float(dim) * (max_x - min_x) +
                //         min_x, (Float(y) + 0.5) / Float(dim) * (max_y -
                //         min_y) + min_y, (Float(z) + 0.5) / Float(dim) *
                //         (max_z - min_z) +
                //             min_z),
                //     true)[0];

                if (dense_val > 0.0) {
                    // std::cout << "x: " << x << " y: " << y << " z: " << z
                    //           << std::endl;
                    setMajVoxel(x, y, z, 1.0);
                    // setMajVoxel(x, y, z, 3.0);

                    // setMajVoxel(x + 1, y, z, 1.0);
                    // setMajVoxel(x - 1, y, z, 1.0);
                    // setMajVoxel(x, y + 1, z, 1.0);
                    // setMajVoxel(x, y - 1, z, 1.0);
                    // setMajVoxel(x, y, z + 1, 1.0);
                    // setMajVoxel(x, y, z - 1, 1.0);
                }
                // setMajVoxel(x, y, z, 1.0);
                // setMajVoxel(x, y, z, 1.0);
                else
                    setMajVoxel(x, y, z, 0.01);
            }
        }
    }

    std::cout << "finished preprocess" << std::endl;

    // hack for visualization purposes
    // dense->endInitialBuildState();
}

void IntervalAdaptiveDensityMajorant::UpdateMajorant(const Point3f& p,
                                                     Float density) {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    std::cout << "Updating Majorant: " << density << std::endl;

    // std::cout << "x: " << x << " y: " << y << " z: " << z
    //           << " density: " << density << std::endl;

    // uncomment for truly adaptive behavior
    Float maxVal = getMajVoxel(x, y, z);
    std::cout << "MaxVal: " << maxVal << std::endl;
    if (density > maxVal) setMajVoxel(x, y, z, density + delta);
}

}  // namespace pbrt
