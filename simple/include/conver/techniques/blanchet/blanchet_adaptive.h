#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetAdaptive : Technique<T>
{
    BlanchetAdaptive(Float r = 0.52) : blanchet_r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // base number of integral evaluations is 1, debias from there
        // std::vector<Float> base_samples = std::vector<Float>();
        // std::vector<Float> podd_samples = std::vector<Float>();
        // std::vector<Float> peven_samples = std::vector<Float>();
        // std::vector<Float> p1_samples = std::vector<Float>();

        std::vector<Float> all_samples = std::vector<Float>();

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

        all_samples.push_back(problem->getBounds()->uniformSample(rng));
        cost++;
        Float base = problem->eval(problem->eval_integral(all_samples));
        Float weight = 1.0;
        Float contrib = base;
        Float contrib_last = contrib;
        Float c = 1.0;

        // TODO: rewrite this so that it is Mitsuba friendly
        while (true)
        {
            // this is currently a really slow implementation, but whatever
            std::vector<Float> first_half_samples = std::vector<Float>();
            std::vector<Float> second_half_samples = std::vector<Float>();

            int n = all_samples.size();
            for (int j = 0; j < n; ++j)
            {
                first_half_samples.push_back(all_samples[j]);

                Float loc = problem->getBounds()->uniformSample(rng);
                second_half_samples.push_back(loc);
                all_samples.push_back(loc);
                cost++;
            }

            Float tmp_one = problem->eval(problem->eval_integral(first_half_samples));
            Float tmp_two = problem->eval(problem->eval_integral(second_half_samples));
            Float tmp_all = problem->eval(problem->eval_integral(all_samples));

            Float contrib_new = weight * (tmp_all - 0.5 * (tmp_one + tmp_two));
            // Float ratio = std::min(std::abs(contrib_new / contrib_last) * c, 1.0);
            // Float ratio = std::min(std::sqrt(std::abs(contrib_new)), 1.0);
            // ratio = std::min(0.45, 1.0);
            Float ratio = 0.35;
            // ratio = std::min(ratio, 0.35);
            contrib += contrib_new;

            if (rng.nextDouble() > ratio)
            {
                break;
            }
            weight *= 1.0 / ratio;
            contrib_last = contrib_new;
        }

        return contrib;

        // // int total_n = pow(2, n + 1);

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

        // // return base + (p1 - 0.5 * (odd + even)) / blanchet_pmf;
        // return base + (p1 - (even)) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_adaptive"; }

    Float blanchet_r;
};