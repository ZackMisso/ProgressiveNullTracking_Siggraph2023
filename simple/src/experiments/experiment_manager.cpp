#include <conver/experiment_manager.h>

void ExperimentManager::run(Experiment *experiment)
{
    experiment->preProcess();
    experiment->run();
    delete experiment;
}

void ExperimentManager::run(Experiment *experiment, int test_ind)
{
    experiment->preProcess();
    experiment->run(test_ind);
    delete experiment;
}

void ExperimentManager::runExperiment(int argc, char *argv[])
{
    if (std::string(argv[1]) == "--exp_multi_jans")
    {
        run(new ExponentialMultiJansPlots());
        return;
    }

    if (std::string(argv[1]) == "--exp_jans")
    {
        run(new ExponentialJansPlots());
        return;
    }

    if (std::string(argv[1]) == "--exp_nonbound_jans")
    {
        run(new ExponentialJansNonBoundPlots());
        return;
    }

    if (std::string(argv[1]) == "--exp_jans_ind")
    {
        int test_ind = std::stoi(argv[2]);
        run(new ExponentialJansPlots(), test_ind);
        return;
    }

    if (std::string(argv[1]) == "--exp_nonbound_var")
    {
        int test_ind = std::stoi(argv[2]);
        run(new ExponentialNonBoundVariance());
        return;
    }

    // if (std::string(argv[1]) == "--exp_ratio_nonbound")
    // {
    //     run(new ExponentialRatioNonbound());
    //     return;
    // }

    if (std::string(argv[1]) == "--exp_bias_ext")
    {
        run(new ExponentialBiasExt());
        return;
    }

    if (std::string(argv[1]) == "--figure_8")
    {
        run(new Figure_8());
        return;
    }

    if (std::string(argv[1]) == "--figure_2")
    {
        run(new Figure_2());
        return;
    }

    if (std::string(argv[1]) == "--exp_nonbound_bias_vs_noise")
    {
        int test_ind = std::stoi(argv[2]);
        run(new ExponentialNonBoundBiasVsNoise());
        return;
    }

    std::cout << "unrecognized experiment name: " << std::string(argv[1]) << std::endl;
}
