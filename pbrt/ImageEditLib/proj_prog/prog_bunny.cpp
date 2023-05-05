#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

int main(int argc, char *argv[])
{
    std::string base_path = "/Users/corneria/Documents/Research/bias_research/progressive_controls/6398d9dc531c9eefb55fa6a3/figures/bunny/";
    std::string path_to_rat = "/Users/corneria/Documents/Research/bias_research/progressive_controls/pbrttest/results/bunny_conv/ratio/";
    std::string path_to_ours = "/Users/corneria/Documents/Research/bias_research/progressive_controls/pbrttest/results/bunny_conv/grid_80/";
    // std::string end_path = "/Users/corneria/Documents/Research/bias_research/pbrttest/hypn_results/disk_smoke/no_update_no_clamp/final/";

    imedit::Image gt = imedit::Image(base_path + "gt.exr");

    int min_samp = 1;
    int max_samp = 4096;

    std::ofstream file;
    file.open(base_path + "mses.txt");

    for (int i = min_samp; i <= max_samp; i *= 2)
    {
        std::cout << "Computing MSE for " << i << " samples" << std::endl;
        // std::cout << "opening ratio" << std::endl;
        imedit::Image rat = imedit::Image(path_to_rat + "image_ps_" + std::to_string(i) + ".exr");
        // std::cout << "opening ours" << std::endl;
        imedit::Image ours = imedit::Image(path_to_ours + "image_ps_" + std::to_string(i) + ".exr");
        // std::cout << "computing mse" << std::endl;
        file << i << std::endl;
        file << mean_sqr_error(rat, gt) << std::endl;
        file << mean_sqr_error(ours, gt) << std::endl;
    }

    file.close();

    int height = 100;
    int width = 100;
    int base_j = 460;
    int base_i = 620;

    std::vector<int> spps = std::vector<int>();
    spps.push_back(8);
    spps.push_back(64);
    spps.push_back(512);
    spps.push_back(4096);

    imedit::Image gt_inset = imedit::Image(width, height);

    for (int k = 0; k < spps.size(); ++k)
    {
        std::cout << "Creating insets for " << spps[k] << " samples" << std::endl;

        imedit::Image rat = imedit::Image(base_path + "rat_" + std::to_string(spps[k]) + ".exr");
        imedit::Image ours = imedit::Image(base_path + "ours_" + std::to_string(spps[k]) + ".exr");

        imedit::Image rat_inset = imedit::Image(width, height);
        imedit::Image our_inset = imedit::Image(width, height);

        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                gt_inset(j, i) = gt(j + base_j, i + base_i);
                rat_inset(j, i) = rat(j + base_j, i + base_i);
                our_inset(j, i) = ours(j + base_j, i + base_i);
            }
        }

        gt_inset.write(base_path + "inset_gt.png");
        gt_inset.write(base_path + "inset_gt.exr");
        rat_inset.write(base_path + "inset_rat_" + std::to_string(spps[k]) + ".exr");
        rat_inset.write(base_path + "inset_rat_" + std::to_string(spps[k]) + ".png");
        our_inset.write(base_path + "inset_ours_" + std::to_string(spps[k]) + ".exr");
        our_inset.write(base_path + "inset_ours_" + std::to_string(spps[k]) + ".png");
    }

    height = 960 - 140;
    width = 1080;
    base_j = 0;
    base_i = 140;

    imedit::Image smol_gt = imedit::Image(width, height);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            smol_gt(j, i) = gt(j + base_j, i + base_i);
        }
    }

    smol_gt.write(base_path + "smol_gt.png");
    smol_gt.write(base_path + "smol_gt.exr");
}
