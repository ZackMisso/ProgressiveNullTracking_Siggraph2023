#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_BLANCHETCORRES_H
#define PBRT_CORE_BLANCHETCORRES_H

#include "pbrt.h"
#include "spectrum.h"

namespace pbrt
{

// TODO; maybe store pmf here too
struct DebiasedClampingSpectrum
{
    DebiasedClampingSpectrum()
        : biased(0.f), top(0.f), bot(0.f), k(0) { }

    DebiasedClampingSpectrum(Spectrum base)
        : biased(base), top(base), bot(base), k(0) { }

    DebiasedClampingSpectrum(const DebiasedClampingSpectrum& other)
        : biased(other.biased),
          top(other.top),
          bot(other.bot),
          k(other.k) { }

    DebiasedClampingSpectrum(Spectrum biased,
                             Spectrum top,
                             Spectrum bot,
                             int k)
        : biased(biased), top(top), bot(bot), k(k) { }

    void setAll(Spectrum val)
    {
        biased = val;
        top = val;
        bot = val;
    }

    // TODO: idk if this is really correct for all of these
    DebiasedClampingSpectrum& operator*=(const DebiasedClampingSpectrum& res)
    {
        biased *= res.biased;
        top *= res.top;
        bot *= res.bot;

        return *this;
    }

    DebiasedClampingSpectrum& operator+=(const DebiasedClampingSpectrum& res)
    {
        biased += res.biased;
        top += res.top;
        bot += res.bot;

        return *this;
    }

    DebiasedClampingSpectrum& operator*=(const Spectrum& val)
    {
        biased *= val;
        top *= val;
        bot *= val;

        return *this;
    }

    DebiasedClampingSpectrum& operator+=(const Spectrum& val)
    {
        biased += val;
        top += val;
        bot += val;

        return *this;
    }

    DebiasedClampingSpectrum& operator/=(const Spectrum& val)
    {
        biased = biased / val;
        top = top / val;
        bot = bot / val;

        return *this;
    }

    DebiasedClampingSpectrum& operator-=(const Spectrum& val)
    {
        biased = biased - val;
        top = top - val;
        bot = bot - val;

        return *this;
    }

    DebiasedClampingSpectrum& operator*=(Float val)
    {
        biased *= val;
        top *= val;
        bot *= val;

        return *this;
    }

    DebiasedClampingSpectrum& operator+=(Float val)
    {
        biased += val;
        top += val;
        bot += val;

        return *this;
    }

    DebiasedClampingSpectrum& operator/=(Float val)
    {
        biased /= val;
        top /= val;
        bot /= val;

        return *this;
    }

    DebiasedClampingSpectrum& operator-=(Float val)
    {
        biased = biased - val;
        top = top - val;
        bot = bot - val;

        return *this;
    }

    DebiasedClampingSpectrum operator*(const DebiasedClampingSpectrum &res) const
    {
        return DebiasedClampingSpectrum(biased * res.biased,
                                        top * res.top,
                                        bot * res.bot,
                                        k);
    }

    DebiasedClampingSpectrum operator+(const DebiasedClampingSpectrum &res) const
    {
        return DebiasedClampingSpectrum(biased + res.biased,
                                        top + res.top,
                                        bot + res.bot,
                                        k);
    }

    DebiasedClampingSpectrum operator*(const Spectrum& val) const
    {
        return DebiasedClampingSpectrum(biased * val,
                                        top * val,
                                        bot * val,
                                        k);
    }

    DebiasedClampingSpectrum operator/(const Spectrum& val) const
    {
        return DebiasedClampingSpectrum(biased / val,
                                        top / val,
                                        bot / val,
                                        k);
    }

    DebiasedClampingSpectrum operator*(Float val) const
    {
        return DebiasedClampingSpectrum(biased * val,
                                        top * val,
                                        bot * val,
                                        k);
    }

    DebiasedClampingSpectrum operator/(Float val) const
    {
        return DebiasedClampingSpectrum(biased / val,
                                        top / val,
                                        bot / val,
                                        k);
    }

    DebiasedClampingSpectrum operator+(Float val) const
    {
        return DebiasedClampingSpectrum(biased + val,
                                        top + val,
                                        bot + val,
                                        k);
    }

    bool IsBlack() const
    {
        return biased.IsBlack();
    }

    Spectrum biased;
    Spectrum top;
    Spectrum bot;
    int k;
};

struct BlanchetCorrelatedTs
{
    BlanchetCorrelatedTs()
        : all(0.f),
          odd(0.f),
          even(0.f),
          bias(0.f) { }

    BlanchetCorrelatedTs(Float val)
      : all(val),
        odd(val),
        even(val),
        bias(val) { }

    Float all;
    Float odd;
    Float even;
    Float bias;
};

// TODO: This will get extremely complicated very quickly after all of this is
//       actually derived and implemented. This is basically going to be a
//       placeholder.
struct BlanchetDifferentialResults
{
    BlanchetDifferentialResults()
        : I_0(Spectrum(0.f)),
          I_x(Spectrum(0.f)),
          dI_x(Spectrum(0.f)) { }

    BlanchetDifferentialResults(Spectrum val)
        : I_0(val),
          I_x(val),
          dI_x(val) { }

    BlanchetDifferentialResults(Spectrum I_0,
                                Spectrum I_x,
                                Spectrum dI_x) :
        I_0(I_0),
        I_x(I_x),
        dI_x(dI_x) { }

    // TODO: idk if this is really correct for all of these
    BlanchetDifferentialResults& operator*=(const BlanchetDifferentialResults& res)
    {
        I_0 *= res.I_0;
        I_x *= res.I_x;
        dI_x *= res.dI_x;

        return *this;
    }

    BlanchetDifferentialResults& operator+=(const BlanchetDifferentialResults& res)
    {
        I_0 += res.I_0;
        I_x += res.I_x;
        dI_x += res.dI_x;

        return *this;
    }

    BlanchetDifferentialResults& operator*=(const Spectrum& val)
    {
        I_0 *= val;
        I_x *= val;
        dI_x *= val;

        return *this;
    }

    BlanchetDifferentialResults& operator+=(const Spectrum& val)
    {
        I_0 += val;
        I_x += val;
        dI_x += val;

        return *this;
    }

    BlanchetDifferentialResults& operator/=(const Spectrum& val)
    {
        I_0 = I_0 / val;
        I_x = I_x / val;
        dI_x = dI_x / val;

        return *this;
    }

    BlanchetDifferentialResults& operator-=(const Spectrum& val)
    {
        I_0 = I_0 - val;
        I_x = I_x - val;
        dI_x = dI_x - val;

        return *this;
    }

    BlanchetDifferentialResults& operator*=(Float val)
    {
        I_0 *= val;
        I_x *= val;
        dI_x *= val;

        return *this;
    }

    BlanchetDifferentialResults& operator+=(Float val)
    {
        I_0 += val;
        I_x += val;
        dI_x += val;

        return *this;
    }

    BlanchetDifferentialResults& operator/=(Float val)
    {
        I_0 /= val;
        I_x /= val;
        dI_x /= val;

        return *this;
    }

    BlanchetDifferentialResults& operator-=(Float val)
    {
        I_0 = I_0 - val;
        I_x = I_x - val;
        dI_x = dI_x - val;

        return *this;
    }

    BlanchetDifferentialResults operator*(const BlanchetDifferentialResults &res) const
    {
        return BlanchetDifferentialResults(I_0 * res.I_0,
                                           I_x * res.I_x,
                                           dI_x * res.dI_x);
    }

    BlanchetDifferentialResults operator+(const BlanchetDifferentialResults &res) const
    {
        return BlanchetDifferentialResults(I_0 + res.I_0,
                                           I_x + res.I_x,
                                           dI_x + res.dI_x);
    }

    BlanchetDifferentialResults operator*(const Spectrum& val) const
    {
        return BlanchetDifferentialResults(I_0 * val,
                                           I_x * val,
                                           dI_x * val);
    }

    BlanchetDifferentialResults operator/(const Spectrum& val) const
    {
        return BlanchetDifferentialResults(I_0 / val,
                                           I_x / val,
                                           dI_x / val);
    }

    BlanchetDifferentialResults operator*(Float val) const
    {
        return BlanchetDifferentialResults(I_0 * val,
                                           I_x * val,
                                           dI_x * val);
    }

    BlanchetDifferentialResults operator/(Float val) const
    {
        return BlanchetDifferentialResults(I_0 / val,
                                           I_x / val,
                                           dI_x / val);
    }

    BlanchetDifferentialResults operator+(Float val) const
    {
        return BlanchetDifferentialResults(I_0 + val,
                                           I_x + val,
                                           dI_x + val);
    }

    void exponentiate()
    {
        I_0 = Exp(-I_0);
        I_x = Exp(-I_x);
        dI_x = Exp(-dI_x);
    }

    bool IsBlack() const
    {
        return I_0.IsBlack();
    }

    Spectrum I_0;
    Spectrum I_x;
    Spectrum dI_x;
};

// TODO: find a new place for this eventually
struct BlanchetCorrelatedResults
{
    BlanchetCorrelatedResults()
        : all(Spectrum(0.f)),
          odd(Spectrum(0.f)),
          even(Spectrum(0.f)),
          bias(Spectrum(0.f)) { }

    BlanchetCorrelatedResults(Spectrum all,
                              Spectrum odd,
                              Spectrum even,
                              Spectrum bias)
        : all(all),
          odd(odd),
          even(even),
          bias(bias) { }

    BlanchetCorrelatedResults(Spectrum base)
        : all(base),
          odd(base),
          even(base),
          bias(base) { }

          BlanchetCorrelatedResults(const BlanchetCorrelatedResults& other)
              : all(other.all),
                odd(other.odd),
                even(other.even),
                bias(other.bias) { }

    BlanchetCorrelatedResults& operator*=(const BlanchetCorrelatedResults& res)
    {
        all *= res.all;
        odd *= res.odd;
        even *= res.even;
        bias *= res.bias;

        return *this;
    }

    BlanchetCorrelatedResults& operator+=(const BlanchetCorrelatedResults& res)
    {
        all += res.all;
        odd += res.odd;
        even += res.even;
        bias += res.bias;

        return *this;
    }

    BlanchetCorrelatedResults& operator*=(const Spectrum& val)
    {
        all *= val;
        odd *= val;
        even *= val;
        bias *= val;

        return *this;
    }

    BlanchetCorrelatedResults& operator+=(const Spectrum& val)
    {
        all += val;
        odd += val;
        even += val;
        bias += val;

        return *this;
    }

    BlanchetCorrelatedResults& operator/=(const Spectrum& val)
    {
        all = all / val;
        odd = odd / val;
        even = even / val;
        bias = bias / val;

        return *this;
    }

    BlanchetCorrelatedResults& operator-=(const Spectrum& val)
    {
        all = all - val;
        odd = odd - val;
        even = even - val;
        bias = bias - val;

        return *this;
    }

    BlanchetCorrelatedResults& operator*=(Float val)
    {
        all *= val;
        odd *= val;
        even *= val;
        bias *= val;

        return *this;
    }

    BlanchetCorrelatedResults& operator+=(Float val)
    {
        all += val;
        odd += val;
        even += val;
        bias += val;

        return *this;
    }

    BlanchetCorrelatedResults& operator/=(Float val)
    {
        all /= val;
        odd /= val;
        even /= val;
        bias /= val;

        return *this;
    }

    BlanchetCorrelatedResults& operator-=(Float val)
    {
        all = all - val;
        odd = odd - val;
        even = even - val;
        bias = bias - val;

        return *this;
    }

    BlanchetCorrelatedResults operator*(const BlanchetCorrelatedResults &res) const
    {
        return BlanchetCorrelatedResults(all * res.all,
                                         odd * res.odd,
                                         even * res.even,
                                         bias * res.bias);
    }

    BlanchetCorrelatedResults operator+(const BlanchetCorrelatedResults &res) const
    {
        return BlanchetCorrelatedResults(all + res.all,
                                         odd + res.odd,
                                         even + res.even,
                                         bias + res.bias);
    }

    BlanchetCorrelatedResults operator/(const BlanchetCorrelatedResults& res) const
    {
        return BlanchetCorrelatedResults(all / res.all,
                                         odd / res.odd,
                                         even / res.even,
                                         bias / res.bias);
    }

    BlanchetCorrelatedResults operator*(const Spectrum& val) const
    {
        return BlanchetCorrelatedResults(all * val,
                                         odd * val,
                                         even * val,
                                         bias * val);
    }

    BlanchetCorrelatedResults operator/(const Spectrum& val) const
    {
        return BlanchetCorrelatedResults(all / val,
                                         odd / val,
                                         even / val,
                                         bias / val);
    }

    BlanchetCorrelatedResults operator*(Float val) const
    {
        return BlanchetCorrelatedResults(all * val,
                                         odd * val,
                                         even * val,
                                         bias * val);
    }

    BlanchetCorrelatedResults operator/(Float val) const
    {
        return BlanchetCorrelatedResults(all / val,
                                         odd / val,
                                         even / val,
                                         bias / val);
    }

    BlanchetCorrelatedResults operator+(Float val) const
    {
        return BlanchetCorrelatedResults(all + val,
                                         odd + val,
                                         even + val,
                                         bias + val);
    }

    void exponentiate()
    {
        all = Exp(-all);
        odd = Exp(-odd);
        even = Exp(-even);
        bias = Exp(-bias);
    }

    bool IsBlack() const
    {
        return bias.IsBlack();
    }

    Spectrum all;
    Spectrum odd;
    Spectrum even;
    Spectrum bias;
};

struct BlanchetCorrelatedFdDebiasing
{
    // this needs to store 4 values per beta, and has to keep track of 7 betas

    BlanchetCorrelatedFdDebiasing()
    {
        initialize_data();

        for (int i = 0; i < 7; ++i)
        {
            all[i] = Spectrum(0.f);
            odd[i] = Spectrum(0.f);
            even[i] = Spectrum(0.f);
            bias[i] = Spectrum(0.f);
        }
    }

    BlanchetCorrelatedFdDebiasing(const BlanchetCorrelatedFdDebiasing& other)
    {
        initialize_data();

        for (int i = 0; i < 7; ++i)
        {
            all[i] = other.all[i];
            odd[i] = other.odd[i];
            even[i] = other.even[i];
            bias[i] = other.bias[i];
        }
    }

    BlanchetCorrelatedFdDebiasing(Spectrum all_val,
                                  Spectrum odd_val,
                                  Spectrum even_val,
                                  Spectrum bias_val)
    {
        initialize_data();

        for (int i = 0; i < 7; ++i)
        {
            all[i] = all_val;
            odd[i] = odd_val;
            even[i] = even_val;
            bias[i] = bias_val;
        }
    }

    BlanchetCorrelatedFdDebiasing(Spectrum base)
    {
        initialize_data();

        for (int i = 0; i < 7; ++i)
        {
            all[i] = base;
            odd[i] = base;
            even[i] = base;
            bias[i] = base;
        }
    }

    BlanchetCorrelatedFdDebiasing(Float base)
    {
        initialize_data();

        for (int i = 0; i < 7; ++i)
        {
            all[i] = Spectrum(base);
            odd[i] = Spectrum(base);
            even[i] = Spectrum(base);
            bias[i] = Spectrum(base);
        }
    }

    void initialize_data()
    {
        all = std::vector<Spectrum>(7);
        odd = std::vector<Spectrum>(7);
        even = std::vector<Spectrum>(7);
        bias = std::vector<Spectrum>(7);
    }

    void setAll(Spectrum base)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] = base;
            odd[i] = base;
            even[i] = base;
            bias[i] = base;
        }
    }

    BlanchetCorrelatedFdDebiasing& operator*=(const BlanchetCorrelatedFdDebiasing& res)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] *= res.all[i];
            odd[i] *= res.odd[i];
            even[i] *= res.even[i];
            bias[i] *= res.bias[i];
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator+=(const BlanchetCorrelatedFdDebiasing& res)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] += res.all[i];
            odd[i] += res.odd[i];
            even[i] += res.even[i];
            bias[i] += res.bias[i];
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator*=(const Spectrum& val)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] *= val;
            odd[i] *= val;
            even[i] *= val;
            bias[i] *= val;
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator+=(const Spectrum& val)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] += val;
            odd[i] += val;
            even[i] += val;
            bias[i] += val;
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator/=(const Spectrum& val)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] = all[i] / val;
            odd[i] = odd[i] / val;
            even[i] = even[i] / val;
            bias[i] = bias[i] / val;
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator-=(const Spectrum& val)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] = all[i] - val;
            odd[i] = odd[i] - val;
            even[i] = even[i] - val;
            bias[i] = bias[i] - val;
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator*=(Float val)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] *= val;
            odd[i] *= val;
            even[i] *= val;
            bias[i] *= val;
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator+=(Float val)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] += val;
            odd[i] += val;
            even[i] += val;
            bias[i] += val;
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator/=(Float val)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] /= val;
            odd[i] /= val;
            even[i] /= val;
            bias[i] /= val;
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing& operator-=(Float val)
    {
        for (int i = 0; i < 7; ++i)
        {
            all[i] = all[i] - val;
            odd[i] = odd[i] - val;
            even[i] = even[i] - val;
            bias[i] = bias[i] - val;
        }

        return *this;
    }

    BlanchetCorrelatedFdDebiasing operator*(const BlanchetCorrelatedFdDebiasing &res) const
    {
        BlanchetCorrelatedFdDebiasing ret = res;

        for (int i = 0; i < 7; ++i)
        {
            ret.all[i] *= all[i];
            ret.even[i] *= even[i];
            ret.odd[i] *= odd[i];
            ret.bias[i] *= bias[i];
        }

        return ret;
    }

    BlanchetCorrelatedFdDebiasing operator+(const BlanchetCorrelatedFdDebiasing &res) const
    {
        BlanchetCorrelatedFdDebiasing ret = res;

        for (int i = 0; i < 7; ++i)
        {
            ret.all[i] += all[i];
            ret.even[i] += even[i];
            ret.odd[i] += odd[i];
            ret.bias[i] += bias[i];
        }

        return ret;
    }

    BlanchetCorrelatedFdDebiasing operator*(const Spectrum& val) const
    {
        BlanchetCorrelatedFdDebiasing ret = BlanchetCorrelatedFdDebiasing(val);

        for (int i = 0; i < 7; ++i)
        {
            ret.all[i] *= all[i];
            ret.even[i] *= even[i];
            ret.odd[i] *= odd[i];
            ret.bias[i] *= bias[i];
        }

        return ret;
    }

    BlanchetCorrelatedFdDebiasing operator/(const Spectrum& val) const
    {
        BlanchetCorrelatedFdDebiasing ret = BlanchetCorrelatedFdDebiasing(val);

        for (int i = 0; i < 7; ++i)
        {
            ret.all[i] = all[i] / ret.all[i];
            ret.even[i] = even[i] / ret.even[i];
            ret.odd[i] = odd[i] / ret.odd[i];
            ret.bias[i] = bias[i] / ret.bias[i];
        }

        return ret;
    }

    BlanchetCorrelatedFdDebiasing operator*(Float val) const
    {
        BlanchetCorrelatedFdDebiasing ret = BlanchetCorrelatedFdDebiasing(val);

        for (int i = 0; i < 7; ++i)
        {
            ret.all[i] *= all[i];
            ret.even[i] *= even[i];
            ret.odd[i] *= odd[i];
            ret.bias[i] *= bias[i];
        }

        return ret;
    }

    BlanchetCorrelatedFdDebiasing operator/(Float val) const
    {
        BlanchetCorrelatedFdDebiasing ret = BlanchetCorrelatedFdDebiasing(val);

        for (int i = 0; i < 7; ++i)
        {
            ret.all[i] = all[i] / ret.all[i];
            ret.even[i] = even[i] / ret.even[i];
            ret.odd[i] = odd[i] / ret.odd[i];
            ret.bias[i] = bias[i] / ret.bias[i];
        }

        return ret;
    }

    BlanchetCorrelatedFdDebiasing operator+(Float val) const
    {
        BlanchetCorrelatedFdDebiasing ret = BlanchetCorrelatedFdDebiasing(val);

        for (int i = 0; i < 7; ++i)
        {
            ret.all[i] += all[i];
            ret.even[i] += even[i];
            ret.odd[i] += odd[i];
            ret.bias[i] += bias[i];
        }

        return ret;
    }

    void exponentiate()
    {
        std::cout << "if you see this something is very wrong" << std::endl;
    }

    bool IsBlack() const
    {
        return bias[0].IsBlack();
    }

    // everything is stored in this order:
    // 0
    // +h
    // -h
    // +h1
    // -h1
    // +h2
    // -h2
    std::vector<Spectrum> all;
    std::vector<Spectrum> odd;
    std::vector<Spectrum> even;
    std::vector<Spectrum> bias;
};

}  // namespace pbrt

#endif  // PBRT_CORE_BLANCHETCORRES_H
