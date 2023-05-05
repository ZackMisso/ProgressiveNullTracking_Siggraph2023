#include "trans_hammersley.h"
// #include <transm/util.h>

namespace pbrt
{

Trans_Hammersley::Trans_Hammersley(int ns, unsigned int seed) : TSampler(ns, seed)
{
    inv = 1.0 / double(ns);
}

void Trans_Hammersley::reseedSampler()
{
    TSampler::reseedSampler();

    halton.init_random(&rng);
    scramble = rng.nextUInt();
}

Float Trans_Hammersley::Get1D()
{
    double sample = 0.0;

    if (currentDim == 0) {
        sample = randomDigitScramble(currentPixelSampleIndex * inv, scramble);
    } else {
        if (currentDim > 255)
            return rng.nextDouble();

        sample = halton.sample(currentDim, currentPixelSampleIndex);
    }

    currentDim++;
    return sample;
}

std::unique_ptr<Sampler> Trans_Hammersley::Clone(int seed)
{
    Trans_Hammersley* sampler = new Trans_Hammersley(*this);
    sampler->sampSeed = sampSeed;
    sampler->jitSeed = jitSeed;
    sampler->reseedJitter();
    sampler->reseedSampler();

    return std::unique_ptr<Sampler>(sampler);
}

bool Trans_Hammersley::StartNextSample()
{
    TSampler::StartNextSample();

    reseedSampler();

    return true;
}

}
