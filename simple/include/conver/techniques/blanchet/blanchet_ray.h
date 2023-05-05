#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetRay : Technique<T>
{
    BlanchetRay(Float r = 0.65) : blanchet_r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // base number of integral evaluations is 1, debias from there
        std::vector<Float> base_samples = std::vector<Float>();
        std::vector<Float> p_1_samples = std::vector<Float>();
        std::vector<Float> p_2_samples = std::vector<Float>();
        std::vector<Float> p1_samples = std::vector<Float>();

        Float a = 0.0;
        Float b = M_PI / 2.0;

        // choose n and compute its pmf
        int n = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - blanchet_r)) - 1;
        Float blanchet_pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);

        if (n < 0)
            std::cout << "WHAT IN THE HECK n = " << n << std::endl;
        if (n > 20)
            std::cout << "LARGE N: " << n << std::endl;

        // sample the n evals
        int total_n = pow(2, n + 1);

        Float base = 0.0;
        Float epsilon = rng.nextDouble();
        Float loc = a;
        Float delta = (b - a) / Float(total_n);

        for (int i = 0; i < total_n; ++i)
        {
            Float loc = (epsilon + i) * delta;

            base += problem->eval(problem->eval_integral(loc));

            if (i % 2 == 0)
                p_1_samples.push_back(loc);
            else
                p_2_samples.push_back(loc);
            p1_samples.push_back(loc);
            cost++;
        }

        // compute the reciprocals
        base /= Float(total_n);
        Float p_1 = problem->eval(problem->eval_integral(p_1_samples));
        Float p_2 = problem->eval(problem->eval_integral(p_2_samples));
        Float p1 = problem->eval(problem->eval_integral(p1_samples));

        // return the result
        return base + (p1 - 0.5 * (p_1 + p_2)) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_ray"; }

    Float blanchet_r;
};