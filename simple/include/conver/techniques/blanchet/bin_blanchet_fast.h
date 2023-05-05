#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetBinFast : Technique<T>
{
    BlanchetBinFast(Float r = 0.65) : blanchet_r(r) {}

    // TODO: need to make this faster for the composite sine function
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng) 
    {
        // base number of integral evaluations is 1, debias from there
        // std::vector<Float> base_samples = std::vector<Float>();
        // std::vector<Float> p_1_samples = std::vector<Float>();
        // std::vector<Float> p_2_samples = std::vector<Float>();
        // std::vector<Float> p1_samples = std::vector<Float>();

        Float a = 0.0;
        Float b = 1.0;
        Float min_val = 1.0 / 2.0;

        // choose n and compute its pmf
        int n = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - blanchet_r)) - 1;
        if (n < 0)
            n = 0; // this can happen apparently
        Float blanchet_pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);

        int base_n = 4;
        n += base_n;

        // sample the n evals
        int total_n = pow(2, n + 1);

        Float base = 0.0;
        Float epsilon = rng.nextDouble();
        Float loc = a;
        Float delta = (b - a) / Float(total_n);

        Float p_1 = 0.0;
        Float p_2 = 0.0;
        Float p1 = 0.0;

        int full_base_n = pow(2, base_n);
        std::vector<Float> bases = std::vector<Float>(total_n / full_base_n);
        for (int i = 0; i < bases.size(); ++i)
        {
            bases[i] = 0.0;
        }

        for (int i = 0; i < total_n; ++i)
        {
            Float loc = (epsilon + i) * delta;

            Float eval = problem->eval_integral(loc);

            int iter = i / (total_n / full_base_n);
            int res = i % (total_n / full_base_n);
            // std::cout << "res: " << res << std::endl;
            // std::cout << "full base: " << full_base_n << std::endl;
            // std::cout << "total_n: " << total_n << std::endl;
            bases[res] = bases[res] + 1.0 / double(iter + 1) * (eval - bases[res]);

            // if (res > full_base_n)
            // {
            //     std::cout << "i: " << i << std::endl;
            //     std::cout << "res: " << res << std::endl;
            //     std::cout << "full: " << full_base_n << std::endl;
            //     std::cout << "HUF" << std::endl;
            // }
            // if (cnt == base_n)
            // {
            //     base += problem->eval(base_tmp, min_val);
            //     cnt = 0;
            //     base_tmp = 0.0;
            // }
            // else
            // {
            //     cnt++;
            // }

            // base += problem->eval(eval, min_val);

            if (i % 2 == 0)
                p_1 += eval;
            else
                p_2 += eval;
            p1 += eval;
            cost++;
        }

        // std::cout << "wat2" << std::endl;

        for (int i = 0; i < bases.size(); ++i)
        {
            base += problem->eval(bases[i], min_val);
        }
        base /= Float(bases.size());

        p_1 /= Float(total_n / 2);
        p_2 /= Float(total_n / 2);
        p1 /= Float(total_n);

        // compute the reciprocals
        // base /= Float(total_n);
        p_1 = problem->eval(p_1, min_val / Float(total_n) * 2.0);
        p_2 = problem->eval(p_2, min_val / Float(total_n) * 2.0);
        p1 = problem->eval(p1, min_val / Float(total_n));

        // std::cout << "wat" << std::endl;
        // bases.clear();
        // std::cout << "base: " << base << std::endl;

        // return the result
        return base + (p1 - 0.5 * (p_1 + p_2)) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_bin_fast_" + std::to_string(int(blanchet_r * 100.0)); }

    Float blanchet_r;
};