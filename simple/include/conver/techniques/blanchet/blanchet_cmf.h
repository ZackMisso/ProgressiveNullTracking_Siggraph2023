#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetCmf : Technique<T>
{
    BlanchetCmf(Float r = 0.52) : blanchet_r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // TODO: replace this soonish with its analytical formulation
        Float c = blanchet_r;
        c = 0.65;
        Float val = rng.nextDouble();
        // Float blanchet_pmf = c;
        // Float cmf = blanchet_pmf;
        int n = std::ceil(log(1.0 - val) / log(1.0 - c)) - 1;
        if (n < 0)
            n = 0; // this can happen apparently

        // while (cmf < val)
        // {
        //     ++n;
        //     blanchet_pmf *= 1.0 - c;
        //     cmf += blanchet_pmf;
        // }

        // n = 10;

        int total_n = pow(2, n + 1);
        std::vector<Float> all_samples = std::vector<Float>(total_n);

        cost += total_n;

        for (int i = 0; i < total_n; ++i)
        {
            Float loc = problem->getBounds()->uniformSample(rng);
            all_samples[i] = loc;
        }

        Float result = problem->eval(problem->eval_integral(all_samples)) / std::pow(1.0 - c, n + 1);

        for (int i = 0; i < n + 1; ++i)
        {
            // TODO: can improve variance here by taking the average of different sized integrals
            int num_samps = std::pow(2, i);

            // std::cout << some_samps.size() << std::endl;
            Float val = 0.0;
            for (int j = 0; j < total_n; j += num_samps)
            {
                std::vector<Float> some_samps = std::vector<Float>(num_samps);
                for (int k = 0; k < num_samps; ++k)
                {
                    some_samps[k] = all_samples[j + k];
                }
                val += problem->eval(problem->eval_integral(some_samps));
            }
            val /= Float(total_n) / Float(num_samps);
            result -= c * val / std::pow(1.0 - c, i + 1);
        }

        return result;

        // // base number of integral evaluations is 1, debias from there
        // std::vector<Float> base_samples = std::vector<Float>();
        // std::vector<Float> podd_samples = std::vector<Float>();
        // std::vector<Float> peven_samples = std::vector<Float>();
        // std::vector<Float> p1_samples = std::vector<Float>();

        // // choose n and compute the blanchet pmf
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

        // base_samples.push_back(problem->getBounds()->uniformSample(rng));
        // cost++;

        // int total_n = pow(2, n + 1);

        // for (int i = 0; i < total_n; ++i)
        // {
        //     Float loc = problem->getBounds()->uniformSample(rng);

        //     if (i % 2 == 0)
        //         podd_samples.push_back(loc);
        //     else
        //         peven_samples.push_back(loc);
        //     p1_samples.push_back(loc);
        //     cost++;
        // }

        // Float base = problem->eval(problem->eval_integral(base_samples));
        // Float odd = problem->eval(problem->eval_integral(podd_samples));
        // Float even = problem->eval(problem->eval_integral(peven_samples));
        // Float p1 = problem->eval(problem->eval_integral(p1_samples));

        // return base + (p1 - 0.5 * (odd + even)) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_cmf"; }

    Float blanchet_r;
};