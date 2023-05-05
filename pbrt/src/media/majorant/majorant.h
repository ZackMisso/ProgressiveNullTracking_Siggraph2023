#pragma once

// this file contains the definitions of all of the different
// majorant functions

#include "../density/densityfunc.h"

// TODO: fix macro grid majorants

namespace pbrt {

struct MacroGridQuery {
    MacroGridQuery()
        : a(Point3f(0.0, 0.0, 0.0)), b(Point3f(1.0, 1.0, 1.0)), maj(1.0) {}

    MacroGridQuery(const MacroGridQuery& other) {
        a = other.a;
        b = other.b;
        maj = other.maj;
        t_start = other.t_start;
        t_end = other.t_end;
        macro_index = other.macro_index;
    }

    MacroGridQuery(const Point3f a, const Point3f b) : a(a), b(b), maj(1.0) {}

    MacroGridQuery(const Point3f a, const Point3f b, Float maj)
        : a(a), b(b), maj(maj) {}

    MacroGridQuery(const Point3f a, const Point3f b, Float t_start, Float t_end)
        : a(a), b(b), t_start(t_start), t_end(t_end) {}

    MacroGridQuery(const Point3f a, const Point3f b, Float maj, Float t_start,
                   Float t_end)
        : a(a), b(b), maj(maj), t_start(t_start), t_end(t_end) {}

    MacroGridQuery(const Point3f a, const Point3f b, Point3i ind, Float maj,
                   Float t_start, Float t_end)
        : a(a),
          b(b),
          macro_index(ind),
          maj(maj),
          t_start(t_start),
          t_end(t_end) {}

    Point3f a;
    Point3f b;
    Point3i macro_index;
    Float maj;
    Float t_start;
    Float t_end;
};

struct MacroGridFunction {
    MacroGridFunction() {
        non_empty = std::vector<MacroGridQuery>();
        lengths = std::vector<Float>();
    }

    MacroGridFunction(const std::vector<MacroGridQuery>& queries, Float tMin,
                      Float tMax)
        : tMin(tMin), tMax(tMax) {
        cull(queries);
    }

    MacroGridFunction(Float tMin, Float tMax) : tMin(tMin), tMax(tMax) {
        actual_tMax = tMax;
        actual_tMin = tMin;
        lengths.push_back(tMax - tMin);
        non_empty.push_back(
            MacroGridQuery(Point3f(), Point3f(), 1.0, tMin, tMax));
    }

    void cull(const std::vector<MacroGridQuery>& queries) {
        // if (queries.size() == 1 && queries[0].maj > 0.0)
        // {
        //     tMin = queries[0].t_start;
        //     tMax = queries[0].t_end;
        //     lengths.push_back(tMax - tMin);
        //     non_empty.push_back(queries[0]);
        //     return;
        // }

        if (queries.size() == 0) {
            Float tmp = tMin;
            tMin = tMax;
            tMax = tmp;
        }

        for (int i = 0; i < queries.size(); ++i) {
            if (queries[i].maj > 0.0) {
                non_empty.push_back(queries[i]);
            }
        }

        // stores the original bounds
        Float real_tMin = tMin;
        Float real_tMax = tMax;

        // computes the original min and max after boundry culling
        Float actual_tMin =
            non_empty[0].t_start * (real_tMax - real_tMin) + real_tMin;
        Float actual_tMax =
            non_empty[non_empty.size() - 1].t_end * (real_tMax - real_tMin) +
            real_tMin;

        // computes the new total distance, and the individual lengths
        Float t_total = 0.0;
        lengths = std::vector<Float>();
        for (int i = 0; i < non_empty.size(); ++i) {
            non_empty[i].t_start =
                non_empty[i].t_start * (real_tMax - real_tMin) + real_tMin;
            non_empty[i].t_end =
                non_empty[i].t_end * (real_tMax - real_tMin) + real_tMin;
            lengths.push_back(non_empty[i].t_end - non_empty[i].t_start);
            t_total += non_empty[i].t_end - non_empty[i].t_start;
        }

        // resets the tMin and tMax
        tMin = 0.0;
        tMax = t_total;
    }

    Float unmap(Float local_t) const {
        if (lengths.size() == 0) return tMax;
        // if (lengths.size() == 1) return local_t

        Float start = 0.0;
        Float prev = start;
        int index = 0;

        while (true) {
            prev = start;
            start += lengths[index];

            if (start > local_t) break;

            index++;
            if (index >= lengths.size()) break;
        }

        // std::cout << "index: " << index << std::endl;

        if (index >= lengths.size()) {
            // maybe extra stuff is not necessary but eh
            return tMax + 100.0;
        }

        // you will always over count by 1
        // index--;

        Float proxy = (local_t - prev) / (start - prev);
        // std::cout << "local_t: " << local_t << std::endl;
        // std::cout << "prev: " << prev << std::endl;
        // std::cout << "start: " << start << std::endl;
        // std::cout << "PROXY: " << proxy << std::endl;

        return (non_empty[index].t_end - non_empty[index].t_start) * proxy +
               non_empty[index].t_start;
    }

    void debug() const {
        for (int i = 0; i < non_empty.size(); ++i) {
            std::cout << i << std::endl;
            std::cout << "    start: " << non_empty[i].t_start << std::endl;
            std::cout << "    end: " << non_empty[i].t_end << std::endl;
            std::cout << "    maj: " << non_empty[i].maj << std::endl;
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::vector<MacroGridQuery> non_empty;
    std::vector<Float> lengths;
    Float tMin;
    Float tMax;

    Float actual_tMin;
    Float actual_tMax;
};

class MajorantFunction {
  public:
    MajorantFunction(const DensityFunction* density) : density_func(density) {}

    virtual ~MajorantFunction() {}

    virtual Float Maj(const Point3f& pa, const Point3f& pb) const = 0;
    virtual Float Maj(const Point3f& p) const { return 0.0; }

    // this method is used for deciding how to traverse the macro grid
    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb) const {
        std::vector<MacroGridQuery> queries = std::vector<MacroGridQuery>();
        // std::cout << "inside here" << std::endl;

        queries.push_back(MacroGridQuery(pa, pb, Maj(pa, pb)));

        return queries;
    }

    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(const Point3f& pa,
                                                            const Point3f& pb,
                                                            Float tMin,
                                                            Float tMax) const {
        std::vector<MacroGridQuery> queries = std::vector<MacroGridQuery>();

        queries.push_back(MacroGridQuery(pa, pb, Maj(pa, pb), tMin, tMax));

        return queries;
    }

    virtual std::vector<MacroGridQuery> MacroVoxelTraversal_DEBUG(
        const Point3f& pa, const Point3f& pb, Float tMin, Float tMax) const {
        return MacroVoxelTraversal(pa, pb, tMin, tMax);
    }

    virtual bool Uses_Macro_Traversal() const { return false; }

    virtual void UpdateMajorant(const Point3f& p, Float density){
        // does nothing for now
    };

    virtual void RenderVerticalSlice(std::string filename, int ps) const {
        // does nothing for now
    };

    virtual void debug(Point3f p) const {
        // TODO
    }

    void setScale(Float param) { scale = param; }

  protected:
    const DensityFunction* density_func;
    Float scale;
};

class ConstBoundedMajorant : public MajorantFunction {
  public:
    ConstBoundedMajorant(const DensityFunction* density);

    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;

    Float maxDensity;
};

class ConstMacroMajorant : public MajorantFunction {
  public:
    ConstMacroMajorant(const DensityFunction* density, int w, int h, int d);

    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;

    void initialize(const DensityFunction* density);

    Float* macroDense;
    int width;
    int height;
    int depth;
};

class ConstAdaptiveMajorant : public MajorantFunction {
  public:
    ConstAdaptiveMajorant(const DensityFunction* density);

    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual void UpdateMajorant(const Point3f& p, Float density);

    Float maxDensity;
};

// this data structure is a naive cube
class IntervalAdaptiveMajorant : public MajorantFunction {
  public:
    IntervalAdaptiveMajorant(const DensityFunction* density, int dim);
    ~IntervalAdaptiveMajorant();

    void initialize(const DensityFunction* density);
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;
    virtual void UpdateMajorant(const Point3f& p, Float density);
    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb, Float baseTMin,
        Float baseTMax) const;
    void setMajVoxel(int x, int y, int z, Float maj);
    Float getMajVoxel(int x, int y, int z) const;

    Float* majGrid;
    int dim;
    Float delta;  // added on to the found max density while traversing
};

// this data structure is a naive cube that is initialized by sampling the
// density
class IntervalAdaptiveDensityMajorant : public MajorantFunction {
  public:
    IntervalAdaptiveDensityMajorant(const DensityFunction* density, int dim);
    ~IntervalAdaptiveDensityMajorant();

    void initialize(const DensityFunction* density);
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;
    virtual void UpdateMajorant(const Point3f& p, Float density);
    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb, Float baseTMin,
        Float baseTMax) const;
    virtual std::vector<MacroGridQuery> MacroVoxelTraversal_DEBUG(
        const Point3f& pa, const Point3f& pb, Float tMin, Float tMax) const;
    void setMajVoxel(int x, int y, int z, Float maj);
    Float getMajVoxel(int x, int y, int z) const;
    virtual void debug(Point3f p) const;

    Float* majGrid;
    int dim;
    Float delta;  // added on to the found max density while traversing
};

// this data structure is initialized directly from a density grid
class IntervalAdaptiveDensityVDB : public MajorantFunction {
  public:
    IntervalAdaptiveDensityVDB(const DensityFunction* density, int dim);
    ~IntervalAdaptiveDensityVDB();

    void initialize(const DensityFunction* density);
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;

    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb, Float baseTMin,
        Float baseTMax) const;

    Float getMajVoxel(int x, int y, int z) const;

    virtual void UpdateMajorant(const Point3f& p, Float density);
    void setMajVoxel(int x, int y, int z, Float maj);

    virtual void debug(Point3f p) const;

    virtual void RenderVerticalSlice(std::string filename, int ps) const;

    Float* majGrid;
    int dim;
    Float delta;  // added on to the found max density while traversing
};

// this data structure is initialized directly from a density grid
class IntervalAdaptiveDensityVDBTracking : public MajorantFunction {
  public:
    IntervalAdaptiveDensityVDBTracking(const DensityFunction* density, int dim);
    ~IntervalAdaptiveDensityVDBTracking();

    void initialize(const DensityFunction* density);
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;

    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb, Float baseTMin,
        Float baseTMax) const;

    Float getMajVoxel(int x, int y, int z) const;
    Float getGTMajVoxel(int x, int y, int z) const;

    virtual void UpdateMajorant(const Point3f& p, Float density);
    void setMajVoxel(int x, int y, int z, Float maj);
    void setGTMajVoxel(int x, int y, int z, Float maj);

    virtual void debug(Point3f p) const;

    virtual void RenderVerticalSlice(std::string filename, int ps) const;

    Float* gtGrid;
    Float* majGrid;
    int dim;
    Float delta;        // added on to the found max density while traversing
    Float start_dense;  // the starting density per voxel
};

// this data structure is initialized directly from a density grid
class IntervalAdaptiveDensityVDBTight : public MajorantFunction {
  public:
    IntervalAdaptiveDensityVDBTight(const DensityFunction* density, int dim);
    ~IntervalAdaptiveDensityVDBTight();

    void initialize(const DensityFunction* density);
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;

    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb, Float baseTMin,
        Float baseTMax) const;

    Float getMajVoxel(int x, int y, int z) const;
    Float getGTMajVoxel(int x, int y, int z) const;

    virtual void UpdateMajorant(const Point3f& p, Float density);
    void setMajVoxel(int x, int y, int z, Float maj);
    void setGTMajVoxel(int x, int y, int z, Float maj);

    virtual void debug(Point3f p) const;

    virtual void RenderVerticalSlice(std::string filename, int ps) const;

    Float* gtGrid;
    Float* majGrid;
    int dim;
    Float delta;        // added on to the found max density while traversing
    Float start_dense;  // the starting density per voxel
};

// this data structure is initialized directly from a density grid
class IntervalAdaptiveDensityVDBHardcode : public MajorantFunction {
  public:
    IntervalAdaptiveDensityVDBHardcode(const DensityFunction* density, int dim);
    ~IntervalAdaptiveDensityVDBHardcode();

    void initialize(const DensityFunction* density);
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;

    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb, Float baseTMin,
        Float baseTMax) const;

    Float getMajVoxel(int x, int y, int z) const;

    virtual void UpdateMajorant(const Point3f& p, Float density);
    void setMajVoxel(int x, int y, int z, Float maj);

    virtual void debug(Point3f p) const;

    Float* majGrid;
    int dim;
    Float delta;  // added on to the found max density while traversing
};

#if OPENVDB

class VDBMacroMajorant : public MajorantFunction {
  public:
    VDBMacroMajorant(const DensityFunction* density, int w, int h, int d);

    ~VDBMacroMajorant();

    virtual Point3i Maj_Ind(const Point3f& p) const;
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;
    Float Maj(const Point3i& p) const;
    // Float SingleMaj(const Point3i& p) const;
    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb) const;

    void initialize(const DensityFunction* density);

    virtual bool Uses_Macro_Traversal() const { return true; }

    bool is_inside(Point3f p) const;

    // TODO: maybe store this in a VDB Grid
    // DensityFunction* density_ref;
    Float* macroDense;
    int width;
    int height;
    int depth;

    Float box_min_x;
    Float box_min_y;
    Float box_min_z;
    Float box_max_x;
    Float box_max_y;
    Float box_max_z;
};

// this data structure is initialized directly from a density grid
// it sets the majorant = partial * real_density so that the majorant is
// always non-bounding for visualization purposes.
class IntervalAdaptiveDensityVDBPartial : public MajorantFunction {
  public:
    IntervalAdaptiveDensityVDBPartial(const DensityFunction* density, int dim);
    ~IntervalAdaptiveDensityVDBPartial();

    void initialize(const DensityFunction* density);
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;

    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb, Float baseTMin,
        Float baseTMax) const;

    Float getMajVoxel(int x, int y, int z) const;

    virtual void UpdateMajorant(const Point3f& p, Float density);
    void setMajVoxel(int x, int y, int z, Float maj);

    virtual void debug(Point3f p) const;

    virtual void RenderVerticalSlice(std::string filename, int ps) const;

    Float* majGrid;
    int dim;
    Float partial;
};

class IntervalAdaptiveDensityVDBConstant : public MajorantFunction {
  public:
    IntervalAdaptiveDensityVDBConstant(const DensityFunction* density,
                                       Float global_maj, int dim);
    ~IntervalAdaptiveDensityVDBConstant();

    void initialize(const DensityFunction* density);
    virtual Float Maj(const Point3f& pa, const Point3f& pb) const;
    virtual Float Maj(const Point3f& p) const;

    virtual std::vector<MacroGridQuery> MacroVoxelTraversal(
        const Point3f& pa, const Point3f& pb, Float baseTMin,
        Float baseTMax) const;

    Float getMajVoxel(int x, int y, int z) const;

    virtual void UpdateMajorant(const Point3f& p, Float density);
    void setMajVoxel(int x, int y, int z, Float maj);

    virtual void debug(Point3f p) const;

    virtual void RenderVerticalSlice(std::string filename, int ps) const;

    Float* majGrid;
    Float global_maj;
    int dim;
};

#endif

}  // namespace pbrt
