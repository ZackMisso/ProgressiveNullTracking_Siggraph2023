#include "figure_6.h"

Figure_6::Figure_6(pbrt::Options options) {
    system("mkdir ../../results/figure_6/");
    system("mkdir ../../results/figure_6/iterations/");

    // The maximum density for the smoke in this figure is a little more than
    // 16, and we choose to increment the majorant by 0.5 every iteration. The
    // first 32 images will be biased, while the last 96 will be unbiased.
    // However, the vast majority of the medium's density is fairly low (<1.0).
    // So while the first 32 images will be technically biased, only for the
    // first few iterations will it be visibly noticable for most of the image.

    // We cut corners a bit by choosing to stop incrementing the majorant once
    // it becomes bounding (at 20). This is to keep the render times from
    // becoming astronomical, otherwise the results for this figure would take
    // too long to generate. They already take about 16 days on my laptop alone
    // for the full number of iterations. This has zero impact on what we convey
    // however, since for each iteration the expected value is the same (for
    // i>=20).

    Float maj = 0.1;
    int n = 128;
    int samples_per_iteration = 128;
    int ground_truth_samples = 128;  // 8192

    bool render_iters = true;
    bool render_gt = false;

    n = 20;
    int start_iter = 0;

    // Float maj = 0.1;
    // int n = 128;
    // int samples_per_iteration = 1;
    // int ground_truth_samples = 128; // 8192

    // initialize random number generator
    pcg32 rng = pcg32(0xaccdc, 0xba53ba11);

    if (render_iters) {
        for (int i = 0; i < start_iter; ++i) {
            // this is used for naively splitting jobs across my different
            // computers
            rng.nextUInt();
            rng.nextUInt();
            rng.nextUInt();
            maj += 0.5;
            if (maj > 20.0) maj = 20.0;
        }
        for (int i = 0; i < n; ++i) {
            std::string path = "../../results/figure_6/iterations/" +
                               std::to_string(i) + "_iter";

            runEqualSampleRenders(options, rng, path, maj,
                                  samples_per_iteration);

            maj += 0.5;
            if (maj > 20.0) maj = 20.0;
        }

        // print running average of renders
        imedit::Image base =
            imedit::Image("../../results/figure_6/iterations/0_iter.exr");

        std::string base_path = "../../results/figure_6/";
        base.write(base_path + "n_1.exr");
        base.write(base_path + "n_1.png");

        for (int i = 1; i < n; ++i) {
            std::string path = "../../results/figure_6/iterations/" +
                               std::to_string(i) + "_iter.exr";
            imedit::Image image = imedit::Image(path);

            base = base + (image - base) * 1.0 / (double(i + 1));

            // i am currently lazy, otherwise this could be formatted better
            if (i == 1) {
                base.write(base_path + "n_2.exr");
                base.write(base_path + "n_2.png");
            }
            if (i == 3) {
                base.write(base_path + "n_4.exr");
                base.write(base_path + "n_4.png");
            }
            if (i == 7) {
                base.write(base_path + "n_8.exr");
                base.write(base_path + "n_8.png");
            }
            if (i == 15) {
                base.write(base_path + "n_16.exr");
                base.write(base_path + "n_16.png");
            }
            if (i == 31) {
                base.write(base_path + "n_32.exr");
                base.write(base_path + "n_32.png");
            }
            if (i == 63) {
                base.write(base_path + "n_64.exr");
                base.write(base_path + "n_64.png");
            }
            if (i == 127) {
                base.write(base_path + "n_128.exr");
                base.write(base_path + "n_128.png");
            }
        }
    }

    if (render_gt) {
        maj = 20.0;

        // render the ground truth
        std::string path = "../../results/figure_6/gt";

        runEqualSampleRenders(options, rng, path, maj, ground_truth_samples);
    }
}

void Figure_6::initializeSceneMediumBox(Float majorant) {
    pbrtAttributeBegin();

    Float squeeze = 0.75;
    Float stretch = 1.2;

    pbrtScale(46 * 60.0 / 57.2 * squeeze, 40.8 * 60.0 / 57.2 * squeeze,
              60.0 * stretch);
    pbrtTranslate(0.0, 0.0, 0.46);
    pbrtRotate(90.0, -1.0, 0.0, 0.0);

    Float min = -0.5;
    Float max = 0.5;

    Float scale = 1.0;

    initializeVDBMedium(
        "vol", "heterogeneous", "ratio", "interval_adap_vdb_constant", "const",
        "trans",  // delta tracking
        std::vector<Float>{0.8 * scale, 0.8 * scale, 0.8 * scale},
        std::vector<Float>{0.7 * scale, 0.7 * scale,
                           0.7 * scale},  // delta / raymarching
        std::vector<Float>{min, min, min}, std::vector<Float>{max, max, max},
        false, false, false, false, majorant, "simple_smoke_66.vdb");

    pbrtMaterial("", ParamSet());
    pbrtMediumInterface("vol", "");

    // NOTE: These have to be the same as the min bounds
    initializeBoxShape(min, max, min, max, min, max);

    pbrtAttributeEnd();
}

void Figure_6::runEqualSampleRenders(pbrt::Options options, pcg32& rng,
                                     std::string path, Float majorant,
                                     int samples) {
    pbrtInit(options);

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

    // initializeIntegrator(200000000, -1, 5, "volpath", "independent",
    //                      "independent", false, false);

    initializeIntegratorSeeded(200000000, -1, 5, "volpath", "independent",
                               "independent", rng.nextUInt(), rng.nextUInt(),
                               rng.nextUInt(), false, false);

    initializeSampler("random", samples);

    initializePixelFilter("box");

    initializeFilm(path, 540, 1080);

    pbrtScale(-1.0, 1.0, 1.0);

    pbrtLookAt(50.0, -60.0, 40.5, 0.0, 0.0, 33.0,  // was at .4
               0.0, 0.0, -1.0);

    initializeCamera("perspective", 35);  // was at 35

    pbrtWorldBegin();

    initializeLightSources();

    pbrtAttributeBegin();
    initializeNamedMaterial_Glass("glass", 1.5, 0.5, 0.6, false);
    pbrtRotate(135.0, 0.0, 0.0, 1.0);
    pbrtRotate(180.0, 1.0, 0.0, 0.0);
    pbrtTranslate(0.0, 0.0, -70.0);
    pbrtScale(100.0, 80.0, 300.0);
    initializePlyShape("geometry/photo_backdrop_smooth_2.ply");
    pbrtAttributeEnd();

    initializeSceneMediumBox(majorant);

    pbrtWorldEnd();

    pbrtCleanup();
}

void Figure_6::initializeLightSources() {
    pbrtAttributeBegin();

    initializeDistantLightSource(std::vector<Float>{2.0, 2.0, 2.0},
                                 Point3f(0.0, 0.0, -1.0),
                                 Point3f(0.0, 0.0, 0.0));

    pbrtAttributeEnd();

    pbrtAttributeBegin();

    Float env_scale = 2.0;
    initializeInfiniteLightSource(
        "textures/white.png",
        std::vector<Float>{0.1 * env_scale, 0.3 * env_scale,
                           0.6 * env_scale});  // 1.46
    pbrtAttributeEnd();
}
