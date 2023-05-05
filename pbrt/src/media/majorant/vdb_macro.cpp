#include "../density/vdb_density.h"
#include "majorant.h"

#if OPENVDB

namespace pbrt {

VDBMacroMajorant::VDBMacroMajorant(const DensityFunction* density, int w, int h,
                                   int d)
    : MajorantFunction(density),
      // density_ref(density),
      width(w),
      height(h),
      depth(d) {
    // macroDense = new Float[width * height * depth];

    std::cout << "Macro Majorant Width: " << width << std::endl;
    std::cout << "Macro Majorant Height: " << height << std::endl;
    std::cout << "Macro Majorant Depth: " << depth << std::endl;

    initialize(density);
}

VDBMacroMajorant::~VDBMacroMajorant() {
    std::cout << "DELETING MACRO GRID" << std::endl;
    delete[] macroDense;
    // std::cout << "DELETING MACRO GRID" << std::endl;
}

Float VDBMacroMajorant::Maj(const Point3i& p) const {
    int j = p[0];
    int i = p[1];
    int k = p[2];

    if (j >= 0 && j < width && i >= 0 && i < height && k >= 0 && k < depth)
        return macroDense[(k * height + i) * width + j];

    return 0.0;
}

// Float VDBMacroMajorant::SingleMaj(const Point3i& p) const
// {
//     int j = p[0];
//     int i = p[1];
//     int k = p[2];
//
//     if (j >= 0 && j < width && i >= 0 && i < height && k >= 0 && k < depth)
//         return macroDense[(k * height + i) * width + j];
//
//     return 0.0;
// }

Float VDBMacroMajorant::Maj(const Point3f& p) const {
    // Float dx = (box_max_x - box_min_x) / width;
    // Float dy = (box_max_y - box_min_y) / height;
    // Float dz = (box_max_z - box_min_z) / depth;

    // std::cout << "bbox_min_x: " << box_min_x << std::endl;
    // std::cout << "bbox_max_y: " << box_max_y << std::endl;

    int j = (p[0] - box_min_x) / (box_max_x - box_min_x) * (width);
    int i = (p[2] - box_min_y) / (box_max_y - box_min_y) * (height);
    int k = (p[1] - box_min_z) / (box_max_z - box_min_z) * (depth);

    // if (j == width)
    // {
    //     std::cout << "FUCKSD" << std::endl;
    //     std::cout << "p[x]: " << p[0] << std::endl;
    // }

    // std::cout << "j: " << j << std::endl;
    // std::cout << "i: " << i << std::endl;
    // std::cout << "k: " << k << std::endl;

    Float maj = 0.0;
    int range = 0;

    // for (int kk = -range + k; kk <= range + k; ++kk)
    //     for (int ii = -range + i; ii <= range + i; ++ii)
    //         for (int jj = -range + j; jj <= range + j; ++jj)
    maj = std::max(maj, Maj(Point3i(j, i, k)));

    // maj = Maj(Point3i(j, i, k));
    // maj = std::max(maj, Maj(Point3i(j+1, i, k)));
    // maj = std::max(maj, Maj(Point3i(j-1, i, k)));
    // maj = std::max(maj, Maj(Point3i(j, i+1, k)));
    // maj = std::max(maj, Maj(Point3i(j, i-1, k)));
    // maj = std::max(maj, Maj(Point3i(j, i, k+1)));
    // maj = std::max(maj, Maj(Point3i(j, i, k-1)));

    return maj;
}

Point3i VDBMacroMajorant::Maj_Ind(const Point3f& p) const {
    // Float dx = (box_max_x - box_min_x) / width;
    // Float dy = (box_max_y - box_min_y) / height;
    // Float dz = (box_max_z - box_min_z) / depth;

    int j = (p[0] - box_min_x) / (box_max_x - box_min_x) * width;
    int i = (p[2] - box_min_y) / (box_max_y - box_min_y) * height;
    int k = (p[1] - box_min_z) / (box_max_z - box_min_z) * depth;

    // Float maj = 0.0;

    // for (int kk = -1 + k; kk <= 1 + k; ++kk)
    //     for (int ii = -1 + i; ii <= 1 + i; ++ii)
    //         for (int jj = -1 + j; jj <= 1 + j; ++jj)
    //             maj = std::max(maj, Maj(Point3i(jj, ii, kk)));

    return Point3i(j, i, k);
    // maj = std::max(maj, Maj(Point3i(j+1, i, k)));
    // maj = std::max(maj, Maj(Point3i(j-1, i, k)));
    // maj = std::max(maj, Maj(Point3i(j, i+1, k)));
    // maj = std::max(maj, Maj(Point3i(j, i-1, k)));
    // maj = std::max(maj, Maj(Point3i(j, i, k+1)));
    // maj = std::max(maj, Maj(Point3i(j, i, k-1)));

    // return maj;
}

Float VDBMacroMajorant::Maj(const Point3f& pa, const Point3f& pb) const {
    // std::cout << "maj pa: " << pa << std::endl;
    // throw new std::exception();
    Float dx = (box_max_x - box_min_x) / Float(width);
    Float dy = (box_max_y - box_min_y) / Float(height);
    Float dz = (box_max_z - box_min_z) / Float(depth);

    // std::cout << "DX: " << dx << std::endl;
    // std::cout << "DY: " << dy << std::endl;
    // std::cout << "DZ: " << dz << std::endl;
    // std::cout << std::endl;

    int a_ix = floor((pa[0] - box_min_x) / dx);
    int a_iy = floor((pa[2] - box_min_y) / dy);
    int a_iz = floor((pa[1] - box_min_z) / dz);

    int b_ix = floor((pb[0] - box_min_x) / dx);
    int b_iy = floor((pb[2] - box_min_y) / dy);
    int b_iz = floor((pb[1] - box_min_z) / dz);

    Float a_vx = (pa[0] - box_min_x) / dx;
    Float a_vy = (pa[2] - box_min_y) / dy;
    Float a_vz = (pa[1] - box_min_z) / dz;

    Float b_vx = (pb[0] - box_min_x) / dx;
    Float b_vy = (pb[2] - box_min_y) / dy;
    Float b_vz = (pb[1] - box_min_z) / dz;

    // std::cout << "Pa: " << pa << std::endl;
    // std::cout << "Pb: " << pb << std::endl;

    // Float a_vx = pa[0] / dx - (box_min_x / dx);
    // Float a_vy = pa[1] / dy - (box_min_y / dy);
    // Float a_vz = pa[2] / dz - (box_min_z / dz);
    //
    // Float b_vx = pb[0] / dx - (box_min_x / dx);
    // Float b_vy = pb[1] / dy - (box_min_y / dy);
    // Float b_vz = pb[2] / dz - (box_min_z / dz);

    // std::cout << "avx: " << a_vx << std::endl;
    // std::cout << "avy: " << a_vy << std::endl;
    // std::cout << "avz: " << a_vz << std::endl;
    // std::cout << std::endl;
    //
    // std::cout << "bvx: " << b_vx << std::endl;
    // std::cout << "bvy: " << b_vy << std::endl;
    // std::cout << "bvz: " << b_vz << std::endl;
    // std::cout << std::endl;
    //
    // std::cout << "aix: " << a_ix << std::endl;
    // std::cout << "aiy: " << a_iy << std::endl;
    // std::cout << "aiz: " << a_iz << std::endl;
    // std::cout << std::endl;
    //
    // std::cout << "bix: " << b_ix << std::endl;
    // std::cout << "biy: " << b_iy << std::endl;
    // std::cout << "biz: " << b_iz << std::endl;
    // std::cout << std::endl;

    std::vector<Float> t_vals = std::vector<Float>();

    t_vals.push_back(0.0);
    t_vals.push_back(1.0);  // this is probably redundant

    while (a_ix != b_ix) {
        a_ix = a_ix + (b_ix - a_ix) / (std::abs(b_ix - a_ix));
        Float index = Float(a_ix);
        Float t = (index - a_vx) / (b_vx - a_vx);
        // std::cout << "xT: " << t << std::endl;
        // t_vals.push_back(t + 0.001);
        // t_vals.push_back(t - 0.001);
        t_vals.push_back(t);
    }

    while (a_iy != b_iy) {
        a_iy = a_iy + (b_iy - a_iy) / (std::abs(b_iy - a_iy));
        Float index = Float(a_iy);
        Float t = (index - a_vy) / (b_vy - a_vy);
        // std::cout << "yT: " << t << std::endl;
        // t_vals.push_back(t + 0.001);
        // t_vals.push_back(t - 0.001);
        t_vals.push_back(t);
    }

    while (a_iz != b_iz) {
        a_iz = a_iz + (b_iz - a_iz) / (std::abs(b_iz - a_iz));
        Float index = Float(a_iz);
        Float t = (index - a_vz) / (b_vz - a_vz);
        // std::cout << "zT: " << t << std::endl;
        // t_vals.push_back(t + 0.001);
        // t_vals.push_back(t - 0.001);
        t_vals.push_back(t);
    }

    Float maj_val = 0.0;

    for (int pos = 0; pos < t_vals.size(); ++pos) {
        // int j = floor((b_vx - a_vx) * t_vals[pos] + a_vx);
        // int i = floor((b_vy - a_vy) * t_vals[pos] + b_vy);
        // int k = floor((b_vz - a_vz) * t_vals[pos] + b_vz);

        int j = floor((b_vx - a_vx) * t_vals[pos] + a_vx);
        int i = floor((b_vy - a_vy) * t_vals[pos] + a_vy);
        int k = floor((b_vz - a_vz) * t_vals[pos] + a_vz);

        // std::cout << "j: " << j << " i: " << i << " k: " << k << " t: " <<
        // t_vals[pos] << std::endl;

        if (j >= 0 && j < width && i >= 0 && i < height && k >= 0 &&
            k < depth) {
            if (macroDense[(k * height + i) * width + j] > maj_val) {
                // std::cout << "DENSE: " << macroDense[(k * height + i) * width
                // + j] << std::endl;
                maj_val =
                    std::max(maj_val, macroDense[(k * height + i) * width + j]);
            }
        }
    }

    // std::cout << "SCALE: " << scale << std::endl;
    // std::cout << "Maj_val: " << maj_val << std::endl;

    return maj_val;

    // return maj_val * scale;
}

void VDBMacroMajorant::initialize(const DensityFunction* dense) {
    std::cout << "MACROGRID INITIALIZING" << std::endl;
    const VDB_DensityFunction* density = (VDB_DensityFunction*)dense;

    openvdb::Vec3R min = density->getMinLocalSpaceCoord();  // vdb space
    openvdb::Vec3R max = density->getMaxLocalSpaceCoord();  // vdb space

    // storing for use later
    box_min_x = density->getBox_Min_X();  // local space
    box_min_y = density->getBox_Min_Y();  // local space
    box_min_z = density->getBox_Min_Z();  // local space

    box_max_x = density->getBox_Max_X();  // local space
    box_max_y = density->getBox_Max_Y();  // local space
    box_max_z = density->getBox_Max_Z();  // local space

    std::cout << "min.x " << min.x() << std::endl;
    std::cout << "min.y " << min.y() << std::endl;
    std::cout << "min.z " << min.z() << std::endl;

    std::cout << "max.x " << max.x() << std::endl;
    std::cout << "max.y " << max.y() << std::endl;
    std::cout << "max.z " << max.z() << std::endl;

    std::cout << "box_min_x " << box_min_x << std::endl;
    std::cout << "box_max_x " << box_max_x << std::endl;
    std::cout << "box_min_y " << box_min_y << std::endl;
    std::cout << "box_max_y " << box_max_y << std::endl;
    std::cout << "box_min_z " << box_min_z << std::endl;
    std::cout << "box_max_z " << box_max_z << std::endl;

    // remember vdb y and z are flipped
    Float dense_width = max.x() - min.x();   // vdb space
    Float dense_height = max.y() - min.y();  // vdb space
    Float dense_depth = max.z() - min.z();   // vdb space

    // TODO: THIS IS DEBUGGING
    width = max.x() - min.x();   // vdb space
    height = max.y() - min.y();  // vdb space
    depth = max.z() - min.z();   // vdb space

    macroDense = new Float[width * height * depth];

    Float num_x = dense_width / Float(width);    // vdb space -> local space
    Float num_y = dense_height / Float(height);  // vdb space -> local space
    Float num_z = dense_depth / Float(depth);    // vdb space -> local space

    std::cout << "dense_width: " << dense_width << std::endl;
    std::cout << "dense_height: " << dense_height << std::endl;
    std::cout << "dense_depth: " << dense_depth << std::endl;

    std::cout << "Num_X: " << num_x << std::endl;
    std::cout << "Num_Y: " << num_y << std::endl;
    std::cout << "Num_Z: " << num_z << std::endl;
    //
    // std::cout << "MinX: " << min.x() << std::endl;
    // std::cout << "MinY: " << min.y() << std::endl;
    // std::cout << "MinZ: " << min.z() << std::endl;
    //
    // std::cout << "MaxX: " << max.x() << std::endl;
    // std::cout << "MaxY: " << max.y() << std::endl;
    // std::cout << "MaxZ: " << max.z() << std::endl;

    // int overlap = 1;
    int overlap = 0;

    for (int k = 0; k < depth; ++k) {
        // std::cout << "k: " << k << std::endl;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                // std::cout << "Voxel: j: " << j << " i: " << i << " k: " << k
                // << std::endl; std::cout << "(" << std::floor(num_x * j) +
                // min.x() - 5 << ", " << std::floor(num_y * k) + min.y() - 5 <<
                // ", " << std::floor(num_z * i) + min.z() - 5 << ") ->";
                // std::cout << "(" << std::ceil(num_x * (j+1)) + min.x() + 5 <<
                // ", " << std::ceil(num_y * (k+1)) + min.y() + 5 << ", " <<
                // std::ceil(num_z * (i+1)) + min.z() + 5 << std::endl <<
                // std::endl;

                Point3i min_spot =
                    Point3i(std::floor(num_x * Float(j)) + min.x() - overlap,
                            std::floor(num_y * Float(i)) + min.y() - overlap,
                            std::floor(num_z * Float(k)) + min.z() - overlap);
                Point3i max_spot = Point3i( // TODO: this might be broken for non-perf bounds
                    std::ceil(num_x * Float(j + 1)) + min.x() + overlap,
                    std::ceil(num_y * Float(i + 1)) + min.y() + overlap,
                    std::ceil(num_z * Float(k + 1)) + min.z() + overlap);

                // std::cout << "min_x: " << min_spot[0] << " max_x: " <<
                // max_spot[0] << std::endl;

                Spectrum maj = density->getMaxDensityInCube(min_spot, max_spot);

                if (num_x == 1 && num_y == 1 && num_z == 1)
                {
                    Point3i spot = Point3i(j + min.x(),i + min.y(),k + min.z());
                    // std::cout << "HELLO" << std::endl;
                    // std::cout << "boyaaa" << std::endl;
                    Float tmp = density->D(min_spot)[0];
                    // if (maj[0] != tmp)
                    // {
                    //     std::cout << "FUCK I AM AN IDIOT" << std::endl;
                    // }
                    macroDense[(k * height + i) * width + j] = tmp;
                }

                else
                {
                    std::cout << "THIS SHOULD NOT HAPPEN" << std::endl;
                // TODO: make a version for colored media
                macroDense[(k * height + i) * width + j] = maj[0];
                }
            }
        }
    }

    std::cout << "finished creating accel structure" << std::endl;

    // sanity check passes so why doesnt the density runs?
    // Sanity Checks:
    // std::cout << "Starting Macrogrid Sanity Checks" << std::endl;
    //
    // std::cout << "Evaluating point: (-0.08099911467897139,
    // -0.33602578011698858, 0.31431216665967421)" << std::endl;
    //
    // Spectrum point_d = density->D(Point3f(-0.08099911467897139,
    // -0.33602578011698858, 0.31431216665967421)); Float point_dense =
    // point_d[0];
    //
    // Float point_maj = Maj(Point3f(-0.08099911467897139, -0.33602578011698858,
    // 0.31431216665967421));
    //
    // std::cout << "Point Maj: " << point_maj << " Point Dense: " <<
    // point_dense << std::endl;
    //
    // for (int k = min.z(); k < max.z(); ++k)
    // {
    //     for (int i = min.y(); i < max.y(); ++i)
    //     {
    //         for (int j = min.x(); j < max.x(); ++j)
    //         {
    //             Spectrum d = density->D(Point3i(j, i, k));
    //             Spectrum real_d = density->D(Point3f((j - min.x()) / (max.x()
    //             - min.x()) - 0.5,
    //                                                  (k - min.z()) / (max.z()
    //                                                  - min.z()) - 0.5, (i -
    //                                                  min.y()) / (max.y() -
    //                                                  min.y()) - 0.5));
    //
    //             Float dense = (d[0] > 1.0) ? 1.0 : d[0];
    //             Float real_dense = real_d[0];
    //
    //             if (real_dense - dense > 1e-5)
    //             {
    //                 std::cout << "ERROR: REAL != ESTIMATED" << std::endl;
    //                 std::cout << "REAL: " << real_dense << std::endl;
    //                 std::cout << "EST: " << dense << std::endl;
    //                 std::cout << "LOCATION: j: " << j << " i: " << i << " k:
    //                 " << k << std::endl;
    //             }
    //
    //             // std::cout << "j: " << j << " i: " << i << " k: " << k << "
    //             -> ";
    //
    //             Float maj = Maj(Point3f((box_max_x - box_min_x) * ((j -
    //             min.x()) / (max.x() - min.x())) + box_min_x,
    //                                     (box_max_y - box_min_y) * ((k -
    //                                     min.z()) / (max.z() - min.z())) +
    //                                     box_min_y, (box_max_z - box_min_z) *
    //                                     ((i - min.y()) / (max.y() - min.y()))
    //                                     + box_min_z));
    //             // Float maj = macroDense[int(((i-min.y()) / num_y) * height
    //             + int((k-min.z()) / num_z)) * width + int((j-min.x()) /
    //             num_x)];
    //
    //             if (dense > maj)
    //             {
    //                 std::cout << "ERROR: j: " << j << " i: " << i << " k: "
    //                 << k << std::endl; std::cout << "dense = " << dense <<
    //                 "Majorant: " << maj << std::endl;
    //             }
    //         }
    //     }
    // }
    //
    // // std::cout << "Finished all Sanity Checks" << std::endl;
}

bool VDBMacroMajorant::is_inside(Point3f p) const {
    int j = (p[0] - box_min_x) / (box_max_x - box_min_x) * width;
    int i = (p[2] - box_min_y) / (box_max_y - box_min_y) * height;
    int k = (p[1] - box_min_z) / (box_max_z - box_min_z) * depth;

    if (j >= 0 && j < width && i >= 0 && i < height && k >= 0 && k < depth)
        return true;

    return false;
}

std::vector<MacroGridQuery> VDBMacroMajorant::MacroVoxelTraversal(
    const Point3f& pa, const Point3f& pb) const {
    std::vector<MacroGridQuery> queries = std::vector<MacroGridQuery>();

    Float dx = (box_max_x - box_min_x) / width;
    Float dy = (box_max_y - box_min_y) / height;
    Float dz = (box_max_z - box_min_z) / depth;

    int a_ix = floor((pa[0] - box_min_x) / dx);
    int a_iy = floor((pa[2] - box_min_y) / dy);
    int a_iz = floor((pa[1] - box_min_z) / dz);

    int b_ix = floor((pb[0] - box_min_x) / dx);
    int b_iy = floor((pb[2] - box_min_y) / dy);
    int b_iz = floor((pb[1] - box_min_z) / dz);

    Float a_vx = (pa[0] - box_min_x) / dx;
    Float a_vy = (pa[2] - box_min_y) / dy;
    Float a_vz = (pa[1] - box_min_z) / dz;

    Float b_vx = (pb[0] - box_min_x) / dx;
    Float b_vy = (pb[2] - box_min_y) / dy;
    Float b_vz = (pb[1] - box_min_z) / dz;

    std::vector<Float> t_vals = std::vector<Float>();

    t_vals.push_back(0.0);

    while (a_ix != b_ix) {
        a_ix = a_ix + (b_ix - a_ix) / (std::abs(b_ix - a_ix));
        Float index = Float(a_ix);
        Float t = (index - a_vx) / (b_vx - a_vx);

        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    while (a_iy != b_iy) {
        a_iy = a_iy + (b_iy - a_iy) / (std::abs(b_iy - a_iy));
        Float index = Float(a_iy);
        Float t = (index - a_vy) / (b_vy - a_vy);
        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    while (a_iz != b_iz) {
        a_iz = a_iz + (b_iz - a_iz) / (std::abs(b_iz - a_iz));
        Float index = Float(a_iz);
        Float t = (index - a_vz) / (b_vz - a_vz);
        if (t > 0.0 && t < 1.0) t_vals.push_back(t);
    }

    t_vals.push_back(1.0);

    // I wish i did not have to sort but oh well... improve this later
    std::sort(t_vals.begin(), t_vals.end());

    for (int pos = 0; pos < t_vals.size() - 1; ++pos) {
        if (t_vals[pos] >= 1.0) continue;

        Point3f a = Point3f((pb[0] - pa[0]) * t_vals[pos] + pa[0],
                            (pb[1] - pa[1]) * t_vals[pos] + pa[1],
                            (pb[2] - pa[2]) * t_vals[pos] + pa[2]);

        Point3f b = Point3f((pb[0] - pa[0]) * t_vals[pos + 1] + pa[0],
                            (pb[1] - pa[1]) * t_vals[pos + 1] + pa[1],
                            (pb[2] - pa[2]) * t_vals[pos + 1] + pa[2]);

        Point3f c = (a + b) / 2.0;

        Float majorant = Maj(c);

        if (majorant < 0.0) std::cout << majorant << std::endl;
        if (majorant > 1000000.0) std::cout << majorant << std::endl;

        if (t_vals[pos] < 0.0) std::cout << "heck" << std::endl;
        if (t_vals[pos] > 1.0) std::cout << "heck" << std::endl;
        if (t_vals[pos + 1] > 1.0) std::cout << "heck" << std::endl;

        // if (a[0] > b[0] || a[1] > b[1] || a[2] > b[2])
        // {
        //     std::cout << "THIS IS WRONG" << std::endl;
        // }

        queries.push_back(MacroGridQuery(a, b, Maj_Ind(c), majorant,
                                         t_vals[pos], t_vals[pos + 1]));
    }

    // std::cout << "ret" << std::endl;

    return queries;
}

}  // namespace pbrt

#endif
