#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct AdaptiveDeltaExp : Technique<T>
{
    AdaptiveDeltaExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // Float invMaj = 1.0 / maj;

        Float maj_to_use = maj;
        Float invMaj = 1.0 / maj_to_use;
        // Float invBase = 1.0 / maj;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();
        Float last_t = t;

        Float weight = 1.0;

        while (true)
        {
            last_t = t;

            t -= std::log(1 - rng.nextDouble()) * invMaj;
            if (t >= problem->getBounds()->getEnd())
                break;

            Float dens = problem->eval_function(t);
            cost++;

            Float chk = (maj - dens) * invMaj * exp(-maj * (t - last_t)) / exp(-maj_to_use * (t - last_t));
            Float wi = chk;
            Float accept = std::min(chk, 1.0);

            if (accept < rng.nextDouble())
                return 0.0;

            weight *= wi / accept;

            maj_to_use = std::max(maj - dens, maj * 0.1);
            invMaj = 1.0 / maj_to_use;
        }

        return weight * exp(-maj * (problem->getBounds()->getEnd() - last_t)) / exp(-maj_to_use * (problem->getBounds()->getEnd() - last_t));
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "adaptive_delta_exp"; }

    Float maj;
};