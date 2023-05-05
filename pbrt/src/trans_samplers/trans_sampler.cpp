#include "trans_sampler.h"

namespace pbrt
{

TSampler::TSampler(int ns) : Sampler(ns)
{
    currentPixelSampleIndex = 0;
    currentDim = 0;
}

TSampler::TSampler(int ns, unsigned int seed) : Sampler(ns)
{
    currentPixelSampleIndex = 0;
    currentDim = 0;
    baseSeed = seed;
    reseed(seed);
}

void TSampler::reseed(unsigned int seed)
{
    // std::cout << "WEEEE: " << seed << std::endl;
    baseSeed = seed;
    sampSeed = seed;
    jitSeed = Hash1(((seed << 5) + ((seed << 2) ^ 0x26afb)) ^ (0x41ffbc0) >> 2);

    reseedSampler();
    reseedJitter();
}

void TSampler::reseedSampler()
{
    rng = pcg32(baseSeed, 123);
    rng = pcg32(rng.nextUInt(), 1);
}

void TSampler::reseedJitter()
{
    jitRNG = pcg32(jitSeed, 1);
}

Point2f TSampler::Get2D()
{
    return Point2f(Get1D(), Get1D());
}

bool TSampler::StartNextSample()
{
    currentDim = 0;
    currentPixelSampleIndex++;
    return true;
}

void TSampler::StartPixel(const Point2i& p)
{
    currentPixelSampleIndex = 0;
    currentDim = 0;
}

}
