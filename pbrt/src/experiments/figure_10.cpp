#include "figure_10.h"

#if OPENVDB

Figure_10::Figure_10(pbrt::Options options) {
    system("mkdir ../../results/figure_11/");
    system("mkdir ../../results/figure_11/ratio/");
    system("mkdir ../../results/figure_11/progressive/");

    // int samples = 4096;
    int samples = 16;

    pcg32 rng = pcg32(0xaccdc, 0xba53ba11);
    std::string scatType = "trans";
    std::string transType = "ratio";

    for (int i = 0; i < 1000; ++i) {
        // just some additional random decisions to spice things up
        rng.nextUInt();
        rng.nextUInt();
        rng.nextUInt();
    }

    Globals::ACCEL_SCENE = 2;
    runEqualSampleRenders(rng, "../../results/figure_11/ratio/ratio", options,
                          transType, scatType, samples, false);
    runEqualSampleRenders(rng,
                          "../../results/figure_11/progressive/progressive",
                          options, transType, scatType, samples, true);
}

void Figure_10::initializeSceneMediumBox(std::string transType,
                                         std::string scatType, std::string name,
                                         std::string maj, Float majorant,
                                         bool is_progressive) {
    pbrtAttributeBegin();

    pbrtScale(60.0 * 57.7 / 57.7, 60.0 * 43.8 / 57.7, 60.0 * 57.2 / 57.7);
    pbrtTranslate(0.0, 0.0, 0.5);
    pbrtRotate(90.0, 1.0, 0.0, 0.0);
    pbrtRotate(180.0, 0.0, 1.0, 0.0);

    Float min = -0.5;
    Float max = 0.5;

    Float dense_scale = 4.0;

    std::string accel_struct = "interval_adap_vdb_tight";
    if (is_progressive) {
        accel_struct = "interval_adap_vdb_track";
    }

    initializeVDBMedium(
        "vol", "heterogeneous", transType, accel_struct, "const", scatType,
        std::vector<Float>{0.3 * dense_scale, 0.3 * dense_scale,
                           0.3 * dense_scale},
        std::vector<Float>{0.7 * dense_scale, 0.7 * dense_scale,
                           0.7 * dense_scale},  // delta / raymarching
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        false, false, false, false, majorant, name);

    pbrtMaterial("", ParamSet());
    pbrtMediumInterface("vol", "");

    // NOTE: These have to be the same as the min bounds
    initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();
}

void Figure_10::runEqualSampleRenders(pcg32& rng, std::string name,
                                      pbrt::Options options,
                                      std::string transType,
                                      std::string scatType, int samples,
                                      bool is_progressive) {
    pbrtInit(options);

    Float majorant = 3.0;
    int bounces = 20;

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

    // majorant grid
    initializeIntegratorSeeded(200000000, -1, bounces, "majorant",
                               "independent", "independent", rng.nextUInt(),
                               rng.nextUInt(), rng.nextUInt(), false, false);

    initializeSampler("random", samples);

    initializePixelFilter("box");

    initializeFilm("" + name, 1080, 1080);

    pbrtScale(-1.0, 1.0, 1.0);

    double theta = 5.0 * M_PI / 12.0;

    double xpos = -50.0 * cos(theta);
    double ypos = 30 * sin(theta) + 60.0;

    pbrtLookAt(xpos, ypos, 40.5, 0.0, 0.0, 27.0, 0.0, 0.0, 1.0);

    initializeCamera("perspective", 50);

    pbrtWorldBegin();

    initializeLightSources();

    initializeSceneMediumBox(transType, scatType, "bunny_cloud.vdb", "vdb",
                             majorant, is_progressive);

    pbrtWorldEnd();

    pbrtCleanup();
}

void Figure_10::initializeLightSources() {
    Float scale = 10.0;

    pbrtAttributeBegin();

    initializeDistantLightSource(
        std::vector<Float>{0.0 / 255.0 * scale * 1.6, 0.0 / 255.0 * scale * 1.6,
                           255.0 / 255.0 * scale * 4},
        Point3f(1.0, -4.0, 1.5), Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

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
