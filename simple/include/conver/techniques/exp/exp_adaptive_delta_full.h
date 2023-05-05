#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct AdaptiveDeltaFullExp : Technique<T>
{
    AdaptiveDeltaFullExp(Float maj) : maj(maj)
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

        Float weight = 1.0;

        while (true)
        {
            Float last_t = t;

            t -= std::log(1 - rng.nextDouble()) * invStep;

            if (t >= problem->getBounds()->getEnd())
            {
                Float dist = problem->getBounds()->getEnd() - last_t;
                Tr *= weight * exp((step_maj - current_maj) * dist);
                break;
            }

            Float dens = problem->eval_function(t);
            max_dense = std::max(max_dense, dens);
            cost++;

            if (current_maj < dens)
            {
                // restart estimator with a new maximum density
                // this is biased, but the bias is nearly unnoticable
                return eval_problem(problem, samples, cost, rng, max_dense, maj);
            }

            Float chk = (current_maj - dens) * invStep *
                        exp((step_maj - current_maj) * (t - last_t));
            Float wi = chk;
            Float accept = std::min(chk, 1.0);

            if (accept < rng.nextDouble())
                return 0.0;

            weight *= wi / accept;

            max_dense = std::max(dens, max_dense);
            step_maj = std::max(current_maj - dens, maj * 0.1);
            current_maj = std::max(max_dense, step_maj);
            invStep = 1.0 / step_maj;
        }

        // adaptive estimate for the maximum density in the track
        running_max = std::max(running_max, max_dense);

        return Tr;
    }

    virtual void setMaj(T val)
    {
        maj = val;
        experiment_preprocess();
    }

    virtual std::string getName() const { return "adaptive_delta_full"; }

    Float maj;
    Float running_max;
};