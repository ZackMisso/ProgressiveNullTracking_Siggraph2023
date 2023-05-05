#pragma once

#include "experiment_base.h"
#include "pbrt.h"
#include "trans_samplers/pcg32.h"

#if OPENVDB

class Figure_9 : public ExperimentBase {
  public:
    Figure_9(pbrt::Options options);

    void equal_cost_run(pcg32& rng, pbrt::Options options, long ext_calls,
                        std::string path, std::string transType,
                        bool is_progressive);

    void equal_sample_run(pcg32& rng, pbrt::Options options, int samples,
                          std::string path, std::string transType,
                          bool is_progressive);

    void initializeSceneMediumBox(std::string transType, std::string name,
                                  std::string maj, Float majorant,
                                  bool is_progressive);

    void initializeLightSources();
};

#endif
