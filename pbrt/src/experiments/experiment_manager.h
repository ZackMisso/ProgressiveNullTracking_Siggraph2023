#pragma once

#include "api.h"
#include "paramset.h"
#include "pbrt.h"

namespace pbrt {

class ExperimentManager {
  public:
    static bool run_experiment(pbrt::Options options, int start_index, int argc,
                               char *argv[]);
};

}  // namespace pbrt
