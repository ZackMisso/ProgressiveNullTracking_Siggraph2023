// #pragma once

// #include "densityfunc.h"

// #if OPENVDB

// // Note:: Openvdb needs to be installed on your machine for this to work
// // openvdb includes
// #include <openvdb/openvdb.h>
// #include <openvdb/tools/ChangeBackground.h>

// namespace pbrt {

// class VDB_DensityFunction : public DensityFunction {
//   public:
//     VDB_DensityFunction(openvdb::FloatGrid::Ptr grid);
//     VDB_DensityFunction(std::string filename);
//     ~VDB_DensityFunction();

//     virtual Spectrum D(const Point3f& p, bool buildState = false) const;
//     virtual Spectrum D_Debug(const Point3f& p, bool buildState = false)
//     const; Spectrum D(const Point3i& p) const;

//     virtual Spectrum getMaxDensity() const;
//     virtual Spectrum getMinDensity() const;

//     virtual Spectrum getMaxDensityInRange(const Point3f& a,
//                                           const Point3f& b) const;

//     virtual Spectrum getMinDensityInRange(const Point3f& a,
//                                           const Point3f& b) const;

//     virtual Spectrum getMaxDensityInCube(const Point3i& a,
//                                          const Point3i& b) const;

//     virtual bool requiresLocalSpace() const { return true; }

//     void preProcess();

//     void setWorldSpace(bool param) { worldSpace = param; }
//     void setClampDensities(bool param) { clampDensities = param; }
//     void setRetargetDensities(bool param) { retargetDensities = param; }
//     void setRetainAspectRatio(bool param) { retainAspectRatio = param; }

//     void setBox_Min_X(Float param) { box_min_x = param; }
//     void setBox_Min_Y(Float param) { box_min_y = param; }
//     void setBox_Min_Z(Float param) { box_min_z = param; }
//     void setBox_Max_X(Float param) { box_max_x = param; }
//     void setBox_Max_Y(Float param) { box_max_y = param; }
//     void setBox_Max_Z(Float param) { box_max_z = param; }

//     Float getBox_Min_X() const { return box_min_x; }
//     Float getBox_Min_Y() const { return box_min_y; }
//     Float getBox_Min_Z() const { return box_min_z; }
//     Float getBox_Max_X() const { return box_max_x; }
//     Float getBox_Max_Y() const { return box_max_y; }
//     Float getBox_Max_Z() const { return box_max_z; }

//     openvdb::Coord getMin() const { return min; }
//     openvdb::Coord getMax() const { return max; }

//     virtual Point3i getMinDimensions() const {
//         return Point3i(min[0], min[1], min[2]);
//     }
//     virtual Point3i getMaxDimensions() const {
//         return Point3i(max[0], max[1], max[2]);
//     }

//     bool getWorldSpace() const { return worldSpace; }
//     bool getClampDensities() const { return clampDensities; }
//     bool getRetargetDensities() const { return retargetDensities; }
//     bool getRetainAspectRation() const { return retainAspectRatio; }

//     // these methods are used for macro grid majorant operations
//     openvdb::Vec3R getMinLocalSpaceCoord() const;
//     openvdb::Vec3R getMaxLocalSpaceCoord() const;
//     // openvdb::Vec3R localToIndexSpace(const Point3f& p) const;

//   protected:
//     openvdb::GridBase::Ptr
//         baseGrid;  // this is here to correctly clear memory after rendering
//         in
//                    // case multiple renders are ran through pbrt api calls
//     openvdb::FloatGrid::Ptr grid;

//     // meta-settings
//     bool worldSpace;      // if this density function should be applied in
//     world
//                           // space
//     bool clampDensities;  // clamp all density values to 1
//     bool retargetDensities;  // rescale density values so the max is 1
//     bool retainAspectRatio;  // scale index space to local space by only
//     mapping
//                              // largest whd dimension instead all of them

//     openvdb::Coord min;
//     openvdb::Coord max;

//     Float box_min_x;
//     Float box_min_y;
//     Float box_min_z;

//     Float box_max_x;
//     Float box_max_y;
//     Float box_max_z;

//     Float xScaleFactor;
//     Float yScaleFactor;
//     Float zScaleFactor;

//     Float xTransFactor;
//     Float yTransFactor;
//     Float zTransFactor;

//     Float maxDensity;
// };

// }  // namespace pbrt

// #endif
