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
    std::string path_to_data = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/photon_decomp/images/";

    imedit::Image full = imedit::Image(path_to_data + "decomp_full.exr");
    imedit::Image biased = imedit::Image(path_to_data + "decomp_biased.exr");
    imedit::Image bias = imedit::Image(path_to_data + "decomp_bias.exr");

    imedit::Image neg_bias = bias;
    imedit::Image pos_bias = bias;

    for (int i = 0; i < neg_bias.height(); ++i)
    {
        for (int j = 0; j < neg_bias.width(); ++j)
        {
            if (neg_bias(j, i, 0) > 0.0)
                neg_bias(j, i, 0) = 0.0;
            else
                neg_bias(j, i, 0) = -neg_bias(j, i, 0);
            if (neg_bias(j, i, 1) > 0.0)
                neg_bias(j, i, 1) = 0.0;
            else
                neg_bias(j, i, 1) = -neg_bias(j, i, 1);
            if (neg_bias(j, i, 2) > 0.0)
                neg_bias(j, i, 2) = 0.0;
            else
                neg_bias(j, i, 2) = -neg_bias(j, i, 2);
            if (pos_bias(j, i, 0) < 0.0)
                pos_bias(j, i, 0) = 0.0;
            if (pos_bias(j, i, 1) < 0.0)
                pos_bias(j, i, 1) = 0.0;
            if (pos_bias(j, i, 2) < 0.0)
                pos_bias(j, i, 2) = 0.0;
        }
    }

    neg_bias.exposure(4.0);
    pos_bias.exposure(4.0);

    neg_bias.write(path_to_data + "neg_bias.exr");
    neg_bias.write(path_to_data + "neg_bias.png");

    pos_bias.write(path_to_data + "pos_bias.exr");
    pos_bias.write(path_to_data + "pos_bias.png");

    return 0;
}
