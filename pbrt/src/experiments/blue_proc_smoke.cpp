#include "blue_proc_smoke.h"

BlueProcSmokeExperiment::BlueProcSmokeExperiment(pbrt::Options options) {
    // int samples = 8192 * 8;
    // int samples = 256;
    // int samples = 1024;
    // int samples = 2048;
    // int samples = 64;
    // int samples = 1;
    // int samples = 4096;
    int samples = 8192 * 2;

    std::string transType = "ratio";
    // std::string transType = "track_length";
    // std::string transType = "ratio_adap";
    // std::string transType = "track_length_adap";

    std::string scatType = "trans";
    // std::string scatType = "trans_adap";
    // std::string scatType = "rat";

    // std::string gridType = "vdb";
    std::string gridType = "const";
    // std::string gridType = "const_adap";
    // std::string gridType = "interval_adap";
    // std::string gridType = "interval_dense_adap";

    runEqualSampleRenders("../results/prog_smoke/nonbound/image", options,
                          transType, scatType, gridType, 0.01, samples, 5, 65);
}

void BlueProcSmokeExperiment::initializeSceneMediumBox(std::string transType,
                                                       std::string scatType,
                                                       std::string name,
                                                       std::string maj,
                                                       Float majorant) {
    pbrtAttributeBegin();
    pbrtScale(60.0, 60.0, 60.0);
    pbrtTranslate(0.0, 0.0, 0.5);

    Float min = -0.5;
    Float max = 0.5;

    initializeProcSphereMedium(
        "vol", "heterogeneous", transType,
        // maj,
        maj, "const",
        // "vdb",
        scatType,
        // "trans",        // delta tracking
        // "raymarch",       // raymarching
        // "unbiased_ray",   // unbiased raymarching
        // "unbiased_hom",
        // "unbiased_cor_ray", // more correllated unbiased raymarching
        // "unbiased_uniform",
        // "absorptive",  // smoke only
        std::vector<Float>{0.3, 0.3, 0.3},
        std::vector<Float>{0.7, 0.7, 0.7},  // delta / raymarching
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        false, true, false, false, majorant, name);

    pbrtMaterial("", ParamSet());
    pbrtMediumInterface("vol", "");

    // NOTE: These have to be the same as the min bounds
    initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();
}

void BlueProcSmokeExperiment::runEqualSampleRenders(
    std::string name, pbrt::Options options, std::string transType,
    std::string scatType, std::string gridType, Float majorant, int samples,
    int bounces, int r) {
    pbrtInit(options);

    std::string machine = "corneria";
    // std::string machine = "katina";

    if (IS_LINUX) {
        SetSearchDirectory(
            DirectoryContaining("/home/firal/Documents/Research/testscenes/"
                                "vdb_smoke/dragon_10.pbrt"));
        exp_path = "/home/firal/Documents/Research/testscenes/vdb_smoke/";
    } else {
        SetSearchDirectory(DirectoryContaining(
            "/Users/" + machine +
            "/Documents/Research/testscenes/vdb_smoke/dragon_10.pbrt"));
        exp_path =
            "/Users/" + machine + "/Documents/Research/testscenes/vdb_smoke/";
    }

    initializeIntegrator(200000000,
                         // 400000000,
                         -1, bounces,
                         // "volpath_recursive",
                         // "volpath_recursive_blanchet",
                         // "volpath_recursive_correlated_blanchet",
                         // "volpath_recursive_correlated_blanchet_cmf",
                         // "volpath_recursive_debias_correllated_n",
                         // "volpath_nonexp_debias",
                         "volpath",
                         //  "majorant",
                         // "volpath_recursive_debias_cust",
                         // "volpath",
                         "independent", "independent", false, false);

    initializeSampler("random", samples);

    // initializePixelFilter("box");
    initializePixelFilter("gaussian");
    initializeFilm("" + name, 1080 / 2, 1080 / 2);

    pbrtScale(-1.0, 1.0, 1.0);

    double theta = 6.0 * M_PI / 12.0;

    double xpos = -50.0 * cos(theta);
    double ypos = 30 * sin(theta) + 60.0;

    pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 27.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 55);  // big is at 70

    pbrtWorldBegin();

    initializeLightSources();

    initializeSceneMediumBox(transType, scatType, "bunny_cloud.vdb", gridType,
                             majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

void BlueProcSmokeExperiment::initializeLightSources() {
    Float scale = 5.0;

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{3.0 * scale, 1.5 * scale, 0.0 * scale},
        Point3f(1.0, -4.0, 1.5), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{0.0 * scale, 1.5 * scale, 3.0 * scale},
        Point3f(-5.0, -4.0, 11.0), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{0.0 * scale, 0.0 * scale, 1.0 * scale},
        Point3f(1.0, -1.0, 0.1), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();
}
