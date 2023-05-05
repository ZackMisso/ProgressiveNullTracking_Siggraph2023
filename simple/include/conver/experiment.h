#pragma once

#include <conver/conver.h>
#include <conver/technique.h>

class Experiment
{
public:
    virtual void run() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDataPath() const = 0;
    virtual void preProcess() = 0;

    virtual void run(int test) const
    {
        run();
    }
};

class ExponentialJansPlots : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment_noise(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_const(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_scos(pcg32 &rng,
                             std::vector<Technique<Float> *> techniques,
                             std::string prefix) const;
    void run_experiment_gauss(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    virtual std::string getName() const { return "exp_jans"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class ExponentialJansNonBoundPlots : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment_noise(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_const(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_scos(pcg32 &rng,
                             std::vector<Technique<Float> *> techniques,
                             std::string prefix) const;
    void run_experiment_gauss(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    virtual std::string getName() const { return "exp_nonbound_jans"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class ExponentialMultiJansPlots : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment_noise(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_const(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_scos(pcg32 &rng,
                             std::vector<Technique<Float> *> techniques,
                             std::string prefix) const;
    void run_experiment_gauss(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void finalize_scos(std::vector<Technique<Float> *> techniques,
                       std::string prefix) const;
    virtual std::string getName() const { return "exp_multi_jans"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class ExponentialNonBoundVariance : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment_noise(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_const(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_scos(pcg32 &rng,
                             std::vector<Technique<Float> *> techniques,
                             std::string prefix) const;
    void run_experiment_gauss(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    virtual std::string getName() const { return "exp_nonbound_var"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class ExponentialNonBoundBiasVsNoise : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment_noise(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_const(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    void run_experiment_scos(pcg32 &rng,
                             std::vector<Technique<Float> *> techniques,
                             std::string prefix) const;
    void run_experiment_gauss(pcg32 &rng,
                              std::vector<Technique<Float> *> techniques,
                              std::string prefix) const;
    virtual std::string getName() const { return "exp_nonbound_bias_vs_noise"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class Figure_2 : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment(pcg32 &rng,
                        std::string prefix) const;
    virtual std::string getName() const { return "figure_2"; }
    virtual std::string getDataPath() const
    {
        return "../../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class ExponentialBiasExt : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment(pcg32 &rng,
                        std::string prefix) const;
    virtual std::string getName() const { return "exp_bias_ext"; }
    virtual std::string getDataPath() const
    {
        return "../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};

class Figure_8 : public Experiment
{
public:
    virtual void run() const;
    virtual void run(int test) const;
    void run_experiment(pcg32 &rng,
                        std::string prefix) const;
    void run_experiment_for_extinction(
        pcg32 &rng,
        std::string prefix,
        Function *function) const;
    virtual std::string getName() const { return "figure_9"; }
    virtual std::string getDataPath() const
    {
        return "../../results/" + getName() + "/";
    }
    virtual void preProcess()
    {
        std::string cmd = "mkdir " + getDataPath();
        system(cmd.c_str());
    }
};
