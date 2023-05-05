#include <exception>

#include "majorant.h"

namespace pbrt {

IntervalAdaptiveDensityVDBHardcode::IntervalAdaptiveDensityVDBHardcode(
    const DensityFunction* density, int dim)
    : MajorantFunction(density), dim(dim) {
    // delta = 0.1;   // for sphere medium
    // delta = 0.05;  // for disk medium
    // delta = 3.0;
    // initialize(density);
    delta = 1.0;
    // delta = 1.0;

    initialize(density);
}

IntervalAdaptiveDensityVDBHardcode::~IntervalAdaptiveDensityVDBHardcode() {
    delete[] majGrid;
}

std::vector<MacroGridQuery>
IntervalAdaptiveDensityVDBHardcode::MacroVoxelTraversal(const Point3f& pa,
                                                        const Point3f& pb,
                                                        Float baseTMin,
                                                        Float baseTMax) const {
    std::vector<MacroGridQuery> queries = std::vector<MacroGridQuery>();

    Float dx = 1.0 / Float(dim);
    Float dy = 1.0 / Float(dim);
    Float dz = 1.0 / Float(dim);

    int a_ix = std::floor((pa[0] - (-0.5)) / dx);
    int a_iy = std::floor((pa[1] - (-0.5)) / dy);
    int a_iz = std::floor((pa[2] - (-0.5)) / dz);

    int b_ix = std::floor((pb[0] - (-0.5)) / dx);
    int b_iy = std::floor((pb[1] - (-0.5)) / dy);
    int b_iz = std::floor((pb[2] - (-0.5)) / dz);

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

        if (majorant > 0.01) majorant = delta;

        int x = floor((c[0] - (-0.5)) * Float(dim));
        int y = floor((c[1] - (-0.5)) * Float(dim));
        int z = floor((c[2] - (-0.5)) * Float(dim));

        queries.push_back(
            MacroGridQuery(a, b, Point3i(x, y, z), majorant,
                           t_vals[pos] * (baseTMax - baseTMin) + baseTMin,
                           t_vals[pos + 1] * (baseTMax - baseTMin) + baseTMin));
    }

    return queries;
}

// extremely inefficient
Float IntervalAdaptiveDensityVDBHardcode::Maj(const Point3f& p) const {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    return getMajVoxel(x, y, z);
}

void IntervalAdaptiveDensityVDBHardcode::debug(Point3f p) const {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));
}

Float IntervalAdaptiveDensityVDBHardcode::Maj(const Point3f& pa,
                                              const Point3f& pb) const {
    return Maj((pa + pb) / 2.0);
}

void IntervalAdaptiveDensityVDBHardcode::setMajVoxel(int x, int y, int z,
                                                     Float maj) {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;

    // std::cout << "MAJ: " << maj << std::endl;

    std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
    std::cout << "OLD: " << majGrid[z * dim * dim + y * dim + x]
              << " NEW: " << maj << std::endl;

    majGrid[z * dim * dim + y * dim + x] = maj;
}

Float IntervalAdaptiveDensityVDBHardcode::getMajVoxel(int x, int y,
                                                      int z) const {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;

    return majGrid[z * dim * dim + y * dim + x];
}

void IntervalAdaptiveDensityVDBHardcode::initialize(
    const DensityFunction* dense) {
    majGrid = new Float[dim * dim * dim];

    if (dense == nullptr) return;  // debug logic

    Point3i density_min = dense->getMinDimensions();
    Vector3f density_cross = dense->getDimensionLengths();
    Vector3f trans_factor = dense->getTransFactor();
    trans_factor = Vector3f(0.0, 0.0, 0.0);

    int pad_dist = 1;  // 2

    Vector3f step = density_cross / Float(dim);
    Vector3f half_step = step / 2;
    Vector3f box_width = Vector3f(std::ceil(half_step[0]) + pad_dist,
                                  std::ceil(half_step[1]) + pad_dist,
                                  std::ceil(half_step[2]) + pad_dist);

    for (int x = 0; x < dim; ++x) {
        for (int y = 0; y < dim; ++y) {
            for (int z = 0; z < dim; ++z) {
                Vector3f voxel_center =
                    Vector3f(Float(x) * step[0] + half_step[0] +
                                 density_min[0] + trans_factor[0],
                             Float(y) * step[1] + half_step[1] +
                                 density_min[1] + trans_factor[1],
                             Float(z) * step[2] + half_step[2] +
                                 density_min[2] + trans_factor[2]);

                Vector3f min_exact = voxel_center - box_width;
                Vector3f max_exact = voxel_center + box_width;

                Point3i min =
                    Point3i(std::floor(min_exact[0]), std::floor(min_exact[1]),
                            std::floor(min_exact[2]));

                Point3i max =
                    Point3i(std::floor(max_exact[0]), std::floor(max_exact[1]),
                            std::floor(max_exact[2]));

                setMajVoxel(x, y, z, dense->getMaxDensityInCube(min, max)[0]);
            }
        }
    }

    std::cout << "finished preprocess" << std::endl;

    // hack for visualization purposes
    // dense->endInitialBuildState();
}

void IntervalAdaptiveDensityVDBHardcode::UpdateMajorant(const Point3f& p,
                                                        Float density) {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    Float maxVal = getMajVoxel(x, y, z);

    density_func->D_Debug(p);

    // uncomment for truly adaptive behavior

    if (density > maxVal) setMajVoxel(x, y, z, density + delta);
}

}  // namespace pbrt
