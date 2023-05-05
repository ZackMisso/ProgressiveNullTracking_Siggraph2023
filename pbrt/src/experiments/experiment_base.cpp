#include "experiment_base.h"

#include <fstream>
#include <sstream>

void ExperimentBase::initializeIntegrator(int sec, long extCalls, int maxD,
                                          std::string integrator,
                                          std::string trans, std::string ff,
                                          bool isCorrelated,
                                          bool isMultithreaded) {
    std::unique_ptr<int[]> maxDepth(new int[1]);
    maxDepth[0] = maxD;

    std::unique_ptr<int[]> maxSeconds(new int[1]);
    maxSeconds[0] = sec;

    std::unique_ptr<long[]> maxExtCalls(new long[1]);
    maxExtCalls[0] = extCalls;

    std::unique_ptr<std::string[]> trans_sampler(new std::string[1]);
    trans_sampler[0] = trans;

    std::unique_ptr<std::string[]> ff_sampler(new std::string[1]);
    ff_sampler[0] = ff;

    std::unique_ptr<bool[]> ff_correlated(new bool[1]);
    ff_correlated[0] = isCorrelated;

    std::unique_ptr<bool[]> multithreaded(new bool[1]);
    multithreaded[0] = isMultithreaded;

    ParamSet integratorSet;
    integratorSet.AddInt("maxdepth", std::move(maxDepth), 1);
    integratorSet.AddInt("maxSeconds", std::move(maxSeconds), 1);
    integratorSet.AddLong("maxExtCalls", std::move(maxExtCalls), 1);
    integratorSet.AddString("trans_sampler", std::move(trans_sampler), 1);
    integratorSet.AddString("ff_sampler", std::move(ff_sampler), 1);
    integratorSet.AddBool("ff_correlated", std::move(ff_correlated), 1);
    integratorSet.AddBool("multithreaded", std::move(multithreaded), 1);

    pbrtIntegrator(integrator, integratorSet);
}

void ExperimentBase::initializeConsistentIntegrator(
    int sec, long extCalls, int maxD, Float scale_val, int seedOne, int seedTwo,
    int seedThree, std::string integrator, std::string trans, std::string ff,
    bool isCorrelated, bool isMultithreaded) {
    std::unique_ptr<int[]> maxDepth(new int[1]);
    maxDepth[0] = maxD;

    std::unique_ptr<int[]> maxSeconds(new int[1]);
    maxSeconds[0] = sec;

    std::unique_ptr<int[]> oneSeed(new int[1]);
    oneSeed[0] = seedOne;

    std::unique_ptr<int[]> twoSeed(new int[1]);
    twoSeed[0] = seedTwo;

    std::unique_ptr<int[]> threeSeed(new int[1]);
    threeSeed[0] = seedThree;

    std::unique_ptr<Float[]> scale(new Float[1]);
    scale[0] = scale_val;

    std::unique_ptr<long[]> maxExtCalls(new long[1]);
    maxExtCalls[0] = extCalls;

    std::unique_ptr<std::string[]> trans_sampler(new std::string[1]);
    trans_sampler[0] = trans;

    std::unique_ptr<std::string[]> ff_sampler(new std::string[1]);
    ff_sampler[0] = ff;

    std::unique_ptr<bool[]> ff_correlated(new bool[1]);
    ff_correlated[0] = isCorrelated;

    std::unique_ptr<bool[]> multithreaded(new bool[1]);
    multithreaded[0] = isMultithreaded;

    ParamSet integratorSet;
    integratorSet.AddInt("maxdepth", std::move(maxDepth), 1);
    integratorSet.AddInt("maxSeconds", std::move(maxSeconds), 1);
    integratorSet.AddInt("seedOne", std::move(oneSeed), 1);
    integratorSet.AddInt("seedTwo", std::move(twoSeed), 1);
    integratorSet.AddInt("seedThree", std::move(threeSeed), 1);
    integratorSet.AddFloat("scale", std::move(scale), 1);
    integratorSet.AddLong("maxExtCalls", std::move(maxExtCalls), 1);
    integratorSet.AddString("trans_sampler", std::move(trans_sampler), 1);
    integratorSet.AddString("ff_sampler", std::move(ff_sampler), 1);
    integratorSet.AddBool("ff_correlated", std::move(ff_correlated), 1);
    integratorSet.AddBool("multithreaded", std::move(multithreaded), 1);

    pbrtIntegrator(integrator, integratorSet);
}

void ExperimentBase::initializeIntegratorSeeded(
    int sec, long extCalls, int maxD, std::string integrator, std::string trans,
    std::string ff, int seedOne, int seedTwo, int seedThree, bool isCorrelated,
    bool isMultithreaded) {
    std::unique_ptr<int[]> maxDepth(new int[1]);
    maxDepth[0] = maxD;

    std::unique_ptr<int[]> maxSeconds(new int[1]);
    maxSeconds[0] = sec;

    std::unique_ptr<int[]> oneSeed(new int[1]);
    oneSeed[0] = seedOne;

    std::unique_ptr<int[]> twoSeed(new int[1]);
    twoSeed[0] = seedTwo;

    std::unique_ptr<int[]> threeSeed(new int[1]);
    threeSeed[0] = seedThree;

    std::unique_ptr<long[]> maxExtCalls(new long[1]);
    maxExtCalls[0] = extCalls;

    std::unique_ptr<std::string[]> trans_sampler(new std::string[1]);
    trans_sampler[0] = trans;

    std::unique_ptr<std::string[]> ff_sampler(new std::string[1]);
    ff_sampler[0] = ff;

    std::unique_ptr<bool[]> ff_correlated(new bool[1]);
    ff_correlated[0] = isCorrelated;

    std::unique_ptr<bool[]> multithreaded(new bool[1]);
    multithreaded[0] = isMultithreaded;

    ParamSet integratorSet;
    integratorSet.AddInt("maxdepth", std::move(maxDepth), 1);
    integratorSet.AddInt("maxSeconds", std::move(maxSeconds), 1);
    integratorSet.AddInt("seedOne", std::move(oneSeed), 1);
    integratorSet.AddInt("seedTwo", std::move(twoSeed), 1);
    integratorSet.AddInt("seedThree", std::move(threeSeed), 1);
    integratorSet.AddLong("maxExtCalls", std::move(maxExtCalls), 1);
    integratorSet.AddString("trans_sampler", std::move(trans_sampler), 1);
    integratorSet.AddString("ff_sampler", std::move(ff_sampler), 1);
    integratorSet.AddBool("ff_correlated", std::move(ff_correlated), 1);
    integratorSet.AddBool("multithreaded", std::move(multithreaded), 1);

    pbrtIntegrator(integrator, integratorSet);
}

void ExperimentBase::initializeSampler(std::string sampler, int pixelSamples) {
    std::unique_ptr<int[]> pixSamples(new int[1]);
    pixSamples[0] = pixelSamples;

    ParamSet samplerSet;
    samplerSet.AddInt("pixelsamples", std::move(pixSamples), 1);

    pbrtSampler(sampler, samplerSet);
}

void ExperimentBase::initializePixelFilter(std::string filter) {
    pbrtPixelFilter(filter, ParamSet());
}

void ExperimentBase::initializeFilm(std::string output, int xres, int yres) {
    std::unique_ptr<int[]> yresolution(new int[1]);
    yresolution[0] = yres;

    std::unique_ptr<int[]> xresolution(new int[1]);
    xresolution[0] = xres;

    std::unique_ptr<std::string[]> filename(new std::string[1]);
    filename[0] = output;

    ParamSet filmSet;
    filmSet.AddInt("yresolution", std::move(yresolution), 1);
    filmSet.AddInt("xresolution", std::move(xresolution), 1);
    filmSet.AddString("filename", std::move(filename), 1);

    pbrtFilm("image", filmSet);
}

void ExperimentBase::initializeFilm(std::vector<Float> crop, std::string output,
                                    Float scle, int xres, int yres) {
    std::unique_ptr<int[]> yresolution(new int[1]);
    yresolution[0] = yres;

    std::unique_ptr<int[]> xresolution(new int[1]);
    xresolution[0] = xres;

    std::unique_ptr<std::string[]> filename(new std::string[1]);
    filename[0] = output;

    std::unique_ptr<Float[]> cropwindow(new Float[4]);
    cropwindow[0] = crop[0];
    cropwindow[1] = crop[1];
    cropwindow[2] = crop[2];
    cropwindow[3] = crop[3];

    std::unique_ptr<Float[]> scale(new Float[1]);
    scale[0] = scle;

    ParamSet filmSet;
    filmSet.AddInt("yresolution", std::move(yresolution), 1);
    filmSet.AddInt("xresolution", std::move(xresolution), 1);
    filmSet.AddFloat("cropwindow", std::move(cropwindow), 4);
    filmSet.AddString("filename", std::move(filename), 1);
    filmSet.AddFloat("scale", std::move(scale), 1);

    pbrtFilm("image", filmSet);
}

void ExperimentBase::initializeCamera(std::string camera, Float fov_val) {
    std::unique_ptr<Float[]> fov(new Float[1]);
    fov[0] = fov_val;

    ParamSet cameraSet;
    cameraSet.AddFloat("fov", std::move(fov), 1);

    pbrtCamera(camera, cameraSet);
}

void ExperimentBase::initializeTrimesh(std::vector<int> inds,
                                       std::vector<Float> uvs,
                                       std::vector<Float> ps) {
    std::unique_ptr<int[]> indices(new int[inds.size()]);
    for (int i = 0; i < inds.size(); ++i) {
        indices[i] = inds[i];
    }

    std::unique_ptr<Point2f[]> uv(new Point2f[uvs.size() / 2]);
    for (int i = 0; i < uvs.size(); i += 2) {
        Point2f pt(uvs[i], uvs[i + 1]);
        uv[i / 2] = pt;
    }

    std::unique_ptr<Point3f[]> P(new Point3f[ps.size() / 3]);
    for (int i = 0; i < ps.size(); i += 3) {
        Point3f pt(ps[i], ps[i + 1], ps[i + 2]);
        P[i / 3] = pt;
    }

    ParamSet trimeshSet;
    trimeshSet.AddInt("indices", std::move(indices), inds.size());
    trimeshSet.AddPoint2f("uv", std::move(uv), uvs.size() / 2);
    trimeshSet.AddPoint3f("P", std::move(P), ps.size() / 3);

    pbrtShape("trianglemesh", trimeshSet);
}

void ExperimentBase::initializeTrimesh(std::vector<int> inds,
                                       std::vector<Float> ps) {
    std::unique_ptr<int[]> indices(new int[inds.size()]);
    for (int i = 0; i < inds.size(); ++i) {
        indices[i] = inds[i];
    }

    std::unique_ptr<Point3f[]> P(new Point3f[ps.size() / 3]);
    for (int i = 0; i < ps.size(); i += 3) {
        Point3f pt(ps[i], ps[i + 1], ps[i + 2]);
        P[i / 3] = pt;
    }

    ParamSet trimeshSet;
    trimeshSet.AddInt("indices", std::move(indices), inds.size());
    trimeshSet.AddPoint3f("P", std::move(P), ps.size() / 3);

    pbrtShape("trianglemesh", trimeshSet);
}

void ExperimentBase::initializeShapeParamSet(ParamSet& paramSet,
                                             std::string indFile,
                                             std::string stFile,
                                             std::string pFile,
                                             std::string nFile) {
    std::vector<int> indices = std::vector<int>();
    std::vector<Float> sts = std::vector<Float>();
    std::vector<Float> ps = std::vector<Float>();
    std::vector<Float> ns = std::vector<Float>();

    if (!indFile.empty()) {
        std::ifstream indStream;
        indStream.open(exp_path + indFile);
        std::stringstream indTokens;
        indTokens << indStream.rdbuf();

        std::string str;
        while (!indTokens.eof()) {
            indTokens >> str;
            indices.push_back(stoi(str));
        }
    }

    if (!stFile.empty()) {
        std::ifstream stStream;
        stStream.open(exp_path + stFile);
        std::stringstream stTokens;
        stTokens << stStream.rdbuf();

        std::string str;
        while (!stTokens.eof()) {
            stTokens >> str;
            sts.push_back(stod(str));
        }
    }

    if (!pFile.empty()) {
        std::ifstream pStream;
        pStream.open(exp_path + pFile);
        std::stringstream pTokens;
        pTokens << pStream.rdbuf();

        std::string str;
        while (!pTokens.eof()) {
            pTokens >> str;
            ps.push_back(stod(str));
        }
    }

    if (!nFile.empty()) {
        std::ifstream nStream;
        nStream.open(exp_path + nFile);
        std::stringstream nTokens;
        nTokens << nStream.rdbuf();

        std::string str;
        while (!nTokens.eof()) {
            nTokens >> str;
            ns.push_back(stod(str));
        }
    }

    initializeShapeParamSet(paramSet, indices, sts, ps, ns);
}

void ExperimentBase::initializeShape(std::string shape, std::string indFile,
                                     std::string stFile, std::string pFile,
                                     std::string nFile) {
    std::vector<int> indices = std::vector<int>();
    std::vector<Float> sts = std::vector<Float>();
    std::vector<Float> ps = std::vector<Float>();
    std::vector<Float> ns = std::vector<Float>();

    if (!indFile.empty()) {
        std::ifstream indStream;
        indStream.open(exp_path + indFile);
        std::stringstream indTokens;
        indTokens << indStream.rdbuf();

        std::string str;
        while (!indTokens.eof()) {
            indTokens >> str;
            indices.push_back(stoi(str));
        }
    }

    if (!stFile.empty()) {
        std::ifstream stStream;
        stStream.open(exp_path + stFile);
        std::stringstream stTokens;
        stTokens << stStream.rdbuf();

        std::string str;
        while (!stTokens.eof()) {
            stTokens >> str;
            sts.push_back(stod(str));
        }
    }

    if (!pFile.empty()) {
        std::ifstream pStream;
        pStream.open(exp_path + pFile);
        std::stringstream pTokens;
        pTokens << pStream.rdbuf();

        std::string str;
        while (!pTokens.eof()) {
            pTokens >> str;
            ps.push_back(stod(str));
        }
    }

    if (!nFile.empty()) {
        std::ifstream nStream;
        nStream.open(exp_path + nFile);
        std::stringstream nTokens;
        nTokens << nStream.rdbuf();

        std::string str;
        while (!nTokens.eof()) {
            nTokens >> str;
            ns.push_back(stod(str));
        }
    }

    initializeShape(shape, indices, sts, ps, ns);
}

void ExperimentBase::initializeShapeParamSet(ParamSet& paramSet,
                                             const std::vector<int>& inds,
                                             const std::vector<Float>& sts,
                                             const std::vector<Float>& ps,
                                             const std::vector<Float>& ns) {
    if (inds.size()) {
        std::unique_ptr<int[]> indices(new int[inds.size()]);
        for (int i = 0; i < inds.size(); ++i) {
            indices[i] = inds[i];
        }
        paramSet.AddInt("indices", std::move(indices), inds.size());
    }

    if (sts.size()) {
        std::unique_ptr<Point2f[]> st(new Point2f[sts.size() / 2]);
        for (int i = 0; i < sts.size(); i += 2) {
            Point2f pt = Point2f(sts[i], sts[i + 1]);
            st[i / 2] = pt;
        }  // NOTE: CHANGED THIS -- MAY BREAK THINGS
        paramSet.AddPoint2f("uv", std::move(st), sts.size() / 2);
    }

    if (ps.size()) {
        std::unique_ptr<Point3f[]> P(new Point3f[ps.size() / 3]);
        for (int i = 0; i < ps.size(); i += 3) {
            Point3f pt = Point3f(ps[i], ps[i + 1], ps[i + 2]);
            P[i / 3] = pt;
        }
        paramSet.AddPoint3f("P", std::move(P), ps.size() / 3);
    }

    if (ns.size()) {
        std::unique_ptr<Normal3f[]> N(new Normal3f[ns.size() / 3]);
        for (int i = 0; i < ns.size(); i += 3) {
            Normal3f pt = Normal3f(ns[i], ns[i + 1], ns[i + 2]);
            N[i / 3] = pt;
        }
        paramSet.AddNormal3f("N", std::move(N), ns.size() / 3);
    }
}

void ExperimentBase::initializeShape(std::string shape,
                                     const std::vector<int>& inds,
                                     const std::vector<Float>& sts,
                                     const std::vector<Float>& ps,
                                     const std::vector<Float>& ns) {
    ParamSet shapeSet;

    if (inds.size()) {
        std::unique_ptr<int[]> indices(new int[inds.size()]);
        for (int i = 0; i < inds.size(); ++i) {
            indices[i] = inds[i];
        }
        shapeSet.AddInt("indices", std::move(indices), inds.size());
    }

    if (sts.size()) {
        std::unique_ptr<Point2f[]> st(new Point2f[sts.size() / 2]);
        for (int i = 0; i < sts.size(); i += 2) {
            Point2f pt = Point2f(sts[i], sts[i + 1]);
            st[i / 2] = pt;
        }
        shapeSet.AddPoint2f("st", std::move(st), sts.size() / 2);
    }

    if (ps.size()) {
        std::unique_ptr<Point3f[]> P(new Point3f[ps.size() / 3]);
        for (int i = 0; i < ps.size(); i += 3) {
            Point3f pt = Point3f(ps[i], ps[i + 1], ps[i + 2]);
            P[i / 3] = pt;
        }
        shapeSet.AddPoint3f("P", std::move(P), ps.size() / 3);
    }

    if (ns.size()) {
        std::unique_ptr<Normal3f[]> N(new Normal3f[ns.size() / 3]);
        for (int i = 0; i < ns.size(); i += 3) {
            Normal3f pt = Normal3f(ns[i], ns[i + 1], ns[i + 2]);
            N[i / 3] = pt;
        }
        shapeSet.AddNormal3f("N", std::move(N), ns.size() / 3);
    }

    pbrtShape(shape, shapeSet);
}

void ExperimentBase::initializeVDBMedium(
    std::string name, std::string medType, std::string trans, std::string maj,
    std::string min, std::string samp, std::vector<Float> sig_a,
    std::vector<Float> sig_s, std::vector<Float> minB, std::vector<Float> maxB,
    bool m_worldSpace, bool m_clampDensities, bool m_retargetDensities,
    bool m_retainAspectRatio, Float scale, std::string vdb_file) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> densityFunc(new std::string[1]);
    densityFunc[0] = "vdb";

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::unique_ptr<std::string[]> vdb_filename(new std::string[1]);
    vdb_filename[0] = exp_path + vdb_file;

    std::unique_ptr<bool[]> worldSpace(new bool[1]);
    worldSpace[0] = m_worldSpace;

    std::unique_ptr<bool[]> clampDensities(new bool[1]);
    clampDensities[0] = m_clampDensities;

    std::unique_ptr<bool[]> retargetDensities(new bool[1]);
    retargetDensities[0] = m_retargetDensities;

    std::unique_ptr<bool[]> retainAspectRatio(new bool[1]);
    retainAspectRatio[0] = m_retainAspectRatio;

    std::unique_ptr<Float[]> majScale(new Float[1]);
    majScale[0] = scale;

    ParamSet mediumSet;

    mediumSet.AddString("type", std::move(type), 1);
    mediumSet.AddString("densityFunc", std::move(densityFunc), 1);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddString("vdb_filename", std::move(vdb_filename), 1);
    mediumSet.AddBool("worldSpace", std::move(worldSpace), 1);
    mediumSet.AddBool("clampDensities", std::move(clampDensities), 1);
    mediumSet.AddBool("retargetDensities", std::move(retargetDensities), 1);
    mediumSet.AddBool("retainAspectRatio", std::move(retainAspectRatio), 1);
    mediumSet.AddFloat("majScale", std::move(majScale), 1);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeProcSphereMedium(
    std::string name, std::string medType, std::string trans, std::string maj,
    std::string min, std::string samp, std::vector<Float> sig_a,
    std::vector<Float> sig_s, std::vector<Float> minB, std::vector<Float> maxB,
    bool m_worldSpace, bool m_clampDensities, bool m_retargetDensities,
    bool m_retainAspectRatio, Float scale, std::string vdb_file) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> densityFunc(new std::string[1]);
    densityFunc[0] = "proc_sphere";

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::unique_ptr<std::string[]> vdb_filename(new std::string[1]);
    vdb_filename[0] = exp_path + vdb_file;

    std::unique_ptr<bool[]> worldSpace(new bool[1]);
    worldSpace[0] = m_worldSpace;

    std::unique_ptr<bool[]> clampDensities(new bool[1]);
    clampDensities[0] = m_clampDensities;

    std::unique_ptr<bool[]> retargetDensities(new bool[1]);
    retargetDensities[0] = m_retargetDensities;

    std::unique_ptr<bool[]> retainAspectRatio(new bool[1]);
    retainAspectRatio[0] = m_retainAspectRatio;

    std::unique_ptr<Float[]> majScale(new Float[1]);
    majScale[0] = scale;

    ParamSet mediumSet;

    mediumSet.AddString("type", std::move(type), 1);
    mediumSet.AddString("densityFunc", std::move(densityFunc), 1);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddString("vdb_filename", std::move(vdb_filename), 1);
    mediumSet.AddBool("worldSpace", std::move(worldSpace), 1);
    mediumSet.AddBool("clampDensities", std::move(clampDensities), 1);
    mediumSet.AddBool("retargetDensities", std::move(retargetDensities), 1);
    mediumSet.AddBool("retainAspectRatio", std::move(retainAspectRatio), 1);
    mediumSet.AddFloat("majScale", std::move(majScale), 1);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeSphereMedium(
    std::string name, std::string medType, std::string trans, std::string maj,
    std::string min, std::string samp, std::vector<Float> sig_a,
    std::vector<Float> sig_s, std::vector<Float> minB, std::vector<Float> maxB,
    bool m_worldSpace, bool m_clampDensities, bool m_retargetDensities,
    bool m_retainAspectRatio, Float scale, std::string vdb_file) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> densityFunc(new std::string[1]);
    densityFunc[0] = "sphere";

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::unique_ptr<std::string[]> vdb_filename(new std::string[1]);
    vdb_filename[0] = exp_path + vdb_file;

    std::unique_ptr<bool[]> worldSpace(new bool[1]);
    worldSpace[0] = m_worldSpace;

    std::unique_ptr<bool[]> clampDensities(new bool[1]);
    clampDensities[0] = m_clampDensities;

    std::unique_ptr<bool[]> retargetDensities(new bool[1]);
    retargetDensities[0] = m_retargetDensities;

    std::unique_ptr<bool[]> retainAspectRatio(new bool[1]);
    retainAspectRatio[0] = m_retainAspectRatio;

    std::unique_ptr<Float[]> majScale(new Float[1]);
    majScale[0] = scale;

    ParamSet mediumSet;

    mediumSet.AddString("type", std::move(type), 1);
    mediumSet.AddString("densityFunc", std::move(densityFunc), 1);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddString("vdb_filename", std::move(vdb_filename), 1);
    mediumSet.AddBool("worldSpace", std::move(worldSpace), 1);
    mediumSet.AddBool("clampDensities", std::move(clampDensities), 1);
    mediumSet.AddBool("retargetDensities", std::move(retargetDensities), 1);
    mediumSet.AddBool("retainAspectRatio", std::move(retainAspectRatio), 1);
    mediumSet.AddFloat("majScale", std::move(majScale), 1);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeDiskMedium(
    std::string name, std::string medType, std::string trans, std::string maj,
    std::string min, std::string samp, Float proc_time,
    std::vector<Float> sig_a, std::vector<Float> sig_s, std::vector<Float> minB,
    std::vector<Float> maxB, bool m_worldSpace, bool m_clampDensities,
    bool m_retargetDensities, bool m_retainAspectRatio, Float scale,
    std::string vdb_file) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> densityFunc(new std::string[1]);
    densityFunc[0] = "disk";

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::unique_ptr<std::string[]> vdb_filename(new std::string[1]);
    vdb_filename[0] = exp_path + vdb_file;

    std::unique_ptr<bool[]> worldSpace(new bool[1]);
    worldSpace[0] = m_worldSpace;

    std::unique_ptr<bool[]> clampDensities(new bool[1]);
    clampDensities[0] = m_clampDensities;

    std::unique_ptr<bool[]> retargetDensities(new bool[1]);
    retargetDensities[0] = m_retargetDensities;

    std::unique_ptr<bool[]> retainAspectRatio(new bool[1]);
    retainAspectRatio[0] = m_retainAspectRatio;

    std::unique_ptr<Float[]> majScale(new Float[1]);
    majScale[0] = scale;

    std::unique_ptr<Float[]> procTime(new Float[1]);
    procTime[0] = proc_time;

    ParamSet mediumSet;

    mediumSet.AddString("type", std::move(type), 1);
    mediumSet.AddString("densityFunc", std::move(densityFunc), 1);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddString("vdb_filename", std::move(vdb_filename), 1);
    mediumSet.AddBool("worldSpace", std::move(worldSpace), 1);
    mediumSet.AddBool("clampDensities", std::move(clampDensities), 1);
    mediumSet.AddBool("retargetDensities", std::move(retargetDensities), 1);
    mediumSet.AddBool("retainAspectRatio", std::move(retainAspectRatio), 1);
    mediumSet.AddFloat("majScale", std::move(majScale), 1);
    mediumSet.AddFloat("proc_time", std::move(procTime), 1);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeGridMedium(
    std::string name, std::string medType, std::string trans, std::string maj,
    std::string min, std::string samp, std::vector<Float> sig_a,
    std::vector<Float> sig_s, std::vector<Float> t_p0, std::vector<Float> t_p1,
    std::vector<Float> minB, std::vector<Float> maxB, std::string denseFile) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> densityFunc(new std::string[1]);
    densityFunc[0] = "grid";

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Point3f[]> p0(new Point3f[1]);
    p0[0] = Point3f(t_p0[0], t_p0[1], t_p0[2]);

    std::unique_ptr<Point3f[]> p1(new Point3f[1]);
    p1[0] = Point3f(t_p1[0], t_p1[1], t_p1[2]);

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::ifstream inputStream;
    inputStream.open(exp_path + denseFile);
    std::stringstream tokenStream;
    tokenStream << inputStream.rdbuf();

    std::string str;
    tokenStream >> str;
    std::unique_ptr<int[]> nx(new int[1]);
    nx[0] = stoi(str);

    tokenStream >> str;
    std::unique_ptr<int[]> ny(new int[1]);
    ny[0] = stoi(str);

    tokenStream >> str;
    std::unique_ptr<int[]> nz(new int[1]);
    nz[0] = stoi(str);

    int size = nx[0] * ny[0] * nz[0];
    int index = 0;

    std::unique_ptr<Float[]> density(new Float[size]);

    while (tokenStream >> str) {
        density[index++] = stod(str);

        if (index == size) break;
    }

    ParamSet mediumSet;

    mediumSet.AddString("type", std::move(type), 1);
    mediumSet.AddString("densityFunc", std::move(densityFunc), 1);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddInt("nx", std::move(nx), 1);
    mediumSet.AddInt("ny", std::move(ny), 1);
    mediumSet.AddInt("nz", std::move(nz), 1);
    mediumSet.AddPoint3f("p0", std::move(p0), 1);
    mediumSet.AddPoint3f("p1", std::move(p1), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddFloat("density", std::move(density), size);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeMedium(
    std::string name, std::string medType, std::string dense, std::string trans,
    std::string maj, std::string min, std::string samp,
    std::vector<Float> sig_a, std::vector<Float> sig_s, std::vector<Float> minB,
    std::vector<Float> maxB, Float scale) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> densityFunc(new std::string[1]);
    densityFunc[0] = dense;

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::unique_ptr<Float[]> majScale(new Float[1]);
    majScale[0] = scale;

    ParamSet mediumSet;
    mediumSet.AddString("type", std::move(type), 1);
    mediumSet.AddString("densityFunc", std::move(densityFunc), 1);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddFloat("majScale", std::move(majScale), 1);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeNonClassicalMedium(
    std::string name, std::string medType, std::string dense, std::string trans,
    std::string transType, std::string maj, std::string min, std::string samp,
    std::vector<Float> sig_a, std::vector<Float> sig_s, std::vector<Float> minB,
    std::vector<Float> maxB, Float scale) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> densityFunc(new std::string[1]);
    densityFunc[0] = dense;

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> transFunc(new std::string[1]);
    transFunc[0] = transType;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::unique_ptr<Float[]> majScale(new Float[1]);
    majScale[0] = scale;

    ParamSet mediumSet;
    mediumSet.AddString("type", std::move(type), 1);
    mediumSet.AddString("densityFunc", std::move(densityFunc), 1);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    mediumSet.AddString("transFunc", std::move(transFunc), 1);
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddFloat("majScale", std::move(majScale), 1);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeProceduralMedium(
    std::string name, std::string medType, std::string texName,
    std::string trans, std::string maj, std::string min, std::string samp,
    std::vector<Float> sig_a, std::vector<Float> sig_s, std::vector<Float> minB,
    std::vector<Float> maxB, std::vector<Float> scle, Float majorantScale) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Float[]> scale(new Float[3]);
    scale[0] = scle[0];
    scale[1] = scle[1];
    scale[2] = scle[2];

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::unique_ptr<Float[]> majScale(new Float[1]);
    majScale[0] = majorantScale;

    ParamSet mediumSet;
    mediumSet.AddString("type", std::move(type), 1);
    // mediumSet.AddTexture("procedural", texName);
    std::cout << "JERE" << std::endl;
    mediumSet.AddTexture("procedural", texName);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    std::cout << "HERE" << std::endl;
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddRGBSpectrum("scale", std::move(scale), 3);
    mediumSet.AddFloat("majScale", std::move(majScale), 1);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeMedium(
    std::string name, std::string medType, std::string dense, std::string trans,
    std::string maj, std::string min, std::string samp,
    std::vector<Float> sig_a, std::vector<Float> sig_s, std::vector<Float> minB,
    std::vector<Float> maxB, std::string densityFile, Point3f p0_val,
    Point3f p1_val, Float interp, Float fixed, int nx_val, int ny_val,
    int nz_val) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = medType;

    std::unique_ptr<std::string[]> densityFunc(new std::string[1]);
    densityFunc[0] = dense;

    std::unique_ptr<std::string[]> transEstFunc(new std::string[1]);
    transEstFunc[0] = trans;

    std::unique_ptr<std::string[]> majFunc(new std::string[1]);
    majFunc[0] = maj;

    std::unique_ptr<std::string[]> minFunc(new std::string[1]);
    minFunc[0] = min;

    std::unique_ptr<std::string[]> sampFunc(new std::string[1]);
    sampFunc[0] = samp;

    std::unique_ptr<Float[]> sigma_a(new Float[3]);
    sigma_a[0] = sig_a[0];
    sigma_a[1] = sig_a[1];
    sigma_a[2] = sig_a[2];

    std::unique_ptr<Float[]> sigma_s(new Float[3]);
    sigma_s[0] = sig_s[0];
    sigma_s[1] = sig_s[1];
    sigma_s[2] = sig_s[2];

    std::unique_ptr<Point3f[]> p0(new Point3f[1]);
    p0[0] = p0_val;

    std::unique_ptr<Point3f[]> p1(new Point3f[1]);
    p1[0] = p1_val;

    std::unique_ptr<int[]> nx(new int[1]);
    nx[0] = nx_val;

    std::unique_ptr<int[]> ny(new int[1]);
    ny[0] = ny_val;

    std::unique_ptr<int[]> nz(new int[1]);
    nz[0] = nz_val;

    std::unique_ptr<Float[]> v_interp(new Float[1]);
    v_interp[0] = interp;

    std::unique_ptr<Float[]> v_fixed(new Float[1]);
    v_fixed[0] = fixed;

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(minB[0], minB[1], minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(maxB[0], maxB[1], maxB[2]);

    std::unique_ptr<Float[]> density(new Float[nx[0] * ny[0] * nz[0]]);

    std::ifstream inputStream;
    inputStream.open(densityFile);
    std::stringstream tokenStream;
    tokenStream << inputStream.rdbuf();

    int size = nx[0] * ny[0] * nz[0];

    std::string str;
    int index = 0;
    while (tokenStream >> str) {
        density[index++] = stod(str);

        if (index == size) break;
    }

    ParamSet mediumSet;
    mediumSet.AddString("type", std::move(type), 1);
    mediumSet.AddString("densityFunc", std::move(densityFunc), 1);
    mediumSet.AddString("transEstFunc", std::move(transEstFunc), 1);
    mediumSet.AddString("majFunc", std::move(majFunc), 1);
    mediumSet.AddString("minFunc", std::move(minFunc), 1);
    mediumSet.AddString("sampFunc", std::move(sampFunc), 1);
    mediumSet.AddInt("nx", std::move(nx), 1);
    mediumSet.AddInt("ny", std::move(ny), 1);
    mediumSet.AddInt("nz", std::move(nz), 1);
    mediumSet.AddPoint3f("p0", std::move(p0), 1);
    mediumSet.AddPoint3f("p1", std::move(p1), 1);
    mediumSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    mediumSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    mediumSet.AddRGBSpectrum("sigma_a", std::move(sigma_a), 3);
    mediumSet.AddRGBSpectrum("sigma_s", std::move(sigma_s), 3);
    mediumSet.AddFloat("interp", std::move(v_interp), 1);
    mediumSet.AddFloat("fixed", std::move(v_fixed), 1);
    mediumSet.AddFloat("density", std::move(density), size);

    pbrtMakeNamedMedium(name, mediumSet);
}

void ExperimentBase::initializeInfiniteLightSource(std::string file,
                                                   std::vector<Float> scle) {
    std::unique_ptr<Float[]> scale(new Float[3]);
    scale[0] = scle[0];
    scale[1] = scle[1];
    scale[2] = scle[2];

    std::unique_ptr<std::string[]> mapname(new std::string[1]);
    mapname[0] = file;

    ParamSet lightSourceSet;
    lightSourceSet.AddRGBSpectrum("scale", std::move(scale), 3);
    lightSourceSet.AddString("mapname", std::move(mapname), 1);

    pbrtLightSource("infinite", lightSourceSet);
}

void ExperimentBase::initializeInfiniteLightSource(std::string file,
                                                   std::vector<Float> scle,
                                                   std::vector<Float> t_l) {
    std::unique_ptr<Float[]> scale(new Float[3]);
    scale[0] = scle[0];
    scale[1] = scle[1];
    scale[2] = scle[2];

    std::unique_ptr<Float[]> L(new Float[3]);
    L[0] = t_l[0];
    L[1] = t_l[1];
    L[2] = t_l[2];

    std::unique_ptr<std::string[]> mapname(new std::string[1]);
    mapname[0] = file;

    ParamSet lightSourceSet;
    lightSourceSet.AddRGBSpectrum("scale", std::move(scale), 3);
    lightSourceSet.AddRGBSpectrum("L", std::move(L), 3);
    lightSourceSet.AddString("mapname", std::move(mapname), 1);

    pbrtLightSource("infinite", lightSourceSet);
}

void ExperimentBase::initializeDistantLightSource(std::vector<Float> t_l,
                                                  Point3f t_from,
                                                  Point3f t_to) {
    std::unique_ptr<Float[]> scale(new Float[3]);
    scale[0] = 1.0;
    scale[1] = 1.0;
    scale[2] = 1.0;

    std::unique_ptr<Float[]> L(new Float[3]);
    L[0] = t_l[0];
    L[1] = t_l[1];
    L[2] = t_l[2];

    std::unique_ptr<Point3f[]> to(new Point3f[1]);
    to[0] = t_to;

    std::unique_ptr<Point3f[]> from(new Point3f[1]);
    from[0] = t_from;

    ParamSet lightSourceSet;
    lightSourceSet.AddRGBSpectrum("scale", std::move(scale), 3);
    lightSourceSet.AddRGBSpectrum("L", std::move(L), 3);
    lightSourceSet.AddPoint3f("to", std::move(to), 1);
    lightSourceSet.AddPoint3f("from", std::move(from), 1);

    pbrtLightSource("distant", lightSourceSet);
}

void ExperimentBase::initializePointLightSource(std::vector<Float> i_val) {
    std::unique_ptr<Float[]> I(new Float[3]);
    I[0] = i_val[0];
    I[1] = i_val[1];
    I[2] = i_val[2];

    ParamSet lightSourceSet;
    lightSourceSet.AddRGBSpectrum("I", std::move(I), 3);

    pbrtLightSource("point", lightSourceSet);
}

void ExperimentBase::initializePointCorPLightSource(
    std::vector<Float> i_val, std::vector<Float> dir_val) {
    std::unique_ptr<Float[]> I(new Float[3]);
    I[0] = i_val[0];
    I[1] = i_val[1];
    I[2] = i_val[2];

    std::unique_ptr<Vector3f[]> dir(new Vector3f[1]);
    dir[0] = Vector3f(dir_val[0], dir_val[1], dir_val[2]);
    // dir[1] = dir_val[1];
    // dir[2] = dir_val[2];

    ParamSet lightSourceSet;
    lightSourceSet.AddRGBSpectrum("I", std::move(I), 3);
    lightSourceSet.AddVector3f("dif_dir", std::move(dir), 1);

    pbrtLightSource("point_cor_p", lightSourceSet);
}

void ExperimentBase::initializeSpotLightSource(std::vector<Float> i_val,
                                               Point3f pfrom, Point3f pto,
                                               Float conea, Float conedelt) {
    std::unique_ptr<Float[]> I(new Float[3]);
    I[0] = i_val[0];
    I[1] = i_val[1];
    I[2] = i_val[2];

    std::unique_ptr<Float[]> coneangle(new Float[1]);
    coneangle[0] = conea;

    std::unique_ptr<Float[]> conedeltaangle(new Float[1]);
    conedeltaangle[0] = conedelt;

    std::unique_ptr<Point3f[]> from(new Point3f[1]);
    from[0] = pfrom;

    std::unique_ptr<Point3f[]> to(new Point3f[1]);
    to[0] = pto;

    ParamSet lightSourceSet;
    lightSourceSet.AddRGBSpectrum("I", std::move(I), 3);
    lightSourceSet.AddFloat("coneangle", std::move(coneangle), 1);
    lightSourceSet.AddFloat("conedeltaangle", std::move(conedeltaangle), 1);
    lightSourceSet.AddPoint3f("from", std::move(from), 1);
    lightSourceSet.AddPoint3f("to", std::move(to), 1);

    pbrtLightSource("spot", lightSourceSet);
}

void ExperimentBase::initializeMaterial_Matte(std::vector<Float> kd,
                                              Float sig) {
    std::unique_ptr<Float[]> Kd(new Float[3]);
    Kd[0] = kd[0];
    Kd[1] = kd[1];
    Kd[2] = kd[2];

    std::unique_ptr<Float[]> sigma(new Float[1]);
    sigma[0] = sig;

    ParamSet materialSet;
    materialSet.AddFloat("sigma", std::move(sigma), 1);
    materialSet.AddRGBSpectrum("Kd", std::move(Kd), 3);

    pbrtMaterial("matte", materialSet);
}

void ExperimentBase::initializeMaterial_Matte(std::string tex_kd, Float sig) {
    std::unique_ptr<Float[]> sigma(new Float[1]);
    sigma[0] = sig;

    ParamSet materialSet;
    materialSet.AddFloat("sigma", std::move(sigma), 1);
    materialSet.AddTexture("Kd", tex_kd);

    pbrtMaterial("matte", materialSet);
}

void ExperimentBase::initializeMaterial_ReflectiveMatte(std::vector<Float> kd,
                                                        std::vector<Float> kr) {
    // std::cout << "what" << std::endl;
    std::unique_ptr<Float[]> Kd(new Float[3]);
    Kd[0] = kd[0];
    Kd[1] = kd[1];
    Kd[2] = kd[2];

    std::unique_ptr<Float[]> Kr(new Float[3]);
    Kr[0] = kr[0];
    Kr[1] = kr[1];
    Kr[2] = kr[2];

    ParamSet materialSet;
    materialSet.AddRGBSpectrum("Kd", std::move(Kd), 3);
    materialSet.AddRGBSpectrum("Kr", std::move(Kr), 3);

    pbrtMaterial("refl_matte", materialSet);
}

void ExperimentBase::initializeMaterial_ReflectiveMatte(std::string kd,
                                                        std::vector<Float> kr) {
    std::unique_ptr<Float[]> Kr(new Float[3]);
    Kr[0] = kr[0];
    Kr[1] = kr[1];
    Kr[2] = kr[2];

    ParamSet materialSet;
    materialSet.AddTexture("Kd", kd);
    materialSet.AddRGBSpectrum("Kr", std::move(Kr), 3);

    pbrtMaterial("refl_matte", materialSet);
}

void ExperimentBase::initializeMaterial_Metal(std::string eta_spec,
                                              std::string k_spec, Float rough) {
    std::unique_ptr<Float[]> roughness(new Float[1]);
    roughness[0] = rough;

    const char* eta_str = eta_spec.c_str();
    const char* k_str = k_spec.c_str();

    ParamSet metalSet;
    metalSet.AddFloat("roughness", std::move(roughness), 1);
    metalSet.AddSampledSpectrumFiles("eta", &eta_str, 1);
    metalSet.AddSampledSpectrumFiles("k", &k_str, 1);

    pbrtMaterial("metal", metalSet);
}

void ExperimentBase::initializeNamedMaterial_Matte(std::string name,
                                                   std::vector<Float> kd,
                                                   Float sig) {
    std::unique_ptr<Float[]> Kd(new Float[3]);
    Kd[0] = kd[0];
    Kd[1] = kd[1];
    Kd[2] = kd[2];

    std::unique_ptr<Float[]> sigma(new Float[1]);
    sigma[0] = sig;

    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "matte";

    ParamSet materialSet;
    materialSet.AddFloat("sigma", std::move(sigma), 1);
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddRGBSpectrum("Kd", std::move(Kd), 3);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeNamedMaterial_Matte(std::string name,
                                                   std::string tex_kd,
                                                   Float sig) {
    std::unique_ptr<Float[]> sigma(new Float[1]);
    sigma[0] = sig;

    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "matte";

    ParamSet materialSet;
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddFloat("sigma", std::move(sigma), 1);
    materialSet.AddTexture("Kd", tex_kd);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeNamedMaterial_Substrate(
    std::string name, std::vector<Float> ks, std::string tex_kd, Float u_rough,
    Float v_rough, bool remap) {
    std::unique_ptr<Float[]> Ks(new Float[3]);
    Ks[0] = ks[0];
    Ks[1] = ks[1];
    Ks[2] = ks[2];

    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "substrate";

    std::unique_ptr<Float[]> uroughness(new Float[1]);
    uroughness[0] = u_rough;

    std::unique_ptr<Float[]> vroughness(new Float[1]);
    vroughness[0] = v_rough;

    std::unique_ptr<bool[]> remaproughness(new bool[1]);
    remaproughness[0] = remap;

    ParamSet materialSet;
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddRGBSpectrum("Ks", std::move(Ks), 3);
    materialSet.AddTexture("Kd", tex_kd);
    materialSet.AddFloat("uroughness", std::move(uroughness), 1);
    materialSet.AddFloat("vroughness", std::move(vroughness), 1);
    materialSet.AddBool("remaproughness", std::move(remaproughness), 1);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeNamedMaterial_Mirror(std::string name) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "mirror";

    ParamSet materialSet;
    materialSet.AddString("type", std::move(type), 1);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeNamedMaterial_Metal(std::string name,
                                                   std::vector<Float> eta_spec,
                                                   std::vector<Float> k_spec,
                                                   float u_rough, float v_rough,
                                                   bool remap) {
    std::unique_ptr<Float[]> eta(new Float[3]);
    eta[0] = eta_spec[0];
    eta[1] = eta_spec[1];
    eta[2] = eta_spec[2];

    std::unique_ptr<Float[]> k(new Float[3]);
    k[0] = k_spec[0];
    k[1] = k_spec[1];
    k[2] = k_spec[2];

    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "metal";

    std::unique_ptr<Float[]> uroughness(new Float[1]);
    uroughness[0] = u_rough;

    std::unique_ptr<Float[]> vroughness(new Float[1]);
    vroughness[0] = v_rough;

    std::unique_ptr<bool[]> remaproughness(new bool[1]);
    remaproughness[0] = remap;

    ParamSet materialSet;
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddRGBSpectrum("eta", std::move(eta), 3);
    materialSet.AddRGBSpectrum("k", std::move(k), 3);
    materialSet.AddFloat("uroughness", std::move(uroughness), 1);
    materialSet.AddFloat("vroughness", std::move(vroughness), 1);
    materialSet.AddBool("remaproughness", std::move(remaproughness), 1);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeNamedMaterial_Glass(std::string name,
                                                   float index_val,
                                                   float u_rough, float v_rough,
                                                   bool remap) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "glass";

    std::unique_ptr<Float[]> index(new Float[1]);
    index[0] = index_val;

    std::unique_ptr<Float[]> uroughness(new Float[1]);
    uroughness[0] = u_rough;

    std::unique_ptr<Float[]> vroughness(new Float[1]);
    vroughness[0] = v_rough;

    std::unique_ptr<bool[]> remaproughness(new bool[1]);
    remaproughness[0] = remap;

    ParamSet materialSet;
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddFloat("index", std::move(index), 3);
    materialSet.AddFloat("uroughness", std::move(uroughness), 1);
    materialSet.AddFloat("vroughness", std::move(vroughness), 1);
    materialSet.AddBool("remaproughness", std::move(remaproughness), 1);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeNamedMaterial_Glass(std::string name,
                                                   float index_val,
                                                   bool remap) {
    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "glass";

    std::unique_ptr<Float[]> index(new Float[1]);
    index[0] = index_val;

    std::unique_ptr<bool[]> remaproughness(new bool[1]);
    remaproughness[0] = remap;

    ParamSet materialSet;
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddFloat("index", std::move(index), 3);
    materialSet.AddBool("remaproughness", std::move(remaproughness), 1);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeNamedMaterial_Metal(std::string name,
                                                   std::string eta_spec,
                                                   std::string k_spec,
                                                   Float rough) {
    std::unique_ptr<Float[]> roughness(new Float[1]);
    roughness[0] = rough;

    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "metal";

    const char* eta_str = eta_spec.c_str();
    const char* k_str = k_spec.c_str();

    ParamSet metalSet;
    metalSet.AddFloat("roughness", std::move(roughness), 1);
    metalSet.AddString("type", std::move(type), 1);
    metalSet.AddSampledSpectrumFiles("eta", &eta_str, 1);
    metalSet.AddSampledSpectrumFiles("k", &k_str, 1);

    pbrtMakeNamedMaterial(name, metalSet);
}

void ExperimentBase::initializeTextureImg(std::string name, std::string type,
                                          std::string texname,
                                          std::string imagepath) {
    std::unique_ptr<std::string[]> filename(new std::string[1]);
    filename[0] = imagepath;

    ParamSet textureSet;
    textureSet.AddString("filename", std::move(filename), 1);

    pbrtTexture(name, type, texname, textureSet);
}

void ExperimentBase::initializeTextureImg(std::string name, std::string type,
                                          std::string texname,
                                          std::string imagepath, bool tri) {
    std::unique_ptr<std::string[]> filename(new std::string[1]);
    filename[0] = imagepath;

    std::unique_ptr<bool[]> trilinear(new bool[1]);
    trilinear[0] = tri;

    ParamSet textureSet;
    textureSet.AddString("filename", std::move(filename), 1);
    textureSet.AddBool("trilinear", std::move(trilinear), 1);

    pbrtTexture(name, type, texname, textureSet);
}

void ExperimentBase::initializeNamedMaterial_Uber(
    std::string name, std::vector<Float> ks, std::vector<Float> kd,
    std::vector<Float> opacity_val) {
    std::unique_ptr<Float[]> Ks(new Float[3]);
    Ks[0] = ks[0];
    Ks[1] = ks[1];
    Ks[2] = ks[2];

    std::unique_ptr<Float[]> Kd(new Float[3]);
    Kd[0] = kd[0];
    Kd[1] = kd[1];
    Kd[2] = kd[2];

    std::unique_ptr<Float[]> opacity(new Float[3]);
    opacity[0] = opacity_val[0];
    opacity[1] = opacity_val[1];
    opacity[2] = opacity_val[2];

    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = "uber";

    ParamSet materialSet;
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddRGBSpectrum("Ks", std::move(Ks), 3);
    materialSet.AddRGBSpectrum("Kd", std::move(Kd), 3);
    materialSet.AddRGBSpectrum("opacity", std::move(opacity), 3);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeScaleTexture(std::string name, std::string type,
                                            std::string texname,
                                            std::string tex1_path,
                                            std::vector<Float> tex2_val) {
    std::unique_ptr<Float[]> rgb(new Float[3]);
    rgb[0] = tex2_val[0];
    rgb[1] = tex2_val[1];
    rgb[2] = tex2_val[2];

    ParamSet textureSet;
    textureSet.AddTexture("tex1", tex1_path);
    textureSet.AddRGBSpectrum("tex2", std::move(rgb), 3);

    pbrtTexture(name, type, texname, textureSet);
}

void ExperimentBase::initializeNamedMaterial(std::string name, Float rough,
                                             Float ind, std::string typ,
                                             std::vector<Float> kd,
                                             std::vector<Float> ks,
                                             std::vector<Float> kt,
                                             std::vector<Float> opac) {
    std::unique_ptr<Float[]> roughness(new Float[1]);
    roughness[0] = rough;

    std::unique_ptr<Float[]> index(new Float[1]);
    index[0] = ind;

    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = typ;

    std::unique_ptr<Float[]> Kd(new Float[3]);
    Kd[0] = kd[0];
    Kd[1] = kd[1];
    Kd[2] = kd[2];

    std::unique_ptr<Float[]> Ks(new Float[3]);
    Ks[0] = ks[0];
    Ks[1] = ks[1];
    Ks[2] = ks[2];

    std::unique_ptr<Float[]> Kt(new Float[3]);
    Kt[0] = kt[0];
    Kt[1] = kt[1];
    Kt[2] = kt[2];

    std::unique_ptr<Float[]> opacity(new Float[3]);
    opacity[0] = opac[0];
    opacity[1] = opac[1];
    opacity[2] = opac[2];

    ParamSet materialSet;
    materialSet.AddFloat("roughness", std::move(roughness), 1);
    materialSet.AddFloat("index", std::move(index), 1);
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddRGBSpectrum("Kd", std::move(Kd), 3);
    materialSet.AddRGBSpectrum("Ks", std::move(Ks), 3);
    materialSet.AddRGBSpectrum("Kt", std::move(Kt), 3);
    materialSet.AddRGBSpectrum("opacity", std::move(opacity), 3);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeNamedMaterial(std::string name, Float rough,
                                             Float ind, std::string typ,
                                             std::string kd_tex,
                                             std::vector<Float> ks,
                                             std::vector<Float> kt,
                                             std::vector<Float> opac) {
    std::unique_ptr<Float[]> roughness(new Float[1]);
    roughness[0] = rough;

    std::unique_ptr<Float[]> index(new Float[1]);
    index[0] = ind;

    std::unique_ptr<std::string[]> type(new std::string[1]);
    type[0] = typ;

    std::unique_ptr<Float[]> Ks(new Float[3]);
    Ks[0] = ks[0];
    Ks[1] = ks[1];
    Ks[2] = ks[2];

    std::unique_ptr<Float[]> Kt(new Float[3]);
    Kt[0] = kt[0];
    Kt[1] = kt[1];
    Kt[2] = kt[2];

    std::unique_ptr<Float[]> opacity(new Float[3]);
    opacity[0] = opac[0];
    opacity[1] = opac[1];
    opacity[2] = opac[2];

    ParamSet materialSet;
    materialSet.AddFloat("roughness", std::move(roughness), 1);
    materialSet.AddFloat("index", std::move(index), 1);
    materialSet.AddString("type", std::move(type), 1);
    materialSet.AddTexture("Kd", kd_tex);
    materialSet.AddRGBSpectrum("Ks", std::move(Ks), 3);
    materialSet.AddRGBSpectrum("Kt", std::move(Kt), 3);
    materialSet.AddRGBSpectrum("opacity", std::move(opacity), 3);

    pbrtMakeNamedMaterial(name, materialSet);
}

void ExperimentBase::initializeDiffuseAreaLightSource(std::vector<Float> col) {
    std::unique_ptr<Float[]> L(new Float[3]);
    L[0] = col[0];
    L[1] = col[1];
    L[2] = col[2];

    ParamSet lightSet;
    lightSet.AddRGBSpectrum("L", std::move(L), 3);

    pbrtAreaLightSource("diffuse", lightSet);
}

void ExperimentBase::initializeDistantLightSource(std::vector<Float> col) {
    std::unique_ptr<Float[]> L(new Float[3]);
    L[0] = col[0];
    L[1] = col[1];
    L[2] = col[2];

    ParamSet lightSet;
    lightSet.AddRGBSpectrum("L", std::move(L), 3);

    pbrtAreaLightSource("distant", lightSet);
}

void ExperimentBase::initializeGlass(std::string r, std::string t, Float ind) {
    std::unique_ptr<Float[]> index(new Float[1]);
    index[0] = ind;

    ParamSet glassSet;

    glassSet.AddTexture("Kt", t);
    glassSet.AddTexture("Kr", r);
    glassSet.AddFloat("index", std::move(index), 1);

    pbrtMaterial("glass", glassSet);
}

void ExperimentBase::initializeGlass(std::string r, Float ind) {
    std::unique_ptr<Float[]> Kt(new Float[3]);
    Kt[0] = 1.0;
    Kt[1] = 1.0;
    Kt[2] = 1.0;

    std::unique_ptr<Float[]> index(new Float[1]);
    index[0] = ind;

    ParamSet glassSet;

    glassSet.AddRGBSpectrum("Kt", std::move(Kt), 3);
    glassSet.AddTexture("Kr", r);
    glassSet.AddFloat("index", std::move(index), 1);

    pbrtMaterial("glass", glassSet);
}

void ExperimentBase::initializeGlass(std::vector<Float> r, std::vector<Float> t,
                                     Float ind) {
    std::unique_ptr<Float[]> Kt(new Float[3]);
    Kt[0] = t[0];
    Kt[1] = t[1];
    Kt[2] = t[2];

    std::unique_ptr<Float[]> Kr(new Float[3]);
    Kr[0] = r[0];
    Kr[1] = r[1];
    Kr[2] = r[2];

    std::unique_ptr<Float[]> index(new Float[1]);
    index[0] = ind;

    ParamSet glassSet;

    glassSet.AddRGBSpectrum("Kt", std::move(Kt), 3);
    glassSet.AddRGBSpectrum("Kr", std::move(Kr), 3);
    glassSet.AddFloat("index", std::move(index), 1);

    pbrtMaterial("glass", glassSet);
}

void ExperimentBase::initializeHackyGlass(std::vector<Float> r,
                                          std::vector<Float> t, Float ind) {
    std::unique_ptr<Float[]> Kt(new Float[3]);
    Kt[0] = t[0];
    Kt[1] = t[1];
    Kt[2] = t[2];

    std::unique_ptr<Float[]> Kr(new Float[3]);
    Kr[0] = r[0];
    Kr[1] = r[1];
    Kr[2] = r[2];

    std::unique_ptr<Float[]> index(new Float[1]);
    index[0] = ind;

    ParamSet glassSet;

    glassSet.AddRGBSpectrum("Kt", std::move(Kt), 3);
    glassSet.AddRGBSpectrum("Kr", std::move(Kr), 3);
    glassSet.AddFloat("index", std::move(index), 1);

    pbrtMaterial("glass_hack", glassSet);
}

void ExperimentBase::initializeVoronoiSpiralTexture(
    std::string name, std::vector<Float> t_levelColors,
    std::vector<Float> t_amplitudes, std::vector<Float> t_phases,
    std::vector<Float> t_zPeriod, std::vector<Float> t_noisePeriod,
    std::vector<Float> t_noiseAmp, std::vector<Float> t_minB,
    std::vector<Float> t_maxB, Float t_radius, int t_levelPoints) {
    std::unique_ptr<Float[]> levelColors(new Float[t_levelColors.size()]);
    for (int i = 0; i < t_levelColors.size(); ++i) {
        levelColors[i] = t_levelColors[i];
    }

    std::unique_ptr<Float[]> amplitudes(new Float[t_amplitudes.size()]);
    for (int i = 0; i < t_amplitudes.size(); ++i) {
        amplitudes[i] = t_amplitudes[i];
    }

    std::unique_ptr<Float[]> phases(new Float[t_phases.size()]);
    for (int i = 0; i < t_phases.size(); ++i) {
        phases[i] = t_phases[i];
    }

    std::unique_ptr<Float[]> zPeriod(new Float[t_zPeriod.size()]);
    for (int i = 0; i < t_zPeriod.size(); ++i) {
        zPeriod[i] = t_zPeriod[i];
    }

    std::unique_ptr<Float[]> noisePeriod(new Float[t_noisePeriod.size()]);
    for (int i = 0; i < t_noisePeriod.size(); ++i) {
        noisePeriod[i] = t_noisePeriod[i];
    }

    std::unique_ptr<Float[]> noiseAmp(new Float[t_noiseAmp.size()]);
    for (int i = 0; i < t_noiseAmp.size(); ++i) {
        noiseAmp[i] = t_noiseAmp[i];
    }

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(t_minB[0], t_minB[1], t_minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(t_maxB[0], t_maxB[1], t_maxB[2]);

    std::unique_ptr<Float[]> radius(new Float[1]);
    radius[0] = t_radius;

    std::unique_ptr<int[]> levelPoints(new int[1]);
    levelPoints[0] = t_levelPoints;

    ParamSet textureParams;

    textureParams.AddRGBSpectrum("levelColors", std::move(levelColors),
                                 t_levelColors.size());
    textureParams.AddRGBSpectrum("noiseAmp", std::move(noiseAmp), 3);
    textureParams.AddRGBSpectrum("noisePeriod", std::move(noisePeriod), 3);
    textureParams.AddFloat("amplitudes", std::move(amplitudes),
                           t_amplitudes.size());
    textureParams.AddFloat("phases", std::move(phases), t_phases.size());
    textureParams.AddFloat("zPeriod", std::move(zPeriod), t_zPeriod.size());
    textureParams.AddPoint3f("minBounds", std::move(minBounds), 1);
    textureParams.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    textureParams.AddFloat("radius", std::move(radius), 1);
    textureParams.AddInt("levelPoints", std::move(levelPoints), 1);

    pbrtTexture(name, "spectrum", "voronoi_spiral", textureParams);
}

void ExperimentBase::initializePointCloudMedium(std::string name,
                                                std::vector<Float> points,
                                                std::vector<Float> densities,
                                                std::vector<Float> t_minB,
                                                std::vector<Float> t_maxB) {
    std::unique_ptr<Float[]> colors(new Float[densities.size()]);
    for (int i = 0; i < densities.size(); ++i) {
        colors[i] = densities[i];
    }

    std::unique_ptr<Point3f[]> locations(new Point3f[points.size() / 3]);
    for (int i = 0; i < points.size() / 3; ++i) {
        locations[i] =
            Point3f(points[i * 3], points[i * 3 + 1], points[i * 3 + 2]);
    }

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(t_minB[0], t_minB[1], t_minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(t_maxB[0], t_maxB[1], t_maxB[2]);

    ParamSet textureParams;

    textureParams.AddRGBSpectrum("densities", std::move(colors),
                                 densities.size());
    textureParams.AddPoint3f("locations", std::move(locations),
                             points.size() / 3);
    textureParams.AddPoint3f("minBounds", std::move(minBounds), 1);
    textureParams.AddPoint3f("maxBounds", std::move(maxBounds), 1);

    pbrtTexture(name, "spectrum", "point_interp", textureParams);
}

void ExperimentBase::initializeDeintegratedTextureMedium(
    std::string name, std::string file, std::vector<Float> t_minB,
    std::vector<Float> t_maxB, bool t_along_z) {
    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(t_minB[0], t_minB[1], t_minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(t_maxB[0], t_maxB[1], t_maxB[2]);

    std::unique_ptr<std::string[]> file_name(new std::string[1]);
    file_name[0] = file;

    std::unique_ptr<bool[]> along_z(new bool[1]);
    along_z[0] = t_along_z;

    ParamSet textureParams;

    textureParams.AddString("filename", std::move(file_name), 1);
    textureParams.AddPoint3f("minBounds", std::move(minBounds), 1);
    textureParams.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    textureParams.AddBool("along_z", std::move(along_z), 1);

    pbrtTexture(name, "spectrum", "deintegrated_texture", textureParams);
}

void ExperimentBase::initializeChessTextureMedium(std::string name,
                                                  std::string file,
                                                  std::vector<Float> t_minB,
                                                  std::vector<Float> t_maxB) {
    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(t_minB[0], t_minB[1], t_minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(t_maxB[0], t_maxB[1], t_maxB[2]);

    std::unique_ptr<std::string[]> file_name(new std::string[1]);
    file_name[0] = file;

    ParamSet textureParams;

    textureParams.AddString("filename", std::move(file_name), 1);
    textureParams.AddPoint3f("minBounds", std::move(minBounds), 1);
    textureParams.AddPoint3f("maxBounds", std::move(maxBounds), 1);

    pbrtTexture(name, "spectrum", "chess_texture", textureParams);
}

void ExperimentBase::initializeChessTextureMedium(
    std::string name, std::string file, std::vector<Float> t_minB,
    std::vector<Float> t_maxB, Float t_hue, Float t_sat, Float t_turb_period,
    Float t_xtrans, Float t_ytrans, Float t_ztrans, Float t_g_const,
    Float t_x_const, Float t_t_const, Float t_phase, Float t_power) {
    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(t_minB[0], t_minB[1], t_minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(t_maxB[0], t_maxB[1], t_maxB[2]);

    std::unique_ptr<std::string[]> file_name(new std::string[1]);
    file_name[0] = file;

    std::unique_ptr<Float[]> hue(new Float[1]);
    hue[0] = t_hue;

    std::unique_ptr<Float[]> sat(new Float[1]);
    sat[0] = t_sat;

    std::unique_ptr<Float[]> turb_period(new Float[1]);
    turb_period[0] = t_turb_period;

    std::unique_ptr<Float[]> xtrans(new Float[1]);
    xtrans[0] = t_xtrans;

    std::unique_ptr<Float[]> ytrans(new Float[1]);
    ytrans[0] = t_ytrans;

    std::unique_ptr<Float[]> ztrans(new Float[1]);
    ztrans[0] = t_ztrans;

    std::unique_ptr<Float[]> g_const(new Float[1]);
    g_const[0] = t_g_const;

    std::unique_ptr<Float[]> x_const(new Float[1]);
    x_const[0] = t_x_const;

    std::unique_ptr<Float[]> t_const(new Float[1]);
    t_const[0] = t_t_const;

    std::unique_ptr<Float[]> phase(new Float[1]);
    phase[0] = t_phase;

    std::unique_ptr<Float[]> power(new Float[1]);
    power[0] = t_power;

    ParamSet textureParams;

    textureParams.AddString("filename", std::move(file_name), 1);
    textureParams.AddPoint3f("minBounds", std::move(minBounds), 1);
    textureParams.AddPoint3f("maxBounds", std::move(maxBounds), 1);
    textureParams.AddFloat("hue", std::move(hue), 1);
    textureParams.AddFloat("sat", std::move(sat), 1);
    textureParams.AddFloat("turb_period", std::move(turb_period), 1);
    textureParams.AddFloat("xtrans", std::move(xtrans), 1);
    textureParams.AddFloat("ytrans", std::move(ytrans), 1);
    textureParams.AddFloat("ztrans", std::move(ztrans), 1);
    textureParams.AddFloat("g_const", std::move(g_const), 1);
    textureParams.AddFloat("x_const", std::move(x_const), 1);
    textureParams.AddFloat("t_const", std::move(t_const), 1);
    textureParams.AddFloat("phase", std::move(phase), 1);
    textureParams.AddFloat("power", std::move(power), 1);

    pbrtTexture(name, "spectrum", "chess_texture", textureParams);
}

void ExperimentBase::initializeCloudTexture(
    std::string name, Float t_period, Float t_resolution, Float t_hardCodedMax,
    Float t_hardCodedMin, Float t_distPeriod, Float t_distFactor,
    Float t_threshStart, Float t_threshSlope, std::vector<Float> t_minB,
    std::vector<Float> t_maxB) {
    std::unique_ptr<Float[]> period(new Float[1]);
    period[0] = t_period;

    std::unique_ptr<Float[]> resolution(new Float[1]);
    resolution[0] = t_resolution;

    std::unique_ptr<Float[]> hardCodedMax(new Float[1]);
    hardCodedMax[0] = t_hardCodedMax;

    std::unique_ptr<Float[]> hardCodedMin(new Float[1]);
    hardCodedMin[0] = t_hardCodedMin;

    std::unique_ptr<Float[]> distPeriod(new Float[1]);
    distPeriod[0] = t_distPeriod;

    std::unique_ptr<Float[]> distFactor(new Float[1]);
    distFactor[0] = t_distFactor;

    std::unique_ptr<Float[]> threshStart(new Float[1]);
    threshStart[0] = t_threshStart;

    std::unique_ptr<Float[]> threshSlope(new Float[1]);
    threshSlope[0] = t_threshSlope;

    std::unique_ptr<Point3f[]> minBounds(new Point3f[1]);
    minBounds[0] = Point3f(t_minB[0], t_minB[1], t_minB[2]);

    std::unique_ptr<Point3f[]> maxBounds(new Point3f[1]);
    maxBounds[0] = Point3f(t_maxB[0], t_maxB[1], t_maxB[2]);

    ParamSet cloudSet;
    cloudSet.AddFloat("period", std::move(period), 1);
    cloudSet.AddFloat("resolution", std::move(resolution), 1);
    cloudSet.AddFloat("hardCodedMin", std::move(hardCodedMin), 1);
    cloudSet.AddFloat("hardCodedMax", std::move(hardCodedMax), 1);
    cloudSet.AddFloat("distPeriod", std::move(distPeriod), 1);
    cloudSet.AddFloat("distFactor", std::move(distFactor), 1);
    cloudSet.AddFloat("threshStart", std::move(threshStart), 1);
    cloudSet.AddFloat("threshSlope", std::move(threshSlope), 1);
    cloudSet.AddPoint3f("minBounds", std::move(minBounds), 1);
    cloudSet.AddPoint3f("maxBounds", std::move(maxBounds), 1);

    pbrtTexture(name, "spectrum", "cloud", cloudSet);
}

void ExperimentBase::initializePlyShape(std::string path) {
    std::unique_ptr<std::string[]> filename(new std::string[1]);
    filename[0] = path;

    ParamSet shapeSet;
    shapeSet.AddString("filename", std::move(filename), 1);

    pbrtShape("plymesh", shapeSet);
}

void ExperimentBase::initializeSphereShape(Float t_radius) {
    std::unique_ptr<Float[]> radius(new Float[1]);
    radius[0] = t_radius;

    ParamSet shapeSet;
    shapeSet.AddFloat("radius", std::move(radius), 1);

    pbrtShape("sphere", shapeSet);
}

void ExperimentBase::initializeBoxShape(Float minx, Float maxx, Float miny,
                                        Float maxy, Float minz, Float maxz) {
    std::unique_ptr<int[]> indices(new int[36]);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 1;
    indices[5] = 3;

    indices[6] = 2;
    indices[7] = 3;
    indices[8] = 4;
    indices[9] = 4;
    indices[10] = 3;
    indices[11] = 5;

    indices[12] = 4;
    indices[13] = 5;
    indices[14] = 6;
    indices[15] = 6;
    indices[16] = 5;
    indices[17] = 7;

    indices[18] = 6;
    indices[19] = 7;
    indices[20] = 0;
    indices[21] = 0;
    indices[22] = 7;
    indices[23] = 1;

    indices[24] = 1;
    indices[25] = 7;
    indices[26] = 3;
    indices[27] = 3;
    indices[28] = 7;
    indices[29] = 5;

    indices[30] = 6;
    indices[31] = 0;
    indices[32] = 4;
    indices[33] = 4;
    indices[34] = 0;
    indices[35] = 2;

    std::unique_ptr<Point3f[]> P(new Point3f[8]);
    P[0] = Point3f(minx, miny, maxz);
    P[1] = Point3f(maxx, miny, maxz);
    P[2] = Point3f(minx, maxy, maxz);
    P[3] = Point3f(maxx, maxy, maxz);
    P[4] = Point3f(minx, maxy, minz);
    P[5] = Point3f(maxx, maxy, minz);
    P[6] = Point3f(minx, miny, minz);
    P[7] = Point3f(maxx, miny, minz);

    ParamSet shapeSet;

    shapeSet.AddInt("indices", std::move(indices), 36);
    shapeSet.AddPoint3f("P", std::move(P), 8);

    pbrtShape("trianglemesh", shapeSet);
}

void ExperimentBase::compareRMSE(std::string pathToGroundTruth,
                                 std::vector<std::string>& imagePaths) {
    // imedit::Image<Float> gtImage = imedit::Image<Float>(pathToGroundTruth);
    //
    // for (int i = 0; i < imagePaths.size(); ++i)
    // {
    //     imedit::Image<Float> testImage = imedit::Image<Float>(imagePaths[i]);
    //
    //     // TODO
    // }
}
