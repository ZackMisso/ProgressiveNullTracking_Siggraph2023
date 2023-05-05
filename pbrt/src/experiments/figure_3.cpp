#include "figure_3.h"

// this figure is slightly different than the others. Instead of constructing
// scenes through a programatical interface, we recall pbrt to run using
// specific scene files with slight changes in variables

Figure_3::Figure_3(pbrt::Options options) {
    // initialize random number generator
    pcg32 rng = pcg32(0xacdc, 0x3137);

    // create results directory
    system("mkdir ../../results/figure_3/");

    std::string gt_call = "./pbrt --experiment figure_3_instance_gt " +
                          Globals::TEST_SCENES_PATH +
                          "dragon_final/dragon_gt.pbrt";

    std::string bound_call = "./pbrt --experiment figure_3_instance " +
                             Globals::TEST_SCENES_PATH +
                             "dragon_final/dragon_bound.pbrt";

    std::string slight_call = "./pbrt --experiment figure_3_instance " +
                              Globals::TEST_SCENES_PATH +
                              "dragon_final/dragon_slightly.pbrt";

    std::string signif_call = "./pbrt --experiment figure_3_instance " +
                              Globals::TEST_SCENES_PATH +
                              "dragon_final/dragon_signif.pbrt";

    // generate the fully bounded render
    system(bound_call.c_str());
    // generate the slightly nonbounding render
    system(slight_call.c_str());
    // generate the significantly nonbounding render
    system(signif_call.c_str());
    // generate the graphs
    system("python3 ../../ConvergenceRater/prog_scripts/figure_3.py");
    // generate the ground truth
    // system(gt_call.c_str());
    // TODO: call script to generate inset images
}
