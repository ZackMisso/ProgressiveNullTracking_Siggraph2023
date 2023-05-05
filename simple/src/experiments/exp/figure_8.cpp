#include <conver/experiment.h>
#include <conver/simple_funcs.h>
#include <conver/grapher.h>
#include <conver/problem.h>
#include <conver/technique.h>
#include <conver/sample_interval.h>
#include <conver/conv_eval.h>
#include <conver/techniques/all_techs.h>

#include <imedit/image.h>

void Figure_8::run_experiment_for_extinction(
    pcg32 &rng,
    std::string prefix,
    Function *function) const
{
    Float maj = function->compute_max() * 1.5;

    ExpTrans exp_prob = ExpTrans(function);

    PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(8192, 17);

    std::vector<Technique<Float> *> techniques = std::vector<Technique<Float> *>();
    techniques.push_back(new AdaptiveRatioExp<Float>(maj));
    techniques.push_back(new RatioExp<Float>(maj));

    ConvergenceEvaluator<Float> evaluator = ConvergenceEvaluator<Float>(&exp_prob, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
    evaluator.evaluate();

    std::vector<ConvergenceRecord> data_records = std::vector<ConvergenceRecord>();

    for (int j = 0; j < evaluator.getData()->data_records.size(); ++j)
        data_records.push_back(evaluator.getData()->data_records[j]);

    std::string path = getDataPath() + prefix;

    std::ofstream file;
    file.open(path + "_res.txt");

    for (int i = 0; i < data_records[0].data_points.size(); ++i)
    {
        file << data_records[0].data_points[i].samples << std::endl;
        file << data_records[0].data_points[i].work_norm_var << std::endl;
        file << data_records[1].data_points[i].work_norm_var << std::endl;
    }

    file.close();
}

void Figure_8::run_experiment(
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

    CompositeSineFunction comp_func = CompositeSineFunction(rng, 10, 0.0, 1.0);
    comp_func.normalize();
    Grapher::graphFunctionWithData(comp_func, path + "comp_sine.txt", path + "comp_sine.png");

    ConstantFunction const_func = ConstantFunction(1.0, 0.0, 1.0);
    Grapher::graphFunctionWithData(const_func, path + "const.txt", path + "const.png");

    GaussianFunction gauss_func = GaussianFunction(1.0, 0.2, 0.63, 0.0, 1.0);
    gauss_func.solve_for_mean(1.0);
    Grapher::graphFunctionWithData(gauss_func, path + "gauss.txt", path + "gauss.png");

    LinIncFunction lin_func = LinIncFunction(1.0, 0.0, 1.0);
    Grapher::graphFunctionWithData(lin_func, path + "lin.txt", path + "lin.png");

    std::cout << "running experiment for const function" << std::endl;
    run_experiment_for_extinction(rng, prefix + "const", &const_func);

    std::cout << "running experiment for comp_sine function" << std::endl;
    run_experiment_for_extinction(rng, prefix + "comp_sine", &comp_func);

    std::cout << "running experiment for gauss function" << std::endl;
    run_experiment_for_extinction(rng, prefix + "gauss", &gauss_func);

    std::cout << "running experiment for lin function" << std::endl;
    run_experiment_for_extinction(rng, prefix + "lin", &lin_func);

    std::cout << "calling python script to generate results" << std::endl;
    system("python3 ../prog_scripts/figure_9.py");
}

void Figure_8::run() const
{
    pcg32 rng = pcg32(0x12312, 0xadcd);
    run_experiment(rng, "data/");
}

void Figure_8::run(int test) const
{
    pcg32 rng = pcg32(0x12312, 0xadcd);
    run_experiment(rng, "data/");
}
