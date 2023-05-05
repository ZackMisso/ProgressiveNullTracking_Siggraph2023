#pragma once

#include <conver/conver.h>

struct Function
{
    Function(Float a, Float b) : a(a), b(b) {}

    virtual Float eval(Float x) const = 0;

    virtual void solve_for_mean(Float new_mean) {}

    // naively implements finite difference
    virtual Float eval_deriv(Float x) const
    {
        return (eval(x + 0.001) - eval(x - 0.001)) / 0.002;
    }

    virtual Float integrate() const
    {
        // evaluated using ray marching with an extremely small step size by
        // default. Analytic methods can extend this to be more accureate

        int steps = IntegrationSteps;
        Float step_size = (b - a) / Float(steps);
        Float od = 0.0;

        for (int i = 0; i < steps; ++i)
        {
            Float x = a + Float(i) * Float(step_size);
            od += eval(x) * step_size;
        }

        return od;
    }

    virtual Float integrate(Float tmp_a, Float tmp_b)
    {
        Float old_a = a;
        Float old_b = b;

        a = tmp_a;
        b = tmp_b;

        Float res = Function::integrate();

        a = old_a;
        b = old_b;

        return res;
    }

    // TODO: maybe throw an exception here
    virtual Float compute_max()
    {
        Float val = 0.0;
        for (Float x = a; x <= b; x += 0.0001)
        {
            val = std::max(val, eval(x));
        }
        return val;
    }

    virtual Float variance(Float loc) const
    {
        // this should ideally be extended, but for now naively implement
        Float delta = 0.05;

        Float var = 0.0;
        Float mean = 0.0;
        std::vector<Float> vals = std::vector<Float>(11);

        Float x = -delta + loc;
        for (int i = 0; i < 11; ++i)
        {
            vals[i] = eval(x);
            x += delta / 5.0;
        }

        for (int i = 0; i < vals.size(); ++i)
        {
            mean += vals[i];
        }
        mean /= Float(vals.size());

        for (int i = 0; i < vals.size(); ++i)
        {
            var += (mean - vals[i]) * (mean - vals[i]);
        }

        return var / Float(vals.size());
    }

    virtual std::string name() const = 0;

    Float a;
    Float b;
};

struct ConstantFunction : public Function
{
    ConstantFunction(Float amplitude, Float a, Float b)
        : Function(a, b),
          amplitude(amplitude) {}

    virtual Float eval(Float x) const
    {
        return amplitude;
    }

    virtual Float integrate() const
    {
        return amplitude * (b - a);
    }

    virtual Float compute_max() const
    {
        return amplitude;
    }

    virtual void solve_for_mean(Float mean)
    {
        amplitude = mean / (b - a);
    }

    virtual std::string name() const { return "const"; }

    Float amplitude;
};

struct BinaryFunction : public Function
{
    BinaryFunction(Float amplitude, Float a, Float b)
        : Function(a, b),
          amplitude(amplitude) {}

    virtual Float eval(Float x) const
    {
        if (x < (b - a) / 2.0)
            return 2.0 * amplitude;
        return 0.0;
    }

    virtual Float integrate() const
    {
        return amplitude * (b - a);
    }

    virtual Float compute_max() const
    {
        return 2.0 * amplitude;
    }

    virtual void solve_for_mean(Float mean)
    {
        amplitude = mean / (b - a);
    }

    virtual std::string name() const { return "binar"; }

    Float amplitude;
};

struct TriangleWaveFunction : public Function
{
    TriangleWaveFunction(Float period,
                         Float amplitude,
                         Float phase,
                         Float a,
                         Float b)
        : Function(a, b),
          period(period),
          amplitude(amplitude),
          phase(phase) {}

    virtual Float eval(Float x) const
    {
        Float local_x = mod(x + phase, period);

        if (local_x <= 0.5)
            return amplitude * local_x * 2.0;
        else
            return amplitude * (1.0 - (local_x - 0.5) * 2.0);
    }

    virtual std::string name() const { return "tri"; }

    Float period;
    Float amplitude;
    Float phase;
};

struct SquareWaveFunction : public Function
{
    SquareWaveFunction(Float period,
                       Float amplitude,
                       Float phase,
                       Float a,
                       Float b)
        : Function(a, b),
          period(period),
          amplitude(amplitude),
          phase(phase) {}

    virtual Float eval(Float x) const
    {
        Float local_x = mod(x + phase, period);

        if (local_x <= 0.5)
            return 0.0;
        return amplitude;
    }

    virtual std::string name() const { return "sqr"; }

    Float period;
    Float amplitude;
    Float phase;
};

struct SineWaveFunction : public Function
{
    SineWaveFunction(Float a, Float b)
        : Function(a, b),
          period(1.0),
          amplitude(1.0),
          phase(0.0) {}

    SineWaveFunction(Float period,
                     Float amplitude,
                     Float phase,
                     Float a,
                     Float b)
        : Function(a, b),
          period(period),
          amplitude(amplitude),
          phase(phase) {}

    SineWaveFunction(const SineWaveFunction &other)
        : Function(other.a, other.b),
          period(other.period),
          amplitude(other.amplitude),
          phase(other.phase) {}

    virtual Float integrate() const
    {
        Float ax = a / period * M_PI * 2.0 + phase;
        Float bx = b / period * M_PI * 2.0 + phase;

        return amplitude / (4.0 * M_PI) * (2.0 * M_PI * (b - a) + period * cos(ax) - period * cos(bx));
    }

    virtual void solve_for_mean(Float new_mean)
    {
        amplitude = 2.0 * new_mean / (b - a);
    }

    virtual Float compute_max() const
    {
        return amplitude;
    }

    virtual Float integrate(Float tmp_a, Float tmp_b)
    {
        Float old_a = a;
        Float old_b = b;

        a = tmp_a;
        b = tmp_b;

        Float res = integrate();

        a = old_a;
        b = old_b;

        return res;
    }

    virtual Float eval(Float x) const
    {
        return amplitude / 2.0 * (1.0 + sin(x / period * M_PI * 2.0 + phase));
    }

    virtual Float eval_deriv(Float x) const
    {
        return amplitude * M_PI / period * cos(x / period * M_PI * 2.0 + phase);
    }

    virtual std::string name() const { return "sin"; }

    Float period;
    Float amplitude;
    Float phase;
};

struct CompositeSineFunction : public Function
{
    CompositeSineFunction(pcg32 &rng, int num_funcs, Float a, Float b)
        : Function(a, b)
    {
        funcs = std::vector<SineWaveFunction>();

        Float min_amp = 0.1;
        Float max_amp = 2.0;

        Float min_phase = 0.0;
        Float max_phase = 4.0;

        Float min_period = 0.01;
        Float max_period = 0.5;

        for (int i = 0; i < num_funcs; ++i)
        {
            Float period = (max_period - min_period) * rng.nextDouble() + min_period;
            Float phase = (max_phase - min_phase) * rng.nextDouble() + min_phase;
            Float amp = (max_amp - min_amp) * rng.nextDouble() + min_amp;
            SineWaveFunction sin_func = SineWaveFunction(period, amp, phase, a, b);
            funcs.push_back(sin_func);
        }
    }

    CompositeSineFunction(const CompositeSineFunction &other)
        : Function(other.a, other.b)
    {
        funcs = std::vector<SineWaveFunction>();

        for (int i = 0; i < other.funcs.size(); ++i)
        {
            funcs.push_back(other.funcs[i]);
        }
    }

    virtual void normalize()
    {
        Float val = integrate();

        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude /= val;
        }
    }

    virtual Float compute_max()
    {
        Float val = 0.0;
        for (Float x = a; x <= b; x += 0.0001)
        {
            val = std::max(val, eval(x));
        }
        return val;
    }

    virtual void solve_for_mean(Float new_mean)
    {
        normalize();
        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude *= new_mean;
        }
    }

    virtual Float integrate() const
    {
        Float val = 0.0;
        for (int i = 0; i < funcs.size(); ++i)
        {
            val += funcs[i].integrate();
        }
        return val;
    }

    virtual Float integrate(Float tmp_a, Float tmp_b)
    {
        Float val = 0.0;
        for (int i = 0; i < funcs.size(); ++i)
        {
            val += funcs[i].integrate(tmp_a, tmp_b);
        }
        return val;
    }

    virtual Float eval(Float x) const
    {
        Float val = 0.0;
        for (int i = 0; i < funcs.size(); ++i)
        {
            val += funcs[i].eval(x);
        }
        return val;
    }

    virtual Float eval_deriv(Float x) const
    {
        Float val = 0.0;
        for (int i = 0; i < funcs.size(); ++i)
        {
            val += funcs[i].eval_deriv(x);
        }
        return val;
    }

    void set_mean(Float mean)
    {
        // this assumes that this function is normalized
        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude = funcs[i].amplitude * mean;
        }
    }

    virtual std::string name() const { return "sin_noise"; }

    std::vector<SineWaveFunction> funcs;
};

struct CompositeSineExpWeightedFunction : public Function
{
    CompositeSineExpWeightedFunction(pcg32 &rng, int num_funcs, Float we, Float a, Float b)
        : Function(a, b),
          we(we)
    {
        funcs = std::vector<SineWaveFunction>();

        Float min_amp = 0.1;
        Float max_amp = 2.0;

        Float min_phase = 0.0;
        Float max_phase = 4.0;

        Float min_period = 0.01;
        Float max_period = 0.5;

        for (int i = 0; i < num_funcs; ++i)
        {
            Float period = (max_period - min_period) * rng.nextDouble() + min_period;
            Float phase = (max_phase - min_phase) * rng.nextDouble() + min_phase;
            Float amp = (max_amp - min_amp) * rng.nextDouble() + min_amp;
            SineWaveFunction sin_func = SineWaveFunction(period, amp, phase, a, b);
            funcs.push_back(sin_func);
        }
    }

    CompositeSineExpWeightedFunction(const CompositeSineExpWeightedFunction &other)
        : Function(other.a, other.b)
    {
        funcs = std::vector<SineWaveFunction>();

        for (int i = 0; i < other.funcs.size(); ++i)
        {
            funcs.push_back(other.funcs[i]);
        }
    }

    virtual void normalize()
    {
        Float val = integrate();

        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude /= val;
        }
    }

    virtual Float compute_max()
    {
        Float val = 0.0;
        for (Float x = a; x <= b; x += 0.0001)
        {
            val = std::max(val, eval(x));
        }
        return val;
    }

    virtual void solve_for_mean(Float new_mean)
    {
        std::cout << "THIS IS BROKEN: solve_for_mean" << std::endl;
        normalize();
        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude *= new_mean;
        }
    }

    virtual Float eval(Float x) const
    {
        Float val = 0.0;
        for (int i = 0; i < funcs.size(); ++i)
        {
            val += funcs[i].eval(x);
        }
        if (x < 0.5)
            return val * exp((x - 0.5) * we);
        else
            return val * exp(std::min(x - 0.5, 0.3) * 2.5 * we);
    }

    void set_mean(Float mean)
    {
        std::cout << "THIS IS BROKEN: mean" << std::endl;
        // this assumes that this function is normalized
        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude = funcs[i].amplitude * mean;
        }
    }

    virtual std::string name() const { return "sin_noise"; }

    std::vector<SineWaveFunction> funcs;
    Float we;
};

struct CompositeSineStepWeightedFunction : public Function
{
    CompositeSineStepWeightedFunction(pcg32 &rng, int num_funcs, Float we1, Float we2, Float a, Float b)
        : Function(a, b),
          we1(we1),
          we2(we2)
    {
        funcs = std::vector<SineWaveFunction>();

        Float min_amp = 0.1;
        Float max_amp = 2.0;

        Float min_phase = 0.0;
        Float max_phase = 4.0;

        Float min_period = 0.01;
        Float max_period = 0.5;

        for (int i = 0; i < num_funcs; ++i)
        {
            Float period = (max_period - min_period) * rng.nextDouble() + min_period;
            Float phase = (max_phase - min_phase) * rng.nextDouble() + min_phase;
            Float amp = (max_amp - min_amp) * rng.nextDouble() + min_amp;
            SineWaveFunction sin_func = SineWaveFunction(period, amp, phase, a, b);
            funcs.push_back(sin_func);
        }
    }

    CompositeSineStepWeightedFunction(const CompositeSineStepWeightedFunction &other)
        : Function(other.a, other.b)
    {
        funcs = std::vector<SineWaveFunction>();

        for (int i = 0; i < other.funcs.size(); ++i)
        {
            funcs.push_back(other.funcs[i]);
        }
    }

    virtual void normalize()
    {
        Float val = integrate();

        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude /= val;
        }
    }

    virtual Float compute_max()
    {
        Float val = 0.0;
        for (Float x = a; x <= b; x += 0.0001)
        {
            val = std::max(val, eval(x));
        }
        return val;
    }

    virtual void solve_for_mean(Float new_mean)
    {
        normalize();
        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude *= new_mean;
        }
    }

    virtual Float eval(Float x) const
    {
        Float val = 0.0;
        for (int i = 0; i < funcs.size(); ++i)
        {
            val += funcs[i].eval(x);
        }
        if (x < 1.0)
            return val + we1;
        else
            return val + we2;
    }

    void set_mean(Float mean)
    {
        std::cout << "THIS IS BROKEN: mean" << std::endl;
        // this assumes that this function is normalized
        for (int i = 0; i < funcs.size(); ++i)
        {
            funcs[i].amplitude = funcs[i].amplitude * mean;
        }
    }

    virtual std::string name() const { return "sin_noise"; }

    std::vector<SineWaveFunction> funcs;
    Float we1;
    Float we2;
};

struct TransmittanceMeasureFunction : public Function
{
    TransmittanceMeasureFunction(Function *other, Float a, Float b)
        : Function(a, b),
          func(other)
    {
    }

    TransmittanceMeasureFunction(const TransmittanceMeasureFunction &other)
        : Function(other.a, other.b),
          func(other.func)
    {
    }

    virtual void normalize()
    {
        std::cout << "TransmittanceMeasureFunction: does not normalize" << std::endl;
    }

    virtual Float compute_max()
    {
        return 1.0;
    }

    virtual void solve_for_mean(Float new_mean)
    {
        std::cout << "TransmittanceMeasureFunction: does not solve_for_mean" << std::endl;
    }

    virtual Float eval(Float x) const
    {
        return exp(-func->integrate(0.0, x));
    }

    void set_mean(Float mean)
    {
        std::cout << "TransmittanceMeasureFunction: does not set_mean" << std::endl;
    }

    virtual std::string name() const { return "transmittance"; }

    Function *func;
};

struct PowerLawFunction : public Function
{
    PowerLawFunction(Float a, Float b)
        : Function(a, b),
          alpha(1.0) {}

    PowerLawFunction(Float alpha,
                     Float a,
                     Float b)
        : Function(a, b),
          alpha(alpha) {}

    PowerLawFunction(const PowerLawFunction &other)
        : Function(other.a, other.b),
          alpha(other.alpha) {}

    // TODO: maybe store the pows for a and b if integrate will be called many times
    virtual Float integrate() const
    {
        if (alpha >= 0.0 && alpha < 1.0)
            return (std::pow(b, 1.0 - alpha) - std::pow(a, 1.0 - alpha)) * (1.0 / (1.0 - alpha));
        std::cout << "ALPHA SHOULD ONLY BE BETWEEN 0 AND 1" << std::endl;
        return -1.0;
    }

    virtual Float integrate(Float tmp_a, Float tmp_b)
    {
        Float old_a = a;
        Float old_b = b;

        a = tmp_a;
        b = tmp_b;

        Float res = integrate();

        a = old_a;
        b = old_b;

        return res;
    }

    virtual Float eval(Float x) const
    {
        // if (x == 0.0)
        //     return 1.0 / std::pow(1.0, alpha);
        return 1.0 / std::pow(1.0 - x, alpha);
    }

    virtual Float eval_deriv(Float x) const
    {
        return 0.0;
        std::cout << "this might not be needed" << std::endl;
        // return amplitude * M_PI / period * cos(x / period * M_PI * 2.0 + phase);
    }

    virtual std::string name() const { return "law_" + std::to_string(alpha); }

    Float alpha;
};

struct PointyWaveFunction : public Function
{
    PointyWaveFunction(Float period,
                       Float amplitude,
                       Float phase,
                       Float a,
                       Float b)
        : Function(a, b),
          period(period),
          amplitude(amplitude),
          phase(phase) {}

    virtual Float eval_deriv(Float x) const
    {
        Float xx = x - phase;
        xx = xx / period;
        xx = xx - floor(xx);
        xx = abs(2.0 * xx - 1.0);

        return exp(-amplitude * xx * period / 2.0);
    }

    virtual std::string name() const { return "pointy"; }

    Float period;
    Float amplitude;
    Float phase;
};

struct LinIncFunction : public Function
{
    LinIncFunction(Float mean,
                   Float a,
                   Float b)
        : Function(a, b),
          mean(mean) {}

    virtual Float integrate() const
    {
        return mean * (b - a);
    }

    virtual Float eval(Float x) const
    {
        return (x - a) / (b - a) * 2.0 * mean;
    }

    virtual Float eval_deriv(Float x) const
    {
        return (2.0 * mean) / (b - a);
    }

    virtual std::string name() const { return "lin"; }

    Float mean;
};

struct GaussianFunction : public Function
{
    GaussianFunction(Float amplitude,
                     Float sig,
                     Float m,
                     Float a,
                     Float b)
        : Function(a, b),
          amplitude(amplitude),
          sig(sig),
          m(m) {}

    virtual Float eval(Float x) const
    {
        Float exponent = (x - m) * (x - m) / (2.0 * sig * sig);
        return amplitude * std::exp(-exponent);
    }

    virtual Float integrate() const
    {
        Float resolution = 1000000.0;
        Float sum = 0.0;
        Float stepSize = (b - a) / resolution;

        for (Float i = 0.0; i < b; i += stepSize)
        {
            sum += stepSize * eval(i);
        }

        return sum;
    }

    virtual Float compute_max() const
    {
        return amplitude;
    }

    virtual void solve_for_mean(Float mean)
    {
        Float current_mean = integrate() / (b - a);
        Float new_amp = mean * amplitude / current_mean;
        amplitude = new_amp;
    }

    virtual std::string name() const { return "gauss"; }

    Float amplitude;
    Float sig;
    Float m;
};

// lol, this is a visualization hack
struct RatioTrackFunction : public Function
{
    RatioTrackFunction(pcg32 &rng,
                       Function *func,
                       Float maj,
                       Float a,
                       Float b)
        : Function(a, b)
    {
        xs = std::vector<Float>();
        evals = std::vector<Float>();

        Float x = a;
        Float Tr = 1.0;

        Float invMaj = 1.0 / maj;

        while (true)
        {
            xs.push_back(x);
            evals.push_back(Tr);

            x -= std::log(1 - rng.nextDouble()) * invMaj;
            if (x >= b)
                break;

            xs.push_back(x);
            evals.push_back(Tr);

            Tr *= (maj - func->eval(x)) * invMaj;
        }
        xs.push_back(b);
        evals.push_back(Tr);
    }

    virtual Float eval(Float x) const
    {
        int index = 1;
        while (xs[index] < x)
        {
            index++;
        }
        return evals[index--];
    }

    virtual std::string name() const { return "ratio"; }

    std::vector<Float> xs;
    std::vector<Float> evals;
    Float maj;
};

// lol, this is a visualization hack
struct AverageRatioTrackFunction : public Function
{
    AverageRatioTrackFunction(pcg32 &rng,
                              Function *func,
                              Float maj,
                              Float a,
                              Float b,
                              int num)
        : Function(a, b)
    {
        ratios = std::vector<RatioTrackFunction>();
        for (int i = 0; i < num; ++i)
        {
            ratios.push_back(RatioTrackFunction(rng, func, maj, a, b));
        }
    }

    virtual Float eval(Float x) const
    {
        Float average = 0.0;

        for (int i = 0; i < ratios.size(); ++i)
        {
            average = average + 1.0 / Float(i + 1) * (ratios[i].eval(x) - average);
        }

        return average;
    }

    virtual std::string name() const { return "avg_ratio"; }

    std::vector<RatioTrackFunction> ratios;
};

// struct ConstFunction : public Function
// {
//     ConstFunction(Float mean,
//                   Float a,
//                   Float b)
//         : Function(a, b),
//           mean(mean) {}

//     virtual Float integrate() const
//     {
//         return mean * (b - a);
//     }

//     virtual Float eval(Float x) const
//     {
//         if (x < a || x > b)
//             return 0.0;
//         return mean;
//     }

//     virtual Float eval_deriv(Float x) const
//     {
//         return 0.0;
//     }

//     virtual std::string name() const { return "const"; }

//     Float mean;
// };