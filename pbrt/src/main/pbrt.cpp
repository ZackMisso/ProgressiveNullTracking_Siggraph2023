
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include "pbrt.h"

#include "api.h"
#include "experiments/blue_proc_smoke.h"
#include "experiments/bunny_conv.h"
#include "experiments/dis_cloud.h"
#include "experiments/exp_adaptive_smoke.h"
#include "experiments/exp_disk_smoke.h"
#include "experiments/exp_prog_smoke.h"
#include "experiments/exp_sphere_smoke.h"
#include "experiments/experiment_manager.h"
#include "experiments/ff_verify.h"
#include "experiments/raymarch_comp.h"
#include "experiments/vdb_smoke.h"
#include "integrator.h"
#include "media/majorant/majorant.h"
#include "parallel.h"
#include "parser.h"

#if LOGGING
#include <glog/logging.h>
#endif

#if OPENVDB
#include <openvdb/openvdb.h>
#include <openvdb/tools/ChangeBackground.h>

#include "media/density/vdb_density.h"
#endif

using namespace pbrt;

static void usage(const char *msg = nullptr) {
    if (msg) fprintf(stderr, "pbrt: %s\n\n", msg);

    fprintf(stderr, R"(usage: pbrt [<options>] <filename.pbrt...>
Rendering options:
  --cropwindow <x0,x1,y0,y1> Specify an image crop window.
  --help               Print this help text.
  --nthreads <num>     Use specified number of threads for rendering.
  --outfile <filename> Write the final image to the given filename.
  --quick              Automatically reduce a number of quality settings to
                       render more quickly.
  --quiet              Suppress all text output other than error messages.

Logging options:
  --logdir <dir>       Specify directory that log files should be written to.
                       Default: system temp directory (e.g. $TMPDIR or /tmp).
  --logtostderr        Print all logging messages to stderr.
  --minloglevel <num>  Log messages at or above this level (0 -> INFO,
                       1 -> WARNING, 2 -> ERROR, 3-> FATAL). Default: 0.
  --v <verbosity>      Set VLOG verbosity.

Reformatting options:
  --cat                Print a reformatted version of the input file(s) to
                       standard output. Does not render an image.
  --toply              Print a reformatted version of the input file(s) to
                       standard output and convert all triangle meshes to
                       PLY files. Does not render an image.
  --err_gen <file>     Runs the error generation subsystem with the specified
                       job file
)");
    exit(msg ? 1 : 0);
}

// main program
int main(int argc, char *argv[]) {
#if LOGGING
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold = 1;  // Warning and above.
#endif

#if OPENVDB
    openvdb::initialize();
#endif

    // initializes some defaults
    Globals::NO_CLAMP = true;
    Globals::MONO_DENSE_SPEC_ALBEDO = false;
    Globals::WRITE_INCREMENTS = false;
    Globals::TEST_SCENES_PATH = "~/Documents/Research/testscenes/";

    Options options;
    std::vector<std::string> filenames;
    // Process command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--nthreads") || !strcmp(argv[i], "-nthreads")) {
            if (i + 1 == argc) usage("missing value after --nthreads argument");
            options.nThreads = atoi(argv[++i]);
        } else if (!strncmp(argv[i], "--nthreads=", 11)) {
            options.nThreads = atoi(&argv[i][11]);
        } else if (!strcmp(argv[i], "--outfile") ||
                   !strcmp(argv[i], "-outfile")) {
            if (i + 1 == argc) usage("missing value after --outfile argument");
            options.imageFile = argv[++i];
        } else if (!strcmp(argv[i], "--cropwindow") ||
                   !strcmp(argv[i], "-cropwindow")) {
            if (i + 4 >= argc)
                usage("missing value after --cropwindow argument");
            options.cropWindow[0][0] = atof(argv[++i]);
            options.cropWindow[0][1] = atof(argv[++i]);
            options.cropWindow[1][0] = atof(argv[++i]);
            options.cropWindow[1][1] = atof(argv[++i]);
        } else if (!strncmp(argv[i], "--outfile=", 10)) {
            options.imageFile = &argv[i][10];
        } else if (!strcmp(argv[i], "--logdir") ||
                   !strcmp(argv[i], "-logdir")) {
            if (i + 1 == argc) usage("missing value after --logdir argument");
#if LOGGING
            FLAGS_log_dir = argv[++i];
#endif
        } else if (!strncmp(argv[i], "--logdir=", 9)) {
#if LOGGING
            FLAGS_log_dir = &argv[i][9];
#endif
        } else if (!strcmp(argv[i], "--minloglevel") ||
                   !strcmp(argv[i], "-minloglevel")) {
            if (i + 1 == argc)
                usage("missing value after --minloglevel argument");
#if LOGGING
            FLAGS_minloglevel = atoi(argv[++i]);
#endif
        } else if (!strncmp(argv[i], "--minloglevel=", 14)) {
#if LOGGING
            FLAGS_minloglevel = atoi(&argv[i][14]);
#endif
        } else if (!strcmp(argv[i], "--quick") || !strcmp(argv[i], "-quick")) {
            options.quickRender = true;
        } else if (!strcmp(argv[i], "--quiet") || !strcmp(argv[i], "-quiet")) {
            options.quiet = true;
        } else if (!strcmp(argv[i], "--cat") || !strcmp(argv[i], "-cat")) {
            options.cat = true;
        } else if (!strcmp(argv[i], "--toply") || !strcmp(argv[i], "-toply")) {
            options.toPly = true;
        } else if (!strcmp(argv[i], "--v") || !strcmp(argv[i], "-v")) {
            if (i + 1 == argc) usage("missing value after --v argument");
#if LOGGING
            FLAGS_v = atoi(argv[++i]);
#endif
        } else if (!strncmp(argv[i], "--v=", 4)) {
#if LOGGING
            FLAGS_v = atoi(argv[i] + 4);
#endif
        } else if (!strcmp(argv[i], "--logtostderr")) {
#if LOGGING
            FLAGS_logtostderr = true;
#endif
        } else if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-help") ||
                   !strcmp(argv[i], "-h")) {
            usage();
            return 0;
        } else if (!strcmp(argv[i], "--experiment")) {
            std::cout << "running experiment manager" << std::endl;
            bool should_quit =
                ExperimentManager::run_experiment(options, ++i, argc, argv);
            if (should_quit)
                return 0;
            else {
                if (!Globals::MONO_DENSE_SPEC_ALBEDO)
                    std::cout << "ITDIDNOTWORK" << std::endl;
            }
        }
        // else if (!strcmp(argv[i], "--err_gen")) {
        //     // ErrorGenerator(std::string(argv[++i]));
        //     return 0;
        //     // TODO
        // }
        else
            filenames.push_back(argv[i]);
    }

    // Print welcome banner
    if (!options.quiet && !options.cat && !options.toPly) {
        if (sizeof(void *) == 4)
            printf(
                "*** WARNING: This is a 32-bit build of pbrt. It will crash "
                "if used to render highly complex scenes. ***\n");
        printf("pbrt version 3 (built %s at %s) [Detected %d cores]\n",
               __DATE__, __TIME__, NumSystemCores());
#ifndef NDEBUG
#if LOGGING
        LOG(INFO) << "Running debug build";
#endif
        printf("*** DEBUG BUILD ***\n");
#endif  // !NDEBUG
        printf(
            "Copyright (c)1998-2018 Matt Pharr, Greg Humphreys, and Wenzel "
            "Jakob.\n");
        printf(
            "The source code to pbrt (but *not* the book contents) is covered "
            "by the BSD License.\n");
        printf("See the file LICENSE.txt for the conditions of the license.\n");
        fflush(stdout);
    }

    // VDB_DensityFunction("/Users/corneria/Desktop/VDB_Smoke/default_smoke_10.vdb");

    // return 0;

    // pbrtInit(options);

    // if (STAT_TRACKING)
    // {
    //     Global_Tracker::initialize();
    // }

    // // debug code
    // IntervalAdaptiveDensityMajorant maj =
    //     IntervalAdaptiveDensityMajorant(nullptr, 10);

    // std::vector<MacroGridQuery> queries = maj.MacroVoxelTraversal(
    //     Point3f(-0.5, -0.5, -0.5), Point3f(0.5, 0.5, 0.5), 0.0, 1.0);

    // for (int i = 0; i < queries.size(); ++i) {
    //     std::cout << "T_START: " << queries[i].t_start << std::endl;
    //     std::cout << "T_END: " << queries[i].t_end << std::endl;
    //     std::cout << std::endl;
    // }

    // adaptive sampling experiments
    // delete new ExpAdaptiveSmoke_Tests(options);
    // delete new ExpProgressiveSmoke_Tests(options);
    // delete new ExpSphereSmoke_Tests(options);
    // delete new ExpDiskSmoke_Tests(options);
    // delete new VDB_Smoke_Tests(options);
    // delete new FF_Verify(options);
    // delete new BlueProcSmokeExperiment(options);
    // delete new DisneyCloud(options);
    // delete new BunnyConvergenceExperiment(options);
    // delete new RaymarchComparisonExperiment(options);

    // pbrtCleanup();
    // return 0;

    // if (is_power_of_2(513))
    // {
    //     std::cout << "FALSE" << std::endl;
    // }
    // std::cout << "TRUE" << std::endl;
    // return 0;

    pbrtInit(options);

    // Process scene description
    if (filenames.empty()) {
        // Parse scene from standard input
        return 0;
        pbrtParseFile("-");
    } else {
        // std::cout << "WHOW" << std::endl;
        // Parse scene from input files
        for (const std::string &f : filenames) pbrtParseFile(f);
    }

    pbrtCleanup();
    return 0;
}
