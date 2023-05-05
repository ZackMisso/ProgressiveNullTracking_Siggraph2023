#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

imedit::Image negative_positive_image(const imedit::Image &image,
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

    return mod_image;
}

imedit::Image negative_positive_image_blue_red(const imedit::Image &image,
                                               const std::vector<imedit::Pixel> &colors,
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
        }
    }

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");

    return mod_image;
}

imedit::Image negative_positive_image_coolwarm_one(const imedit::Image &image,
                                                   const std::vector<imedit::Pixel> &colors,
                                                   double min_val,
                                                   double max_val,
                                                   std::string path)
{
    imedit::Image mod_image = image;

    imedit::Pixel neg_color = imedit::Pixel(0.230, 0.299, 0.754);
    imedit::Pixel pos_color = imedit::Pixel(0.706, 0.016, 0.150);
    imedit::Pixel neu_color = imedit::Pixel(0.865, 0.865, 0.865);

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = 0; j < mod_image.width(); ++j)
        {
            double proxy = mod_image(j, i, 0);

            if (proxy < 0.0)
            {
                // std::cout << "proxy:" << proxy << std::endl;
                proxy = exp(-proxy / min_val);

                // proxy /= min_val;
                // proxy = exp(proxy) - 1.0;
                // std::cout << "proxy: " << proxy << std::endl;
                if (proxy > 1.0)
                    proxy = 1.0;

                mod_image(j, i) = neg_color * proxy + neu_color * (1.0 - proxy);
            }
            else
            {
                proxy = exp(proxy / max_val);
                // proxy /= max_val;
                // proxy = exp(proxy) - 1.0;
                if (proxy > 1.0)
                    proxy = 1.0;
                mod_image(j, i) = pos_color * proxy + neu_color * (1.0 - proxy);
            }
        }
    }

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");

    return mod_image;
}

imedit::Image negative_positive_image_coolwarm_two(const imedit::Image &image,
                                                   const std::vector<imedit::Pixel> &wat_colors,
                                                   double min_val,
                                                   double max_val,
                                                   std::string path)
{
    imedit::Image mod_image = image;

    // imedit::Pixel neg_color = imedit::Pixel(0.230, 0.299, 0.754);
    // imedit::Pixel pos_color = imedit::Pixel(0.706, 0.016, 0.150);
    // imedit::Pixel neu_color = imedit::Pixel(0.865, 0.865, 0.865);

    std::vector<imedit::Pixel> colors = std::vector<imedit::Pixel>();

    // imedit::color_map_seaborn_dark(colors);
    colors.push_back(imedit::Pixel(0.230, 0.299, 0.754));
    colors.push_back(imedit::Pixel(0.0, 0.0, 0.0));
    colors.push_back(imedit::Pixel(0.706, 0.016, 0.150));

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = 0; j < mod_image.width(); ++j)
        {
            double proxy = mod_image(j, i, 0);
            bool flip = false;

            if (proxy < 0.0)
            {
                proxy = (1.0 - proxy / min_val) * 0.5;
            }
            else
            {
                proxy = proxy / max_val * 0.5 + 0.5;
                flip = true;
                // if (proxy == 0.5)
                // {
                //     std::cout << "here" << std::endl;
                //     std::cout << proxy * double(colors.size() - 1) << std::endl;
                //     std::cout << floor(proxy * double(colors.size() - 1)) << std::endl;
                // }
            }

            if (proxy < 0.0)
                proxy = 0.0;
            if (proxy > 1.0)
                proxy = 1.0;

            double location = proxy * double(colors.size() - 1);
            double diff = location - floor(location);
            int index_one = floor(location);
            int index_two = index_one + 1;
            // if (flip || true)
            // {
            //     diff = ceil(location) - location;
            // }
            // float diff = ceil(location) - location;
            if (index_two > colors.size() - 1)
                index_two = index_one;

            imedit::Pixel color_one = colors[index_one];
            imedit::Pixel color_two = colors[index_two];

            // mod_image(j, i) = imedit::Pixel(proxy) *
            //                   1.0;

            // if (diff == 0.0)
            // {
            //     // std::cout << "index: " << index_one << std::endl;
            //     // std::cout << "loc: " << location << std::endl;
            //     mod_image(j, i) = imedit::Pixel(color_one.r,
            //                                     color_one.g,
            //                                     color_one.b) *
            //                       1.0;
            // }
            // else
            // {

            mod_image(j, i) = imedit::Pixel(color_one.r * (1.f - diff) + color_two.r * diff,
                                            color_one.g * (1.f - diff) + color_two.g * diff,
                                            color_one.b * (1.f - diff) + color_two.b * diff) *
                              1.0;
            // }
        }
    }

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");

    imedit::Image col_imag = imedit::Image(512, 64);
    imedit::color_map_image(col_imag, colors);
    col_imag.write(path + "_color_map.exr");
    col_imag.write(path + "_color_map.png");

    return mod_image;
}

imedit::Image negative_positive_image_coolwarm_three(const imedit::Image &image,
                                                     const std::vector<imedit::Pixel> &wat_colors,
                                                     double min_val,
                                                     double max_val,
                                                     std::string path)
{
    imedit::Image mod_image = image;
    std::vector<imedit::Pixel> colors = std::vector<imedit::Pixel>();

    // min_val = min_val / 3.0;
    // max_val = max_val / 3.0;

    // colors.push_back(imedit::Pixel(0.706, 0.016, 0.150));
    // colors.push_back(imedit::Pixel(0.0, 0.0, 0.0));
    // colors.push_back(imedit::Pixel(0.230, 0.754, 0.299));

    // colors.push_back(imedit::Pixel(0.07456216517419136, 0.2102029734738408, 0.7186691355093193));
    // // colors.push_back(imedit::Pixel(0.018553924323881373, 0.046619591313979344, 0.1449108593697092));
    // colors.push_back(imedit::Pixel(0.0, 0.0, 0.0));
    // // colors.push_back(imedit::Pixel(0.0768602508264685, 0.01316048979969254, 0.011611692287637265));
    // colors.push_back(imedit::Pixel(0.7629384194757707, 0.10785370061495982, 0.09318612182878613));

    // double scale = 15.0;

    // colors.push_back(imedit::Pixel(0.07456216517419136, 0.2102029734738408, 0.7186691355093193) * scale);
    // colors.push_back(imedit::Pixel(0.049377252292421306, 0.13512620986969753, 0.45098018617053065) * scale);
    // colors.push_back(imedit::Pixel(0.031185422066276638, 0.08222025569956037, 0.26611146568698685) * scale);
    // colors.push_back(imedit::Pixel(0.018553924323881373, 0.046619591313979344, 0.1449108593697092) * scale);
    // colors.push_back(imedit::Pixel(0.010023155500973725, 0.02355922317698572, 0.0691346836256632) * scale);
    // colors.push_back(imedit::Pixel(0.004950187511246355, 0.010612644931521711, 0.028679998418209028) * scale);
    // colors.push_back(imedit::Pixel(0.002100778720348246, 0.003915583905719169, 0.009268098849464964) * scale);
    // colors.push_back(imedit::Pixel(0.0007046774666555331, 0.0010343262825853756, 0.001909038736573322) * scale);
    // colors.push_back(imedit::Pixel(0.00246027503388696, 0.0007269890791601767, 0.0006770107517254481) * scale);
    // colors.push_back(imedit::Pixel(0.011018109461309496, 0.002445129298006709, 0.0022156748901371055) * scale);
    // colors.push_back(imedit::Pixel(0.032705179550125946, 0.006193752385133489, 0.005523016489336662) * scale);
    // colors.push_back(imedit::Pixel(0.0768602508264685, 0.01316048979969254, 0.011611692287637265) * scale);
    // colors.push_back(imedit::Pixel(0.15830420621172656, 0.025236530463523466, 0.02209599779566718) * scale);
    // colors.push_back(imedit::Pixel(0.2871799140835518, 0.04351829397725358, 0.037891855898273964) * scale);
    // colors.push_back(imedit::Pixel(0.48222096537478665, 0.07029249200567932, 0.06094178682894862) * scale);
    // colors.push_back(imedit::Pixel(0.7629384194757707, 0.10785370061495982, 0.09318612182878613) * scale);

    imedit::color_map_seaborn_dark(colors);

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = 0; j < mod_image.width(); ++j)
        {
            double proxy = mod_image(j, i, 0);
            bool flip = false;

            if (proxy < 0.0)
            {
                // proxy = -exp(proxy / min_val - 1.0);
                proxy = (1.0 - proxy / min_val) * 0.5;
            }
            else
            {
                // proxy = exp(proxy / max_val - 1.0);
                // std::cout << "proxy: " << proxy << std::endl;
                proxy = proxy / max_val * 0.5 + 0.5;

                flip = true;
            }

            if (proxy < 0.0)
                proxy = 0.0;
            if (proxy > 1.0)
                proxy = 1.0;

            double location = proxy * double(colors.size() - 1);
            double diff = location - floor(location);
            int index_one = floor(location);
            int index_two = index_one + 1;

            if (index_two > colors.size() - 1)
                index_two = index_one;

            imedit::Pixel color_one = colors[index_one];
            imedit::Pixel color_two = colors[index_two];

            mod_image(j, i) = imedit::Pixel(color_one.r * (1.f - diff) + color_two.r * diff,
                                            color_one.g * (1.f - diff) + color_two.g * diff,
                                            color_one.b * (1.f - diff) + color_two.b * diff) *
                              1.0;
        }
    }

    mod_image.write(path + ".exr");
    mod_image.write(path + ".png");

    imedit::Image col_imag = imedit::Image(512, 64);
    imedit::color_map_image(col_imag, colors);
    col_imag.write(path + "_color_map.exr");
    col_imag.write(path + "_color_map.png");

    return mod_image;
}

imedit::Image negative_positive_image_coolwarm_dark(const imedit::Image &image,
                                                    const std::vector<imedit::Pixel> &colors,
                                                    double min_val,
                                                    double max_val,
                                                    std::string path)
{
    imedit::Image mod_image = image;

    imedit::Pixel neg_color = imedit::Pixel(0.230, 0.299, 0.754) * 1.4;
    imedit::Pixel pos_color = imedit::Pixel(0.706, 0.016, 0.150) * 1.4;
    imedit::Pixel neu_color = imedit::Pixel(0.0, 0.0, 0.0);

    for (int i = 0; i < mod_image.height(); ++i)
    {
        for (int j = 0; j < mod_image.width(); ++j)
        {
            double proxy = mod_image(j, i, 0);

            if (proxy < 0.0)
            {
                proxy /= min_val;

                mod_image(j, i) = neg_color * proxy + neu_color * (1.0 - proxy);
            }
            else
            {
                proxy /= max_val;
                mod_image(j, i) = pos_color * proxy + neu_color * (1.0 - proxy);
            }
        }
    }

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

int main(int argc, char *argv[])
{
    std::string path_to_data = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/ao_prog/images/";

    imedit::Image gt_ao_image = imedit::Image(path_to_data + "gt_ao_image.exr");
    imedit::Image gt_fd_image = imedit::Image(path_to_data + "gt_fd_image.exr");

    // min: -0.0167694
    // max: 0.150146

    // get the color map
    std::vector<imedit::Pixel>
        colors = std::vector<imedit::Pixel>();
    color_map_inferno(colors);

    // double min_val = -0.0167694;
    // double max_val = 0.150146;

    double min_val = gt_fd_image.min();
    double max_val = gt_fd_image.max();

    imedit::Image gt_fc_image = negative_positive_image(gt_fd_image, min_val, max_val, path_to_data + "gt_fd_fc_image");

    imedit::Image res_1_image = imedit::Image(path_to_data + "3_accum_col.exr");
    imedit::Image res_2_image = imedit::Image(path_to_data + "63_accum_col.exr");
    imedit::Image res_3_image = imedit::Image(path_to_data + "8191_accum_col.exr");

    imedit::Image res_1_mix = split_image(gt_fc_image, res_1_image, 1, imedit::Pixel(0.0));
    imedit::Image res_2_mix = split_image(gt_fc_image, res_2_image, 1, imedit::Pixel(0.0));
    imedit::Image res_3_mix = split_image(gt_fc_image, res_3_image, 1, imedit::Pixel(0.0));

    res_1_mix.write(path_to_data + "3_accum_mix.exr");
    res_1_mix.write(path_to_data + "3_accum_mix.png");
    res_2_mix.write(path_to_data + "63_accum_mix.exr");
    res_2_mix.write(path_to_data + "63_accum_mix.png");
    res_3_mix.write(path_to_data + "8191_accum_mix.exr");
    res_3_mix.write(path_to_data + "8191_accum_mix.png");

    gt_ao_image.exposure(0.8);
    gt_ao_image.write(path_to_data + "gt_ao_image_dark.exr");
    gt_ao_image.write(path_to_data + "gt_ao_image_dark.png");

    return 0;
}
