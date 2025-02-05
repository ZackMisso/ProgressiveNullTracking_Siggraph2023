// #pragma once

// #include <conver/conver.h>
// #include <conver/problem.h>

// template <typename T>
// struct BlanchetBinFinal : Technique<T>
// {
//     BlanchetBinFinal(Float r = 0.65, int base_n = 0) : blanchet_r(r), base_n(0) {}

//     virtual T eval_problem(const Problem<T> *problem, int samples, int &cost, pcg32 &rng)
//     {
//         Float a = 0.0;
//         Float b = 1.0;

//         // choose n and compute its pmf
//         int n = std::ceil(log(1.0 - rng.nextDouble()) / log(1.0 - blanchet_r)) - 1;
//         if (n < 0)
//             n = 0; // this can happen apparently
//         Float blanchet_pmf = blanchet_r * std::pow((1.0 - blanchet_r), n);

//         n += base_n;

//         Float min_val = pow(1.0 / 2.0, base_n + 1);
//         Float min_val_p = pow(1.0 / 2.0, n + 1);
//         Float min_val_p1 = pow(1.0 / 2.0, n + 2);

//         // sample the n evals
//         int total_n = pow(2, n + 1);

//         Float base = 0.0;
//         Float epsilon = rng.nextDouble();
//         Float loc = a;
//         Float delta = (b - a) / Float(total_n);

//         Float p_1 = 0.0;
//         Float p_2 = 0.0;
//         Float p1 = 0.0;

//         int full_base_n = pow(2, base_n);
//         std::vector<Float> bases = std::vector<Float>(total_n / full_base_n);
//         for (int i = 0; i < bases.size(); ++i)
//         {
//             bases[i] = 0.0;
//         }

//         for (int i = 0; i < total_n; ++i)
//         {
//             Float loc = (epsilon + i) * delta;

//             Float eval = problem->eval_integral(loc);

//             int iter = i / (total_n / full_base_n);
//             int res = i % (total_n / full_base_n);
//             bases[res] = bases[res] + 1.0 / double(iter + 1) * (eval - bases[res]);

//             if (i % 2 == 0)
//                 p_1 += eval;
//             else
//                 p_2 += eval;
//             p1 += eval;
//             cost++;
//         }

//         for (int i = 0; i < bases.size(); ++i)
//         {
//             base += problem->eval(bases[i], min_val);
//         }
//         base /= Float(bases.size());

//         p_1 /= Float(total_n / 2);
//         p_2 /= Float(total_n / 2);
//         p1 /= Float(total_n);

//         // compute the reciprocals
//         p_1 = problem->eval(p_1, min_val_p);
//         p_2 = problem->eval(p_2, min_val_p);
//         p1 = problem->eval(p1, min_val_p1);

//         // return the result
//         return base + (p1 - 0.5 * (p_1 + p_2)) / blanchet_pmf;
//     }

//     virtual void setMaj(T val) { base_step = val; }
//     virtual std::string getName() const { return "blanchet_" + std::to_string(base_n); }

//     Float blanchet_r;
//     Float base_step;
//     int base_n;
// };