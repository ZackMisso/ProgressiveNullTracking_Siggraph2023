#pragma once

#include "experiment_base.h"

#if OPENVDB

class ExpDiskSmoke_Tests : public ExperimentBase {
  public:
    ExpDiskSmoke_Tests(pbrt::Options options);

    void runEqualSampleRenders(std::string name, pbrt::Options options,
                               std::string transType, std::string scatType,
                               std::string gridType, Float majorant,
                               int samples, int bounces, int r, Float time);

    void initializeSceneMediumBox(std::string transType, std::string scatType,
                                  std::string name, std::string maj,
                                  Float majorant, Float time);

    void initializeLightSources();
};

#endif
