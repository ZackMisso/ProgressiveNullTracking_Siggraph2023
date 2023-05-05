#pragma once

#include "experiment_base.h"
#include "pbrt.h"
#include "trans_samplers/pcg32.h"

#if OPENVDB

class DisneyCloud : public ExperimentBase {
  public:
    DisneyCloud(pbrt::Options options);

    void runTest(pbrt::Options options, std::string transType, bool isGT,
                 std::string name, int iter);

    void findAngle(pbrt::Options options, std::string transType, Float majorant,
                   Float anlge, long samples);

    void findAngle_batchRun(pcg32& rng, pbrt::Options options,
                            std::string transType, Float majorant, Float anlge,
                            long samples, int index);

    void runEqualExtRenders(pbrt::Options options, std::string transType,
                            Float majorant, long samples);

    void initializeSceneMediumBox(std::string transType, std::string name,
                                  std::string maj, Float majorant);

    void initializeLightSources();
};

#endif
