#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

int main(int argc, char *argv[])
{
    std::cout << "what" << std::endl;
    std::string path = "/Users/corneria/Documents/Research/bias_research/Differential_Experiments/PSDR_Vol/paper_results/diff_render/solid_box_zack/results/";

    imedit::Image test = imedit::Image(path + "deriv_take_1.exr");
    test.write(path + "blah.exr");
}
