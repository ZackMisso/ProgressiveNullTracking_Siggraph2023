#include "figure_5.h"

Figure_5::Figure_5(pbrt::Options options) {
    // initialize random number generator
    pcg32 rng = pcg32(0xacdc, 0x3137);

    // create results directory
    system("mkdir ../../results/figure_5/");

    // THIS FIGURE REQUIRES THAT FIGURE 9 HAS ALREADY BEEN GENERATED

    // render the bounding image
    gtBounding(rng, options);
    // render the non-bounding but clamped image
    gtNonBounding(rng, options);
    // generate the graphs
    // system("python3 ../../ConvergenceRater/prog_scripts/figure_5.py");
}

void Figure_5::initializeSceneMediumBox(std::string transType, std::string name,
                                        std::string scatType,
                                        std::string vdbType, Float majorant) {
    pbrtAttributeBegin();

    pbrtScale(60.0 * 994.0 / 1225.0, 60.0, 60.0 * 676.0 / 1225.0);
    pbrtTranslate(0.0, 0.0, 0.5);

    pbrtTranslate(-0.02, 0.0, 0.0);
    pbrtRotate(90.0, 1.0, 0.0, 0.0);

    Float min = -0.5;
    Float max = 0.5;

    Float scale = 2.0;

    initializeVDBMedium(
        "vol", "heterogeneous", transType, vdbType, "const",
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

void Figure_5::gtBounding(pcg32& rng, pbrt::Options options) {
    pbrtInit(options);

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

    initializeIntegratorSeeded(200000000, -1, 100, "volpath", "independent",
                               "independent", rng.nextUInt(), rng.nextUInt(),
                               rng.nextUInt(), false, false);

    initializeSampler("random", 128);
    initializePixelFilter("box");

    initializeFilm("../../results/figure_5/bounding", 800, 600);

    pbrtScale(-1.0, 1.0, 1.0);

    pbrtLookAt(130.0 * cos(0.0), 130.0 * sin(0.0), -1.0, 0.0, 0.0,
               10.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 25);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    initializeSceneMediumBox("ratio", "wdas_cloud_half.vdb",  // "vdb",
                             "trans", "interval_adap_vdb", 1.0);

    pbrtWorldEnd();

    pbrtCleanup();
}

void Figure_5::gtNonBounding(pcg32& rng, pbrt::Options options) {
    pbrtInit(options);

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

    initializeIntegratorSeeded(200000000, -1, 100, "volpath", "independent",
                               "independent", rng.nextUInt(), rng.nextUInt(),
                               rng.nextUInt(), false, false);

    initializeSampler("random", 128);
    initializePixelFilter("box");

    initializeFilm("../../results/figure_5/nonbounding", 800, 600);

    pbrtScale(-1.0, 1.0, 1.0);

    pbrtLookAt(130.0 * cos(0.0), 130.0 * sin(0.0), -1.0, 0.0, 0.0,
               10.0,  // was at .4
               0.0, 0.0, 1.0);

    initializeCamera("perspective", 25);  // was at 30

    pbrtWorldBegin();

    initializeLightSources();

    initializeSceneMediumBox("ratio", "wdas_cloud_half.vdb",  // "vdb",
                             "trans", "interval_adap_vdb_partial", 1.0);

    pbrtWorldEnd();

    pbrtCleanup();
}

void Figure_5::initializeLightSources() {
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
