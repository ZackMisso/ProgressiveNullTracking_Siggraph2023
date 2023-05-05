#include "ff_verify.h"

#if OPENVDB

FF_Verify::FF_Verify(pbrt::Options options) {
    std::vector<Float> majorants = std::vector<Float>();
    majorants.push_back(20.0);

    std::vector<Float> equalSamples = std::vector<Float>();
    equalSamples.push_back(32);

    std::vector<std::string> estimators = std::vector<std::string>();
    estimators.push_back("ratio");
    // estimators.push_back("raymarch");
    // estimators.push_back("next_flight_ratio");
    // estimators.push_back("unidirectional");
    // estimators.push_back("pseries_cumulative");
    // estimators.push_back("pseries_cdf");
    // estimators.push_back("blanchet");
    // estimators.push_back("blanchet_naive_maj");
    // estimators.push_back("blanchet_bias");
    // estimators.push_back("blanchet_osmc");
    // estimators.push_back("blanchet_maj");
    // estimators.push_back("pseries_rr");
    // estimators.push_back("pseries_ratio");
    // estimators.push_back("pseries_next_flight_ratio");
    // estimators.push_back("pseries_interp");
    // estimators.push_back("bidirectional");

    // runEqualSampleRenders(options, "ratio", 20.0, 8);
    runEqualSampleRenders(options, "ratio", 0.1, 8192);
}

void FF_Verify::initializeSceneMediumBox(std::string transType,
                                         std::string name, std::string maj,
                                         Float majorant) {
    pbrtAttributeBegin();

    // I hope this works
    // pbrtScale(60.0, 60.0, 60.0);

    Float squeeze = 0.75;
    Float stretch = 1.2;

    // pbrtScale(46 * 60.0 / 57.2, 40.8 * 60.0 / 57.2, 60.0);
    // pbrtScale(46 * 60.0 / 57.2 * squeeze, 40.8 * 60.0 / 57.2 *
    // squeeze, 60.0); pbrtScale(40.8 * 60.0 / 57.2, 46 * 60.0 / 57.2, 60.0);
    pbrtScale(46 * 60.0 / 57.2 * squeeze, 40.8 * 60.0 / 57.2 * squeeze,
              60.0 * stretch);
    // pbrtRotate(90.0, 1.0, 0.0, 0.0);
    // pbrtScale(46 * 60.0 / 57.2 * squeeze, 40.8 * 60.0 / 57.2 *
    // squeeze, 60.0);
    // pbrtRotate(90.0, -1.0, 0.0, 0.0);
    // pbrtRotate(180.0, 0.0, 0.0, 1.0);
    // pbrtTranslate(0.0, 0.0, 0.55);
    pbrtTranslate(0.0, 0.0, 0.46);
    pbrtRotate(90.0, -1.0, 0.0, 0.0);

    // pbrtRotate(90.0, 1.0, 0.0, 0.0);

    Float min = -0.5;
    Float max = 0.5;

    Float scale = 1.0;

    // std::cout << "WHAT" << std::endl;

    initializeVDBMedium(
        "vol", "heterogeneous", transType, "interval_adap_vdb", "const",
        "trans",  // delta tracking
        std::vector<Float>{0.8 * scale, 0.8 * scale, 0.8 * scale},
        // std::vector<Float>{0.0, 0.0, 0.0}, // smoke only
        std::vector<Float>{0.7 * scale, 0.7 * scale,
                           0.7 * scale},  // delta / raymarching
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        // false, true, false, true, majorant, name);
        false, false, false, false, majorant, name);

    // initializeVDBMedium(
    //     "vol", "heterogeneous", transType, maj, "const",
    //     "trans",  // delta tracking
    //     std::vector<Float>{0.3 * scale, 0.3 * scale, 0.3 * scale},
    //     // std::vector<Float>{0.0, 0.0, 0.0}, // smoke only
    //     std::vector<Float>{0.7 * scale, 0.7 * scale,
    //                        0.7 * scale},  // delta / raymarching
    //     std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
    //     // false, true, false, true, majorant, name);
    //     false, false, false, false, majorant, name);

    pbrtMaterial("", ParamSet());
    pbrtMediumInterface("vol", "");

    // NOTE: These have to be the same as the min bounds
    initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();
}

void FF_Verify::runEqualSampleRenders(pbrt::Options options,
                                      std::string transType, Float majorant,
                                      int samples) {
    pbrtInit(options);

    // SetSearchDirectory(
    //     DirectoryContaining("/Users/corneria/Documents/Research/testscenes/"
    //                         "vdb_smoke/dragon_10.pbrt"));
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

    initializeIntegrator(200000000, -1,
                         5,  //"volpath_recursive",
                             // "volpath_recursive_blanchet",
                             //  "volpath_recursive_correlated_blanchet",
                         "volpath", "independent", "independent", false, false);

    initializeSampler("random", samples);

    initializePixelFilter("box");

    initializeFilm("../results/verify_ff/gt/image", 540, 1080);

    pbrtScale(-1.0, 1.0, 1.0);

    pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
               0.0, 0.0, -1.0);

    // initializeCamera("perspective", 85);  // was at 35
    initializeCamera("perspective", 35);  // was at 35

    pbrtWorldBegin();

    initializeLightSources();

    /*pbrtAttributeBegin();
    initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
    pbrtRotate(135.0, 0.0, 0.0, 1.0);
    pbrtTranslate(0.0, 0.0, -1.0);

    pbrtScale(20.0, 100.0, 100.0);
    initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
    pbrtAttributeEnd();*/

    pbrtAttributeBegin();
    // initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
    // initializeMaterial_Matte(std::vector<Float>{0.02, 0.02, 0.02});
    // initializeMaterial_ReflectiveMatte(std::vector<Float>{0.0, 0.0, 0.0},
    //                                    std::vector<Float>{0.8, 0.8, 0.8});
    initializeNamedMaterial_Glass("glass", 1.5, 0.5, 0.6, false);
    pbrtRotate(135.0, 0.0, 0.0, 1.0);
    pbrtRotate(180.0, 1.0, 0.0, 0.0);
    pbrtTranslate(0.0, 0.0, -70.0);

    // pbrtScale(20.0, 100.0, 100.0);
    // pbrtScale(2.0, 10.0, 10.0);
    pbrtScale(100.0, 80.0, 300.0);
    initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
    pbrtAttributeEnd();

    initializeSceneMediumBox(transType, "simple_smoke_66.vdb", "const",
                             majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

void FF_Verify::initializeLightSources() {
    // pbrtAttributeBegin();

    // initializeDistantLightSource(std::vector<Float>{12.0, 12.0, 12.0},
    //                              Point3f(1.0, -4.0, 1.5),
    //                              Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(std::vector<Float>{12.0, 1.0, 0.0},
    //                              Point3f(1.0, -4.0, -1.5),
    //                              Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    // pbrtAttributeBegin();

    // initializeDistantLightSource(std::vector<Float>{0.0, 0.0, 20.0},
    //                              Point3f(-1.0, -4.0, 0.0),
    //                              Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();

    pbrtAttributeBegin();

    initializeDistantLightSource(std::vector<Float>{2.0, 2.0, 2.0},
                                 Point3f(0.0, 0.0, -1.0),
                                 Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    // Float env_scale = 0.7;
    // Float env_scale = 7.0;
    Float env_scale = 2.0;
    initializeInfiniteLightSource(
        "textures/white.png",
        std::vector<Float>{0.1 * env_scale, 0.3 * env_scale,
                           0.6 * env_scale});  // 1.46
    pbrtAttributeEnd();

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

    // pbrtAttributeBegin();

    // initializeDistantLightSource(std::vector<Float>{3.0, 3.0, 3.0},
    //                              Point3f(1.0, -40.0, -1.0),
    //                              Point3f(0.0, 0.0, 0.0));

    // pbrtAttributeEnd();
}

#endif
