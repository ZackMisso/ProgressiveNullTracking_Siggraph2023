#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct Technique
{
public:
    virtual ~Technique() {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) = 0;

    // used to reset a technique if it adapts overtime
    virtual void experiment_preprocess() {}

    virtual T eval_problem_secondary(const Problem<T> *problem, int samples, Float &cost, pcg32 &rng)
    {
        T val = 0.0;
        int tmp_cost = 0;

        for (int i = 0; i < samples; ++i)
        {
            val = val + 1.0 / Float(i + 1) * (eval_problem(problem, 1, tmp_cost, rng) - val);
        }

        cost += tmp_cost;

        return val;
    }

    virtual T eval_jans_problem_secondary(const Problem<T> *problem, int samples, Float &cost, pcg32 &rng)
    {
        T val = 0.0;
        int tmp_cost = 0;

        for (int i = 0; i < samples; ++i)
        {
            val = val + 1.0 / Float(i + 1) * (eval_problem(problem, 1, tmp_cost, rng) - val);
        }

        cost += Float(tmp_cost) / Float(samples);

        return val;
    }

    virtual T eval_integral_problem_secondary(Problem<T> *problem, int samples, Float &cost, pcg32 &rng)
    {
        T val = 0.0;
        int tmp_cost = 0;

        for (int i = 0; i < samples; ++i)
        {
            Float weight = problem->pre_sample(rng);
            val += weight * eval_problem(problem, 1, tmp_cost, rng);
        }

        // cost += Float(tmp_cost) / Float(samples);
        cost += tmp_cost;
        val /= Float(samples);

        return val;
    }

    virtual void setMaj(T Maj) {}

    virtual std::string getName() const = 0;

protected:
};

template <typename T>
struct NaiveMC : Technique<T>
{
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        std::vector<Float> sampless = std::vector<Float>();

        for (int i = 0; i < samples; ++i)
        {
            sampless.push_back(problem->getBounds()->uniformSample(rng));
            cost++;
        }

        return problem->eval(problem->eval_integral(sampless));
    }

    virtual std::string getName() const { return "naive_mc"; }
};

template <typename T>
struct Expected : Technique<T>
{
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        cost++;
        return problem->expected();
    }

    virtual std::string getName() const { return "expected"; }
};

template <typename T>
struct TestMC : Technique<T>
{
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        std::vector<Float> sampless = std::vector<Float>();

        for (int i = 0; i < samples; ++i)
        {
            sampless.push_back(problem->getBounds()->uniformSample(rng));
            cost++;
        }

        return problem->eval(sampless);
    }

    virtual std::string getName() const { return "mc"; }
};

template <typename T>
struct NaiveFD : Technique<T>
{
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        T step_size = 1.5 / Float(samples);
        // TODO: how to incorporate cost here
        return problem->eval(step_size);
    }

    virtual std::string getName() const { return "naive_fd"; }
};

// template <typename T>
struct UnbiasedNaiveFD : Technique<Float>
{
    virtual Float eval_problem(const Problem<Float> *problem, int samples, int &cost, pcg32 &rng)
    {
        // TODO: figure out what these original techniques were supposed to be used for
        // this assumes problem->eval evaluates a finite difference
        Float mean = 0.0;

        for (int i = 0; i < samples; ++i)
        {
            Float val = rng.nextDouble();

            Float r = 0.5;
            Float blanchet_pmf = r;
            Float cmf = blanchet_pmf;
            int blanchet_n = 0;

            while (cmf < val)
            {
                ++blanchet_n;
                blanchet_pmf *= 1.0 - r;
                cmf += blanchet_pmf;
            }

            Float base_eval = problem->eval(1.5);
            Float p1_eval = problem->eval(1.5 * pow(2.0, -Float(blanchet_n + 1)));
            Float p_eval = problem->eval(1.5 * pow(2.0, -Float(blanchet_n)));

            mean += base_eval + (p1_eval - p_eval) / blanchet_pmf;
        }

        mean /= Float(samples);

        return mean;
    }

    virtual std::string getName() const { return "unbiased_naive_fd"; }
};