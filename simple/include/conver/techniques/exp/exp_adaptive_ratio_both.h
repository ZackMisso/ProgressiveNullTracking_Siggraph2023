#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct AdaptiveRatioBothExp : Technique<T>
{
    AdaptiveRatioBothExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float maj_to_use = maj;
        Float current_maj = maj;
        Float invMaj = 1.0 / maj_to_use;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();

        while (true)
        {
            Float last_t = t;

            t -= std::log(1 - rng.nextDouble()) * invMaj;
            if (t >= problem->getBounds()->getEnd())
            {
                Tr *= exp(-current_maj * (problem->getBounds()->getEnd() - last_t)) / exp(-maj_to_use * (problem->getBounds()->getEnd() - last_t));
                break;
            }

            Float dens = problem->eval_function(t);
            cost++;

            Tr *= (current_maj - dens) * invMaj * exp(-current_maj * (t - last_t)) / exp(-maj_to_use * (t - last_t));

            maj_to_use = std::max(std::max(maj - dens, maj * 0.1), problem->compute_function_max());
            current_maj = maj_to_use;

            invMaj = 1.0 / maj_to_use;
        }

        return Tr;
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "adaptive_ratio_both_exp"; }

    Float maj;
};