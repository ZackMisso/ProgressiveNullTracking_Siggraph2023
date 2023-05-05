#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetMC : Technique<T>
{
    BlanchetMC(Float r = 0.65) : blanchet_r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // base number of integral evaluations is 1, debias from there
        std::vector<Float> base_samples = std::vector<Float>();
        std::vector<Float> podd_samples = std::vector<Float>();
        std::vector<Float> peven_samples = std::vector<Float>();
        std::vector<Float> p1_samples = std::vector<Float>();

        // choose n and compute the blanchet pmf
        // Float val = rng.nextDouble();
        // Float blanchet_pmf = blanchet_r;
        // Float cmf = blanchet_pmf;
        // int n = 0;

        // while (cmf < val)
        // {
        //     ++n;
        //     blanchet_pmf *= 1.0 - blanchet_r;
        //     cmf += blanchet_pmf;
        // }

        int n = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - blanchet_r)) - 1;
        Float blanchet_pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);

        base_samples.push_back(problem->getBounds()->uniformSample(rng));
        cost++;

        int total_n = pow(2, n + 1);

        for (int i = 0; i < total_n; ++i)
        {
            Float loc = problem->getBounds()->uniformSample(rng);

            if (i % 2 == 0)
                podd_samples.push_back(loc);
            else
                peven_samples.push_back(loc);
            p1_samples.push_back(loc);
            cost++;
        }

        // Float base = 0.0;
        // for (int i = 0; i < p1_samples.size(); ++i)
        // {
        //     base += problem->eval(problem->eval_integral(p1_samples[i]));
        // }
        // base /= Float(p1_samples.size());

        Float base = problem->eval(problem->eval_integral(base_samples));
        Float odd = problem->eval(problem->eval_integral(podd_samples));
        Float even = problem->eval(problem->eval_integral(peven_samples));
        Float p1 = problem->eval(problem->eval_integral(p1_samples));

        return base + (p1 - 0.5 * (odd + even)) / blanchet_pmf;
        // return base + (p1 - (even)) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_mc"; }

    Float blanchet_r;
};