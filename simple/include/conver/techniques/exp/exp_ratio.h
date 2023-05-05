#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct RatioExp : Technique<T>
{
    RatioExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // Float tau = maj / problem->getBounds()->uniformPDF();
        // Float sample = rng.nextDouble();
        // Float val = exp(-tau);
        // Float cdf = val;
        // Float prob = val;
        // Float Tr = Float(1.0);
        // Float i = 1.0;

        // while (cdf < sample)
        // {
        //     Float x = problem->getBounds()->uniformSample(rng);
        //     Float dense = problem->eval_integral(x);
        //     Tr *= (tau - dense) / tau;
        //     prob *= tau / i;
        //     cost++;

        //     cdf += prob;
        //     i += 1.0;
        // }

        // return Tr;

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

            Tr *= (maj - dens) * invMaj;
        }

        return Tr;
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "ratio_exp"; }

    Float maj;
};