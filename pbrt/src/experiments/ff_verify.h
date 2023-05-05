#pragma once

#include "experiment_base.h"

#if OPENVDB

class FF_Verify : public ExperimentBase {
  public:
    FF_Verify(pbrt::Options options);

    void runEqualSampleRenders(pbrt::Options options, std::string transType,
                               Float majorant, int samples);

    void initializeSceneMediumBox(std::string transType, std::string name,
                                  std::string maj, Float majorant);

    void initializeLightSources();
};

#endif
