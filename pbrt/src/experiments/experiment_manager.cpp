#include "experiment_manager.h"

#include <fstream>
#include <sstream>

#include "../trans_samplers/pcg32.h"
#include "figure_1.h"
#include "figure_10.h"
#include "figure_3.h"
#include "figure_4.h"
#include "figure_5.h"
#include "figure_6.h"
#include "figure_9.h"

namespace pbrt {

bool ExperimentManager::run_experiment(pbrt::Options options, int start_index,
                                       int argc, char *argv[]) {
    std::string experiment = std::string(argv[start_index]);
    std::cout << "running experiment: " << experiment << std::endl;

    if (experiment == "figure_1") {
        Globals::WRITE_INCREMENTS = false;
        Globals::ACCEL_SCENE = 1;
        Figure_1 experiment_instance = Figure_1(options);
        return false;
    } else if (experiment == "figure_3") {
        Figure_3 experiment_instance = Figure_3(options);
        return true;
    } else if (experiment == "figure_3_instance") {
        // specify that ratio tracking should take into account
        // non-monochromatic color channels and then render the specified
        // scenefile using pbrt's default behaviour

        Globals::MONO_DENSE_SPEC_ALBEDO = true;
        return false;
    } else if (experiment == "figure_3_instance_gt") {
        // specify that ratio tracking should take into account
        // non-monochromatic color channels and then render the specified
        // scenefile using pbrt's default behaviour

        Globals::MONO_DENSE_SPEC_ALBEDO = true;

        // for convenience, we can also output intermediate images for every
        // 2^n pixel samples by setting WRITE_INCREMENTS to true

        Globals::WRITE_INCREMENTS = true;

        return false;
    } else if (experiment == "figure_4") {
        Figure_4 experiment_instance = Figure_4(options);
        return true;
    } else if (experiment == "figure_4_gt") {
        // no additional setup necessary
        return false;
    } else if (experiment == "figure_4_clamped") {
        Globals::NO_CLAMP = false;
        return false;
    } else if (experiment == "figure_4_prior") {
        // no additional setup necessary
        return false;
    } else if (experiment == "figure_4_progressive") {
        Globals::NO_CLAMP = false;
        return false;
    } else if (experiment == "figure_5") {
        Globals::NO_CLAMP = false;

        // for convenience, we can also output intermediate images for every
        // 2^n pixel samples by setting WRITE_INCREMENTS to true

        Globals::WRITE_INCREMENTS = true;

        Figure_5 experiment_instance = Figure_5(options);
        return true;
    } else if (experiment == "figure_6") {
        Globals::NO_CLAMP = false;

        // for convenience, we can also output intermediate images for every
        // 2^n pixel samples by setting WRITE_INCREMENTS to true

        Globals::WRITE_INCREMENTS = false;
        // Globals::WRITE_INCREMENTS = true;

        Figure_6 experiment_instance = Figure_6(options);
        return true;
    } else if (experiment == "figure_9") {
        Figure_9 experiment_instance = Figure_9(options);
        return true;
    } else if (experiment == "figure_10") {
        Globals::NO_CLAMP = false;
        Globals::WRITE_INCREMENTS = true;
        Figure_10 experiment_instance = Figure_10(options);
        return true;
    } else {
        std::cout << "Error :: experiment not recognized " << std::endl;
        return false;
    }
}

}  // namespace pbrt