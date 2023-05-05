#pragma once

#include <conver/conver.h>
#include <conver/problem.h>

template <typename T>
struct BlanchetTaylor : Technique<T>
{
    BlanchetTaylor(Float r = 0.65) : blanchet_r(r) {}

    // f(x, k) = 1/k + (k-x)/k^2 + (k-x)^2/k^3
    // TODO: get elementary means working maybe???
    virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
    {
        // base number of integral evaluations is 1, debias from there
        // std::vector<Float> base_samples = std::vector<Float>();
        // std::vector<Float> podd_samples = std::vector<Float>();
        // std::vector<Float> peven_samples = std::vector<Float>();
        // std::vector<Float> p1_samples = std::vector<Float>();

        // choose n and compute the blanchet pmf
        Float val = rng.nextDouble();
        int n = std::ceil(log(1.0 - val) / log(1.0 - blanchet_r)) - 1;
        Float pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);

        int total_n = pow(2, n + 2);
        std::vector<Float> all_samples = std::vector<Float>();
        for (int i = 0; i < total_n; ++i)
        {
            Float loc = problem->getBounds()->uniformSample(rng);
            all_samples.push_back(loc);
            cost++;
        }

        // compute I(n+1)
        Float np1_contr = 0.0;
        std::vector<Float> np1_one = std::vector<Float>();
        std::vector<Float> np1_two = std::vector<Float>();

        for (int i = 0; i < total_n / 2; ++i)
            np1_one.push_back(all_samples[i]);
        for (int i = total_n / 2; i < total_n; ++i)
            np1_two.push_back(all_samples[i]);

        // Float np1_one_eval = problem->eval(problem->eval_integral(np1_one));
        // Float np1_two_eval = problem->eval(problem->eval_integral(np1_two));
        Float np1_one_eval = problem->eval_integral(np1_one);
        Float np1_two_eval = problem->eval_integral(np1_two);

        // np1_contr = 0.5 * ((1.0 / np1_one_eval) + (np1_one_eval - np1_two_eval) / (np1_one_eval * np1_one_eval) +
        //                    (1.0 / np1_two_eval) + (np1_two_eval - np1_one_eval) / (np1_two_eval * np1_two_eval));

        np1_contr = 0.5 * ((1.0 / np1_one_eval) + //(np1_one_eval - np1_two_eval) / (np1_one_eval * np1_one_eval) +
                           (1.0 / np1_two_eval)); // + (np1_two_eval - np1_one_eval) / (np1_two_eval * np1_two_eval));

        // compute I(n)
        Float n_contr = 0.0;
        std::vector<Float> n_one = std::vector<Float>();
        std::vector<Float> n_two = std::vector<Float>();
        std::vector<Float> n_three = std::vector<Float>();
        std::vector<Float> n_four = std::vector<Float>();

        for (int i = 0; i < total_n / 4; ++i)
            n_one.push_back(all_samples[i]);
        for (int i = total_n / 4; i < total_n / 2; ++i)
            n_two.push_back(all_samples[i]);
        for (int i = total_n / 2; i < 3 * total_n / 4; ++i)
            n_three.push_back(all_samples[i]);
        for (int i = 3 * total_n / 4; i < total_n; ++i)
            n_four.push_back(all_samples[i]);

        // Float n_one_eval = problem->eval(problem->eval_integral(n_one));
        // Float n_two_eval = problem->eval(problem->eval_integral(n_two));
        // Float n_three_eval = problem->eval(problem->eval_integral(n_three));
        // Float n_four_eval = problem->eval(problem->eval_integral(n_four));
        Float n_one_eval = problem->eval_integral(n_one);
        Float n_two_eval = problem->eval_integral(n_two);
        Float n_three_eval = problem->eval_integral(n_three);
        Float n_four_eval = problem->eval_integral(n_four);

        // n_contr = 0.25 * ((1.0 / n_one_eval) +
        //                   (n_one_eval - n_two_eval) / (n_one_eval * n_one_eval) +
        //                   (n_one_eval - n_two_eval) * (n_one_eval - n_three_eval) / (n_one_eval * n_one_eval * n_one_eval) +
        //                   (n_one_eval - n_two_eval) * (n_one_eval - n_three_eval) * (n_one_eval - n_four_eval) / (n_one_eval * n_one_eval * n_one_eval * n_one_eval) +
        //                   (1.0 / n_two_eval) +
        //                   (n_two_eval - n_one_eval) / (n_two_eval * n_two_eval) +
        //                   (n_two_eval - n_one_eval) * (n_two_eval - n_three_eval) / (n_two_eval * n_two_eval * n_two_eval) +
        //                   (n_two_eval - n_one_eval) * (n_two_eval - n_three_eval) * (n_two_eval - n_four_eval) / (n_two_eval * n_two_eval * n_two_eval * n_two_eval) +
        //                   (1.0 / n_three_eval) +
        //                   (n_three_eval - n_one_eval) / (n_three_eval * n_three_eval) +
        //                   (n_three_eval - n_one_eval) * (n_three_eval - n_two_eval) / (n_three_eval * n_three_eval * n_three_eval) +
        //                   (n_three_eval - n_one_eval) * (n_three_eval - n_two_eval) * (n_three_eval - n_four_eval) / (n_three_eval * n_three_eval * n_three_eval * n_three_eval) +
        //                   (1.0 / n_four_eval) +
        //                   (n_four_eval - n_one_eval) / (n_four_eval * n_four_eval) +
        //                   (n_four_eval - n_one_eval) * (n_four_eval - n_two_eval) / (n_four_eval * n_four_eval * n_four_eval) +
        //                   (n_four_eval - n_one_eval) * (n_four_eval - n_two_eval) * (n_four_eval - n_three_eval) / (n_four_eval * n_four_eval * n_four_eval * n_four_eval));

        // n_contr = 0.25 * ((1.0 / n_one_eval) +
        //                   (n_one_eval - (n_two_eval + n_three_eval + n_four_eval) / 3.0) / (n_one_eval * n_one_eval) +
        //                   //   (n_one_eval - n_two_eval) * (n_one_eval - n_three_eval) / (n_one_eval * n_one_eval * n_one_eval) +
        //                   //   (n_one_eval - n_two_eval) * (n_one_eval - n_three_eval) * (n_one_eval - n_four_eval) / (n_one_eval * n_one_eval * n_one_eval * n_one_eval) +
        //                   (1.0 / n_two_eval) +
        //                   (n_two_eval - (n_one_eval + n_three_eval + n_four_eval) / 3.0) / (n_two_eval * n_two_eval) +
        //                   //   (n_two_eval - n_one_eval) * (n_two_eval - n_three_eval) / (n_two_eval * n_two_eval * n_two_eval) +
        //                   //   (n_two_eval - n_one_eval) * (n_two_eval - n_three_eval) * (n_two_eval - n_four_eval) / (n_two_eval * n_two_eval * n_two_eval * n_two_eval) +
        //                   (1.0 / n_three_eval) +
        //                   (n_three_eval - (n_one_eval + n_two_eval + n_four_eval) / 3.0) / (n_three_eval * n_three_eval) +
        //                   //   (n_three_eval - n_one_eval) * (n_three_eval - n_two_eval) / (n_three_eval * n_three_eval * n_three_eval) +
        //                   //   (n_three_eval - n_one_eval) * (n_three_eval - n_two_eval) * (n_three_eval - n_four_eval) / (n_three_eval * n_three_eval * n_three_eval * n_three_eval) +
        //                   (1.0 / n_four_eval) +
        //                   (n_four_eval - (n_one_eval + n_two_eval + n_three_eval) / 3.0) / (n_four_eval * n_four_eval));
        // //   (n_four_eval - n_one_eval) * (n_four_eval - n_two_eval) / (n_four_eval * n_four_eval * n_four_eval) +
        // //   (n_four_eval - n_one_eval) * (n_four_eval - n_two_eval) * (n_four_eval - n_three_eval) / (n_four_eval * n_four_eval * n_four_eval * n_four_eval));

        n_contr = 0.25 * ((1.0 / n_one_eval) +
                          //   (n_one_eval - (n_two_eval + n_three_eval + n_four_eval) / 3.0) / (n_one_eval * n_one_eval) +
                          //   (n_one_eval - n_two_eval) * (n_one_eval - n_three_eval) / (n_one_eval * n_one_eval * n_one_eval) +
                          //   (n_one_eval - n_two_eval) * (n_one_eval - n_three_eval) * (n_one_eval - n_four_eval) / (n_one_eval * n_one_eval * n_one_eval * n_one_eval) +
                          (1.0 / n_two_eval) +
                          //   (n_two_eval - (n_one_eval + n_three_eval + n_four_eval) / 3.0) / (n_two_eval * n_two_eval) +
                          //   (n_two_eval - n_one_eval) * (n_two_eval - n_three_eval) / (n_two_eval * n_two_eval * n_two_eval) +
                          //   (n_two_eval - n_one_eval) * (n_two_eval - n_three_eval) * (n_two_eval - n_four_eval) / (n_two_eval * n_two_eval * n_two_eval * n_two_eval) +
                          (1.0 / n_three_eval) +
                          //(n_three_eval - (n_one_eval + n_two_eval + n_four_eval) / 3.0) / (n_three_eval * n_three_eval) +
                          //   (n_three_eval - n_one_eval) * (n_three_eval - n_two_eval) / (n_three_eval * n_three_eval * n_three_eval) +
                          //   (n_three_eval - n_one_eval) * (n_three_eval - n_two_eval) * (n_three_eval - n_four_eval) / (n_three_eval * n_three_eval * n_three_eval * n_three_eval) +
                          (1.0 / n_four_eval)); // +
                                                //(n_four_eval - (n_one_eval + n_two_eval + n_three_eval) / 3.0) / (n_four_eval * n_four_eval));
        //   (n_four_eval - n_one_eval) * (n_four_eval - n_two_eval) / (n_four_eval * n_four_eval * n_four_eval) +
        //   (n_four_eval - n_one_eval) * (n_four_eval - n_two_eval) * (n_four_eval - n_three_eval) / (n_four_eval * n_four_eval * n_four_eval * n_four_eval));

        // compute I(1)
        Float base = 0.0;
        Float full = problem->eval_integral(all_samples);
        for (int i = 0; i < total_n; ++i)
        {
            // Float eval = problem->eval_integral(all_samples[i]);
            // base += (1.0 / all_samples[i]) +
            //         (1.0 / (all_samples[i] * all_samples[i])) * (eval - ((full * Float(total_n) - eval) / Float(total_n)));
            base += problem->eval(problem->eval_integral(all_samples[i]));
        }
        base /= Float(total_n);

        return base + (np1_contr - n_contr) / pmf;

        // return base +
        //        (0.5 * problem->eval(problem->eval_integral(np1_one)) +
        //         0.5 * problem->eval(problem->eval_integral(np1_two)) - 0.25 * (problem->eval(problem->eval_integral(n_one)) + problem->eval(problem->eval_integral(n_two)) + problem->eval(problem->eval_integral(n_three)) + problem->eval(problem->eval_integral(n_four)))) /
        //            pmf;

        // base_samples.push_back(problem->getBounds()->uniformSample(rng));
        // cost++;

        // int total_n = pow(2, n + 1);

        // for (int i = 0; i < total_n; ++i)
        // {
        //     Float loc = problem->getBounds()->uniformSample(rng);

        //     if (i % 2 == 0)
        //         podd_samples.push_back(loc);
        //     else
        //         peven_samples.push_back(loc);
        //     p1_samples.push_back(loc);
        //     cost++;
        // }

        // Float base = problem->eval(problem->eval_integral(base_samples));
        // Float odd = problem->eval(problem->eval_integral(podd_samples));
        // Float even = problem->eval(problem->eval_integral(peven_samples));
        // Float p1 = problem->eval(problem->eval_integral(p1_samples));

        // return base + (p1 - 0.5 * (odd + even)) / blanchet_pmf;
    }

    virtual std::string getName() const { return "blanchet_taylor"; }

    Float blanchet_r;
};