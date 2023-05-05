#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

imedit::Image negative_positive_image(const imedit::Image &image,
                                      const std::vector<imedit::Pixel> &colors,
                                      double max_proxy,
                                      std::string path)
{
    imedit::Image mod_image = image;
    double neg_amp = 20.0;

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = 0; j < mod_image.width(); ++j)
        {
            double proxy = mod_image(j, i, 0);
            // proxy = (proxy + max_proxy) / (2.0 * max_proxy); // range [-max_proxy, max_proxy]
            // imedit::Pixel color = colors[0] * proxy + colors[colors.size() - 1] * (1.0 - proxy);
            // mod_image(j, i) = color;

            // if (proxy < 0.0)
            //     mod_image(j, i) = mod_image(j, i) * imedit::Pixel(0.0, 0.0, -1.0);
            // else
            //     mod_image(j, i) = mod_image(j, i) * imedit::Pixel(1.0, 0.0, 0.0);

            if (proxy < 0.0)
                mod_image(j, i) = mod_image(j, i) * imedit::Pixel(neg_amp * -3.22609671e-01 * 0.5, neg_amp * -5.56335178e-02 * 0.5, neg_amp * -4.26376869e-01 * 0.5);
            else
                mod_image(j, i) = mod_image(j, i) * imedit::Pixel(9.68040817e-01, 8.28515491e-01, 2.49971582e-01);

            // Pixel(3.22609671e-01, 5.56335178e-02, 4.26376869e-01)
            //     Pixel(9.68040817e-01, 8.28515491e-01, 2.49971582e-01)
        }
    }

    double scale = 100.0;
    mod_image.exposure(scale);

    // std::cout << "writing modded image" << std::endl;

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");

    return mod_image;
}

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

imedit::Image split_image(const imedit::Image &left,
                          const imedit::Image &right,
                          int buffer,
                          imedit::Pixel mid_color)
{
    imedit::Image mix_image = imedit::Image(left.width(), left.height());

    for (int i = 0; i < mix_image.height(); ++i)
    {
        for (int j = 0; j < mix_image.width(); ++j)
        {
            int dist = j - mix_image.width() / 2;

            if (dist < -buffer)
            {
                mix_image(j, i) = left(j, i);
            }
            else if (dist > buffer)
            {
                mix_image(j, i) = right(j, i);
            }
            else
            {
                mix_image(j, i) = mid_color;
            }
        }
    }

    return mix_image;
}

void create_3_inset(const imedit::Image &input, std::string path, std::string name)
{
    imedit::Image inset = imedit::Image(54, 66);

    int start_x = 330;
    int start_y = 504;
    int end_x = start_x + inset.width();
    int end_y = start_y + inset.height();

    for (int i = start_y; i < end_y; ++i)
    {
        for (int j = start_x; j < end_x; ++j)
        {
            inset(j - start_x, i - start_y) = input(j, i);
        }
    }

    inset = double_sized_image(inset);
    inset = double_sized_image(inset);

    inset.write(path + "3/" + name + "_inset.exr");
    inset.write(path + "3/" + name + "_inset.png");
}

void create_8_inset(const imedit::Image &input, std::string path, std::string name)
{
    imedit::Image inset = imedit::Image(54, 66);

    int start_x = 590;
    int start_y = 1030;
    int end_x = start_x + inset.width();
    int end_y = start_y + inset.height();

    for (int i = start_y; i < end_y; ++i)
    {
        for (int j = start_x; j < end_x; ++j)
        {
            inset(j - start_x, i - start_y) = input(j, i);
        }
    }

    inset = double_sized_image(inset);
    inset = double_sized_image(inset);

    inset.write(path + "8/" + name + "_inset.exr");
    inset.write(path + "8/" + name + "_inset.png");
}

void create_combined_image(const imedit::Image &top, const imedit::Image &bot, std::string path)
{
    imedit::Image comb = imedit::Image(top.width(), top.height());

    // std::cout << "in here" << std::endl;

    for (int i = 0; i < comb.height(); ++i)
    {
        for (int j = 0; j < comb.width(); ++j)
        {
            double mid_i = double(comb.width() / 2 - j) * 0.5 + double(comb.height() / 2);

            double dist_to_mid_i = std::abs(double(i) - mid_i);
            if (dist_to_mid_i < 2)
                comb(j, i) = imedit::Pixel(1.0);
            else if (mid_i < i)
                comb(j, i) = top(j, i);
            else
                comb(j, i) = bot(j, i);
        }
    }

    comb.write(path + "comb_gt.exr");
    comb.write(path + "comb_gt.png");
}

int main(int argc, char *argv[])
{
    std::string path_to_data = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/trans_bunny/images/";

    imedit::Image gt = imedit::Image(path_to_data + "gt.exr");

    imedit::Image cmf = imedit::Image(path_to_data + "pseries_cdf.exr");
    imedit::Image pcume = imedit::Image(path_to_data + "pseries_cumulative.exr");
    imedit::Image ratio = imedit::Image(path_to_data + "ratio.exr");
    imedit::Image nvidia = imedit::Image(path_to_data + "exp_nvidia.exr");
    imedit::Image tele = imedit::Image(path_to_data + "tele.exr");

    double cmf_mse = root_mean_sqr_error(gt, cmf);
    double pcume_mse = root_mean_sqr_error(gt, pcume);
    double ratio_mse = root_mean_sqr_error(gt, ratio);
    double nvidia_mse = root_mean_sqr_error(gt, nvidia);
    double tele_mse = root_mean_sqr_error(gt, tele);

    std::cout << "RMSE" << std::endl;
    std::cout << "pcmf: " << cmf_mse << std::endl;
    std::cout << "pcume: " << pcume_mse << std::endl;
    std::cout << "ratio: " << ratio_mse << std::endl;
    std::cout << "nvidia: " << nvidia_mse << std::endl;
    std::cout << "tele: " << tele_mse << std::endl;
    std::cout << std::endl;

    imedit::Image final_gt = imedit::Image(gt.width() + 600, gt.height() - 200);

    for (int i = 0; i < final_gt.height(); ++i)
    {
        for (int j = 0; j < final_gt.width(); ++j)
        {
            int orig_i = i + 100;
            int orig_j = j - 530;

            bool in = false;

            if (orig_j >= 0 && orig_j < gt.width())
            {
                if (orig_i >= 0 && orig_i < gt.height())
                {
                    final_gt(j, i) = gt(orig_j, orig_i);
                    in = true;
                }
            }

            if (!in)
            {
                final_gt(j, i) = imedit::Pixel(0.0);
            }
        }
    }

    final_gt.write(path_to_data + "final_im.exr");
    final_gt.write(path_to_data + "final_im.png");

    // now to create the insets
    // create_3_inset(gt_3, path_to_data, "gt_ref");
    // create_3_inset(pink_cmf_exp_3, path_to_data, "pink_cmf_exp");
    // create_3_inset(pink_pcume_exp_3, path_to_data, "pink_pcume_exp");
    // create_3_inset(pink_pcume_3, path_to_data, "pink_pcume");
    // create_3_inset(pink_ratio_3, path_to_data, "pink_ratio");
    // create_3_inset(pink_tay_ray_exp_3, path_to_data, "pink_nvidia");
    // create_3_inset(pink_tay_3, path_to_data, "pink_tay");
    // create_3_inset(pink_tele_3, path_to_data, "pink_tele");

    // create_combined_image(gt_8, gt_3, path_to_data);

    // create_8_inset(gt_8, path_to_data, "gt_ref");
    // create_8_inset(pink_cmf_exp_8, path_to_data, "pink_cmf_exp");
    // create_8_inset(pink_pcume_exp_8, path_to_data, "pink_pcume_exp");
    // create_8_inset(pink_pcume_8, path_to_data, "pink_pcume");
    // create_8_inset(pink_ratio_8, path_to_data, "pink_ratio");
    // create_8_inset(pink_tay_ray_exp_8, path_to_data, "pink_nvidia");
    // create_8_inset(pink_tay_8, path_to_data, "pink_tay");
    // create_8_inset(pink_tele_8, path_to_data, "pink_tele");

    // imedit::Image deb_image = imedit::Image(path_to_data + "deb.exr");
    // imedit::Image pm_image = imedit::Image(path_to_data + "pm.exr");

    // imedit::Image deb_inset_image = imedit::Image(60, 90);
    // imedit::Image pm_inset_image = imedit::Image(60, 90);

    // int start_j = 330;
    // int end_j = start_j + deb_inset_image.width();
    // int start_i = 780;
    // int end_i = start_i + deb_inset_image.height();

    // for (int i = start_i; i < end_i; i++)
    // {
    //     for (int j = start_j; j < end_j; j++)
    //     {
    //         deb_inset_image(j - start_j, i - start_i) = deb_image(j, i);
    //         pm_inset_image(j - start_j, i - start_i) = pm_image(j, i);
    //     }
    // }

    // // increase size to improve visibility
    // deb_inset_image = double_sized_image(deb_inset_image);
    // deb_inset_image = double_sized_image(deb_inset_image);

    // // increase size to improve visibility
    // pm_inset_image = double_sized_image(pm_inset_image);
    // pm_inset_image = double_sized_image(pm_inset_image);

    // deb_inset_image.write(path_to_data + "deb_inset.exr");
    // deb_inset_image.write(path_to_data + "deb_inset.png");
    // pm_inset_image.write(path_to_data + "pm_inset.exr");
    // pm_inset_image.write(path_to_data + "pm_inset.png");

    return 0;
}
