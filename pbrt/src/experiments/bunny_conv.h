#pragma once

#include "experiment_base.h"
#include "trans_samplers/pcg32.h"

#if OPENVDB

class BunnyConvergenceExperiment : public ExperimentBase {
  public:
    BunnyConvergenceExperiment(pbrt::Options options);

    void runEqualSampleDebiasedRenders(pcg32& rng, std::string name,
                                       pbrt::Options options,
                                       std::string transType,
                                       std::string scatType, Float majorant,
                                       int samples, int bounces, int r);

    void runEqualSampleRenders(pcg32& rng, std::string name,
                               pbrt::Options options, std::string transType,
                               std::string scatType, Float majorant,
                               int samples, int bounces, int r);

    void initializeSceneMediumBox(std::string transType, std::string scatType,
                                  std::string name, std::string maj,
                                  Float majorant);

    void runEqualCostDebiasedRenders(pcg32& rng, std::string name,
                                     pbrt::Options options,
                                     std::string transType,
                                     std::string scatType, Float majorant,
                                     int ext_calls, int bounces, int r);

    void runEqualCostRenders(pcg32& rng, std::string name,
                             pbrt::Options options, std::string transType,
                             std::string scatType, Float majorant,
                             int ext_calls, int bounces, int r);

    void initializeLightSources();
};

#endif
