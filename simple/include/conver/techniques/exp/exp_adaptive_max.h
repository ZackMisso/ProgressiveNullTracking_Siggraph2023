#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct AdaptiveMaxExp : Technique<T>
{
    AdaptiveMaxExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual void experiment_preprocess()
    {
        running_max = 0.0;
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // Float current_maj = maj;
        // Float step_maj = maj;
        // Float invStep = 1.0 / step_maj;
        // Float max_dense = 0.0;

        // Float Tr = 1.0;
        // Float t = problem->getBounds()->getStart();

        // bool first_step = true;

        // while (true)
        // {
        //     Float last_t = t;

        //     t -= std::log(1 - rng.nextDouble()) * invStep;

        //     if (t >= problem->getBounds()->getEnd())
        //     {
        //         Float dist = problem->getBounds()->getEnd() - last_t;
        //         Tr *= exp((step_maj - current_maj) * dist);
        //         break;
        //     }

        //     Float dens = problem->eval_function(t);
        //     cost++;

        //     if (current_maj < dens)
        //     {
        //         std::cout << "THIS SHOULD NEVER OCCUR" << std::endl;
        //         t = last_t;
        //         current_maj = maj;
        //         step_maj = maj;
        //         invStep = 1.0 / step_maj;
        //         continue;
        //     }

        //     // if (step_maj < 0.0)
        //     // {
        //     //     std::cout << "haha wtf" << std::endl;
        //     // }

        //     // if (!first_step)
        //     // {
        //     //     // if (current_maj < dens)
        //     //     //     current_maj = maj;
        //     //     // current_maj = (max_dense > dens && max_dense >= step_maj) ? max_dense : maj;
        //     // }

        //     // current_maj = std::max(step_maj, 2.0 * dens);
        //     // if (current_maj - dens < 0.0)
        //     //     std::cout << "IT HAPPENED" << std::endl;

        //     Tr *= (current_maj - dens) * invStep *
        //           exp((step_maj - current_maj) * (t - last_t));

        //     // if (dens < step_maj)
        //     //     current_maj = step_maj;

        //     // current_maj = std::max(step_maj, dens);
        //     // current_maj = std::min(current_maj, maj);

        //     // max_dense = std::max(dens, max_dense);
        //     Float old_step = step_maj;
        //     step_maj = std::max(max_dense, std::max(current_maj - dens, maj * 0.1));
        //     // step_maj = std::max(maj - dens, problem->compute_function_max());
        //     invStep = 1.0 / step_maj;
        //     current_maj = old_step;

        //     // if (step_maj < 0.0) std::cout << "FUCCK" << std::endl;

        //     // test
        //     // current_maj = step_maj;

        //     first_step = false;
        // }

        // return Tr;

        return eval_problem(problem, samples, cost, rng, running_max, maj);
    }

    virtual T eval_problem(
        const Problem<T> *problem,
        int samples,
        int &cost,
        pcg32 &rng,
        Float last_max_dense,
        Float start_maj)
    {
        Float current_maj = start_maj;
        Float step_maj = maj;
        Float invStep = 1.0 / step_maj;
        Float max_dense = last_max_dense;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();

        bool first_step = true;

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
            max_dense = std::max(max_dense, dens);
            cost++;

            if (current_maj < dens)
            {
                // maybe play with initial max dense
                return eval_problem(problem, samples, cost, rng, max_dense, maj);
                // std::cout << "THIS SHOULD NEVER OCCUR" << std::endl;
                // t = last_t;
                // current_maj = maj;
                // step_maj = maj;
                // invStep = 1.0 / step_maj;
                // continue;
            }

            // if (step_maj < 0.0)
            // {
            //     std::cout << "haha wtf" << std::endl;
            // }

            // if (!first_step)
            // {
            //     // if (current_maj < dens)
            //     //     current_maj = maj;
            //     // current_maj = (max_dense > dens && max_dense >= step_maj) ? max_dense : maj;
            // }

            // current_maj = std::max(step_maj, 2.0 * dens);
            // if (current_maj - dens < 0.0)
            //     std::cout << "IT HAPPENED" << std::endl;

            Tr *= (current_maj - dens) * invStep *
                  exp((step_maj - current_maj) * (t - last_t));

            // if (dens < step_maj)
            //     current_maj = step_maj;

            // current_maj = std::max(step_maj, dens);
            // current_maj = std::min(current_maj, maj);

            max_dense = std::max(dens, max_dense);
            Float old_step = step_maj;
            step_maj = std::max(current_maj - dens, maj * 0.1);
            // step_maj = std::max(maj - dens, problem->compute_function_max());
            invStep = 1.0 / step_maj;
            current_maj = std::max(max_dense, step_maj);

            // if (step_maj < 0.0) std::cout << "FUCCK" << std::endl;

            // test
            // current_maj = step_maj;

            first_step = false;
        }

        running_max = std::max(running_max, max_dense);

        return Tr;
    }

    virtual void setMaj(T val)
    {
        maj = val;
        experiment_preprocess();
    }

    virtual std::string getName() const { return "sandbox_exp"; }

    Float maj;
    Float running_max;
};