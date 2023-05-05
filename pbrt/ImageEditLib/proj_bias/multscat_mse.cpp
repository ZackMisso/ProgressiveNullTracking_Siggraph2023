#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

// TODO: fully convert this script
int main(int argc, char* argv[])
{
    // imedit::Image img_one = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/smoke_5/512_unb_step/volpath.exr");
    // imedit::Image img_two = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/smoke_5/512_unb_step/volpath_ps_16384.exr");
    // imedit::Image img_three = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/smoke_5/512_unb_step/volpath_ps_8192.exr");
    // imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/smoke_5/gt.exr");

    imedit::Image gt = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/multi_scat/gt/volpath.exr");

    std::ofstream mse_data;
    mse_data.open("mse.txt");

    for (int index = 10; index <= 200; index += 10)
    {
        imedit::Image weighted = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/multi_scat/"+std::to_string(index)+"/weighted/volpath.exr");
        imedit::Image debiased = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/multi_scat/"+std::to_string(index)+"/debiased/volpath.exr");

        double weighted_err = imedit::mean_sqr_error(gt, weighted);
        double debiased_err = imedit::mean_sqr_error(gt, debiased);

        mse_data << double(index) / 100.0 << std::endl;
        mse_data << weighted_err << std::endl;
        mse_data << debiased_err << std::endl;
    }

    mse_data.close();

    return 0;
}
