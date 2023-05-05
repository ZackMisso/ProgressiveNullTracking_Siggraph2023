#include "trans_permute.h"

Trans_Permutation::Trans_Permutation() {
    indices = std::vector<uint32_t>();
}

Trans_Permutation::Trans_Permutation(uint32_t size) {
    indices = std::vector<uint32_t>(size);
}

void Trans_Permutation::initialize() {
    for (int i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }
}

void Trans_Permutation::resize(uint32_t newSize) {
    indices.resize(newSize);
}

int32_t Trans_Permutation::operator[](const uint32_t i) const {
    return indices[i];
}

void Trans_Permutation::shuffle(pcg32& rand) {
    rand.shuffle(indices.begin(), indices.end());
}

uint32_t Trans_Permutation::getSize() const {
    return indices.size();
}

//! In-place enumeration of random permutations
/*!
    Returns the i-th element of the p-th pseudo-random permutation of the
    numbers 0..(l-1).

    Based on method described in the tech report:

    Andrew Kensler. "Correlated Multi-Jittered Sampling",
    Pixar Technical Memo 13-01.
*/
unsigned int Trans_Permutation::permute(unsigned int i, unsigned int l, unsigned int p)
{
    unsigned w = l - 1;
    w |= w >> 1;
    w |= w >> 2;
    w |= w >> 4;
    w |= w >> 8;
    w |= w >> 16;
    do
    {
        i ^= p;             i *= 0xe170893d;
        i ^= p >> 16;
        i ^= (i & w) >> 4;
        i ^= p >> 8;        i *= 0x0929eb3f;
        i ^= p >> 23;
        i ^= (i & w) >> 1;  i *= 1 | p >> 27;
        i *= 0x6935fa69;
        i ^= (i & w) >> 11; i *= 0x74dcb303;
        i ^= (i & w) >> 2;  i *= 0x9e501cc3;
        i ^= (i & w) >> 2;  i *= 0xc860a3df;
        i &= w;
        i ^= i >> 5;
    } while (i >= l);

    return (i + p) % l;
}
