#include "trans_latin_stream.h"
#include "trans_permute.h"

namespace pbrt
{

Trans_LatinHyperCubes_Stream::Trans_LatinHyperCubes_Stream(int ns, unsigned int seed) : TSampler(ns)
{
    currentTransSegment = 1;
    currentFFSegment = 1;
    sampSeed = seed;
    baseSeed = seed;
}

Float Trans_LatinHyperCubes_Stream::Get1D()
{
    int32_t value = Trans_Permutation::permute(currentPixelSampleIndex, samplesPerPixel, rng.nextUInt());

    double sample = double(value) + 0.5;
    double jitter = (jitRNG.nextDouble() - 0.5);
    sample += jitter;
    sample /= double(samplesPerPixel);
    currentDim++;

    return sample;
}

std::unique_ptr<Sampler> Trans_LatinHyperCubes_Stream::Clone(int seed)
{
    Trans_LatinHyperCubes_Stream* sampler = new Trans_LatinHyperCubes_Stream(*this);

    return std::unique_ptr<Sampler>(sampler);
}

bool Trans_LatinHyperCubes_Stream::StartNextSample()
{
    TSampler::StartNextSample();

    return true;
}

void Trans_LatinHyperCubes_Stream::StartPixel(const Point2i &p,
                                              int max_x_res,
                                              int max_y_res,
                                              int multiplier)
{
    currentTransSegment = 1;
    currentFFSegment = 1;

    sampSeed = baseSeed + p[1] * max_x_res + p[0] + max_x_res * max_y_res * multiplier;
}

void Trans_LatinHyperCubes_Stream::BeforeTransmittanceSample()
{
    rng.seed(142, Hash2(sampSeed * 4000 + currentTransSegment));
    currentTransSegment++;
}

void Trans_LatinHyperCubes_Stream::BeforeFreeFlightSample()
{
    rng.seed(142, Hash2(sampSeed * 4000 + 1000 + currentFFSegment));
    currentFFSegment++;
}

}
