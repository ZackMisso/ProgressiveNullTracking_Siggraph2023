#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

int main(int argc, char *argv[])
{
    int start_task = 1;
    int end_task = 38;

    uint32_t seed_one = 0x134;
    uint32_t seed_two = 0xacd;

    int tasks_chosen = 1;

    pcg32 rng = pcg32(seed_one, seed_two);

    for (int i = 0; i < tasks_chosen; ++i)
        rng.nextFloat();

    std::cout << "Your next task is: " << int(rng.nextFloat() * (end_task - start_task) + start_task) << std::endl;

    return 0;
}
