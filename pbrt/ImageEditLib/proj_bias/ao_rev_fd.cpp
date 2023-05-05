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

void find_failed_runs()
{
    int failures = 0;

    std::string base_path = "/Users/corneria/Desktop/ao_cluster/";

    for (int index = 0; index < 8192 * 10; index++)
    {
        std::string path_to_check = base_path + std::to_string(index) + "_image.exr";

        std::ifstream test;
        test.open(path_to_check);

        // std::cout << "starting index: " << index << std::endl;
        // std::cout << "checking path: " << path_to_check << std::endl;

        if (test)
        {
            // does nothing
        }
        else
        {
            failures++;
            std::cout << index << " failed" << std::endl;
        }

        test.close();
    }

    std::cout << "failure check is complete" << std::endl;
    std::cout << "Number of failures: " << failures << std::endl;
}

void eval_progressive_final_camera_ready_scene()
{
    // TODO: maybe regenerate GT
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_2/gt_fd_image.exr");
    imedit::Image accum = imedit::Image(400, 400);

    std::string path = "/Users/corneria/Desktop/ao_cluster/";
    std::string res_path = "/Users/corneria/Desktop/ao_scene/";

    // std::ofstream rmse_data;
    // rmse_data.open(path + "rmse.txt");
    // std::ofstream mse_data;
    // mse_data.open(path + "mse.txt");

    // // get the color map
    // std::vector<imedit::Pixel>
    //     colors = std::vector<imedit::Pixel>();
    // color_map_inferno(colors);

    // // pre-process to get min and max values for bounds on proxy
    // // double min_val = 1000000.0;
    // // double max_val = -1000000.0;
    // // for (int index = 0; index < 8192; index++)
    // // {
    // //     std::cout << "pre-processing index: " << index << std::endl;
    // //     imedit::Image tmp = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_prog_2/" + std::to_string(index) + "_image.exr");
    // //     double tmp_min = tmp.min();
    // //     double tmp_max = tmp.max();

    // //     if (min_val > tmp_min)
    // //         min_val = tmp_min;
    // //     if (max_val < tmp_max)
    // //         max_val = tmp_max;
    // // }

    // // also check the ground truth image
    // double min_val = gt.min();
    // double max_val = gt.max();

    // std::cout << "min: " << min_val << std::endl;
    // std::cout << "max: " << max_val << std::endl;

    // bool check_local_stats = false;

    // if (check_local_stats)
    // {
    //     for (int index = 0; index < 8192 * 1; index++)
    //     {
    //         std::cout << "checking stats for: " << index << std::endl;
    //         imedit::Image tmp = imedit::Image(path + std::to_string(index) + "_image.exr");

    //         double tmp_min = tmp.min();
    //         double tmp_max = tmp.max();

    //         if (min_val > tmp_min)
    //             min_val = tmp_min;
    //         if (max_val < tmp_max)
    //             max_val = tmp_max;

    //         std::cout << "min: " << min_val << std::endl;
    //         std::cout << "max: " << max_val << std::endl;
    //         std::cout << std::endl;
    //     }
    // }

    // int failures = 0;

    // // if (min_val > tmp_min)
    // //     min_val = tmp_min;
    // // if (max_val < tmp_max)
    // //     max_val = tmp_max;

    int failures = 0;

    // for (int index = 0; index < 8192 * 0; index++)
    for (int index = 0; index < 8192 * 16; index++)
    // for (int index = 75800; index < 75900; index++)
    // for (int index = 0; index < 8192; index++)
    {
        try
        {
            if (index % 8192 == 0)
                std::cout << "starting index: " << index << std::endl;
            imedit::Image tmp = imedit::Image(path + std::to_string(index) + "_image.exr");

            accum = accum + (tmp - accum) * (1.0 / double(index + 1));

            // // compute rmse
            // double rmse_err = imedit::root_mean_sqr_error(gt, accum);
            // double mse_err = imedit::mean_sqr_error(gt, accum);

            // create and save negative positive image
            // negative_positive_image(accum, colors, min_val, max_val, path + std::to_string(index) + "_accum_col");

            // save non-colored image
            if (index == 0 || index == 1 || index == 3 || index == 7 || index == 15 || index == 31 || index == 63 || index == 127 || index == 255 || index == 511 || index == 1023 || (index + 1) % 2048 == 0)
            {
                accum.write(res_path + "accum_" + std::to_string(index) + ".exr");
            }
            // accum.write("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_prog_2/" + std::to_string(index) + "_accum.png");

            // rmse_data << rmse_err << std::endl;
            // mse_data << mse_err << std::endl;
        }
        catch (...)
        {
            failures++;
            std::cout << "Failed: " << index << std::endl;
        }
    }

    std::cout << "FAILED RUNS: " << failures << std::endl;

    // rmse_data.close();
    // mse_data.close();
}

void negative_positive_image_final(const imedit::Image &image,
                                   const std::vector<imedit::Pixel> &colors,
                                   double in_min_val,
                                   double in_max_val,
                                   std::string path)
{
    imedit::Image mod_image = image;

    imedit::Pixel neg_color = imedit::Pixel(0.0, 0.0, 1.0);
    imedit::Pixel pos_color = imedit::Pixel(1.0, 0.0, 0.0);

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = 0; j < mod_image.width(); ++j)
        {
            // basic
            // if (false && j < mod_image.width() / 2)
            if (false)
            {
                double proxy = mod_image(j, i, 0);
                double val = std::max(in_max_val, std::abs(in_min_val));
                // std::cout << "PROXY: " << proxy << std::endl;

                if (proxy < 0.0)
                {
                    proxy = std::abs(proxy);
                    proxy /= val;
                    mod_image(j, i) = neg_color * proxy;
                }
                else
                {
                    proxy /= val;
                    mod_image(j, i) = pos_color * proxy;
                }
            }
            else
            {
                // double proxy = mod_image(j, i, 0);

                // double tmp_max_val = (max_val - min_val) * 0.5 + min_val;
                // double tmp_min_val = (max_val - min_val) * 0.5 + min_val;

                // double tmp_max_val = (in_max_val * 0.75);
                // double tmp_min_val = (in_min_val * 0.75);

                // max_val = tmp_max_val;
                // min_val = tmp_min_val;

                double val = std::max(in_max_val, std::abs(in_min_val));
                // val = 50.0; // most scenes use this
                // val *= 0.3;

                // double max_val = in_max_val;
                // double min_val = in_min_val;

                // double max_val = tmp_max_val;
                // double min_val = tmp_min_val;

                // if (proxy < 0.0)
                // {
                //     proxy = std::abs(proxy) / std::abs(min_val);
                //     proxy *= 0.5;
                //     proxy = 0.5 - proxy;
                //     if (proxy < 0.0)
                //         proxy = 0.0;
                // }
                // else
                // {
                //     proxy /= max_val;
                //     proxy = proxy * 0.5 + 0.5;
                // }
                // // proxy = (proxy + max_proxy) / (2.0 * max_proxy); // range [-max_proxy, max_proxy]
                // // imedit::Pixel color = colors[0] * proxy + colors[colors.size() - 1] * (1.0 - proxy);

                // imedit::Pixel color = imedit::color_from_proxy_basic(colors, proxy);
                // // color = imedit::Pixel(proxy);
                // mod_image(j, i) = color;

                double proxy = mod_image(j, i, 0);

                if (proxy < 0.0)
                {
                    // proxy = std::abs(proxy) / std::abs(in_min_val);
                    proxy = std::abs(proxy) / val;
                    proxy *= 0.5;
                    proxy = 0.5 - proxy;
                    if (proxy < 0.0)
                        proxy = 0.0;
                }
                else
                {
                    // proxy /= in_max_val;
                    proxy /= val;
                    proxy = proxy * 0.5 + 0.5;
                    if (proxy > 1.0)
                        proxy = 0.99;
                }
                // proxy = (proxy + max_proxy) / (2.0 * max_proxy); // range [-max_proxy, max_proxy]
                // imedit::Pixel color = colors[0] * proxy + colors[colors.size() - 1] * (1.0 - proxy);

                imedit::Pixel color = imedit::color_from_proxy_basic(colors, proxy);
                // imedit::Pixel color = imedit::Pixel(proxy);
                mod_image(j, i) = color;
            }

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

    // double scale = 1.0;
    // mod_image.exposure(scale);

    // std::cout << "writing modded image" << std::endl;

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");
}

void color_map_from_image(std::string image_path, std::vector<imedit::Pixel> &colors)
{
    imedit::Image image = imedit::Image(image_path);

    for (int i = 3; i < image.width() - 3; ++i)
    {
        colors.push_back(image(i, image.height() / 2));
    }
}

void make_false_color_gt()
{
    // todo
    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_2/gt_fd_image.exr");
    imedit::Image accum_test = imedit::Image("/Users/corneria/Desktop/accum_test.exr");

    imedit::Image final_gt = imedit::Image(400, 400);

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/fusion.png", colors);

    std::string res_path = "/Users/corneria/Desktop/";
    double min_val = gt.min();
    double max_val = gt.max();

    negative_positive_image_final(gt, colors, min_val, max_val, res_path + "test_cm");
    negative_positive_image_final(accum_test, colors, min_val, max_val, res_path + "test_accum_cm");

    imedit::Image color_map(400, 50);
    color_map_image(color_map, colors);

    color_map.write(res_path + "cm.exr");
}

// void make_false_color_shader_gt()
// {
//     // todo
//     imedit::Image gt = imedit::Image("/Users/corneria/Desktop/image.exr");
//     // imedit::Image accum_test = imedit::Image("/Users/corneria/Desktop/accum_test.exr");

//     imedit::Image final_gt = imedit::Image(gt.width(), gt.height());

//     // get the color map
//     std::vector<imedit::Pixel>
//         colors = std::vector<imedit::Pixel>();
//     color_map_seaborn_dark(colors);
//     colors.clear();
//     color_map_from_image("/Users/corneria/Desktop/cm_2.png", colors);

//     std::string res_path = "/Users/corneria/Desktop/";
//     double min_val = gt.min();
//     double max_val = gt.max();

//     negative_positive_image_final(gt, colors, min_val, max_val, res_path + "shader");
//     // negative_positive_image_final(accum_test, colors, min_val, max_val, res_path + "test_accum_cm");

//     imedit::Image color_map(400, 50);
//     color_map_image(color_map, colors);

//     color_map.write(res_path + "cm.exr");
// }

void debug_color_map()
{
    imedit::Image image = imedit::Image("/Users/corneria/Desktop/cm_2.png");

    image.write("/Users/corneria/Desktop/cm_2_doublewrite.png");
    image.write("/Users/corneria/Desktop/cm_2_doublewrite.exr");

    imedit::Image exr_1 = imedit::Image("/Users/corneria/Desktop/cm_2_doublewrite.exr");
    exr_1.write("/Users/corneria/Desktop/cm_2_doublewrite_2.exr");

    imedit::Image png_1 = imedit::Image("/Users/corneria/Desktop/cm_2_doublewrite.png");
    png_1.write("/Users/corneria/Desktop/cm_2_doublewrite_2.png");
}

void fuck()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/consistent_rev/images/";
    imedit::Image image = imedit::Image(path + "path_0.exr");

    for (int i = 1; i < 16; ++i)
    {
        imedit::Image other = imedit::Image(path + "path_" + std::to_string(i) + ".exr");

        // accum + (tmp - accum) * (1.0 / double(index + 1));
        image = image + (other - image) * (1.0 / double(i + 1));
    }

    image.write("/Users/corneria/Desktop/test.exr");
}

void find_failed_runs_consist()
{
    int failures = 0;

    std::string base_path = "/Users/corneria/Desktop/consistent_rev/images/";

    for (int index = 0; index < 8192; index++)
    {
        std::string path_to_check = base_path + "path_" + std::to_string(index) + ".exr";

        std::ifstream test;
        test.open(path_to_check);

        // std::cout << "starting index: " << index << std::endl;
        // std::cout << "checking path: " << path_to_check << std::endl;

        if (test)
        {
            // does nothing
        }
        else
        {
            failures++;
            std::cout << index << " failed" << std::endl;
        }

        test.close();
    }

    std::cout << "failure check is complete" << std::endl;
    std::cout << "Number of failures: " << failures << std::endl;
}

void eval_progressive_final_camera_ready_consist_scene()
{
    // TODO: maybe regenerate GT
    // imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_2/gt_fd_image.exr");
    imedit::Image accum = imedit::Image(800, 800);

    std::string path = "/Users/corneria/Desktop/consistent_rev/images/";
    std::string res_path = "/Users/corneria/Desktop/consist_scene/";

    int failures = 0;

    // for (int index = 0; index < 8192 * 0; index++)
    for (int index = 0; index < 8192; index++)
    // for (int index = 0; index < 8192; index++)
    {
        try
        {
            std::cout << "starting index: " << index << std::endl;
            imedit::Image tmp = imedit::Image(path + "path_" + std::to_string(index) + ".exr");

            accum = accum + (tmp - accum) * (1.0 / double(index + 1));

            // save non-colored image
            if (index == 0 || index == 1 || index == 3 || index == 7 || index == 15 || index == 31 || index == 63 || index == 127 || index == 255 || index == 511 || index == 1023 || index == 2047 || index == 4095 || index == 8191)
            {
                accum.write(res_path + "accum_" + std::to_string(index) + ".exr");
            }
        }
        catch (...)
        {
            failures++;
            std::cout << index << std::endl;
        }
    }

    std::cout << "FAILED RUNS: " << failures << std::endl;
}

void make_inset_image(
    std::string input_path,
    std::string output_path,
    int start_x, int start_y,
    int width, int height)
{
    imedit::Image input = imedit::Image(input_path);
    imedit::Image output = imedit::Image(width, height);

    // std::cout << "here" << std::endl;

    for (int i = start_y; i < start_y + height; ++i)
    {
        for (int j = start_x; j < start_x + width; ++j)
        {
            output(j - start_x, i - start_y) = input(j, i);
        }
    }

    // std::cout << "writing image to: " << output_path << std::endl;

    output.write(output_path);
}

void quadruple_image_size(std::string param)
{
    imedit::Image input = imedit::Image(param);
    imedit::Image resized = imedit::Image(input.width() * 4, input.height() * 4);

    // std::cout << "what" << std::endl;
    for (int i = 0; i < resized.height(); ++i)
    {
        for (int j = 0; j < resized.width(); ++j)
        {
            // std::cout << "j: " << j << std::endl;
            int new_j = int(floor(float(j) / 4.0));
            int new_i = int(floor(float(i) / 4.0));
            // std::cout << "new j: " << new_j << std::endl;
            resized(j, i) = input(new_j, new_i);
        }
    }
    // std::cout << "done" << std::endl;

    resized.write(param);
}

void make_false_color_consist_gt()
{
    // todo
    // imedit::Image gt = imedit::Image("/Users/corneria/Desktop/image.exr");
    // imedit::Image accum_test = imedit::Image("/Users/corneria/Desktop/accum_test.exr");

    imedit::Image consist_1 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_15.exr");
    imedit::Image consist_2 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_255.exr");
    imedit::Image consist_3 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_8191.exr");

    // imedit::Image final_gt = imedit::Image(gt.width(), gt.height());

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/fusion.png", colors);

    std::string res_path = "/Users/corneria/Desktop/consist_scene/";
    double min_val = consist_1.min();
    double max_val = consist_1.max();

    negative_positive_image_final(consist_1, colors, min_val, max_val, res_path + "consist_1");
    negative_positive_image_final(consist_2, colors, min_val, max_val, res_path + "consist_2");
    negative_positive_image_final(consist_3, colors, min_val, max_val, res_path + "consist_3");
    // negative_positive_image_final(accum_test, colors, min_val, max_val, res_path + "test_accum_cm");

    make_inset_image(res_path + "consist_1.exr", res_path + "consist_1_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_2.exr", res_path + "consist_2_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_3.exr", res_path + "consist_3_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_1.exr", res_path + "consist_1_inset.png", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_2.exr", res_path + "consist_2_inset.png", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_3.exr", res_path + "consist_3_inset.png", 45, 40, 80, 80);

    quadruple_image_size(res_path + "consist_1_inset.png");
    quadruple_image_size(res_path + "consist_2_inset.png");
    quadruple_image_size(res_path + "consist_3_inset.png");

    // imedit::Image color_map(400, 50);
    // color_map_image(color_map, colors);

    // color_map.write(res_path + "cm.exr");
}

void make_false_color_shader_gt()
{
    // todo
    imedit::Image gt = imedit::Image("/Users/corneria/Desktop/rev_shader/gt_image.exr");
    imedit::Image biased = imedit::Image("/Users/corneria/Desktop/rev_shader/biased_image.exr");
    imedit::Image diff = imedit::Image("/Users/corneria/Desktop/rev_shader/diff_image.exr");
    // imedit::Image accum_test = imedit::Image("/Users/corneria/Desktop/accum_test.exr");

    // imedit::Image final_gt = imedit::Image(gt.width(), gt.height());

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/fusion.png", colors);

    std::string res_path = "/Users/corneria/Desktop/rev_shader/";
    double min_val = gt.min();
    double max_val = gt.max();

    negative_positive_image_final(gt, colors, min_val, max_val, res_path + "shader_gt");
    negative_positive_image_final(biased, colors, min_val, max_val, res_path + "shader_biased");
    negative_positive_image_final(diff, colors, min_val, max_val, res_path + "shader_diff");
    // negative_positive_image_final(gt, colors, min_val, max_val, res_path + "shader");
    // negative_positive_image_final(accum_test, colors, min_val, max_val, res_path + "test_accum_cm");

    // std::cout << "what " << std::endl;
    make_inset_image(res_path + "shader_gt.exr", res_path + "shader_gt_inset.exr", 175, 240, 50, 35);
    make_inset_image(res_path + "shader_biased.exr", res_path + "shader_biased_inset.exr", 175, 240, 50, 35);
    make_inset_image(res_path + "shader_diff.exr", res_path + "shader_diff_inset.exr", 175, 240, 50, 35);

    make_inset_image(res_path + "shader_gt.png", res_path + "shader_gt_inset.png", 175, 240, 50, 35);
    make_inset_image(res_path + "shader_biased.png", res_path + "shader_biased_inset.png", 175, 240, 50, 35);
    make_inset_image(res_path + "shader_diff.png", res_path + "shader_diff_inset.png", 175, 240, 50, 35);

    quadruple_image_size(res_path + "shader_gt_inset.png");
    quadruple_image_size(res_path + "shader_biased_inset.png");
    quadruple_image_size(res_path + "shader_diff_inset.png");

    // imedit::Image color_map(400, 50);
    // color_map_image(color_map, colors);

    // color_map.write(res_path + "cm.exr");
}

void negative_positive_image_final_wall(const imedit::Image &image,
                                        const std::vector<imedit::Pixel> &colors,
                                        double in_min_val,
                                        double in_max_val,
                                        std::string path)
{
    imedit::Image mod_image = image;

    imedit::Pixel neg_color = imedit::Pixel(0.0, 0.0, 1.0);
    imedit::Pixel pos_color = imedit::Pixel(1.0, 0.0, 0.0);

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = 0; j < mod_image.width(); ++j)
        {
            // basic
            // if (false && j < mod_image.width() / 2)
            if (false)
            {
                double proxy = mod_image(j, i, 0);
                double val = std::max(in_max_val, std::abs(in_min_val));
                // std::cout << "PROXY: " << proxy << std::endl;

                if (proxy < 0.0)
                {
                    proxy = std::abs(proxy);
                    proxy /= val;
                    mod_image(j, i) = neg_color * proxy;
                }
                else
                {
                    proxy /= val;
                    mod_image(j, i) = pos_color * proxy;
                }
            }
            else
            {
                double val = std::max(in_max_val, std::abs(in_min_val));

                double proxy = mod_image(j, i, 0);

                if (proxy < 0.0)
                {
                    // proxy = std::abs(proxy) / std::abs(in_min_val);
                    proxy = std::abs(proxy) / val;
                    proxy *= 0.5;
                    proxy = 0.5 - proxy;
                    if (proxy < 0.0)
                        proxy = 0.0;
                }
                else
                {
                    // proxy /= in_max_val;
                    proxy /= val;
                    proxy = proxy * 0.5 + 0.5;
                    if (proxy > 1.0)
                        proxy = 0.99;
                }
                // proxy = (proxy + max_proxy) / (2.0 * max_proxy); // range [-max_proxy, max_proxy]
                // imedit::Pixel color = colors[0] * proxy + colors[colors.size() - 1] * (1.0 - proxy);

                imedit::Pixel color = imedit::color_from_proxy_basic(colors, proxy);
                // imedit::Pixel color = imedit::Pixel(proxy);
                mod_image(j, i) = color;
            }
        }
    }

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = mod_image.width() / 2 - 2; j < mod_image.width() / 2 + 2; ++j)
        {
            mod_image(j, i) = imedit::Pixel(0.0);
        }
    }

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");
}

void make_false_color_ao_gt()
{
    // todo
    imedit::Image gt = imedit::Image("/Users/corneria/Desktop/rev_ao/gt.exr");
    imedit::Image accum_0 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_0.exr");
    imedit::Image accum_1 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_1.exr");
    imedit::Image accum_3 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_3.exr");
    imedit::Image accum_7 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_7.exr");
    imedit::Image accum_15 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_15.exr");
    imedit::Image accum_31 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_31.exr");
    imedit::Image accum_63 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_63.exr");
    imedit::Image accum_127 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_127.exr");
    imedit::Image accum_255 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_255.exr");
    imedit::Image accum_511 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_511.exr");
    imedit::Image accum_1023 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_1023.exr");
    imedit::Image accum_2047 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_2047.exr");
    imedit::Image accum_4095 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_4095.exr");
    imedit::Image accum_8191 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_8191.exr");
    imedit::Image accum_75775 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_75775.exr");
    // imedit::Image accum_test = imedit::Image("/Users/corneria/Desktop/accum_test.exr");

    // imedit::Image final_gt = imedit::Image(gt.width(), gt.height());

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/fusion.png", colors);

    std::string res_path = "/Users/corneria/Desktop/rev_ao/";
    double min_val = gt.min();
    double max_val = gt.max();

    negative_positive_image_final(gt, colors, min_val, max_val, res_path + "gt_image");
    negative_positive_image_final(accum_0, colors, min_val, max_val, res_path + "accum_0_image");
    negative_positive_image_final(accum_1, colors, min_val, max_val, res_path + "accum_1_image");
    negative_positive_image_final(accum_3, colors, min_val, max_val, res_path + "accum_3_image");
    negative_positive_image_final(accum_7, colors, min_val, max_val, res_path + "accum_7_image");
    negative_positive_image_final(accum_15, colors, min_val, max_val, res_path + "accum_15_image");
    negative_positive_image_final(accum_31, colors, min_val, max_val, res_path + "accum_31_image");
    negative_positive_image_final(accum_63, colors, min_val, max_val, res_path + "accum_63_image");
    negative_positive_image_final(accum_127, colors, min_val, max_val, res_path + "accum_127_image");
    negative_positive_image_final(accum_255, colors, min_val, max_val, res_path + "accum_255_image");
    negative_positive_image_final(accum_511, colors, min_val, max_val, res_path + "accum_511_image");
    negative_positive_image_final(accum_1023, colors, min_val, max_val, res_path + "accum_1023_image");
    negative_positive_image_final(accum_2047, colors, min_val, max_val, res_path + "accum_2047_image");
    negative_positive_image_final(accum_4095, colors, min_val, max_val, res_path + "accum_4095_image");
    negative_positive_image_final(accum_8191, colors, min_val, max_val, res_path + "accum_8191_image");
    negative_positive_image_final(accum_75775, colors, min_val, max_val, res_path + "accum_75775_image");

    imedit::Image one = gt;
    imedit::Image two = gt;
    imedit::Image three = gt;
    imedit::Image four = gt;

    for (int i = 0; i < gt.height(); ++i)
    {
        for (int j = gt.width() / 2; j < gt.width(); ++j)
        {
            one(j, i) = accum_0(j, i);
            two(j, i) = accum_15(j, i);
            three(j, i) = accum_1023(j, i);
            four(j, i) = accum_75775(j, i);
        }
    }

    negative_positive_image_final_wall(one, colors, min_val, max_val, res_path + "image_one");
    negative_positive_image_final_wall(two, colors, min_val, max_val, res_path + "image_two");
    negative_positive_image_final_wall(three, colors, min_val, max_val, res_path + "image_three");
    negative_positive_image_final_wall(four, colors, min_val, max_val, res_path + "image_four");

    // for (int i = 0; i < gt.height(); ++i)
    // {
    //     for (int j = gt.width()/2 - 2; j < gt.width()/2 + 2; ++j)
    //     {
    //         one(j, i) = imedit::Pixel(0.0);
    //         two(j, i) = imedit::Pixel(0.0);
    //         three(j, i) = imedit::Pixel(0.0);
    //         four(j, i) = imedit::Pixel(0.0);
    //     }
    // }

    // one.write(res_path + "image_one.exr");
    // two.write(res_path + "image_two.exr");
    // three.write(res_path + "image_three.exr");
    // four.write(res_path + "image_four.exr");

    // one.write(res_path + "image_one.png");
    // two.write(res_path + "image_two.png");
    // three.write(res_path + "image_three.png");
    // four.write(res_path + "image_four.png");

    // negative_positive_image_final(gt, colors, min_val, max_val, res_path + "gt_image");
    // negative_positive_image_final(accum_0, colors, min_val, max_val, res_path + "accum_0_image");
    // negative_positive_image_final(accum_1, colors, min_val, max_val, res_path + "accum_1_image");
    // negative_positive_image_final(accum_3, colors, min_val, max_val, res_path + "accum_3_image");
    // negative_positive_image_final(accum_7, colors, min_val, max_val, res_path + "accum_7_image");
    // negative_positive_image_final(accum_15, colors, min_val, max_val, res_path + "accum_15_image");
    // negative_positive_image_final(accum_31, colors, min_val, max_val, res_path + "accum_31_image");
    // negative_positive_image_final(accum_63, colors, min_val, max_val, res_path + "accum_63_image");
    // negative_positive_image_final(accum_127, colors, min_val, max_val, res_path + "accum_127_image");
    // negative_positive_image_final(accum_255, colors, min_val, max_val, res_path + "accum_255_image");
    // negative_positive_image_final(accum_511, colors, min_val, max_val, res_path + "accum_511_image");
    // negative_positive_image_final(accum_1023, colors, min_val, max_val, res_path + "accum_1023_image");
    // negative_positive_image_final(accum_2047, colors, min_val, max_val, res_path + "accum_2047_image");
    // negative_positive_image_final(accum_4095, colors, min_val, max_val, res_path + "accum_4095_image");
    // negative_positive_image_final(accum_8191, colors, min_val, max_val, res_path + "accum_8191_image");
    // negative_positive_image_final(accum_75775, colors, min_val, max_val, res_path + "accum_75775_image");

    // negative_positive_image_final(gt, colors, min_val, max_val, res_path + "shader_gt");
    // negative_positive_image_final(biased, colors, min_val, max_val, res_path + "shader_biased");
    // negative_positive_image_final(diff, colors, min_val, max_val, res_path + "shader_diff");
    // negative_positive_image_final(gt, colors, min_val, max_val, res_path + "shader");
    // negative_positive_image_final(accum_test, colors, min_val, max_val, res_path + "test_accum_cm");

    // std::cout << "what " << std::endl;
    // make_inset_image(res_path + "shader_gt.exr", res_path + "shader_gt_inset.exr", 175, 240, 50, 35);
    // make_inset_image(res_path + "shader_biased.exr", res_path + "shader_biased_inset.exr", 175, 240, 50, 35);
    // make_inset_image(res_path + "shader_diff.exr", res_path + "shader_diff_inset.exr", 175, 240, 50, 35);

    // make_inset_image(res_path + "shader_gt.png", res_path + "shader_gt_inset.png", 175, 240, 50, 35);
    // make_inset_image(res_path + "shader_biased.png", res_path + "shader_biased_inset.png", 175, 240, 50, 35);
    // make_inset_image(res_path + "shader_diff.png", res_path + "shader_diff_inset.png", 175, 240, 50, 35);

    // quadruple_image_size(res_path + "shader_gt_inset.png");
    // quadruple_image_size(res_path + "shader_biased_inset.png");
    // quadruple_image_size(res_path + "shader_diff_inset.png");

    // imedit::Image color_map(400, 50);
    // color_map_image(color_map, colors);

    // color_map.write(res_path + "cm.exr");
}

void compute_ao_error()
{
    std::cout << "what" << std::endl;
    imedit::Image gt = imedit::Image("/Users/corneria/Desktop/rev_ao/gt.exr");
    // imedit::Image accum = imedit::Image(400, 400);

    std::string path = "/Users/corneria/Desktop/rev_ao/";
    // std::string res_path = "/Users/corneria/Desktop/ao_scene/";

    std::ofstream rmse_data;
    rmse_data.open(path + "rmse.txt");
    std::ofstream mse_data;
    mse_data.open(path + "mse.txt");

    int samps = 1;

    for (; samps < 10000; samps *= 2)
    {
        std::cout << "parsing: " << path + "accum_" + std::to_string(samps - 1) + ".exr" << std::endl;
        imedit::Image accumed = imedit::Image(path + "accum_" + std::to_string(samps - 1) + ".exr");

        double rmse_err = imedit::root_mean_sqr_error(gt, accumed);
        double mse_err = imedit::mean_sqr_error(gt, accumed);

        rmse_data << samps << std::endl
                  << rmse_err << std::endl;
        mse_data << samps << std::endl
                 << mse_err << std::endl;
    }

    rmse_data.close();
    mse_data.close();
}

int main(int argc, char *argv[])
{
    std::cout << "evaluating progressive translation scene" << std::endl;
    // eval_progressive_trans_scene();
    // eval_progressive_x_scene();
    // eval_progressive_z_scene();
    // eval_progressive_final_scene(); // this one

    // debug_color_map();

    // find_failed_runs();
    eval_progressive_final_camera_ready_scene();

    // make_false_color_gt();

    // make_false_color_shader_gt();

    // find_failed_runs_consist();
    // eval_progressive_final_camera_ready_consist_scene();

    // fuck();

    // consistent scene revision
    // make_false_color_consist_gt();

    // shader scene revision
    // make_false_color_shader_gt();

    // ao scene revision
    // compute_ao_error();
    // make_false_color_ao_gt();

    // std::cout << "evaluating progressive scene" << std::endl;
    // eval_progressive_scene();
    // std::cout << "evaluating debiased scene" << std::endl;
    // eval_debiased_scene();
    // std::cout << "generating graphs" << std::endl;
    // system("python3 /Users/corneria/Documents/Research/bias_research/pbrttest/src/scripts/ao_fd.py");

    return 0;
}
