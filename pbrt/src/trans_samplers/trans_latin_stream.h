#pragma once

#include "trans_sampler.h"

namespace pbrt
{

class Trans_LatinHyperCubes_Stream : public TSampler
{
public:
    Trans_LatinHyperCubes_Stream(int ns, unsigned int seed);
    virtual Float Get1D();
    Float sample_dimension();
    virtual std::unique_ptr<Sampler> Clone(int seed);
    virtual bool StartNextSample();
    virtual void BeforeTransmittanceSample();
    virtual void BeforeFreeFlightSample();
    virtual void StartPixel(const Point2i &p,
                            int max_x_res,
                            int max_y_res,
                            int multiplier);

private:
    uint64_t currentFFSegment;
    uint64_t currentTransSegment;
};

}
