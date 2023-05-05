#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct TeleNaiveBounds : Technique<T>
{
    TeleNaiveBounds(Float maj) : maj(maj) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float base_step = std::min(0.5 / maj, problem->getBounds()->getEnd() - problem->getBounds()->getStart());
        Float a = 0.0;
        Float b = 1.0;

        Float t = a;
        Float Tr = 1.0;

        while (t < b)
        {
            Float step = std::min(base_step, b-t);
            Float I0_od = step * problem->eval_function(t + rng.nextDouble() * step);
            Float IN1_od = I0_od;
            Float IN_od = I0_od;
            Float I0 = problem->eval(I0_od);
            Float W = 0.0;
            Float i = 1;
            cost++;

            while (true)
            {
                Float od = step * problem->eval_function(t + rng.nextDouble() * step);
                Float IN = problem->eval(IN_od);
                IN1_od = IN_od + 1.0 / (i+1) * (od - IN_od);
                Float IN1 = problem->eval(IN1_od);
                Float diff = IN1 - IN;
                Float accept = std::min(std::abs(IN1 - IN), 0.1);
                cost++;
                if (rng.nextDouble() > accept)
                {
                    break;
                }
                W += diff / accept;
                i += 1.0;
            }

            Tr *= (I0 + W);

            t += step;
        }

        return Tr;
    }

    virtual void setMaj(T val) { maj = val; }
    virtual std::string getName() const { return "tele_naive_bounds"; }

    Float maj;
};