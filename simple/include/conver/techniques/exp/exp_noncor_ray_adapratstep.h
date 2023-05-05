#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct RayMarchAdaptiveRatStepNonCorExp : Technique<T>
{
    RayMarchAdaptiveRatStepNonCorExp(Float step) : step(step)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float od = 0.0;
        Float t = problem->getBounds()->getStart();

        Float last_step = step;

        while (t < problem->getBounds()->getEnd())
        {
            Float this_step = -std::log(1 - rng.nextDouble()) * last_step;
            this_step = std::min(this_step, problem->getBounds()->getEnd() - t);
            Float loc = t + this_step * rng.nextDouble();
            Float func = problem->eval_function(loc);
            Float contr = this_step * func;
            last_step = std::max(1.0 / (1.0 / step - func), 0.01);
            cost++;

            if (loc < problem->getBounds()->getEnd())
                od += contr;

            t += this_step;
        }

        return exp(-od);
    }

    virtual void setMaj(T val) { step = 1.0 / val; }

    virtual std::string getName() const { return "noncor_ray_adapratstep_exp"; }

    Float step;
};