#pragma once

#include "api.h"
#include "paramset.h"
#include "pbrt.h"

using namespace pbrt;

class ExperimentBase {
  public:
    // ExperimentBase();

    // virtual void createResultsDirectory();

    virtual void initializeIntegrator(int sec = 1000000, long extCalls = -1,
                                      int maxD = 50,
                                      std::string integrator = "volpath",
                                      std::string trans = "independent",
                                      std::string ff = "independent",
                                      bool isCorrelated = false,
                                      bool isMultithreaded = false);

    virtual void initializeConsistentIntegrator(
        int sec = 1000000, long extCalls = -1, int maxD = 50, Float scale = 1.0,
        int seedOne = 0x0, int seedTwo = 0xaaa, int seedThree = 0xaca,
        std::string integrator = "volpath_fd_lightpos_consistent",
        std::string trans = "independent", std::string ff = "independent",
        bool isCorrelated = false, bool isMultithreaded = false);

    virtual void initializeIntegratorSeeded(
        int sec = 1000000, long extCalls = -1, int maxD = 50,
        std::string integrator = "volpath", std::string trans = "independent",
        std::string ff = "independent", int seedOne = 0x0, int seedTwo = 0xaaa,
        int seedThree = 0xaca, bool isCorrelated = false,
        bool isMultithreaded = false);

    virtual void initializeSampler(std::string sampler = "random",
                                   int pixelSamples = 16);

    virtual void initializePixelFilter(std::string filter = "box");

    virtual void initializeFilm(std::string output, int xres, int yres);

    virtual void initializeFilm(std::vector<Float> crop, std::string output,
                                Float scle, int xres, int yres);

    virtual void initializeCamera(std::string camera, Float fov);

    virtual void initializeTrimesh(std::vector<int> inds,
                                   std::vector<Float> uvs,
                                   std::vector<Float> ps);

    virtual void initializeTrimesh(std::vector<int> inds,
                                   std::vector<Float> ps);

    virtual void initializeShape(std::string shape, std::string indFile,
                                 std::string stFile, std::string pFile,
                                 std::string nFile);

    virtual void initializeShape(std::string shape,
                                 const std::vector<int>& indices,
                                 const std::vector<Float>& st,
                                 const std::vector<Float>& p,
                                 const std::vector<Float>& n);

    virtual void initializeShapeParamSet(ParamSet& paramSet,
                                         std::string indFile,
                                         std::string stFile, std::string pFile,
                                         std::string nFile);

    virtual void initializeShapeParamSet(ParamSet& paramSet,
                                         const std::vector<int>& indices,
                                         const std::vector<Float>& st,
                                         const std::vector<Float>& p,
                                         const std::vector<Float>& n);

    virtual void initializeGridMedium(
        std::string name, std::string medType, std::string trans,
        std::string maj, std::string min, std::string samp,
        std::vector<Float> sig_a, std::vector<Float> sig_s,
        std::vector<Float> t_p0, std::vector<Float> t_p1,
        std::vector<Float> minB, std::vector<Float> maxB,
        std::string denseFile);

    virtual void initializeVDBMedium(
        std::string name, std::string medType, std::string trans,
        std::string maj, std::string min, std::string samp,
        std::vector<Float> sig_a, std::vector<Float> sig_s,
        std::vector<Float> minB, std::vector<Float> maxB, bool worldSpace,
        bool clampDensities, bool retargetDensities, bool retainAspectRatio,
        Float scale, std::string vdb_file);

    virtual void initializeProcSphereMedium(
        std::string name, std::string medType, std::string trans,
        std::string maj, std::string min, std::string samp,
        std::vector<Float> sig_a, std::vector<Float> sig_s,
        std::vector<Float> minB, std::vector<Float> maxB, bool worldSpace,
        bool clampDensities, bool retargetDensities, bool retainAspectRatio,
        Float scale, std::string vdb_file);

    virtual void initializeSphereMedium(
        std::string name, std::string medType, std::string trans,
        std::string maj, std::string min, std::string samp,
        std::vector<Float> sig_a, std::vector<Float> sig_s,
        std::vector<Float> minB, std::vector<Float> maxB, bool worldSpace,
        bool clampDensities, bool retargetDensities, bool retainAspectRatio,
        Float scale, std::string vdb_file);

    virtual void initializeDiskMedium(
        std::string name, std::string medType, std::string trans,
        std::string maj, std::string min, std::string samp, Float proc_time,
        std::vector<Float> sig_a, std::vector<Float> sig_s,
        std::vector<Float> minB, std::vector<Float> maxB, bool worldSpace,
        bool clampDensities, bool retargetDensities, bool retainAspectRatio,
        Float scale, std::string vdb_file);

    virtual void initializeMedium(std::string name, std::string medType,
                                  std::string dense, std::string trans,
                                  std::string maj, std::string min,
                                  std::string samp, std::vector<Float> sig_a,
                                  std::vector<Float> sig_s,
                                  std::vector<Float> minB,
                                  std::vector<Float> maxB, Float scale = 1.0);

    virtual void initializeNonClassicalMedium(
        std::string name, std::string medType, std::string dense,
        std::string trans, std::string transType, std::string maj,
        std::string min, std::string samp, std::vector<Float> sig_a,
        std::vector<Float> sig_s, std::vector<Float> minB,
        std::vector<Float> maxB, Float scale = 1.0);

    virtual void initializeProceduralMedium(
        std::string name, std::string medType, std::string texName,
        std::string trans, std::string maj, std::string min, std::string samp,
        std::vector<Float> sig_a, std::vector<Float> sig_s,
        std::vector<Float> minB, std::vector<Float> maxB,
        std::vector<Float> scale, Float majorantScale = 1.0);

    virtual void initializeMedium(
        std::string name, std::string medType, std::string dense,
        std::string trans, std::string maj, std::string min, std::string samp,
        std::vector<Float> sig_a, std::vector<Float> sig_s,
        std::vector<Float> minB, std::vector<Float> maxB,
        std::string densityFile, Point3f p0_val, Point3f p1_val, Float interp,
        Float fixed, int nx_val, int ny_val, int nz_val);

    virtual void initializeInfiniteLightSource(std::string file,
                                               std::vector<Float> scle);

    virtual void initializeInfiniteLightSource(std::string file,
                                               std::vector<Float> scle,
                                               std::vector<Float> t_l);

    virtual void initializeDistantLightSource(std::vector<Float> L,
                                              Point3f from, Point3f to);

    virtual void initializePointLightSource(std::vector<Float> i_val);
    virtual void initializePointCorPLightSource(std::vector<Float> i_val,
                                                std::vector<Float> dir_val);

    virtual void initializeSpotLightSource(std::vector<Float> i_val,
                                           Point3f pfrom, Point3f pto,
                                           Float conea = 30.0,
                                           Float conedelt = 5.0);

    virtual void initializeMaterial_Matte(std::vector<Float> kd,
                                          Float sigma = 0.f);

    virtual void initializeMaterial_Matte(std::string tex_kd,
                                          Float sigma = 0.f);

    virtual void initializeMaterial_ReflectiveMatte(std::vector<Float> kd,
                                                    std::vector<Float> kr);

    virtual void initializeMaterial_ReflectiveMatte(std::string kd,
                                                    std::vector<Float> kr);

    // virtual void initializeMaterial_Substrate(std::vector<Float> kd,
    //                                     Float sigma = 0.f);

    virtual void initializeMaterial_Metal(std::string eta_spec,
                                          std::string k_spec, Float rough);

    virtual void initializeNamedMaterial_Matte(std::string name,
                                               std::vector<Float> kd,
                                               Float sigma = 0.f);

    virtual void initializeNamedMaterial_Matte(std::string name,
                                               std::string tex_kd,
                                               Float sigma = 0.f);

    virtual void initializeNamedMaterial_Substrate(std::string name,
                                                   std::vector<Float> ks,
                                                   std::string tex_kd,
                                                   Float u_rough, Float v_rough,
                                                   bool remap = false);

    virtual void initializeNamedMaterial_Mirror(std::string name);

    virtual void initializeNamedMaterial_Glass(std::string name,
                                               float index_val, float u_rough,
                                               float v_rough,
                                               bool remap = false);

    virtual void initializeNamedMaterial_Glass(std::string name,
                                               float index_val,
                                               bool remap = false);

    virtual void initializeNamedMaterial_Metal(std::string name,
                                               std::vector<Float> eta_spec,
                                               std::vector<Float> k_spec,
                                               float u_rough, float v_rough,
                                               bool remap = false);

    virtual void initializeNamedMaterial_Metal(std::string name,
                                               std::string eta_spec,
                                               std::string k_spec, Float rough);

    virtual void initializeNamedMaterial_Uber(std::string name,
                                              std::vector<Float> ks,
                                              std::vector<Float> kd,
                                              std::vector<Float> opacity);

    virtual void initializeTextureImg(std::string name, std::string type,
                                      std::string texname,
                                      std::string imagepath);

    virtual void initializeTextureImg(std::string name, std::string type,
                                      std::string texname,
                                      std::string imagepath, bool tri);

    virtual void initializeScaleTexture(std::string name, std::string type,
                                        std::string texname,
                                        std::string tex1_path,
                                        std::vector<Float> tex2_val);

    virtual void initializeNamedMaterial(std::string name, Float rough,
                                         Float ind, std::string typ,
                                         std::vector<Float> kd,
                                         std::vector<Float> ks,
                                         std::vector<Float> kt,
                                         std::vector<Float> opac);

    virtual void initializeNamedMaterial(std::string name, Float rough,
                                         Float ind, std::string typ,
                                         std::string kd_tex,
                                         std::vector<Float> ks,
                                         std::vector<Float> kt,
                                         std::vector<Float> opac);

    virtual void initializeDiffuseAreaLightSource(std::vector<Float> col);

    virtual void initializeDistantLightSource(std::vector<Float> col);

    virtual void initializeGlass(std::vector<Float> r, std::vector<Float> t,
                                 Float ind);

    virtual void initializeGlass(std::string r, std::string t, Float ind);

    virtual void initializeGlass(std::string r, Float ind);

    virtual void initializeHackyGlass(std::vector<Float> r,
                                      std::vector<Float> t, Float ind);

    virtual void initializeVoronoiSpiralTexture(
        std::string name, std::vector<Float> t_levelColors,
        std::vector<Float> t_amplitudes, std::vector<Float> t_phases,
        std::vector<Float> t_zPeriod, std::vector<Float> t_noisePeriod,
        std::vector<Float> t_noiseAmp, std::vector<Float> t_minB,
        std::vector<Float> t_maxB, Float t_radius, int t_levelPoints);

    virtual void initializePointCloudMedium(std::string name,
                                            std::vector<Float> points,
                                            std::vector<Float> densities,
                                            std::vector<Float> minB,
                                            std::vector<Float> maxB);

    virtual void initializeDeintegratedTextureMedium(std::string name,
                                                     std::string file,
                                                     std::vector<Float> minB,
                                                     std::vector<Float> maxB,
                                                     bool along_z = false);

    virtual void initializeChessTextureMedium(std::string name,
                                              std::string file,
                                              std::vector<Float> minB,
                                              std::vector<Float> maxB);

    virtual void initializeChessTextureMedium(
        std::string name, std::string file, std::vector<Float> minB,
        std::vector<Float> maxB, Float t_hue, Float t_sat, Float t_turb_period,
        Float t_xtrans, Float t_ytrans, Float t_ztrans, Float t_g_const,
        Float t_x_const, Float t_t_const, Float t_phase, Float t_power);

    virtual void initializeCloudTexture(
        std::string name, Float t_period, Float t_resolution,
        Float t_hardCodedMax, Float t_hardCodedMin, Float t_distPeriod,
        Float t_distFactor, Float t_threshStart, Float t_threshSlope,
        std::vector<Float> t_minB, std::vector<Float> t_maxB);

    virtual void initializePlyShape(std::string path);

    virtual void initializeSphereShape(Float radius);

    virtual void initializeBoxShape(Float minx, Float maxx, Float miny,
                                    Float maxy, Float minz, Float maxz);

    virtual void compareRMSE(std::string pathToGroundTruth,
                             std::vector<std::string>& imagePaths);

    std::string exp_path;
};
