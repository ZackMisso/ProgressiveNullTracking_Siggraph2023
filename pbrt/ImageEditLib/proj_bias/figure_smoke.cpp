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

void split_image(const imedit::Image &left,
                 const imedit::Image &right,
                 std::string path)
{
    imedit::Image mix_image = imedit::Image(left.width(), left.height());

    int buffer = 2;
    imedit::Pixel mid_color = imedit::Pixel(1.0);

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

    mix_image.write(path + "comb_gt.exr");
    mix_image.write(path + "comb_gt.png");
}

void split_image_horizontal(const imedit::Image &bot,
                            const imedit::Image &top,
                            std::string path)
{
    imedit::Image mix_image = imedit::Image(top.width(), top.height());

    int buffer = 0;
    imedit::Pixel mid_color = imedit::Pixel(1.0);

    for (int i = 0; i < mix_image.height(); ++i)
    {
        for (int j = 0; j < mix_image.width(); ++j)
        {
            int dist = i - mix_image.height() / 2;

            if (dist < 0)
            {
                mix_image(j, i) = bot(j, i);
            }
            else
            {
                mix_image(j, i) = top(j, i);
            }
            // else
            // {
            //     mix_image(j, i) = mid_color;
            // }
        }
    }

    mix_image.write(path + ".exr");
    mix_image.write(path + ".png");
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
    std::string path_to_data = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/trans_pink_smoke/images/";

    imedit::Image gt_3 = imedit::Image(path_to_data + "3/gt.exr");
    imedit::Image gt_8 = imedit::Image(path_to_data + "8/gt.exr");
    imedit::Image gt_exp = imedit::Image(path_to_data + "exp/gt.exr");

    imedit::Image pink_cmf_exp_3 = imedit::Image(path_to_data + "3/pink_cmf_exp.exr");
    imedit::Image pink_pcume_exp_3 = imedit::Image(path_to_data + "3/pink_pcume_exp.exr");
    imedit::Image pink_pcume_3 = imedit::Image(path_to_data + "3/pink_pcume.exr");
    imedit::Image pink_ratio_3 = imedit::Image(path_to_data + "3/pink_ratio.exr");
    imedit::Image pink_tay_ray_exp_3 = imedit::Image(path_to_data + "3/pink_tay_ray_exp.exr");
    imedit::Image pink_tay_3 = imedit::Image(path_to_data + "3/pink_tay.exr");
    imedit::Image pink_tele_3 = imedit::Image(path_to_data + "3/tele.exr");

    double pink_cmf_exp_3_mse = root_mean_sqr_error(gt_3, pink_cmf_exp_3);
    double pink_pcume_exp_3_mse = root_mean_sqr_error(gt_3, pink_pcume_exp_3);
    double pink_pcume_3_mse = root_mean_sqr_error(gt_3, pink_pcume_3);
    double pink_ratio_3_mse = root_mean_sqr_error(gt_3, pink_ratio_3);
    double pink_tay_ray_exp_3_mse = root_mean_sqr_error(gt_3, pink_tay_ray_exp_3);
    double pink_tay_3_mse = root_mean_sqr_error(gt_3, pink_tay_3);
    double pink_tele_3_mse = root_mean_sqr_error(gt_3, pink_tele_3);

    std::cout << "MSE for c = 0.3: " << std::endl;
    std::cout << "pink cmf exp: " << pink_cmf_exp_3_mse << std::endl;
    std::cout << "pink pcume exp: " << pink_pcume_exp_3_mse << std::endl;
    std::cout << "pink pcume: " << pink_pcume_3_mse << std::endl;
    std::cout << "pink ratio: " << pink_ratio_3_mse << std::endl;
    std::cout << "pink tay exp: " << pink_tay_ray_exp_3_mse << std::endl;
    std::cout << "pink tay: " << pink_tay_3_mse << std::endl;
    std::cout << "tele: " << pink_tele_3_mse << std::endl;
    std::cout << std::endl;

    imedit::Image pink_cmf_exp_8 = imedit::Image(path_to_data + "8/pink_cmf_exp.exr");
    imedit::Image pink_pcume_exp_8 = imedit::Image(path_to_data + "8/pink_pcume_exp.exr");
    imedit::Image pink_pcume_8 = imedit::Image(path_to_data + "8/pink_pcume.exr");
    imedit::Image pink_ratio_8 = imedit::Image(path_to_data + "8/pink_ratio.exr");
    imedit::Image pink_tay_ray_exp_8 = imedit::Image(path_to_data + "8/pink_tay_ray_exp.exr");
    imedit::Image pink_tay_8 = imedit::Image(path_to_data + "8/pink_tay.exr");
    imedit::Image pink_tele_8 = imedit::Image(path_to_data + "8/tele.exr");

    double pink_cmf_exp_8_mse = root_mean_sqr_error(gt_8, pink_cmf_exp_8);
    double pink_pcume_exp_8_mse = root_mean_sqr_error(gt_8, pink_pcume_exp_8);
    double pink_pcume_8_mse = root_mean_sqr_error(gt_8, pink_pcume_8);
    double pink_ratio_8_mse = root_mean_sqr_error(gt_8, pink_ratio_8);
    double pink_tay_ray_exp_8_mse = root_mean_sqr_error(gt_8, pink_tay_ray_exp_8);
    double pink_tay_8_mse = root_mean_sqr_error(gt_8, pink_tay_8);
    double pink_tele_8_mse = root_mean_sqr_error(gt_8, pink_tele_8);

    std::cout << "RMSE for c = 0.8: " << std::endl;
    std::cout << "pink cmf exp: " << pink_cmf_exp_8_mse << std::endl;
    std::cout << "pink pcume exp: " << pink_pcume_exp_8_mse << std::endl;
    std::cout << "pink pcume: " << pink_pcume_8_mse << std::endl;
    std::cout << "pink ratio: " << pink_ratio_8_mse << std::endl;
    std::cout << "pink tay exp: " << pink_tay_ray_exp_8_mse << std::endl;
    std::cout << "pink tay: " << pink_tay_8_mse << std::endl;
    std::cout << "tele: " << pink_tele_8_mse << std::endl;
    std::cout << std::endl;

    imedit::Image cmf_exp = imedit::Image(path_to_data + "exp/pseries_cdf.exr");
    imedit::Image pcume_exp = imedit::Image(path_to_data + "exp/pseries_cumulative.exr");
    imedit::Image ratio_exp = imedit::Image(path_to_data + "exp/ratio.exr");
    imedit::Image nvidia_exp = imedit::Image(path_to_data + "exp/exp_nvidia.exr");
    imedit::Image tele_exp = imedit::Image(path_to_data + "exp/tele.exr");

    double cmf_exp_mse = root_mean_sqr_error(gt_exp, cmf_exp);
    double pcume_exp_mse = root_mean_sqr_error(gt_exp, pcume_exp);
    double ratio_exp_mse = root_mean_sqr_error(gt_exp, ratio_exp);
    double nvidia_exp_mse = root_mean_sqr_error(gt_exp, nvidia_exp);
    double tele_exp_mse = root_mean_sqr_error(gt_exp, tele_exp);

    std::cout << "RMSE for exponential: " << std::endl;
    std::cout << "cmf exp: " << cmf_exp_mse << std::endl;
    std::cout << "pcume exp: " << pcume_exp_mse << std::endl;
    std::cout << "ratio: " << ratio_exp_mse << std::endl;
    std::cout << "nvidia: " << nvidia_exp_mse << std::endl;
    std::cout << "tele: " << tele_exp_mse << std::endl;
    std::cout << std::endl;

    // now to create the insets
    create_3_inset(gt_3, path_to_data, "gt_ref");
    create_3_inset(pink_cmf_exp_3, path_to_data, "pink_cmf_exp");
    create_3_inset(pink_pcume_exp_3, path_to_data, "pink_pcume_exp");
    create_3_inset(pink_pcume_3, path_to_data, "pink_pcume");
    create_3_inset(pink_ratio_3, path_to_data, "pink_ratio");
    create_3_inset(pink_tay_ray_exp_3, path_to_data, "pink_nvidia");
    create_3_inset(pink_tay_3, path_to_data, "pink_tay");
    create_3_inset(pink_tele_3, path_to_data, "pink_tele");

    split_image(gt_3, gt_8, path_to_data);

    create_8_inset(gt_8, path_to_data, "gt_ref");
    create_8_inset(pink_cmf_exp_8, path_to_data, "pink_cmf_exp");
    create_8_inset(pink_pcume_exp_8, path_to_data, "pink_pcume_exp");
    create_8_inset(pink_pcume_8, path_to_data, "pink_pcume");
    create_8_inset(pink_ratio_8, path_to_data, "pink_ratio");
    create_8_inset(pink_tay_ray_exp_8, path_to_data, "pink_nvidia");
    create_8_inset(pink_tay_8, path_to_data, "pink_tay");
    create_8_inset(pink_tele_8, path_to_data, "pink_tele");

    create_8_inset(gt_exp, path_to_data, "exp_gt_ref");
    create_8_inset(cmf_exp, path_to_data, "exp_cmf");
    create_8_inset(pcume_exp, path_to_data, "exp_pcume");
    create_8_inset(ratio_exp, path_to_data, "exp_ratio");
    create_8_inset(nvidia_exp, path_to_data, "exp_nvidia");
    create_8_inset(tele_exp, path_to_data, "exp_tele");

    create_3_inset(gt_exp, path_to_data, "exp_gt_ref");
    create_3_inset(cmf_exp, path_to_data, "exp_cmf");
    create_3_inset(pcume_exp, path_to_data, "exp_pcume");
    create_3_inset(ratio_exp, path_to_data, "exp_ratio");
    create_3_inset(nvidia_exp, path_to_data, "exp_nvidia");
    create_3_inset(tele_exp, path_to_data, "exp_tele");

    // std::cout << "pink 3" << std::endl;
    imedit::Image tele_3 = imedit::Image(path_to_data + "3/pink_tele_inset.exr");
    imedit::Image nvidia_3 = imedit::Image(path_to_data + "3/pink_nvidia_inset.exr");
    imedit::Image ratio_3 = imedit::Image(path_to_data + "3/pink_ratio_inset.exr");

    imedit::Image tele_8 = imedit::Image(path_to_data + "8/pink_tele_inset.exr");
    imedit::Image nvidia_8 = imedit::Image(path_to_data + "8/pink_nvidia_inset.exr");
    imedit::Image ratio_8 = imedit::Image(path_to_data + "8/pink_ratio_inset.exr");

    imedit::Image tele_exp_3 = imedit::Image(path_to_data + "3/exp_tele_inset.exr");
    imedit::Image nvidia_exp_3 = imedit::Image(path_to_data + "3/exp_nvidia_inset.exr");
    imedit::Image ratio_exp_3 = imedit::Image(path_to_data + "3/exp_ratio_inset.exr");

    imedit::Image tele_exp_8 = imedit::Image(path_to_data + "8/exp_tele_inset.exr");
    imedit::Image nvidia_exp_8 = imedit::Image(path_to_data + "8/exp_nvidia_inset.exr");
    imedit::Image ratio_exp_8 = imedit::Image(path_to_data + "8/exp_ratio_inset.exr");

    split_image_horizontal(tele_exp_3, tele_3, path_to_data + "3/tele_final_inset");
    split_image_horizontal(nvidia_exp_3, nvidia_3, path_to_data + "3/nvidia_final_inset");
    split_image_horizontal(ratio_exp_3, ratio_3, path_to_data + "3/ratio_final_inset");

    split_image_horizontal(tele_exp_8, tele_8, path_to_data + "8/tele_final_inset");
    split_image_horizontal(nvidia_exp_8, nvidia_8, path_to_data + "8/nvidia_final_inset");
    split_image_horizontal(ratio_exp_8, ratio_8, path_to_data + "8/ratio_final_inset");

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
