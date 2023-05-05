#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

int main(int argc, char *argv[])
{
    std::string base_path = "/Users/corneria/Documents/Research/bias_research/progressive_controls/6398d9dc531c9eefb55fa6a3/figures/naive_prog/";
    // std::string end_path = "/Users/corneria/Documents/Research/bias_research/pbrttest/hypn_results/disk_smoke/no_update_no_clamp/final/";

    imedit::Image gt = imedit::Image(base_path + "gt.exr");
    imedit::Image one = imedit::Image(base_path + "maj_1.exr");
    imedit::Image two = imedit::Image(base_path + "maj_2.exr");
    imedit::Image three = imedit::Image(base_path + "maj_3.exr");
    imedit::Image four = imedit::Image(base_path + "maj_4.exr");

    imedit::Image im = one;

    for (int i = 0; i < 128; ++i)
    {
        if (i == 0)
        {
            im.write(base_path + "one.exr");
            im.write(base_path + "one.png");
        }
        else if (i == 1)
        {
            im = im + (two - im) * 1.0 / double(i + 1);
            im.write(base_path + "two.exr");
            im.write(base_path + "two.png");
        }
        if (i == 2)
        {
            im = im + (three - im) * 1.0 / double(i + 1);
        }
        if (i == 3)
        {
            im = im + (four - im) * 1.0 / double(i + 1);
            im.write(base_path + "three.exr");
            im.write(base_path + "three.png");
        }
        else
        {
            im = im + (gt - im) * 1.0 / double(i + 1);
        }
    }

    im.write(base_path + "four.exr");
    im.write(base_path + "four.png");
}
