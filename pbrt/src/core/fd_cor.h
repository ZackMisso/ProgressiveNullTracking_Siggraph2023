#pragma once

#include "pbrt.h"
#include "spectrum.h"

namespace pbrt
{

struct FD_DebCor_Sigma_T_Spectrum
{
    FD_DebCor_Sigma_T_Spectrum(Spectrum L_0,
                               Spectrum L_H,
                               Spectrum L_H1,
                               Spectrum L_H2,
                               Float h,
                               Float h1,
                               Float h2,
                               Float pmf,
                               Float sample_sig,
                               Float sigma_t0,
                               Float sigma_s0,
                               Float sigma_th,
                               Float sigma_sh,
                               Float sigma_th1,
                               Float sigma_sh1,
                               Float sigma_th2,
                               Float sigma_sh2)
        : L_0(L_0),
          L_H(L_H),
          L_H1(L_H1),
          L_H2(L_H2),
          h(h),
          h1(h1),
          h2(h2),
          pmf(pmf),
          sample_sig(sample_sig),
          sigma_t0(sigma_t0),
          sigma_s0(sigma_s0),
          sigma_th(sigma_th),
          sigma_sh(sigma_sh),
          sigma_th1(sigma_th1),
          sigma_sh1(sigma_sh1),
          sigma_th2(sigma_th2),
          sigma_sh2(sigma_sh2) { }

    FD_DebCor_Sigma_T_Spectrum(Float h,
                               Float h1,
                               Float h2,
                               Float pmf,
                               Float sample_sig,
                               Float sigma_t0,
                               Float sigma_s0,
                               Float sigma_th,
                               Float sigma_sh,
                               Float sigma_th1,
                               Float sigma_sh1,
                               Float sigma_th2,
                               Float sigma_sh2)
        : L_0(Spectrum(0.f)),
          L_H(Spectrum(0.f)),
          L_H1(Spectrum(0.f)),
          L_H2(Spectrum(0.f)),
          h(h),
          h1(h1),
          h2(h2),
          pmf(pmf),
          sample_sig(sample_sig),
          sigma_t0(sigma_t0),
          sigma_s0(sigma_s0),
          sigma_th(sigma_th),
          sigma_sh(sigma_sh),
          sigma_th1(sigma_th1),
          sigma_sh1(sigma_sh1),
          sigma_th2(sigma_th2),
          sigma_sh2(sigma_sh2)  { }

    FD_DebCor_Sigma_T_Spectrum(const FD_DebCor_Sigma_T_Spectrum& other)
        : L_0(other.L_0),
          L_H(other.L_H),
          L_H1(other.L_H1),
          L_H2(other.L_H2),
          h(other.h),
          h1(other.h1),
          h2(other.h2),
          pmf(other.pmf),
          sample_sig(other.sample_sig),
          sigma_t0(other.sigma_t0),
          sigma_s0(other.sigma_s0),
          sigma_th(other.sigma_th),
          sigma_sh(other.sigma_sh),
          sigma_th1(other.sigma_th1),
          sigma_sh1(other.sigma_sh1),
          sigma_th2(other.sigma_th2),
          sigma_sh2(other.sigma_sh2) { }

    void setAll(Spectrum val)
    {
        L_0 = val;
        L_H = val;
        L_H1 = val;
        L_H2 = val;
    }

    // TODO: idk if this is really correct for all of these
    FD_DebCor_Sigma_T_Spectrum& operator*=(const FD_DebCor_Sigma_T_Spectrum& res)
    {
        L_0 *= res.L_0;
        L_H *= res.L_H;
        L_H1 *= res.L_H1;
        L_H2 *= res.L_H2;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator+=(const FD_DebCor_Sigma_T_Spectrum& res)
    {
        L_0 += res.L_0;
        L_H += res.L_H;
        L_H1 += res.L_H1;
        L_H2 += res.L_H2;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator*=(const Spectrum& val)
    {
        L_0 *= val;
        L_H *= val;
        L_H1 *= val;
        L_H2 *= val;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator+=(const Spectrum& val)
    {
        L_0 += val;
        L_H += val;
        L_H1 += val;
        L_H2 += val;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator/=(const Spectrum& val)
    {
        L_0 = L_0 / val;
        L_H = L_H / val;
        L_H1 = L_H1 / val;
        L_H2 = L_H2 / val;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator-=(const Spectrum& val)
    {
        L_0 = L_0 - val;
        L_H = L_H - val;
        L_H1 = L_H1 - val;
        L_H2 = L_H2 - val;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator*=(Float val)
    {
        L_0 *= val;
        L_H *= val;
        L_H1 *= val;
        L_H2 *= val;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator+=(Float val)
    {
        L_0 += val;
        L_H += val;
        L_H1 += val;
        L_H2 += val;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator/=(Float val)
    {
        L_0 /= val;
        L_H /= val;
        L_H1 /= val;
        L_H2 /= val;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum& operator-=(Float val)
    {
        L_0 = L_0 - val;
        L_H = L_H - val;
        L_H1 = L_H1 - val;
        L_H2 = L_H2 - val;

        return *this;
    }

    FD_DebCor_Sigma_T_Spectrum create_copy(Spectrum L0,
                                           Spectrum LH,
                                           Spectrum LH1,
                                           Spectrum LH2) const
    {
        return FD_DebCor_Sigma_T_Spectrum(L0,
                                          LH,
                                          LH1,
                                          LH2,
                                          h,
                                          h1,
                                          h2,
                                          pmf,
                                          sample_sig,
                                          sigma_t0,
                                          sigma_s0,
                                          sigma_th,
                                          sigma_sh,
                                          sigma_th1,
                                          sigma_sh1,
                                          sigma_th2,
                                          sigma_sh2);
    }

    FD_DebCor_Sigma_T_Spectrum operator*(const FD_DebCor_Sigma_T_Spectrum &res) const
    {
        return create_copy(L_0 * res.L_0,
                           L_H * res.L_H,
                           L_H1 * res.L_H1,
                           L_H2 * res.L_H2);
    }

    FD_DebCor_Sigma_T_Spectrum operator+(const FD_DebCor_Sigma_T_Spectrum &res) const
    {
        return create_copy(L_0 + res.L_0,
                           L_H + res.L_H,
                           L_H1 + res.L_H1,
                           L_H2 + res.L_H2);
    }

    FD_DebCor_Sigma_T_Spectrum operator*(const Spectrum& val) const
    {
        return create_copy(L_0 * val,
                           L_H * val,
                           L_H1 * val,
                           L_H2 * val);
    }

    FD_DebCor_Sigma_T_Spectrum operator/(const Spectrum& val) const
    {
        return create_copy(L_0 / val,
                           L_H / val,
                           L_H1 / val,
                           L_H2 / val);
    }

    FD_DebCor_Sigma_T_Spectrum operator*(Float val) const
    {
        return create_copy(L_0 * val,
                           L_H * val,
                           L_H1 * val,
                           L_H2 * val);
    }

    FD_DebCor_Sigma_T_Spectrum operator/(Float val) const
    {
        return create_copy(L_0 / val,
                           L_H / val,
                           L_H1 / val,
                           L_H2 / val);
    }

    FD_DebCor_Sigma_T_Spectrum operator+(Float val) const
    {
        return create_copy(L_0 + val,
                           L_H + val,
                           L_H1 + val,
                           L_H2 + val);
    }

    bool IsBlack() const
    {
        return L_0.IsBlack();
    }

    Spectrum L_0;
    Spectrum L_H;
    Spectrum L_H1;
    Spectrum L_H2;

    Float sigma_t0;
    Float sigma_s0;
    Float sigma_th;
    Float sigma_sh;
    Float sigma_th1;
    Float sigma_sh1;
    Float sigma_th2;
    Float sigma_sh2;

    Float sample_sig;
    Float pmf;

    Float h;
    Float h1;
    Float h2;
};

struct FD_Cor_Sigma_T_Spectrum
{
    FD_Cor_Sigma_T_Spectrum(Spectrum L_0,
                            Spectrum L_H,
                            Float h,
                            Float sample_sig,
                            Float sigma_t0,
                            Float sigma_s0,
                            Float sigma_th,
                            Float sigma_sh)
        : L_0(L_0),
          L_H(L_H),
          h(h),
          sample_sig(sample_sig),
          sigma_t0(sigma_t0),
          sigma_s0(sigma_s0),
          sigma_th(sigma_th),
          sigma_sh(sigma_sh) { }

    FD_Cor_Sigma_T_Spectrum(Float h,
                            Float sample_sig,
                            Float sigma_t0,
                            Float sigma_s0,
                            Float sigma_th,
                            Float sigma_sh)
        : L_0(Spectrum(0.f)),
          L_H(Spectrum(0.f)),
          h(h),
          sample_sig(sample_sig),
          sigma_t0(sigma_t0),
          sigma_s0(sigma_s0),
          sigma_th(sigma_th),
          sigma_sh(sigma_sh) { }

    FD_Cor_Sigma_T_Spectrum(const FD_Cor_Sigma_T_Spectrum& other)
        : L_0(other.L_0),
          L_H(other.L_H),
          h(other.h),
          sample_sig(other.sample_sig),
          sigma_t0(other.sigma_t0),
          sigma_s0(other.sigma_s0),
          sigma_th(other.sigma_th),
          sigma_sh(other.sigma_sh) { }

    void setAll(Spectrum val)
    {
        L_0 = val;
        L_H = val;
    }

    // TODO: idk if this is really correct for all of these
    FD_Cor_Sigma_T_Spectrum& operator*=(const FD_Cor_Sigma_T_Spectrum& res)
    {
        L_0 *= res.L_0;
        L_H *= res.L_H;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator+=(const FD_Cor_Sigma_T_Spectrum& res)
    {
        L_0 += res.L_0;
        L_H += res.L_H;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator*=(const Spectrum& val)
    {
        L_0 *= val;
        L_H *= val;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator+=(const Spectrum& val)
    {
        L_0 += val;
        L_H += val;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator/=(const Spectrum& val)
    {
        L_0 = L_0 / val;
        L_H = L_H / val;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator-=(const Spectrum& val)
    {
        L_0 = L_0 - val;
        L_H = L_H - val;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator*=(Float val)
    {
        L_0 *= val;
        L_H *= val;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator+=(Float val)
    {
        L_0 += val;
        L_H += val;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator/=(Float val)
    {
        L_0 /= val;
        L_H /= val;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum& operator-=(Float val)
    {
        L_0 = L_0 - val;
        L_H = L_H - val;

        return *this;
    }

    FD_Cor_Sigma_T_Spectrum operator*(const FD_Cor_Sigma_T_Spectrum &res) const
    {
        return FD_Cor_Sigma_T_Spectrum(L_0 * res.L_0,
                                       L_H * res.L_H,
                                       h,
                                       sample_sig,
                                       sigma_t0,
                                       sigma_s0,
                                       sigma_th,
                                       sigma_sh);
    }

    FD_Cor_Sigma_T_Spectrum operator+(const FD_Cor_Sigma_T_Spectrum &res) const
    {
        return FD_Cor_Sigma_T_Spectrum(L_0 + res.L_0,
                                       L_H + res.L_H,
                                       h,
                                       sample_sig,
                                       sigma_t0,
                                       sigma_s0,
                                       sigma_th,
                                       sigma_sh);
    }

    FD_Cor_Sigma_T_Spectrum operator*(const Spectrum& val) const
    {
        return FD_Cor_Sigma_T_Spectrum(L_0 * val,
                                       L_H * val,
                                       h,
                                       sample_sig,
                                       sigma_t0,
                                       sigma_s0,
                                       sigma_th,
                                       sigma_sh);
    }

    FD_Cor_Sigma_T_Spectrum operator/(const Spectrum& val) const
    {
        return FD_Cor_Sigma_T_Spectrum(L_0 / val,
                                       L_H / val,
                                       h,
                                       sample_sig,
                                       sigma_t0,
                                       sigma_s0,
                                       sigma_th,
                                       sigma_sh);
    }

    FD_Cor_Sigma_T_Spectrum operator*(Float val) const
    {
        return FD_Cor_Sigma_T_Spectrum(L_0 * val,
                                       L_H * val,
                                       h,
                                       sample_sig,
                                       sigma_t0,
                                       sigma_s0,
                                       sigma_th,
                                       sigma_sh);
    }

    FD_Cor_Sigma_T_Spectrum operator/(Float val) const
    {
        return FD_Cor_Sigma_T_Spectrum(L_0 / val,
                                       L_H / val,
                                       h,
                                       sample_sig,
                                       sigma_t0,
                                       sigma_s0,
                                       sigma_th,
                                       sigma_sh);
    }

    FD_Cor_Sigma_T_Spectrum operator+(Float val) const
    {
        return FD_Cor_Sigma_T_Spectrum(L_0 + val,
                                       L_H + val,
                                       h,
                                       sample_sig,
                                       sigma_t0,
                                       sigma_s0,
                                       sigma_th,
                                       sigma_sh);
    }

    bool IsBlack() const
    {
      return L_0.IsBlack();
    }

    Spectrum L_0;
    Spectrum L_H;

    Float sigma_t0;
    Float sigma_s0;
    Float sigma_th;
    Float sigma_sh;
    Float sample_sig;

    Float h;
};

struct FD_Cor_Spectrum
{
    FD_Cor_Spectrum()
        : L_0(Spectrum(0.f)),
          L_H(Spectrum(0.f)),
          h(1.0) { }

    FD_Cor_Spectrum(Spectrum L_0,
                    Spectrum L_H,
                    Float h)
        : L_0(L_0),
          L_H(L_H),
          h(h) { }

    FD_Cor_Spectrum(Float h)
        : L_0(Spectrum(0.f)),
          L_H(Spectrum(0.f)),
          h(h) { }

    FD_Cor_Spectrum(const FD_Cor_Spectrum& other)
        : L_0(other.L_0),
          L_H(other.L_H),
          h(other.h) { }

    void setAll(Spectrum val)
    {
        L_0 = val;
        L_H = val;
    }

    // TODO: idk if this is really correct for all of these
    FD_Cor_Spectrum& operator*=(const FD_Cor_Spectrum& res)
    {
        L_0 *= res.L_0;
        L_H *= res.L_H;

        return *this;
    }

    FD_Cor_Spectrum& operator+=(const FD_Cor_Spectrum& res)
    {
        L_0 += res.L_0;
        L_H += res.L_H;

        return *this;
    }

    FD_Cor_Spectrum& operator*=(const Spectrum& val)
    {
        L_0 *= val;
        L_H *= val;

        return *this;
    }

    FD_Cor_Spectrum& operator+=(const Spectrum& val)
    {
        L_0 += val;
        L_H += val;

        return *this;
    }

    FD_Cor_Spectrum& operator/=(const Spectrum& val)
    {
        L_0 = L_0 / val;
        L_H = L_H / val;

        return *this;
    }

    FD_Cor_Spectrum& operator-=(const Spectrum& val)
    {
        L_0 = L_0 - val;
        L_H = L_H - val;

        return *this;
    }

    FD_Cor_Spectrum& operator*=(Float val)
    {
        L_0 *= val;
        L_H *= val;

        return *this;
    }

    FD_Cor_Spectrum& operator+=(Float val)
    {
        L_0 += val;
        L_H += val;

        return *this;
    }

    FD_Cor_Spectrum& operator/=(Float val)
    {
        L_0 /= val;
        L_H /= val;

        return *this;
    }

    FD_Cor_Spectrum& operator-=(Float val)
    {
        L_0 = L_0 - val;
        L_H = L_H - val;

        return *this;
    }

    FD_Cor_Spectrum operator*(const FD_Cor_Spectrum &res) const
    {
        return FD_Cor_Spectrum(L_0 * res.L_0,
                               L_H * res.L_H,
                               h);
    }

    FD_Cor_Spectrum operator+(const FD_Cor_Spectrum &res) const
    {
        return FD_Cor_Spectrum(L_0 + res.L_0,
                               L_H + res.L_H,
                               h);
    }

    FD_Cor_Spectrum operator*(const Spectrum& val) const
    {
        return FD_Cor_Spectrum(L_0 * val,
                               L_H * val,
                               h);
    }

    FD_Cor_Spectrum operator/(const Spectrum& val) const
    {
        return FD_Cor_Spectrum(L_0 / val,
                               L_H / val,
                               h);
    }

    FD_Cor_Spectrum operator*(Float val) const
    {
        return FD_Cor_Spectrum(L_0 * val,
                               L_H * val,
                               h);
    }

    FD_Cor_Spectrum operator/(Float val) const
    {
        return FD_Cor_Spectrum(L_0 / val,
                               L_H / val,
                               h);
    }

    FD_Cor_Spectrum operator+(Float val) const
    {
        return FD_Cor_Spectrum(L_0 + val,
                               L_H + val,
                               h);
    }

    bool IsBlack() const
    {
        return L_0.IsBlack();
    }

    Spectrum L_0;
    Spectrum L_H;

    Float h;
};

struct FD_Cor_Deb_Spectrum
{
    FD_Cor_Deb_Spectrum()
        : L_0(Spectrum(0.f)),
          L_H(Spectrum(0.f)),
          L_H1(Spectrum(0.f)),
          L_H2(Spectrum(0.f)),
          h(1.0),
          h1(1.0),
          h2(1.0) { }

    FD_Cor_Deb_Spectrum(Float val)
      : L_0(Spectrum(val)),
        L_H(Spectrum(val)),
        L_H1(Spectrum(val)),
        L_H2(Spectrum(val)),
        h(1.0),
        h1(1.0),
        h2(1.0) { }

    FD_Cor_Deb_Spectrum(Spectrum val)
      : L_0(val),
        L_H(val),
        L_H1(val),
        L_H2(val),
        h(1.0),
        h1(1.0),
        h2(1.0) { }

    FD_Cor_Deb_Spectrum
    (
        Spectrum L_0,
        Spectrum L_H,
        Spectrum L_H1,
        Spectrum L_H2,
        Float h,
        Float h1,
        Float h2
    )
        : L_0(L_0),
          L_H(L_H),
          L_H1(L_H1),
          L_H2(L_H2),
          h(h),
          h1(h1),
          h2(h2) { }

    FD_Cor_Deb_Spectrum(Float h, Float h1, Float h2)
        : L_0(Spectrum(0.f)),
          L_H(Spectrum(0.f)),
          L_H1(Spectrum(0.f)),
          L_H2(Spectrum(0.f)),
          h(h),
          h1(h1),
          h2(h2) { }

    FD_Cor_Deb_Spectrum(const FD_Cor_Deb_Spectrum& other)
        : L_0(other.L_0),
          L_H(other.L_H),
          L_H1(other.L_H1),
          L_H2(other.L_H2),
          h(other.h),
          h1(other.h1),
          h2(other.h2) { }

    void setAll(Spectrum val)
    {
        L_0 = val;
        L_H = val;
        L_H1 = val;
        L_H2 = val;
    }

    // TODO: idk if this is really correct for all of these
    FD_Cor_Deb_Spectrum& operator*=(const FD_Cor_Deb_Spectrum& res)
    {
        L_0 *= res.L_0;
        L_H *= res.L_H;
        L_H1 *= res.L_H1;
        L_H2 *= res.L_H2;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator+=(const FD_Cor_Deb_Spectrum& res)
    {
        L_0 += res.L_0;
        L_H += res.L_H;
        L_H1 += res.L_H1;
        L_H2 += res.L_H2;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator*=(const Spectrum& val)
    {
        L_0 *= val;
        L_H *= val;
        L_H1 *= val;
        L_H2 *= val;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator+=(const Spectrum& val)
    {
        L_0 += val;
        L_H += val;
        L_H1 += val;
        L_H2 += val;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator/=(const Spectrum& val)
    {
        L_0 = L_0 / val;
        L_H = L_H / val;
        L_H1 = L_H1 / val;
        L_H2 = L_H2 / val;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator-=(const Spectrum& val)
    {
        L_0 = L_0 - val;
        L_H = L_H - val;
        L_H1 = L_H1 - val;
        L_H2 = L_H2 - val;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator*=(Float val)
    {
        L_0 *= val;
        L_H *= val;
        L_H1 *= val;
        L_H2 *= val;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator+=(Float val)
    {
        L_0 += val;
        L_H += val;
        L_H1 += val;
        L_H2 += val;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator/=(Float val)
    {
        L_0 /= val;
        L_H /= val;
        L_H1 /= val;
        L_H2 /= val;

        return *this;
    }

    FD_Cor_Deb_Spectrum& operator-=(Float val)
    {
        L_0 = L_0 - val;
        L_H = L_H - val;
        L_H1 = L_H1 - val;
        L_H2 = L_H2 - val;

        return *this;
    }

    FD_Cor_Deb_Spectrum operator*(const FD_Cor_Deb_Spectrum &res) const
    {
        return FD_Cor_Deb_Spectrum(L_0 * res.L_0,
                                   L_H * res.L_H,
                                   L_H1 * res.L_H1,
                                   L_H2 * res.L_H2,
                                   h, h1, h2);
    }

    FD_Cor_Deb_Spectrum operator+(const FD_Cor_Deb_Spectrum &res) const
    {
        return FD_Cor_Deb_Spectrum(L_0 + res.L_0,
                                   L_H + res.L_H,
                                   L_H1 + res.L_H1,
                                   L_H2 + res.L_H2,
                                   h, h1, h2);
    }

    FD_Cor_Deb_Spectrum operator*(const Spectrum& val) const
    {
        return FD_Cor_Deb_Spectrum(L_0 * val,
                                   L_H * val,
                                   L_H1 * val,
                                   L_H2 * val,
                                   h, h1, h2);
    }

    FD_Cor_Deb_Spectrum operator/(const Spectrum& val) const
    {
        return FD_Cor_Deb_Spectrum(L_0 / val,
                                   L_H / val,
                                   L_H1 / val,
                                   L_H2 / val,
                                   h, h1, h2);
    }

    FD_Cor_Deb_Spectrum operator*(Float val) const
    {
        return FD_Cor_Deb_Spectrum(L_0 * val,
                                   L_H * val,
                                   L_H1 * val,
                                   L_H2 * val,
                                   h, h1, h2);
    }

    FD_Cor_Deb_Spectrum operator/(Float val) const
    {
        return FD_Cor_Deb_Spectrum(L_0 / val,
                                   L_H / val,
                                   L_H1 / val,
                                   L_H2 / val,
                                   h, h1, h2);
    }

    FD_Cor_Deb_Spectrum operator+(Float val) const
    {
        return FD_Cor_Deb_Spectrum(L_0 + val,
                                   L_H + val,
                                   L_H1 + val,
                                   L_H2 + val,
                                   h, h1, h2);
    }

    bool IsBlack() const
    {
        return L_0.IsBlack();
    }

    Spectrum L_0;
    Spectrum L_H;
    Spectrum L_H1;
    Spectrum L_H2;

    Float h;
    Float h1;
    Float h2;
};

struct FD_Cor_Deb_Cent_Spectrum
{
    FD_Cor_Deb_Cent_Spectrum()
        : L_0(Spectrum(0.f)),
          L_H00(Spectrum(0.f)),
          L_H01(Spectrum(0.f)),
          L_H10(Spectrum(0.f)),
          L_H11(Spectrum(0.f)),
          L_H20(Spectrum(0.f)),
          L_H21(Spectrum(0.f)),
          h(1.0),
          h1(1.0),
          h2(1.0) { }

    FD_Cor_Deb_Cent_Spectrum(Float val)
      : L_0(Spectrum(val)),
        L_H00(Spectrum(val)),
        L_H01(Spectrum(val)),
        L_H10(Spectrum(val)),
        L_H11(Spectrum(val)),
        L_H20(Spectrum(val)),
        L_H21(Spectrum(val)),
        h(1.0),
        h1(1.0),
        h2(1.0) { }

    FD_Cor_Deb_Cent_Spectrum(Spectrum val)
      : L_0(val),
        L_H00(val),
        L_H01(val),
        L_H10(val),
        L_H11(val),
        L_H20(val),
        L_H21(val),
        h(1.0),
        h1(1.0),
        h2(1.0) { }

    FD_Cor_Deb_Cent_Spectrum
    (
        Spectrum L_0,
        Spectrum L_H00,
        Spectrum L_H01,
        Spectrum L_H10,
        Spectrum L_H11,
        Spectrum L_H20,
        Spectrum L_H21,
        Float h,
        Float h1,
        Float h2
    )
        : L_0(L_0),
          L_H00(L_H00),
          L_H01(L_H01),
          L_H10(L_H10),
          L_H11(L_H11),
          L_H20(L_H20),
          L_H21(L_H21),
          h(h),
          h1(h1),
          h2(h2) { }

    FD_Cor_Deb_Cent_Spectrum(Float h, Float h1, Float h2)
        : L_0(Spectrum(0.f)),
          L_H00(Spectrum(0.f)),
          L_H01(Spectrum(0.f)),
          L_H10(Spectrum(0.f)),
          L_H11(Spectrum(0.f)),
          L_H20(Spectrum(0.f)),
          L_H21(Spectrum(0.f)),
          h(h),
          h1(h1),
          h2(h2) { }

    FD_Cor_Deb_Cent_Spectrum(const FD_Cor_Deb_Cent_Spectrum& other)
        : L_0(other.L_0),
          L_H00(other.L_H00),
          L_H01(other.L_H01),
          L_H10(other.L_H10),
          L_H11(other.L_H11),
          L_H20(other.L_H20),
          L_H21(other.L_H21),
          h(other.h),
          h1(other.h1),
          h2(other.h2) { }

    void setAll(Spectrum val)
    {
        L_0 = val;
        L_H00 = val;
        L_H01 = val;
        L_H10 = val;
        L_H11 = val;
        L_H20 = val;
        L_H21 = val;
    }

    // TODO: idk if this is really correct for all of these
    FD_Cor_Deb_Cent_Spectrum& operator*=(const FD_Cor_Deb_Cent_Spectrum& res)
    {
        L_0 *= res.L_0;
        L_H00 *= res.L_H00;
        L_H01 *= res.L_H01;
        L_H10 *= res.L_H10;
        L_H11 *= res.L_H11;
        L_H20 *= res.L_H20;
        L_H21 *= res.L_H21;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator+=(const FD_Cor_Deb_Cent_Spectrum& res)
    {
        L_0 += res.L_0;
        L_H00 += res.L_H00;
        L_H01 += res.L_H01;
        L_H10 += res.L_H10;
        L_H11 += res.L_H11;
        L_H20 += res.L_H20;
        L_H21 += res.L_H21;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator*=(const Spectrum& val)
    {
        L_0 *= val;
        L_H00 *= val;
        L_H01 *= val;
        L_H10 *= val;
        L_H11 *= val;
        L_H20 *= val;
        L_H21 *= val;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator+=(const Spectrum& val)
    {
        L_0 += val;
        L_H00 += val;
        L_H01 += val;
        L_H10 += val;
        L_H11 += val;
        L_H20 += val;
        L_H21 += val;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator/=(const Spectrum& val)
    {
        L_0 = L_0 / val;
        L_H00 = L_H00 / val;
        L_H01 = L_H01 / val;
        L_H10 = L_H10 / val;
        L_H11 = L_H11 / val;
        L_H20 = L_H20 / val;
        L_H21 = L_H21 / val;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator-=(const Spectrum& val)
    {
        L_0 = L_0 - val;
        L_H00 = L_H00 - val;
        L_H01 = L_H01 - val;
        L_H10 = L_H10 - val;
        L_H11 = L_H11 - val;
        L_H20 = L_H20 - val;
        L_H21 = L_H21 - val;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator*=(Float val)
    {
        L_0 *= val;
        L_H00 *= val;
        L_H01 *= val;
        L_H10 *= val;
        L_H11 *= val;
        L_H20 *= val;
        L_H21 *= val;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator+=(Float val)
    {
        L_0 += val;
        L_H00 += val;
        L_H01 += val;
        L_H10 += val;
        L_H11 += val;
        L_H20 += val;
        L_H21 += val;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator/=(Float val)
    {
        L_0 /= val;
        L_H00 /= val;
        L_H01 /= val;
        L_H10 /= val;
        L_H11 /= val;
        L_H20 /= val;
        L_H21 /= val;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum& operator-=(Float val)
    {
        L_0 = L_0 - val;
        L_H00 = L_H00 - val;
        L_H01 = L_H01 - val;
        L_H10 = L_H10 - val;
        L_H11 = L_H11 - val;
        L_H20 = L_H20 - val;
        L_H21 = L_H21 - val;

        return *this;
    }

    FD_Cor_Deb_Cent_Spectrum operator*(const FD_Cor_Deb_Cent_Spectrum &res) const
    {
        return FD_Cor_Deb_Cent_Spectrum
        (
            L_0 * res.L_0,
            L_H00 * res.L_H00,
            L_H01 * res.L_H01,
            L_H10 * res.L_H10,
            L_H11 * res.L_H11,
            L_H20 * res.L_H20,
            L_H21 * res.L_H21,
            h, h1, h2
        );
    }

    FD_Cor_Deb_Cent_Spectrum operator+(const FD_Cor_Deb_Cent_Spectrum &res) const
    {
        return FD_Cor_Deb_Cent_Spectrum
        (
            L_0 + res.L_0,
            L_H00 + res.L_H00,
            L_H01 + res.L_H01,
            L_H10 + res.L_H10,
            L_H11 + res.L_H11,
            L_H20 + res.L_H20,
            L_H21 + res.L_H21,
            h, h1, h2
        );
    }

    FD_Cor_Deb_Cent_Spectrum operator*(const Spectrum& val) const
    {
        return FD_Cor_Deb_Cent_Spectrum
        (
            L_0 * val,
            L_H00 * val,
            L_H01 * val,
            L_H10 * val,
            L_H11 * val,
            L_H20 * val,
            L_H21 * val,
            h, h1, h2
        );
    }

    FD_Cor_Deb_Cent_Spectrum operator/(const Spectrum& val) const
    {
        return FD_Cor_Deb_Cent_Spectrum
        (
            L_0 / val,
            L_H00 / val,
            L_H01 / val,
            L_H10 / val,
            L_H11 / val,
            L_H20 / val,
            L_H21 / val,
            h, h1, h2
        );
    }

    FD_Cor_Deb_Cent_Spectrum operator*(Float val) const
    {
        return FD_Cor_Deb_Cent_Spectrum
        (
            L_0 * val,
            L_H00 * val,
            L_H01 * val,
            L_H10 * val,
            L_H11 * val,
            L_H20 * val,
            L_H21 * val,
            h, h1, h2
        );
    }

    FD_Cor_Deb_Cent_Spectrum operator/(Float val) const
    {
        return FD_Cor_Deb_Cent_Spectrum
        (
            L_0 / val,
            L_H00 / val,
            L_H01 / val,
            L_H10 / val,
            L_H11 / val,
            L_H20 / val,
            L_H21 / val,
            h, h1, h2
        );
    }

    FD_Cor_Deb_Cent_Spectrum operator+(Float val) const
    {
        return FD_Cor_Deb_Cent_Spectrum
        (
            L_0 + val,
            L_H00 + val,
            L_H01 + val,
            L_H10 + val,
            L_H11 + val,
            L_H20 + val,
            L_H21 + val,
            h, h1, h2
        );
    }

    bool IsBlack() const
    {
        return L_0.IsBlack();
    }

    Spectrum L_0;
    Spectrum L_H00;
    Spectrum L_H01;
    Spectrum L_H10;
    Spectrum L_H11;
    Spectrum L_H20;
    Spectrum L_H21;

    Float h;
    Float h1;
    Float h2;
};

};
