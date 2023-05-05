#include "figure_1.h"

#include "trans_samplers/pcg32.h"

// this file sets up the renders used in our teaser figure.
// the actual teaser rendered gt, progressive, ratio_noclamp_nolearn,
// and ratio_clamp_nolearn at 8192 spp, while also visualizing our
// progressive method's acceleration structure before 1 pixel finished,
// after 32 spp, and after all 8192 spp.
//
// all renders except for gt (because it uses a global majorant)
// also output 2D slices of the entire acceleration structure after
// rendering.

Figure_1::Figure_1(pbrt::Options options) {
    system("mkdir ../../results/figure_1/");
    system("mkdir ../../results/figure_1/gt/");
    system("mkdir ../../results/figure_1/ratio_noclamp_nolearn/");
    system("mkdir ../../results/figure_1/ratio_noclamp_learn/");
    system("mkdir ../../results/figure_1/ratio_clamp_nolearn/");
    system("mkdir ../../results/figure_1/progressive/");
    system("mkdir ../../results/figure_1/adap_progressive/");

    bool render_gt = true;  // global bounding majorant
    bool render_ratio_noclamp_nolearn = true;
    bool render_ratio_noclamp_learn = true;
    bool render_ratio_clamp_nolearn = true;  // our biasing step
    bool render_progressive = true;          // our progressive method
    bool render_adap_progressive =
        true;  // our progressive method + adaptive ratio imp sampling

    int samples = 128;
    std::string transType;  // the technique used for transmittance estimation
    std::string scatType;   // the technique used for ff-sampling
    std::string gridType;   // the acceleration datastructure

    pcg32 rng = pcg32(0xcacd, 0x129a);

    // render gt (ratio w/ bounding initialization)
    if (render_gt) {
        transType = "ratio";
        scatType = "trans_weight";
        gridType = "const";

        runEqualSampleRenders("../../results/figure_1/gt/gt", options,
                              transType, scatType, gridType, 3.0, samples, 5,
                              65);
    }

    // render ratio (bad initialization + no learning bounds)
    if (render_ratio_noclamp_nolearn) {
        transType = "ratio";
        scatType = "trans_weight";
        gridType = "interval_adap_vdb_constant";

        Globals::NO_CLAMP = true;

        runEqualSampleRenders(
            "../../results/figure_1/ratio_noclamp_nolearn/"
            "ratio_nolearn_noclamp",
            options, transType, scatType, gridType, 0.01, samples, 5, 65);
    }

    // render ratio (bad initialization + learning bounds)
    if (render_ratio_noclamp_learn) {
        transType = "ratio";
        scatType = "trans_weight";
        gridType = "interval_adap_vdb_track";

        Globals::NO_CLAMP = true;

        runEqualSampleRenders(
            "../../results/figure_1/ratio_noclamp_learn/"
            "ratio_noclamp_learn",
            options, transType, scatType, gridType, 3.0, samples, 5, 65);
    }

    // render ratio clamped with no bounds learning
    if (render_ratio_clamp_nolearn) {
        transType = "ratio";
        scatType = "trans_weight";
        gridType = "interval_adap_vdb_constant";

        Globals::NO_CLAMP = false;

        runEqualSampleRenders(
            "../../results/figure_1/ratio_clamp_nolearn/ratio_clamp_nolearn",
            options, transType, scatType, gridType, 0.01, samples, 5, 65);
    }

    // render ours (ratio with clamping while learning bounds)
    if (render_progressive) {
        transType = "ratio";
        scatType = "trans_weight";
        gridType = "interval_adap_vdb_track";

        Globals::NO_CLAMP = false;

        runEqualSampleRenders("../../results/figure_1/progressive/progressive",
                              options, transType, scatType, gridType, 3.0,
                              samples, 5, 65);
    }

    // render ours (ratio with clamping while learning bounds)
    if (render_adap_progressive) {
        transType = "ratio_adap";
        scatType = "trans_weight";
        gridType = "interval_adap_vdb_track";

        Globals::NO_CLAMP = false;

        runEqualSampleRenders(
            "../../results/figure_1/adap_progressive/adap_progressive", options,
            transType, scatType, gridType, 3.0, samples, 5, 65);
    }
}

void Figure_1::initializeSceneMediumBox(std::string transType,
                                        std::string scatType, std::string name,
                                        std::string maj, Float majorant) {
    pbrtAttributeBegin();

    // I hope this works
    // pbrtScale(60.0 * 57.7 / 57.7, 60.0 * 43.8 / 57.7, 60.0 * 57.2 / 57.7);
    pbrtScale(60.0, 60.0, 60.0);
    // pbrtScale(40.0, 40.0, 40.0);
    pbrtTranslate(0.0, 0.0, 0.5);

    // Float min = -0.5;
    // Float max = 0.5;

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

void Figure_1::runEqualSampleRenders(std::string name, pbrt::Options options,
                                     std::string transType,
                                     std::string scatType, std::string gridType,
                                     Float majorant, int samples, int bounces,
                                     int r) {
    pbrtInit(options);

    std::string machine = "corneria";
    // std::string machine = "katina";
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

    initializeIntegrator(200000000, -1, bounces, "majorant", "independent",
                         "independent", false, false);

    initializeSampler("random", samples);

    initializePixelFilter("gaussian");

    initializeFilm("" + name, 1080 / 2, 1080 / 2);

    // use this for the actual scene
    pbrtScale(-1.0, 1.0, 1.0);

    double theta = 6.0 * M_PI / 12.0;

    double xpos = -50.0 * cos(theta);
    double ypos = 30 * sin(theta) + 60.0;

    // use this for the actual scene
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

void Figure_1::initializeLightSources() {
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
