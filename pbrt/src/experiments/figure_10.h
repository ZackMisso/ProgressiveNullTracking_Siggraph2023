#pragma once

#include "experiment_base.h"
#include "trans_samplers/pcg32.h"

#if OPENVDB

class Figure_10 : public ExperimentBase {
  public:
    Figure_10(pbrt::Options options);

    void runEqualSampleRenders(pcg32& rng, std::string name,
                               pbrt::Options options, std::string transType,
                               std::string scatType, int samples,
                               bool is_progressive);

    void initializeSceneMediumBox(std::string transType, std::string scatType,
                                  std::string name, std::string maj,
                                  Float majorant, bool is_progressive);

    void initializeLightSources();
};

#endif
