#pragma once

#include "experiment_base.h"
#include "pbrt.h"
#include "trans_samplers/pcg32.h"

class Figure_5 : public ExperimentBase {
  public:
    Figure_5(pbrt::Options options);

    void runTest(pbrt::Options options, std::string transType, bool isGT,
                 std::string name, int iter);

    void gtBounding(pcg32& rng, pbrt::Options options);

    void gtNonBounding(pcg32& rng, pbrt::Options options);

    void initializeSceneMediumBox(std::string transType, std::string name,
                                  std::string scatType, std::string vdbType,
                                  Float majorant);

    void initializeLightSources();
};
