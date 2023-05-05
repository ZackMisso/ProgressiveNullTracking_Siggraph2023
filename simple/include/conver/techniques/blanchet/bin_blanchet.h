#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetBin : Technique<T>
{
    BlanchetBin(Float r = 0.65) : blanchet_r(r) {}

    // TODO: need to make this faster for the composite sine function
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // base number of integral evaluations is 1, debias from there
        std::vector<Float> base_samples = std::vector<Float>();
        std::vector<Float> p_1_samples = std::vector<Float>();
        std::vector<Float> p_2_samples = std::vector<Float>();
        std::vector<Float> p1_samples = std::vector<Float>();

        Float a = 0.0;
        Float b = 1.0;
        Float min_val = 1.0 / 2.0;

        // choose n and compute its pmf
        int n = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - blanchet_r)) - 1;
        if (n < 0)
            n = 0; // this can happen apparently
        Float blanchet_pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);

        // sample the n evals
        int total_n = pow(2, n + 1);

        Float base = 0.0;
        Float epsilon = rng.nextDouble();
        Float loc = a;
        Float delta = (b - a) / Float(total_n);

        for (int i = 0; i < total_n; ++i)
        {
            Float loc = (epsilon + i) * delta;

            base += problem->eval(problem->eval_integral(loc), min_val);

            if (i % 2 == 0)
                p_1_samples.push_back(loc);
            else
                p_2_samples.push_back(loc);
            p1_samples.push_back(loc);
            cost++;
        }

        // compute the reciprocals
        base /= Float(total_n);
        Float p_1 = problem->eval(problem->eval_integral(p_1_samples), min_val / pow(2, n));
        Float p_2 = problem->eval(problem->eval_integral(p_2_samples), min_val / pow(2, n));
        Float p1 = problem->eval(problem->eval_integral(p1_samples), min_val / pow(2, n + 1));

        // return the result
        return base + (p1 - 0.5 * (p_1 + p_2)) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_bin_" + std::to_string(int(blanchet_r * 100.0)); }

    Float blanchet_r;
};