#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct RayMarchRatStepNonCorExp : Technique<T>
{
    RayMarchRatStepNonCorExp(Float step) : step(step)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float od = 0.0;
        Float t = problem->getBounds()->getStart();

        while (t < problem->getBounds()->getEnd())
        {
            Float this_step = -std::log(1 - rng.nextDouble()) * step;
            this_step = std::min(this_step, problem->getBounds()->getEnd() - t);
            Float loc = t + this_step * rng.nextDouble();
            Float contr = this_step * problem->eval_function(loc);
            cost++;

            if (loc < problem->getBounds()->getEnd())
                od += contr;

            t += this_step;
        }

        return exp(-od);
    }

    virtual void setMaj(T val) { step = 1.0 / val; }

    virtual std::string getName() const { return "noncor_ray_ratstep_exp"; }

    Float step;
};