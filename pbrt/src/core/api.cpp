
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

// core/api.cpp*
#include "api.h"

#include "film.h"
#include "medium.h"
#include "parallel.h"
#include "paramset.h"
#include "scene.h"
#include "spectrum.h"
#include "stats.h"

// API Additional Headers
#include "accelerators/bvh.h"
#include "accelerators/kdtreeaccel.h"
#include "cameras/environment.h"
#include "cameras/orthographic.h"
#include "cameras/perspective.h"
#include "cameras/realistic.h"
#include "filters/box.h"
#include "filters/gaussian.h"
#include "filters/mitchell.h"
#include "filters/sinc.h"
#include "filters/triangle.h"
#include "integrators/ao.h"
#include "integrators/bdpt.h"
#include "integrators/directlighting.h"
#include "integrators/majorant.h"
#include "integrators/mlt.h"
#include "integrators/normals.h"
#include "integrators/path.h"
#include "integrators/path_blanchet.h"
#include "integrators/sppm.h"
#include "integrators/volpath.h"
#include "integrators/volpath_debias.h"
#include "integrators/volpath_nonexp.h"
#include "integrators/volpath_nonexp_debias.h"
#include "integrators/whitted.h"
#include "integrators/whitted_blanchet.h"
#include "lights/diffuse.h"
#include "lights/distant.h"
#include "lights/distant_angle.h"
#include "lights/distant_angle_prog_cor.h"
#include "lights/goniometric.h"
#include "lights/infinite.h"
#include "lights/point.h"
#include "lights/point_cor_p.h"
#include "lights/projection.h"
#include "lights/spot.h"
#include "materials/disney.h"
#include "materials/fourier.h"
#include "materials/glass.h"
#include "materials/glass_hack.h"
#include "materials/hair.h"
#include "materials/kdsubsurface.h"
#include "materials/matte.h"
#include "materials/metal.h"
#include "materials/metal_cor_rough.h"
#include "materials/metal_cor_rough_debiased.h"
#include "materials/metal_cor_rough_debiased_center.h"
#include "materials/mirror.h"
#include "materials/mixmat.h"
#include "materials/plastic.h"
#include "materials/refl_matte.h"
#include "materials/substrate.h"
#include "materials/subsurface.h"
#include "materials/translucent.h"
#include "materials/uber.h"
#include "media/heterogeneous.h"
#include "media/heterogeneous_nonexp.h"
#include "media/homogeneous.h"
#include "media/majorant/majorant.h"
#include "media/minorant.h"
#include "samplers/halton.h"
#include "samplers/maxmin.h"
#include "samplers/random.h"
#include "samplers/sobol.h"
#include "samplers/stratified.h"
#include "samplers/zerotwosequence.h"
#include "shapes/cone.h"
#include "shapes/curve.h"
#include "shapes/cylinder.h"
#include "shapes/disk.h"
#include "shapes/heightfield.h"
#include "shapes/hyperboloid.h"
#include "shapes/loopsubdiv.h"
#include "shapes/nurbs.h"
#include "shapes/paraboloid.h"
#include "shapes/plymesh.h"
#include "shapes/sphere.h"
#include "shapes/triangle.h"
#include "textures/bilerp.h"
#include "textures/checkerboard.h"
#include "textures/constant.h"
#include "textures/dots.h"
#include "textures/fbm.h"
#include "textures/imagemap.h"
#include "textures/marble.h"
#include "textures/mix.h"
#include "textures/ptex.h"
#include "textures/scale.h"
#include "textures/uv.h"
#include "textures/windy.h"
#include "textures/wrinkled.h"
// #include "media/majorant/const_bounded."

#include "media/density/const_density.h"
#include "media/density/densityfunc.h"
#include "media/density/linear_density.h"
#include "media/density/proc_sphere_density.h"
#include "media/density/simple_disk_density.h"
#include "media/density/simple_sphere_density.h"
#include "media/density/texture_density.h"
#if OPENVDB
#include "media/density/vdb_density.h"
#endif

#include <stdio.h>

#include <map>

#include "media/ff_samplers/ff_absorptive.h"
#include "media/ff_samplers/ff_analytic.h"
#include "media/ff_samplers/ff_const.h"
#include "media/ff_samplers/ff_delta.h"
#include "media/ff_samplers/ff_delta_adap.h"
#include "media/ff_samplers/ff_delta_weight.h"
#include "media/ff_samplers/ff_delta_weight_signif.h"
#include "media/ff_samplers/ff_delta_weight_slight.h"
#include "media/ff_samplers/ff_rat.h"
#include "media/ff_samplers/ff_rat_adap.h"
#include "media/ff_samplers/ff_raymarch.h"
#include "media/trans_ests/analytic.h"
#include "media/trans_ests/const_trans.h"
#include "media/trans_ests/davis_pink_tay.h"
#include "media/trans_ests/debiased_raymarch.h"
#include "media/trans_ests/exp_nvidia.h"
#include "media/trans_ests/next_flight_ratio.h"
#include "media/trans_ests/pink_pcume.h"
#include "media/trans_ests/pink_pcume_exp.h"
#include "media/trans_ests/pink_taylor_ray.h"
#include "media/trans_ests/pink_taylor_ray_exp.h"
#include "media/trans_ests/pseries_cdf.h"
#include "media/trans_ests/pseries_cumulative.h"
#include "media/trans_ests/pseries_ratio.h"
#include "media/trans_ests/ratio.h"
#include "media/trans_ests/ratio_adap.h"
#include "media/trans_ests/ratio_pink.h"
#include "media/trans_ests/ratio_signif.h"
#include "media/trans_ests/ratio_slight.h"
#include "media/trans_ests/ray_march.h"
#include "media/trans_ests/ray_march_nonexp.h"  // TODO: delete thi
#include "media/trans_ests/track_length.h"
#include "media/trans_ests/track_length_adap.h"
#include "media/trans_funcs/davis.h"
#include "media/trans_funcs/davis_weinstein.h"
#include "media/trans_funcs/exp.h"
#include "media/trans_funcs/exp_cub.h"
#include "media/trans_funcs/exp_sqr.h"
#include "media/trans_funcs/lin.h"
#include "procedural_media/chess_media.h"
#include "procedural_media/cloud_media.h"
#include "procedural_media/deintegrated_texture.h"
#include "procedural_media/point_interp_media.h"
#include "procedural_media/voronoi_spiral_media.h"

namespace pbrt {

// API Global Variables
Options PbrtOptions;

// API Local Classes
PBRT_CONSTEXPR int MaxTransforms = 2;
PBRT_CONSTEXPR int StartTransformBits = 1 << 0;
PBRT_CONSTEXPR int EndTransformBits = 1 << 1;
PBRT_CONSTEXPR int AllTransformsBits = (1 << MaxTransforms) - 1;
struct TransformSet {
    // TransformSet Public Methods
    Transform &operator[](int i) {
#if LOGGING
        CHECK_GE(i, 0);
        CHECK_LT(i, MaxTransforms);
#endif
        return t[i];
    }
    const Transform &operator[](int i) const {
#if LOGGING
        CHECK_GE(i, 0);
        CHECK_LT(i, MaxTransforms);
#endif
        return t[i];
    }
    friend TransformSet Inverse(const TransformSet &ts) {
        TransformSet tInv;
        for (int i = 0; i < MaxTransforms; ++i) tInv.t[i] = Inverse(ts.t[i]);
        return tInv;
    }
    bool IsAnimated() const {
        for (int i = 0; i < MaxTransforms - 1; ++i)
            if (t[i] != t[i + 1]) return true;
        return false;
    }

  private:
    Transform t[MaxTransforms];
};

struct RenderOptions {
    // RenderOptions Public Methods
    Integrator *MakeIntegrator() const;
    Scene *MakeScene();
    Camera *MakeCamera() const;

    // RenderOptions Public Data
    Float transformStartTime = 0, transformEndTime = 1;
    std::string FilterName = "gaussian";
    ParamSet FilterParams;
    std::string FilmName = "image";
    ParamSet FilmParams;
    std::string SamplerName = "halton";
    ParamSet SamplerParams;
    std::string AcceleratorName = "bvh";
    ParamSet AcceleratorParams;
    std::string IntegratorName = "path";
    ParamSet IntegratorParams;
    std::string CameraName = "perspective";
    ParamSet CameraParams;
    TransformSet CameraToWorld;
    std::map<std::string, std::shared_ptr<Medium>> namedMedia;
    std::vector<std::shared_ptr<Medium>> all_media;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Primitive>> primitives;
    std::map<std::string, std::vector<std::shared_ptr<Primitive>>> instances;
    std::vector<std::shared_ptr<Primitive>> *currentInstance = nullptr;
    bool haveScatteringMedia = false;
};

// MaterialInstance represents both an instance of a material as well as
// the information required to create another instance of it (possibly with
// different parameters from the shape).
struct MaterialInstance {
    MaterialInstance() = default;
    MaterialInstance(const std::string &name,
                     const std::shared_ptr<Material> &mtl, ParamSet params)
        : name(name), material(mtl), params(std::move(params)) {}

    std::string name;
    std::shared_ptr<Material> material;
    ParamSet params;
};

struct GraphicsState {
    // Graphics State Methods
    GraphicsState()
        : floatTextures(std::make_shared<FloatTextureMap>()),
          spectrumTextures(std::make_shared<SpectrumTextureMap>()),
          namedMaterials(std::make_shared<NamedMaterialMap>()) {
        ParamSet empty;
        TextureParams tp(empty, empty, *floatTextures, *spectrumTextures);
        std::shared_ptr<Material> mtl(CreateMatteMaterial(tp));
        currentMaterial =
            std::make_shared<MaterialInstance>("matte", mtl, ParamSet());
    }
    std::shared_ptr<Material> GetMaterialForShape(const ParamSet &geomParams);
    MediumInterface CreateMediumInterface();

    // Graphics State
    std::string currentInsideMedium, currentOutsideMedium;

    // Updated after book publication: floatTextures, spectrumTextures, and
    // namedMaterials are all implemented using a "copy on write" approach
    // for more efficient GraphicsState management.  When state is pushed
    // in pbrtAttributeBegin(), we don't immediately make a copy of these
    // maps, but instead record that each one is shared.  Only if an item
    // is added to one is a unique copy actually made.
    using FloatTextureMap =
        std::map<std::string, std::shared_ptr<Texture<Float>>>;
    std::shared_ptr<FloatTextureMap> floatTextures;
    bool floatTexturesShared = false;

    using SpectrumTextureMap =
        std::map<std::string, std::shared_ptr<Texture<Spectrum>>>;
    std::shared_ptr<SpectrumTextureMap> spectrumTextures;
    bool spectrumTexturesShared = false;

    using NamedMaterialMap =
        std::map<std::string, std::shared_ptr<MaterialInstance>>;
    std::shared_ptr<NamedMaterialMap> namedMaterials;
    bool namedMaterialsShared = false;

    std::shared_ptr<MaterialInstance> currentMaterial;
    ParamSet areaLightParams;
    std::string areaLight;
    bool reverseOrientation = false;
};

STAT_MEMORY_COUNTER("Memory/TransformCache", transformCacheBytes);
STAT_PERCENT("Scene/TransformCache hits", nTransformCacheHits,
             nTransformCacheLookups);
STAT_INT_DISTRIBUTION("Scene/Probes per TransformCache lookup",
                      transformCacheProbes);

// Note: TransformCache has been reimplemented and has a slightly different
// interface compared to the version described in the third edition of
// Physically Based Rendering.  The new version is more efficient in both
// space and memory, which is helpful for highly complex scenes.
//
// The new implementation uses a hash table to store Transforms (rather
// than a std::map, which generally uses a red-black tree).  Further,
// it doesn't always store the inverse of the transform; if a caller
// wants the inverse as well, they are responsible for storing it.
//
// The hash table size is always a power of two, allowing for the use of a
// bitwise AND to turn hash values into table offsets.  Quadratic probing
// is used when there is a hash collision.
class TransformCache {
  public:
    TransformCache() : hashTable(512), hashTableOccupancy(0) {}

    // TransformCache Public Methods
    Transform *Lookup(const Transform &t) {
        ++nTransformCacheLookups;

        int offset = Hash(t) & (hashTable.size() - 1);
        int step = 1;
        while (true) {
            // Keep looking until we find the Transform or determine that
            // it's not present.
            if (!hashTable[offset] || *hashTable[offset] == t) break;
            // Advance using quadratic probing.
            offset = (offset + step * step) & (hashTable.size() - 1);
            ++step;
        }
        ReportValue(transformCacheProbes, step);
        Transform *tCached = hashTable[offset];
        if (tCached)
            ++nTransformCacheHits;
        else {
            tCached = arena.Alloc<Transform>();
            *tCached = t;
            Insert(tCached);
        }
        return tCached;
    }

    void Clear() {
        transformCacheBytes +=
            arena.TotalAllocated() + hashTable.size() * sizeof(Transform *);
        hashTable.resize(512);
        hashTable.clear();
        // TODO :: submit bug report to pbrt
        hashTable = std::vector<Transform *>(512);
        hashTableOccupancy = 0;
        arena.Reset();
    }

  private:
    void Insert(Transform *tNew);
    void Grow();

    static uint64_t Hash(const Transform &t) {
        const char *ptr = (const char *)(&t.GetMatrix());
        size_t size = sizeof(Matrix4x4);
        uint64_t hash = 14695981039346656037ull;
        while (size > 0) {
            hash ^= *ptr;
            hash *= 1099511628211ull;
            ++ptr;
            --size;
        }
        return hash;
    }

    // TransformCache Private Data
    std::vector<Transform *> hashTable;
    int hashTableOccupancy;
    MemoryArena arena;
};

void TransformCache::Insert(Transform *tNew) {
    if (++hashTableOccupancy == hashTable.size() / 2) Grow();

    int offset = Hash(*tNew) & (hashTable.size() - 1);
    int step = 1;
    while (true) {
        if (hashTable[offset] == nullptr) {
            hashTable[offset] = tNew;
            return;
        }
        // Advance using quadratic probing.
        offset = (offset + step * step) & (hashTable.size() - 1);
        ++step;
    }
}

void TransformCache::Grow() {
    std::vector<Transform *> newTable(2 * hashTable.size());

#if LOGGING
    LOG(INFO) << "Growing transform cache hash table to " << newTable.size();
#endif

    // Insert current elements into newTable.
    for (Transform *tEntry : hashTable) {
        if (!tEntry) continue;

        int offset = Hash(*tEntry) & (newTable.size() - 1);
        int step = 1;
        while (true) {
            if (newTable[offset] == nullptr) {
                newTable[offset] = tEntry;
                break;
            }
            // Advance using quadratic probing.
            offset = (offset + step * step) & (hashTable.size() - 1);
            ++step;
        }
    }

    std::swap(hashTable, newTable);
}

// API Static Data
enum class APIState { Uninitialized, OptionsBlock, WorldBlock };
static APIState currentApiState = APIState::Uninitialized;
static TransformSet curTransform;
static uint32_t activeTransformBits = AllTransformsBits;
static std::map<std::string, TransformSet> namedCoordinateSystems;
static std::unique_ptr<RenderOptions> renderOptions;
static GraphicsState graphicsState;
static std::vector<GraphicsState> pushedGraphicsStates;
static std::vector<TransformSet> pushedTransforms;
static std::vector<uint32_t> pushedActiveTransformBits;
static TransformCache transformCache;
int catIndentCount = 0;

// API Forward Declarations
std::vector<std::shared_ptr<Shape>> MakeShapes(const std::string &name,
                                               const Transform *ObjectToWorld,
                                               const Transform *WorldToObject,
                                               bool reverseOrientation,
                                               const ParamSet &paramSet);

// API Macros
#define VERIFY_INITIALIZED(func)                           \
    if (!(PbrtOptions.cat || PbrtOptions.toPly) &&         \
        currentApiState == APIState::Uninitialized) {      \
        Error(                                             \
            "pbrtInit() must be before calling \"%s()\". " \
            "Ignoring.",                                   \
            func);                                         \
        return;                                            \
    } else /* swallow trailing semicolon */
#define VERIFY_OPTIONS(func)                             \
    VERIFY_INITIALIZED(func);                            \
    if (!(PbrtOptions.cat || PbrtOptions.toPly) &&       \
        currentApiState == APIState::WorldBlock) {       \
        Error(                                           \
            "Options cannot be set inside world block; " \
            "\"%s\" not allowed.  Ignoring.",            \
            func);                                       \
        return;                                          \
    } else /* swallow trailing semicolon */
#define VERIFY_WORLD(func)                                   \
    VERIFY_INITIALIZED(func);                                \
    if (!(PbrtOptions.cat || PbrtOptions.toPly) &&           \
        currentApiState == APIState::OptionsBlock) {         \
        Error(                                               \
            "Scene description must be inside world block; " \
            "\"%s\" not allowed. Ignoring.",                 \
            func);                                           \
        return;                                              \
    } else /* swallow trailing semicolon */
#define FOR_ACTIVE_TRANSFORMS(expr)           \
    for (int i = 0; i < MaxTransforms; ++i)   \
        if (activeTransformBits & (1 << i)) { \
            expr                              \
        }
#define WARN_IF_ANIMATED_TRANSFORM(func)                             \
    do {                                                             \
        if (curTransform.IsAnimated())                               \
            Warning(                                                 \
                "Animated transformations set; ignoring for \"%s\" " \
                "and using the start transform only",                \
                func);                                               \
    } while (false) /* swallow trailing semicolon */

// Object Creation Function Definitions
std::vector<std::shared_ptr<Shape>> MakeShapes(const std::string &name,
                                               const Transform *object2world,
                                               const Transform *world2object,
                                               bool reverseOrientation,
                                               const ParamSet &paramSet) {
    std::vector<std::shared_ptr<Shape>> shapes;
    std::shared_ptr<Shape> s;
    if (name == "sphere")
        s = CreateSphereShape(object2world, world2object, reverseOrientation,
                              paramSet);
    // Create remaining single _Shape_ types
    else if (name == "cylinder")
        s = CreateCylinderShape(object2world, world2object, reverseOrientation,
                                paramSet);
    else if (name == "disk")
        s = CreateDiskShape(object2world, world2object, reverseOrientation,
                            paramSet);
    else if (name == "cone")
        s = CreateConeShape(object2world, world2object, reverseOrientation,
                            paramSet);
    else if (name == "paraboloid")
        s = CreateParaboloidShape(object2world, world2object,
                                  reverseOrientation, paramSet);
    else if (name == "hyperboloid")
        s = CreateHyperboloidShape(object2world, world2object,
                                   reverseOrientation, paramSet);
    if (s != nullptr) shapes.push_back(s);

    // Create multiple-_Shape_ types
    else if (name == "curve")
        shapes = CreateCurveShape(object2world, world2object,
                                  reverseOrientation, paramSet);
    else if (name == "trianglemesh") {
        if (PbrtOptions.toPly) {
            int nvi;
            const int *vi = paramSet.FindInt("indices", &nvi);

            if (nvi < 500) {
                // It's a small mesh; don't bother with a PLY file after all.
                printf("%*sShape \"%s\" ", catIndentCount, "", name.c_str());
                paramSet.Print(catIndentCount);
                printf("\n");
            } else {
                static int count = 1;
                const char *plyPrefix =
                    getenv("PLY_PREFIX") ? getenv("PLY_PREFIX") : "mesh";
                std::string fn =
                    StringPrintf("%s_%05d.ply", plyPrefix, count++);

                int npi, nuvi, nsi, nni;
                const Point3f *P = paramSet.FindPoint3f("P", &npi);
                const Point2f *uvs = paramSet.FindPoint2f("uv", &nuvi);
                if (!uvs) uvs = paramSet.FindPoint2f("st", &nuvi);
                std::vector<Point2f> tempUVs;
                if (!uvs) {
                    const Float *fuv = paramSet.FindFloat("uv", &nuvi);
                    if (!fuv) fuv = paramSet.FindFloat("st", &nuvi);
                    if (fuv) {
                        nuvi /= 2;
                        tempUVs.reserve(nuvi);
                        for (int i = 0; i < nuvi; ++i)
                            tempUVs.push_back(
                                Point2f(fuv[2 * i], fuv[2 * i + 1]));
                        uvs = &tempUVs[0];
                    }
                }
                const Normal3f *N = paramSet.FindNormal3f("N", &nni);
                const Vector3f *S = paramSet.FindVector3f("S", &nsi);
                int nfi;
                const int *faceIndices = paramSet.FindInt("faceIndices", &nfi);

#if LOGGING
                if (faceIndices) {
                    CHECK_EQ(nfi, nvi / 3);
                }
#endif

                if (!WritePlyFile(fn.c_str(), nvi / 3, vi, npi, P, S, N, uvs,
                                  faceIndices))
                    Error("Unable to write PLY file \"%s\"", fn.c_str());

                ParamSet ps = paramSet;
                ps.EraseInt("indices");
                ps.ErasePoint3f("P");
                ps.ErasePoint2f("uv");
                ps.ErasePoint2f("st");
                ps.EraseNormal3f("N");
                ps.EraseVector3f("S");
                ps.EraseInt("faceIndices");

                printf("%*sShape \"plymesh\" \"string filename\" \"%s\" ",
                       catIndentCount, "", fn.c_str());
                ps.Print(catIndentCount);
                printf("\n");
            }
        } else
            shapes = CreateTriangleMeshShape(object2world, world2object,
                                             reverseOrientation, paramSet,
                                             &*graphicsState.floatTextures);
    } else if (name == "plymesh")
        shapes = CreatePLYMesh(object2world, world2object, reverseOrientation,
                               paramSet, &*graphicsState.floatTextures);
    else if (name == "heightfield")
        shapes = CreateHeightfield(object2world, world2object,
                                   reverseOrientation, paramSet);
    else if (name == "loopsubdiv")
        shapes = CreateLoopSubdiv(object2world, world2object,
                                  reverseOrientation, paramSet);
    else if (name == "nurbs")
        shapes = CreateNURBS(object2world, world2object, reverseOrientation,
                             paramSet);
    else
        Warning("Shape \"%s\" unknown.", name.c_str());
    return shapes;
}

STAT_COUNTER("Scene/Materials created", nMaterialsCreated);

std::shared_ptr<Material> MakeMaterial(const std::string &name,
                                       const TextureParams &mp) {
    Material *material = nullptr;
    // std::cout << "NAME:: " << name << std::endl;
    if (name == "" || name == "none")
        return nullptr;
    else if (name == "matte")
        material = CreateMatteMaterial(mp);
    else if (name == "refl_matte")
        material = CreateReflectiveMatteMaterial(mp);
    else if (name == "plastic")
        material = CreatePlasticMaterial(mp);
    else if (name == "translucent")
        material = CreateTranslucentMaterial(mp);
    else if (name == "glass")
        material = CreateGlassMaterial(mp);
    else if (name == "glass_hack")
        material = CreateGlass_Hack_Material(mp);
    else if (name == "mirror")
        material = CreateMirrorMaterial(mp);
    else if (name == "hair")
        material = CreateHairMaterial(mp);
    else if (name == "disney")
        material = CreateDisneyMaterial(mp);
    else if (name == "mix") {
        std::string m1 = mp.FindString("namedmaterial1", "glass");
        std::string m2 = mp.FindString("namedmaterial2", "metal");
        std::shared_ptr<Material> mat1, mat2;
        if (graphicsState.namedMaterials->find(m1) ==
            graphicsState.namedMaterials->end()) {
            Error("Named material \"%s\" undefined.  Using \"matte\"",
                  m1.c_str());
            mat1 = MakeMaterial("matte", mp);
        } else
            mat1 = (*graphicsState.namedMaterials)[m1]->material;

        if (graphicsState.namedMaterials->find(m2) ==
            graphicsState.namedMaterials->end()) {
            Error("Named material \"%s\" undefined.  Using \"matte\"",
                  m2.c_str());
            mat2 = MakeMaterial("matte", mp);
        } else
            mat2 = (*graphicsState.namedMaterials)[m2]->material;

        material = CreateMixMaterial(mp, mat1, mat2);
    } else if (name == "metal")
        material = CreateMetalMaterial(mp);
    else if (name == "metal_cor_rough") {
        material = CreateMetalCorRoughMaterial(mp);
    } else if (name == "metal_cor_rough_debiased") {
        material = CreateMetalCorRoughDebiasedMaterial(mp);
    } else if (name == "metal_cor_rough_debiased_center") {
        material = CreateMetalCorRoughDebiasedCenterMaterial(mp);
    } else if (name == "substrate")
        material = CreateSubstrateMaterial(mp);
    else if (name == "uber")
        material = CreateUberMaterial(mp);
    else if (name == "subsurface")
        material = CreateSubsurfaceMaterial(mp);
    else if (name == "kdsubsurface")
        material = CreateKdSubsurfaceMaterial(mp);
    else if (name == "fourier")
        material = CreateFourierMaterial(mp);
    else {
        Warning("Material \"%s\" unknown. Using \"matte\".", name.c_str());
        material = CreateMatteMaterial(mp);
    }

    if ((name == "subsurface" || name == "kdsubsurface") &&
        (renderOptions->IntegratorName != "path" &&
         (renderOptions->IntegratorName != "path_rand_cor" &&
          (renderOptions->IntegratorName != "path_fd_distant" &&
           (renderOptions->IntegratorName != "path_fd_rough" &&
            (renderOptions->IntegratorName != "path_fd_rough_debiased" &&
             (renderOptions->IntegratorName != "path_blanchet" &&
              (renderOptions->IntegratorName != "path_blanchet_cust" &&
               (renderOptions->IntegratorName != "path_blanchet_var" &&
                (renderOptions->IntegratorName != "majorant" &&
                 (renderOptions->IntegratorName != "volpath" &&
                  (renderOptions->IntegratorName != "volpath_debias" &&
                   (renderOptions->IntegratorName != "volpath_fd_lightpos" &&
                    (renderOptions->IntegratorName !=
                         "volpath_fd_lightpos_debiased" &&
                     (renderOptions->IntegratorName != "volpath_ray_rand" &&
                      (renderOptions->IntegratorName != "volpath_mut_fd" &&
                       (renderOptions->IntegratorName !=
                            "volpath_nonexp_beta" &&
                        (renderOptions->IntegratorName !=
                             "volpath_mut_fd_debiased" &&
                         (renderOptions->IntegratorName !=
                              "volpath_mut_fd_cor" &&
                          (renderOptions->IntegratorName !=
                               "volpath_mut_fd_cor_debiased" &&
                           (renderOptions->IntegratorName != "volpath_nonexp" &&
                            (renderOptions->IntegratorName !=
                                 "volpath_nonexp_grid" &&
                             (renderOptions->IntegratorName !=
                                  "volpath_nonexp_debias" &&
                              (renderOptions->IntegratorName !=
                                   "volpath_nonexp_debias_alllights" &&
                               (renderOptions->IntegratorName !=
                                    "volpath_nonexp_debias_alllights_grid" &&
                                (renderOptions->IntegratorName !=
                                     "volpath_nonexp_debias_beta" &&
                                 (renderOptions->IntegratorName !=
                                      "volpath_nonexp_debias_fd_beta" &&
                                  (renderOptions->IntegratorName !=
                                       "volpath_diff_trans" &&
                                   (renderOptions->IntegratorName !=
                                        "volpath_clamp_nee" &&
                                    (renderOptions->IntegratorName !=
                                         "volpath_clamp_nee_blanchet" &&
                                     (renderOptions->IntegratorName !=
                                          "volpath_recursive" &&
                                      (renderOptions->IntegratorName !=
                                           "volpath_recursive_blanchet" &&
                                       (renderOptions->IntegratorName !=
                                            "volpath_recursive_correlated_"
                                            "blanchet" &&
                                        (renderOptions->IntegratorName !=
                                             "volpath_recursive_correlated_"
                                             "blanchet_cmf" &&
                                         (renderOptions->IntegratorName !=
                                              "volpath_recursive_debias_cust" &&
                                          (renderOptions->IntegratorName !=
                                               "volpath_recursive_debias_"
                                               "correllated_n" &&
                                           (renderOptions->IntegratorName !=
                                                "volpath_copy" &&
                                            (renderOptions->IntegratorName !=
                                                 "volpath_sing_copy" &&
                                             (renderOptions->IntegratorName !=
                                                  "volpath_ss_rr" &&
                                              (renderOptions->IntegratorName !=
                                                   "volpath_sing_eqi_sb_"
                                                   "trans" &&
                                               (renderOptions->IntegratorName !=
                                                    "volpath_ss_reg" &&
                                                (renderOptions
                                                         ->IntegratorName !=
                                                     "volpath_ss_uni" &&
                                                 (renderOptions
                                                          ->IntegratorName !=
                                                      "med_map" &&
                                                  (renderOptions
                                                       ->IntegratorName !=
                                                   "volpath_ss_"
                                                   "de"
                                                   "l"))))))))))))))))))))))))))))))))))))))))))))
        Warning(
            "Subsurface scattering material \"%s\" used, but \"%s\" "
            "integrator doesn't support subsurface scattering. "
            "Use \"path\", or \"volpath\".",
            name.c_str(), renderOptions->IntegratorName.c_str());

    mp.ReportUnused();
    if (!material)
        Error("Unable to create material \"%s\"", name.c_str());
    else
        ++nMaterialsCreated;
    return std::shared_ptr<Material>(material);
}

std::shared_ptr<Texture<Float>> MakeFloatTexture(const std::string &name,
                                                 const Transform &tex2world,
                                                 const TextureParams &tp) {
    Texture<Float> *tex = nullptr;
    if (name == "constant")
        tex = CreateConstantFloatTexture(tex2world, tp);
    else if (name == "scale")
        tex = CreateScaleFloatTexture(tex2world, tp);
    else if (name == "mix")
        tex = CreateMixFloatTexture(tex2world, tp);
    else if (name == "bilerp")
        tex = CreateBilerpFloatTexture(tex2world, tp);
    else if (name == "imagemap")
        tex = CreateImageFloatTexture(tex2world, tp);
    else if (name == "uv")
        tex = CreateUVFloatTexture(tex2world, tp);
    else if (name == "checkerboard")
        tex = CreateCheckerboardFloatTexture(tex2world, tp);
    else if (name == "dots")
        tex = CreateDotsFloatTexture(tex2world, tp);
    else if (name == "fbm")
        tex = CreateFBmFloatTexture(tex2world, tp);
    else if (name == "wrinkled")
        tex = CreateWrinkledFloatTexture(tex2world, tp);
    else if (name == "marble")
        tex = CreateMarbleFloatTexture(tex2world, tp);
    else if (name == "windy")
        tex = CreateWindyFloatTexture(tex2world, tp);
    else if (name == "ptex")
        tex = CreatePtexFloatTexture(tex2world, tp);
    else
        Warning("Float texture \"%s\" unknown.", name.c_str());
    tp.ReportUnused();
    return std::shared_ptr<Texture<Float>>(tex);
}

std::shared_ptr<Texture<Spectrum>> MakeSpectrumTexture(
    const std::string &name, const Transform &tex2world,
    const TextureParams &tp) {
    Texture<Spectrum> *tex = nullptr;
    if (name == "constant")
        tex = CreateConstantSpectrumTexture(tex2world, tp);
    else if (name == "scale")
        tex = CreateScaleSpectrumTexture(tex2world, tp);
    else if (name == "mix")
        tex = CreateMixSpectrumTexture(tex2world, tp);
    else if (name == "bilerp")
        tex = CreateBilerpSpectrumTexture(tex2world, tp);
    else if (name == "imagemap")
        tex = CreateImageSpectrumTexture(tex2world, tp);
    else if (name == "uv")
        tex = CreateUVSpectrumTexture(tex2world, tp);
    else if (name == "checkerboard")
        tex = CreateCheckerboardSpectrumTexture(tex2world, tp);
    else if (name == "dots")
        tex = CreateDotsSpectrumTexture(tex2world, tp);
    else if (name == "fbm")
        tex = CreateFBmSpectrumTexture(tex2world, tp);
    else if (name == "wrinkled")
        tex = CreateWrinkledSpectrumTexture(tex2world, tp);
    else if (name == "marble")
        tex = CreateMarbleSpectrumTexture(tex2world, tp);
    else if (name == "windy")
        tex = CreateWindySpectrumTexture(tex2world, tp);
    else if (name == "ptex")
        tex = CreatePtexSpectrumTexture(tex2world, tp);
    else if (name == "voronoi_spiral")
        tex = CreateVoronoiSpiralProceduralMedia(tp);
    else if (name == "point_interp")
        tex = CreatePointInterpMedia(tp);
#if OPENVDB
    else if (name == "deintegrated_texture")
        tex = CreateDeintegratedTextureMedia(tp);
#endif
    else if (name == "chess_texture")
        tex = CreateChessTextureMedia(tp);
    else if (name == "cloud")
        tex = CreateCloudProceduralMedia(tp);
    else
        Warning("Spectrum texture \"%s\" unknown.", name.c_str());
    tp.ReportUnused();
    return std::shared_ptr<Texture<Spectrum>>(tex);
}

std::shared_ptr<Medium> MakeMedium(const std::string &name,
                                   const TextureParams &tp,
                                   const Transform &medium2world) {
    std::cout << "Medium Transform:" << std::endl;
    std::cout << medium2world << std::endl;

    ParamSet paramSet = tp.GetMaterialParams();
    Float sig_a_rgb[3] = {.0011f, .0024f, .014f},
          sig_s_rgb[3] = {2.55f, 3.21f, 3.77f};
    Spectrum sig_a = Spectrum::FromRGB(sig_a_rgb),
             sig_s = Spectrum::FromRGB(sig_s_rgb);
    std::string preset = paramSet.FindOneString("preset", "");
    bool found = GetMediumScatteringProperties(preset, &sig_a, &sig_s);
    if (preset != "" && !found)
        Warning("Material preset \"%s\" not found.  Using defaults.",
                preset.c_str());
    Float scale = paramSet.FindOneFloat("scale", 1.f);
    Float g = paramSet.FindOneFloat("g", 0.0);
    std::cout << "G: " << g << std::endl;
    sig_a = paramSet.FindOneSpectrum("sigma_a", sig_a) * scale;
    sig_s = paramSet.FindOneSpectrum("sigma_s", sig_s) * scale;
    int trType = paramSet.FindOneInt("trType", 1);
    Medium *m = NULL;
    // std::cout << "NAMEL " << name << std::endl;
    // if (name == "homogeneous") {
    //     m = new HomogeneousMedium(sig_a, sig_s, g);
    // } else
    if (name == "heterogeneous") {
        // new implementation
        std::string densityFuncName =
            paramSet.FindOneString("densityFunc", "texture");
        std::string transEstName = paramSet.FindOneString("transEstFunc", "");
        std::string majFuncName = paramSet.FindOneString("majFunc", "");
        std::string minFuncName = paramSet.FindOneString("minFunc", "");
        std::string sampFuncName = paramSet.FindOneString("sampFunc", "");

        DensityFunction *density = nullptr;
        T_Estimator *t_est = nullptr;
        MajorantFunction *maj = nullptr;
        MinorantFunction *min = nullptr;
        FF_Sampler *ff_samp = nullptr;

        Point3f minBounds =
            paramSet.FindOnePoint3f("minBounds", Point3f(-0.5, -0.5, -0.5));
        Point3f maxBounds =
            paramSet.FindOnePoint3f("maxBounds", Point3f(0.5, 0.5, 0.5));
        Bounds3f mediumBounds = Bounds3f(minBounds, maxBounds);

        Float majScale = paramSet.FindOneFloat("majScale", 1.0);

        // std::cout << "MAJ SCALE: " << majScale << std::endl;

        Transform medToWorld;

        // create the density function //
        if (densityFuncName == "const") {
            Spectrum dense = paramSet.FindOneSpectrum("dense", Spectrum(1.0));
            // Spectrum dense =
            //     paramSet.FindOneSpectrum("dense",
            //                              Spectrum(0.025));  // dragon
            // Spectrum dense =
            //     paramSet.FindOneSpectrum("dense", Spectrum(0.5));  // specter
            density = new ConstantDensity(dense);
        } else if (densityFuncName == "unit") {
            // Spectrum dense = paramSet.FindOneSpectrum("dense",
            // Spectrum(0.1));
            density = new ConstantDensity(Spectrum(1.f));
        } else if (densityFuncName == "linear") {
            LinearDensityFunction *linFunc = new LinearDensityFunction();

            density = linFunc;
            // std::cout << "Linear Mediums not supported yet" << std::endl;
            // return NULL;
        } else if (densityFuncName == "fractal") {
            std::cout << "Fractal Mediums not supported yet" << std::endl;
            return NULL;
        } else if (densityFuncName == "texture") {
            std::shared_ptr<Texture<Spectrum>> texture =
                tp.GetSpectrumTextureOrNull("procedural");
            if (!texture) std::cout << "da fuk" << std::endl;
            Spectrum scale = paramSet.FindOneSpectrum("scale", Spectrum(1.0));
            density = new TextureDensityFunction(scale, texture);
        } else if (densityFuncName == "proc_sphere") {
            std::cout << "proc sphere should be initialized" << std::endl;
            density = new ProceduralSphereDensity(Spectrum(0.25), 0.0, 0.0, 0.0,
                                                  0.35, 0.1);
        } else if (densityFuncName == "sphere") {
            std::cout << "simple sphere should be initialized" << std::endl;
            density = new SimpleSphereDensity(Spectrum(1.0), 0.0, 0.0, 0.0);
        } else if (densityFuncName == "disk") {
            std::cout << "simple sphere should be initialized" << std::endl;
            Float time = paramSet.FindOneFloat("proc_time", 0.0);
            density = new SimpleDiskDensity(Spectrum(0.5), 0.06, 1.0 / 0.3,
                                            0.4 - 0.8 * time);
        }
#if OPENVDB
        else if (densityFuncName == "vdb") {
            std::string vdb_filename =
                paramSet.FindOneString("vdb_filename", "notfound.vdb");

            Point3f min_box =
                paramSet.FindOnePoint3f("minBounds", Point3f(0.0, 0.0, 0.0));
            Point3f max_box =
                paramSet.FindOnePoint3f("maxBounds", Point3f(1.0, 1.0, 1.0));

            std::cout << "max_box y: " << max_box[1] << std::endl;
            std::cout << "min_box y: " << min_box[1] << std::endl;

            bool worldSpace = paramSet.FindOneBool("worldSpace", true);
            bool clampDensities = paramSet.FindOneBool("clampDensities", false);
            if (clampDensities) std::cout << "CLAMPING DENSITIES" << std::endl;
            bool retargetDensities =
                paramSet.FindOneBool("retargetDensities", false);
            bool retainAspectRatio =
                paramSet.FindOneBool("retainAspectRatio", false);

            std::cout << "before create dense" << std::endl;

            VDB_DensityFunction *vdb_dense =
                new VDB_DensityFunction(vdb_filename);

            std::cout << "after create dense" << std::endl;

            vdb_dense->setWorldSpace(worldSpace);
            vdb_dense->setClampDensities(clampDensities);
            vdb_dense->setRetargetDensities(retargetDensities);
            vdb_dense->setRetainAspectRatio(retainAspectRatio);

            vdb_dense->setBox_Min_X(min_box[0]);
            vdb_dense->setBox_Min_Y(min_box[1]);
            vdb_dense->setBox_Min_Z(min_box[2]);
            vdb_dense->setBox_Max_X(max_box[0]);
            vdb_dense->setBox_Max_Y(max_box[1]);
            vdb_dense->setBox_Max_Z(max_box[2]);

            vdb_dense->preProcess();

            density = vdb_dense;
        }
#endif
        else {
            // Warning("Medium \"%s\" unknown.", name.c_str());
            std::cout << "Unknown Density Function" << std::endl;
            return NULL;
        }
        /////////////////////////////////

        // create the transmittance estimator //
        if (transEstName == "track_length") {
            t_est = new TrackLength();
        } else if (transEstName == "track_length_adap") {
            t_est = new TrackLengthAdaptive();
        } else if (transEstName == "ratio") {
            t_est = new RatioTracking(1.0 / majScale);
        } else if (transEstName == "ratio_adap") {
            t_est = new RatioTrackingAdaptive(1.0 / majScale);
        } else if (transEstName == "ratio_slight") {
            t_est = new RatioTracking_Slight(1.0 / majScale);
        } else if (transEstName == "ratio_signif") {
            t_est = new RatioTracking_Signif(1.0 / majScale);
        } else if (transEstName == "const") {
            t_est = new ConstTracking();
        } else if (transEstName == "next_flight_ratio") {
            t_est = new NextFlightRatioTracking();
        } else if (transEstName == "pseries_cumulative") {
            t_est = new Pseries_Cumulative();
        } else if (transEstName == "analytic") {
            t_est = new AnalyticTrans();
        } else if (transEstName ==
                   "pseries_cdf") {  // TODO: this should be called cmf
            t_est = new Pseries_CDF();
        } else if (transEstName == "exp_nvidia") {
            t_est = new ExpNvidia(1.0 / majScale);
        } else if (transEstName == "pseries_ratio") {
            t_est = new Pseries_Ratio();
        } else if (transEstName == "raymarch") {
            t_est = new RayMarchTrans(majScale);
        } else if (transEstName == "raymarch_nonexp") {
            t_est = new RayMarchNonExpTrans();
        } else if (transEstName == "debiased_raymarch") {
            t_est = new DebiasedRaymarch(majScale);
        } else {
            std::cout << "Unknown Transmittance Estimator" << std::endl;
            return NULL;
        }
        ////////////////////////////////////////

        // TODO: read in maj size from input

        // create the majorant function //
        if (majFuncName == "const") {
            maj = new ConstBoundedMajorant(density);
        } else if (majFuncName == "const_adap") {
            maj = new ConstAdaptiveMajorant(density);
        } else if (majFuncName == "interval_adap") {
            // maj = new IntervalAdaptiveMajorant(density, 1);
            // maj = new IntervalAdaptiveMajorant(density, 2);
            // maj = new IntervalAdaptiveMajorant(density, 4);
            maj = new IntervalAdaptiveMajorant(density, 10);
        } else if (majFuncName == "interval_dense_adap") {
            // maj = new IntervalAdaptiveDensityMajorant(density, 1);
            // maj = new IntervalAdaptiveDensityMajorant(density, 2);
            // maj = new IntervalAdaptiveDensityMajorant(density, 4);
            maj = new IntervalAdaptiveDensityMajorant(density, 5);
        } else if (majFuncName == "interval_adap_vdb") {
            // maj = new IntervalAdaptiveDensityVDB(density, 10);
            // maj = new IntervalAdaptiveDensityVDB(density, 20);
            maj = new IntervalAdaptiveDensityVDB(density, 80);
        } else if (majFuncName == "interval_adap_vdb_partial") {
            // maj = new IntervalAdaptiveDensityVDBPartial(density, 20);
            maj = new IntervalAdaptiveDensityVDBPartial(density, 80);
        } else if (majFuncName == "interval_adap_vdb_constant") {
            maj = new IntervalAdaptiveDensityVDBConstant(density, majScale, 20);
        } else if (majFuncName == "interval_adap_vdb_hardcode") {
            maj = new IntervalAdaptiveDensityVDBHardcode(density, 10);
        } else if (majFuncName == "interval_adap_vdb_track") {
            // maj = new IntervalAdaptiveDensityVDBTracking(density, 10);
            // maj = new IntervalAdaptiveDensityVDBTracking(density, 20);
            // maj = new IntervalAdaptiveDensityVDBTracking(density, 40);
            maj = new IntervalAdaptiveDensityVDBTracking(density, 80);
        } else if (majFuncName == "interval_adap_vdb_tight") {
            // maj = new IntervalAdaptiveDensityVDBTracking(density, 10);
            // maj = new IntervalAdaptiveDensityVDBTracking(density, 20);
            // maj = new IntervalAdaptiveDensityVDBTracking(density, 40);
            maj = new IntervalAdaptiveDensityVDBTight(density, 80);
        } else if (majFuncName == "macroc") {
            // TODO: make these inputs
            int w = 10;
            int h = 10;
            int d = 10;
            maj = new ConstMacroMajorant(density, w, h, d);
        }
#if OPENVDB
        else if (majFuncName == "vdb") {
            std::cout << "creating vdb macro" << std::endl;
            if (densityFuncName != "vdb") {
                std::cout << "VDB Macro Majorants can only work with a VDB "
                             "Density Function"
                          << std::endl;
                return NULL;
            }

            int w = 1225;
            int h = 1225;
            int d = 1225;
            maj = new VDBMacroMajorant(density, w, h, d);
        }
#endif
        else {
            std::cout << "Unknown Majorant Function" << std::endl;
            return NULL;
        }
        //////////////////////////////////

        // create the sample function //
        if (sampFuncName == "trans") {
            ff_samp = new FF_Trans_Sampler(1.0 / majScale);
        } else if (sampFuncName == "trans_adap") {
            ff_samp = new FF_Trans_Adaptive_Sampler(1.0 / majScale);
        } else if (sampFuncName == "trans_weight") {
            ff_samp = new FF_Trans_Weighted_Sampler(1.0 / majScale);
        } else if (sampFuncName == "trans_weight_slight") {
            ff_samp = new FF_Trans_Weighted_Slight_Sampler(1.0 / majScale);
        } else if (sampFuncName == "trans_weight_signif") {
            ff_samp = new FF_Trans_Weighted_Signif_Sampler(1.0 / majScale);
        } else if (sampFuncName == "rat") {
            ff_samp = new FF_Rat_Sampler(1.0 / majScale);
        } else if (sampFuncName == "const") {
            ff_samp = new FF_ConstTracking();
        } else if (sampFuncName == "raymarch") {
            ff_samp = new FF_Raymarching();
        } else if (sampFuncName == "analytic") {
            ff_samp = new FF_Analytic();
        } else if (sampFuncName == "absorptive") {
            ff_samp = new FF_Absorptive_Sampler();
        } else {
            std::cout << "Unknown Sample Function" << std::endl;
            return NULL;
        }
        ////////////////////////////////

        // create the minorant function //
        if (minFuncName == "const") {
            min = new ConstBoundedMinorant(density);
        } else {
            std::cout << "Unknown Minorant Function" << std::endl;
            return NULL;
        }
        //////////////////////////////////

        if (!t_est) std::cout << "ERROR" << std::endl;
        if (!density) std::cout << "ERROR" << std::endl;
        if (!min) std::cout << "ERROR" << std::endl;
        if (!maj) std::cout << "ERROR" << std::endl;
        if (!ff_samp) std::cout << "ERROR" << std::endl;

        Point3f p0 = paramSet.FindOnePoint3f("p0", Point3f(0.f, 0.f, 0.f));
        Point3f p1 = paramSet.FindOnePoint3f("p1", Point3f(1.f, 1.f, 1.f));

        // Point3f p0 = paramSet.FindOnePoint3f("p0", Point3f(-0.5, -0.5,
        // -0.5)); Point3f p1 = paramSet.FindOnePoint3f("p1", Point3f(0.5, 0.5,
        // 0.5));

        std::cout << "P1: " << p1.x << " " << p1.y << " " << p1.z << std::endl;

        Transform data2Medium = Translate(Vector3f(p0)) *
                                Scale(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);

        std::cout << "Data 2 Medium Transform: " << std::endl;
        std::cout << data2Medium << std::endl;

        std::cout << std::endl;

        Transform medTrans = medium2world * data2Medium;
        std::cout << "Final Medium Transform:" << std::endl;
        std::cout << medTrans << std::endl;

        HeterogeneousMedium *medium = new HeterogeneousMedium(
            sig_a, sig_s, g, t_est, density, min, maj, ff_samp,
            // medToWorld,
            medium2world * data2Medium, mediumBounds);

        medium->setMajScale(majScale);

        m = medium;
    } else
        Warning("Medium \"%s\" unknown.", name.c_str());
    paramSet.ReportUnused();
    return std::shared_ptr<Medium>(m);
}

std::shared_ptr<Light> MakeLight(const std::string &name,
                                 const ParamSet &paramSet,
                                 const Transform &light2world,
                                 const MediumInterface &mediumInterface) {
    std::shared_ptr<Light> light;

    if (name == "point")
        light =
            CreatePointLight(light2world, mediumInterface.outside, paramSet);
    else if (name == "point_cor_p")
        light = CreatePointCorPLight(light2world, mediumInterface.outside,
                                     paramSet);
    else if (name == "spot")
        light = CreateSpotLight(light2world, mediumInterface.outside, paramSet);
    // else if (name == "goniometric")
    //     light = CreateGoniometricLight(light2world, mediumInterface.outside,
    //                                    paramSet);
    else if (name == "projection")
        light = CreateProjectionLight(light2world, mediumInterface.outside,
                                      paramSet);
    else if (name == "distant")
        light = CreateDistantLight(light2world, paramSet);
    else if (name == "distant_angle")
        light = CreateDistantAngleLight(light2world, paramSet);
    else if (name == "distant_angle_prog_cor") {
        light = CreateDistantAngleProgCorLight(light2world, paramSet);
    } else if (name == "infinite" || name == "exinfinite")
        light = CreateInfiniteLight(light2world, paramSet);
    else
        Warning("Light \"%s\" unknown.", name.c_str());
    paramSet.ReportUnused();
    return light;
}

std::shared_ptr<AreaLight> MakeAreaLight(const std::string &name,
                                         const Transform &light2world,
                                         const MediumInterface &mediumInterface,
                                         const ParamSet &paramSet,
                                         const std::shared_ptr<Shape> &shape) {
    std::shared_ptr<AreaLight> area;
    if (name == "area" || name == "diffuse")
        area = CreateDiffuseAreaLight(light2world, mediumInterface.outside,
                                      paramSet, shape);
    else
        Warning("Area light \"%s\" unknown.", name.c_str());
    paramSet.ReportUnused();
    return area;
}

std::shared_ptr<Primitive> MakeAccelerator(
    const std::string &name, std::vector<std::shared_ptr<Primitive>> prims,
    const ParamSet &paramSet) {
    std::shared_ptr<Primitive> accel;
    if (name == "bvh")
        accel = CreateBVHAccelerator(std::move(prims), paramSet);
    else if (name == "kdtree")
        accel = CreateKdTreeAccelerator(std::move(prims), paramSet);
    else
        Warning("Accelerator \"%s\" unknown.", name.c_str());
    paramSet.ReportUnused();
    return accel;
}

Camera *MakeCamera(const std::string &name, const ParamSet &paramSet,
                   const TransformSet &cam2worldSet, Float transformStart,
                   Float transformEnd, Film *film) {
    Camera *camera = nullptr;
    MediumInterface mediumInterface = graphicsState.CreateMediumInterface();
    static_assert(MaxTransforms == 2,
                  "TransformCache assumes only two transforms");
    Transform *cam2world[2] = {transformCache.Lookup(cam2worldSet[0]),
                               transformCache.Lookup(cam2worldSet[1])};
    AnimatedTransform animatedCam2World(cam2world[0], transformStart,
                                        cam2world[1], transformEnd);
    if (name == "perspective")
        camera = CreatePerspectiveCamera(paramSet, animatedCam2World, film,
                                         mediumInterface.outside);
    else if (name == "orthographic")
        camera = CreateOrthographicCamera(paramSet, animatedCam2World, film,
                                          mediumInterface.outside);
    else if (name == "realistic")
        camera = CreateRealisticCamera(paramSet, animatedCam2World, film,
                                       mediumInterface.outside);
    else if (name == "environment")
        camera = CreateEnvironmentCamera(paramSet, animatedCam2World, film,
                                         mediumInterface.outside);
    else
        Warning("Camera \"%s\" unknown.", name.c_str());
    paramSet.ReportUnused();
    return camera;
}

std::shared_ptr<Sampler> MakeSampler(const std::string &name,
                                     const ParamSet &paramSet,
                                     const Film *film) {
    Sampler *sampler = nullptr;
    if (name == "lowdiscrepancy" || name == "02sequence")
        sampler = CreateZeroTwoSequenceSampler(paramSet);
    else if (name == "maxmindist")
        sampler = CreateMaxMinDistSampler(paramSet);
    else if (name == "halton")
        sampler = CreateHaltonSampler(paramSet, film->GetSampleBounds());
    else if (name == "sobol")
        sampler = CreateSobolSampler(paramSet, film->GetSampleBounds());
    else if (name == "random")
        sampler = CreateRandomSampler(paramSet);
    else if (name == "stratified")
        sampler = CreateStratifiedSampler(paramSet);
    else
        Warning("Sampler \"%s\" unknown.", name.c_str());
    paramSet.ReportUnused();
    return std::shared_ptr<Sampler>(sampler);
}

std::unique_ptr<Filter> MakeFilter(const std::string &name,
                                   const ParamSet &paramSet) {
    Filter *filter = nullptr;
    if (name == "box")
        filter = CreateBoxFilter(paramSet);
    else if (name == "gaussian")
        filter = CreateGaussianFilter(paramSet);
    else if (name == "mitchell")
        filter = CreateMitchellFilter(paramSet);
    else if (name == "sinc")
        filter = CreateSincFilter(paramSet);
    else if (name == "triangle")
        filter = CreateTriangleFilter(paramSet);
    else {
        Error("Filter \"%s\" unknown.", name.c_str());
        exit(1);
    }
    paramSet.ReportUnused();
    return std::unique_ptr<Filter>(filter);
}

Film *MakeFilm(const std::string &name, const ParamSet &paramSet,
               std::unique_ptr<Filter> filter) {
    Film *film = nullptr;
    if (name == "image")
        film = CreateFilm(paramSet, std::move(filter));
    else
        Warning("Film \"%s\" unknown.", name.c_str());
    paramSet.ReportUnused();
    return film;
}

// API Function Definitions
void pbrtInit(const Options &opt) {
    PbrtOptions = opt;
    // API Initialization
    if (currentApiState != APIState::Uninitialized)
        Error("pbrtInit() has already been called.");
    currentApiState = APIState::OptionsBlock;
    renderOptions.reset(new RenderOptions);
    graphicsState = GraphicsState();
    catIndentCount = 0;

    // General \pbrt Initialization
    SampledSpectrum::Init();
    ParallelInit();  // Threads must be launched before the profiler is
                     // initialized.
    InitProfiler();
}

void pbrtCleanup() {
    // API Cleanup
    if (currentApiState == APIState::Uninitialized)
        Error("pbrtCleanup() called without pbrtInit().");
    else if (currentApiState == APIState::WorldBlock)
        Error("pbrtCleanup() called while inside world block.");
    currentApiState = APIState::Uninitialized;
    ParallelCleanup();
    CleanupProfiler();
}

void pbrtIdentity() {
    VERIFY_INITIALIZED("Identity");
    FOR_ACTIVE_TRANSFORMS(curTransform[i] = Transform();)
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sIdentity\n", catIndentCount, "");
}

void pbrtTranslate(Float dx, Float dy, Float dz) {
    VERIFY_INITIALIZED("Translate");
    FOR_ACTIVE_TRANSFORMS(curTransform[i] = curTransform[i] *
                                            Translate(Vector3f(dx, dy, dz));)
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sTranslate %.9g %.9g %.9g\n", catIndentCount, "", dx, dy, dz);
}

void pbrtTransform(Float tr[16]) {
    VERIFY_INITIALIZED("Transform");
    FOR_ACTIVE_TRANSFORMS(
        curTransform[i] = Transform(Matrix4x4(
            tr[0], tr[4], tr[8], tr[12], tr[1], tr[5], tr[9], tr[13], tr[2],
            tr[6], tr[10], tr[14], tr[3], tr[7], tr[11], tr[15]));)
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sTransform [ ", catIndentCount, "");
        for (int i = 0; i < 16; ++i) printf("%.9g ", tr[i]);
        printf(" ]\n");
    }
}

void pbrtConcatTransform(Float tr[16]) {
    VERIFY_INITIALIZED("ConcatTransform");
    FOR_ACTIVE_TRANSFORMS(
        curTransform[i] =
            curTransform[i] *
            Transform(Matrix4x4(tr[0], tr[4], tr[8], tr[12], tr[1], tr[5],
                                tr[9], tr[13], tr[2], tr[6], tr[10], tr[14],
                                tr[3], tr[7], tr[11], tr[15]));)
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sConcatTransform [ ", catIndentCount, "");
        for (int i = 0; i < 16; ++i) printf("%.9g ", tr[i]);
        printf(" ]\n");
    }
}

void pbrtRotate(Float angle, Float dx, Float dy, Float dz) {
    VERIFY_INITIALIZED("Rotate");
    FOR_ACTIVE_TRANSFORMS(curTransform[i] =
                              curTransform[i] *
                              Rotate(angle, Vector3f(dx, dy, dz));)
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sRotate %.9g %.9g %.9g %.9g\n", catIndentCount, "", angle, dx,
               dy, dz);
}

void pbrtScale(Float sx, Float sy, Float sz) {
    VERIFY_INITIALIZED("Scale");
    FOR_ACTIVE_TRANSFORMS(curTransform[i] =
                              curTransform[i] * Scale(sx, sy, sz);)
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sScale %.9g %.9g %.9g\n", catIndentCount, "", sx, sy, sz);
}

void pbrtLookAt(Float ex, Float ey, Float ez, Float lx, Float ly, Float lz,
                Float ux, Float uy, Float uz) {
    VERIFY_INITIALIZED("LookAt");
    Transform lookAt =
        LookAt(Point3f(ex, ey, ez), Point3f(lx, ly, lz), Vector3f(ux, uy, uz));
    FOR_ACTIVE_TRANSFORMS(curTransform[i] = curTransform[i] * lookAt;);
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf(
            "%*sLookAt %.9g %.9g %.9g\n%*s%.9g %.9g %.9g\n"
            "%*s%.9g %.9g %.9g\n",
            catIndentCount, "", ex, ey, ez, catIndentCount + 8, "", lx, ly, lz,
            catIndentCount + 8, "", ux, uy, uz);
}

void pbrtCoordinateSystem(const std::string &name) {
    VERIFY_INITIALIZED("CoordinateSystem");
    // std::cout << "COORD SYS: " << name << std::endl;
    namedCoordinateSystems[name] = curTransform;
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sCoordinateSystem \"%s\"\n", catIndentCount, "",
               name.c_str());
}

void pbrtCoordSysTransform(const std::string &name) {
    VERIFY_INITIALIZED("CoordSysTransform");
    if (namedCoordinateSystems.find(name) != namedCoordinateSystems.end())
        curTransform = namedCoordinateSystems[name];
    else
        Warning("Couldn't find named coordinate system \"%s\"", name.c_str());
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sCoordSysTransform \"%s\"\n", catIndentCount, "",
               name.c_str());
}

void pbrtActiveTransformAll() {
    activeTransformBits = AllTransformsBits;
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sActiveTransform All\n", catIndentCount, "");
}

void pbrtActiveTransformEndTime() {
    activeTransformBits = EndTransformBits;
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sActiveTransform EndTime\n", catIndentCount, "");
}

void pbrtActiveTransformStartTime() {
    activeTransformBits = StartTransformBits;
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sActiveTransform StartTime\n", catIndentCount, "");
}

void pbrtTransformTimes(Float start, Float end) {
    VERIFY_OPTIONS("TransformTimes");
    renderOptions->transformStartTime = start;
    renderOptions->transformEndTime = end;
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sTransformTimes %.9g %.9g\n", catIndentCount, "", start, end);
}

void pbrtPixelFilter(const std::string &name, const ParamSet &params) {
    VERIFY_OPTIONS("PixelFilter");
    renderOptions->FilterName = name;
    renderOptions->FilterParams = params;
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sPixelFilter \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtFilm(const std::string &type, const ParamSet &params) {
    VERIFY_OPTIONS("Film");
    renderOptions->FilmParams = params;
    renderOptions->FilmName = type;
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sFilm \"%s\" ", catIndentCount, "", type.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtSampler(const std::string &name, const ParamSet &params) {
    VERIFY_OPTIONS("Sampler");
    renderOptions->SamplerName = name;
    renderOptions->SamplerParams = params;
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sSampler \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtAccelerator(const std::string &name, const ParamSet &params) {
    VERIFY_OPTIONS("Accelerator");
    renderOptions->AcceleratorName = name;
    renderOptions->AcceleratorParams = params;
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sAccelerator \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtIntegrator(const std::string &name, const ParamSet &params) {
    VERIFY_OPTIONS("Integrator");
    renderOptions->IntegratorName = name;
    // renderOptions->IntegratorName = "maj_eff";
    // renderOptions->IntegratorName = "maj_max";
    // renderOptions->IntegratorName = "volpath_trans";
    // renderOptions->IntegratorName = "volpath_abs";
    // renderOptions->IntegratorName = "volpath_sing";
    // renderOptions->IntegratorName = "volpath_sing_eqi";
    // renderOptions->IntegratorName = "volpath_sing_eqi_combined";
    // std::cout << "WARNING: using hardcoded integrator" << std::endl;
    renderOptions->IntegratorParams = params;
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sIntegrator \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtCamera(const std::string &name, const ParamSet &params) {
    VERIFY_OPTIONS("Camera");
    renderOptions->CameraName = name;
    renderOptions->CameraParams = params;
    renderOptions->CameraToWorld = Inverse(curTransform);
    namedCoordinateSystems["camera"] = renderOptions->CameraToWorld;
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sCamera \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtMakeNamedMedium(const std::string &name, const ParamSet &params) {
    VERIFY_INITIALIZED("MakeNamedMedium");
    WARN_IF_ANIMATED_TRANSFORM("MakeNamedMedium");
    // NEW LOGIC:
    ParamSet emptyParams;
    TextureParams mp(emptyParams, params, *graphicsState.floatTextures,
                     *graphicsState.spectrumTextures);

    std::string type = params.FindOneString("type", "");
    if (type == "")
        Error("No parameter string \"type\" found in MakeNamedMedium");
    else {
        // std::cout << "Current Transform:" << std::endl;
        // std::cout << curTransform[0] << std::endl;
        std::shared_ptr<Medium> medium =
            // MakeMedium(type, params, curTransform[0]);
            MakeMedium(type, mp, curTransform[0]);
        if (medium) {
            renderOptions->namedMedia[name] = medium;
            renderOptions->all_media.push_back(medium);
        }
    }
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sMakeNamedMedium \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtMediumInterface(const std::string &insideName,
                         const std::string &outsideName) {
    VERIFY_INITIALIZED("MediumInterface");
    graphicsState.currentInsideMedium = insideName;
    graphicsState.currentOutsideMedium = outsideName;
    renderOptions->haveScatteringMedia = true;
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sMediumInterface \"%s\" \"%s\"\n", catIndentCount, "",
               insideName.c_str(), outsideName.c_str());
}

void pbrtWorldBegin() {
    VERIFY_OPTIONS("WorldBegin");
    currentApiState = APIState::WorldBlock;
    for (int i = 0; i < MaxTransforms; ++i) curTransform[i] = Transform();
    activeTransformBits = AllTransformsBits;
    namedCoordinateSystems["world"] = curTransform;
    if (PbrtOptions.cat || PbrtOptions.toPly) printf("\n\nWorldBegin\n\n");
}

void pbrtAttributeBegin() {
    VERIFY_WORLD("AttributeBegin");
    pushedGraphicsStates.push_back(graphicsState);
    graphicsState.floatTexturesShared = graphicsState.spectrumTexturesShared =
        graphicsState.namedMaterialsShared = true;
    pushedTransforms.push_back(curTransform);
    pushedActiveTransformBits.push_back(activeTransformBits);
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("\n%*sAttributeBegin\n", catIndentCount, "");
        catIndentCount += 4;
    }
}

void pbrtAttributeEnd() {
    VERIFY_WORLD("AttributeEnd");
    if (!pushedGraphicsStates.size()) {
        Error(
            "Unmatched pbrtAttributeEnd() encountered. "
            "Ignoring it.");
        return;
    }
    graphicsState = std::move(pushedGraphicsStates.back());
    pushedGraphicsStates.pop_back();
    curTransform = pushedTransforms.back();
    pushedTransforms.pop_back();
    activeTransformBits = pushedActiveTransformBits.back();
    pushedActiveTransformBits.pop_back();
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        catIndentCount -= 4;
        printf("%*sAttributeEnd\n", catIndentCount, "");
    }
}

void pbrtTransformBegin() {
    VERIFY_WORLD("TransformBegin");
    pushedTransforms.push_back(curTransform);
    pushedActiveTransformBits.push_back(activeTransformBits);
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sTransformBegin\n", catIndentCount, "");
        catIndentCount += 4;
    }
}

void pbrtTransformEnd() {
    VERIFY_WORLD("TransformEnd");
    if (!pushedTransforms.size()) {
        Error(
            "Unmatched pbrtTransformEnd() encountered. "
            "Ignoring it.");
        return;
    }
    curTransform = pushedTransforms.back();
    pushedTransforms.pop_back();
    activeTransformBits = pushedActiveTransformBits.back();
    pushedActiveTransformBits.pop_back();
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        catIndentCount -= 4;
        printf("%*sTransformEnd\n", catIndentCount, "");
    }
}

void pbrtTexture(const std::string &name, const std::string &type,
                 const std::string &texname, const ParamSet &params) {
    VERIFY_WORLD("Texture");
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sTexture \"%s\" \"%s\" \"%s\" ", catIndentCount, "",
               name.c_str(), type.c_str(), texname.c_str());
        params.Print(catIndentCount);
        printf("\n");
        return;
    }

    std::cout << "Name: " << name << std::endl;
    std::cout << "Type: " << type << std::endl;
    std::cout << "TexName: " << texname << std::endl;
    std::cout << std::endl;

    TextureParams tp(params, params, *graphicsState.floatTextures,
                     *graphicsState.spectrumTextures);
    if (type == "float") {
        // Create _Float_ texture and store in _floatTextures_
        if (graphicsState.floatTextures->find(name) !=
            graphicsState.floatTextures->end())
            Warning("Texture \"%s\" being redefined", name.c_str());
        WARN_IF_ANIMATED_TRANSFORM("Texture");
        std::shared_ptr<Texture<Float>> ft =
            MakeFloatTexture(texname, curTransform[0], tp);
        if (ft) {
            // TODO: move this to be a GraphicsState method, also don't
            // provide direct floatTextures access?
            if (graphicsState.floatTexturesShared) {
                graphicsState.floatTextures =
                    std::make_shared<GraphicsState::FloatTextureMap>(
                        *graphicsState.floatTextures);
                graphicsState.floatTexturesShared = false;
            }
            (*graphicsState.floatTextures)[name] = ft;
        }
    } else if (type == "color" || type == "spectrum") {
        // Create _color_ texture and store in _spectrumTextures_
        if (graphicsState.spectrumTextures->find(name) !=
            graphicsState.spectrumTextures->end())
            Warning("Texture \"%s\" being redefined", name.c_str());
        WARN_IF_ANIMATED_TRANSFORM("Texture");
        std::shared_ptr<Texture<Spectrum>> st =
            MakeSpectrumTexture(texname, curTransform[0], tp);
        if (st) {
            if (graphicsState.spectrumTexturesShared) {
                graphicsState.spectrumTextures =
                    std::make_shared<GraphicsState::SpectrumTextureMap>(
                        *graphicsState.spectrumTextures);
                graphicsState.spectrumTexturesShared = false;
            }
            (*graphicsState.spectrumTextures)[name] = st;
        }
    } else
        Error("Texture type \"%s\" unknown.", type.c_str());
}

void pbrtMaterial(const std::string &name, const ParamSet &params) {
    VERIFY_WORLD("Material");
    ParamSet emptyParams;
    TextureParams mp(params, emptyParams, *graphicsState.floatTextures,
                     *graphicsState.spectrumTextures);
    std::shared_ptr<Material> mtl = MakeMaterial(name, mp);
    graphicsState.currentMaterial =
        std::make_shared<MaterialInstance>(name, mtl, params);

    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sMaterial \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtMakeNamedMaterial(const std::string &name, const ParamSet &params) {
    VERIFY_WORLD("MakeNamedMaterial");
    // error checking, warning if replace, what to use for transform?
    ParamSet emptyParams;
    TextureParams mp(params, emptyParams, *graphicsState.floatTextures,
                     *graphicsState.spectrumTextures);
    std::string matName = mp.FindString("type");
    WARN_IF_ANIMATED_TRANSFORM("MakeNamedMaterial");
    if (matName == "")
        Error("No parameter string \"type\" found in MakeNamedMaterial");

    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sMakeNamedMaterial \"%s\" ", catIndentCount, "",
               name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    } else {
        std::shared_ptr<Material> mtl = MakeMaterial(matName, mp);
        if (graphicsState.namedMaterials->find(name) !=
            graphicsState.namedMaterials->end())
            Warning("Named material \"%s\" redefined.", name.c_str());
        if (graphicsState.namedMaterialsShared) {
            graphicsState.namedMaterials =
                std::make_shared<GraphicsState::NamedMaterialMap>(
                    *graphicsState.namedMaterials);
            graphicsState.namedMaterialsShared = false;
        }
        (*graphicsState.namedMaterials)[name] =
            std::make_shared<MaterialInstance>(matName, mtl, params);
    }
}

void pbrtNamedMaterial(const std::string &name) {
    VERIFY_WORLD("NamedMaterial");
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sNamedMaterial \"%s\"\n", catIndentCount, "", name.c_str());
        return;
    }

    auto iter = graphicsState.namedMaterials->find(name);
    if (iter == graphicsState.namedMaterials->end()) {
        Error("NamedMaterial \"%s\" unknown.", name.c_str());
        return;
    }
    graphicsState.currentMaterial = iter->second;
}

void pbrtLightSource(const std::string &name, const ParamSet &params) {
    VERIFY_WORLD("LightSource");
    WARN_IF_ANIMATED_TRANSFORM("LightSource");
    MediumInterface mi = graphicsState.CreateMediumInterface();
    std::shared_ptr<Light> lt = MakeLight(name, params, curTransform[0], mi);
    if (!lt)
        Error("LightSource: light type \"%s\" unknown.", name.c_str());
    else
        renderOptions->lights.push_back(lt);
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sLightSource \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtAreaLightSource(const std::string &name, const ParamSet &params) {
    VERIFY_WORLD("AreaLightSource");
    graphicsState.areaLight = name;
    graphicsState.areaLightParams = params;
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sAreaLightSource \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }
}

void pbrtShape(const std::string &name, const ParamSet &params) {
    VERIFY_WORLD("Shape");
    std::vector<std::shared_ptr<Primitive>> prims;
    std::vector<std::shared_ptr<AreaLight>> areaLights;
    if (PbrtOptions.cat || (PbrtOptions.toPly && name != "trianglemesh")) {
        printf("%*sShape \"%s\" ", catIndentCount, "", name.c_str());
        params.Print(catIndentCount);
        printf("\n");
    }

    if (!curTransform.IsAnimated()) {
        // Initialize _prims_ and _areaLights_ for static shape

        // Create shapes for shape _name_
        Transform *ObjToWorld = transformCache.Lookup(curTransform[0]);
        Transform *WorldToObj = transformCache.Lookup(Inverse(curTransform[0]));
        std::vector<std::shared_ptr<Shape>> shapes =
            MakeShapes(name, ObjToWorld, WorldToObj,
                       graphicsState.reverseOrientation, params);
        if (shapes.empty()) return;
        std::shared_ptr<Material> mtl =
            graphicsState.GetMaterialForShape(params);
        params.ReportUnused();
        MediumInterface mi = graphicsState.CreateMediumInterface();
        prims.reserve(shapes.size());
        for (auto s : shapes) {
            // Possibly create area light for shape
            std::shared_ptr<AreaLight> area;
            if (graphicsState.areaLight != "") {
                area = MakeAreaLight(graphicsState.areaLight, curTransform[0],
                                     mi, graphicsState.areaLightParams, s);
                if (area) areaLights.push_back(area);
            }
            prims.push_back(
                std::make_shared<GeometricPrimitive>(s, mtl, area, mi));
        }
    } else {
        // Initialize _prims_ and _areaLights_ for animated shape

        // Create initial shape or shapes for animated shape
        if (graphicsState.areaLight != "")
            Warning(
                "Ignoring currently set area light when creating "
                "animated shape");
        Transform *identity = transformCache.Lookup(Transform());
        std::vector<std::shared_ptr<Shape>> shapes = MakeShapes(
            name, identity, identity, graphicsState.reverseOrientation, params);
        if (shapes.empty()) return;

        // Create _GeometricPrimitive_(s) for animated shape
        std::shared_ptr<Material> mtl =
            graphicsState.GetMaterialForShape(params);
        params.ReportUnused();
        MediumInterface mi = graphicsState.CreateMediumInterface();
        prims.reserve(shapes.size());
        for (auto s : shapes)
            prims.push_back(
                std::make_shared<GeometricPrimitive>(s, mtl, nullptr, mi));

        // Create single _TransformedPrimitive_ for _prims_

        // Get _animatedObjectToWorld_ transform for shape
        static_assert(MaxTransforms == 2,
                      "TransformCache assumes only two transforms");
        Transform *ObjToWorld[2] = {transformCache.Lookup(curTransform[0]),
                                    transformCache.Lookup(curTransform[1])};

        AnimatedTransform animatedObjectToWorld(
            ObjToWorld[0], renderOptions->transformStartTime, ObjToWorld[1],
            renderOptions->transformEndTime);
        if (prims.size() > 1) {
            std::shared_ptr<Primitive> bvh = std::make_shared<BVHAccel>(prims);
            prims.clear();
            prims.push_back(bvh);
        }

        prims[0] = std::make_shared<TransformedPrimitive>(
            prims[0], animatedObjectToWorld);
    }
    // Add _prims_ and _areaLights_ to scene or current instance
    if (renderOptions->currentInstance) {
        if (areaLights.size())
            Warning("Area lights not supported with object instancing");
        renderOptions->currentInstance->insert(
            renderOptions->currentInstance->end(), prims.begin(), prims.end());
    } else {
        renderOptions->primitives.insert(renderOptions->primitives.end(),
                                         prims.begin(), prims.end());
        if (areaLights.size())
            renderOptions->lights.insert(renderOptions->lights.end(),
                                         areaLights.begin(), areaLights.end());
    }
}

// Attempt to determine if the ParamSet for a shape may provide a value for
// its material's parameters. Unfortunately, materials don't provide an
// explicit representation of their parameters that we can query and
// cross-reference with the parameter values available from the shape.
//
// Therefore, we'll apply some "heuristics".
bool shapeMaySetMaterialParameters(const ParamSet &ps) {
    for (const auto &param : ps.textures)
        // Any texture other than one for an alpha mask is almost certainly
        // for a Material (or is unused!).
        if (param->name != "alpha" && param->name != "shadowalpha") return true;

    // Special case spheres, which are the most common non-mesh primitive.
    for (const auto &param : ps.floats)
        if (param->nValues == 1 && param->name != "radius") return true;

    // Extra special case strings, since plymesh uses "filename", curve "type",
    // and loopsubdiv "scheme".
    for (const auto &param : ps.strings)
        if (param->nValues == 1 && param->name != "filename" &&
            param->name != "type" && param->name != "scheme")
            return true;

    // For all other parameter types, if there is a single value of the
    // parameter, assume it may be for the material. This should be valid
    // (if conservative), since no materials currently take array
    // parameters.
    for (const auto &param : ps.bools)
        if (param->nValues == 1) return true;
    for (const auto &param : ps.ints)
        if (param->nValues == 1) return true;
    for (const auto &param : ps.point2fs)
        if (param->nValues == 1) return true;
    for (const auto &param : ps.vector2fs)
        if (param->nValues == 1) return true;
    for (const auto &param : ps.point3fs)
        if (param->nValues == 1) return true;
    for (const auto &param : ps.vector3fs)
        if (param->nValues == 1) return true;
    for (const auto &param : ps.normals)
        if (param->nValues == 1) return true;
    for (const auto &param : ps.spectra)
        if (param->nValues == 1) return true;

    return false;
}

std::shared_ptr<Material> GraphicsState::GetMaterialForShape(
    const ParamSet &shapeParams) {
#if LOGGING
    CHECK(currentMaterial);
#endif
    if (shapeMaySetMaterialParameters(shapeParams)) {
        // Only create a unique material for the shape if the shape's
        // parameters are (apparently) going to provide values for some of
        // the material parameters.
        TextureParams mp(shapeParams, currentMaterial->params, *floatTextures,
                         *spectrumTextures);
        return MakeMaterial(currentMaterial->name, mp);
    } else
        return currentMaterial->material;
}

MediumInterface GraphicsState::CreateMediumInterface() {
    MediumInterface m;
    if (currentInsideMedium != "") {
        if (renderOptions->namedMedia.find(currentInsideMedium) !=
            renderOptions->namedMedia.end())
            m.inside = renderOptions->namedMedia[currentInsideMedium].get();
        else
            Error("Named medium \"%s\" undefined.",
                  currentInsideMedium.c_str());
    }
    if (currentOutsideMedium != "") {
        if (renderOptions->namedMedia.find(currentOutsideMedium) !=
            renderOptions->namedMedia.end())
            m.outside = renderOptions->namedMedia[currentOutsideMedium].get();
        else
            Error("Named medium \"%s\" undefined.",
                  currentOutsideMedium.c_str());
    }
    return m;
}

void pbrtReverseOrientation() {
    VERIFY_WORLD("ReverseOrientation");
    graphicsState.reverseOrientation = !graphicsState.reverseOrientation;
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sReverseOrientation\n", catIndentCount, "");
}

void pbrtObjectBegin(const std::string &name) {
    VERIFY_WORLD("ObjectBegin");
    pbrtAttributeBegin();
    if (renderOptions->currentInstance)
        Error("ObjectBegin called inside of instance definition");
    renderOptions->instances[name] = std::vector<std::shared_ptr<Primitive>>();
    renderOptions->currentInstance = &renderOptions->instances[name];
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sObjectBegin \"%s\"\n", catIndentCount, "", name.c_str());
}

STAT_COUNTER("Scene/Object instances created", nObjectInstancesCreated);

void pbrtObjectEnd() {
    VERIFY_WORLD("ObjectEnd");
    if (!renderOptions->currentInstance)
        Error("ObjectEnd called outside of instance definition");
    renderOptions->currentInstance = nullptr;
    pbrtAttributeEnd();
    ++nObjectInstancesCreated;
    if (PbrtOptions.cat || PbrtOptions.toPly)
        printf("%*sObjectEnd\n", catIndentCount, "");
}

STAT_COUNTER("Scene/Object instances used", nObjectInstancesUsed);

void pbrtObjectInstance(const std::string &name) {
    VERIFY_WORLD("ObjectInstance");
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sObjectInstance \"%s\"\n", catIndentCount, "", name.c_str());
        return;
    }

    // Perform object instance error checking
    if (renderOptions->currentInstance) {
        Error("ObjectInstance can't be called inside instance definition");
        return;
    }
    if (renderOptions->instances.find(name) == renderOptions->instances.end()) {
        Error("Unable to find instance named \"%s\"", name.c_str());
        return;
    }
    std::vector<std::shared_ptr<Primitive>> &in =
        renderOptions->instances[name];
    if (in.empty()) return;
    ++nObjectInstancesUsed;
    if (in.size() > 1) {
        // Create aggregate for instance _Primitive_s
        std::shared_ptr<Primitive> accel(
            MakeAccelerator(renderOptions->AcceleratorName, std::move(in),
                            renderOptions->AcceleratorParams));
        if (!accel) accel = std::make_shared<BVHAccel>(in);
        in.clear();
        in.push_back(accel);
    }
    static_assert(MaxTransforms == 2,
                  "TransformCache assumes only two transforms");
    // Create _animatedInstanceToWorld_ transform for instance
    Transform *InstanceToWorld[2] = {transformCache.Lookup(curTransform[0]),
                                     transformCache.Lookup(curTransform[1])};
    AnimatedTransform animatedInstanceToWorld(
        InstanceToWorld[0], renderOptions->transformStartTime,
        InstanceToWorld[1], renderOptions->transformEndTime);
    std::shared_ptr<Primitive> prim(
        std::make_shared<TransformedPrimitive>(in[0], animatedInstanceToWorld));
    renderOptions->primitives.push_back(prim);
}

void pbrtWorldEnd() {
    VERIFY_WORLD("WorldEnd");
    // Ensure there are no pushed graphics states
    while (pushedGraphicsStates.size()) {
        Warning("Missing end to pbrtAttributeBegin()");
        pushedGraphicsStates.pop_back();
        pushedTransforms.pop_back();
    }
    while (pushedTransforms.size()) {
        Warning("Missing end to pbrtTransformBegin()");
        pushedTransforms.pop_back();
    }

    // Create scene and render
    if (PbrtOptions.cat || PbrtOptions.toPly) {
        printf("%*sWorldEnd\n", catIndentCount, "");
    } else {
        std::unique_ptr<Integrator> integrator(renderOptions->MakeIntegrator());
        std::unique_ptr<Scene> scene(renderOptions->MakeScene());

// This is kind of ugly; we directly override the current profiler
// state to switch from parsing/scene construction related stuff to
// rendering stuff and then switch it back below. The underlying
// issue is that all the rest of the profiling system assumes
// hierarchical inheritance of profiling state; this is the only
// place where that isn't the case.
#if LOGGING
        CHECK_EQ(CurrentProfilerState(), ProfToBits(Prof::SceneConstruction));
#endif
        ProfilerState = ProfToBits(Prof::IntegratorRender);

#if RENDER_MUTABLE
        if (scene && integrator) {
            integrator->Render_Mutable(*scene);
        }
#else
        if (scene && integrator) {
            integrator->Render(*scene);
        }
#endif

#if LOGGING
        CHECK_EQ(CurrentProfilerState(), ProfToBits(Prof::IntegratorRender));
#endif
        ProfilerState = ProfToBits(Prof::SceneConstruction);
    }

    // std::cout << "Clearing Cache" << std::endl;

    // Clean up after rendering. Do this before reporting stats so that
    // destructors can run and update stats as needed.
    graphicsState = GraphicsState();
    transformCache.Clear();
    currentApiState = APIState::OptionsBlock;
    ImageTexture<Float, Float>::ClearCache();
    ImageTexture<RGBSpectrum, Spectrum>::ClearCache();
    renderOptions.reset(new RenderOptions);

    // if (!PbrtOptions.cat && !PbrtOptions.toPly) {
    //     MergeWorkerThreadStats();
    //     ReportThreadStats();
    //     if (!PbrtOptions.quiet) {
    //         PrintStats(stdout);
    //         ReportProfilerResults(stdout);
    //         ClearStats();
    //         ClearProfiler();
    //     }
    // }

    for (int i = 0; i < MaxTransforms; ++i) curTransform[i] = Transform();
    activeTransformBits = AllTransformsBits;
    namedCoordinateSystems.erase(namedCoordinateSystems.begin(),
                                 namedCoordinateSystems.end());
}

Scene *RenderOptions::MakeScene() {
    std::shared_ptr<Primitive> accelerator = MakeAccelerator(
        AcceleratorName, std::move(primitives), AcceleratorParams);
    if (!accelerator) accelerator = std::make_shared<BVHAccel>(primitives);
    Scene *scene = new Scene(accelerator, lights, all_media);
    // Erase primitives and lights from _RenderOptions_
    primitives.clear();
    lights.clear();
    return scene;
}

Integrator *RenderOptions::MakeIntegrator() const {
    std::shared_ptr<const Camera> camera(MakeCamera());
    if (!camera) {
        Error("Unable to create camera");
        return nullptr;
    }

    std::shared_ptr<Sampler> sampler =
        MakeSampler(SamplerName, SamplerParams, camera->film);
    if (!sampler) {
        Error("Unable to create sampler.");
        return nullptr;
    }

    std::cout << "INTEGRATOR NAME: " << IntegratorName << std::endl;

    Integrator *integrator = nullptr;
    if (IntegratorName == "whitted")
        integrator = CreateWhittedIntegrator(IntegratorParams, sampler, camera);
    else if (IntegratorName == "whitted_blanchet")
        integrator =
            CreateWhittedBlanchetIntegrator(IntegratorParams, sampler, camera);
    else if (IntegratorName == "normals")
        integrator = CreateNormalsIntegrator(IntegratorParams, sampler, camera);
    else if (IntegratorName == "directlighting")
        integrator =
            CreateDirectLightingIntegrator(IntegratorParams, sampler, camera);
    else if (IntegratorName == "path")
        integrator = CreatePathIntegrator(IntegratorParams, sampler, camera);
    else if (IntegratorName == "path_blanchet")
        integrator =
            CreatePathBlanchetIntegrator(IntegratorParams, sampler, camera);
    else if (IntegratorName == "volpath")
        integrator = CreateVolPathIntegrator(IntegratorParams, sampler, camera);
    else if (IntegratorName == "volpath_debias")
        integrator =
            CreateVolPathDebiasedIntegrator(IntegratorParams, sampler, camera);
    else if (IntegratorName == "majorant")
        integrator =
            CreateMajorantIntegrator(IntegratorParams, sampler, camera);
    // else if (IntegratorName == "volpath_nonexp")
    //     integrator =
    //         CreateVolPathNonExpIntegrator(IntegratorParams, sampler, camera);
    // else if (IntegratorName == "volpath_nonexp_debias")
    //     integrator = CreateVolPathNonExpDebiasIntegrator(IntegratorParams,
    //                                                      sampler, camera);
    else if (IntegratorName == "bdpt") {
        integrator = CreateBDPTIntegrator(IntegratorParams, sampler, camera);
    }
    // else if (IntegratorName == "bdpt_rand_cor")
    //     integrator =
    //         CreateBDPTRandCorIntegrator(IntegratorParams, sampler, camera);
    // else if (IntegratorName == "bdpt_rand_cor_mod")
    //     integrator =
    //         CreateBDPTRandCorModIntegrator(IntegratorParams, sampler,
    //         camera);
    else if (IntegratorName == "mlt") {
        integrator = CreateMLTIntegrator(IntegratorParams, camera);
    } else if (IntegratorName == "ambientocclusion") {
        integrator = CreateAOIntegrator(IntegratorParams, sampler, camera);
    } else if (IntegratorName == "sppm") {
        integrator = CreateSPPMIntegrator(IntegratorParams, camera);
    } else {
        Error("Integrator \"%s\" unknown.", IntegratorName.c_str());
        return nullptr;
    }

    if (renderOptions->haveScatteringMedia && IntegratorName != "volpath" &&
        IntegratorName != "volpath_debias" && IntegratorName != "majorant" &&
        IntegratorName != "volpath_recursive" &&
        IntegratorName != "volpath_fd_lightpos" &&
        IntegratorName != "volpath_fd_lightpos_debiased" &&
        IntegratorName != "volpath_fd_lightpos_debiased_center" &&
        IntegratorName != "volpath_mut_fd" &&
        IntegratorName != "volpath_mut_fd_debiased" &&
        IntegratorName != "volpath_mut_fd_cor" &&
        IntegratorName != "volpath_mut_fd_cor_debiased" &&
        IntegratorName != "volpath_clamp_nee" &&
        IntegratorName != "volpath_clamp_nee_blanchet" &&
        IntegratorName != "volpath_nonexp" &&
        IntegratorName != "volpath_nonexp_grid" &&
        IntegratorName != "volpath_nonexp_beta" &&
        IntegratorName != "volpath_nonexp_debias" &&
        IntegratorName != "volpath_nonexp_debias_alllights" &&
        IntegratorName != "volpath_nonexp_debias_alllights_grid" &&
        IntegratorName != "volpath_nonexp_debias_beta" &&
        IntegratorName != "volpath_nonexp_debias_fd_beta" &&
        IntegratorName != "volpath_diff_trans" &&
        IntegratorName != "volpath_ray_rand" &&
        IntegratorName != "volpath_recursive_blanchet" &&
        IntegratorName != "volpath_recursive_correlated_blanchet" &&
        IntegratorName != "volpath_recursive_correlated_blanchet_cmf" &&
        IntegratorName != "volpath_recursive_debias_cust" &&
        IntegratorName != "volpath_recursive_debias_correllated_n" &&
        IntegratorName != "bdpt" && IntegratorName != "mlt" &&
        IntegratorName != "bdpt_rand_cor" &&
        IntegratorName != "bdpt_rand_cor_mod" &&
        IntegratorName != "volpath_ss_rr" &&
        IntegratorName != "volpath_sing_eqi_sb_trans" &&
        IntegratorName != "volpath_ss_reg" &&
        IntegratorName != "volpath_ss_uni" &&
        IntegratorName != "volpath_ss_del" && IntegratorName != "med_map" &&
        IntegratorName != "volpath_trans" && IntegratorName != "volpath_abs" &&
        IntegratorName != "volpath_abs_debias" &&
        IntegratorName != "volpath_sing" &&
        IntegratorName != "volpath_sing_eqi" && IntegratorName != "maj_eff" &&
        IntegratorName != "maj_max" && IntegratorName != "vol_sppm" &&
        IntegratorName != "vol_sppm_recursive" &&
        IntegratorName != "volpath_sing_eqi_combined" &&
        IntegratorName != "volpath_sing_combined") {
        Warning(
            "Scene has scattering media but \"%s\" integrator doesn't support "
            "volume scattering. Consider using \"volpath\", \"bdpt\", or "
            "\"mlt\".",
            IntegratorName.c_str());
    }

    IntegratorParams.ReportUnused();
    // Warn if no light sources are defined
    if (lights.empty())
        Warning(
            "No light sources defined in scene; "
            "rendering a black image.");
    return integrator;
}

Camera *RenderOptions::MakeCamera() const {
    std::unique_ptr<Filter> filter = MakeFilter(FilterName, FilterParams);
    Film *film = MakeFilm(FilmName, FilmParams, std::move(filter));
    if (!film) {
        Error("Unable to create film.");
        return nullptr;
    }
    Camera *camera = pbrt::MakeCamera(CameraName, CameraParams, CameraToWorld,
                                      renderOptions->transformStartTime,
                                      renderOptions->transformEndTime, film);
    return camera;
}

}  // namespace pbrt
