#pragma once

#include "experiment_base.h"
#include "pbrt.h"
#include "trans_samplers/pcg32.h"

class Figure_3 : public ExperimentBase {
  public:
    Figure_3(pbrt::Options options);
};
