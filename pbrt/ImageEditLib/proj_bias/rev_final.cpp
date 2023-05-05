#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

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
    for (int index = 0; index < 8192 * 10; index++)
    // for (int index = 0; index < 8192; index++)
    {
        try
        {
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
            std::cout << index << std::endl;
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
                    proxy = std::abs(proxy) / std::abs(in_min_val);
                    // proxy = std::abs(proxy) / val;
                    proxy *= 0.5;
                    proxy = 0.5 - proxy;
                    if (proxy < 0.0)
                        proxy = 0.0;
                }
                else
                {
                    proxy /= in_max_val;
                    // proxy /= val;
                    // proxy *= 10.0;
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

////////// CONSIST COLOR MAPS //////////
void make_false_color_fusion_consist_gt()
{
    imedit::Image consist_1 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_15.exr");
    imedit::Image consist_2 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_255.exr");
    imedit::Image consist_3 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_8191.exr");

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/fusion.png", colors);

    std::string res_path = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/consist_fusion/";
    double min_val = consist_1.min();
    double max_val = consist_1.max();

    double tmp_val = 40.0;

    negative_positive_image_final(consist_1, colors, -tmp_val, tmp_val, res_path + "consist_1");
    negative_positive_image_final(consist_2, colors, -tmp_val, tmp_val, res_path + "consist_2");
    negative_positive_image_final(consist_3, colors, -tmp_val, tmp_val, res_path + "consist_3");

    make_inset_image(res_path + "consist_1.exr", res_path + "consist_1_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_2.exr", res_path + "consist_2_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_3.exr", res_path + "consist_3_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_1.exr", res_path + "consist_1_inset.png", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_2.exr", res_path + "consist_2_inset.png", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_3.exr", res_path + "consist_3_inset.png", 45, 40, 80, 80);

    quadruple_image_size(res_path + "consist_1_inset.png");
    quadruple_image_size(res_path + "consist_2_inset.png");
    quadruple_image_size(res_path + "consist_3_inset.png");
}

void make_false_color_iceburn_consist_gt()
{
    imedit::Image consist_1 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_15.exr");
    imedit::Image consist_2 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_255.exr");
    imedit::Image consist_3 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_8191.exr");

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/cm_2.png", colors);

    std::string res_path = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/consist_iceburn/";
    double min_val = consist_1.min();
    double max_val = consist_1.max();

    double tmp_val = 40.0;

    negative_positive_image_final(consist_1, colors, -tmp_val, tmp_val, res_path + "consist_1");
    negative_positive_image_final(consist_2, colors, -tmp_val, tmp_val, res_path + "consist_2");
    negative_positive_image_final(consist_3, colors, -tmp_val, tmp_val, res_path + "consist_3");

    make_inset_image(res_path + "consist_1.exr", res_path + "consist_1_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_2.exr", res_path + "consist_2_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_3.exr", res_path + "consist_3_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_1.exr", res_path + "consist_1_inset.png", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_2.exr", res_path + "consist_2_inset.png", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_3.exr", res_path + "consist_3_inset.png", 45, 40, 80, 80);

    quadruple_image_size(res_path + "consist_1_inset.png");
    quadruple_image_size(res_path + "consist_2_inset.png");
    quadruple_image_size(res_path + "consist_3_inset.png");
}
////////// CONSIST COLOR MAPS //////////

////////// SHADER COLOR MAPS //////////
void make_false_color_fusion_shader_gt()
{
    // todo
    imedit::Image gt = imedit::Image("/Users/corneria/Desktop/rev_shader/gt_image.exr");
    imedit::Image biased = imedit::Image("/Users/corneria/Desktop/rev_shader/biased_image.exr");
    imedit::Image diff = imedit::Image("/Users/corneria/Desktop/rev_shader/diff_image.exr");

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/fusion.png", colors);

    std::string res_path = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/shader_fusion/";
    double min_val = gt.min();
    double max_val = gt.max();

    negative_positive_image_final(gt, colors, -50.0, 50.0, res_path + "shader_gt");
    negative_positive_image_final(biased, colors, -50.0, 50.0, res_path + "shader_biased");
    negative_positive_image_final(diff, colors, -50.0, 50.0, res_path + "shader_diff");

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
}

void make_false_color_iceburn_shader_gt()
{
    // todo
    imedit::Image gt = imedit::Image("/Users/corneria/Desktop/rev_shader/gt_image.exr");
    imedit::Image biased = imedit::Image("/Users/corneria/Desktop/rev_shader/biased_image.exr");
    imedit::Image diff = imedit::Image("/Users/corneria/Desktop/rev_shader/diff_image.exr");

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/cm_2.png", colors);

    std::string res_path = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/shader_iceburn/";
    double min_val = gt.min();
    double max_val = gt.max();

    double tmp_max = 40.0;

    negative_positive_image_final(gt, colors, -tmp_max, tmp_max, res_path + "shader_gt");
    negative_positive_image_final(biased, colors, -tmp_max, tmp_max, res_path + "shader_biased");
    negative_positive_image_final(diff, colors, -tmp_max, tmp_max, res_path + "shader_diff");

    make_inset_image(res_path + "shader_gt.exr", res_path + "shader_gt_inset.exr", 175, 240, 50, 35);
    make_inset_image(res_path + "shader_biased.exr", res_path + "shader_biased_inset.exr", 175, 240, 50, 35);
    make_inset_image(res_path + "shader_diff.exr", res_path + "shader_diff_inset.exr", 175, 240, 50, 35);

    make_inset_image(res_path + "shader_gt.png", res_path + "shader_gt_inset.png", 175, 240, 50, 35);
    make_inset_image(res_path + "shader_biased.png", res_path + "shader_biased_inset.png", 175, 240, 50, 35);
    make_inset_image(res_path + "shader_diff.png", res_path + "shader_diff_inset.png", 175, 240, 50, 35);

    quadruple_image_size(res_path + "shader_gt_inset.png");
    quadruple_image_size(res_path + "shader_biased_inset.png");
    quadruple_image_size(res_path + "shader_diff_inset.png");
}
////////// SHADER COLOR MAPS //////////

void negative_positive_image_final_wall(const imedit::Image &image,
                                        const std::vector<imedit::Pixel> &colors,
                                        imedit::Pixel wall_color,
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
            if (false)
            {
                double proxy = mod_image(j, i, 0);
                double val = std::max(in_max_val, std::abs(in_min_val));

                if (proxy < 0.0)
                {
                    proxy = std::abs(proxy);
                    // proxy /= std::abs(in_min_val);
                    proxy /= val;
                    mod_image(j, i) = neg_color * proxy;
                }
                else
                {
                    // proxy /= std::abs(in_max_val);
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
                    proxy = std::abs(proxy) / std::abs(in_min_val);
                    // proxy = std::abs(std::pow(std::abs(proxy), 1.0 / 1.4)) / std::abs(in_min_val);
                    // proxy = std::abs(proxy) / val;
                    // proxy = std::pow(proxy, 5.0);
                    proxy *= 0.5;
                    proxy = 0.5 - proxy;
                    if (proxy < 0.0)
                        proxy = 0.0;
                }
                else
                {
                    // double a = 1.0;
                    // double n = 0.5;
                    // proxy = a * pow(fabs(proxy), n);

                    proxy /= in_max_val;

                    // proxy = std::pow(std::abs(proxy), 1.0 / 1.4) / in_max_val;
                    // proxy *= 1.25;
                    // proxy = std::pow(proxy, 5.0);
                    // proxy /= val;
                    // proxy = proxy * 0.5 + 0.5;
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

    // for (int i = 0; i < mod_image.height(); ++i)
    // {
    //     for (int j = mod_image.width() / 2 - 2; j < mod_image.width() / 2 + 2; ++j)
    //     {
    //         mod_image(j, i) = wall_color;
    //     }
    // }

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");
}

////////// AO COLOR MAPS //////////
void make_false_color_fusion_ao_gt()
{
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

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/fusion.png", colors);

    std::string res_path = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/ao_fusion/";
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

    negative_positive_image_final_wall(one, colors, imedit::Pixel(0.0), min_val, max_val, res_path + "image_one");
    negative_positive_image_final_wall(two, colors, imedit::Pixel(0.0), min_val, max_val, res_path + "image_two");
    negative_positive_image_final_wall(three, colors, imedit::Pixel(0.0), min_val, max_val, res_path + "image_three");
    negative_positive_image_final_wall(four, colors, imedit::Pixel(0.0), min_val, max_val, res_path + "image_four");
}

void make_false_color_iceburn_ao_gt()
{
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

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/cm_2.png", colors);

    std::string res_path = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/ao_iceburn/";
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

    negative_positive_image_final_wall(one, colors, imedit::Pixel(1.0), min_val, max_val, res_path + "image_one");
    negative_positive_image_final_wall(two, colors, imedit::Pixel(1.0), min_val, max_val, res_path + "image_two");
    negative_positive_image_final_wall(three, colors, imedit::Pixel(1.0), min_val, max_val, res_path + "image_three");
    negative_positive_image_final_wall(four, colors, imedit::Pixel(1.0), min_val, max_val, res_path + "image_four");
}
////////// AO COLOR MAPS //////////

void compute_ao_error()
{
    // std::cout << "what" << std::endl;
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

void make_false_color_shader_for_cmap(
    std::string cmap,
    double min,
    double max)
{
    // todo
    imedit::Image gt = imedit::Image("/Users/corneria/Desktop/rev_shader/gt_image.exr");
    imedit::Image biased = imedit::Image("/Users/corneria/Desktop/rev_shader/biased_image.exr");
    imedit::Image diff = imedit::Image("/Users/corneria/Desktop/rev_shader/diff_image.exr");

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/cmaps/" + cmap + ".png", colors);

    // std::string res_path = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/shader_fusion/";
    std::string res_path = "/Users/corneria/Desktop/cmaps/" + cmap + "/shader/";

    std::string cmd = "mkdir /Users/corneria/Desktop/cmaps/" + cmap + "/";
    system(cmd.c_str());
    cmd = "mkdir " + res_path;
    system(cmd.c_str());

    negative_positive_image_final(gt, colors, min, max, res_path + "shader_gt");
    negative_positive_image_final(biased, colors, min, max, res_path + "shader_biased");
    negative_positive_image_final(diff, colors, min, max, res_path + "shader_diff");

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
}

void make_false_color_consist_for_cmap(
    std::string cmap,
    double min,
    double max)
{
    imedit::Image consist_1 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_15.exr");
    imedit::Image consist_2 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_255.exr");
    imedit::Image consist_3 = imedit::Image("/Users/corneria/Desktop/consist_scene/accum_8191.exr");

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/cmaps/" + cmap + ".png", colors);

    // std::string res_path = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/consist_fusion/";
    std::string res_path = "/Users/corneria/Desktop/cmaps/" + cmap + "/consist/";

    std::string cmd = "mkdir /Users/corneria/Desktop/cmaps/" + cmap + "/";
    system(cmd.c_str());
    cmd = "mkdir " + res_path;
    system(cmd.c_str());

    negative_positive_image_final(consist_1, colors, min, max, res_path + "consist_1");
    negative_positive_image_final(consist_2, colors, min, max, res_path + "consist_2");
    negative_positive_image_final(consist_3, colors, min, max, res_path + "consist_3");

    make_inset_image(res_path + "consist_1.exr", res_path + "consist_1_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_2.exr", res_path + "consist_2_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_3.exr", res_path + "consist_3_inset.exr", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_1.exr", res_path + "consist_1_inset.png", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_2.exr", res_path + "consist_2_inset.png", 45, 40, 80, 80);
    make_inset_image(res_path + "consist_3.exr", res_path + "consist_3_inset.png", 45, 40, 80, 80);

    quadruple_image_size(res_path + "consist_1_inset.png");
    quadruple_image_size(res_path + "consist_2_inset.png");
    quadruple_image_size(res_path + "consist_3_inset.png");
}

void make_false_color_ao_for_cmap(
    std::string cmap,
    double min,
    double max,
    imedit::Pixel color)
{
    imedit::Image gt = imedit::Image("/Users/corneria/Desktop/rev_ao/gt.exr");
    imedit::Image accum_0 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_0.exr");
    // imedit::Image accum_1 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_1.exr");
    // imedit::Image accum_3 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_3.exr");
    // imedit::Image accum_7 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_7.exr");
    imedit::Image accum_15 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_15.exr");
    // imedit::Image accum_31 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_31.exr");
    // imedit::Image accum_63 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_63.exr");
    // imedit::Image accum_127 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_127.exr");
    // imedit::Image accum_255 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_255.exr");
    // imedit::Image accum_511 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_511.exr");
    imedit::Image accum_1023 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_1023.exr");
    // imedit::Image accum_2047 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_2047.exr");
    // imedit::Image accum_4095 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_4095.exr");
    // imedit::Image accum_8191 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_8191.exr");
    imedit::Image accum_75775 = imedit::Image("/Users/corneria/Desktop/rev_ao/accum_75775.exr");
    imedit::Image accum_final = imedit::Image("/Users/corneria/Desktop/ao_scene/accum_131071.exr");

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_seaborn_dark(colors);
    colors.clear();
    color_map_from_image("/Users/corneria/Desktop/cmaps/" + cmap + ".png", colors);

    std::string res_path = "/Users/corneria/Desktop/cmaps/" + cmap + "/ao/";

    std::string cmd = "mkdir /Users/corneria/Desktop/cmaps/" + cmap + "/";
    system(cmd.c_str());
    cmd = "mkdir " + res_path;
    system(cmd.c_str());

    double min_val = gt.min();
    double max_val = gt.max();

    min_val = min;
    max_val = max;

    // std::cout << "min: " << min_val << std::endl;
    // std::cout << "max: " << max_val << std::endl;

    negative_positive_image_final(gt, colors, min_val, max_val, res_path + "gt_image");
    negative_positive_image_final(accum_0, colors, min_val, max_val, res_path + "accum_0_image");
    // negative_positive_image_final(accum_1, colors, min_val, max_val, res_path + "accum_1_image");
    // negative_positive_image_final(accum_3, colors, min_val, max_val, res_path + "accum_3_image");
    // negative_positive_image_final(accum_7, colors, min_val, max_val, res_path + "accum_7_image");
    negative_positive_image_final(accum_15, colors, min_val, max_val, res_path + "accum_15_image");
    // negative_positive_image_final(accum_31, colors, min_val, max_val, res_path + "accum_31_image");
    // negative_positive_image_final(accum_63, colors, min_val, max_val, res_path + "accum_63_image");
    // negative_positive_image_final(accum_127, colors, min_val, max_val, res_path + "accum_127_image");
    // negative_positive_image_final(accum_255, colors, min_val, max_val, res_path + "accum_255_image");
    // negative_positive_image_final(accum_511, colors, min_val, max_val, res_path + "accum_511_image");
    negative_positive_image_final(accum_1023, colors, min_val, max_val, res_path + "accum_1023_image");
    // negative_positive_image_final(accum_2047, colors, min_val, max_val, res_path + "accum_2047_image");
    // negative_positive_image_final(accum_4095, colors, min_val, max_val, res_path + "accum_4095_image");
    // negative_positive_image_final(accum_8191, colors, min_val, max_val, res_path + "accum_8191_image");
    negative_positive_image_final(accum_75775, colors, min_val, max_val, res_path + "accum_75775_image");
    negative_positive_image_final(accum_final, colors, min_val, max_val, res_path + "accum_131071_image");

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
            // four(j, i) = accum_75775(j, i);
            four(j, i) = accum_final(j, i);
        }
    }

    // negative_positive_image_final_wall(one, colors, imedit::Pixel(0.0), min_val, max_val, res_path + "image_one");
    // negative_positive_image_final_wall(two, colors, imedit::Pixel(0.0), min_val, max_val, res_path + "image_two");
    // negative_positive_image_final_wall(three, colors, imedit::Pixel(0.0), min_val, max_val, res_path + "image_three");
    // negative_positive_image_final_wall(four, colors, imedit::Pixel(0.0), min_val, max_val, res_path + "image_four");

    negative_positive_image_final_wall(one, colors, color, min_val, max_val, res_path + "image_one");
    negative_positive_image_final_wall(two, colors, color, min_val, max_val, res_path + "image_two");
    negative_positive_image_final_wall(three, colors, color, min_val, max_val, res_path + "image_three");
    negative_positive_image_final_wall(four, colors, color, min_val, max_val, res_path + "image_four");
}

int main(int argc, char *argv[])
{
    std::cout << "generating revision info" << std::endl;

    // find_failed_runs();
    // eval_progressive_final_camera_ready_scene();

    // make_false_color_gt();

    // make_false_color_shader_gt();

    // shader scene revision
    // make_false_color_fusion_shader_gt();
    // make_false_color_iceburn_shader_gt();

    // consistent scene revision
    // make_false_color_fusion_consist_gt();
    // make_false_color_iceburn_consist_gt();

    // ao scene revision
    // compute_ao_error();
    // make_false_color_fusion_ao_gt();
    // make_false_color_iceburn_ao_gt();

    // new version
    std::vector<std::string> cmaps = std::vector<std::string>();
    // cmaps.push_back("copper");
    // cmaps.push_back("emergency");
    // cmaps.push_back("fusion");
    // cmaps.push_back("guppy");
    // cmaps.push_back("holly");
    // cmaps.push_back("iceburn");
    // cmaps.push_back("pride");
    cmaps.push_back("prinsenvlag");
    // cmaps.push_back("redshift");
    // cmaps.push_back("seasons");
    // cmaps.push_back("seaweed");
    // cmaps.push_back("viola");
    // cmaps.push_back("waterlily");
    // cmaps.push_back("watermelon");
    // cmaps.push_back("wildfire");

    std::vector<double> shader_mins = std::vector<double>();
    std::vector<double> shader_maxs = std::vector<double>();
    for (int i = 0; i < cmaps.size(); ++i)
    {
        shader_mins.push_back(-10.0);
        shader_maxs.push_back(50.0);
    }

    std::vector<double> consist_mins = std::vector<double>();
    std::vector<double> consist_maxs = std::vector<double>();
    for (int i = 0; i < cmaps.size(); ++i)
    {
        // consist_mins.push_back(-2.5);
        // consist_maxs.push_back(2.5);
        consist_mins.push_back(-1.0);
        consist_maxs.push_back(1.0);
    }

    std::vector<double> ao_mins = std::vector<double>();
    std::vector<double> ao_maxs = std::vector<double>();
    std::vector<imedit::Pixel> ao_colors = std::vector<imedit::Pixel>();
    for (int i = 0; i < cmaps.size(); ++i)
    {
        ao_mins.push_back(-0.003);
        ao_maxs.push_back(0.10);
        if (i == 5 || i == 8 || i == 10 || i == 13 || i == 14)
            ao_colors.push_back(imedit::Pixel(1.0));
        else
            ao_colors.push_back(imedit::Pixel(0.0));
    }

    // make color mapped images
    for (int i = 0; i < cmaps.size(); ++i)
    {
        make_false_color_shader_for_cmap(cmaps[i], shader_mins[i], shader_maxs[i]);
        make_false_color_consist_for_cmap(cmaps[i], consist_mins[i], consist_maxs[i]);
        make_false_color_ao_for_cmap(cmaps[i], ao_mins[i], ao_maxs[i], ao_colors[i]);
    }

    // creates tickable colormaps
    for (int i = 0; i < cmaps.size(); ++i)
    {
        imedit::Image base = imedit::Image("/Users/corneria/Desktop/cmaps/" + cmaps[i] + ".png");

        // make thin horizontal colors
        imedit::Image horiz = imedit::Image(base.width(), 100);
        for (int i = 0; i < horiz.height(); ++i)
        {
            for (int j = 0; j < horiz.width(); ++j)
            {
                horiz(j, i) = base(j, i);
            }
        }
        horiz.write("/Users/corneria/Desktop/cmaps/paper_maps/horiz_" + cmaps[i] + ".png");

        // make thin vertical colors
        imedit::Image vert = imedit::Image(100, base.width());
        for (int i = 0; i < base.height(); ++i)
        {
            for (int j = 0; j < base.width(); ++j)
            {
                vert(i, base.width() - j - 1) = base(j, i);
            }
        }
        vert.write("/Users/corneria/Desktop/cmaps/paper_maps/vert_" + cmaps[i] + ".png");
    }

    // TODO: create python script to make color map
    for (int i = 0; i < cmaps.size(); ++i)
    {
        // call python for each cmap to generate the horizontal and vertical ticks
    }

    // find_failed_runs_consist();
    // eval_progressive_final_camera_ready_consist_scene();

    return 0;
}
