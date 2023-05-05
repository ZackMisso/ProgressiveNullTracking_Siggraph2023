#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct AdaptiveRatioClampExp : Technique<T>
{
    AdaptiveRatioClampExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual void experiment_preprocess()
    {
        running_max = 0.0;
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        return eval_problem(problem, samples, cost, rng, running_max, maj);
    }

    virtual T eval_problem(
        const Problem<T> *problem,
        int samples,
        int &cost,
        pcg32 &rng,
        Float last_max_dense,
        Float start_maj)
    {
        Float current_maj = start_maj;
        Float step_maj = maj;
        Float invStep = 1.0 / step_maj;
        Float max_dense = last_max_dense;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();

        bool first_step = true;

        while (true)
        {
            Float last_t = t;

            // sample a distance based on the sampling majorant
            t -= std::log(1 - rng.nextDouble()) * invStep;

            if (t >= problem->getBounds()->getEnd())
            {
                // account for difference in sampling weight
                Float dist = problem->getBounds()->getEnd() - last_t;
                Tr *= exp((step_maj - current_maj) * dist);
                break;
            }

            // compute extinction and update max extinction
            Float dens = problem->eval_function(t);
            max_dense = std::max(max_dense, dens);
            cost++;

            if (current_maj < dens)
            {
                // restart estimator with a new maximum density
                // this is biased, but the bias is eventually unnoticable
                // return eval_problem(problem, samples, cost, rng, max_dense, maj);
                dens = current_maj;
                running_max = std::max(running_max, max_dense);
                // this will return 0 eventually
                return 0.0;
            }

            // typical ratio tracking update with difference in
            // sampling and null majorant
            Tr *= (current_maj - dens) * invStep *
                  exp((step_maj - current_maj) * (t - last_t));

            // set the sampling majorant to the current null extinction
            step_maj = std::max(current_maj - dens, maj * 0.1);
            // update the null majorant
            current_maj = std::max(max_dense, step_maj);
            // update the inverse sampling majorant
            invStep = 1.0 / step_maj;
        }

        // update adaptive estimate for the max density in this region
        running_max = std::max(running_max, max_dense);

        return Tr;
    }

    virtual void setMaj(T val)
    {
        maj = val;
        experiment_preprocess();
    }

    virtual std::string getName() const { return "adaptive_ratio_clamp"; }

    Float maj;
    Float running_max;
};