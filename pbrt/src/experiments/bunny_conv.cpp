#include "bunny_conv.h"

#if OPENVDB

BunnyConvergenceExperiment::BunnyConvergenceExperiment(pbrt::Options options) {
    int samples = 4096;
    // int samples = 256;
    // int samples = 16;

    pcg32 rng = pcg32(0xaccdc, 0xba53ba11);

    std::string transType = "raymarch";
    std::string scatType = "trans";

    for (int i = 0; i < 1000; ++i) {
        rng.nextUInt();
        rng.nextUInt();
        rng.nextUInt();
    }

    // for (int i = 0; i < ests.size(); ++i) {
    //     // runEqualCostRenders(
    //     //     rng, "../results/exp_smoke/sing_scat/equal_cost/" + ests[i],
    //     //     options, ests[i], scatType, 1.0, ext_calls, 1, 65);

    //     rng.nextUInt();
    //     rng.nextUInt();
    //     rng.nextUInt();
    // }

    // runEqualCostDebiasedRenders(
    //     rng, "../results/exp_smoke/sing_scat/equal_cost/tele", options,
    //     "raymarch", scatType, 1.0, ext_calls, 1, 65);

    // runEqualSampleDebiasedRenders(
    //     rng, "../results/raymarch_comparison/sandbox/take_36", options,
    //     "ratio", scatType, 3.0, samples, 20, 65);

    // runEqualSampleDebiasedRenders(
    //     rng, "../results/raymarch_comparison/gt_2/image", options, "ratio",
    //     scatType, 3.0, samples, 20, 65);

    runEqualSampleDebiasedRenders(rng, "../results/bunny_conv/full/image",
                                  options, "ratio", scatType, 3.0, samples, 20,
                                  65);

    // runEqualSampleDebiasedRenders(rng, "../results/bunny_conv/take_4/image",
    //                               options, "ratio", scatType, 3.0, samples,
    //                               20, 65);
}

void BunnyConvergenceExperiment::initializeSceneMediumBox(std::string transType,
                                                          std::string scatType,
                                                          std::string name,
                                                          std::string maj,
                                                          Float majorant) {
    pbrtAttributeBegin();

    // pbrtScale(60.0 * 57.7 / 57.7, 60.0 * 57.2 / 57.7, 60.0 * 43.8 / 57.7);
    pbrtScale(60.0 * 57.7 / 57.7, 60.0 * 43.8 / 57.7, 60.0 * 57.2 / 57.7);
    // pbrtRotate(90.0, 1.0, 0.0, 0.0);
    pbrtTranslate(0.0, 0.0, 0.5);
    pbrtRotate(90.0, 1.0, 0.0, 0.0);
    pbrtRotate(180.0, 0.0, 1.0, 0.0);

    Float min = -0.5;
    Float max = 0.5;

    Float dense_scale = 4.0;

    initializeVDBMedium(
        "vol", "heterogeneous", transType,
        // maj,
        // "const", "const",
        // "vdb", "const",
        // "interval_adap_vdb", "const",
        "interval_adap_vdb_track", "const",
        // "vdb",
        scatType,
        // "trans",        // delta tracking
        // "raymarch",       // raymarching
        // "unbiased_ray",   // unbiased raymarching
        // "unbiased_hom",
        // "unbiased_cor_ray", // more correllated unbiased raymarching
        // "unbiased_uniform",
        // "absorptive",  // smoke only
        std::vector<Float>{0.3 * dense_scale, 0.3 * dense_scale,
                           0.3 * dense_scale},
        std::vector<Float>{0.7 * dense_scale, 0.7 * dense_scale,
                           0.7 * dense_scale},  // delta / raymarching
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        false, false, false, false, majorant, name);

    pbrtMaterial("", ParamSet());
    pbrtMediumInterface("vol", "");

    // debugging logic
    // initializeMaterial_Matte(std::vector<Float>{1.0, 0.0, 0.0});

    // NOTE: These have to be the same as the min bounds
    initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();
}

void BunnyConvergenceExperiment::runEqualSampleRenders(
    pcg32& rng, std::string name, pbrt::Options options, std::string transType,
    std::string scatType, Float majorant, int samples, int bounces, int r) {
    pbrtInit(options);

    // SetSearchDirectory(DirectoryContaining("/Users/corneria/Documents/Research/testscenes/vdb_smoke/dragon_10.pbrt"));
    // exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

    std::string machine = RENDER_MACHINE;

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

    initializeIntegratorSeeded(200000000, -1, bounces, "volpath", "independent",
                               "independent", rng.nextUInt(), rng.nextUInt(),
                               rng.nextUInt(), false, false);

    initializeSampler("random", samples);

    initializePixelFilter("box");

    // initializeFilm("" + name,
    //                //"better_smoke_test_60",
    //                900, 1100);

    initializeFilm("" + name, 1080, 1080);

    pbrtScale(-1.0, 1.0, 1.0);

    double theta = 5.0 * M_PI / 12.0;

    double xpos = -50.0 * cos(theta);
    double ypos = 30 * sin(theta) + 60.0;

    // pbrtLookAt(-50.0, 60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0); // USE THIS FOR FUN

    // pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0);

    pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 27.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 50);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    // pbrtAttributeBegin();
    // initializeMaterial_Matte(std::vector<Float>{0.01, 0.01, 0.01});
    // // pbrtRotate(135.0, 0.0, 0.0, 1.0);
    // pbrtRotate(-50.0, 0.0, 0.0, 1.0);
    // pbrtTranslate(0.0, 0.0, -1.0);

    // pbrtScale(20.0, 100.0, 100.0);
    // initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
    // pbrtAttributeEnd();

    initializeSceneMediumBox(transType, scatType, "bunny_cloud.vdb", "vdb",
                             majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

void BunnyConvergenceExperiment::runEqualCostRenders(
    pcg32& rng, std::string name, pbrt::Options options, std::string transType,
    std::string scatType, Float majorant, int ext_calls, int bounces, int r) {
    pbrtInit(options);

    // SetSearchDirectory(DirectoryContaining("/Users/corneria/Documents/Research/testscenes/vdb_smoke/dragon_10.pbrt"));
    // exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

    std::string machine = RENDER_MACHINE;

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

    initializeIntegratorSeeded(200000000, ext_calls, bounces, "volpath",
                               "independent", "independent", rng.nextUInt(),
                               rng.nextUInt(), rng.nextUInt(), false, false);

    initializeSampler("random", 256);

    initializePixelFilter("box");

    // initializeFilm("" + name,
    //                //"better_smoke_test_60",
    //                900, 1100);

    initializeFilm("" + name, 1080, 1080);

    pbrtScale(-1.0, 1.0, 1.0);

    double theta = 5.0 * M_PI / 12.0;

    double xpos = -50.0 * cos(theta);
    double ypos = 30 * sin(theta) + 60.0;

    // pbrtLookAt(-50.0, 60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0); // USE THIS FOR FUN

    // pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0);

    pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 27.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 50);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    // pbrtAttributeBegin();
    // initializeMaterial_Matte(std::vector<Float>{0.01, 0.01, 0.01});
    // // pbrtRotate(135.0, 0.0, 0.0, 1.0);
    // pbrtRotate(-50.0, 0.0, 0.0, 1.0);
    // pbrtTranslate(0.0, 0.0, -1.0);

    // pbrtScale(20.0, 100.0, 100.0);
    // initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
    // pbrtAttributeEnd();

    initializeSceneMediumBox(transType, scatType, "bunny_cloud.vdb", "vdb",
                             majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

void BunnyConvergenceExperiment::runEqualSampleDebiasedRenders(
    pcg32& rng, std::string name, pbrt::Options options, std::string transType,
    std::string scatType, Float majorant, int samples, int bounces, int r) {
    pbrtInit(options);

    // SetSearchDirectory(DirectoryContaining("/Users/corneria/Documents/Research/testscenes/vdb_smoke/dragon_10.pbrt"));
    // exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

    std::string machine = RENDER_MACHINE;

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

    // initializeIntegratorSeeded(200000000, -1, bounces, "volpath_debias",
    //                            "independent", "independent", rng.nextUInt(),
    //                            rng.nextUInt(), rng.nextUInt(), false, false);

    // regular volpath
    // initializeIntegratorSeeded(200000000, -1, bounces, "volpath",
    // "independent",
    //                            "independent", rng.nextUInt(), rng.nextUInt(),
    //                            rng.nextUInt(), false, false);

    // majorant grid
    initializeIntegratorSeeded(200000000, -1, bounces, "majorant",
                               "independent", "independent", rng.nextUInt(),
                               rng.nextUInt(), rng.nextUInt(), false, false);

    initializeSampler("random", samples);

    initializePixelFilter("box");

    // initializeFilm("" + name,
    //                //"better_smoke_test_60",
    //                900, 1100);

    initializeFilm("" + name, 1080, 1080);

    pbrtScale(-1.0, 1.0, 1.0);

    double theta = 5.0 * M_PI / 12.0;

    double xpos = -50.0 * cos(theta);
    double ypos = 30 * sin(theta) + 60.0;

    // pbrtLookAt(-50.0, 60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0); // USE THIS FOR FUN

    // pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0);

    pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 27.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 50);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    // pbrtAttributeBegin();
    // initializeMaterial_Matte(std::vector<Float>{0.01, 0.01, 0.01});
    // // pbrtRotate(135.0, 0.0, 0.0, 1.0);
    // pbrtRotate(-50.0, 0.0, 0.0, 1.0);
    // pbrtTranslate(0.0, 0.0, -1.0);

    // pbrtScale(20.0, 100.0, 100.0);
    // initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
    // pbrtAttributeEnd();

    initializeSceneMediumBox(transType, scatType, "bunny_cloud.vdb", "vdb",
                             majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

void BunnyConvergenceExperiment::runEqualCostDebiasedRenders(
    pcg32& rng, std::string name, pbrt::Options options, std::string transType,
    std::string scatType, Float majorant, int ext_calls, int bounces, int r) {
    pbrtInit(options);

    // SetSearchDirectory(DirectoryContaining("/Users/corneria/Documents/Research/testscenes/vdb_smoke/dragon_10.pbrt"));
    // exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

    std::string machine = RENDER_MACHINE;

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

    initializeIntegratorSeeded(200000000, ext_calls, bounces, "volpath_debias",
                               "independent", "independent", rng.nextUInt(),
                               rng.nextUInt(), rng.nextUInt(), false, false);

    initializeSampler("random", 8192);

    initializePixelFilter("box");

    // initializeFilm("" + name,
    //                //"better_smoke_test_60",
    //                900, 1100);

    initializeFilm("" + name, 1080, 1080);

    pbrtScale(-1.0, 1.0, 1.0);

    double theta = 5.0 * M_PI / 12.0;

    double xpos = -50.0 * cos(theta);
    double ypos = 30 * sin(theta) + 60.0;

    // pbrtLookAt(-50.0, 60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0); // USE THIS FOR FUN

    // pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0);

    pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 27.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 50);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    // pbrtAttributeBegin();
    // initializeMaterial_Matte(std::vector<Float>{0.01, 0.01, 0.01});
    // // pbrtRotate(135.0, 0.0, 0.0, 1.0);
    // pbrtRotate(-50.0, 0.0, 0.0, 1.0);
    // pbrtTranslate(0.0, 0.0, -1.0);

    // pbrtScale(20.0, 100.0, 100.0);
    // initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
    // pbrtAttributeEnd();

    initializeSceneMediumBox(transType, scatType, "bunny_cloud.vdb", "vdb",
                             majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

void BunnyConvergenceExperiment::initializeLightSources() {
    // Float scale = 5.0;

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{2.0 * scale, 1.25 * scale, 0.75 * scale},
    //     Point3f(1.0, -4.0, 1.5), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{0.0 * scale, 1.5 * scale, 3.0 * scale},
    //     Point3f(-5.0, -4.0, 11.0), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{0.0 * scale, 0.0 * scale, 1.0 * scale},
    //     Point3f(1.0, -1.0, 0.1), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // Float scale = 5.0;

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{90.0 / 255.0 * scale, 138.0 / 255.0 * scale,
    //                        145.0 / 255.0 * scale},
    //     Point3f(1.0, -4.0, 1.5), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{239.0 / 255.0 * scale, 38.0 / 255.0 * scale,
    //                        72.0 / 255.0 * scale},
    //     Point3f(-5.0, -4.0, 11.0), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{251.0 / 255.0 * scale, 200.0 / 255.0 * scale,
    //                        0.0 / 255.0 * scale},
    //     Point3f(1.0, -1.0, 0.1), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // Float scale = 10.0;

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{253.0 / 255.0 * scale * 2, 8.0 / 255.0 * scale *
    //     2,
    //                        74.0 / 255.0 * scale * 2},
    //     Point3f(1.0, -4.0, 1.5), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{73.0 / 255.0 * scale * 1.1,
    //                        79.0 / 255.0 * scale * 1.1,
    //                        193.0 / 255.0 * scale * 1.1},
    //     Point3f(-5.0, -4.0, 11.0), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{16.0 / 255.0 * scale * 2, 254.0 / 255.0 * scale *
    //     2,
    //                        251.0 / 255.0 * scale * 2},
    //     Point3f(1.0, -1.0, 0.1), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    Float scale = 10.0;

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{0.0 / 255.0 * scale * 1.6, 0.0 / 255.0 * scale * 1.6,
                           255.0 / 255.0 * scale * 4},
        Point3f(1.0, -4.0, 1.5), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{73.0 / 255.0 * scale * 1.1,
    //                        79.0 / 255.0 * scale * 1.1,
    //                        193.0 / 255.0 * scale * 1.1},
    //     Point3f(-5.0, -4.0, 11.0), Point3f(0.0, 0.0, 0.0));

    // initializeDistantLightSource(
    //     std::vector<Float>{45.0 / 255.0 * scale * 1.6,
    //                        45.0 / 255.0 * scale * 1.6,
    //                        255.0 / 255.0 * scale * 1.6},
    //     Point3f(-5.0, -4.0, 11.0), Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    initializeDistantLightSource(
        std::vector<Float>{255.0 / 255.0 * scale * 4, 80.0 / 255.0 * scale * 4,
                           0.0 / 255.0 * scale * 4},
        Point3f(-5.0, -4.0, 11.0), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{16.0 / 255.0 * scale * 2, 254.0 / 255.0 * scale * 2,
                           251.0 / 255.0 * scale * 2},
        Point3f(1.0, -1.0, 0.1), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();
}

#endif
