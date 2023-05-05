#pragma once

#include "pcg32.h"
#include "sampler.h"

namespace pbrt
{

class TSampler : public Sampler
{
public:
    TSampler(int ns);
    TSampler(int ns, unsigned int seed);
    virtual void reseed(unsigned int seed);
    virtual void reseedSampler();
    virtual void reseedJitter();
    virtual void StartPixel(const Point2i& p);
    virtual Point2f Get2D();
    virtual bool StartNextSample();

    float randomDigitScramble(float f, unsigned scramble)
    {
        return (unsigned(f*0x100000000LL) ^ scramble) * 2.3283064365386962890625e-10f;
    }

    unsigned int hash(unsigned int x)
    {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        return x;
    }

    uint64_t hash(uint64_t x)
    {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        return x;
    }

    uint32_t Hash1(uint64_t aNumber)
    {
        aNumber = ~aNumber + (aNumber << 15); // aNumber = (aNumber << 15) - aNumber - 1;
        aNumber = aNumber ^ (aNumber >> 12);
        aNumber = aNumber + (aNumber << 2);
        aNumber = aNumber ^ (aNumber >> 4);
        aNumber = aNumber * 2057; // aNumber = (aNumber + (aNumber << 3)) + (aNumber << 11);
        aNumber = aNumber ^ (aNumber >> 16);

        return aNumber;
    }

    uint32_t Hash2(uint64_t aNumber)
    {
        aNumber = (aNumber + 0x7ed55d16) + (aNumber << 12);
        aNumber = (aNumber ^ 0xc761c23c) ^ (aNumber >> 19);
        aNumber = (aNumber + 0x165667b1) + (aNumber << 5);
        aNumber = (aNumber + 0xd3a2646c) ^ (aNumber << 9);
        aNumber = (aNumber + 0xfd7046c5) + (aNumber << 3);
        aNumber = (aNumber ^ 0xb55a4f09) ^ (aNumber >> 16);

        return aNumber;
    }

    uint32_t hash(uint32_t seed, uint32_t seq)
    {
        uint32_t delta = 0x9e3779b9;
        uint32_t k0 = 0xA341316C;
        uint32_t k1 = 0xC8013EA4;
        uint32_t k2 = 0xAD90777D;
        uint32_t k3 = 0x7E95761E;

        uint32_t sum = 0;

        for (int i = 0; i < 6; ++i)
        {
            sum += delta;

            seed += ((seq << 4) + k0) ^ (sum + seq) ^ ((seq >> 5) + k1);
            seq += ((seed << 4) + k2) ^ (sum + seed) ^ ((seed >> 5) + k3);
        }

        return seq;
    }

    // static pcg32 globalRNG;

protected:
    pcg32 rng;
    pcg32 jitRNG;
    uint64_t currentDim;
    uint64_t sampSeed;
    uint64_t baseSeed;
    uint64_t jitSeed;
};

}
