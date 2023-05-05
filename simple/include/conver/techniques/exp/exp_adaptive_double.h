#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct AdaptiveDoubleExp : Technique<T>
{
    AdaptiveDoubleExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float current_maj = maj;
        Float step_maj = maj;
        Float invStep = 1.0 / step_maj;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();

        while (true)
        {
            Float last_t = t;

            t -= std::log(1 - rng.nextDouble()) * invStep;

            if (t >= problem->getBounds()->getEnd())
            {
                Float dist = problem->getBounds()->getEnd() - last_t;
                Tr *= exp((step_maj - current_maj) * dist);
                break;
            }

            Float dens = problem->eval_function(t);
            cost++;

            // current_maj = std::max(step_maj, 2.0 * dens);
            // if (current_maj - dens < 0.0)
            //     std::cout << "IT HAPPENED" << std::endl;

            Tr *= (current_maj - dens) * invStep *
                  exp((step_maj - current_maj) * (t - last_t));

            // if (dens < step_maj)
            //     current_maj = step_maj;

            current_maj = std::max(step_maj, 2.0 * dens);
            current_maj = std::min(current_maj, maj);

            step_maj = std::max(current_maj - dens, current_maj * 0.1);
            invStep = 1.0 / step_maj;
        }

        return Tr;
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "adaptive_double_exp"; }

    Float maj;
};