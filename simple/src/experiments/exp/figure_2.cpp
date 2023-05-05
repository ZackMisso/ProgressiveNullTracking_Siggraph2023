#include <conver/experiment.h>
#include <conver/simple_funcs.h>
#include <conver/grapher.h>
#include <conver/problem.h>
#include <conver/technique.h>
#include <conver/sample_interval.h>
#include <conver/conv_eval.h>
#include <conver/techniques/all_techs.h>

#include <imedit/image.h>

void Figure_2::run_experiment(
    pcg32 &rng,
    std::string prefix) const
{
    // create the necessary storage folder
    std::string path = getDataPath() + prefix;
    std::string cmd = "mkdir " + path;
    system(cmd.c_str());

    // graph the results
    GraphSettings loglog = GraphSettings();
    loglog.x_log = false;
    loglog.y_log = false;

    std::vector<ConvergenceData> conv_data = std::vector<ConvergenceData>();
    std::vector<double> vals = std::vector<double>();

    CompositeSineStepWeightedFunction comp_func = CompositeSineStepWeightedFunction(rng, 5, 0.1, 3.0, 0.0, 5.0);
    comp_func.normalize();
    Grapher::graphFunctionWithData(comp_func, path + "ext_func.txt", path + "ext_func.png");

    TransmittanceMeasureFunction trans_func = TransmittanceMeasureFunction(&comp_func, 0.0, 5.0);
    Grapher::graphFunctionWithData(trans_func, path + "trans_func.txt", path + "trans_func.png");

    int trials = 1000;

    AverageRatioTrackFunction avg_rat = AverageRatioTrackFunction(rng, &comp_func, 3.5, 0.0, 5.0, trials);
    Grapher::graphFunctionWithData(avg_rat, path + "avg_rat.txt", path + "avg_rat.png");

    AverageRatioTrackFunction avg_non_rat = AverageRatioTrackFunction(rng, &comp_func, 0.4, 0.0, 5.0, trials);
    Grapher::graphFunctionWithData(avg_non_rat, path + "avg_non_rat.txt", path + "avg_non_rat.png");

    for (int i = 0; i < trials; ++i)
    {
        std::cout << "Running trial: " << (i + 1) << " out of " << trials << std::endl;
        std::string iter_path = getDataPath() + prefix + std::to_string(i) + "/";
        std::string iter_rm = "rm -rf " + iter_path;
        system(iter_rm.c_str());
        std::string iter_cmd = "mkdir " + iter_path;
        system(iter_cmd.c_str());

        Grapher::graphFunctionWithData(avg_rat.ratios[i], iter_path + "rat.txt", iter_path + "rat.png");
        Grapher::graphFunctionWithData(avg_non_rat.ratios[i], iter_path + "non_rat.txt", iter_path + "non_rat.png");
    }

    std::cout << "calling python script to generate results" << std::endl;
    system("python3 ../prog_scripts/figure_2.py");
}

void Figure_2::run() const
{
    pcg32 rng = pcg32(0x12312, 0xadcd);
    run_experiment(rng, "data/");
}

void Figure_2::run(int test) const
{
    pcg32 rng = pcg32(0x12312, 0xadcd);
    run_experiment(rng, "data/");
}
