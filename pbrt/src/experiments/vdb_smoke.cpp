#include "vdb_smoke.h"

#if OPENVDB

VDB_Smoke_Tests::VDB_Smoke_Tests(pbrt::Options options) {
    // int start = 66;
    // int end = 67;
    //
    // for (int i = start; i < end; ++i)
    //     runTest("ratio", true, "simple_smoke_" + std::to_string(i) + ".vdb");

    // runTest("ratio", true, "simple_smoke_66.vdb", 0);

    // for (int i = 2; i < 240; ++i)
    //     runTest("ratio", false, "swirly_smoke_" + std::to_string(i) + ".vdb",
    //     i);

    std::vector<Float> majorants = std::vector<Float>();
    // majorants.push_back(0.1);
    // // majorants.push_back(0.2);
    // majorants.push_back(0.3);
    // // majorants.push_back(0.4);
    // // majorants.push_back(0.5);
    // majorants.push_back(0.6);
    // // majorants.push_back(0.7);
    // // majorants.push_back(0.8);
    // // majorants.push_back(0.9);
    // majorants.push_back(1.0);
    // majorants.push_back(1.5);
    // majorants.push_back(2.0);
    // // majorants.push_back(2.5);
    // // majorants.push_back(3.5);
    // majorants.push_back(5.0);
    // // majorants.push_back(7.5);
    // majorants.push_back(10.0);
    // // majorants.push_back(15.0);
    // // majorants.push_back(20.0);

    // majorants.push_back(0.1);
    // majorants.push_back(0.3);
    // majorants.push_back(0.6);
    majorants.push_back(20.0);
    // majorants.push_back(1.5);
    // majorants.push_back(2.5);
    // majorants.push_back(5.0);

    std::vector<Float> const_extCalls = std::vector<Float>();
    // const_extCalls.push_back(40000000 * 2);
    // // const_extCalls.push_back(40000000 * 2);
    // const_extCalls.push_back(40000000 * 2);
    // // const_extCalls.push_back(40000000 * 2);
    // // const_extCalls.push_back(40000000 * 2);
    // const_extCalls.push_back(48166386 * 2);
    // // const_extCalls.push_back(56744152 * 2);
    // // const_extCalls.push_back(65349042 * 2);
    // // const_extCalls.push_back(73944685 * 2);
    // const_extCalls.push_back(82572722 * 2);
    // const_extCalls.push_back(125802342 * 2);
    // const_extCalls.push_back(169161497 * 2);
    // // const_extCalls.push_back(212562451 * 2);
    // // const_extCalls.push_back(299497124 * 2);
    // const_extCalls.push_back(430045736 * 2);
    // // const_extCalls.push_back(647817883 * 2);
    // const_extCalls.push_back(865709986 * 2);
    // // const_extCalls.push_back(1301676808 * 2);
    // // const_extCalls.push_back(1737764972 * 2);

    // const_extCalls.push_back(40000000 * 2);
    // const_extCalls.push_back(40000000 * 2);
    // const_extCalls.push_back(48166386 * 2);
    const_extCalls.push_back(82572722 * 2);
    // const_extCalls.push_back(125802342 * 2);
    // const_extCalls.push_back(212562451 * 2);
    // const_extCalls.push_back(430045736 * 2);

    std::vector<Float> macro_extCalls = std::vector<Float>();
    // macro_extCalls.push_back(45388457 * 2);
    // macro_extCalls.push_back(56976634 * 2);
    // macro_extCalls.push_back(68558811 * 2);
    // macro_extCalls.push_back(138051169 * 2);
    // macro_extCalls.push_back(253859384 * 2);

    std::vector<Float> equalSamples = std::vector<Float>();
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);

    equalSamples.push_back(128);
    //
    // equalSamples.push_back(10000);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // //
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);
    // equalSamples.push_back(8);

    std::vector<std::string> estimators = std::vector<std::string>();
    // estimators.push_back("track_length");
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

    // equal ext call renders
    // for (int j = 0; j < const_extCalls.size(); ++j)
    // {
    //     for (int i = 0; i < estimators.size(); ++i)
    //     {
    //         std::string mk_loc = "mkdir
    //         /Users/corneria/Documents/Research/pbrttest/build/vdb_figure/" +
    //         std::to_string(majorants[j]); std::system(mk_loc.c_str());
    //
    //         runEqualExtRenders(options, estimators[i], majorants[j],
    //         const_extCalls[j]);
    //     }
    // }

    // // // equal random pixel samples
    for (int j = 0; j < equalSamples.size(); ++j) {
        for (int i = 0; i < estimators.size(); ++i) {
            std::string mk_loc =
                "mkdir "
                "/Users/corneria/Documents/Research/pbrttest/build/"
                "vdb_figure_strat/" +
                std::to_string(majorants[j]) + "_rand";
            std::system(mk_loc.c_str());

            runEqualSampleRenders(options, estimators[i], majorants[j],
                                  equalSamples[j]);
        }
    }
    //
    // // equal stratified pixel samples
    // for (int j = 0; j < equalSamples.size(); ++j)
    // {
    //     for (int i = 0; i < estimators.size(); ++i)
    //     {
    //         std::string mk_loc = "mkdir
    //         /Users/corneria/Documents/Research/pbrttest/build_final/vdb_figure_strat/"
    //         + std::to_string(majorants[j]) + "_strat";
    //         std::system(mk_loc.c_str());
    //
    //         runEqualSampleStratifiedRenders(options, estimators[i],
    //         majorants[j], equalSamples[j]);
    //     }
    // }

    // macrogrid - equal ext call renders
    // for (int i = 0; i < estimators.size(); ++i)
    // {
    //     for (int j = 0; j < macro_extCalls.size(); ++j)
    //     {
    //         std::string mk_loc = "mkdir
    //         /Users/corneria/Documents/Research/pbrttest/build_final/vdb_macro_figure/"
    //         + std::to_string(majorants[j]); std::system(mk_loc.c_str());
    //
    //         runEqualExtRenders_Macrogrid(estimators[i], majorants[j],
    //         macro_extCalls[j]);
    //     }
    // }

    // macrogrid - equal random pixel samples
    // for (int i = 0; i < estimators.size(); ++i)
    // {
    //     for (int j = 0; j < equalSamples.size(); ++j)
    //     {
    //         std::string mk_loc = "mkdir
    //         /Users/corneria/Documents/Research/pbrttest/build_final/vdb_macro_figure_rand/"
    //         + std::to_string(majorants[j]); std::system(mk_loc.c_str());
    //
    //         runEqualSampleRenders_Macrogrid(estimators[i], majorants[j],
    //         equalSamples[j]);
    //     }
    // }

    // macrogrid - equal stratified pixel samples
    // for (int i = 0; i < estimators.size(); ++i)
    // {
    //     for (int j = 0; j < equalSamples.size(); ++j)
    //     {
    //         std::string mk_loc = "mkdir
    //         /Users/corneria/Documents/Research/pbrttest/build_final/vdb_macro_figure_strat/"
    //         + std::to_string(majorants[j]); std::system(mk_loc.c_str());
    //
    //         runEqualSampleStratifiedRenders_Macrogrid(estimators[i],
    //         majorants[j], equalSamples[j]);
    //     }
    // }
}

void VDB_Smoke_Tests::initializeSceneMediumBox(std::string transType,
                                               std::string name,
                                               std::string maj,
                                               Float majorant) {
    pbrtAttributeBegin();

    // I hope this works
    pbrtScale(60.0, 60.0, 60.0);
    pbrtTranslate(0.0, 0.0, 0.5);

    Float min = -0.5;
    Float max = 0.5;

    // std::cout << "WHAT" << std::endl;

    initializeVDBMedium(
        "vol", "heterogeneous", transType, maj, "const",
        "trans",  // delta tracking
        // "raymarch",       // raymarching
        // "unbiased_ray",   // unbiased raymarching
        // "unbiased_hom",
        // "unbiased_cor_ray", // more correllated unbiased raymarching
        // "unbiased_uniform",
        // "absorptive",  // smoke only
        std::vector<Float>{0.3, 0.3, 0.3},
        // std::vector<Float>{0.0, 0.0, 0.0}, // smoke only
        std::vector<Float>{0.7, 0.7, 0.7},  // delta / raymarching
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        false, true, false, true, majorant, name);

    pbrtMaterial("", ParamSet());
    pbrtMediumInterface("vol", "");

    // debugging logic
    // initializeMaterial_Matte(std::vector<Float>{1.0, 0.0, 0.0});

    // NOTE: These have to be the same as the min bounds
    initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();
}

// void VDB_Smoke_Tests::runTest(std::string transType, bool isGT,
//                               std::string name, int iter) {
//     SetSearchDirectory(
//         DirectoryContaining("/Users/corneria/Documents/Research/testscenes/"
//                             "vdb_smoke/dragon_10.pbrt"));
//     exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

//     initializeIntegrator(200000000,
//                          // 100000000,
//                          -1, 1, "volpath_recursive",
//                          //"volpath_recursive_blanchet",
//                          // "volpath_recursive_correlated_blanchet",
//                          //"volpath",
//                          "stream", "independent", false, isGT);

//     initializeSampler("random", 24000);

//     initializePixelFilter("box");

//     initializeFilm(  //"vdb_smoke_tests/take_5_" + std::to_string(iter) + "_"
//     +
//                      // transType,
//         "gt_vdb", 900, 1100);

//     pbrtScale(-1.0, 1.0, 1.0);

//     // pbrtLookAt(2.8 + 24.0, -2.8 - 24.0, 15.5,
//     // -1.8 - 15.0, 1.8 + 15.0, 13.5, // was at .4
//     // 0.0, 0.0, 15.5, // was at .4
//     // 0.0, 0.0, 1.0);

//     pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0,
//                33.0,  // was at .4
//                       // 0.0, 0.0, 15.5, // was at .4
//                0.0, 0.0, 1.0);

//     initializeCamera("perspective", 35);  // was at 30

//     pbrtWorldBegin();

//     initializeLightSources();

//     pbrtAttributeBegin();
//     initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
//     pbrtRotate(135.0, 0.0, 0.0, 1.0);
//     pbrtTranslate(0.0, 0.0, -1.0);

//     pbrtScale(20.0, 100.0, 100.0);
//     // pbrtTranslate(0.0, 0.0, -30.0);
//     initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
//     pbrtAttributeEnd();

//     // for alignment purposes only
//     // pbrtAttributeBegin();
//     // initializeMaterial_Matte(std::vector<Float>{1.0, 0.0, 0.0});
//     // initializeSphereShape(1.0);
//     // pbrtAttributeEnd();

//     // initializeSceneMedium(transType, name);
//     initializeSceneMediumBox(transType, name, "const", 1.0);

//     pbrtWorldEnd();
// }

// void VDB_Smoke_Tests::runEqualExtRenders(pbrt::Options options,
//                                          std::string transType, Float
//                                          majorant, int samples) {
//     pbrtInit(options);

//     SetSearchDirectory(
//         DirectoryContaining("/Users/corneria/Documents/Research/testscenes/"
//                             "vdb_smoke/dragon_10.pbrt"));
//     exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

//     initializeIntegrator(200000000, samples,
//                          // -1,
//                          5,
//                          //"volpath_recursive",
//                          //"volpath_recursive_blanchet",
//                          "volpath_recursive_correlated_blanchet",
//                          //"volpath",
//                          "independent", "independent", false, false);

//     initializeSampler("random", 8192);

//     initializePixelFilter("box");

//     initializeFilm("vdb_figure/" + std::to_string(majorant) + "/" +
//     transType,
//                    //"better_smoke_test_60",
//                    900, 1100);

//     pbrtScale(-1.0, 1.0, 1.0);

//     pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
//                0.0, 0.0, 1.0);

//     initializeCamera("perspective", 35);  // was at 30

//     pbrtWorldBegin();

//     initializeLightSources();

//     pbrtAttributeBegin();
//     initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
//     pbrtRotate(135.0, 0.0, 0.0, 1.0);
//     pbrtTranslate(0.0, 0.0, -1.0);

//     pbrtScale(20.0, 100.0, 100.0);
//     initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
//     pbrtAttributeEnd();

//     initializeSceneMediumBox(transType, "simple_smoke_66.vdb", "const",
//                              majorant);

//     pbrtWorldEnd();

//     pbrtCleanup();
// }

void VDB_Smoke_Tests::runEqualSampleRenders(pbrt::Options options,
                                            std::string transType,
                                            Float majorant, int samples) {
    pbrtInit(options);

    SetSearchDirectory(
        DirectoryContaining("/Users/corneria/Documents/Research/testscenes/"
                            "vdb_smoke/dragon_10.pbrt"));
    exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

    initializeIntegrator(200000000,
                         // 400000000,
                         -1, 5,  //"volpath_recursive",
                         // "volpath_recursive_blanchet",
                         //  "volpath_recursive_correlated_blanchet",
                         "volpath", "independent", "independent", false, false);

    initializeSampler("random", samples);

    initializePixelFilter("box");

    initializeFilm("../results/verify_ff/scene_setup/take_8", 900 / 2,
                   1100 / 2);

    pbrtScale(-1.0, 1.0, 1.0);

    pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 35);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    /*pbrtAttributeBegin();
    initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
    pbrtRotate(135.0, 0.0, 0.0, 1.0);
    pbrtTranslate(0.0, 0.0, -1.0);

    pbrtScale(20.0, 100.0, 100.0);
    initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
    pbrtAttributeEnd();*/

    initializeSceneMediumBox(transType, "simple_smoke_66.vdb", "const",
                             majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

// void VDB_Smoke_Tests::runEqualExtRenders_Macrogrid(std::string transType,
//                                                    Float majorant,
//                                                    int samples) {
//     SetSearchDirectory(
//         DirectoryContaining("/Users/corneria/Documents/Research/testscenes/"
//                             "vdb_smoke/dragon_10.pbrt"));
//     exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

//     initializeIntegrator(200000000, samples,
//                          // -1,
//                          1,
//                          //"volpath_recursive",
//                          // "volpath_recursive_blanchet",
//                          "volpath_recursive_correlated_blanchet",
//                          //"volpath",
//                          "independent", "independent", false, false);

//     initializeSampler("random", 8192);

//     initializePixelFilter("box");

//     initializeFilm(
//         "vdb_macro_figure/" + std::to_string(majorant) + "/" + transType,
//         //"better_smoke_test_60",
//         900, 1100);

//     pbrtScale(-1.0, 1.0, 1.0);

//     pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
//                0.0, 0.0, 1.0);

//     initializeCamera("perspective", 35);  // was at 30

//     pbrtWorldBegin();

//     initializeLightSources();

//     pbrtAttributeBegin();
//     initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
//     pbrtRotate(135.0, 0.0, 0.0, 1.0);
//     pbrtTranslate(0.0, 0.0, -1.0);

//     pbrtScale(20.0, 100.0, 100.0);
//     initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
//     pbrtAttributeEnd();

//     initializeSceneMediumBox(transType, "simple_smoke_66.vdb", "vdb",
//     majorant);

//     pbrtWorldEnd();
// }

// void VDB_Smoke_Tests::runEqualSampleRenders_Macrogrid(std::string transType,
//                                                       Float majorant,
//                                                       int samples) {
//     SetSearchDirectory(
//         DirectoryContaining("/Users/corneria/Documents/Research/testscenes/"
//                             "vdb_smoke/dragon_10.pbrt"));
//     exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

//     initializeIntegrator(200000000,
//                          // 400000000,
//                          -1, 5,
//                          //"volpath_recursive",
//                          // "volpath_recursive_blanchet",
//                          "volpath_recursive_correlated_blanchet",
//                          //"volpath",
//                          "independent", "independent", false, false);

//     initializeSampler("random", samples);

//     initializePixelFilter("box");

//     initializeFilm("vdb_macro_figure_rand/" + std::to_string(majorant) + "/"
//     +
//                        transType + "_rand",
//                    //"better_smoke_test_60",
//                    900, 1100);

//     pbrtScale(-1.0, 1.0, 1.0);

//     pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
//                0.0, 0.0, 1.0);

//     initializeCamera("perspective", 35);  // was at 30

//     pbrtWorldBegin();

//     initializeLightSources();

//     pbrtAttributeBegin();
//     initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
//     pbrtRotate(135.0, 0.0, 0.0, 1.0);
//     pbrtTranslate(0.0, 0.0, -1.0);

//     pbrtScale(20.0, 100.0, 100.0);
//     initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
//     pbrtAttributeEnd();

//     initializeSceneMediumBox(transType, "simple_smoke_66.vdb", "vdb",
//     majorant);

//     pbrtWorldEnd();
// }

// void VDB_Smoke_Tests::runEqualSampleStratifiedRenders_Macrogrid(
//     std::string transType, Float majorant, int samples) {
//     SetSearchDirectory(
//         DirectoryContaining("/Users/corneria/Documents/Research/testscenes/"
//                             "vdb_smoke/dragon_10.pbrt"));
//     exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

//     initializeIntegrator(200000000,
//                          // 400000000,
//                          -1, 5,
//                          //"volpath_recursive",
//                          // "volpath_recursive_blanchet",
//                          "volpath_recursive_correlated_blanchet",
//                          //"volpath",
//                          "stream", "independent", false, false);

//     initializeSampler("random", samples);

//     initializePixelFilter("box");

//     initializeFilm("vdb_macro_figure_strat/" + std::to_string(majorant) + "/"
//     +
//                        transType + "_strat",
//                    //"better_smoke_test_60",
//                    900, 1100);

//     pbrtScale(-1.0, 1.0, 1.0);

//     pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
//                0.0, 0.0, 1.0);

//     initializeCamera("perspective", 35);  // was at 30

//     pbrtWorldBegin();

//     initializeLightSources();

//     pbrtAttributeBegin();
//     initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
//     pbrtRotate(135.0, 0.0, 0.0, 1.0);
//     pbrtTranslate(0.0, 0.0, -1.0);

//     pbrtScale(20.0, 100.0, 100.0);
//     initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
//     pbrtAttributeEnd();

//     initializeSceneMediumBox(transType, "simple_smoke_66.vdb", "vdb",
//     majorant);

//     pbrtWorldEnd();
// }

// void VDB_Smoke_Tests::runEqualSampleStratifiedRenders(pbrt::Options options,
//                                                       std::string transType,
//                                                       Float majorant,
//                                                       int samples) {
//     pbrtInit(options);

//     SetSearchDirectory(
//         DirectoryContaining("/Users/corneria/Documents/Research/testscenes/"
//                             "vdb_smoke/dragon_10.pbrt"));
//     exp_path = "/Users/corneria/Documents/Research/testscenes/vdb_smoke/";

//     initializeIntegrator(200000000,
//                          // 400000000,
//                          -1, 5,
//                          //"volpath_recursive",
//                          // "volpath_recursive_blanchet",
//                          "volpath_recursive_correlated_blanchet",
//                          //"volpath",
//                          "stream", "independent", false, false);

//     initializeSampler("random", samples);

//     initializePixelFilter("box");

//     initializeFilm("vdb_figure_strat/" + std::to_string(majorant) + "_strat/"
//     +
//                        transType + "_strat",
//                    //"better_smoke_test_60",
//                    900, 1100);

//     pbrtScale(-1.0, 1.0, 1.0);

//     pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
//                0.0, 0.0, 1.0);

//     initializeCamera("perspective", 35);  // was at 30

//     pbrtWorldBegin();

//     initializeLightSources();

//     pbrtAttributeBegin();
//     initializeMaterial_Matte(std::vector<Float>{0.56, 0.56, 0.56});
//     pbrtRotate(135.0, 0.0, 0.0, 1.0);
//     pbrtTranslate(0.0, 0.0, -1.0);

//     pbrtScale(20.0, 100.0, 100.0);
//     initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
//     pbrtAttributeEnd();

//     initializeSceneMediumBox(transType, "simple_smoke_66.vdb", "const",
//                              majorant);

//     pbrtWorldEnd();

//     pbrtCleanup();
// }

void VDB_Smoke_Tests::initializeLightSources() {
    pbrtAttributeBegin();

    initializeDistantLightSource(std::vector<Float>{2.0, 2.0, 2.0},
                                 Point3f(1.0, -4.0, 1.5),
                                 Point3f(0.0, 0.0, 0.0));

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
