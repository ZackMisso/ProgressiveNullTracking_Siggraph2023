#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

int main(int argc, char *argv[])
{
    std::string base_path = "/Users/corneria/Documents/Research/bias_research/progressive_controls/6398d9dc531c9eefb55fa6a3/figures/perf_comp/";
    // std::string end_path = "/Users/corneria/Documents/Research/bias_research/pbrttest/hypn_results/disk_smoke/no_update_no_clamp/final/";

    imedit::Image gt = imedit::Image(base_path + "gt.exr");
    imedit::Image rat = imedit::Image(base_path + "ratio.exr");
    imedit::Image ours = imedit::Image(base_path + "ours.exr");
    imedit::Image debiased = imedit::Image(base_path + "debiased.exr");
    imedit::Image nvidia = imedit::Image(base_path + "kettunen.exr");
    imedit::Image pseries = imedit::Image(base_path + "pseries_cdf.exr");
    // imedit::Image half = imedit::Image(base_path + "half.exr");

    imedit::Image rat_foxy = imedit::Image(base_path + "test/foxy.exr");
    imedit::Image rat_foxy_adap = imedit::Image(base_path + "test/foxy_adap.exr");
    imedit::Image rat_slight = imedit::Image(base_path + "test/slight.exr");
    imedit::Image rat_signif = imedit::Image(base_path + "test/signif.exr");
    imedit::Image rat_rat = imedit::Image(base_path + "test/ratio.exr");

    int height = 600;
    int width = 650;
    int base_j = 60;
    int base_i = 0;

    imedit::Image gt_fig = imedit::Image(width, height);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            gt_fig(j, i) = gt(j + base_j, i + base_i);
        }
    }

    // gt_fig.write(base_path + "gt_fig.exr");
    // gt_fig.write(base_path + "gt_fig.png");

    height = 60;
    width = 30;
    base_j = 250 + base_j;
    base_i = 230;

    imedit::Image gt_inset = imedit::Image(width, height);
    imedit::Image rat_inset = imedit::Image(width, height);
    imedit::Image ours_inset = imedit::Image(width, height);
    imedit::Image debiased_inset = imedit::Image(width, height);
    imedit::Image nvidia_inset = imedit::Image(width, height);
    imedit::Image pseries_inset = imedit::Image(width, height);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            gt_inset(j, i) = gt(j + base_j, i + base_i);
            rat_inset(j, i) = rat(j + base_j, i + base_i);
            ours_inset(j, i) = ours(j + base_j, i + base_i);
            debiased_inset(j, i) = debiased(j + base_j, i + base_i);
            nvidia_inset(j, i) = nvidia(j + base_j, i + base_i);
            pseries_inset(j, i) = pseries(j + base_j, i + base_i);
        }
    }

    gt_inset = double_sized_image(gt_inset);
    gt_inset = double_sized_image(gt_inset);
    gt_inset = double_sized_image(gt_inset);

    rat_inset = double_sized_image(rat_inset);
    rat_inset = double_sized_image(rat_inset);
    rat_inset = double_sized_image(rat_inset);

    ours_inset = double_sized_image(ours_inset);
    ours_inset = double_sized_image(ours_inset);
    ours_inset = double_sized_image(ours_inset);

    debiased_inset = double_sized_image(debiased_inset);
    debiased_inset = double_sized_image(debiased_inset);
    debiased_inset = double_sized_image(debiased_inset);

    nvidia_inset = double_sized_image(nvidia_inset);
    nvidia_inset = double_sized_image(nvidia_inset);
    nvidia_inset = double_sized_image(nvidia_inset);

    pseries_inset = double_sized_image(pseries_inset);
    pseries_inset = double_sized_image(pseries_inset);
    pseries_inset = double_sized_image(pseries_inset);

    // gt_inset.write(base_path + "inset_gt.exr");
    // rat_inset.write(base_path + "inset_ratio.exr");
    // ours_inset.write(base_path + "inset_ours.exr");
    // nvidia_inset.write(base_path + "inset_kettunen.exr");
    // debiased_inset.write(base_path + "inset_debiased.exr");
    // pseries_inset.write(base_path + "inset_pseries_cdf.exr");
    // gt_inset.write(base_path + "inset_gt.png");
    // rat_inset.write(base_path + "inset_ratio.png");
    // ours_inset.write(base_path + "inset_ours.png");
    // nvidia_inset.write(base_path + "inset_kettunen.png");
    // debiased_inset.write(base_path + "inset_debiased.png");
    // pseries_inset.write(base_path + "inset_pseries_cdf.png");

    std::cout << "Rat MSE: " << mean_sqr_error(gt, rat) << std::endl;
    std::cout << "Ours MSE: "
              << mean_sqr_error(gt, ours) << std::endl;
    std::cout << "Nvidia MSE: "
              << mean_sqr_error(gt, nvidia) << std::endl;
    std::cout << "Deiased MSE: "
              << mean_sqr_error(gt, debiased) << std::endl;
    std::cout << "Pseries MSE: "
              << mean_sqr_error(gt, pseries) << std::endl;

    std::cout << "Regular Adap MSE: " << mean_sqr_error(gt, rat_foxy_adap) << std::endl;
    std::cout << "Regular MSE: " << mean_sqr_error(gt, rat_foxy) << std::endl;
    std::cout << "Slight MSE: " << mean_sqr_error(gt, rat_slight) << std::endl;
    std::cout << "Signif MSE: " << mean_sqr_error(gt, rat_signif) << std::endl;
    std::cout << "Ratio MSE: " << mean_sqr_error(gt, rat_rat) << std::endl;
}
