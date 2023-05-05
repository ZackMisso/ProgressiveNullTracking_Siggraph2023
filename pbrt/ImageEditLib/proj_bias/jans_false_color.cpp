#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

// TODO: start implementing this

// imedit::Image negative_positive_image(const imedit::Image &image,
//                                       const std::vector<imedit::Pixel> &colors,
//                                       double max_proxy,
//                                       std::string path)
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

//     return mod_image;
// }

// void eval_progressive_final_scene()
// {
//     imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_sol/image.exr");
//     imedit::Image accum = imedit::Image(400, 400);

//     std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_cluster_55/";

//     std::ofstream rmse_data;
//     rmse_data.open(path + "rmse.txt");

//     // get the color map
//     std::vector<imedit::Pixel>
//         colors = std::vector<imedit::Pixel>();
//     color_map_inferno(colors);

//     // pre-process to get min and max values for bounds on proxy
//     double min_val = 1000000.0;
//     double max_val = -1000000.0;
//     // for (int index = 0; index < 8192; index++)
//     // {
//     //     std::cout << "pre-processing index: " << index << std::endl;
//     //     imedit::Image tmp = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_prog_2/" + std::to_string(index) + "_image.exr");
//     //     double tmp_min = tmp.min();
//     //     double tmp_max = tmp.max();

//     //     if (min_val > tmp_min)
//     //         min_val = tmp_min;
//     //     if (max_val < tmp_max)
//     //         max_val = tmp_max;
//     // }

//     // also check the ground truth image
//     double tmp_min = gt.min();
//     double tmp_max = gt.max();

//     int failures = 0;

//     if (min_val > tmp_min)
//         min_val = tmp_min;
//     if (max_val < tmp_max)
//         max_val = tmp_max;

//     for (int index = 0; index < 8192 * 4; index++)
//     // for (int index = 0; index < 8192; index++)
//     {
//         try
//         {
//             std::cout << "starting index: " << index << std::endl;
//             imedit::Image tmp = imedit::Image(path + std::to_string(index) + "_image.exr");

//             accum = accum + (tmp - accum) * (1.0 / double(index + 1));

//             // compute rmse
//             double rmse_err = imedit::root_mean_sqr_error(gt, accum);

//             // create and save negative positive image
//             negative_positive_image(accum, colors, fmax(std::abs(min_val), max_val), path + std::to_string(index) + "_accum_col");

//             // save non-colored image
//             accum.write(path + std::to_string(index) + "_accum.exr");
//             // accum.write("/Users/corneria/Documents/Research/bias_research/pbrttest/results/ao/ao_final_prog_2/" + std::to_string(index) + "_accum.png");

//             rmse_data << rmse_err << std::endl;
//         }
//         catch (...)
//         {
//             failures++;
//             std::cout << index << std::endl;
//         }
//     }

//     std::cout << "FAILED RUNS: " << failures << std::endl;

//     rmse_data.close();
// }

// imedit::Image split_image(const imedit::Image &left,
//                           const imedit::Image &right,
//                           int buffer,
//                           imedit::Pixel mid_color)
// {
//     imedit::Image mix_image = imedit::Image(left.width(), left.height());

//     for (int i = 0; i < mix_image.height(); ++i)
//     {
//         for (int j = 0; j < mix_image.width(); ++j)
//         {
//             int dist = j - mix_image.width() / 2;

//             if (dist < -buffer)
//             {
//                 mix_image(j, i) = left(j, i);
//             }
//             else if (dist > buffer)
//             {
//                 mix_image(j, i) = right(j, i);
//             }
//             else
//             {
//                 mix_image(j, i) = mid_color;
//             }
//         }
//     }

//     return mix_image;
// }

// int main(int argc, char *argv[])
// {
//     std::string path_to_data = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/ao_prog/images/";

//     imedit::Image gt_ao_image = imedit::Image(path_to_data + "gt_ao_image.exr");
//     imedit::Image gt_fd_image = imedit::Image(path_to_data + "gt_fd_image.exr");

//     // get the color map
//     std::vector<imedit::Pixel>
//         colors = std::vector<imedit::Pixel>();
//     color_map_inferno(colors);

//     double min_val = gt_fd_image.min();
//     double max_val = gt_fd_image.max();

//     imedit::Image gt_fc_image = negative_positive_image(gt_fd_image, colors, fmax(std::abs(min_val), max_val), path_to_data + "gt_fd_fc_image");

//     imedit::Image res_1_image = imedit::Image(path_to_data + "3_accum_col.exr");
//     imedit::Image res_2_image = imedit::Image(path_to_data + "63_accum_col.exr");
//     imedit::Image res_3_image = imedit::Image(path_to_data + "8191_accum_col.exr");

//     imedit::Image res_1_mix = split_image(gt_fc_image, res_1_image, 1, imedit::Pixel(0.0));
//     imedit::Image res_2_mix = split_image(gt_fc_image, res_2_image, 1, imedit::Pixel(0.0));
//     imedit::Image res_3_mix = split_image(gt_fc_image, res_3_image, 1, imedit::Pixel(0.0));

//     res_1_mix.write(path_to_data + "3_accum_mix.exr");
//     res_1_mix.write(path_to_data + "3_accum_mix.png");
//     res_2_mix.write(path_to_data + "63_accum_mix.exr");
//     res_2_mix.write(path_to_data + "63_accum_mix.png");
//     res_3_mix.write(path_to_data + "8191_accum_mix.exr");
//     res_3_mix.write(path_to_data + "8191_accum_mix.png");

//     gt_ao_image.exposure(0.8);
//     gt_ao_image.write(path_to_data + "gt_ao_image_dark.exr");
//     gt_ao_image.write(path_to_data + "gt_ao_image_dark.png");

//     return 0;
// }

void create_false_color_images(std::string path,
                               const std::vector<std::string>& ests,
                               std::string post_fix,
                               int color_type)
{
    std::string cmd = "mkdir " + path + "proxies/";
    system(cmd.c_str());

    cmd = "mkdir " + path + "false_color/";
    system(cmd.c_str());

    cmd = "mkdir " + path + "color_stops/";
    system(cmd.c_str());

    std::vector<imedit::Image> base_images = std::vector<imedit::Image>();

    for (int i = 0; i < ests.size(); ++i)
    {
        imedit::Image image = imedit::Image(path + ests[i] + post_fix + ".exr");
        base_images.push_back(image);
    }

    double err_min = 10000000000.0;
    double err_max = -10000000000.0;

    // preprocess logic
    std::vector<int> histogram = std::vector<int>();
    std::vector<double> stops = std::vector<double>();

    std::vector<imedit::Pixel> colors = std::vector<imedit::Pixel>();

    if (color_type == 0)
        imedit::color_map_viridis(colors);
    else if (color_type == 1)
        imedit::color_map_magma(colors);
    else if (color_type == 2)
        imedit::color_map_inferno(colors);

    int hist_size = 0;

    // find the minimum and maximum of each image
    for (int i = 0; i < ests.size(); ++i)
    {
        imedit::Image image = imedit::Image(path + ests[i] + post_fix + ".exr");
        hist_size += image.width() * image.height();

        double min = image.min();
        double max = image.max();

        if (min < err_min) err_min = min;
        if (max > err_max) err_max = max;
    }

    histogram = std::vector<int>(100000, 0);

    for (int i = 0; i < ests.size(); ++i)
    {
        std::cout << ests[i] << std::endl;
        imedit::Image image = imedit::Image(path + ests[i] + post_fix + ".exr");
        imedit::Image proxy_image = image;

        // std::cout << "min: " << err_min << std::endl;
        // std::cout << "max: " << err_max << std::endl;

        falseColorProxy(image,
                        proxy_image,
                        err_min,
                        err_max);

        histogram_grayscale(proxy_image,
                            histogram,
                            0.0,
                            1.0);
    }

    // sanity //
    int sum = 0;
    for (int i = 0; i < histogram.size(); ++i)
    {
        sum += histogram[i];
    }
    assert(sum = hist_size);
    //////////

    // figure out the color stops
    stops.push_back(0.0);

    for (int i = 1; i < colors.size() - 1; ++i)
    {
        int accum = 0;

        for (int k = 0; k < histogram.size(); ++k)
        {
            accum += histogram[k];

            if (double(accum) / double(hist_size) >
                double(i) / double(colors.size() - 1))
            {
                stops.push_back(double(k) / double(histogram.size()));
                k = histogram.size();
            }
        }
    }

    stops.push_back(1.0);

    // TODO: finish from here

    for (int i = 0; i < ests.size(); ++i)
    {
        imedit::Image image = imedit::Image(path + ests[i] + post_fix + ".exr");
        imedit::Image fc_image = image;
        imedit::Image proxy_image = image;

        falseColor(image,
                    fc_image,
                    stops,
                    colors,
                    err_min,
                    err_max,
                    false);

        falseColorProxy(image,
                        proxy_image,
                        err_min,
                        err_max);

        imedit::Image color_map_img = imedit::Image(2048, 256);
        // color_map_image(color_map_img, colors, stops[j]);
        imedit::color_map_image(color_map_img, colors);
        std::vector<double> ticks = std::vector<double>();
        imedit::color_map_ticks(ticks, stops, 10);
        std::vector<double> tick_values = std::vector<double>();

        double logmin = -std::log(err_min + 0.0000000001);
        double logmax = -std::log(err_max + 0.0000000001);

        for (int k = 0; k < ticks.size(); ++k)
        {
            double val = (logmax - logmin) * ticks[k];
            val += logmin;
            val = exp(-val);
            tick_values.push_back(val);
        }

        // TODO: incorporate error bounds
        // ofstream err_bnds;
        // err_bnds.open(files[i] + "color_stops/map" + errType[j] + ".txt");
        // err_bnds << err_mins[j] << endl;
        // err_bnds << err_maxs[j] << endl;
        // err_bnds << ticks.size() << endl;
        // for (int k = 0; k < ticks.size(); ++k)
        // {
        //     err_bnds << ticks[k] << endl;
        // }
        // for (int k = 0; k < tick_values.size(); ++k)
        // {
        //     err_bnds << tick_values[k] << endl;
        // }
        // err_bnds.close();

        // fcImage_hdr.write(files[i] + "false_color/" + errType[j] + ".png");
        fc_image.write(path + "false_color/" + ests[i] + post_fix + ".exr");
        fc_image.write(path + "false_color/" + ests[i] + post_fix + ".png");
        fc_image.write(path + "false_color/" + ests[i] + post_fix + ".jpeg");
        proxy_image.write(path + "proxies/" + ests[i] + post_fix + ".exr");
        // proxy_hdr.write(files[i] + "../txt/" + errType[j] + ".txt");
        
        // TODO: incorporate color maps
        color_map_img.write(path + "color_stops/map_"+ ests[i] + post_fix + ".png");
        color_map_img.write(path + "color_stops/map_"+ ests[i] + post_fix +  ".jpeg");
        color_map_img.write(path + "color_stops/map_"+ ests[i] + post_fix + ".exr");
        // write_histogram_data(stops[j], files[i] + "color_stops/" + errType[j] + ".txt", true);

        // TODO: incorporate downsampling
        // Image<double> dfc2 = fcImage_hdr.downsampled_2x2();
        // Image<double> dfc = fcImage_hdr.downsampled_4x4();
        // dfc2.write(files[i] + "false_color/" + errType[j] + "_ds.jpeg");
        // dfc.write(files[i] + "false_color/" + errType[j] + "_ds4.jpeg");

        // TODO: incorporate graphing scripts
        // string command = "python ../../paper/zack/create_cononical_plots.py " + files[i] + " " + errType[j] + " " + std::to_string(min) + " " + std::to_string(max) + " 0";
        // std::cout << "command: " << command << std::endl;
        // system(command.c_str());
        // command = "python ../../paper/zack/create_cononical_plots.py " + files[i] + " " + errType[j] + " " + std::to_string(min) + " " + std::to_string(max) + " 1";
        // system(command.c_str());
        // command = "python ../../paper/zack/create_cononical_plots.py " + files[i] + " " + errType[j] + " " + std::to_string(min) + " " + std::to_string(max) + " 2";
        // system(command.c_str());
        // command = "python ../../paper/zack/create_cononical_plots.py " + files[i] + " " + errType[j] + " " + std::to_string(min) + " " + std::to_string(max) + " 3";
        // system(command.c_str());
    }
    
    std::cout << "got here so far" << std::endl;
}

void make_exp_jans(std::string path)
{
    std::vector<std::string> exp_jans_ests = std::vector<std::string>();

    exp_jans_ests.push_back("blanchet_2");
    exp_jans_ests.push_back("nvidia_exp");
    exp_jans_ests.push_back("pcmf_exp");
    exp_jans_ests.push_back("pcume_exp");
    exp_jans_ests.push_back("pratio_exp");

    create_false_color_images(path, exp_jans_ests, "_work", 0);
    create_false_color_images(path, exp_jans_ests, "_mse", 1);
    create_false_color_images(path, exp_jans_ests, "_work_norm_var", 2);
}

void make_pink_jans(std::string path)
{
    std::vector<std::string> pink_jans_ests = std::vector<std::string>();

    pink_jans_ests.push_back("blanchet_2");
    pink_jans_ests.push_back("nvidia_pink_5");
    pink_jans_ests.push_back("nvidia_pink_exp");
    pink_jans_ests.push_back("pcmf_pink_exp");
    pink_jans_ests.push_back("pcume_pink_exp");
    pink_jans_ests.push_back("pcume_pink");
    pink_jans_ests.push_back("ratio_pink");

    create_false_color_images(path, pink_jans_ests, "_work", 0);
    create_false_color_images(path, pink_jans_ests, "_mse", 1);
    create_false_color_images(path, pink_jans_ests, "_work_norm_var", 2);
}

void make_recip_jans(std::string path)
{
    std::vector<std::string> recip_jans_ests = std::vector<std::string>();

    recip_jans_ests.push_back("blanchet_2");
    recip_jans_ests.push_back("pcume_recip");
    recip_jans_ests.push_back("bernoulli_nonbin");

    create_false_color_images(path, recip_jans_ests, "_work", 0);
    create_false_color_images(path, recip_jans_ests, "_mse", 1);
    create_false_color_images(path, recip_jans_ests, "_work_norm_var", 2);
}

int main(int argc, char *argv[])
{
    std::cout << "creating false color images" << std::endl;

    std::string base_path = "/Users/corneria/Documents/Research/bias_research/ConvergenceRater/results/";
    // make_exp_jans(base_path + "exp_jans/gauss_all/");
    // make_exp_jans(base_path + "exp_jans/const_all/");
    // make_exp_jans(base_path + "exp_jans/noise_all/");
    // make_exp_jans(base_path + "exp_jans/scos_all/");

    // make_pink_jans(base_path + "pink_jans/3/gauss_all/");
    // make_pink_jans(base_path + "pink_jans/3/const_all/");
    // make_pink_jans(base_path + "pink_jans/3/noise_all/");
    // make_pink_jans(base_path + "pink_jans/3/scos_all/");

    // make_pink_jans(base_path + "pink_jans/5/gauss_all/");
    // make_pink_jans(base_path + "pink_jans/5/const_all/");
    // make_pink_jans(base_path + "pink_jans/5/noise_all/");
    // make_pink_jans(base_path + "pink_jans/5/scos_all/");

    make_recip_jans(base_path + "recip_figure/gauss_all/");
    make_recip_jans(base_path + "recip_figure/const_all/");
    make_recip_jans(base_path + "recip_figure/binar_all/");
    make_recip_jans(base_path + "recip_figure/scos_all/");


    // std::string path_to_data = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/ao_prog/images/";

    // imedit::Image gt_ao_image = imedit::Image(path_to_data + "gt_ao_image.exr");
    // imedit::Image gt_fd_image = imedit::Image(path_to_data + "gt_fd_image.exr");

    // // get the color map
    // std::vector<imedit::Pixel>
    //     colors = std::vector<imedit::Pixel>();
    // color_map_inferno(colors);

    // double min_val = gt_fd_image.min();
    // double max_val = gt_fd_image.max();

    // imedit::Image gt_fc_image = negative_positive_image(gt_fd_image, colors, fmax(std::abs(min_val), max_val), path_to_data + "gt_fd_fc_image");

    // imedit::Image res_1_image = imedit::Image(path_to_data + "3_accum_col.exr");
    // imedit::Image res_2_image = imedit::Image(path_to_data + "63_accum_col.exr");
    // imedit::Image res_3_image = imedit::Image(path_to_data + "8191_accum_col.exr");

    // imedit::Image res_1_mix = split_image(gt_fc_image, res_1_image, 1, imedit::Pixel(0.0));
    // imedit::Image res_2_mix = split_image(gt_fc_image, res_2_image, 1, imedit::Pixel(0.0));
    // imedit::Image res_3_mix = split_image(gt_fc_image, res_3_image, 1, imedit::Pixel(0.0));

    // res_1_mix.write(path_to_data + "3_accum_mix.exr");
    // res_1_mix.write(path_to_data + "3_accum_mix.png");
    // res_2_mix.write(path_to_data + "63_accum_mix.exr");
    // res_2_mix.write(path_to_data + "63_accum_mix.png");
    // res_3_mix.write(path_to_data + "8191_accum_mix.exr");
    // res_3_mix.write(path_to_data + "8191_accum_mix.png");

    // gt_ao_image.exposure(0.8);
    // gt_ao_image.write(path_to_data + "gt_ao_image_dark.exr");
    // gt_ao_image.write(path_to_data + "gt_ao_image_dark.png");

    // return 0;
}
