#pragma once

#include "trans_sampler.h"
// #include "trans_halton_sampler.h"
#include "halton_process.h"

namespace pbrt
{

class Trans_Halton : public TSampler
{
public:
    Trans_Halton(int ns, unsigned int seed);
    virtual Float Get1D();
    virtual void reseedSampler();
    virtual std::unique_ptr<Sampler> Clone(int seed);
    virtual bool StartNextSample();

private:
    // Trans_Halton_sampler halton;
    Halton_Process<pcg32> halton;
};

}
