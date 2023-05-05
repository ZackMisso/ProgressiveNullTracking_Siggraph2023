#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

int main(int argc, char *argv[])
{
    std::string base_path = "/Users/corneria/Documents/Research/bias_research/progressive_controls/6398d9dc531c9eefb55fa6a3/figures/var_reason/";
    // std::string end_path = "/Users/corneria/Documents/Research/bias_research/pbrttest/hypn_results/disk_smoke/no_update_no_clamp/final/";

    imedit::Image gt = imedit::Image(base_path + "gt.exr");
    imedit::Image bound = imedit::Image(base_path + "bound.exr");
    imedit::Image ok = imedit::Image(base_path + "ok.exr");
    imedit::Image bad = imedit::Image(base_path + "bad.exr");

    int height = 45;
    int width = 60;
    int base_j = 280;
    int base_i = 270;

    imedit::Image gt_inset = imedit::Image(width, height);
    imedit::Image bound_inset = imedit::Image(width, height);
    imedit::Image ok_inset = imedit::Image(width, height);
    imedit::Image bad_inset = imedit::Image(width, height);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            gt_inset(j, i) = gt(j + base_j, i + base_i);
            bound_inset(j, i) = bound(j + base_j, i + base_i);
            ok_inset(j, i) = ok(j + base_j, i + base_i);
            bad_inset(j, i) = bad(j + base_j, i + base_i);
        }
    }

    gt_inset = double_sized_image(gt_inset);
    gt_inset = double_sized_image(gt_inset);
    gt_inset = double_sized_image(gt_inset);
    bound_inset = double_sized_image(bound_inset);
    bound_inset = double_sized_image(bound_inset);
    bound_inset = double_sized_image(bound_inset);
    ok_inset = double_sized_image(ok_inset);
    ok_inset = double_sized_image(ok_inset);
    ok_inset = double_sized_image(ok_inset);
    bad_inset = double_sized_image(bad_inset);
    bad_inset = double_sized_image(bad_inset);
    bad_inset = double_sized_image(bad_inset);

    gt_inset.write(base_path + "inset_gt.exr");
    gt_inset.write(base_path + "inset_gt.png");
    bound_inset.write(base_path + "inset_bound.exr");
    bound_inset.write(base_path + "inset_bound.png");
    ok_inset.write(base_path + "inset_ok.exr");
    ok_inset.write(base_path + "inset_ok.png");
    bad_inset.write(base_path + "inset_bad.exr");
    bad_inset.write(base_path + "inset_bad.png");
}
