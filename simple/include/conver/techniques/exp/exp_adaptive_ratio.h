#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct AdaptiveRatioExp : Technique<T>
{
    AdaptiveRatioExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float maj_to_use = maj;
        Float invMaj = 1.0 / maj_to_use;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();

        // std::cout << "maj: " << maj << std::endl;

        while (true)
        {
            Float last_t = t;

            t -= std::log(1 - rng.nextDouble()) * invMaj;
            if (t >= problem->getBounds()->getEnd())
            {
                Tr *= exp(-maj * (problem->getBounds()->getEnd() - last_t)) / exp(-maj_to_use * (problem->getBounds()->getEnd() - last_t));
                break;
            }

            Float dens = problem->eval_function(t);
            cost++;

            // std::cout << "maj: " << maj << std::endl;
            // std::cout << "dense: " << dens << std::endl;

            Tr *= (maj - dens) * invMaj * exp(-maj * (t - last_t)) / exp(-maj_to_use * (t - last_t));

            maj_to_use = std::max(maj - dens, maj * 0.1);
            invMaj = 1.0 / maj_to_use;
        }

        return Tr;
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "adaptive_ratio_exp"; }

    Float maj;
};