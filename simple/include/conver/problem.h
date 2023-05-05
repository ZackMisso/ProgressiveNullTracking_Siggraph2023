#pragma once

#include <conver/conver.h>
#include <conver/bounds.h>
#include <conver/simple_funcs.h>

template <typename T>
struct Problem
{
public:
    // these methods evaluate the problem given integral evaluations
    virtual T eval(Float sample) const = 0;
    virtual T eval(const std::vector<Float> &samples) const { return T(0); }
    virtual T eval_function(Float sample) const { return T(0); }
    virtual T eval_function_variance(Float sample) const { return T(0); }
    // these methods just evaluate the integrals with no non-linear modification
    virtual T eval_integral(Float sample) const { return T(0); }
    virtual T eval_integral(const std::vector<Float> &samples) const { return T(0); }
    // this is the expected value of the non-linearly modified integral
    virtual T expected() const = 0;
    // this is the expected value of the integral
    virtual T expected_integral() const { return T(0); }

    virtual T pre_sample(pcg32 &rng) { return T(0); }

    virtual T eval(Float sample, Float min) const
    {
        return eval(sample);
    }

    virtual T eval(const std::vector<Float> &samples, Float min) const
    {
        return eval(samples);
    }

    virtual T eval_rep_if_zero(Float sample, Float min) const
    {
        return eval(sample);
    }

    virtual T eval_rep_if_zero(const std::vector<Float> &samples, Float min) const
    {
        return eval(samples);
    }

    virtual Float eval_rot(Float sample, Float epsilon, int m) const
    {
        return eval(sample);
    }

    virtual Float eval_rot(const std::vector<Float> &samples) const
    {
        return eval(samples);
    }

    virtual T compute_function_max() const
    {
        return T(0.0);
    }

    virtual T get_variable(int val) const
    {
        std::cout << "ERROR: THIS SHOULD NEVER BE CALLED: non-override get_variable" << std::endl;
        return T(0.0);
    }

    Bounds<T> *getBounds() const { return bounds; }
    virtual std::string getName() const = 0;

protected:
    Bounds<T> *bounds;
};

struct ExpTrans : public Problem<Float>
{
    ExpTrans(Function *func);
    ~ExpTrans();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual Float eval_function_variance(Float sample) const
    {
        return optical_depth->variance(sample);
    }

    virtual Float compute_function_max() const
    {
        return optical_depth->compute_max();
    }

    virtual std::string getName() const { return "exp_trans"; }

protected:
    Function *optical_depth;
};

struct PinkDavis : public Problem<Float>
{
    PinkDavis(Function *func, Float c);
    ~PinkDavis();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "pink_davis"; }

protected:
    Function *optical_depth;
    Float c;
};

struct IntegralEval : public Problem<Float>
{
    IntegralEval(Function *func);
    ~IntegralEval();

    virtual Float eval(Float sample) const;
    virtual Float eval(const std::vector<Float> &samples) const;
    virtual Float eval_integral(Float sample) const;
    virtual Float eval_integral(const std::vector<Float> &samples) const;
    virtual Float expected() const;
    virtual Float expected_integral() const;
    virtual Float eval_function(Float sample) const;

    virtual std::string getName() const { return "integral"; }

protected:
    Function *optical_depth; // TODO: change the name of this eventually
    Float alpha;
};