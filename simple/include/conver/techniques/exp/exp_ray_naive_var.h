#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct RayMarchNaiveVarExp : Technique<T>
{
    RayMarchNaiveVarExp(Float step) : step(step)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float od = 0.0;
        Float rn = rng.nextDouble();
        Float t = problem->getBounds()->getStart();

        while (t < problem->getBounds()->getEnd())
        {
            Float var = problem->eval_function_variance(t);
            Float this_step;
            if (var < 1e-4) this_step = step;
            else
                this_step = 1.0 / var;
            // std::cout << var << std::endl;
            
            this_step = std::min(this_step, problem->getBounds()->getEnd() - t);

            Float loc = t + this_step * rn;
            Float contr = this_step * problem->eval_function(loc);
            cost++;

            if (loc < problem->getBounds()->getEnd())
                od += contr;

            t += this_step;
        }

        return exp(-od);
    }

    virtual void setMaj(T val) { step = 1.0 / val; }

    virtual std::string getName() const { return "ray_naive_var_exp"; }

    Float step;
};