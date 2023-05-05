#pragma once

#include "experiment_base.h"

class BlueProcSmokeExperiment : public ExperimentBase {
  public:
    BlueProcSmokeExperiment(pbrt::Options options);

    void runEqualSampleRenders(std::string name, pbrt::Options options,
                               std::string transType, std::string scatType,
                               std::string gridType, Float majorant,
                               int samples, int bounces, int r);

    void initializeSceneMediumBox(std::string transType, std::string scatType,
                                  std::string name, std::string maj,
                                  Float majorant);

    void initializeLightSources();
};
