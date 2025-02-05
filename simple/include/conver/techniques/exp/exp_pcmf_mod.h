#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct PCmfModExp : Technique<T>
{
    PCmfModExp(Float maj) : maj(maj)
    {
        // blah
    }

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        Float pdf = problem->getBounds()->uniformPDF();
        Float tau_max = maj / pdf;

        Float weight = 1.0;
        Float sum = 0.0;

        Float end_portion = 0.99;
        Float last_pdf = exp(-tau_max);
        Float running_cdf = 0.0;
        Float rr;
        Float i = 1.0;
        Float Tr = 0.0;
        do
        {
            rr = rng.nextDouble();
        } while (rr == 0.0);
        while (running_cdf < end_portion)
        {
            Float pos = problem->getBounds()->uniformSample(rng);
            running_cdf += last_pdf;
            Float invI = 1.0 / i;
            Float dense = problem->eval_integral(pos);
            Float wi = invI * (tau_max - dense);
            last_pdf *= tau_max * invI;
            Tr += weight;
            weight *= wi;
            i += 1.0;
            cost++;
        }

        while (true)
        {
            Float rr = rng.nextDouble();
            Float accept = std::abs(weight);

            Tr += weight;

            if (accept <= rr)
                break;

            Float pos = problem->getBounds()->uniformSample(rng);
            Float invI = 1.0 / i;
            Float dense = problem->eval_integral(pos);
            Float wi = invI * (tau_max - dense);

            weight *= (wi / accept);
            i += 1.0;
            cost++;
        }

        return Tr * exp(-tau_max);
    }

    virtual void setMaj(T val) { maj = val; }

    virtual std::string getName() const { return "pcmf_exp_mod"; }

    Float maj;
};