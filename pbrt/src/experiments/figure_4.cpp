#include "figure_4.h"

#include "trans_samplers/pcg32.h"

// this figure is slightly different than the others. Instead of constructing
// scenes through a programatical interface, we recall pbrt to run using
// specific scene files with slight changes in variables.

// In the paper we include equal cost (extinction call) renders, but here by
// default we just render equal pixel samples for simplicity. This can be
// configured in the individual pbrt scene files.

Figure_4::Figure_4(pbrt::Options options) {
    // create results directory
    system("mkdir ../../results/figure_4/");

    std::string gt_call =
        "./pbrt --experiment figure_4_gt "
        "../scenes/volumetric-caustic/fig_1_gt.pbrt";

    std::string clamped_call =
        "./pbrt --experiment figure_4_clamped "
        "../scenes/volumetric-caustic/fig_1_clamped.pbrt";

    std::string prior_call =
        "./pbrt --experiment figure_4_prior "
        "../scenes/volumetric-caustic/fig_1_prior.pbrt";

    std::string progressive_call =
        "./pbrt --experiment figure_4_progressive "
        "../scenes/volumetric-caustic/fig_1_progressive.pbrt";

    // generate the fully bounded render
    system(clamped_call.c_str());
    // generate the slightly nonbounding render
    system(prior_call.c_str());
    // generate the significantly nonbounding render
    system(progressive_call.c_str());
    // generate the ground truth
    system(gt_call.c_str());
}
