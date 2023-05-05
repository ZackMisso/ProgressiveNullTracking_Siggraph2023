#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct AdaptiveMaxNFExp : Technique<T>
{
    AdaptiveMaxNFExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float current_maj = maj;
        Float step_maj = maj;
        Float invStep = 1.0 / step_maj;
        Float max_dense = 0.0;

        Float Tr = 0.0;
        Float w = 1.0;
        Float t = problem->getBounds()->getStart();

        bool first_step = true;

        while (true)
        {
            Float last_t = t;

            t -= std::log(1 - rng.nextDouble()) * invStep;

            Tr += w * exp(-current_maj * (problem->getBounds()->getEnd() - last_t));

            if (t >= problem->getBounds()->getEnd())
            {
                break;
                // Float dist = problem->getBounds()->getEnd() - last_t;
                // Tr *= exp((step_maj - current_maj) * dist);
                // break;
            }

            Float dens = problem->eval_function(t);
            cost++;

            if (!first_step)
            {
                if (current_maj < dens)
                    current_maj = maj;
                // if (current_maj < dens)
                //     current_maj = (max_dense > dens && max_dense >= step_maj) ? max_dense : maj;
            }

            // current_maj = std::max(step_maj, 2.0 * dens);
            // if (current_maj - dens < 0.0)
            //     std::cout << "IT HAPPENED" << std::endl;

            w *= (current_maj - dens) * invStep *
                 exp((step_maj - current_maj) * (t - last_t));

            // if (dens < step_maj)
            //     current_maj = step_maj;

            // current_maj = std::max(step_maj, dens);
            // current_maj = std::min(current_maj, maj);

            step_maj = std::max(current_maj - dens, current_maj * 0.1);
            invStep = 1.0 / step_maj;
            current_maj = step_maj;

            first_step = false;

            max_dense = std::max(dens, max_dense);
        }

        return Tr;
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "adaptive_max_nf_exp"; }

    Float maj;
};