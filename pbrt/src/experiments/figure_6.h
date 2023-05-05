#pragma once

#include "experiment_base.h"
#include "trans_samplers/pcg32.h"

#if OPENVDB

class Figure_6 : public ExperimentBase {
  public:
    Figure_6(pbrt::Options options);

    void runEqualSampleRenders(pbrt::Options options, pcg32& rng,
                               std::string path, Float majorant, int samples);

    void initializeSceneMediumBox(Float majorant);

    void initializeLightSources();
};

#endif
