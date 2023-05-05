#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct IterRatioExp : Technique<T>
{
    IterRatioExp(Float maj, Float perc) : maj(maj), perc(perc)
    {
        epsilon = 0.1;
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // // actual implementation
        Float Tr = 1.0;

        Float max_ext = 0.0;
        Float new_ext = maj;

        int cnt = 0;
        while (new_ext != max_ext)
        {
            // if (cnt == 1)
            //     std::cout << "this is working" << std::endl;
            Float old_ext = max_ext;
            max_ext = new_ext;
            Tr *= eval_problem_for_range(problem, cost, rng, old_ext, new_ext);
            ++cnt;
        }

        return Tr;

        // debug implementation
        // Float Tr = 1.0;

        // Float max_ext = 0.0;
        // Float new_ext = maj;

        // Tr *= eval_problem_for_range(problem, cost, rng, max_ext, new_ext);

        // max_ext = maj;
        // new_ext = trueMaj;

        // Tr *= eval_problem_for_range(problem, cost, rng, max_ext, new_ext);

        // return Tr;
    }

    T eval_problem_for_range(const Problem<T> *problem, int &cost, pcg32 &rng, Float min_ext, Float &max_ext)
    {
        Float res = max_ext - min_ext + epsilon;
        Float invMaj = 1.0 / res;

        Float Tr = 1.0;
        Float t = problem->getBounds()->getStart();

        while (true)
        {
            t -= std::log(1 - rng.nextDouble()) * invMaj;
            if (t >= problem->getBounds()->getEnd())
                break;

            Float dens = problem->eval_function(t) - min_ext;
            cost++;
            if (dens < 0.0)
            {
                dens = 0.0;
            }

            Float diff = (res - dens);
            if (diff < epsilon)
            {
                diff = epsilon;
                max_ext = std::max(max_ext, dens + min_ext);
            }
            Float val = diff * invMaj;
            Tr *= val;
        }

        return Tr;
    }

    virtual void setMaj(T val)
    {
        maj = val * perc;
        trueMaj = val;
    }

    virtual std::string getName() const { return "ratio_iter_" + std::to_string(int(perc * 10.0)) + "_exp"; }

    Float maj;
    Float perc;
    Float trueMaj;
    Float epsilon;
};