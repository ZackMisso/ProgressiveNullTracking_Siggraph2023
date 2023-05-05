#include <conver/experiment.h>
#include <conver/simple_funcs.h>
#include <conver/grapher.h>
#include <conver/problem.h>
#include <conver/technique.h>
#include <conver/sample_interval.h>
#include <conver/conv_eval.h>
#include <conver/techniques/all_techs.h>

#include <imedit/image.h>

void ExponentialBiasExt::run_experiment(
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
    Grapher::graphFunctionWithData(comp_func, path + "ext_func.txt", path + "ext_func.png");

    TransmittanceMeasureFunction trans_func = TransmittanceMeasureFunction(&comp_func, 0.0, 1.0);
    Grapher::graphFunctionWithData(trans_func, path + "trans_func.txt", path + "trans_func.png");
}

void ExponentialBiasExt::run() const
{
    pcg32 rng = pcg32(0x12312, 0xadcd);
    run_experiment(rng, "test/");
}

void ExponentialBiasExt::run(int test) const
{
    pcg32 rng = pcg32(0x12312, 0xadcd);
    run_experiment(rng, "test/");
}
