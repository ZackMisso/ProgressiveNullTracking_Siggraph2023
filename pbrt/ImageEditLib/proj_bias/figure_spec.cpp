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

    int buffer = 2;
    imedit::Pixel mid_color = imedit::Pixel(1.0);

    for (int i = 0; i < mix_image.height(); ++i)
    {
        for (int j = 0; j < mix_image.width(); ++j)
        {
            int dist = i - mix_image.height() / 2;

            if (dist < -buffer)
            {
                mix_image(j, i) = bot(j, i);
            }
            else if (dist > buffer)
            {
                mix_image(j, i) = top(j, i);
            }
            else
            {
                mix_image(j, i) = mid_color;
            }
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

void evaluate_conv_data(std::string input_path,
                        std::string output_path,
                        int max_samples)
{
    imedit::Image gt = imedit::Image(input_path + "_gt.exr");

    std::ofstream out;
    out.open(output_path + ".txt");

    int index = 1;
    while (index <= max_samples)
    {
        imedit::Image deb_image = imedit::Image(input_path + "_deb_ps_" + std::to_string(index) + ".exr");

        double variance = sqr_error(deb_image, gt);
        std::cout << "variance: " << variance << std::endl;
        out << index << std::endl;
        out << variance << std::endl;

        index *= 2;
    }

    out.close();
}

void evaluate_conv_data(std::string input_path,
                        std::string output_path,
                        std::string name,
                        int iterations,
                        int max_samples)
{
    // std::cout << "gt" << std::endl;
    // std::cout << input_path + "_gt.exr" << std::endl;
    imedit::Image gt = imedit::Image(input_path + name + "_gt.exr");

    std::vector<double> means = std::vector<double>();
    std::vector<double> vars = std::vector<double>();
    for (int i = 1; i <= max_samples; i *= 2)
    {
        means.push_back(0.0);
        vars.push_back(0.0);
    }

    // std::cout << "starting iters" << std::endl;
    for (int i = 0; i < iterations; ++i)
    {
        std::cout << "evaluating iter: " << i << std::endl;
        int index = 1;
        int j = 0;
        while (index <= max_samples)
        {
            imedit::Image deb_image = imedit::Image(input_path + std::to_string(i) + "/" + name + "_deb_ps_" + std::to_string(index) + ".exr");

            double variance = sqr_error(deb_image, gt);
            double err = error(deb_image, gt);
            // std::cout << "variance: " << variance << std::endl;
            // out << index << std::endl;
            // out << variance << std::endl;

            means[j] = means[j] + 1.0 / double(i + 1) * (err - means[j]);
            vars[j] = vars[j] + 1.0 / double(i + 1) * (variance - vars[j]);

            index *= 2;
            j++;
        }
    }

    std::ofstream out;
    out.open(output_path + ".txt");

    for (int i = 0; i < means.size(); ++i)
    {
        out << pow(2, i) << std::endl;
        out << means[i] << std::endl;
        out << vars[i] << std::endl;
    }

    out.close();
}

int main(int argc, char *argv[])
{
    std::string path_to_final_data = "/Users/corneria/Documents/Research/bias_research/5fd6d63d5c7c413b7656e9e4/figures/spec_mani/images/";
    std::string path_to_conv_data = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/davis_cloud/conv/";

    // evaluate_conv_data(path_to_conv_data + "exp", path_to_final_data + "exp", 262144);
    // evaluate_conv_data(path_to_conv_data + "davis_25", path_to_final_data + "davis_25", 262144);
    // evaluate_conv_data(path_to_conv_data + "davis_50", path_to_final_data + "davis_50", 262144);
    // evaluate_conv_data(path_to_conv_data + "davis_75", path_to_final_data + "davis_75", 262144);

    // path_to_conv_data = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/davis_cloud/conv_batch/";

    //1080x1080, at 420x

    imedit::Image gt = imedit::Image(path_to_final_data + "gt.png");
    imedit::Image cume = imedit::Image(path_to_final_data + "pseries_cume.png");
    imedit::Image tele = imedit::Image(path_to_final_data + "tele.png");
    imedit::Image bern = imedit::Image(path_to_final_data + "bernoulli.png");

    imedit::Image bern_inset = imedit::Image(100,100);
    imedit::Image cume_inset = imedit::Image(100,100);
    imedit::Image tele_inset = imedit::Image(100,100);
    imedit::Image gt_inset = imedit::Image(100,100);

    for (int i = 0; i < bern_inset.height(); ++i)
    {
        for (int j = 0; j < bern_inset.width(); ++j)
        {
            bern_inset(j, i) = bern(j + 105, i + 280);
            cume_inset(j, i) = cume(j + 105, i + 280);
            tele_inset(j, i) = tele(j + 105, i + 280);
            gt_inset(j, i) = gt(j + 105 + 420, i + 280);
        }
    }

    bern_inset = imedit::double_sized_image(bern_inset);
    cume_inset = imedit::double_sized_image(cume_inset);
    tele_inset = imedit::double_sized_image(tele_inset);
    gt_inset = imedit::double_sized_image(gt_inset);

    bern_inset.write(path_to_final_data + "bern_inset.png");
    cume_inset.write(path_to_final_data + "cume_inset.png");
    tele_inset.write(path_to_final_data + "tele_inset.png");
    gt_inset.write(path_to_final_data + "gt_inset.png");


    // evaluate_conv_data(path_to_conv_data, path_to_final_data + "exp", "exp", 512, 8192);
    // evaluate_conv_data(path_to_conv_data, path_to_final_data + "davis_25", "davis_25", 512, 8192);
    // evaluate_conv_data(path_to_conv_data, path_to_final_data + "davis_50", "davis_50", 512, 8192);
    // evaluate_conv_data(path_to_conv_data, path_to_final_data + "davis_75", "davis_75", 512, 8192);

    // imedit::Image exp_gt = imedit::Image(path_to_final_data + "exp_gt.exr");
    // imedit::Image exp_deb = imedit::Image(path_to_final_data + "exp_deb.exr");

    // imedit::Image d25_gt = imedit::Image(path_to_final_data + "davis_25_gt.exr");
    // imedit::Image d25_deb = imedit::Image(path_to_final_data + "davis_25_deb.exr");

    // imedit::Image d50_gt = imedit::Image(path_to_final_data + "davis_5_gt.exr");
    // imedit::Image d50_deb = imedit::Image(path_to_final_data + "davis_5_deb.exr");

    // imedit::Image d75_gt = imedit::Image(path_to_final_data + "davis_75_gt.exr");
    // imedit::Image d75_deb = imedit::Image(path_to_final_data + "davis_75_deb.exr");

    // imedit::Image exp_diff = imedit::Image(640, 360);
    // imedit::Image d25_diff = imedit::Image(640, 360);
    // imedit::Image d50_diff = imedit::Image(640, 360);
    // imedit::Image d75_diff = imedit::Image(640, 360);
    // imedit::Image combine = imedit::Image(640, 360);
    // imedit::Image diff_combine = imedit::Image(640, 360);

    // for (int i = 0; i < combine.height(); ++i)
    // {
    //     for (int j = 0; j < combine.width(); ++j)
    //     {
    //         int act_i = i + 94;
    //         int act_j = j + 90;

    //         exp_diff(j, i) = imedit::Pixel::abs(exp_gt(act_j, act_i) - exp_deb(act_j, act_i));
    //         d25_diff(j, i) = imedit::Pixel::abs(d25_gt(act_j, act_i) - d25_deb(act_j, act_i));
    //         d50_diff(j, i) = imedit::Pixel::abs(d50_gt(act_j, act_i) - d50_deb(act_j, act_i));
    //         d75_diff(j, i) = imedit::Pixel::abs(d75_gt(act_j, act_i) - d75_deb(act_j, act_i));

    //         if (i < combine.height()/4)
    //             combine(j, i) = exp_deb(act_j, act_i);
    //         else if (i < combine.height()/2)
    //             combine(j, i) = d25_deb(act_j, act_i);
    //         else if (i < 3*combine.height()/4)
    //             combine(j, i) = d50_deb(act_j, act_i);
    //         else
    //             combine(j, i) = d75_deb(act_j, act_i);

    //         if (i < combine.height()/4)
    //             diff_combine(j, i) = exp_diff(j, i);
    //         else if (i < combine.height()/2)
    //             diff_combine(j, i) = d25_diff(j, i);
    //         else if (i < 3*combine.height()/4)
    //             diff_combine(j, i) = d50_diff(j, i);
    //         else
    //             diff_combine(j, i) = d75_diff(j, i);
    //     }
    // }

    // combine.write(path_to_final_data + "combine.exr");
    // combine.write(path_to_final_data + "combine.png");

    // diff_combine.write(path_to_final_data + "diff_combine.exr");
    // diff_combine.write(path_to_final_data + "diff_combine.png");

    // exp_diff.write(path_to_final_data + "exp_diff.exr");
    // exp_diff.write(path_to_final_data + "exp_diff.png");

    // d25_diff.write(path_to_final_data + "d25_diff.exr");
    // d25_diff.write(path_to_final_data + "d25_diff.png");

    // d50_diff.write(path_to_final_data + "d50_diff.exr");
    // d50_diff.write(path_to_final_data + "d50_diff.png");

    // d75_diff.write(path_to_final_data + "d75_diff.exr");
    // d75_diff.write(path_to_final_data + "d75_diff.png");

    // imedit::Image gt_3 = imedit::Image(path_to_data + "3/gt.exr");
    // imedit::Image gt_8 = imedit::Image(path_to_data + "8/gt.exr");
    // imedit::Image gt_exp = imedit::Image(path_to_data + "exp/gt.exr");

    // imedit::Image pink_cmf_exp_3 = imedit::Image(path_to_data + "3/pink_cmf_exp.exr");
    // imedit::Image pink_pcume_exp_3 = imedit::Image(path_to_data + "3/pink_pcume_exp.exr");
    // imedit::Image pink_pcume_3 = imedit::Image(path_to_data + "3/pink_pcume.exr");
    // imedit::Image pink_ratio_3 = imedit::Image(path_to_data + "3/pink_ratio.exr");
    // imedit::Image pink_tay_ray_exp_3 = imedit::Image(path_to_data + "3/pink_tay_ray_exp.exr");
    // imedit::Image pink_tay_3 = imedit::Image(path_to_data + "3/pink_tay.exr");
    // imedit::Image pink_tele_3 = imedit::Image(path_to_data + "3/tele.exr");

    // double pink_cmf_exp_3_mse = root_mean_sqr_error(gt_3, pink_cmf_exp_3);
    // double pink_pcume_exp_3_mse = root_mean_sqr_error(gt_3, pink_pcume_exp_3);
    // double pink_pcume_3_mse = root_mean_sqr_error(gt_3, pink_pcume_3);
    // double pink_ratio_3_mse = root_mean_sqr_error(gt_3, pink_ratio_3);
    // double pink_tay_ray_exp_3_mse = root_mean_sqr_error(gt_3, pink_tay_ray_exp_3);
    // double pink_tay_3_mse = root_mean_sqr_error(gt_3, pink_tay_3);
    // double pink_tele_3_mse = root_mean_sqr_error(gt_3, pink_tele_3);

    // std::cout << "MSE for c = 0.3: " << std::endl;
    // std::cout << "pink cmf exp: " << pink_cmf_exp_3_mse << std::endl;
    // std::cout << "pink pcume exp: " << pink_pcume_exp_3_mse << std::endl;
    // std::cout << "pink pcume: " << pink_pcume_3_mse << std::endl;
    // std::cout << "pink ratio: " << pink_ratio_3_mse << std::endl;
    // std::cout << "pink tay exp: " << pink_tay_ray_exp_3_mse << std::endl;
    // std::cout << "pink tay: " << pink_tay_3_mse << std::endl;
    // std::cout << "tele: " << pink_tele_3_mse << std::endl;
    // std::cout << std::endl;

    // imedit::Image pink_cmf_exp_8 = imedit::Image(path_to_data + "8/pink_cmf_exp.exr");
    // imedit::Image pink_pcume_exp_8 = imedit::Image(path_to_data + "8/pink_pcume_exp.exr");
    // imedit::Image pink_pcume_8 = imedit::Image(path_to_data + "8/pink_pcume.exr");
    // imedit::Image pink_ratio_8 = imedit::Image(path_to_data + "8/pink_ratio.exr");
    // imedit::Image pink_tay_ray_exp_8 = imedit::Image(path_to_data + "8/pink_tay_ray_exp.exr");
    // imedit::Image pink_tay_8 = imedit::Image(path_to_data + "8/pink_tay.exr");
    // imedit::Image pink_tele_8 = imedit::Image(path_to_data + "8/tele.exr");

    // double pink_cmf_exp_8_mse = root_mean_sqr_error(gt_8, pink_cmf_exp_8);
    // double pink_pcume_exp_8_mse = root_mean_sqr_error(gt_8, pink_pcume_exp_8);
    // double pink_pcume_8_mse = root_mean_sqr_error(gt_8, pink_pcume_8);
    // double pink_ratio_8_mse = root_mean_sqr_error(gt_8, pink_ratio_8);
    // double pink_tay_ray_exp_8_mse = root_mean_sqr_error(gt_8, pink_tay_ray_exp_8);
    // double pink_tay_8_mse = root_mean_sqr_error(gt_8, pink_tay_8);
    // double pink_tele_8_mse = root_mean_sqr_error(gt_8, pink_tele_8);

    // std::cout << "RMSE for c = 0.8: " << std::endl;
    // std::cout << "pink cmf exp: " << pink_cmf_exp_8_mse << std::endl;
    // std::cout << "pink pcume exp: " << pink_pcume_exp_8_mse << std::endl;
    // std::cout << "pink pcume: " << pink_pcume_8_mse << std::endl;
    // std::cout << "pink ratio: " << pink_ratio_8_mse << std::endl;
    // std::cout << "pink tay exp: " << pink_tay_ray_exp_8_mse << std::endl;
    // std::cout << "pink tay: " << pink_tay_8_mse << std::endl;
    // std::cout << "tele: " << pink_tele_8_mse << std::endl;
    // std::cout << std::endl;

    // imedit::Image cmf_exp = imedit::Image(path_to_data + "exp/pseries_cdf.exr");
    // imedit::Image pcume_exp = imedit::Image(path_to_data + "exp/pseries_cumulative.exr");
    // imedit::Image ratio_exp = imedit::Image(path_to_data + "exp/ratio.exr");
    // imedit::Image nvidia_exp = imedit::Image(path_to_data + "exp/exp_nvidia.exr");
    // imedit::Image tele_exp = imedit::Image(path_to_data + "exp/tele.exr");

    // double cmf_exp_mse = root_mean_sqr_error(gt_exp, cmf_exp);
    // double pcume_exp_mse = root_mean_sqr_error(gt_exp, pcume_exp);
    // double ratio_exp_mse = root_mean_sqr_error(gt_exp, ratio_exp);
    // double nvidia_exp_mse = root_mean_sqr_error(gt_exp, nvidia_exp);
    // double tele_exp_mse = root_mean_sqr_error(gt_exp, tele_exp);

    // std::cout << "RMSE for exponential: " << std::endl;
    // std::cout << "cmf exp: " << cmf_exp_mse << std::endl;
    // std::cout << "pcume exp: " << pcume_exp_mse << std::endl;
    // std::cout << "ratio: " << ratio_exp_mse << std::endl;
    // std::cout << "nvidia: " << nvidia_exp_mse << std::endl;
    // std::cout << "tele: " << tele_exp_mse << std::endl;
    // std::cout << std::endl;

    // // now to create the insets
    // create_3_inset(gt_3, path_to_data, "gt_ref");
    // create_3_inset(pink_cmf_exp_3, path_to_data, "pink_cmf_exp");
    // create_3_inset(pink_pcume_exp_3, path_to_data, "pink_pcume_exp");
    // create_3_inset(pink_pcume_3, path_to_data, "pink_pcume");
    // create_3_inset(pink_ratio_3, path_to_data, "pink_ratio");
    // create_3_inset(pink_tay_ray_exp_3, path_to_data, "pink_nvidia");
    // create_3_inset(pink_tay_3, path_to_data, "pink_tay");
    // create_3_inset(pink_tele_3, path_to_data, "pink_tele");

    // split_image(gt_3, gt_8, path_to_data);

    // create_8_inset(gt_8, path_to_data, "gt_ref");
    // create_8_inset(pink_cmf_exp_8, path_to_data, "pink_cmf_exp");
    // create_8_inset(pink_pcume_exp_8, path_to_data, "pink_pcume_exp");
    // create_8_inset(pink_pcume_8, path_to_data, "pink_pcume");
    // create_8_inset(pink_ratio_8, path_to_data, "pink_ratio");
    // create_8_inset(pink_tay_ray_exp_8, path_to_data, "pink_nvidia");
    // create_8_inset(pink_tay_8, path_to_data, "pink_tay");
    // create_8_inset(pink_tele_8, path_to_data, "pink_tele");

    // create_8_inset(gt_exp, path_to_data, "exp_gt_ref");
    // create_8_inset(cmf_exp, path_to_data, "exp_cmf");
    // create_8_inset(pcume_exp, path_to_data, "exp_pcume");
    // create_8_inset(ratio_exp, path_to_data, "exp_ratio");
    // create_8_inset(nvidia_exp, path_to_data, "exp_nvidia");
    // create_8_inset(tele_exp, path_to_data, "exp_tele");

    // create_3_inset(gt_exp, path_to_data, "exp_gt_ref");
    // create_3_inset(cmf_exp, path_to_data, "exp_cmf");
    // create_3_inset(pcume_exp, path_to_data, "exp_pcume");
    // create_3_inset(ratio_exp, path_to_data, "exp_ratio");
    // create_3_inset(nvidia_exp, path_to_data, "exp_nvidia");
    // create_3_inset(tele_exp, path_to_data, "exp_tele");

    // // std::cout << "pink 3" << std::endl;
    // imedit::Image tele_3 = imedit::Image(path_to_data + "3/pink_tele_inset.exr");
    // imedit::Image nvidia_3 = imedit::Image(path_to_data + "3/pink_nvidia_inset.exr");
    // imedit::Image ratio_3 = imedit::Image(path_to_data + "3/pink_ratio_inset.exr");

    // imedit::Image tele_8 = imedit::Image(path_to_data + "8/pink_tele_inset.exr");
    // imedit::Image nvidia_8 = imedit::Image(path_to_data + "8/pink_nvidia_inset.exr");
    // imedit::Image ratio_8 = imedit::Image(path_to_data + "8/pink_ratio_inset.exr");

    // imedit::Image tele_exp_3 = imedit::Image(path_to_data + "3/exp_tele_inset.exr");
    // imedit::Image nvidia_exp_3 = imedit::Image(path_to_data + "3/exp_nvidia_inset.exr");
    // imedit::Image ratio_exp_3 = imedit::Image(path_to_data + "3/exp_ratio_inset.exr");

    // imedit::Image tele_exp_8 = imedit::Image(path_to_data + "8/exp_tele_inset.exr");
    // imedit::Image nvidia_exp_8 = imedit::Image(path_to_data + "8/exp_nvidia_inset.exr");
    // imedit::Image ratio_exp_8 = imedit::Image(path_to_data + "8/exp_ratio_inset.exr");

    // split_image_horizontal(tele_exp_3, tele_3, path_to_data + "3/tele_final_inset");
    // split_image_horizontal(nvidia_exp_3, nvidia_3, path_to_data + "3/nvidia_final_inset");
    // split_image_horizontal(ratio_exp_3, ratio_3, path_to_data + "3/ratio_final_inset");

    // split_image_horizontal(tele_exp_8, tele_8, path_to_data + "8/tele_final_inset");
    // split_image_horizontal(nvidia_exp_8, nvidia_8, path_to_data + "8/nvidia_final_inset");
    // split_image_horizontal(ratio_exp_8, ratio_8, path_to_data + "8/ratio_final_inset");

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
