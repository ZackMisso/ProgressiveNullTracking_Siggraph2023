// #include "vdb_density.h"

// #if OPENVDB

// #include <openvdb/tools/Interpolation.h>

// namespace pbrt {

// VDB_DensityFunction::VDB_DensityFunction(openvdb::FloatGrid::Ptr grid)
//     : grid(grid) {
//     clampDensities = false;
//     retargetDensities = false;
//     retainAspectRatio = false;
//     worldSpace = true;
// }

// VDB_DensityFunction::VDB_DensityFunction(std::string filename) {
//     clampDensities = false;
//     retargetDensities = false;
//     retainAspectRatio = false;
//     worldSpace = true;

//     std::cout << "opening: " << filename << std::endl;

//     openvdb::io::File file(filename);
//     file.open();

//     // openvdb::GridBase::Ptr baseGrid;

//     std::cout << "Loading VDB Density Grid" << std::endl;

//     for (openvdb::io::File::NameIterator nameIter = file.beginName();
//          nameIter != file.endName(); ++nameIter) {
//         if (nameIter.gridName() == "density") {
//             baseGrid = file.readGrid(nameIter.gridName());
//         } else {
//             std::cout << "Ignoring Grid: " << nameIter.gridName() <<
//             std::endl;
//         }
//     }

//     grid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);

//     file.close();
// }

// VDB_DensityFunction::~VDB_DensityFunction() {
//     std::cout << "Deconstructing VDB Density" << std::endl;
//     grid->clear();
// }

// void VDB_DensityFunction::preProcess() {
//     openvdb::CoordBBox bbox = grid->evalActiveVoxelBoundingBox();

//     min = bbox.min();
//     max = bbox.max();

//     // use this for the cloud - hacky but whatevs
//     // min = density->getMinLocalSpaceCoord();
//     // max = density->getMaxLocalSpaceCoord();

//     std::cout << "min: " << min[0] << " " << min[1] << " " << min[2]
//               << std::endl;
//     std::cout << "max: " << max[0] << " " << max[1] << " " << max[2]
//               << std::endl;

//     openvdb::tools::GridSampler<openvdb::FloatGrid,
//                                 openvdb::tools::PointSampler>
//         sampler(*grid);

//     maxDensity = 0;

//     // TODO:: also implement macrogrid majorants into this mess
//     for (int k = min.z(); k <= max.z(); ++k) {
//         for (int i = min.y(); i <= max.y(); ++i) {
//             for (int j = min.x(); j <= max.x(); ++j) {
//                 Float value = sampler.isSample(
//                     openvdb::Vec3R(Float(j), Float(i), Float(k)));
//                 if (value > maxDensity) {
//                     maxDensity = value;
//                 }
//             }
//         }
//     }

//     std::cout << "MAX DENSITY: " << maxDensity << std::endl;

//     // adding one because the max is inclusive
//     xScaleFactor = (max.x() - min.x() + 1);
//     yScaleFactor = (max.y() - min.y() + 1);
//     zScaleFactor = (max.z() - min.z() + 1);

//     std::cout << "xScaleFactor: " << xScaleFactor << std::endl;
//     std::cout << "yScaleFactor: " << yScaleFactor << std::endl;
//     std::cout << "zScaleFactor: " << zScaleFactor << std::endl;

//     xTransFactor = xScaleFactor;
//     yTransFactor = yScaleFactor;
//     zTransFactor = zScaleFactor;

//     // xTransFactor = 0.0;
//     // yTransFactor = 0.0;
//     // zTransFactor = 0.0;

//     if (retainAspectRatio) {
//         std::cout << "fufk" << std::endl;
//         if (xScaleFactor >= yScaleFactor && xScaleFactor >= zScaleFactor) {
//             yScaleFactor = xScaleFactor;
//             zScaleFactor = xScaleFactor;

//             yTransFactor = (yScaleFactor - yTransFactor) / (2.0);
//             zTransFactor = (zScaleFactor - zTransFactor) / (2.0);
//             xTransFactor = 0.0;
//         } else if (yScaleFactor >= zScaleFactor &&
//                    yScaleFactor >= zScaleFactor) {
//             xScaleFactor = yScaleFactor;
//             zScaleFactor = yScaleFactor;

//             xTransFactor = (xScaleFactor - xTransFactor) / (2.0);
//             zTransFactor = (zScaleFactor - zTransFactor) / (2.0);
//             yTransFactor = 0.0;
//         } else {
//             xScaleFactor = zScaleFactor;
//             yScaleFactor = zScaleFactor;

//             xTransFactor = (xScaleFactor - xTransFactor) / (2.0);
//             yTransFactor = (yScaleFactor - yTransFactor) / (2.0);
//             zTransFactor = 0.0;
//         }
//     } else {
//         std::cout << "Whoop" << std::endl;
//         xTransFactor = 0.0;
//         yTransFactor = 0.0;
//         zTransFactor = 0.0;
//     }

//     // std::cout << "box_x_max: " << box_max_x << " box_x_min: " << box_min_x
//     <<
//     // std::endl;

//     // xScaleFactor /= (box_max_x - box_min_x);
//     // yScaleFactor /= (box_max_z - box_min_z);
//     // zScaleFactor /= (box_max_y - box_min_y);

//     // 0.5, -0.5
//     xScaleFactor /= (box_max_x - box_min_x);
//     yScaleFactor /= (box_max_y - box_min_y);
//     zScaleFactor /= (box_max_z - box_min_z);

//     std::cout << "xScaleFactor final: " << xScaleFactor << std::endl;
//     std::cout << "yScaleFactor final: " << yScaleFactor << std::endl;
//     std::cout << "zScaleFactor final: " << zScaleFactor << std::endl;
// }

// openvdb::Vec3R VDB_DensityFunction::getMinLocalSpaceCoord() const {
//     return openvdb::Vec3R(
//         (box_min_x - box_min_x) * xScaleFactor - xTransFactor + min.x(),
//         (box_min_z - box_min_z) * yScaleFactor - yTransFactor + min.y(),
//         (box_min_y - box_min_y) * zScaleFactor - zTransFactor + min.z());

//     // return openvdb::Vec3R((box_min_x - box_min_x) * xScaleFactor +
//     min.x(),
//     //                       (box_min_z - box_min_z) * yScaleFactor +
//     min.y(),
//     //                       (box_min_y - box_min_y) * zScaleFactor +
//     min.z());
// }

// openvdb::Vec3R VDB_DensityFunction::getMaxLocalSpaceCoord() const {
//     return openvdb::Vec3R(
//         (box_max_x - box_min_x) * xScaleFactor - xTransFactor + min.x(),
//         (box_max_z - box_min_z) * yScaleFactor - yTransFactor + min.y(),
//         (box_max_y - box_min_y) * zScaleFactor - zTransFactor + min.z());

//     // return openvdb::Vec3R((box_max_x - box_min_x) * xScaleFactor +
//     min.x(),
//     //                       (box_max_z - box_min_z) * yScaleFactor +
//     min.y(),
//     //                       (box_max_y - box_min_y) * zScaleFactor +
//     min.z());
// }

// // openvdb::Vec3R VDB_DensityFunction::localToIndexSpace(const Point3f& p)
// const
// // {
// //     return openvdb::Vec3R(
// //         (p[0] - box_min_x) * xScaleFactor - xTransFactor + min.x(),
// //         (p[2] - box_min_z) * yScaleFactor - yTransFactor + min.y(),
// //         (p[1] - box_min_y) * zScaleFactor - zTransFactor + min.z());
// // }

// Spectrum VDB_DensityFunction::D(const Point3f& p, bool buildState) const {
//     nDensityCalls++;

//     openvdb::tools::GridSampler<openvdb::FloatGrid,
//     openvdb::tools::BoxSampler>
//         sampler(*grid);
//     // openvdb::tools::GridSampler<openvdb::FloatGrid,
//     // openvdb::tools::PointSampler> sampler(*grid);

//     // std::cout << openvdb::tools::BoxSampler::radius() << std::endl;

//     if (worldSpace) {
//         // Note: houdini stores the up axis as y while most pbrt scens have z
//         as
//         // the up vector
//         //       so the indices are swivled.

//         // index space sample
//         // Float indexValue = (Float)sampler.isSample(openvdb::Vec3R(p[0],
//         p[1],
//         // p[2]));
//         std::cout << "WHOOP" << std::endl;

//         // world space sample
//         Float indexValue = (Float)sampler.wsSample(
//             openvdb::Vec3R(p[0] / 10.0, p[2] / 10.0, p[1] / 10.0));

//         if (indexValue > 1.0) {
//             // std::cout << "Index Value: " << indexValue << std::endl;
//             indexValue = 1.0;
//         }

//         return Spectrum(indexValue);
//     } else {
//         Float sampledValue;

//         // std::cout << "xTransFactor: " << xTransFactor << std::endl;

// #if NN_VOLUMES
//         // Point3i coord = Point3i(
//         //     int((p[0] - box_min_x) * xScaleFactor - xTransFactor +
//         min.x()),
//         //     int((p[2] - box_min_y) * yScaleFactor - yTransFactor +
//         min.y()),
//         //     int((p[1] - box_min_z) * zScaleFactor - zTransFactor +
//         min.z()));

//         Point3i coord = Point3i(
//             floor((p[0] - box_min_x) * xScaleFactor - xTransFactor +
//             min.x()), floor((p[1] - box_min_y) * yScaleFactor - yTransFactor
//             + min.y()), floor((p[2] - box_min_z) * zScaleFactor -
//             zTransFactor + min.z()));

//         sampledValue = D(coord)[0];
// #else
//         sampledValue = (Float)sampler.isSample(openvdb::Vec3R(
//             (p[0] - box_min_x) * xScaleFactor - xTransFactor + min.x(),
//             (p[1] - box_min_y) * yScaleFactor - yTransFactor + min.y(),
//             (p[2] - box_min_z) * zScaleFactor - zTransFactor + min.z()));
// #endif

// #if CLAMP_VOLUMES
//         if (retargetDensities) {
//             sampledValue /= maxDensity;
//         }

//         if (clampDensities && sampledValue > 1.0) {
//             // std::cout << "WHOOP" << std::endl;
//             sampledValue = 1.0;
//         }
// #endif

//         // if (sampledValue > 1.0) std::cout << "WHOOP" << std::endl;

//         return Spectrum(sampledValue);
//     }
// }

// Spectrum VDB_DensityFunction::D_Debug(const Point3f& p, bool buildState)
// const {
//     nDensityCalls++;

//     openvdb::tools::GridSampler<openvdb::FloatGrid,
//     openvdb::tools::BoxSampler>
//         sampler(*grid);

//     Float sampledValue;

//     Point3i coord = Point3i(
//         floor((p[0] - box_min_x) * xScaleFactor - xTransFactor + min.x()),
//         floor((p[2] - box_min_y) * yScaleFactor - yTransFactor + min.y()),
//         floor((p[1] - box_min_z) * zScaleFactor - zTransFactor + min.z()));

//     std::cout << "D_COORD: " << coord[0] << ", " << coord[1] << ", " <<
//     coord[2]
//               << std::endl;

//     sampledValue = D(coord)[0];

//     return Spectrum(sampledValue);
// }

// Spectrum VDB_DensityFunction::D(const Point3i& p) const {
//     openvdb::tools::GridSampler<openvdb::FloatGrid,
//                                 openvdb::tools::PointSampler>
//         sampler(*grid);

//     Float indexValue =
//         (Float)sampler.isSample(openvdb::Vec3R(p[0], p[1], p[2]));

// #if CLAMP_VOLUMES
//     if (clampDensities && indexValue > 1.0) {
//         // std::cout << "WHOOP" << std::endl;
//         indexValue = 1.0;
//     }
// #endif

//     return Spectrum(indexValue);
// }

// Spectrum VDB_DensityFunction::getMaxDensity() const {
//     if (clampDensities || retargetDensities) {
//         return Spectrum(1.0);
//     }

//     return Spectrum(maxDensity);
// }

// Spectrum VDB_DensityFunction::getMinDensity() const { return Spectrum(0.0); }

// Spectrum VDB_DensityFunction::getMaxDensityInRange(const Point3f& a,
//                                                    const Point3f& b) const {
//     // TODO
//     return Spectrum(1.0);
// }

// Spectrum VDB_DensityFunction::getMinDensityInRange(const Point3f& a,
//                                                    const Point3f& b) const {
//     // TODO
//     return Spectrum(1.0);
// }

// // TODO: Create a spectral form of this method
// // NOTE: the points passed in are in vdb space
// Spectrum VDB_DensityFunction::getMaxDensityInCube(const Point3i& a,
//                                                   const Point3i& b) const {
//     Float max_density = getMinDensity()[0];

//     openvdb::tools::GridSampler<openvdb::FloatGrid,
//                                 openvdb::tools::PointSampler>
//         sampler(*grid);

//     for (int k = a[2]; k <= b[2]; k++) {
//         for (int i = a[1]; i <= b[1]; i++) {
//             for (int j = a[0]; j <= b[0]; j++) {
//                 Float sampledValue =
//                     (Float)sampler.isSample(openvdb::Coord(j, i, k));

//                 if (sampledValue > max_density) {
//                     max_density = sampledValue;
//                 }
//             }
//         }
//     }

//     if (retargetDensities) {
//         max_density /= maxDensity;
//     }

//     if (clampDensities && max_density > 1.0) {
//         std::cout << "OHHHH NOOOOO" << std::endl;
//         max_density = 1.0;
//     }

//     return Spectrum(max_density);
// }

// }  // namespace pbrt

// #endif
