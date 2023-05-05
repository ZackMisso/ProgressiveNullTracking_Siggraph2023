#include "figure_9.h"

#if OPENVDB

Figure_9::Figure_9(pbrt::Options options) {
    pcg32 rng = pcg32(0xacdc, 0x3137);

    system("mkdir ../../results/figure_10/");
    system("mkdir ../../results/figure_10/ratio/");
    system("mkdir ../../results/figure_10/progressive/");
    system("mkdir ../../results/figure_10/pseries_cmf/");
    system("mkdir ../../results/figure_10/ray_debiased/");
    system("mkdir ../../results/figure_10/ray_kettunen/");
    system("mkdir ../../results/figure_10/equal_sample/");
    system("mkdir ../../results/figure_10/equal_sample/ratio/");
    system("mkdir ../../results/figure_10/equal_sample/progressive/");
    system("mkdir ../../results/figure_10/equal_sample/pseries_cmf/");
    system("mkdir ../../results/figure_10/equal_sample/ray_debiased/");
    system("mkdir ../../results/figure_10/equal_sample/ray_kettunen/");

    long num_ext_calls = 5000000000;

    Globals::NO_CLAMP = false;
    Globals::WRITE_INCREMENTS = true;

    equal_cost_run(rng, options, num_ext_calls, "ratio/ratio", "ratio", false);
    equal_cost_run(rng, options, num_ext_calls, "pseries_cmf/pseries_cmf",
                   "pseries_cdf", false);
    equal_cost_run(rng, options, num_ext_calls, "ray_debiased/ray_debiased",
                   "debiased_raymarch", false);
    equal_cost_run(rng, options, num_ext_calls, "ray_kettunen/ray_kettunen",
                   "exp_nvidia", false);
    equal_cost_run(rng, options, num_ext_calls, "progressive/progressive",
                   "ratio", true);

    // equal sample renders for correctness confirmation
    int num_samples = 128;
    // equal_sample_run(rng, options, num_samples, "equal_sample/ratio/ratio",
    //                  "ratio", false);
    // equal_sample_run(rng, options, num_samples,
    //                  "equal_sample/progressive/progressive", "ratio", true);
    // equal_sample_run(rng, options, num_samples,
    //                  "equal_sample/ray_debiased/ray_debiased",
    //                  "debiased_raymarch", false);
    // equal_sample_run(rng, options, num_samples,
    //                  "equal_sample/pseries_cmf/pseries_cmf", "pseries_cdf",
    //                  false);
    // equal_sample_run(rng, options, num_samples,
    //                  "equal_sample/ray_kettunen/ray_kettunen", "exp_nvidia",
    //                  false);
}

void Figure_9::initializeSceneMediumBox(std::string transType, std::string name,
                                        std::string scatType, Float majorant,
                                        bool is_progressive) {
    pbrtAttributeBegin();

    pbrtScale(60.0 * 994.0 / 1225.0, 60.0, 60.0 * 676.0 / 1225.0);
    pbrtTranslate(0.0, 0.0, 0.5);

    pbrtTranslate(-0.02, 0.0, 0.0);
    pbrtRotate(90.0, 1.0, 0.0, 0.0);

    Float min = -0.5;
    Float max = 0.5;

    Float scale = 2.0;

    std::string accel_struct = "interval_adap_vdb_tight";
    if (is_progressive) {
        accel_struct = "interval_adap_vdb_track";
    }

    initializeVDBMedium(
        "vol", "heterogeneous", transType, accel_struct, "const",
        scatType,  // trans, maj, min, scat
        std::vector<Float>{0.01 * scale, 0.01 * scale, 0.01 * scale},
        std::vector<Float>{0.99 * scale, 0.99 * scale, 0.99 * scale},
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        false, false, false, false, majorant, name);

    pbrtTranslate(0.02, 0.0, 0.0);

    pbrtMediumInterface("vol", "");
    pbrtMaterial("", ParamSet());
    initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    initializeMaterial_ReflectiveMatte(std::vector<Float>{0.0, 0.0, 0.0},
                                       std::vector<Float>{0.9, 0.9, 0.9});

    initializeTrimesh(std::vector<int>{0, 1, 2, 2, 3, 0},
                      std::vector<Float>{-100000.0, -100000.0, -1.5, 100000.0,
                                         -100000.0, -1.5, 100000.0, 100000.0,
                                         -1.5, -100000.0, 100000.0, -1.5});
    pbrtAttributeEnd();
}

void Figure_9::equal_cost_run(pcg32& rng, pbrt::Options options, long ext_calls,
                              std::string path, std::string transType,
                              bool is_progressive) {
    pbrtInit(options);

    Float majorant = 1.0;
    Float angle = 0.0;

    std::string machine = RENDER_MACHINE;

    if (IS_LINUX) {
        SetSearchDirectory(
            DirectoryContaining("/home/firal/Documents/Research/testscenes/"
                                "sphere_smoke/dragon_10.pbrt"));
        exp_path = "/home/firal/Documents/Research/testscenes/sphere_smoke/";
    } else {
        SetSearchDirectory(DirectoryContaining(
            "/Users/" + machine +
            "/Documents/Research/testscenes/sphere_smoke/dragon_10.pbrt"));
        exp_path = "/Users/" + machine +
                   "/Documents/Research/testscenes/sphere_smoke/";
    }

    initializeIntegratorSeeded(200000000,
                               // samples,
                               ext_calls, 100, "volpath",
                               //"volpath_nonexp_debias",
                               "independent", "independent", rng.nextUInt(),
                               rng.nextUInt(), rng.nextUInt(), false, false);

    initializeSampler("random", 128);

    initializePixelFilter("box");

    initializeFilm("../../results/figure_10/" + path, 800, 600);

    pbrtScale(-1.0, 1.0, 1.0);

    pbrtLookAt(130.0 * cos(angle), 130.0 * sin(angle), -1.0, 0.0, 0.0, 10.0,
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 25);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    initializeSceneMediumBox(transType, "wdas_cloud_half.vdb", "trans_weight",
                             majorant, is_progressive);

    pbrtWorldEnd();

    pbrtCleanup();
}

void Figure_9::equal_sample_run(pcg32& rng, pbrt::Options options, int samples,
                                std::string path, std::string transType,
                                bool is_progressive) {
    pbrtInit(options);

    Float majorant = 1.0;
    Float angle = 0.0;

    std::string machine = RENDER_MACHINE;

    if (IS_LINUX) {
        SetSearchDirectory(
            DirectoryContaining("/home/firal/Documents/Research/testscenes/"
                                "sphere_smoke/dragon_10.pbrt"));
        exp_path = "/home/firal/Documents/Research/testscenes/sphere_smoke/";
    } else {
        SetSearchDirectory(DirectoryContaining(
            "/Users/" + machine +
            "/Documents/Research/testscenes/sphere_smoke/dragon_10.pbrt"));
        exp_path = "/Users/" + machine +
                   "/Documents/Research/testscenes/sphere_smoke/";
    }

    initializeIntegratorSeeded(200000000,
                               // samples,
                               -1, 100, "volpath",
                               //"volpath_nonexp_debias",
                               "independent", "independent", rng.nextUInt(),
                               rng.nextUInt(), rng.nextUInt(), false, false);

    initializeSampler("random", samples);

    initializePixelFilter("box");

    initializeFilm("../../results/figure_10/" + path, 800, 600);

    pbrtScale(-1.0, 1.0, 1.0);

    pbrtLookAt(130.0 * cos(angle), 130.0 * sin(angle), -1.0, 0.0, 0.0, 10.0,
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 25);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    initializeSceneMediumBox(transType, "wdas_cloud_half.vdb", "trans_weight",
                             majorant, is_progressive);

    pbrtWorldEnd();

    pbrtCleanup();
}

void Figure_9::initializeLightSources() {
    pbrtAttributeBegin();

    Float env_scale = 1.0;
    initializeInfiniteLightSource(
        "textures/white.png",
        std::vector<Float>{0.1 * env_scale, 0.3 * env_scale, 0.8 * env_scale});
    pbrtAttributeEnd();

    pbrtAttributeBegin();

    Float sun_scale = 0.7;

    initializeDistantLightSource(
        std::vector<Float>{6.0 * sun_scale, 6.0 * sun_scale, 6.0 * sun_scale},
        Point3f(1.0, 2.0, 3.0), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();
}

#endif
