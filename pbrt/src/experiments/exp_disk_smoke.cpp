#include "exp_disk_smoke.h"

#if OPENVDB

ExpDiskSmoke_Tests::ExpDiskSmoke_Tests(pbrt::Options options) {
    // int samples = 8192 * 4;
    // int samples = 256;
    // int samples = 1024;
    // int samples = 64;
    // int samples = 1;
    // int samples = 8;
    int samples = 8192;

    std::cout << "STARTING ADAPTIVE TESTS" << std::endl;

    // std::string transType = "raymarch";
    // std::string scatType = "unbiased_ray";

    // std::string transType = "ratio";
    // std::string scatType = "trans";

    std::string transType = "ratio";
    // std::string transType = "track_length";
    // std::string transType = "ratio_adap";
    // std::string transType = "track_length_adap";

    std::string scatType = "trans";
    // std::string scatType = "trans_adap";
    // std::string scatType = "rat";

    // std::string gridType = "vdb";
    // std::string gridType = "const";
    // std::string gridType = "const_adap";
    // std::string gridType = "interval_adap";
    std::string gridType = "interval_dense_adap";

    // runEqualSampleRenders("../results/paper/davis/volpath_fixed", options,
    // transType, scatType, 0.1, samples, 2, 65);
    // runEqualSampleRenders("../results/paper/fuckme/ratio", options,
    // transType, scatType, 10.0, samples, 2, 65);
    // runEqualSampleRenders("../results/exp_smoke/adap_scat/gt_vdb", options,
    // transType,
    //                       scatType, gridType, 0.33, samples, 5, 65);
    // Float time = 1.0;
    // runEqualSampleRenders("../hypn_results/disk_smoke/first/12_take",
    // options,
    //                       transType, scatType, gridType, 1.0, samples, 2, 65,
    //                       time);

    int num_frames = 120;
    Float max_maj = 2.0;

    for (int i = 0; i <= num_frames; ++i) {
        // for (int i = 0; i < 60; ++i) {
        // for (int i = 100; i <= 120; ++i) {
        Float time = Float(i) / Float(num_frames);

        // std::string base_path =
        //     "../hypn_results/disk_smoke/" + std::to_string(i) + "/";
        // std::string base_path = "../hypn_results/disk_smoke/no_update_clamp/"
        // +
        //                         std::to_string(i) + "/";
        // std::string base_path =
        //     "../hypn_results/disk_smoke/no_update_no_clamp/" +
        //     std::to_string(i) + "/";
        // std::string base_path =
        //     "../hypn_results/disk_smoke/progressive/" + std::to_string(i) +
        //     "/";
        std::string base_path =
            "../hypn_results/disk_smoke/maj/" + std::to_string(i) + "/";
        std::string cmd = "mkdir " + base_path;
        system(cmd.c_str());

        runEqualSampleRenders(base_path + "image", options, transType, scatType,
                              gridType, 1.0, samples, 2, 65, time);

        // runEqualSampleRenders(base_path + "maj_image", options, transType,
        //                       scatType, gridType, 1.0, samples, 2, 65, time);
    }

    // int num_frames = 120;
    // Float max_maj = 2.0;

    // for (int i = 1; i <= num_frames; ++i) {
    //     Float maj = max_maj * Float(i) / Float(num_frames);

    //     std::string base_path =
    //         "../hypn_results/sphere_smoke/" + std::to_string(i) + "/";
    //     std::string cmd = "mkdir " + base_path;
    //     system(cmd.c_str());

    //     runEqualSampleRenders(base_path + "image", options, transType,
    //     scatType,
    //                           gridType, maj, samples, 5, 65);
    // }
}

void ExpDiskSmoke_Tests::initializeSceneMediumBox(std::string transType,
                                                  std::string scatType,
                                                  std::string name,
                                                  std::string maj,
                                                  Float majorant, Float time) {
    pbrtAttributeBegin();

    // pbrtScale(60.0 * 57.7 / 57.7, 60.0 * 43.8 / 57.7, 60.0 * 57.2
    // / 57.7);
    pbrtScale(60.0, 60.0, 60.0);
    pbrtTranslate(0.0, 0.0, 0.5);

    // Float min = -0.5;
    // Float max = 0.5;

    Float min = -0.5;
    Float max = 0.5;

    initializeDiskMedium(
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
        time, std::vector<Float>{0.3, 0.3, 0.3},
        std::vector<Float>{0.7, 0.7, 0.7},  // delta / raymarching
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        false, true, false, false, majorant, name);

    pbrtMaterial("", ParamSet());
    pbrtMediumInterface("vol", "");

    // debugging logic
    // initializeMaterial_Matte(std::vector<Float>{1.0, 0.0, 0.0});

    // NOTE: These have to be the same as the min bounds
    initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();
}

void ExpDiskSmoke_Tests::runEqualSampleRenders(
    std::string name, pbrt::Options options, std::string transType,
    std::string scatType, std::string gridType, Float majorant, int samples,
    int bounces, int r, Float time) {
    pbrtInit(options);

    // SetSearchDirectory(DirectoryContaining("/Users/corneria/Documents/Research/testscenes/vdb_smoke/dragon_10.pbrt"));
    // exp_path =
    // "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

    std::string machine = "corneria";
    // std::string machine = "fortuna";
    // std::string machine = "sauria";
    // std::string machine = "titania";

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
                         //  "volpath",
                         "majorant",
                         // "volpath_recursive_debias_cust",
                         // "volpath",
                         "independent", "independent", false, false);

    initializeSampler("random", samples);

    initializePixelFilter("box");

    // initializeFilm("" + name,
    //                //"better_smoke_test_60",
    //                900, 1100);

    // initializeFilm("" + name, 1080, 1080);
    initializeFilm("" + name, 1080 / 4, 1080 / 4);

    pbrtScale(-1.0, 1.0, 1.0);

    double theta = 6.0 * M_PI / 12.0;

    double xpos = -50.0 * cos(theta);
    double ypos = 30 * sin(theta) + 60.0;

    // pbrtLookAt(-50.0, 60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0); // USE THIS FOR FUN

    // pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 33.0,  // was at .4
    //            0.0, 0.0, 1.0);

    pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 27.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 55);  // big is at 70

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

    initializeSceneMediumBox(transType, scatType, "bunny_cloud.vdb", gridType,
                             majorant, time);

    pbrtWorldEnd();

    pbrtCleanup();
}

void ExpDiskSmoke_Tests::initializeLightSources() {
    // pbrtAttributeBegin();

    // initializeDistantLightSource(std::vector<Float>{2.0, 2.0, 2.0},
    //                              Point3f(1.0, -4.0, 1.5),
    //                              Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(std::vector<Float>{3.0, 3.0, 3.0},
    //                              Point3f(-5.0, -4.0, 11.0),
    //                              Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(std::vector<Float>{3.0, 3.0, 3.0},
    //                              Point3f(1.0, -1.0, 0.1),
    //                              Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

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

    // everything below is for fun
    Float scale = 5.0;

    // pbrtAttributeBegin();

    // initializeDistantLightSource(
    //     std::vector<Float>{3.0 * scale, 1.5 * scale, 0.0 * scale},
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

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{0.0 * scale, 0.0 * scale, 5.0 * scale},
        Point3f(1.0, -4.0, 1.5), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{3.0 * scale, 1.5 * scale, 0.0 * scale},
        Point3f(-5.0, -4.0, 11.0), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{0.0 * scale, 1.5 * scale, 3.0 * scale},
        Point3f(1.0, -1.0, 0.1), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();
}

#endif
