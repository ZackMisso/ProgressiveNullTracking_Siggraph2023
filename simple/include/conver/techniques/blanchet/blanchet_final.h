#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetFinal : Technique<T>
{
    BlanchetFinal(Float r = 0.65) : blanchet_r(r) {}

    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // base number of in
        std::vector<Float> weights = std::vector<Float>();
        Float weight = 1.0;
        Float ratio = 0.35;

        Float a = 0.0;
        Float b = M_PI / 2.0;

        while (true)
        {
            weights.push_back(weight);
            if (rng.nextDouble() > ratio)
            {
                break;
            }
            weight *= 1.0 / ratio;
        }
        //\ compute the russian roulette weights \//

        // evaluate all of the integral samples //
        std::vector<Float> all_samples = std::vector<Float>();
        int total_n = pow(2, weights.size());
        cost += total_n;

        Float epsilon = rng.nextDouble();
        Float loc = a;
        Float delta = (b - a) / Float(total_n);

        for (int i = 0; i < total_n; ++i)
        {
            Float loc = (epsilon + i) * delta;
            all_samples.push_back(loc);
        }
        //\ evaluate all of the integral samples \//

        // evaluate all of I(k)'s //
        std::vector<Float> I_evals = std::vector<Float>();

        // weights.size() == n I think
        for (int i = 0; i < weights.size() + 1; ++i)
        {
            // Float val = 0.0;
            // int num_samps = std::pow(2, i);
            // for (int j = 0; j < total_n; j += num_samps)
            // {
            //     std::vector<Float> some_samps = std::vector<Float>(num_samps);
            //     for (int k = 0; k < num_samps; ++k)
            //     {
            //         some_samps[k] = all_samples[j + k];
            //     }
            //     val += problem->eval(problem->eval_integral(some_samps));
            // }
            // val /= Float(total_n) / Float(num_samps);
            // I_evals.push_back(val);

            Float val = 0.0;
            int num_samps = std::pow(2, i);
            for (int j = 0; j < total_n / num_samps; j++)
            {
                std::vector<Float> some_samps = std::vector<Float>(num_samps);
                for (int k = 0; k < num_samps; ++k)
                {
                    some_samps[k] = all_samples[j + k * total_n / num_samps];
                }
                val += problem->eval(problem->eval_integral(some_samps));
            }
            val /= Float(total_n) / Float(num_samps);
            I_evals.push_back(val);
        }
        //\ evaluate all of I(k)'s \//

        // compute the final estimate //
        Float final_est = 0.0;

        for (int i = 0; i < I_evals.size(); ++i)
        {
            if (i == 0)
                final_est += I_evals[i];
            else
            {
                final_est += (I_evals[i] - I_evals[i - 1]) * weights[i - 1];
            }
        }

        return final_est;

        // // tegral evaluations is 1, debias from there
        // std::vector<Float>
        //     base_samples = std::vector<Float>();
        // std::vector<Float> p_1_samples = std::vector<Float>();
        // std::vector<Float> p_2_samples = std::vector<Float>();
        // std::vector<Float> p1_samples = std::vector<Float>();

        // Float a = 0.0;
        // Float b = M_PI / 2.0;

        // // choose n and compute its pmf
        // int n = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - blanchet_r)) - 1;
        // Float blanchet_pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);

        // // sample the n evals
        // int total_n = pow(2, n + 1);

        // Float base = 0.0;
        // Float epsilon = rng.nextDouble();
        // Float loc = a;
        // Float delta = (b - a) / Float(total_n);

        // for (int i = 0; i < total_n; ++i)
        // {
        //     Float loc = (epsilon + i) * delta;

        //     base += problem->eval_rot(problem->eval_integral(loc), epsilon, 1);

        //     if (i % 2 == 0)
        //         p_1_samples.push_back(loc);
        //     else
        //         p_2_samples.push_back(loc);
        //     p1_samples.push_back(loc);
        //     cost++;
        // }

        // // compute the reciprocals
        // base /= Float(total_n);
        // Float p_1 = problem->eval_rot(problem->eval_integral(p_1_samples), epsilon, total_n / 2);
        // Float p_2 = problem->eval_rot(problem->eval_integral(p_2_samples), epsilon, total_n / 2);
        // Float p1 = problem->eval_rot(problem->eval_integral(p1_samples), epsilon, total_n);

        // cost += 2;

        // // return the result
        // return base + (p1 - 0.5 * (p_1 + p_2)) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_final"; }

    Float blanchet_r;
};