#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetBasic : Technique<T>
{
    BlanchetBasic(Float r = 0.65) : blanchet_r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // base number of integral evaluations is 1, debias from there
        std::vector<Float> base_samples = std::vector<Float>();
        std::vector<Float> p_samples = std::vector<Float>();
        std::vector<Float> p1_samples = std::vector<Float>();

        // choose n and compute its pmf
        int n = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - blanchet_r)) - 1;
        Float blanchet_pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);

        // sample the n evals
        int total_n = pow(2, n + 1);

        for (int i = 0; i < total_n; ++i)
        {
            Float loc = problem->getBounds()->uniformSample(rng);
            if (i == 0)
                base_samples.push_back(loc);
            if (i < total_n / 2)
                p_samples.push_back(loc);
            p1_samples.push_back(loc);
            cost++;
        }

        // compute the reciprocals
        Float base = problem->eval(problem->eval_integral(base_samples));
        Float p = problem->eval(problem->eval_integral(p_samples));
        Float p1 = problem->eval(problem->eval_integral(p1_samples));

        // return the result
        return base + (p1 - p) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_basic"; }

    Float blanchet_r;
};