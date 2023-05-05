#pragma once

#include "experiment_base.h"

#if OPENVDB

class VDB_Smoke_Tests : public ExperimentBase {
  public:
    VDB_Smoke_Tests(pbrt::Options options);

    // void runTest(std::string transType, bool isGT, std::string name, int
    // iter);

    // void runEqualExtRenders(pbrt::Options options, std::string transType,
    //                         Float majorant, int samples);

    void runEqualSampleRenders(pbrt::Options options, std::string transType,
                               Float majorant, int samples);

    // void runEqualSampleStratifiedRenders(pbrt::Options options,
    //                                      std::string transType, Float
    //                                      majorant, int samples);

    // void runEqualExtRenders_Macrogrid(std::string transType, Float majorant,
    //                                   int samples);

    // void runEqualSampleRenders_Macrogrid(std::string transType, Float
    // majorant,
    //                                      int samples);

    // void runEqualSampleStratifiedRenders_Macrogrid(std::string transType,
    //                                                Float majorant, int
    //                                                samples);

    void initializeSceneMediumBox(std::string transType, std::string name,
                                  std::string maj, Float majorant);

    void initializeLightSources();
};

#endif
