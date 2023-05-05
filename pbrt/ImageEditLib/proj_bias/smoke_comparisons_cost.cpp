#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

void do_the_thing(std::string loc)
{
    std::cout << "doing the thing for " << loc << std::endl;
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/revisions/smoke/c8_gt.exr");
    imedit::Image accum = imedit::Image(gt.width(), gt.height());

    std::ofstream rmse_data;
    rmse_data.open("/Users/corneria/Documents/Research/bias_research/pbrttest/results/revisions/" + loc + "/rmse.txt");

    for (int index = 13; index <= 99; index++)
    {
        // if (index == 69)
        //     continue;
        std::cout << "doing for step size: " << double(index) / 100.0 << std::endl;

        std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/revisions/" + loc + "/0." + std::to_string(index);

        imedit::Image rat = imedit::Image(path + "/ratio.exr");
        imedit::Image ray = imedit::Image(path + "/ray.exr");
        imedit::Image tay = imedit::Image(path + "/taylor.exr");

        double rmse_err_rat = imedit::root_mean_sqr_error(gt, rat);
        double rmse_err_ray = imedit::root_mean_sqr_error(gt, ray);
        double rmse_err_tay = imedit::root_mean_sqr_error(gt, tay);

        std::ifstream rat_info;
        std::ifstream ray_info;
        std::ifstream tay_info;

        rat_info.open(path + "/ratio_info.txt");
        ray_info.open(path + "/ray_info.txt");
        tay_info.open(path + "/taylor_info.txt");

        double cost_rat = 0.0;
        double cost_ray = 0.0;
        double cost_tay = 0.0;

        rat_info >> cost_rat;
        rat_info >> cost_rat;

        ray_info >> cost_ray;
        ray_info >> cost_ray;

        tay_info >> cost_tay;
        tay_info >> cost_tay;

        rmse_data << cost_rat << std::endl;
        rmse_data << cost_ray << std::endl;
        rmse_data << cost_tay << std::endl;
        rmse_data << rmse_err_rat << std::endl;
        rmse_data << rmse_err_ray << std::endl;
        rmse_data << rmse_err_tay << std::endl;
    }

    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/revisions/" + loc + "/1.00";

    imedit::Image rat = imedit::Image(path + "/ratio.exr");
    imedit::Image ray = imedit::Image(path + "/ray.exr");
    imedit::Image tay = imedit::Image(path + "/taylor.exr");

    double rmse_err_rat = imedit::root_mean_sqr_error(gt, rat);
    double rmse_err_ray = imedit::root_mean_sqr_error(gt, ray);
    double rmse_err_tay = imedit::root_mean_sqr_error(gt, tay);

    std::ifstream rat_info;
    std::ifstream ray_info;
    std::ifstream tay_info;

    rat_info.open(path + "/ratio_info.txt");
    ray_info.open(path + "/ray_info.txt");
    tay_info.open(path + "/taylor_info.txt");

    double cost_rat = 0.0;
    double cost_ray = 0.0;
    double cost_tay = 0.0;

    rat_info >> cost_rat;
    rat_info >> cost_rat;

    ray_info >> cost_ray;
    ray_info >> cost_ray;

    tay_info >> cost_tay;
    tay_info >> cost_tay;

    rmse_data << cost_rat << std::endl;
    rmse_data << cost_ray << std::endl;
    rmse_data << cost_tay << std::endl;
    rmse_data << rmse_err_rat << std::endl;
    rmse_data << rmse_err_ray << std::endl;
    rmse_data << rmse_err_tay << std::endl;

    rmse_data.close();
}

// TODO: fix the renders and re-run this script
int main(int argc, char *argv[])
{
    do_the_thing("smoke_grid_one");
    do_the_thing("smoke_grid_two");
    do_the_thing("smoke_grid_three");
    do_the_thing("smoke_grid_four");
    do_the_thing("smoke_grid_five");
    do_the_thing("smoke_grid_six");
    do_the_thing("smoke_grid_seven");
    do_the_thing("smoke_grid_eight");

    return 0;
}
