// #include "../density/grid_density.h"
#include "majorant.h"

namespace pbrt {

ConstMacroMajorant::ConstMacroMajorant(const DensityFunction* density, int w,
                                       int h, int d)
    : MajorantFunction(density), width(w), height(h), depth(d) {
    macroDense = new Float[width * height * depth];
    std::cout << "Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Depth: " << depth << std::endl;
    initialize(density);
}

// extremely inefficient
Float ConstMacroMajorant::Maj(const Point3f& pa, const Point3f& pb) const {
    // extremely naive implementation
    Vector3f diff = pb - pa;

    int startX = int((pa.x * width - 0.00001));
    int startY = int((pa.y * height - 0.00001));
    int startZ = int((pa.z * depth - 0.00001));

    int endX = int((pb.x * width - 0.00001));
    int endY = int((pb.y * height - 0.00001));
    int endZ = int((pb.z * depth - 0.00001));

    if (startX > endX) {
        int tmp = startX;
        startX = endX;
        endX = tmp;
    }

    if (startY > endY) {
        int tmp = startY;
        startY = endY;
        endY = tmp;
    }

    if (startZ > endZ) {
        int tmp = startZ;
        startZ = endZ;
        endZ = tmp;
    }

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (startZ < 0) startZ = 0;

    if (endX > width - 1) endX = width - 1;
    if (endY > height - 1) endY = height - 1;
    if (endZ > depth - 1) endZ = depth - 1;

    Float maj = 0.0;

    Ray ray = Ray(pa, Normalize(pb - pa), (pb - pa).Length());

    for (int i = startZ; i <= endZ; ++i) {
        for (int j = startY; j <= endY; ++j) {
            for (int k = startX; k <= endX; ++k) {
                Bounds3f bnds = Bounds3f(
                    Point3f(Float(k) / width, Float(j) / height,
                            Float(i) / depth),
                    Point3f(Float(k + 1) / width, Float(j + 1) / height,
                            Float(i + 1) / depth));

                if (Inside(pa, bnds) || Inside(pb, bnds)) {
                    // std::cout << "WOOO" << std::endl;
                    Float tmpMaj = macroDense[(i * height + j) * width + k];

                    // if (tmpMaj > 1.0) std::cout << "WHAT: " << tmpMaj <<
                    // std::endl;

                    if (tmpMaj > maj) maj = tmpMaj;
                    // if (maj == 0.0) {
                    //     std::cout << "HUH 2" << std::endl;
                    //     std::cout << "tmpMaj" << std::endl;
                    // }
                }

                Float tMin;
                Float tMax;

                if (bnds.IntersectP(ray, &tMin, &tMax)) {
                    Float tmpMaj = macroDense[(i * height + j) * width + k];
                    if (tmpMaj > maj) maj = tmpMaj;
                }
            }
        }
    }

    // if (maj == 0.0)
    // {
    //     std::cout << "Maj: " << maj << std::endl;
    //
    //     std::cout << "startX: " << startX << std::endl;
    //     std::cout << "startY: " << startY << std::endl;
    //     std::cout << "startZ: " << startZ << std::endl;
    //
    //     std::cout << "endX: " << endX << std::endl;
    //     std::cout << "endY: " << endY << std::endl;
    //     std::cout << "endZ: " << endZ << std::endl;
    //     std::cout << std::endl;
    // }

    // std::cout << "maj: " << maj << std::endl;

    return maj * scale;

    // better implementation
    // Point3f diff = pb - pa;
    //
    // int startX = int(pa.x * width);
    // int startY = int(pa.y * height);
    // int startZ = int(pa.z * depth);
    //
    // Float maj = 0.0;
    //
    // std::vector<Bounds3f> stack = std::vector<Bounds3f>();
    //
    // stack.push_back(Bounds3f(Point3f(Float(startX) / width,
    //                                  Float(startY) / height,
    //                                  Float(startZ) / depth),
    //                          Point3f(Float(startX + 1) / width,
    //                                  Float(startY + 1) / height,
    //                                  Float(startZ + 1) / depth)));
    //
    // while (!stack.empty())
    // {
    //     Bounds3f b = stack.pop_back();
    //
    //     Ray ray = Ray(pa, Normalize(pb - pa), (pb - pa).Length());
    //
    //     Float tMin;
    //     Float tMax;
    //
    //     if (!b.IntersectP(ray, &tMin, &tMax));
    //
    //
    // }
    //
    //
    //
    // // TODO
    // return 0.0;
}

void ConstMacroMajorant::initialize(const DensityFunction* dense) {
    // this is suuuuupppper old
    // const GridDensityFunction* density = (GridDensityFunction*)dense;
    // Float denseW = Float(density->getWidth());
    // Float denseH = Float(density->getHeight());
    // Float denseD = Float(density->getDepth());

    // Float dx = 1.0 / Float(width);
    // Float dy = 1.0 / Float(height);
    // Float dz = 1.0 / Float(depth);

    // for (int i = 0; i < depth; ++i)
    // {
    //     for (int j = 0; j < height; ++j)
    //     {
    //         for (int k = 0; k < width; ++k)
    //         {
    //             // int startX = int((Float(k) * dx) * denseW);
    //             // int startY = int((Float(j) * dy) * denseH);
    //             // int startZ = int((Float(i) * dz) * denseD);
    //             //
    //             // int endX = int((Float(k + 1) * dx) * denseW);
    //             // int endY = int((Float(j + 1) * dy) * denseH);
    //             // int endZ = int((Float(i + 1) * dz) * denseD);
    //             //
    //             // Float dense = density->getMaxDensityInCube(
    //             //                                  Point3i(startX, startY,
    //             startZ),
    //             //                                  Point3i(endX, endY,
    //             endZ));
    //             //
    //             // macroDense[(i * height + j) * width + k] = dense;

    //             Float dense = density->getMaxDensityInRange(
    //                                                 Point3f(Float(k) * dx,
    //                                                         Float(j) * dy,
    //                                                         Float(i) * dz),
    //                                                 Point3f(Float(k + 1) *
    //                                                 dx,
    //                                                         Float(j + 1) *
    //                                                         dy, Float(i + 1)
    //                                                         * dz))[0];

    //             std::cout << "x: " << k << " y: " << j << " z: " << i << " d:
    //             " << dense << std::endl;

    //             macroDense[(i * height + j) * width + k] = dense;
    //         }
    //     }
    // }
}

}  // namespace pbrt
