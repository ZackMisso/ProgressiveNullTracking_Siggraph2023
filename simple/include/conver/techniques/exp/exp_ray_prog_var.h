#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

// TODO :: this implementation is going to require a lot of thought

template <typename T>
struct RayMarchProgVarExp : Technique<T>
{
    RayMarchProgVarExp(Float step) : step(step)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float od = 0.0;
        Float rn = rng.nextDouble();
        Float t = problem->getBounds()->getStart();
        Float step_one = 0.0;
        Float step_two = 0.0;
        int iter = 0;

        while (t < problem->getBounds()->getEnd())
        {
            Float var = 0.0;
            if (iter > 2)
                var = (step_one - step_two) * (step_one - step_two);

            Float this_step;
            if (var < 1e-4) this_step = step;
            else
                this_step = 1.0 / var;
            
            this_step = std::min(this_step, problem->getBounds()->getEnd() - t);

            Float loc = t + this_step * rn;
            Float contr = this_step * problem->eval_function(loc);
            cost++;

            step_two = step_one;
            step_one = contr / this_step;
            iter++;

            if (loc < problem->getBounds()->getEnd())
                od += contr;

            t += this_step;
        }

        return exp(-od);
    }

    virtual void setMaj(T val) { step = 1.0 / val; }

    virtual std::string getName() const { return "ray_prog_var_exp"; }

    Float step;
};