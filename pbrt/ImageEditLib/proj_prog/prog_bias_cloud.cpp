#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

int main(int argc, char *argv[])
{
    std::string base_path = "/Users/corneria/Documents/Research/bias_research/progressive_controls/6398d9dc531c9eefb55fa6a3/figures/bias_vis/";
    // std::string end_path = "/Users/corneria/Documents/Research/bias_research/pbrttest/hypn_results/disk_smoke/no_update_no_clamp/final/";

    imedit::Image gt = imedit::Image(base_path + "gt.exr");
    imedit::Image half = imedit::Image(base_path + "half.exr");

    int height = 340;
    int width = height * 2;
    int base_j = 50;
    int base_i = 55;

    imedit::Image gt_inset = imedit::Image(width, height);
    imedit::Image half_inset = imedit::Image(width, height);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            gt_inset(j, i) = gt(j + base_j, i + base_i);
            half_inset(j, i) = half(j + base_j, i + base_i);
        }
    }

    gt_inset.write(base_path + "inset_gt.exr");
    gt_inset.write(base_path + "inset_gt.png");
    half_inset.write(base_path + "inset_half.exr");
    half_inset.write(base_path + "inset_half.png");
}
