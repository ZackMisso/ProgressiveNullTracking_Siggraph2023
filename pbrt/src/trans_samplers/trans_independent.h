#pragma once

#include "trans_sampler.h"
#include "sampler.h"

namespace pbrt
{

// This is equivalent to pbrts random implementation

class Trans_Independent : public TSampler {
public:
    Trans_Independent(int ns, unsigned int seed);
    virtual Float Get1D();
    virtual std::unique_ptr<Sampler> Clone(int seed);
    void StartPixel(const Point2i &p,
                    int max_x_res,
                    int max_y_res,
                    int multiplier);
    virtual CameraSample GetCameraSample(const Point2i &pRaster);
    // virtual void BeforeTransmittanceSample();
    // virtual void BeforeFreeFlightSample();
private:
    uint64_t currentFFSegment;
    uint64_t currentTransSegment;
};

}
