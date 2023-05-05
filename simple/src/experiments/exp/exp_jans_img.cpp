#include <conver/experiment.h>
#include <conver/simple_funcs.h>
#include <conver/grapher.h>
#include <conver/problem.h>
#include <conver/technique.h>
#include <conver/sample_interval.h>
#include <conver/conv_eval.h>
#include <conver/techniques/all_techs.h>

#include <imedit/image.h>

void ExponentialJansPlots::run_experiment_noise(
    pcg32 &rng,
    std::vector<Technique<Float> *> techniques,
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

    int trials = JansTrials;
    int res = JansRes;

    double min_p = 0.01;
    double max_p = 0.99;
    double min_yx = 1.0;
    double max_yx = 2.0;

    std::vector<imedit::Image *> images = std::vector<imedit::Image *>();
    for (int i = 0; i < techniques.size(); ++i)
    {
        images.push_back(new imedit::Image(res, res)); // mean
        images.push_back(new imedit::Image(res, res)); // err
        images.push_back(new imedit::Image(res, res)); // mse
        images.push_back(new imedit::Image(res, res)); // work
        images.push_back(new imedit::Image(res, res)); // work_norm_var
    }

    imedit::Image mean_image = imedit::Image(res, res);

    // TODO: create a randomized sine function
    CompositeSineFunction comp_func = CompositeSineFunction(rng, 10, 0.0, 1.0);
    comp_func.normalize();

    Grapher::graphFunction(comp_func, path + "comp_func.png");
    std::cout << "comp func integral: " << comp_func.integrate() << std::endl;

    int i = 0;
    int j = 0;

    for (double p = min_p; p < max_p; p += (max_p - min_p) / Float(res))
    {
        // this will only work for sin_functions
        i = 0;
        std::cout << "Evaluating col: " << j << std::endl;

        // initialize sine function
        CompositeSineFunction comp_iter_func = comp_func;

        Float mean = -std::log(p);

        comp_iter_func.solve_for_mean(mean);
        Float max = comp_iter_func.compute_max(); // compute max was broken, ugh

        // transmittance should be x
        for (double yx = min_yx; yx < max_yx; yx += (max_yx - min_yx) / Float(res))
        {
            Float maj = max * yx;

            for (int k = 0; k < techniques.size(); ++k)
                techniques[k]->setMaj(maj);

            ExpTrans recip = ExpTrans(&comp_iter_func);

            // initialize a sample interval to evaluate the convergence of
            PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(trials, JansSamples);

            ConvergenceEvaluatorJans<Float> evaluator = ConvergenceEvaluatorJans<Float>(&recip, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
            evaluator.evaluate();

            // create convergence data
            ConvergenceData avg_data = ConvergenceData(evaluator.getData()->data_records);

            for (int k = 0; k < techniques.size(); ++k)
            {
                int data_pts = avg_data.getRecordByIndex(k).data_points.size();

                images[5 * k + 0]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].mean));
                images[5 * k + 1]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].bias));
                images[5 * k + 2]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].true_mse));
                images[5 * k + 3]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work));
                images[5 * k + 4]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work_norm_var));
            }

            mean_image.setPixel(j, i, imedit::Pixel(exp(-comp_iter_func.integrate())));

            i++;
        }
        j++;
    }

    for (int k = 0; k < techniques.size(); ++k)
    {
        std::cout << "path: " << getDataPath() + prefix + techniques[k]->getName() + "_mean.exr" << std::endl;
        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.exr");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.exr");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.exr");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.exr");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.exr");

        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.png");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.png");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.png");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.png");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.png");
    }

    mean_image.write(getDataPath() + prefix + "gt_mean.exr");
    mean_image.write(getDataPath() + prefix + "gt_mean.png");
}

void ExponentialJansPlots::run_experiment_const(
    pcg32 &rng,
    std::vector<Technique<Float> *> techniques,
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

    int trials = JansTrials;
    int res = JansRes;

    double min_p = 0.01;
    double max_p = 0.99;
    double min_yx = 1.0;
    double max_yx = 5.0;

    std::vector<imedit::Image *> images = std::vector<imedit::Image *>();
    for (int i = 0; i < techniques.size(); ++i)
    {
        images.push_back(new imedit::Image(res, res)); // mean
        images.push_back(new imedit::Image(res, res)); // err
        images.push_back(new imedit::Image(res, res)); // mse
        images.push_back(new imedit::Image(res, res)); // work
        images.push_back(new imedit::Image(res, res)); // work_norm_var
    }

    imedit::Image mean_image = imedit::Image(res, res);

    // TODO: create a randomized sine function
    ConstantFunction comp_func = ConstantFunction(1.0, 0.0, 1.0);

    Grapher::graphFunction(comp_func, path + "comp_func.png");
    std::cout << "comp func integral: " << comp_func.integrate() << std::endl;

    int i = 0;
    int j = 0;

    for (double p = min_p; p < max_p; p += (max_p - min_p) / Float(res))
    {
        // this will only work for sin_functions
        i = 0;

        std::cout << "Evaluating col: " << j << std::endl;

        ConstantFunction comp_iter_func = comp_func;

        Float mean = -std::log(p);

        comp_iter_func.solve_for_mean(mean);
        Float max = comp_iter_func.compute_max(); // compute max was broken, ugh

        for (double yx = min_yx; yx < max_yx; yx += (max_yx - min_yx) / Float(res))
        {
            Float maj = max * yx;

            for (int k = 0; k < techniques.size(); ++k)
                techniques[k]->setMaj(maj);

            ExpTrans recip = ExpTrans(&comp_iter_func);

            // initialize a sample interval to evaluate the convergence of
            PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(trials, JansSamples);

            ConvergenceEvaluatorJans<Float> evaluator = ConvergenceEvaluatorJans<Float>(&recip, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
            evaluator.evaluate();

            // create convergence data
            ConvergenceData avg_data = ConvergenceData(evaluator.getData()->data_records);

            for (int k = 0; k < techniques.size(); ++k)
            {
                int data_pts = avg_data.getRecordByIndex(k).data_points.size();

                images[5 * k + 0]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].mean));
                // images[5 * k + 0]->setPixel(j, i, yx);
                images[5 * k + 1]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].bias));
                images[5 * k + 2]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].true_mse));
                images[5 * k + 3]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work));
                images[5 * k + 4]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work_norm_var));
            }

            mean_image.setPixel(j, i, imedit::Pixel(exp(-comp_iter_func.integrate())));

            i++;
        }
        j++;
    }

    for (int k = 0; k < techniques.size(); ++k)
    {
        std::cout << "path: " << getDataPath() + prefix + techniques[k]->getName() + "_mean.exr" << std::endl;
        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.exr");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.exr");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.exr");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.exr");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.exr");

        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.png");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.png");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.png");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.png");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.png");
    }

    mean_image.write(getDataPath() + prefix + "gt_mean.exr");
    mean_image.write(getDataPath() + prefix + "gt_mean.png");
}

void ExponentialJansPlots::run_experiment_scos(
    pcg32 &rng,
    std::vector<Technique<Float> *> techniques,
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

    int trials = JansTrials;
    int res = JansRes;

    double min_p = 0.01;
    double max_p = 0.99;
    double min_yx = 1.0;
    double max_yx = 2.0;

    std::vector<imedit::Image *> images = std::vector<imedit::Image *>();
    for (int i = 0; i < techniques.size(); ++i)
    {
        images.push_back(new imedit::Image(res, res)); // mean
        images.push_back(new imedit::Image(res, res)); // err
        images.push_back(new imedit::Image(res, res)); // mse
        images.push_back(new imedit::Image(res, res)); // work
        images.push_back(new imedit::Image(res, res)); // work_norm_var
    }

    imedit::Image mean_image = imedit::Image(res, res);

    // TODO: create a randomized sine function
    SineWaveFunction comp_func = SineWaveFunction(1.0, 1.0, 0.0, 0.0, 1.0);

    Grapher::graphFunction(comp_func, path + "comp_func.png");
    std::cout << "comp func integral: " << comp_func.integrate() << std::endl;

    int i = 0;
    int j = 0;

    // transmittance should be x
    for (double p = min_p; p < max_p; p += (max_p - min_p) / Float(res))
    {
        i = 0;
        std::cout << "Evaluating col: " << j << std::endl;
        SineWaveFunction comp_iter_func = comp_func;

        Float mean = -std::log(p);

        comp_iter_func.solve_for_mean(mean);
        Float max = comp_iter_func.compute_max();

        for (double yx = min_yx; yx < max_yx; yx += (max_yx - min_yx) / Float(res))
        {
            // this will only work for sin_functions
            Float maj = max * yx;

            for (int k = 0; k < techniques.size(); ++k)
                techniques[k]->setMaj(maj);

            ExpTrans recip = ExpTrans(&comp_iter_func);

            // initialize a sample interval to evaluate the convergence of
            PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(trials, JansSamples);

            ConvergenceEvaluatorJans<Float> evaluator = ConvergenceEvaluatorJans<Float>(&recip, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
            evaluator.evaluate();

            // create convergence data
            ConvergenceData avg_data = ConvergenceData(evaluator.getData()->data_records);

            for (int k = 0; k < techniques.size(); ++k)
            {
                int data_pts = avg_data.getRecordByIndex(k).data_points.size();

                images[5 * k + 0]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].mean));
                images[5 * k + 1]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].bias));
                images[5 * k + 2]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].true_mse));
                images[5 * k + 3]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work));
                images[5 * k + 4]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work_norm_var));
            }

            mean_image.setPixel(j, i, imedit::Pixel(exp(-comp_iter_func.integrate())));

            i++;
        }
        j++;
    }

    for (int k = 0; k < techniques.size(); ++k)
    {
        std::cout << "path: " << getDataPath() + prefix + techniques[k]->getName() + "_mean.exr" << std::endl;
        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.exr");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.exr");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.exr");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.exr");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.exr");

        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.png");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.png");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.png");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.png");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.png");
    }

    mean_image.write(getDataPath() + prefix + "gt_mean.exr");
    mean_image.write(getDataPath() + prefix + "gt_mean.png");
}

void ExponentialJansPlots::run_experiment_gauss(
    pcg32 &rng,
    std::vector<Technique<Float> *> techniques,
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

    int trials = JansTrials;
    int res = JansRes;

    double min_p = 0.01;
    double max_p = 0.99;
    double min_yx = 1.0;
    double max_yx = 2.0;

    std::vector<imedit::Image *> images = std::vector<imedit::Image *>();
    for (int i = 0; i < techniques.size(); ++i)
    {
        images.push_back(new imedit::Image(res, res)); // mean
        images.push_back(new imedit::Image(res, res)); // err
        images.push_back(new imedit::Image(res, res)); // mse
        images.push_back(new imedit::Image(res, res)); // work
        images.push_back(new imedit::Image(res, res)); // work_norm_var
    }

    imedit::Image mean_image = imedit::Image(res, res);

    // TODO: create a randomized sine function
    GaussianFunction comp_func = GaussianFunction(1.0, 0.1, 0.4, 0.0, 1.0);

    Grapher::graphFunction(comp_func, path + "comp_func.png");
    std::cout << "comp func integral: " << comp_func.integrate() << std::endl;

    int i = 0;
    int j = 0;

    for (double p = min_p; p < max_p; p += (max_p - min_p) / Float(res))
    {
        i = 0;

        std::cout << "Evaluating col: " << j << std::endl;

        // initialize sine function
        GaussianFunction comp_iter_func = comp_func;

        Float mean = -std::log(p);

        comp_iter_func.solve_for_mean(mean);
        Float max = comp_iter_func.compute_max(); // compute max was broken, ugh

        for (double yx = min_yx; yx < max_yx; yx += (max_yx - min_yx) / Float(res))
        {
            Float maj = max * yx;

            for (int k = 0; k < techniques.size(); ++k)
                techniques[k]->setMaj(maj);

            ExpTrans recip = ExpTrans(&comp_iter_func);

            // initialize a sample interval to evaluate the convergence of
            PowerTwoSampleInterval sample_interval_short = PowerTwoSampleInterval(trials, JansSamples);

            ConvergenceEvaluatorJans<Float> evaluator = ConvergenceEvaluatorJans<Float>(&recip, techniques, &sample_interval_short, rng.nextUInt(), rng.nextUInt());
            evaluator.evaluate();

            // create convergence data
            ConvergenceData avg_data = ConvergenceData(evaluator.getData()->data_records);

            for (int k = 0; k < techniques.size(); ++k)
            {
                int data_pts = avg_data.getRecordByIndex(k).data_points.size();

                images[5 * k + 0]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].mean));
                images[5 * k + 1]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].bias));
                images[5 * k + 2]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].true_mse));
                images[5 * k + 3]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work));
                images[5 * k + 4]->setPixel(j, i, imedit::Pixel(avg_data.getRecordByIndex(k).data_points[data_pts - 1].work_norm_var));
            }

            mean_image.setPixel(j, i, imedit::Pixel(exp(-comp_iter_func.integrate())));

            i++;
        }
        j++;
    }

    for (int k = 0; k < techniques.size(); ++k)
    {
        std::cout << "path: " << getDataPath() + prefix + techniques[k]->getName() + "_mean.exr" << std::endl;
        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.exr");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.exr");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.exr");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.exr");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.exr");

        images[5 * k + 0]->write(getDataPath() + prefix + techniques[k]->getName() + "_mean.png");
        images[5 * k + 1]->write(getDataPath() + prefix + techniques[k]->getName() + "_err.png");
        images[5 * k + 2]->write(getDataPath() + prefix + techniques[k]->getName() + "_mse.png");
        images[5 * k + 3]->write(getDataPath() + prefix + techniques[k]->getName() + "_work.png");
        images[5 * k + 4]->write(getDataPath() + prefix + techniques[k]->getName() + "_work_norm_var.png");
    }

    mean_image.write(getDataPath() + prefix + "gt_mean.exr");
    mean_image.write(getDataPath() + prefix + "gt_mean.png");
}

void ExponentialJansPlots::run() const
{
    // BlanchetBinFast<Float> *blanchet_bin_fast = new BlanchetBinFast<Float>(0.65);
    BlanchetBinFinal<Float> *blanchet_zero = new BlanchetBinFinal<Float>(0.65, 0);
    BlanchetBinFinal<Float> *blanchet_one = new BlanchetBinFinal<Float>(0.65, 1);
    BlanchetBinFinal<Float> *blanchet_two = new BlanchetBinFinal<Float>(0.65, 2);
    BlanchetBinFinal<Float> *blanchet_three = new BlanchetBinFinal<Float>(0.65, 3);
    // BlanchetBinFinal<Float> *blanchet_four = new BlanchetBinFinal<Float>(0.65, 4);
    PCumeExp<Float> *pcume = new PCumeExp<Float>(1.0);
    PCmfExp<Float> *pcmf = new PCmfExp<Float>(1.0);
    PCmfModExp<Float> *pcmf_mod = new PCmfModExp<Float>(1.0);
    PRatioExp<Float> *prat = new PRatioExp<Float>(1.0);
    RatioExp<Float> *rat = new RatioExp<Float>(1.0);
    IterRatioExp<Float> *rat_iter_2 = new IterRatioExp<Float>(1.0, 0.5);
    IterRatioExp<Float> *rat_iter_5 = new IterRatioExp<Float>(1.0, 0.5);
    IterRatioExp<Float> *rat_iter_8 = new IterRatioExp<Float>(1.0, 0.8);
    DeltaExp<Float> *delt = new DeltaExp<Float>(1.0);
    WeightedDeltaExp<Float> *weighted_delt = new WeightedDeltaExp<Float>(1.0);
    AdaptiveDeltaExp<Float> *adap_delt = new AdaptiveDeltaExp<Float>(1.0);
    AdaptiveRatioExp<Float> *adap_rat = new AdaptiveRatioExp<Float>(1.0);
    AdaptiveRatioFullExp<Float> *adap_rat_full = new AdaptiveRatioFullExp<Float>(1.0);
    AdaptiveDeltaFullExp<Float> *adap_delt_full = new AdaptiveDeltaFullExp<Float>(1.0);
    AdaptiveRatioBothExp<Float> *adap_rat_both = new AdaptiveRatioBothExp<Float>(1.0);
    AdaptiveAllDumbExp<Float> *adap_dumb = new AdaptiveAllDumbExp<Float>(1.0);
    AdaptiveDoubleExp<Float> *adap_double = new AdaptiveDoubleExp<Float>(1.0);
    AdaptiveMaxExp<Float> *adap_max = new AdaptiveMaxExp<Float>(1.0);
    AdaptiveMaxNFExp<Float> *adap_max_nf = new AdaptiveMaxNFExp<Float>(1.0);
    NvidiaExp<Float> *nvidia = new NvidiaExp<Float>(1.0);
    TeleCor<Float> *tele_cor = new TeleCor<Float>();
    TeleNaive<Float> *tele_naive = new TeleNaive<Float>(1.0);
    TeleNaiveBounds<Float> *tele_naive_bounds = new TeleNaiveBounds<Float>(1.0);

    RayMarchExp<Float> *ray = new RayMarchExp<Float>(1.0);
    RayMarchRatStepExp<Float> *ray_ratstep = new RayMarchRatStepExp<Float>(1.0);
    RayMarchNaiveVarExp<Float> *ray_naivevar = new RayMarchNaiveVarExp<Float>(1.0);
    RayMarchProgVarExp<Float> *ray_adapvar = new RayMarchProgVarExp<Float>(1.0);
    RayMarchAdaptiveRatStepExp<Float> *ray_adaprat = new RayMarchAdaptiveRatStepExp<Float>(1.0);

    RayMarchNonCorExp<Float> *noncor_ray = new RayMarchNonCorExp<Float>(1.0);
    RayMarchRatStepNonCorExp<Float> *noncor_ray_ratstep = new RayMarchRatStepNonCorExp<Float>(1.0);
    RayMarchNaiveVarNonCorExp<Float> *noncor_ray_naivevar = new RayMarchNaiveVarNonCorExp<Float>(1.0);
    RayMarchProgVarNonCorExp<Float> *noncor_ray_adapvar = new RayMarchProgVarNonCorExp<Float>(1.0);
    RayMarchAdaptiveRatStepNonCorExp<Float> *noncor_ray_adaprat = new RayMarchAdaptiveRatStepNonCorExp<Float>(1.0);

    std::vector<Technique<Float> *> techniques = std::vector<Technique<Float> *>();

    pcg32 rng = pcg32(0x12312, 0xadcd);

    // std::string prefix = "noise_all/";

    // techniques.push_back(blanchet_zero);
    // techniques.push_back(blanchet_one);
    // techniques.push_back(blanchet_two);
    // techniques.push_back(blanchet_three);
    // techniques.push_back(pcume);
    // techniques.push_back(pcmf);
    // techniques.push_back(pcmf_mod);
    // techniques.push_back(prat);
    // techniques.push_back(nvidia);

    // techniques.push_back(delt);
    // techniques.push_back(weighted_delt);
    techniques.push_back(rat_iter_2);
    // techniques.push_back(rat_iter_5);
    // techniques.push_back(rat_iter_8);

    // techniques.push_back(delt);
    // techniques.push_back(adap_delt);
    // techniques.push_back(adap_dumb);
    // techniques.push_back(adap_double);
    // techniques.push_back(adap_max);
    // techniques.push_back(adap_max_nf);

    // techniques.push_back(prat);
    // techniques.push_back(adap_rat_full);
    // techniques.push_back(rat);
    // techniques.push_back(adap_rat);
    // techniques.push_back(tele_cor);
    // techniques.push_back(tele_naive);
    // techniques.push_back(tele_naive_bounds);
    // techniques.push_back(adap_delt_full);

    // techniques.push_back(delt);
    // techniques.push_back(adap_delt);
    // techniques.push_back(rat);
    // techniques.push_back(adap_rat);

    // techniques.push_back(ray);
    // techniques.push_back(ray_ratstep);
    // techniques.push_back(ray_naivevar);
    // techniques.push_back(ray_adapvar);
    // techniques.push_back(noncor_ray);
    // techniques.push_back(noncor_ray_ratstep);
    // techniques.push_back(noncor_ray_naivevar);
    // techniques.push_back(noncor_ray_adapvar);
    // techniques.push_back(ray_adaprat);
    // techniques.push_back(noncor_ray_adaprat);

    // techniques.push_back(adap_rat_both);
    // for (int i = 1; i < 20; ++i)
    // {
    //     techniques.push_back(new AdaptiveRatioMultExp<Float>(1.0, i));
    // }

    run_experiment_const(rng, techniques, "const_all/");
    // run_experiment_scos(rng, techniques, "scos_all/");
    // run_experiment_gauss(rng, techniques, "gauss_all/");
    // run_experiment_noise(rng, techniques, "noise_all/");

    // finish by cleaning up the datastructures
    // delete blanchet_bin_fast;
    delete blanchet_one;
    delete blanchet_two;
    delete blanchet_three;
    // delete blanchet_four;
    delete pcume;
    delete pcmf;
    delete pcmf_mod;
    delete prat;
    delete rat;
    delete rat_iter_2;
    delete rat_iter_5;
    delete rat_iter_8;
    delete delt;
    delete weighted_delt;
    delete adap_rat;
    delete adap_rat_both;
    delete adap_delt;
    delete adap_delt_full;
    delete adap_rat_full;
    delete nvidia;
    delete tele_cor;
    delete tele_naive;
    delete tele_naive_bounds;

    delete ray;
    delete ray_ratstep;
    delete ray_naivevar;
    delete ray_adapvar;
    delete ray_adaprat;
    delete noncor_ray;
    delete noncor_ray_ratstep;
    delete noncor_ray_naivevar;
    delete noncor_ray_adapvar;
    delete noncor_ray_adaprat;
}

void ExponentialJansPlots::run(int test) const
{
    BlanchetBinFast<Float> *blanchet_bin_fast = new BlanchetBinFast<Float>(0.65);
    PCumeExp<Float> *pcume = new PCumeExp<Float>(1.0);
    PCmfExp<Float> *pcmf = new PCmfExp<Float>(1.0);
    PRatioExp<Float> *prat = new PRatioExp<Float>(1.0);
    RatioExp<Float> *rat = new RatioExp<Float>(1.0);
    // PRatioExp<Float> *prat = new PRatioExp<Float>(1.0); // adaptive
    NvidiaExp<Float> *nvidia = new NvidiaExp<Float>(1.0);

    std::vector<Technique<Float> *> techniques = std::vector<Technique<Float> *>();

    pcg32 rng = pcg32(0x12312, 0xadcd);

    if (test == 0)
    {
        techniques.push_back(blanchet_bin_fast);
        run_experiment_const(rng, techniques, "const_all/");
    }
    if (test == 1)
    {
        techniques.push_back(blanchet_bin_fast);
        run_experiment_noise(rng, techniques, "noise_all/");
    }
    if (test == 2)
    {
        techniques.push_back(blanchet_bin_fast);
        run_experiment_scos(rng, techniques, "scos_all/");
    }
    if (test == 3)
    {
        techniques.push_back(blanchet_bin_fast);
        run_experiment_gauss(rng, techniques, "gauss_all/");
    }
    if (test == 4)
    {
        techniques.push_back(pcume);
        run_experiment_const(rng, techniques, "const_all/");
    }
    if (test == 5)
    {
        techniques.push_back(pcume);
        run_experiment_noise(rng, techniques, "noise_all/");
    }
    if (test == 6)
    {
        techniques.push_back(pcume);
        run_experiment_scos(rng, techniques, "scos_all/");
    }
    if (test == 7)
    {
        techniques.push_back(pcume);
        run_experiment_gauss(rng, techniques, "gauss_all/");
    }
    if (test == 8)
    {
        techniques.push_back(prat);
        run_experiment_const(rng, techniques, "const_all/");
    }
    if (test == 9)
    {
        techniques.push_back(prat);
        run_experiment_noise(rng, techniques, "noise_all/");
    }
    if (test == 10)
    {
        techniques.push_back(prat);
        run_experiment_scos(rng, techniques, "scos_all/");
    }
    if (test == 11)
    {
        techniques.push_back(prat);
        run_experiment_gauss(rng, techniques, "gauss_all/");
    }
    if (test == 12)
    {
        techniques.push_back(pcmf);
        run_experiment_const(rng, techniques, "const_all/");
    }
    if (test == 13)
    {
        techniques.push_back(pcmf);
        run_experiment_noise(rng, techniques, "noise_all/");
    }
    if (test == 14)
    {
        techniques.push_back(pcmf);
        run_experiment_scos(rng, techniques, "scos_all/");
    }
    if (test == 15)
    {
        techniques.push_back(pcmf);
        run_experiment_gauss(rng, techniques, "gauss_all/");
    }

    // finish by cleaning up the datastructures
    delete blanchet_bin_fast;
    delete pcume;
    delete pcmf;
    delete prat;
    delete nvidia;
}
