#include <exception>
#include <fstream>

#include "majorant.h"

namespace pbrt {

IntervalAdaptiveDensityVDBTracking::IntervalAdaptiveDensityVDBTracking(
    const DensityFunction* density, int dim)
    : MajorantFunction(density), dim(dim) {
    // delta = 0.1;   // for sphere medium
    // delta = 0.05;  // for disk medium
    // delta = 3.0;
    // initialize(density);
    delta = 0.2;
    start_dense = 0.01;

    initialize(density);
}

IntervalAdaptiveDensityVDBTracking::~IntervalAdaptiveDensityVDBTracking() {
    delete[] majGrid;
    delete[] gtGrid;
}

std::vector<MacroGridQuery>
IntervalAdaptiveDensityVDBTracking::MacroVoxelTraversal(const Point3f& pa,
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
Float IntervalAdaptiveDensityVDBTracking::Maj(const Point3f& p) const {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    return getMajVoxel(x, y, z);
}

void IntervalAdaptiveDensityVDBTracking::debug(Point3f p) const {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    // std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
}

Float IntervalAdaptiveDensityVDBTracking::Maj(const Point3f& pa,
                                              const Point3f& pb) const {
    // return Maj((pa + pb) / 2.0);

    return 1.0;
}

void IntervalAdaptiveDensityVDBTracking::setMajVoxel(int x, int y, int z,
                                                     Float maj) {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;

    // std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
    // std::cout << "OLD: " << majGrid[z * dim * dim + y * dim + x]
    //           << " NEW: " << maj << std::endl;

    majGrid[z * dim * dim + y * dim + x] = maj;
}

void IntervalAdaptiveDensityVDBTracking::setGTMajVoxel(int x, int y, int z,
                                                       Float maj) {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;

    // std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
    // std::cout << "OLD: " << majGrid[z * dim * dim + y * dim + x]
    //           << " NEW: " << maj << std::endl;

    gtGrid[z * dim * dim + y * dim + x] = maj;
}

Float IntervalAdaptiveDensityVDBTracking::getMajVoxel(int x, int y,
                                                      int z) const {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;

    return majGrid[z * dim * dim + y * dim + x];
}

Float IntervalAdaptiveDensityVDBTracking::getGTMajVoxel(int x, int y,
                                                        int z) const {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (z < 0) z = 0;
    if (x >= dim) x = dim - 1;
    if (y >= dim) y = dim - 1;
    if (z >= dim) z = dim - 1;

    return gtGrid[z * dim * dim + y * dim + x];
}

void IntervalAdaptiveDensityVDBTracking::initialize(
    const DensityFunction* dense) {
    majGrid = new Float[dim * dim * dim];
    gtGrid = new Float[dim * dim * dim];

    if (dense == nullptr) return;  // debug logic

    Point3i density_min = dense->getMinDimensions();
    Vector3f density_cross = dense->getDimensionLengths();
    Vector3f trans_factor = dense->getTransFactor();
    std::cout << "TRANS FACTOR: " << trans_factor[0] << " " << trans_factor[1]
              << " " << trans_factor[2] << std::endl;
    trans_factor = Vector3f(0.0, 0.0, 0.0);

    int pad_dist = 0;  // 2

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

                setGTMajVoxel(x, y, z, dense->getMaxDensityInCube(min, max)[0]);
                setMajVoxel(x, y, z, start_dense);
                // for bunny scene
                // Point3i cent =
                //     Point3i((max[0] + min[0]) / 2, (max[1] + min[1]) / 2,
                //             (max[2] + min[2]) / 2);
                // setMajVoxel(x, y, z, dense->getMaxDensityInCube(cent,
                // cent)[0]); setMajVoxel(x, y, z,
                // dense->getMaxDensityInCube(min, max)[0]);
            }
        }
    }

    std::cout << "finished preprocess" << std::endl;

    // hack for visualization purposes
    // dense->endInitialBuildState();
}

void IntervalAdaptiveDensityVDBTracking::UpdateMajorant(const Point3f& p,
                                                        Float density) {
    int x = floor((p[0] - (-0.5)) * Float(dim));
    int y = floor((p[1] - (-0.5)) * Float(dim));
    int z = floor((p[2] - (-0.5)) * Float(dim));

    // std::cout << std::endl;
    // std::cout << "Updating Majorant with Density: " << density << std::endl;
    // std::cout << "At Location: " << p[0] << ", " << p[1] << ", " << p[2]
    //           << std::endl;
    Float maxVal = getMajVoxel(x, y, z);

    // density_func->D_Debug(p);

    // uncomment for truly adaptive behavior

    // std::cout << std::endl;
    // std::cout << "X: " << p[0] - (-0.5) << std::endl;
    // std::cout << "Y: " << p[1] - (-0.5) << std::endl;
    // std::cout << "Z: " << p[2] - (-0.5) << std::endl;
    // std::cout << std::endl;

    // turn this off for biased runs
    if (density > maxVal) {
        // std::cout << "ACTUALLY UPDATING" << std::endl;
        setMajVoxel(x, y, z, density + delta);
        int non_bounding = 0;
        for (int i = 0; i < dim * dim * dim; ++i) {
            if (gtGrid[i] > majGrid[i]) non_bounding++;
        }
        // std::cout << "FIUCKS" << std::endl;
        // std::cout << "Non_bounding Voxels Remaining: " << non_bounding
        //           << std::endl;
    }

    // std::cout << std::endl;
}

void IntervalAdaptiveDensityVDBTracking::RenderVerticalSlice(
    std::string filename, int ps) const {
    std::cout << "RENDERING SLICE TO: " << filename + "_maj.exr" << std::endl;

    int total_non_bounding = 0;
    int initially_non_bounding = 0;
    for (int i = 0; i < dim * dim * dim; ++i) {
        if (gtGrid[i] > majGrid[i]) total_non_bounding++;
        if (gtGrid[i] > start_dense) initially_non_bounding++;
    }
    std::cout << "Non_bounding Voxels Remaining: " << total_non_bounding
              << std::endl;

    std::ofstream file;
    file.open(filename + "_" + std::to_string(ps) + "_bound_info.txt");

    file << initially_non_bounding << std::endl;
    file << total_non_bounding << std::endl;
    file << dim << std::endl;

    // the Globals::ACCEL_SCENE is a constant which is used to determine
    // how to orient the images since every scenes camera views the
    // scenes from a different angle. All this is hardcoded.

    for (int k = 0; k < dim; ++k) {
        imedit::Image image = imedit::Image(dim, dim);

        int local_non_bounding = 0;

        // use this for the bunny
        if (Globals::ACCEL_SCENE == 2) {
            for (int i = 0; i < dim; ++i) {
                for (int j = 0; j < dim; ++j) {
                    image(j, i) = imedit::Pixel(getMajVoxel(j, dim - i - 1, k));
                    if (getMajVoxel(j, dim - 1 - i, k) <
                        getGTMajVoxel(j, dim - 1 - i, k))
                        local_non_bounding++;
                }
            }
        }
        // use this for the teaser
        else if (Globals::ACCEL_SCENE == 1) {
            for (int i = 0; i < dim; ++i) {
                for (int j = 0; j < dim; ++j) {
                    image(j, i) =
                        imedit::Pixel(getMajVoxel(dim - 1 - j, k, dim - i - 1));
                    if (getMajVoxel(dim - 1 - j, dim - 1 - i, k) <
                        getGTMajVoxel(dim - 1 - j, dim - 1 - i, k))
                        local_non_bounding++;
                }
            }
        } else {
            std::cout << "SOFT ERROR: Acceleration structure does not know how "
                         "to orient the images to match the medium slices. "
                         "Please implement custom logic in "
                         "interval_adaptive_vdb_track.cpp to handle your scene"
                      << std::endl;
        }

        file << local_non_bounding << std::endl;

        int resize_factor = 400 / dim;
        imedit::Image full_image = imedit::Image(
            image.width() * resize_factor, image.height() * resize_factor);

        for (int i = 0; i < full_image.height(); ++i) {
            for (int j = 0; j < full_image.height(); ++j) {
                full_image(j, i) = image(j / resize_factor, i / resize_factor);
            }
        }
        full_image.write(filename + "_" + std::to_string(ps) + "_maj_" +
                         std::to_string(k) + ".png");
        full_image.write(filename + "_" + std::to_string(ps) + "_maj_" +
                         std::to_string(k) + ".exr");
    }
    file.close();
}

}  // namespace pbrt
