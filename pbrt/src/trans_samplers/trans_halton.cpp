#include "trans_halton.h"

namespace pbrt
{

Trans_Halton::Trans_Halton(int ns, unsigned int seed) : TSampler(ns, seed) { }

Float Trans_Halton::Get1D()
{
    // std::cout << "INSIDE GET 1D" << std::endl;
    if (currentDim > 255)
        return rng.nextDouble();

    // std::cout << currentPixelSampleIndex << std::endl;

    double sample = halton.sample(currentDim, currentPixelSampleIndex);
    currentDim++;
    return sample;
}

void Trans_Halton::reseedSampler()
{
    TSampler::reseedSampler();

    halton.init_random(&rng);
}

std::unique_ptr<Sampler> Trans_Halton::Clone(int seed)
{
    Trans_Halton* sampler = new Trans_Halton(*this);
    sampler->sampSeed = sampSeed;
    sampler->jitSeed = jitSeed;
    sampler->reseedJitter();
    sampler->reseedSampler();

    return std::unique_ptr<Sampler>(sampler);
}

bool Trans_Halton::StartNextSample()
{
    TSampler::StartNextSample();

    reseedSampler();

    return true;
}

}
