#include "trans_independent.h"
#include "camera.h"

namespace pbrt
{

Trans_Independent::Trans_Independent(int ns, unsigned int seed) : TSampler(ns, seed)
{
    // currentTransSegment = 0;
    // currentFFSegment = 0;
}

Float Trans_Independent::Get1D()
{
    return rng.nextDouble();
}

std::unique_ptr<Sampler> Trans_Independent::Clone(int seed)
{
    Trans_Independent* sampler = new Trans_Independent(*this);
    sampler->sampSeed = sampSeed;
    sampler->jitSeed = jitSeed;
    sampler->reseedJitter();
    sampler->reseedSampler();

    return std::unique_ptr<Sampler>(sampler);
}

void Trans_Independent::StartPixel(const Point2i &p,
                                   int max_x_res,
                                   int max_y_res,
                                   int multiplier)
{
    sampSeed = baseSeed + p[1] * max_x_res + p[0] + max_x_res * max_y_res * multiplier;

    // rng.seed(142, Hash2(sampSeed ^ Hash1(currentPixelSampleIndex)));

    rng.seed(142, hash(sampSeed, currentPixelSampleIndex));
}

// void Trans_Independent::BeforeTransmittanceSample()
// {
//     rng.seed(142, Hash2(sampSeed * 4000 + currentTransSegment));
//     currentTransSegment++;
// }
//
// void Trans_Independent::BeforeFreeFlightSample()
// {
//     rng.seed(142, Hash2(sampSeed * 4000 + 1000 + currentFFSegment));
//     currentFFSegment++;
// }

CameraSample Trans_Independent::GetCameraSample(const Point2i &pRaster)
{
    CameraSample cs;

    #if CAMERA_CENTERED
        cs.pFilm = (Point2f)(pRaster) + Point2f(0.5, 0.5);
    #else
        cs.pFilm = (Point2f)(pRaster) + Get2D();
    #endif
    //
    // cs.pFilm = Point2f(0.5, 0.5);
    cs.time = Get1D();
    cs.pLens = Get2D();
    return cs;
}

}
