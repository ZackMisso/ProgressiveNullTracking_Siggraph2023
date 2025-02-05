#include "dis_cloud.h"

#if OPENVDB

DisneyCloud::DisneyCloud(pbrt::Options options) {
    // runEqualExtRenders(options, "ratio", 1.0, 0);

    pcg32 rng = pcg32(0xacdc, 0x3137);

    // int runs = 2048;
    // for (int i = 0; i < runs; ++i) {
    //     std::string cmd =
    //         "mkdir ../results/disney_cloud/batch/" + std::to_string(i) + "/";
    //     system(cmd.c_str());

    //     if (i < 0) {
    //         rng.nextUInt();
    //         rng.nextUInt();
    //         rng.nextUInt();
    //     } else {
    //         findAngle_batchRun(rng, options, "ratio", 1.0, 0.0, 0, i);
    //     }  // TODO
    // }

    // < 70
    // int runs = 90;
    // for (int i = 0; i < runs; ++i) {
    //     Float angle = 4 * i * M_PI / 180.0;
    //     std::string cmd = "mkdir ../results/disney_cloud/batch_iso/" +  //
    //     batch
    //                       std::to_string(4 * i) + "/";
    //     system(cmd.c_str());

    //     if (i < 70) {
    //         rng.nextUInt();
    //         rng.nextUInt();
    //         rng.nextUInt();
    //     } else {
    //         findAngle_batchRun(rng, options, "ratio", 1.0, angle, 0, 4 * i);
    //     }  // TODO
    // }

    // findAngle_batchRun(rng, options, "ratio", 1.0, M_PI, 0, 0);

    // findAngle_batchRun(rng, options, "ratio", 1.0, 0.0, 0, 0);
    // findAngle_batchRun(rng, options, "ratio", 1.0, 0.0, 0, 0);
    Globals::NO_CLAMP = false;
    Globals::WRITE_INCREMENTS = true;
    findAngle_batchRun(rng, options, "ratio", 1.0, 0.0, 0, 0);
}

void DisneyCloud::initializeSceneMediumBox(std::string transType,
                                           std::string name,
                                           std::string scatType,
                                           Float majorant) {
    pbrtAttributeBegin();

    pbrtScale(60.0 * 994.0 / 1225.0, 60.0, 60.0 * 676.0 / 1225.0);
    // pbrtScale(60.0, 60.0, 60.0);
    pbrtTranslate(0.0, 0.0, 0.5);

    // initializeGlass(std::vector<Float>{1.0, 1.0, 1.0},
    //                 std::vector<Float>{1.0, 1.0, 1.0},
    //                 1.5);

    pbrtTranslate(-0.02, 0.0, 0.0);
    pbrtRotate(90.0, 1.0, 0.0, 0.0);

    Float min = -0.5;
    Float max = 0.5;

    Float scale = 2.0;

    // initializeVDBMedium(
    //     "vol", "heterogeneous", transType, "interval_adap_vdb_track",
    //     "const", scatType,  // trans, maj, min, scat std::vector<Float>{0.01
    //     * scale, 0.01 * scale, 0.01 * scale}, std::vector<Float>{0.99 *
    //     scale, 0.99 * scale, 0.99 * scale}, std::vector<Float>{min, min,
    //     min}, std::vector<Float>{max, max, max}, false, false, false, false,
    //     majorant, name);

    initializeVDBMedium(
        "vol", "heterogeneous", transType, "interval_adap_vdb_track", "const",
        scatType,  // trans, maj, min, scat
        std::vector<Float>{0.01 * scale, 0.01 * scale, 0.01 * scale},
        std::vector<Float>{0.99 * scale, 0.99 * scale, 0.99 * scale},
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        false, false, false, false, majorant, name);

    pbrtTranslate(0.02, 0.0, 0.0);

    pbrtMediumInterface("vol", "");
    // initializeGlass(std::vector<Float>{1.0, 1.0, 1.0},
    //                 std::vector<Float>{1.0, 1.0, 1.0},
    //                 1.5);
    pbrtMaterial("", ParamSet());
    initializeBoxShape(min, max, min, max, min, max);

    // pbrtMediumInterface("vol", "");

    // debugging logic
    // initializeMaterial_Matte(std::vector<Float>{1.0, 0.0, 0.0});

    // NOTE: These have to be the same as the min bounds
    // initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    initializeMaterial_ReflectiveMatte(std::vector<Float>{0.0, 0.0, 0.0},
                                       std::vector<Float>{0.9, 0.9, 0.9});

    // initializeMaterial_ReflectiveMatte(std::vector<Float>{0.0, 0.0, 0.0},
    //                                    std::vector<Float>{1.0, 1.0, 1.0});

    initializeTrimesh(std::vector<int>{0, 1, 2, 2, 3, 0},
                      std::vector<Float>{-100000.0, -100000.0, -1.5, 100000.0,
                                         -100000.0, -1.5, 100000.0, 100000.0,
                                         -1.5, -100000.0, 100000.0, -1.5});
    pbrtAttributeEnd();
}

void DisneyCloud::findAngle_batchRun(pcg32& rng, pbrt::Options options,
                                     std::string transType, Float majorant,
                                     Float angle, long samples, int index) {
    pbrtInit(options);

    // SetSearchDirectory(DirectoryContaining("/Users/corneria/Documents/Research/testscenes/sphere_smoke/dragon_10.pbrt"));
    // exp_path = "/Users/corneria/Documents/Research/testscenes/sphere_smoke/";

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
                               5000000000, 100,
                               //  "volpath_nonexp_debias",
                               // "volpath_nonexp",
                               "volpath",
                               //"volpath_nonexp_debias",
                               "independent", "independent", rng.nextUInt(),
                               rng.nextUInt(), rng.nextUInt(), false, false);

    // initializeSampler("random", 8192 * 8);
    // initializeSampler("random", 16);
    initializeSampler("random", 128);
    // initializeSampler("random", 64);

    initializePixelFilter("box");

    // initializeFilm("../results/disney_cloud/take_34/image", 800, 600);
    initializeFilm("../results/disney_cloud/final_ours/image", 800, 600);

    // initializeFilm(
    //     "../results/disney_cloud/take_6/" + std::to_string(index) + "/image",
    //     800, 600);

    pbrtScale(-1.0, 1.0, 1.0);

    // pbrtLookAt(130.0 * cos(angle), 130.0 * sin(angle), 10.0, 0.0, 0.0,
    //            15.0,  // was at .4
    //            0.0, 0.0, 1.0);

    pbrtLookAt(130.0 * cos(angle), 130.0 * sin(angle), -1.0, 0.0, 0.0,
               10.0,  // was at .4
               0.0, 0.0, 1.0);

    // old
    // pbrtLookAt(130.0 * cos(angle), 130.0 * sin(angle), 10.0, 0.0, 0.0,
    //            15.0,  // was at .4
    //            0.0, 0.0, 1.0);

    initializeCamera("perspective", 25);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    initializeSceneMediumBox(transType, "wdas_cloud_half.vdb",  // "vdb",
                             "trans_weight", majorant);

    // initializeSceneMediumBox(transType, "wdas_cloud_half.vdb",  // "vdb",
    //                          "trans", majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

void DisneyCloud::findAngle(pbrt::Options options, std::string transType,
                            Float majorant, Float angle, long samples) {
    // pbrtInit(options);

    // //
    // SetSearchDirectory(DirectoryContaining("/Users/corneria/Documents/Research/testscenes/sphere_smoke/dragon_10.pbrt"));
    // // exp_path =
    // "/Users/corneria/Documents/Research/testscenes/sphere_smoke/";

    // std::string machine = RENDER_MACHINE;

    // if (IS_LINUX) {
    //     SetSearchDirectory(
    //         DirectoryContaining("/home/firal/Documents/Research/testscenes/"
    //                             "sphere_smoke/dragon_10.pbrt"));
    //     exp_path = "/home/firal/Documents/Research/testscenes/sphere_smoke/";
    // } else {
    //     SetSearchDirectory(DirectoryContaining(
    //         "/Users/" + machine +
    //         "/Documents/Research/testscenes/sphere_smoke/dragon_10.pbrt"));
    //     exp_path = "/Users/" + machine +
    //                "/Documents/Research/testscenes/sphere_smoke/";
    // }

    // initializeIntegrator(200000000,
    //                      // samples,
    //                      -1, 65,
    //                      //  "volpath_nonexp_debias",
    //                      // "volpath_nonexp",
    //                      //  "volpath",
    //                      "volpath_nonexp_debias", "independent",
    //                      "independent", false, false);

    // // initializeSampler("random", 64);
    // // initializeSampler("random", 8192);
    // initializeSampler("random", 8192);
    // // initializeSampler("random", 16);

    // initializePixelFilter("box");

    // initializeFilm("../results/davis_cloud/conv/davis_75_deb",
    //                //"better_smoke_test_60",
    //                800, 600);

    // pbrtScale(-1.0, 1.0, 1.0);

    // pbrtLookAt(130.0 * cos(angle), 130.0 * sin(angle), 10.0, 0.0, 0.0,
    //            15.0,  // was at .4
    //            0.0, 0.0, 1.0);

    // initializeCamera("perspective", 25);  // was at 30

    // pbrtWorldBegin();

    // initializeLightSources();

    // initializeSceneMediumBox(transType, "wdas_cloud_half.vdb", "vdb",
    //                          // "const",
    //                          majorant);

    // pbrtWorldEnd();

    // pbrtCleanup();
}

void DisneyCloud::initializeLightSources() {
    pbrtAttributeBegin();

    Float env_scale = 1.0;
    initializeInfiniteLightSource(
        "textures/white.png",
        std::vector<Float>{0.1 * env_scale, 0.3 * env_scale,
                           0.8 * env_scale});  // 1.46
    pbrtAttributeEnd();

    pbrtAttributeBegin();

    // Float sun_scale = 0.85;

    // initializeDistantLightSource(
    //     std::vector<Float>{10.0 * sun_scale, 4.0 * sun_scale, 0.0 *
    //     sun_scale}, Point3f(1.0, 2.0, 3.0), Point3f(0.0, 0.0, 0.0));

    Float sun_scale = 0.7;  // take 30, 31

    initializeDistantLightSource(
        std::vector<Float>{6.0 * sun_scale, 6.0 * sun_scale, 6.0 * sun_scale},
        Point3f(1.0, 2.0, 3.0), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();
}

#endif
