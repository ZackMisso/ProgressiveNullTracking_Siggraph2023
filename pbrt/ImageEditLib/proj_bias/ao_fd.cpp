#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

void eval_progressive_scene()
{
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_fd_analytic/image.exr");
    imedit::Image accum = imedit::Image(gt.width(), gt.height());

    std::ofstream rmse_data;
    rmse_data.open("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_progressive/rmse.txt");

    for (int index = 0; index < 5000; index++)
    {
        std::cout << "starting index: " << index << std::endl;
        imedit::Image tmp = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_progressive/" + std::to_string(index) + "_image.exr");

        accum = accum + (tmp - accum) * (1.0 / double(index + 1));

        double rmse_err = imedit::root_mean_sqr_error(gt, accum);

        accum.write("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_progressive/" + std::to_string(index) + "_accum.exr");

        rmse_data << rmse_err << std::endl;
    }

    rmse_data.close();
}

void eval_progressive_trans_scene()
{
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans/image.exr");
    imedit::Image accum = imedit::Image(400, 400);

    std::ofstream rmse_data;
    rmse_data.open("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_progressive_test/rmse.txt");

    for (int index = 0; index < 3885; index++)
    {
        std::cout << "starting index: " << index << std::endl;
        imedit::Image tmp = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_progressive_test/" + std::to_string(index) + "_image.exr");

        accum = accum + (tmp - accum) * (1.0 / double(index + 1));

        double rmse_err = imedit::root_mean_sqr_error(gt, accum);

        accum.write("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_progressive_test/" + std::to_string(index) + "_accum.exr");

        rmse_data << rmse_err << std::endl;
    }

    rmse_data.close();
}

void eval_progressive_x_scene()
{
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans_x_sol/image.exr");
    imedit::Image accum = imedit::Image(400, 400);

    std::ofstream rmse_data;
    rmse_data.open("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans_x_prog/rmse.txt");

    for (int index = 0; index < 4192; index++)
    {
        std::cout << "starting index: " << index << std::endl;
        imedit::Image tmp = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans_x_prog/" + std::to_string(index) + "_image.exr");

        accum = accum + (tmp - accum) * (1.0 / double(index + 1));

        double rmse_err = imedit::root_mean_sqr_error(gt, accum);

        accum.write("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans_x_prog/" + std::to_string(index) + "_accum.exr");

        rmse_data << rmse_err << std::endl;
    }

    rmse_data.close();
}

void eval_progressive_z_scene()
{
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans_z_sol/image.exr");
    imedit::Image accum = imedit::Image(400, 400);

    std::ofstream rmse_data;
    rmse_data.open("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans_z_prog/rmse.txt");

    for (int index = 0; index < 3042; index++)
    {
        std::cout << "starting index: " << index << std::endl;
        imedit::Image tmp = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans_z_prog/" + std::to_string(index) + "_image.exr");

        accum = accum + (tmp - accum) * (1.0 / double(index + 1));

        double rmse_err = imedit::root_mean_sqr_error(gt, accum);

        accum.write("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_prog_trans_z_prog/" + std::to_string(index) + "_accum.exr");

        rmse_data << rmse_err << std::endl;
    }

    rmse_data.close();
}

// void negative_positive_image(const imedit::Image &image,
//                              const std::vector<imedit::Pixel> &colors,
//                              double max_proxy,
//                              std::string path)
// {
//     imedit::Image mod_image = image;
//     double neg_amp = 20.0;

//     for (int i = 0; i < mod_image.height(); ++i)
//     {
//         for (int j = 0; j < mod_image.width(); ++j)
//         {
//             double proxy = mod_image(j, i, 0);
//             // proxy = (proxy + max_proxy) / (2.0 * max_proxy); // range [-max_proxy, max_proxy]
//             // imedit::Pixel color = colors[0] * proxy + colors[colors.size() - 1] * (1.0 - proxy);
//             // mod_image(j, i) = color;

//             // if (proxy < 0.0)
//             //     mod_image(j, i) = mod_image(j, i) * imedit::Pixel(0.0, 0.0, -1.0);
//             // else
//             //     mod_image(j, i) = mod_image(j, i) * imedit::Pixel(1.0, 0.0, 0.0);

//             if (proxy < 0.0)
//                 mod_image(j, i) = mod_image(j, i) * imedit::Pixel(neg_amp * -3.22609671e-01 * 0.5, neg_amp * -5.56335178e-02 * 0.5, neg_amp * -4.26376869e-01 * 0.5);
//             else
//                 mod_image(j, i) = mod_image(j, i) * imedit::Pixel(9.68040817e-01, 8.28515491e-01, 2.49971582e-01);

//             // Pixel(3.22609671e-01, 5.56335178e-02, 4.26376869e-01)
//             //     Pixel(9.68040817e-01, 8.28515491e-01, 2.49971582e-01)
//         }
//     }

//     double scale = 100.0;
//     mod_image.exposure(scale);

//     // std::cout << "writing modded image" << std::endl;

//     mod_image.write(path + ".exr");
//     mod_image.write(path + ".png");
// }

void negative_positive_image(const imedit::Image &image,
                             const std::vector<imedit::Pixel> &colors_bl,
                             double min_val,
                             double max_val,
                             std::string path)
{
    imedit::Image mod_image = image;

    imedit::Pixel neg_color = imedit::Pixel(0.0, 0.0, 1.0);
    imedit::Pixel pos_color = imedit::Pixel(1.0, 0.0, 0.0);

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = 0; j < mod_image.width(); ++j)
        {
            double proxy = mod_image(j, i, 0);

            if (proxy < 0.0)
            {
                proxy /= min_val;
                mod_image(j, i) = neg_color * proxy;
            }
            else
            {
                proxy /= max_val;
                mod_image(j, i) = pos_color * proxy;
            }
            // proxy = (proxy + max_proxy) / (2.0 * max_proxy); // range [-max_proxy, max_proxy]
            // imedit::Pixel color = colors[0] * proxy + colors[colors.size() - 1] * (1.0 - proxy);
            // mod_image(j, i) = color;

            // if (proxy < 0.0)
            //     mod_image(j, i) = mod_image(j, i) * imedit::Pixel(0.0, 0.0, -1.0);
            // else
            //     mod_image(j, i) = mod_image(j, i) * imedit::Pixel(1.0, 0.0, 0.0);

            // if (proxy < 0.0)
            //     mod_image(j, i) = mod_image(j, i) * imedit::Pixel(neg_amp * -3.22609671e-01 * 0.5, neg_amp * -5.56335178e-02 * 0.5, neg_amp * -4.26376869e-01 * 0.5);
            // else
            //     mod_image(j, i) = mod_image(j, i) * imedit::Pixel(9.68040817e-01, 8.28515491e-01, 2.49971582e-01);

            // Pixel(3.22609671e-01, 5.56335178e-02, 4.26376869e-01)
            //     Pixel(9.68040817e-01, 8.28515491e-01, 2.49971582e-01)
        }
    }

    // std::vector<imedit::Pixel> colors = std::vector<imedit::Pixel>();

    // colors.push_back(imedit::Pixel(0.230, 0.299, 0.754));
    // colors.push_back(imedit::Pixel(0.0, 0.0, 0.0));
    // colors.push_back(imedit::Pixel(0.706, 0.016, 0.150));

    // for (int i = 0; i < mod_image.height(); ++i)
    // {
    //     for (int j = 0; j < mod_image.width(); ++j)
    //     {
    //         double proxy = mod_image(j, i, 0);
    //         bool flip = false;

    //         if (proxy < 0.0)
    //         {
    //             proxy = (1.0 - proxy / min_val) * 0.5;
    //         }
    //         else
    //         {
    //             proxy = proxy / max_val * 0.5 + 0.5;
    //             flip = true;
    //             // if (proxy == 0.5)
    //             // {
    //             //     std::cout << "here" << std::endl;
    //             //     std::cout << proxy * double(colors.size() - 1) << std::endl;
    //             //     std::cout << floor(proxy * double(colors.size() - 1)) << std::endl;
    //             // }
    //         }

    //         if (proxy < 0.0)
    //             proxy = 0.0;
    //         if (proxy > 1.0)
    //             proxy = 1.0;

    //         double location = proxy * double(colors.size() - 1);
    //         double diff = location - floor(location);
    //         int index_one = floor(location);
    //         int index_two = index_one + 1;
    //         // if (flip || true)
    //         // {
    //         //     diff = ceil(location) - location;
    //         // }
    //         // float diff = ceil(location) - location;
    //         if (index_two > colors.size() - 1)
    //             index_two = index_one;

    //         imedit::Pixel color_one = colors[index_one];
    //         imedit::Pixel color_two = colors[index_two];

    //         // mod_image(j, i) = imedit::Pixel(proxy) *
    //         //                   1.0;

    //         // if (diff == 0.0)
    //         // {
    //         //     // std::cout << "index: " << index_one << std::endl;
    //         //     // std::cout << "loc: " << location << std::endl;
    //         //     mod_image(j, i) = imedit::Pixel(color_one.r,
    //         //                                     color_one.g,
    //         //                                     color_one.b) *
    //         //                       1.0;
    //         // }
    //         // else
    //         // {

    //         mod_image(j, i) = imedit::Pixel(color_one.r * (1.f - diff) + color_two.r * diff,
    //                                         color_one.g * (1.f - diff) + color_two.g * diff,
    //                                         color_one.b * (1.f - diff) + color_two.b * diff) *
    //                           1.0;
    //         // }
    //     }
    // }

    double scale = 1.0;
    mod_image.exposure(scale);

    // std::cout << "writing modded image" << std::endl;

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");
}

void eval_progressive_final_scene()
{
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_2/gt_fd_image.exr");
    imedit::Image accum = imedit::Image(400, 400);

    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_prog_2/";

    std::ofstream rmse_data;
    rmse_data.open(path + "rmse.txt");
    std::ofstream mse_data;
    mse_data.open(path + "mse.txt");

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_inferno(colors);

    // pre-process to get min and max values for bounds on proxy
    // double min_val = 1000000.0;
    // double max_val = -1000000.0;
    // for (int index = 0; index < 8192; index++)
    // {
    //     std::cout << "pre-processing index: " << index << std::endl;
    //     imedit::Image tmp = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_prog_2/" + std::to_string(index) + "_image.exr");
    //     double tmp_min = tmp.min();
    //     double tmp_max = tmp.max();

    //     if (min_val > tmp_min)
    //         min_val = tmp_min;
    //     if (max_val < tmp_max)
    //         max_val = tmp_max;
    // }

    // also check the ground truth image
    double min_val = gt.min();
    double max_val = gt.max();

    std::cout << "min: " << min_val << std::endl;
    std::cout << "max: " << max_val << std::endl;

    bool check_local_stats = false;

    if (check_local_stats)
    {
        for (int index = 0; index < 8192 * 1; index++)
        {
            std::cout << "checking stats for: " << index << std::endl;
            imedit::Image tmp = imedit::Image(path + std::to_string(index) + "_image.exr");

            double tmp_min = tmp.min();
            double tmp_max = tmp.max();

            if (min_val > tmp_min)
                min_val = tmp_min;
            if (max_val < tmp_max)
                max_val = tmp_max;

            std::cout << "min: " << min_val << std::endl;
            std::cout << "max: " << max_val << std::endl;
            std::cout << std::endl;
        }
    }

    int failures = 0;

    // if (min_val > tmp_min)
    //     min_val = tmp_min;
    // if (max_val < tmp_max)
    //     max_val = tmp_max;

    // for (int index = 0; index < 8192 * 0; index++)
    for (int index = 0; index < 8192 * 1; index++)
    // for (int index = 0; index < 8192; index++)
    {
        try
        {
            std::cout << "starting index: " << index << std::endl;
            imedit::Image tmp = imedit::Image(path + std::to_string(index) + "_image.exr");

            accum = accum + (tmp - accum) * (1.0 / double(index + 1));

            // compute rmse
            double rmse_err = imedit::root_mean_sqr_error(gt, accum);
            double mse_err = imedit::mean_sqr_error(gt, accum);

            // create and save negative positive image
            negative_positive_image(accum, colors, min_val, max_val, path + std::to_string(index) + "_accum_col");

            // save non-colored image
            accum.write(path + std::to_string(index) + "_accum.exr");
            // accum.write("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_prog_2/" + std::to_string(index) + "_accum.png");

            rmse_data << rmse_err << std::endl;
            mse_data << mse_err << std::endl;
        }
        catch (...)
        {
            failures++;
            std::cout << index << std::endl;
        }
    }

    std::cout << "FAILED RUNS: " << failures << std::endl;

    rmse_data.close();
    mse_data.close();
}

void eval_debiased_scene()
{
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_fd_analytic/image.exr");

    std::ofstream rmse_data;
    rmse_data.open("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_fd_debiased/rmse.txt");

    for (int index = 1; index <= 16384; index *= 2)
    {
        std::cout << "computing index: " << index << std::endl;
        imedit::Image tmp = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_fd_debiased/image_ps_" + std::to_string(index) + ".exr");
        double rmse_err = imedit::root_mean_sqr_error(gt, tmp);
        rmse_data << rmse_err << std::endl;
    }

    rmse_data.close();
}

int main(int argc, char *argv[])
{
    std::cout << "evaluating progressive translation scene" << std::endl;
    // eval_progressive_trans_scene();
    // eval_progressive_x_scene();
    // eval_progressive_z_scene();
    eval_progressive_final_scene();
    // std::cout << "evaluating progressive scene" << std::endl;
    // eval_progressive_scene();
    // std::cout << "evaluating debiased scene" << std::endl;
    // eval_debiased_scene();
    // std::cout << "generating graphs" << std::endl;
    // system("python3 /Users/corneria/Documents/Research/bias_research/pbrttest/src/scripts/ao_fd.py");

    return 0;
}
