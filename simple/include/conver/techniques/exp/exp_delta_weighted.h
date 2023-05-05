#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct WeightedDeltaExp : Technique<T>
{
    WeightedDeltaExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float invMaj = 1.0 / maj;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();

        while (true)
        {
            t -= std::log(1 - rng.nextDouble()) * invMaj;
            if (t >= problem->getBounds()->getEnd())
                break;

            Float dens = problem->eval_function(t);
            cost++;

            Float null = (maj - dens);
            Float prob = std::abs(null) / (std::abs(null) + dens);

            if (prob < rng.nextDouble())
                return 0.0;

            Tr *= null * invMaj / prob;
        }

        return Tr;
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "delta_weighted_exp"; }

    Float maj;
};