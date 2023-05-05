#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

int main(int argc, char *argv[])
{
    std::string base_path = "/Users/corneria/Documents/Research/bias_research/pbrttest/hypn_results/disk_smoke/no_update_no_clamp/";
    std::string end_path = "/Users/corneria/Documents/Research/bias_research/pbrttest/hypn_results/disk_smoke/no_update_no_clamp/final/";

    for (int i = 0; i <= 120; ++i)
    {
        std::cout << "i: " << i << std::endl;
        imedit::Image image = imedit::Image(base_path + std::to_string(i) + "/image.exr");
        image.write(end_path + "image_" + std::to_string(i) + ".exr");
    }
}
