// This code is based off of code by the following individual, but is adapted
// to generate progressive samples
//
// Copyright (c) 2012 Leonhard Gruenschloss (leonhard@gruenschloss.org)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <algorithm>
#include <vector>

#include "trans_samplers/trans_permute.h"

namespace pbrt
{

// Compute points of the Halton sequence with with digit-permutations for different bases.
template <typename RNG>
class Halton_Process
{
public:
    // Init the permutation arrays using Faure-permutations. Alternatively, init_random can be
    // called before the sampling functionality can be used.
    // void init_faure();

    // Init the permutation arrays using randomized permutations. Alternatively, init_faure can be
    // called before the sampling functionality can be used. The client needs to specify a random
    // number generator function object that contains a function
    // shuffle(first, last).
    void init_random(RNG* rand)
    {
        rng = rand;
        // const unsigned max_base = 1619u;
        // std::vector<std::vector<unsigned short> > perms(max_base + 1);
        // for (unsigned k = 1; k <= 3; ++k) // Keep identity permutations for base 1, 2, 3.
        // {
        //     perms[k].resize(k);
        //     for (unsigned i = 0; i < k; ++i)
        //         perms[k][i] = i;
        // }
        // m_perm2 = rand.nextUInt();   // WJ: also scramble the first dimension
        // for (unsigned base = 1; base <= max_base; ++base)
        // {
        //     perms[base].resize(base);
        //     for (unsigned i = 0; i < base; ++i)
        //         perms[base][i] = i;
        //     rand.shuffle(perms[base].begin(), perms[base].end());
        // }
        // init_tables(perms);
    }

    // Return the number of supported dimensions.
    static unsigned get_num_dimensions() { return 256u; }

    // Return the Halton sample for the given dimension (component) and index.
    // The client must have called init_random or init_faure at least once before.
    // dimension must be smaller than the value returned by get_num_dimensions().
    float sample(unsigned dimension, unsigned index) const
    {
        switch (dimension)
        {
            case 0: { return halton2(index); break; }
            case 1: { return halton3(index); break; }
            case 2: { return halton5(index); break; }
            case 3: { return halton7(index); break; }
            case 4: { return halton11(index); break; }
            case 5: { return halton13(index); break; }
            case 6: { return halton17(index); break; }
            case 7: { return halton19(index); break; }
            case 8: { return halton23(index); break; }
            case 9: { return halton29(index); break; }
            case 10: { return halton31(index); break; }
            case 11: { return halton37(index); break; }
            case 12: { return halton41(index); break; }
            case 13: { return halton43(index); break; }
            case 14: { return halton47(index); break; }
            case 15: { return halton53(index); break; }
            case 16: { return halton59(index); break; }
            case 17: { return halton61(index); break; }
            case 18: { return halton67(index); break; }
            case 19: { return halton71(index); break; }
            case 20: { return halton73(index); break; }
            case 21: { return halton79(index); break; }
            case 22: { return halton83(index); break; }
            case 23: { return halton89(index); break; }
            case 24: { return halton97(index); break; }
            case 25: { return halton101(index); break; }
            case 26: { return halton103(index); break; }
            case 27: { return halton107(index); break; }
            case 28: { return halton109(index); break; }
            case 29: { return halton113(index); break; }
            case 30: { return halton127(index); break; }
            case 31: { return halton131(index); break; }
            case 32: { return halton137(index); break; }
            case 33: { return halton139(index); break; }
            case 34: { return halton149(index); break; }
            case 35: { return halton151(index); break; }
            case 36: { return halton157(index); break; }
            case 37: { return halton163(index); break; }
            case 38: { return halton167(index); break; }
            case 39: { return halton173(index); break; }
            case 40: { return halton179(index); break; }
            case 41: { return halton181(index); break; }
            case 42: { return halton191(index); break; }
            case 43: { return halton193(index); break; }
            case 44: { return halton197(index); break; }
            case 45: { return halton199(index); break; }
            case 46: { return halton211(index); break; }
            case 47: { return halton223(index); break; }
            case 48: { return halton227(index); break; }
            case 49: { return halton229(index); break; }
            case 50: { return halton233(index); break; }
            case 51: { return halton239(index); break; }
            case 52: { return halton241(index); break; }
            case 53: { return halton251(index); break; }
            case 54: { return halton257(index); break; }
            case 55: { return halton263(index); break; }
            case 56: { return halton269(index); break; }
            case 57: { return halton271(index); break; }
            case 58: { return halton277(index); break; }
            case 59: { return halton281(index); break; }
            case 60: { return halton283(index); break; }
            case 61: { return halton293(index); break; }
            case 62: { return halton307(index); break; }
            case 63: { return halton311(index); break; }
            case 64: { return halton313(index); break; }
            case 65: { return halton317(index); break; }
            case 66: { return halton331(index); break; }
            case 67: { return halton337(index); break; }
            case 68: { return halton347(index); break; }
            case 69: { return halton349(index); break; }
            case 70: { return halton353(index); break; }
            case 71: { return halton359(index); break; }
            case 72: { return halton367(index); break; }
            case 73: { return halton373(index); break; }
            case 74: { return halton379(index); break; }
            case 75: { return halton383(index); break; }
            case 76: { return halton389(index); break; }
            case 77: { return halton397(index); break; }
            case 78: { return halton401(index); break; }
            case 79: { return halton409(index); break; }
            case 80: { return halton419(index); break; }
            case 81: { return halton421(index); break; }
            case 82: { return halton431(index); break; }
            case 83: { return halton433(index); break; }
            case 84: { return halton439(index); break; }
            case 85: { return halton443(index); break; }
            case 86: { return halton449(index); break; }
            case 87: { return halton457(index); break; }
            case 88: { return halton461(index); break; }
            case 89: { return halton463(index); break; }
            case 90: { return halton467(index); break; }
            case 91: { return halton479(index); break; }
            case 92: { return halton487(index); break; }
            case 93: { return halton491(index); break; }
            case 94: { return halton499(index); break; }
            case 95: { return halton503(index); break; }
            case 96: { return halton509(index); break; }
            case 97: { return halton521(index); break; }
            case 98: { return halton523(index); break; }
            case 99: { return halton541(index); break; }
            case 100: { return halton547(index); break; }
            case 101: { return halton557(index); break; }
            case 102: { return halton563(index); break; }
            case 103: { return halton569(index); break; }
            case 104: { return halton571(index); break; }
            case 105: { return halton577(index); break; }
            case 106: { return halton587(index); break; }
            case 107: { return halton593(index); break; }
            case 108: { return halton599(index); break; }
            case 109: { return halton601(index); break; }
            case 110: { return halton607(index); break; }
            case 111: { return halton613(index); break; }
            case 112: { return halton617(index); break; }
            case 113: { return halton619(index); break; }
            case 114: { return halton631(index); break; }
            case 115: { return halton641(index); break; }
            case 116: { return halton643(index); break; }
            case 117: { return halton647(index); break; }
            case 118: { return halton653(index); break; }
            case 119: { return halton659(index); break; }
            case 120: { return halton661(index); break; }
            case 121: { return halton673(index); break; }
            case 122: { return halton677(index); break; }
            case 123: { return halton683(index); break; }
            case 124: { return halton691(index); break; }
            case 125: { return halton701(index); break; }
            case 126: { return halton709(index); break; }
            case 127: { return halton719(index); break; }
            case 128: { return halton727(index); break; }
            case 129: { return halton733(index); break; }
            case 130: { return halton739(index); break; }
            case 131: { return halton743(index); break; }
            case 132: { return halton751(index); break; }
            case 133: { return halton757(index); break; }
            case 134: { return halton761(index); break; }
            case 135: { return halton769(index); break; }
            case 136: { return halton773(index); break; }
            case 137: { return halton787(index); break; }
            case 138: { return halton797(index); break; }
            case 139: { return halton809(index); break; }
            case 140: { return halton811(index); break; }
            case 141: { return halton821(index); break; }
            case 142: { return halton823(index); break; }
            case 143: { return halton827(index); break; }
            case 144: { return halton829(index); break; }
            case 145: { return halton839(index); break; }
            case 146: { return halton853(index); break; }
            case 147: { return halton857(index); break; }
            case 148: { return halton859(index); break; }
            case 149: { return halton863(index); break; }
            case 150: { return halton877(index); break; }
            case 151: { return halton881(index); break; }
            case 152: { return halton883(index); break; }
            case 153: { return halton887(index); break; }
            case 154: { return halton907(index); break; }
            case 155: { return halton911(index); break; }
            case 156: { return halton919(index); break; }
            case 157: { return halton929(index); break; }
            case 158: { return halton937(index); break; }
            case 159: { return halton941(index); break; }
            case 160: { return halton947(index); break; }
            case 161: { return halton953(index); break; }
            case 162: { return halton967(index); break; }
            case 163: { return halton971(index); break; }
            case 164: { return halton977(index); break; }
            case 165: { return halton983(index); break; }
            case 166: { return halton991(index); break; }
            case 167: { return halton997(index); break; }
            case 168: { return halton1009(index); break; }
            case 169: { return halton1013(index); break; }
            case 170: { return halton1019(index); break; }
            case 171: { return halton1021(index); break; }
            case 172: { return halton1031(index); break; }
            case 173: { return halton1033(index); break; }
            case 174: { return halton1039(index); break; }
            case 175: { return halton1049(index); break; }
            case 176: { return halton1051(index); break; }
            case 177: { return halton1061(index); break; }
            case 178: { return halton1063(index); break; }
            case 179: { return halton1069(index); break; }
            case 180: { return halton1087(index); break; }
            case 181: { return halton1091(index); break; }
            case 182: { return halton1093(index); break; }
            case 183: { return halton1097(index); break; }
            case 184: { return halton1103(index); break; }
            case 185: { return halton1109(index); break; }
            case 186: { return halton1117(index); break; }
            case 187: { return halton1123(index); break; }
            case 188: { return halton1129(index); break; }
            case 189: { return halton1151(index); break; }
            case 190: { return halton1153(index); break; }
            case 191: { return halton1163(index); break; }
            case 192: { return halton1171(index); break; }
            case 193: { return halton1181(index); break; }
            case 194: { return halton1187(index); break; }
            case 195: { return halton1193(index); break; }
            case 196: { return halton1201(index); break; }
            case 197: { return halton1213(index); break; }
            case 198: { return halton1217(index); break; }
            case 199: { return halton1223(index); break; }
            case 200: { return halton1229(index); break; }
            case 201: { return halton1231(index); break; }
            case 202: { return halton1237(index); break; }
            case 203: { return halton1249(index); break; }
            case 204: { return halton1259(index); break; }
            case 205: { return halton1277(index); break; }
            case 206: { return halton1279(index); break; }
            case 207: { return halton1283(index); break; }
            case 208: { return halton1289(index); break; }
            case 209: { return halton1291(index); break; }
            case 210: { return halton1297(index); break; }
            case 211: { return halton1301(index); break; }
            case 212: { return halton1303(index); break; }
            case 213: { return halton1307(index); break; }
            case 214: { return halton1319(index); break; }
            case 215: { return halton1321(index); break; }
            case 216: { return halton1327(index); break; }
            case 217: { return halton1361(index); break; }
            case 218: { return halton1367(index); break; }
            case 219: { return halton1373(index); break; }
            case 220: { return halton1381(index); break; }
            case 221: { return halton1399(index); break; }
            case 222: { return halton1409(index); break; }
            case 223: { return halton1423(index); break; }
            case 224: { return halton1427(index); break; }
            case 225: { return halton1429(index); break; }
            case 226: { return halton1433(index); break; }
            case 227: { return halton1439(index); break; }
            case 228: { return halton1447(index); break; }
            case 229: { return halton1451(index); break; }
            case 230: { return halton1453(index); break; }
            case 231: { return halton1459(index); break; }
            case 232: { return halton1471(index); break; }
            case 233: { return halton1481(index); break; }
            case 234: { return halton1483(index); break; }
            case 235: { return halton1487(index); break; }
            case 236: { return halton1489(index); break; }
            case 237: { return halton1493(index); break; }
            case 238: { return halton1499(index); break; }
            case 239: { return halton1511(index); break; }
            case 240: { return halton1523(index); break; }
            case 241: { return halton1531(index); break; }
            case 242: { return halton1543(index); break; }
            case 243: { return halton1549(index); break; }
            case 244: { return halton1553(index); break; }
            case 245: { return halton1559(index); break; }
            case 246: { return halton1567(index); break; }
            case 247: { return halton1571(index); break; }
            case 248: { return halton1579(index); break; }
            case 249: { return halton1583(index); break; }
            case 250: { return halton1597(index); break; }
            case 251: { return halton1601(index); break; }
            case 252: { return halton1607(index); break; }
            case 253: { return halton1609(index); break; }
            case 254: { return halton1613(index); break; }
            case 255: { return halton1619(index); break; }
        }
        return 0.f;
    }

private:
    RNG* rng;

    // static unsigned short invert(unsigned short base,
    //                              unsigned short digits,
    //                              unsigned short index,
    //                              const std::vector<unsigned short>& perm)
    // {
    //     // unsigned int perm = rng->nextUInt();
    //     unsigned short result = 0;
    //
    //     for (unsigned short i = 0; i < digits; ++i)
    //     {
    //         // result = result * base + Trans_Permutation::permute(index % base, base, perm);
    //         result = result * base + perm[index % base];
    //         index /= base;
    //     }
    //     return result;
    // }

    static unsigned short invert(unsigned short base,
                                 unsigned short digits,
                                 unsigned short index,
                                 unsigned int perm)
    {
        unsigned short result = 0;

        for (unsigned short i = 0; i < digits; ++i)
        {
            result = result * base + Trans_Permutation::permute(index % base, base, perm);
            index /= base;
        }
        return result;
    }

    float halton2(unsigned index) const
    {
        index = (index << 16) | (index >> 16);
        index = ((index & 0x00ff00ff) << 8) | ((index & 0xff00ff00) >> 8);
        index = ((index & 0x0f0f0f0f) << 4) | ((index & 0xf0f0f0f0) >> 4);
        index = ((index & 0x33333333) << 2) | ((index & 0xcccccccc) >> 2);
        index = ((index & 0x55555555) << 1) | ((index & 0xaaaaaaaa) >> 1);

        index ^= rng->nextUInt(); // WJ: also scramble the first dimension

        union Result
        {
            unsigned u;
            float f;
        } result; // Write reversed bits directly into floating-point mantissa.
        result.u = 0x3f800000u | (index >> 9);

        return result.f - 1.f;
    }

    float halton3(unsigned index) const
    {
        int indexOne = index % 243u;
        int indexTwo = (index / 243u) % 243u;
        int indexThree = (index / 59049u) % 243u;
        int indexFour = (index / 14348907u) % 243u;

        unsigned int perm = rng->nextUInt();

        return (invert(3, 5, indexOne, perm) * 14348907u +
                invert(3, 5, indexTwo, perm) * 59049u +
                invert(3, 5, indexThree, perm) * 243u +
                invert(3, 5, indexFour, perm)) * float(0x1.fffffcp-1 / 3486784401u);
    }

    float halton5(unsigned index) const
    {
        int indexOne = index % 125u;
        int indexTwo = (index / 125u) % 125u;
        int indexThree = (index / 15625u) % 125u;
        int indexFour = (index / 1953125u) % 125u;

        unsigned int perm = rng->nextUInt();

        return (invert(5, 3, indexOne, perm) * 1953125u +
                invert(5, 3, indexTwo, perm) * 15625u +
                invert(5, 3, indexThree, perm) * 125u +
                invert(5, 3, indexFour, perm)) * float(0x1.fffffcp-1 / 244140625u);
    }

    float halton7(unsigned index) const
    {
        int indexOne = index % 343u;
        int indexTwo = (index / 343u) % 343u;
        int indexThree = (index / 117649u) % 343u;

        unsigned int perm = rng->nextUInt();

        return (invert(7, 3, indexOne, perm) * 117649u +
                invert(7, 3, indexTwo, perm) * 343u +
                invert(7, 3, indexThree, perm)) * float(0x1.fffffcp-1 / 40353607u);
    }

    float halton11(unsigned index) const
    {
        int indexOne = index % 121u;
        int indexTwo = (index / 121u) % 121u;
        int indexThree = (index / 14641u) % 121u;
        int indexFour = (index / 1771561u) % 121u;

        unsigned int perm = rng->nextUInt();

        return (invert(11, 2, indexOne, perm) * 1771561u +
                invert(11, 2, indexTwo, perm) * 14641u +
                invert(11, 2, indexThree, perm) * 121u +
                invert(11, 2, indexFour, perm)) * float(0x1.fffffcp-1 / 214358881u);
    }

    float halton13(unsigned index) const
    {
        int indexOne = index % 169u;
        int indexTwo = (index / 169u) % 169u;
        int indexThree = (index / 28561u) % 169u;
        int indexFour = (index / 4826809u) % 169u;

        unsigned int perm = rng->nextUInt();

        return (invert(13, 2, indexOne, perm) * 4826809u +
                invert(13, 2, indexTwo, perm) * 28561u +
                invert(13, 2, indexThree, perm) * 169u +
                invert(13, 2, indexFour, perm)) * float(0x1.fffffcp-1 / 815730721u);
    }

    float halton17(unsigned index) const
    {
        int indexOne = index % 289u;
        int indexTwo = (index / 289u) % 289u;
        int indexThree = (index / 83521u) % 289u;

        unsigned int perm = rng->nextUInt();

        return (invert(17, 2, indexOne, perm) * 83521u +
                invert(17, 2, indexTwo, perm) * 289u +
                invert(17, 2, indexThree, perm)) * float(0x1.fffffcp-1 / 24137569u);
    }

    float halton19(unsigned index) const
    {
        int indexOne = index % 361u;
        int indexTwo = (index / 361u) % 361u;
        int indexThree = (index / 130321u) % 361u;

        unsigned int perm = rng->nextUInt();

        return (invert(19, 2, indexOne, perm) * 130321u +
                invert(19, 2, indexTwo, perm) * 361u +
                invert(19, 2, indexThree, perm)) * float(0x1.fffffcp-1 / 47045881u);
    }

    float halton23(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(23, 1, index % 23u, perm) * 148035889u +
            invert(23, 1, (index / 23u) % 23u, perm) * 6436343u +
            invert(23, 1, (index / 529u) % 23u, perm) * 279841u +
            invert(23, 1, (index / 12167u) % 23u, perm) * 12167u +
            invert(23, 1, (index / 279841u) % 23u, perm) * 529u +
            invert(23, 1, (index / 6436343u) % 23u, perm) * 23u +
            invert(23, 1, (index / 148035889u) % 23u, perm)) * float(0x1.fffffcp-1 / 3404825447u);
    }

    float halton29(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(29, 1, index % 29u, perm) * 20511149u +
            invert(29, 1, (index / 29u) % 29u, perm) * 707281u +
            invert(29, 1, (index / 841u) % 29u, perm) * 24389u +
            invert(29, 1, (index / 24389u) % 29u, perm) * 841u +
            invert(29, 1, (index / 707281u) % 29u, perm) * 29u +
            invert(29, 1, (index / 20511149u) % 29u, perm)) * float(0x1.fffffcp-1 / 594823321u);
    }

    float halton31(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(31, 1, index % 31u, perm) * 28629151u +
            invert(31, 1, (index / 31u) % 31u, perm) * 923521u +
            invert(31, 1, (index / 961u) % 31u, perm) * 29791u +
            invert(31, 1, (index / 29791u) % 31u, perm) * 961u +
            invert(31, 1, (index / 923521u) % 31u, perm) * 31u +
            invert(31, 1, (index / 28629151u) % 31u, perm)) * float(0x1.fffffcp-1 / 887503681u);
    }

    float halton37(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(37, 1, index % 37u, perm) * 69343957u +
            invert(37, 1, (index / 37u) % 37u, perm) * 1874161u +
            invert(37, 1, (index / 1369u) % 37u, perm) * 50653u +
            invert(37, 1, (index / 50653u) % 37u, perm) * 1369u +
            invert(37, 1, (index / 1874161u) % 37u, perm) * 37u +
            invert(37, 1, (index / 69343957u) % 37u, perm)) * float(0x1.fffffcp-1 / 2565726409u);
    }

    float halton41(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(41, 1, index % 41u, perm) * 2825761u +
            invert(41, 1, (index / 41u) % 41u, perm) * 68921u +
            invert(41, 1, (index / 1681u) % 41u, perm) * 1681u +
            invert(41, 1, (index / 68921u) % 41u, perm) * 41u +
            invert(41, 1, (index / 2825761u) % 41u, perm)) * float(0x1.fffffcp-1 / 115856201u);
    }

    float halton43(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(43, 1, index % 43u, perm) * 3418801u +
            invert(43, 1, (index / 43u) % 43u, perm) * 79507u +
            invert(43, 1, (index / 1849u) % 43u, perm) * 1849u +
            invert(43, 1, (index / 79507u) % 43u, perm) * 43u +
            invert(43, 1, (index / 3418801u) % 43u, perm)) * float(0x1.fffffcp-1 / 147008443u);
    }

    float halton47(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(47, 1, index % 47u, perm) * 4879681u +
            invert(47, 1, (index / 47u) % 47u, perm) * 103823u +
            invert(47, 1, (index / 2209u) % 47u, perm) * 2209u +
            invert(47, 1, (index / 103823u) % 47u, perm) * 47u +
            invert(47, 1, (index / 4879681u) % 47u, perm)) * float(0x1.fffffcp-1 / 229345007u);
    }

    float halton53(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(53, 1, index % 53u, perm) * 7890481u +
            invert(53, 1, (index / 53u) % 53u, perm) * 148877u +
            invert(53, 1, (index / 2809u) % 53u, perm) * 2809u +
            invert(53, 1, (index / 148877u) % 53u, perm) * 53u +
            invert(53, 1, (index / 7890481u) % 53u, perm)) * float(0x1.fffffcp-1 / 418195493u);
    }

    float halton59(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(59, 1, index % 59u, perm) * 12117361u +
            invert(59, 1, (index / 59u) % 59u, perm) * 205379u +
            invert(59, 1, (index / 3481u) % 59u, perm) * 3481u +
            invert(59, 1, (index / 205379u) % 59u, perm) * 59u +
            invert(59, 1, (index / 12117361u) % 59u, perm)) * float(0x1.fffffcp-1 / 714924299u);
    }

    float halton61(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(61, 1, index % 61u, perm) * 13845841u +
            invert(61, 1, (index / 61u) % 61u, perm) * 226981u +
            invert(61, 1, (index / 3721u) % 61u, perm) * 3721u +
            invert(61, 1, (index / 226981u) % 61u, perm) * 61u +
            invert(61, 1, (index / 13845841u) % 61u, perm)) * float(0x1.fffffcp-1 / 844596301u);
    }

    float halton67(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(67, 1, index % 67u, perm) * 20151121u +
            invert(67, 1, (index / 67u) % 67u, perm) * 300763u +
            invert(67, 1, (index / 4489u) % 67u, perm) * 4489u +
            invert(67, 1, (index / 300763u) % 67u, perm) * 67u +
            invert(67, 1, (index / 20151121u) % 67u, perm)) * float(0x1.fffffcp-1 / 1350125107u);
    }

    float halton71(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(71, 1, index % 71u, perm) * 25411681u +
            invert(71, 1, (index / 71u) % 71u, perm) * 357911u +
            invert(71, 1, (index / 5041u) % 71u, perm) * 5041u +
            invert(71, 1, (index / 357911u) % 71u, perm) * 71u +
            invert(71, 1, (index / 25411681u) % 71u, perm)) * float(0x1.fffffcp-1 / 1804229351u);
    }

    float halton73(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(73, 1, index % 73u, perm) * 28398241u +
            invert(73, 1, (index / 73u) % 73u, perm) * 389017u +
            invert(73, 1, (index / 5329u) % 73u, perm) * 5329u +
            invert(73, 1, (index / 389017u) % 73u, perm) * 73u +
            invert(73, 1, (index / 28398241u) % 73u, perm)) * float(0x1.fffffcp-1 / 2073071593u);
    }

    float halton79(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(79, 1, index % 79u, perm) * 38950081u +
            invert(79, 1, (index / 79u) % 79u, perm) * 493039u +
            invert(79, 1, (index / 6241u) % 79u, perm) * 6241u +
            invert(79, 1, (index / 493039u) % 79u, perm) * 79u +
            invert(79, 1, (index / 38950081u) % 79u, perm)) * float(0x1.fffffcp-1 / 3077056399u);
    }

    float halton83(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(83, 1, index % 83u, perm) * 47458321u +
            invert(83, 1, (index / 83u) % 83u, perm) * 571787u +
            invert(83, 1, (index / 6889u) % 83u, perm) * 6889u +
            invert(83, 1, (index / 571787u) % 83u, perm) * 83u +
            invert(83, 1, (index / 47458321u) % 83u, perm)) * float(0x1.fffffcp-1 / 3939040643u);
    }

    float halton89(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(89, 1, index % 89u, perm) * 704969u +
            invert(89, 1, (index / 89u) % 89u, perm) * 7921u +
            invert(89, 1, (index / 7921u) % 89u, perm) * 89u +
            invert(89, 1, (index / 704969u) % 89u, perm)) * float(0x1.fffffcp-1 / 62742241u);
    }

    float halton97(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(97, 1, index % 97u, perm) * 912673u +
            invert(97, 1, (index / 97u) % 97u, perm) * 9409u +
            invert(97, 1, (index / 9409u) % 97u, perm) * 97u +
            invert(97, 1, (index / 912673u) % 97u, perm)) * float(0x1.fffffcp-1 / 88529281u);
    }

    float halton101(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(101, 1, index % 101u, perm) * 1030301u +
            invert(101, 1, (index / 101u) % 101u, perm) * 10201u +
            invert(101, 1, (index / 10201u) % 101u, perm) * 101u +
            invert(101, 1, (index / 1030301u) % 101u, perm)) * float(0x1.fffffcp-1 / 104060401u);
    }

    float halton103(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(103, 1, index % 103u, perm) * 1092727u +
            invert(103, 1, (index / 103u) % 103u, perm) * 10609u +
            invert(103, 1, (index / 10609u) % 103u, perm) * 103u +
            invert(103, 1, (index / 1092727u) % 103u, perm)) * float(0x1.fffffcp-1 / 112550881u);
    }

    float halton107(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(107, 1, index % 107u, perm) * 1225043u +
            invert(107, 1, (index / 107u) % 107u, perm) * 11449u +
            invert(107, 1, (index / 11449u) % 107u, perm) * 107u +
            invert(107, 1, (index / 1225043u) % 107u, perm)) * float(0x1.fffffcp-1 / 131079601u);
    }

    float halton109(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(109, 1, index % 109u, perm) * 1295029u +
            invert(109, 1, (index / 109u) % 109u, perm) * 11881u +
            invert(109, 1, (index / 11881u) % 109u, perm) * 109u +
            invert(109, 1, (index / 1295029u) % 109u, perm)) * float(0x1.fffffcp-1 / 141158161u);
    }

    float halton113(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(113, 1, index % 113u, perm) * 1442897u +
            invert(113, 1, (index / 113u) % 113u, perm) * 12769u +
            invert(113, 1, (index / 12769u) % 113u, perm) * 113u +
            invert(113, 1, (index / 1442897u) % 113u, perm)) * float(0x1.fffffcp-1 / 163047361u);
    }

    float halton127(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(127, 1, index % 127u, perm) * 2048383u +
            invert(127, 1, (index / 127u) % 127u, perm) * 16129u +
            invert(127, 1, (index / 16129u) % 127u, perm) * 127u +
            invert(127, 1, (index / 2048383u) % 127u, perm)) * float(0x1.fffffcp-1 / 260144641u);
    }

    float halton131(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(131, 1, index % 131u, perm) * 2248091u +
            invert(131, 1, (index / 131u) % 131u, perm) * 17161u +
            invert(131, 1, (index / 17161u) % 131u, perm) * 131u +
            invert(131, 1, (index / 2248091u) % 131u, perm)) * float(0x1.fffffcp-1 / 294499921u);
    }

    float halton137(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(137, 1, index % 137u, perm) * 2571353u +
            invert(137, 1, (index / 137u) % 137u, perm) * 18769u +
            invert(137, 1, (index / 18769u) % 137u, perm) * 137u +
            invert(137, 1, (index / 2571353u) % 137u, perm)) * float(0x1.fffffcp-1 / 352275361u);
    }

    float halton139(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(139, 1, index % 139u, perm) * 2685619u +
            invert(139, 1, (index / 139u) % 139u, perm) * 19321u +
            invert(139, 1, (index / 19321u) % 139u, perm) * 139u +
            invert(139, 1, (index / 2685619u) % 139u, perm)) * float(0x1.fffffcp-1 / 373301041u);
    }

    float halton149(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(149, 1, index % 149u, perm) * 3307949u +
            invert(149, 1, (index / 149u) % 149u, perm) * 22201u +
            invert(149, 1, (index / 22201u) % 149u, perm) * 149u +
            invert(149, 1, (index / 3307949u) % 149u, perm)) * float(0x1.fffffcp-1 / 492884401u);
    }

    float halton151(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(151, 1, index % 151u, perm) * 3442951u +
            invert(151, 1, (index / 151u) % 151u, perm) * 22801u +
            invert(151, 1, (index / 22801u) % 151u, perm) * 151u +
            invert(151, 1, (index / 3442951u) % 151u, perm)) * float(0x1.fffffcp-1 / 519885601u);
    }

    float halton157(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(157, 1, index % 157u, perm) * 3869893u +
            invert(157, 1, (index / 157u) % 157u, perm) * 24649u +
            invert(157, 1, (index / 24649u) % 157u, perm) * 157u +
            invert(157, 1, (index / 3869893u) % 157u, perm)) * float(0x1.fffffcp-1 / 607573201u);
    }

    float halton163(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(163, 1, index % 163u, perm) * 4330747u +
            invert(163, 1, (index / 163u) % 163u, perm) * 26569u +
            invert(163, 1, (index / 26569u) % 163u, perm) * 163u +
            invert(163, 1, (index / 4330747u) % 163u, perm)) * float(0x1.fffffcp-1 / 705911761u);
    }

    float halton167(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(167, 1, index % 167u, perm) * 4657463u +
            invert(167, 1, (index / 167u) % 167u, perm) * 27889u +
            invert(167, 1, (index / 27889u) % 167u, perm) * 167u +
            invert(167, 1, (index / 4657463u) % 167u, perm)) * float(0x1.fffffcp-1 / 777796321u);
    }

    float halton173(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(173, 1, index % 173u, perm) * 5177717u +
            invert(173, 1, (index / 173u) % 173u, perm) * 29929u +
            invert(173, 1, (index / 29929u) % 173u, perm) * 173u +
            invert(173, 1, (index / 5177717u) % 173u, perm)) * float(0x1.fffffcp-1 / 895745041u);
    }

    float halton179(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(179, 1, index % 179u, perm) * 5735339u +
            invert(179, 1, (index / 179u) % 179u, perm) * 32041u +
            invert(179, 1, (index / 32041u) % 179u, perm) * 179u +
            invert(179, 1, (index / 5735339u) % 179u, perm)) * float(0x1.fffffcp-1 / 1026625681u);
    }

    float halton181(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(181, 1, index % 181u, perm) * 5929741u +
            invert(181, 1, (index / 181u) % 181u, perm) * 32761u +
            invert(181, 1, (index / 32761u) % 181u, perm) * 181u +
            invert(181, 1, (index / 5929741u) % 181u, perm)) * float(0x1.fffffcp-1 / 1073283121u);
    }

    float halton191(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(191, 1, index % 191u, perm) * 6967871u +
            invert(191, 1, (index / 191u) % 191u, perm) * 36481u +
            invert(191, 1, (index / 36481u) % 191u, perm) * 191u +
            invert(191, 1, (index / 6967871u) % 191u, perm)) * float(0x1.fffffcp-1 / 1330863361u);
    }

    float halton193(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(193, 1, index % 193u, perm) * 7189057u +
            invert(193, 1, (index / 193u) % 193u, perm) * 37249u +
            invert(193, 1, (index / 37249u) % 193u, perm) * 193u +
            invert(193, 1, (index / 7189057u) % 193u, perm)) * float(0x1.fffffcp-1 / 1387488001u);
    }

    float halton197(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(197, 1, index % 197u, perm) * 7645373u +
            invert(197, 1, (index / 197u) % 197u, perm) * 38809u +
            invert(197, 1, (index / 38809u) % 197u, perm) * 197u +
            invert(197, 1, (index / 7645373u) % 197u, perm)) * float(0x1.fffffcp-1 / 1506138481u);
    }

    float halton199(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(199, 1, index % 199u, perm) * 7880599u +
            invert(199, 1, (index / 199u) % 199u, perm) * 39601u +
            invert(199, 1, (index / 39601u) % 199u, perm) * 199u +
            invert(199, 1, (index / 7880599u) % 199u, perm)) * float(0x1.fffffcp-1 / 1568239201u);
    }

    float halton211(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(211, 1, index % 211u, perm) * 9393931u +
            invert(211, 1, (index / 211u) % 211u, perm) * 44521u +
            invert(211, 1, (index / 44521u) % 211u, perm) * 211u +
            invert(211, 1, (index / 9393931u) % 211u, perm)) * float(0x1.fffffcp-1 / 1982119441u);
    }

    float halton223(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(223, 1, index % 223u, perm) * 11089567u +
            invert(223, 1, (index / 223u) % 223u, perm) * 49729u +
            invert(223, 1, (index / 49729u) % 223u, perm) * 223u +
            invert(223, 1, (index / 11089567u) % 223u, perm)) * float(0x1.fffffcp-1 / 2472973441u);
    }

    float halton227(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(227, 1, index % 227u, perm) * 11697083u +
            invert(227, 1, (index / 227u) % 227u, perm) * 51529u +
            invert(227, 1, (index / 51529u) % 227u, perm) * 227u +
            invert(227, 1, (index / 11697083u) % 227u, perm)) * float(0x1.fffffcp-1 / 2655237841u);
    }

    float halton229(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(229, 1, index % 229u, perm) * 12008989u +
            invert(229, 1, (index / 229u) % 229u, perm) * 52441u +
            invert(229, 1, (index / 52441u) % 229u, perm) * 229u +
            invert(229, 1, (index / 12008989u) % 229u, perm)) * float(0x1.fffffcp-1 / 2750058481u);
    }

    float halton233(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(233, 1, index % 233u, perm) * 12649337u +
            invert(233, 1, (index / 233u) % 233u, perm) * 54289u +
            invert(233, 1, (index / 54289u) % 233u, perm) * 233u +
            invert(233, 1, (index / 12649337u) % 233u, perm)) * float(0x1.fffffcp-1 / 2947295521u);
    }

    float halton239(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(239, 1, index % 239u, perm) * 13651919u +
            invert(239, 1, (index / 239u) % 239u, perm) * 57121u +
            invert(239, 1, (index / 57121u) % 239u, perm) * 239u +
            invert(239, 1, (index / 13651919u) % 239u, perm)) * float(0x1.fffffcp-1 / 3262808641u);
    }

    float halton241(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(241, 1, index % 241u, perm) * 13997521u +
            invert(241, 1, (index / 241u) % 241u, perm) * 58081u +
            invert(241, 1, (index / 58081u) % 241u, perm) * 241u +
            invert(241, 1, (index / 13997521u) % 241u, perm)) * float(0x1.fffffcp-1 / 3373402561u);
    }

    float halton251(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(251, 1, index % 251u, perm) * 15813251u +
            invert(251, 1, (index / 251u) % 251u, perm) * 63001u +
            invert(251, 1, (index / 63001u) % 251u, perm) * 251u +
            invert(251, 1, (index / 15813251u) % 251u, perm)) * float(0x1.fffffcp-1 / 3969126001u);
    }

    float halton257(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(257, 1, index % 257u, perm) * 66049u +
            invert(257, 1, (index / 257u) % 257u, perm) * 257u +
            invert(257, 1, (index / 66049u) % 257u, perm)) * float(0x1.fffffcp-1 / 16974593u);
    }

    float halton263(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(263, 1, index % 263u, perm) * 69169u +
            invert(263, 1, (index / 263u) % 263u, perm) * 263u +
            invert(263, 1, (index / 69169u) % 263u, perm)) * float(0x1.fffffcp-1 / 18191447u);
    }

    float halton269(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(269, 1, index % 269u, perm) * 72361u +
            invert(269, 1, (index / 269u) % 269u, perm) * 269u +
            invert(269, 1, (index / 72361u) % 269u, perm)) * float(0x1.fffffcp-1 / 19465109u);
    }

    float halton271(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(271, 1, index % 271u, perm) * 73441u +
            invert(271, 1, (index / 271u) % 271u, perm) * 271u +
            invert(271, 1, (index / 73441u) % 271u, perm)) * float(0x1.fffffcp-1 / 19902511u);
    }

    float halton277(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(277, 1, index % 277u, perm) * 76729u +
            invert(277, 1, (index / 277u) % 277u, perm) * 277u +
            invert(277, 1, (index / 76729u) % 277u, perm)) * float(0x1.fffffcp-1 / 21253933u);
    }

    float halton281(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(281, 1, index % 281u, perm) * 78961u +
            invert(281, 1, (index / 281u) % 281u, perm) * 281u +
            invert(281, 1, (index / 78961u) % 281u, perm)) * float(0x1.fffffcp-1 / 22188041u);
    }

    float halton283(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(283, 1, index % 283u, perm) * 80089u +
            invert(283, 1, (index / 283u) % 283u, perm) * 283u +
            invert(283, 1, (index / 80089u) % 283u, perm)) * float(0x1.fffffcp-1 / 22665187u);
    }

    float halton293(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(293, 1, index % 293u, perm) * 85849u +
            invert(293, 1, (index / 293u) % 293u, perm) * 293u +
            invert(293, 1, (index / 85849u) % 293u, perm)) * float(0x1.fffffcp-1 / 25153757u);
    }

    float halton307(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(307, 1, index % 307u, perm) * 94249u +
            invert(307, 1, (index / 307u) % 307u, perm) * 307u +
            invert(307, 1, (index / 94249u) % 307u, perm)) * float(0x1.fffffcp-1 / 28934443u);
    }

    float halton311(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(311, 1, index % 311u, perm) * 96721u +
            invert(311, 1, (index / 311u) % 311u, perm) * 311u +
            invert(311, 1, (index / 96721u) % 311u, perm)) * float(0x1.fffffcp-1 / 30080231u);
    }

    float halton313(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(313, 1, index % 313u, perm) * 97969u +
            invert(313, 1, (index / 313u) % 313u, perm) * 313u +
            invert(313, 1, (index / 97969u) % 313u, perm)) * float(0x1.fffffcp-1 / 30664297u);
    }

    float halton317(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(317, 1, index % 317u, perm) * 100489u +
            invert(317, 1, (index / 317u) % 317u, perm) * 317u +
            invert(317, 1, (index / 100489u) % 317u, perm)) * float(0x1.fffffcp-1 / 31855013u);
    }

    float halton331(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(331, 1, index % 331u, perm) * 109561u +
            invert(331, 1, (index / 331u) % 331u, perm) * 331u +
            invert(331, 1, (index / 109561u) % 331u, perm)) * float(0x1.fffffcp-1 / 36264691u);
    }

    float halton337(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(337, 1, index % 337u, perm) * 113569u +
            invert(337, 1, (index / 337u) % 337u, perm) * 337u +
            invert(337, 1, (index / 113569u) % 337u, perm)) * float(0x1.fffffcp-1 / 38272753u);
    }

    float halton347(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(347, 1, index % 347u, perm) * 120409u +
            invert(347, 1, (index / 347u) % 347u, perm) * 347u +
            invert(347, 1, (index / 120409u) % 347u, perm)) * float(0x1.fffffcp-1 / 41781923u);
    }

    float halton349(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(349, 1, index % 349u, perm) * 121801u +
            invert(349, 1, (index / 349u) % 349u, perm) * 349u +
            invert(349, 1, (index / 121801u) % 349u, perm)) * float(0x1.fffffcp-1 / 42508549u);
    }

    float halton353(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(353, 1, index % 353u, perm) * 124609u +
            invert(353, 1, (index / 353u) % 353u, perm) * 353u +
            invert(353, 1, (index / 124609u) % 353u, perm)) * float(0x1.fffffcp-1 / 43986977u);
    }

    float halton359(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(359, 1, index % 359u, perm) * 128881u +
            invert(359, 1, (index / 359u) % 359u, perm) * 359u +
            invert(359, 1, (index / 128881u) % 359u, perm)) * float(0x1.fffffcp-1 / 46268279u);
    }

    float halton367(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(367, 1, index % 367u, perm) * 134689u +
            invert(367, 1, (index / 367u) % 367u, perm) * 367u +
            invert(367, 1, (index / 134689u) % 367u, perm)) * float(0x1.fffffcp-1 / 49430863u);
    }

    float halton373(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(373, 1, index % 373u, perm) * 139129u +
            invert(373, 1, (index / 373u) % 373u, perm) * 373u +
            invert(373, 1, (index / 139129u) % 373u, perm)) * float(0x1.fffffcp-1 / 51895117u);
    }

    float halton379(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(379, 1, index % 379u, perm) * 143641u +
            invert(379, 1, (index / 379u) % 379u, perm) * 379u +
            invert(379, 1, (index / 143641u) % 379u, perm)) * float(0x1.fffffcp-1 / 54439939u);
    }

    float halton383(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(383, 1, index % 383u, perm) * 146689u +
            invert(383, 1, (index / 383u) % 383u, perm) * 383u +
            invert(383, 1, (index / 146689u) % 383u, perm)) * float(0x1.fffffcp-1 / 56181887u);
    }

    float halton389(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(389, 1, index % 389u, perm) * 151321u +
            invert(389, 1, (index / 389u) % 389u, perm) * 389u +
            invert(389, 1, (index / 151321u) % 389u, perm)) * float(0x1.fffffcp-1 / 58863869u);
    }

    float halton397(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(397, 1, index % 397u, perm) * 157609u +
            invert(397, 1, (index / 397u) % 397u, perm) * 397u +
            invert(397, 1, (index / 157609u) % 397u, perm)) * float(0x1.fffffcp-1 / 62570773u);
    }

    float halton401(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(401, 1, index % 401u, perm) * 160801u +
            invert(401, 1, (index / 401u) % 401u, perm) * 401u +
            invert(401, 1, (index / 160801u) % 401u, perm)) * float(0x1.fffffcp-1 / 64481201u);
    }

    float halton409(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(409, 1, index % 409u, perm) * 167281u +
            invert(409, 1, (index / 409u) % 409u, perm) * 409u +
            invert(409, 1, (index / 167281u) % 409u, perm)) * float(0x1.fffffcp-1 / 68417929u);
    }

    float halton419(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(419, 1, index % 419u, perm) * 175561u +
            invert(419, 1, (index / 419u) % 419u, perm) * 419u +
            invert(419, 1, (index / 175561u) % 419u, perm)) * float(0x1.fffffcp-1 / 73560059u);
    }

    float halton421(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(421, 1, index % 421u, perm) * 177241u +
            invert(421, 1, (index / 421u) % 421u, perm) * 421u +
            invert(421, 1, (index / 177241u) % 421u, perm)) * float(0x1.fffffcp-1 / 74618461u);
    }

    float halton431(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(431, 1, index % 431u, perm) * 185761u +
            invert(431, 1, (index / 431u) % 431u, perm) * 431u +
            invert(431, 1, (index / 185761u) % 431u, perm)) * float(0x1.fffffcp-1 / 80062991u);
    }

    float halton433(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(433, 1, index % 433u, perm) * 187489u +
            invert(433, 1, (index / 433u) % 433u, perm) * 433u +
            invert(433, 1, (index / 187489u) % 433u, perm)) * float(0x1.fffffcp-1 / 81182737u);
    }

    float halton439(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(439, 1, index % 439u, perm) * 192721u +
            invert(439, 1, (index / 439u) % 439u, perm) * 439u +
            invert(439, 1, (index / 192721u) % 439u, perm)) * float(0x1.fffffcp-1 / 84604519u);
    }

    float halton443(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(443, 1, index % 443u, perm) * 196249u +
            invert(443, 1, (index / 443u) % 443u, perm) * 443u +
            invert(443, 1, (index / 196249u) % 443u, perm)) * float(0x1.fffffcp-1 / 86938307u);
    }

    float halton449(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(449, 1, index % 449u, perm) * 201601u +
            invert(449, 1, (index / 449u) % 449u, perm) * 449u +
            invert(449, 1, (index / 201601u) % 449u, perm)) * float(0x1.fffffcp-1 / 90518849u);
    }

    float halton457(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(457, 1, index % 457u, perm) * 208849u +
            invert(457, 1, (index / 457u) % 457u, perm) * 457u +
            invert(457, 1, (index / 208849u) % 457u, perm)) * float(0x1.fffffcp-1 / 95443993u);
    }

    float halton461(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(461, 1, index % 461u, perm) * 212521u +
            invert(461, 1, (index / 461u) % 461u, perm) * 461u +
            invert(461, 1, (index / 212521u) % 461u, perm)) * float(0x1.fffffcp-1 / 97972181u);
    }

    float halton463(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(463, 1, index % 463u, perm) * 214369u +
            invert(463, 1, (index / 463u) % 463u, perm) * 463u +
            invert(463, 1, (index / 214369u) % 463u, perm)) * float(0x1.fffffcp-1 / 99252847u);
    }

    float halton467(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(467, 1, index % 467u, perm) * 218089u +
            invert(467, 1, (index / 467u) % 467u, perm) * 467u +
            invert(467, 1, (index / 218089u) % 467u, perm)) * float(0x1.fffffcp-1 / 101847563u);
    }

    float halton479(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(479, 1, index % 479u, perm) * 229441u +
            invert(479, 1, (index / 479u) % 479u, perm) * 479u +
            invert(479, 1, (index / 229441u) % 479u, perm)) * float(0x1.fffffcp-1 / 109902239u);
    }

    float halton487(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(487, 1, index % 487u, perm) * 237169u +
            invert(487, 1, (index / 487u) % 487u, perm) * 487u +
            invert(487, 1, (index / 237169u) % 487u, perm)) * float(0x1.fffffcp-1 / 115501303u);
    }

    float halton491(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(491, 1, index % 491u, perm) * 241081u +
            invert(491, 1, (index / 491u) % 491u, perm) * 491u +
            invert(491, 1, (index / 241081u) % 491u, perm)) * float(0x1.fffffcp-1 / 118370771u);
    }

    float halton499(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(499, 1, index % 499u, perm) * 249001u +
            invert(499, 1, (index / 499u) % 499u, perm) * 499u +
            invert(499, 1, (index / 249001u) % 499u, perm)) * float(0x1.fffffcp-1 / 124251499u);
    }

    float halton503(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(503, 1, index % 503u, perm) * 253009u +
            invert(503, 1, (index / 503u) % 503u, perm) * 503u +
            invert(503, 1, (index / 253009u) % 503u, perm)) * float(0x1.fffffcp-1 / 127263527u);
    }

    float halton509(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(509, 1, index % 509u, perm) * 259081u +
            invert(509, 1, (index / 509u) % 509u, perm) * 509u +
            invert(509, 1, (index / 259081u) % 509u, perm)) * float(0x1.fffffcp-1 / 131872229u);
    }

    float halton521(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(521, 1, index % 521u, perm) * 271441u +
            invert(521, 1, (index / 521u) % 521u, perm) * 521u +
            invert(521, 1, (index / 271441u) % 521u, perm)) * float(0x1.fffffcp-1 / 141420761u);
    }

    float halton523(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(523, 1, index % 523u, perm) * 273529u +
            invert(523, 1, (index / 523u) % 523u, perm) * 523u +
            invert(523, 1, (index / 273529u) % 523u, perm)) * float(0x1.fffffcp-1 / 143055667u);
    }

    float halton541(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(541, 1, index % 541u, perm) * 292681u +
            invert(541, 1, (index / 541u) % 541u, perm) * 541u +
            invert(541, 1, (index / 292681u) % 541u, perm)) * float(0x1.fffffcp-1 / 158340421u);
    }

    float halton547(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(547, 1, index % 547u, perm) * 299209u +
            invert(547, 1, (index / 547u) % 547u, perm) * 547u +
            invert(547, 1, (index / 299209u) % 547u, perm)) * float(0x1.fffffcp-1 / 163667323u);
    }

    float halton557(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(557, 1, index % 557u, perm) * 310249u +
            invert(557, 1, (index / 557u) % 557u, perm) * 557u +
            invert(557, 1, (index / 310249u) % 557u, perm)) * float(0x1.fffffcp-1 / 172808693u);
    }

    float halton563(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(563, 1, index % 563u, perm) * 316969u +
            invert(563, 1, (index / 563u) % 563u, perm) * 563u +
            invert(563, 1, (index / 316969u) % 563u, perm)) * float(0x1.fffffcp-1 / 178453547u);
    }

    float halton569(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(569, 1, index % 569u, perm) * 323761u +
            invert(569, 1, (index / 569u) % 569u, perm) * 569u +
            invert(569, 1, (index / 323761u) % 569u, perm)) * float(0x1.fffffcp-1 / 184220009u);
    }

    float halton571(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(571, 1, index % 571u, perm) * 326041u +
            invert(571, 1, (index / 571u) % 571u, perm) * 571u +
            invert(571, 1, (index / 326041u) % 571u, perm)) * float(0x1.fffffcp-1 / 186169411u);
    }

    float halton577(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(577, 1, index % 577u, perm) * 332929u +
            invert(577, 1, (index / 577u) % 577u, perm) * 577u +
            invert(577, 1, (index / 332929u) % 577u, perm)) * float(0x1.fffffcp-1 / 192100033u);
    }

    float halton587(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(587, 1, index % 587u, perm) * 344569u +
            invert(587, 1, (index / 587u) % 587u, perm) * 587u +
            invert(587, 1, (index / 344569u) % 587u, perm)) * float(0x1.fffffcp-1 / 202262003u);
    }

    float halton593(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(593, 1, index % 593u, perm) * 351649u +
            invert(593, 1, (index / 593u) % 593u, perm) * 593u +
            invert(593, 1, (index / 351649u) % 593u, perm)) * float(0x1.fffffcp-1 / 208527857u);
    }

    float halton599(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(599, 1, index % 599u, perm) * 358801u +
            invert(599, 1, (index / 599u) % 599u, perm) * 599u +
            invert(599, 1, (index / 358801u) % 599u, perm)) * float(0x1.fffffcp-1 / 214921799u);
    }

    float halton601(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(601, 1, index % 601u, perm) * 361201u +
            invert(601, 1, (index / 601u) % 601u, perm) * 601u +
            invert(601, 1, (index / 361201u) % 601u, perm)) * float(0x1.fffffcp-1 / 217081801u);
    }

    float halton607(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(607, 1, index % 607u, perm) * 368449u +
            invert(607, 1, (index / 607u) % 607u, perm) * 607u +
            invert(607, 1, (index / 368449u) % 607u, perm)) * float(0x1.fffffcp-1 / 223648543u);
    }

    float halton613(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(613, 1, index % 613u, perm) * 375769u +
            invert(613, 1, (index / 613u) % 613u, perm) * 613u +
            invert(613, 1, (index / 375769u) % 613u, perm)) * float(0x1.fffffcp-1 / 230346397u);
    }

    float halton617(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(617, 1, index % 617u, perm) * 380689u +
            invert(617, 1, (index / 617u) % 617u, perm) * 617u +
            invert(617, 1, (index / 380689u) % 617u, perm)) * float(0x1.fffffcp-1 / 234885113u);
    }

    float halton619(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(619, 1, index % 619u, perm) * 383161u +
            invert(619, 1, (index / 619u) % 619u, perm) * 619u +
            invert(619, 1, (index / 383161u) % 619u, perm)) * float(0x1.fffffcp-1 / 237176659u);
    }

    float halton631(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(631, 1, index % 631u, perm) * 398161u +
            invert(631, 1, (index / 631u) % 631u, perm) * 631u +
            invert(631, 1, (index / 398161u) % 631u, perm)) * float(0x1.fffffcp-1 / 251239591u);
    }

    float halton641(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(641, 1, index % 641u, perm) * 410881u +
            invert(641, 1, (index / 641u) % 641u, perm) * 641u +
            invert(641, 1, (index / 410881u) % 641u, perm)) * float(0x1.fffffcp-1 / 263374721u);
    }

    float halton643(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(643, 1, index % 643u, perm) * 413449u +
            invert(643, 1, (index / 643u) % 643u, perm) * 643u +
            invert(643, 1, (index / 413449u) % 643u, perm)) * float(0x1.fffffcp-1 / 265847707u);
    }

    float halton647(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(647, 1, index % 647u, perm) * 418609u +
            invert(647, 1, (index / 647u) % 647u, perm) * 647u +
            invert(647, 1, (index / 418609u) % 647u, perm)) * float(0x1.fffffcp-1 / 270840023u);
    }

    float halton653(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(653, 1, index % 653u, perm) * 426409u +
            invert(653, 1, (index / 653u) % 653u, perm) * 653u +
            invert(653, 1, (index / 426409u) % 653u, perm)) * float(0x1.fffffcp-1 / 278445077u);
    }

    float halton659(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(659, 1, index % 659u, perm) * 434281u +
            invert(659, 1, (index / 659u) % 659u, perm) * 659u +
            invert(659, 1, (index / 434281u) % 659u, perm)) * float(0x1.fffffcp-1 / 286191179u);
    }

    float halton661(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(661, 1, index % 661u, perm) * 436921u +
            invert(661, 1, (index / 661u) % 661u, perm) * 661u +
            invert(661, 1, (index / 436921u) % 661u, perm)) * float(0x1.fffffcp-1 / 288804781u);
    }

    float halton673(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(673, 1, index % 673u, perm) * 452929u +
            invert(673, 1, (index / 673u) % 673u, perm) * 673u +
            invert(673, 1, (index / 452929u) % 673u, perm)) * float(0x1.fffffcp-1 / 304821217u);
    }

    float halton677(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(677, 1, index % 677u, perm) * 458329u +
            invert(677, 1, (index / 677u) % 677u, perm) * 677u +
            invert(677, 1, (index / 458329u) % 677u, perm)) * float(0x1.fffffcp-1 / 310288733u);
    }

    float halton683(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(683, 1, index % 683u, perm) * 466489u +
            invert(683, 1, (index / 683u) % 683u, perm) * 683u +
            invert(683, 1, (index / 466489u) % 683u, perm)) * float(0x1.fffffcp-1 / 318611987u);
    }

    float halton691(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(691, 1, index % 691u, perm) * 477481u +
            invert(691, 1, (index / 691u) % 691u, perm) * 691u +
            invert(691, 1, (index / 477481u) % 691u, perm)) * float(0x1.fffffcp-1 / 329939371u);
    }

    float halton701(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(701, 1, index % 701u, perm) * 491401u +
            invert(701, 1, (index / 701u) % 701u, perm) * 701u +
            invert(701, 1, (index / 491401u) % 701u, perm)) * float(0x1.fffffcp-1 / 344472101u);
    }

    float halton709(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(709, 1, index % 709u, perm) * 502681u +
            invert(709, 1, (index / 709u) % 709u, perm) * 709u +
            invert(709, 1, (index / 502681u) % 709u, perm)) * float(0x1.fffffcp-1 / 356400829u);
    }

    float halton719(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(719, 1, index % 719u, perm) * 516961u +
            invert(719, 1, (index / 719u) % 719u, perm) * 719u +
            invert(719, 1, (index / 516961u) % 719u, perm)) * float(0x1.fffffcp-1 / 371694959u);
    }

    float halton727(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(727, 1, index % 727u, perm) * 528529u +
            invert(727, 1, (index / 727u) % 727u, perm) * 727u +
            invert(727, 1, (index / 528529u) % 727u, perm)) * float(0x1.fffffcp-1 / 384240583u);
    }

    float halton733(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(733, 1, index % 733u, perm) * 537289u +
            invert(733, 1, (index / 733u) % 733u, perm) * 733u +
            invert(733, 1, (index / 537289u) % 733u, perm)) * float(0x1.fffffcp-1 / 393832837u);
    }

    float halton739(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(739, 1, index % 739u, perm) * 546121u +
            invert(739, 1, (index / 739u) % 739u, perm) * 739u +
            invert(739, 1, (index / 546121u) % 739u, perm)) * float(0x1.fffffcp-1 / 403583419u);
    }

    float halton743(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(743, 1, index % 743u, perm) * 552049u +
            invert(743, 1, (index / 743u) % 743u, perm) * 743u +
            invert(743, 1, (index / 552049u) % 743u, perm)) * float(0x1.fffffcp-1 / 410172407u);
    }

    float halton751(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(751, 1, index % 751u, perm) * 564001u +
            invert(751, 1, (index / 751u) % 751u, perm) * 751u +
            invert(751, 1, (index / 564001u) % 751u, perm)) * float(0x1.fffffcp-1 / 423564751u);
    }

    float halton757(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(757, 1, index % 757u, perm) * 573049u +
            invert(757, 1, (index / 757u) % 757u, perm) * 757u +
            invert(757, 1, (index / 573049u) % 757u, perm)) * float(0x1.fffffcp-1 / 433798093u);
    }

    float halton761(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(761, 1, index % 761u, perm) * 579121u +
            invert(761, 1, (index / 761u) % 761u, perm) * 761u +
            invert(761, 1, (index / 579121u) % 761u, perm)) * float(0x1.fffffcp-1 / 440711081u);
    }

    float halton769(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(769, 1, index % 769u, perm) * 591361u +
            invert(769, 1, (index / 769u) % 769u, perm) * 769u +
            invert(769, 1, (index / 591361u) % 769u, perm)) * float(0x1.fffffcp-1 / 454756609u);
    }

    float halton773(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(773, 1, index % 773u, perm) * 597529u +
            invert(773, 1, (index / 773u) % 773u, perm) * 773u +
            invert(773, 1, (index / 597529u) % 773u, perm)) * float(0x1.fffffcp-1 / 461889917u);
    }

    float halton787(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(787, 1, index % 787u, perm) * 619369u +
            invert(787, 1, (index / 787u) % 787u, perm) * 787u +
            invert(787, 1, (index / 619369u) % 787u, perm)) * float(0x1.fffffcp-1 / 487443403u);
    }

    float halton797(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(797, 1, index % 797u, perm) * 635209u +
            invert(797, 1, (index / 797u) % 797u, perm) * 797u +
            invert(797, 1, (index / 635209u) % 797u, perm)) * float(0x1.fffffcp-1 / 506261573u);
    }

    float halton809(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(809, 1, index % 809u, perm) * 654481u +
            invert(809, 1, (index / 809u) % 809u, perm) * 809u +
            invert(809, 1, (index / 654481u) % 809u, perm)) * float(0x1.fffffcp-1 / 529475129u);
    }

    float halton811(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(811, 1, index % 811u, perm) * 657721u +
            invert(811, 1, (index / 811u) % 811u, perm) * 811u +
            invert(811, 1, (index / 657721u) % 811u, perm)) * float(0x1.fffffcp-1 / 533411731u);
    }

    float halton821(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(821, 1, index % 821u, perm) * 674041u +
            invert(821, 1, (index / 821u) % 821u, perm) * 821u +
            invert(821, 1, (index / 674041u) % 821u, perm)) * float(0x1.fffffcp-1 / 553387661u);
    }

    float halton823(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(823, 1, index % 823u, perm) * 677329u +
            invert(823, 1, (index / 823u) % 823u, perm) * 823u +
            invert(823, 1, (index / 677329u) % 823u, perm)) * float(0x1.fffffcp-1 / 557441767u);
    }

    float halton827(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(827, 1, index % 827u, perm) * 683929u +
            invert(827, 1, (index / 827u) % 827u, perm) * 827u +
            invert(827, 1, (index / 683929u) % 827u, perm)) * float(0x1.fffffcp-1 / 565609283u);
    }

    float halton829(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(829, 1, index % 829u, perm) * 687241u +
            invert(829, 1, (index / 829u) % 829u, perm) * 829u +
            invert(829, 1, (index / 687241u) % 829u, perm)) * float(0x1.fffffcp-1 / 569722789u);
    }

    float halton839(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(839, 1, index % 839u, perm) * 703921u +
            invert(839, 1, (index / 839u) % 839u, perm) * 839u +
            invert(839, 1, (index / 703921u) % 839u, perm)) * float(0x1.fffffcp-1 / 590589719u);
    }

    float halton853(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(853, 1, index % 853u, perm) * 727609u +
            invert(853, 1, (index / 853u) % 853u, perm) * 853u +
            invert(853, 1, (index / 727609u) % 853u, perm)) * float(0x1.fffffcp-1 / 620650477u);
    }

    float halton857(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(857, 1, index % 857u, perm) * 734449u +
            invert(857, 1, (index / 857u) % 857u, perm) * 857u +
            invert(857, 1, (index / 734449u) % 857u, perm)) * float(0x1.fffffcp-1 / 629422793u);
    }

    float halton859(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(859, 1, index % 859u, perm) * 737881u +
            invert(859, 1, (index / 859u) % 859u, perm) * 859u +
            invert(859, 1, (index / 737881u) % 859u, perm)) * float(0x1.fffffcp-1 / 633839779u);
    }

    float halton863(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(863, 1, index % 863u, perm) * 744769u +
            invert(863, 1, (index / 863u) % 863u, perm) * 863u +
            invert(863, 1, (index / 744769u) % 863u, perm)) * float(0x1.fffffcp-1 / 642735647u);
    }

    float halton877(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(877, 1, index % 877u, perm) * 769129u +
            invert(877, 1, (index / 877u) % 877u, perm) * 877u +
            invert(877, 1, (index / 769129u) % 877u, perm)) * float(0x1.fffffcp-1 / 674526133u);
    }

    float halton881(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(881, 1, index % 881u, perm) * 776161u +
            invert(881, 1, (index / 881u) % 881u, perm) * 881u +
            invert(881, 1, (index / 776161u) % 881u, perm)) * float(0x1.fffffcp-1 / 683797841u);
    }

    float halton883(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(883, 1, index % 883u, perm) * 779689u +
            invert(883, 1, (index / 883u) % 883u, perm) * 883u +
            invert(883, 1, (index / 779689u) % 883u, perm)) * float(0x1.fffffcp-1 / 688465387u);
    }

    float halton887(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(887, 1, index % 887u, perm) * 786769u +
            invert(887, 1, (index / 887u) % 887u, perm) * 887u +
            invert(887, 1, (index / 786769u) % 887u, perm)) * float(0x1.fffffcp-1 / 697864103u);
    }

    float halton907(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(907, 1, index % 907u, perm) * 822649u +
            invert(907, 1, (index / 907u) % 907u, perm) * 907u +
            invert(907, 1, (index / 822649u) % 907u, perm)) * float(0x1.fffffcp-1 / 746142643u);
    }

    float halton911(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(911, 1, index % 911u, perm) * 829921u +
            invert(911, 1, (index / 911u) % 911u, perm) * 911u +
            invert(911, 1, (index / 829921u) % 911u, perm)) * float(0x1.fffffcp-1 / 756058031u);
    }

    float halton919(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(919, 1, index % 919u, perm) * 844561u +
            invert(919, 1, (index / 919u) % 919u, perm) * 919u +
            invert(919, 1, (index / 844561u) % 919u, perm)) * float(0x1.fffffcp-1 / 776151559u);
    }

    float halton929(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(929, 1, index % 929u, perm) * 863041u +
            invert(929, 1, (index / 929u) % 929u, perm) * 929u +
            invert(929, 1, (index / 863041u) % 929u, perm)) * float(0x1.fffffcp-1 / 801765089u);
    }

    float halton937(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(937, 1, index % 937u, perm) * 877969u +
            invert(937, 1, (index / 937u) % 937u, perm) * 937u +
            invert(937, 1, (index / 877969u) % 937u, perm)) * float(0x1.fffffcp-1 / 822656953u);
    }

    float halton941(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(941, 1, index % 941u, perm) * 885481u +
            invert(941, 1, (index / 941u) % 941u, perm) * 941u +
            invert(941, 1, (index / 885481u) % 941u, perm)) * float(0x1.fffffcp-1 / 833237621u);
    }

    float halton947(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(947, 1, index % 947u, perm) * 896809u +
            invert(947, 1, (index / 947u) % 947u, perm) * 947u +
            invert(947, 1, (index / 896809u) % 947u, perm)) * float(0x1.fffffcp-1 / 849278123u);
    }

    float halton953(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(953, 1, index % 953u, perm) * 908209u +
            invert(953, 1, (index / 953u) % 953u, perm) * 953u +
            invert(953, 1, (index / 908209u) % 953u, perm)) * float(0x1.fffffcp-1 / 865523177u);
    }

    float halton967(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(967, 1, index % 967u, perm) * 935089u +
            invert(967, 1, (index / 967u) % 967u, perm) * 967u +
            invert(967, 1, (index / 935089u) % 967u, perm)) * float(0x1.fffffcp-1 / 904231063u);
    }

    float halton971(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(971, 1, index % 971u, perm) * 942841u +
            invert(971, 1, (index / 971u) % 971u, perm) * 971u +
            invert(971, 1, (index / 942841u) % 971u, perm)) * float(0x1.fffffcp-1 / 915498611u);
    }

    float halton977(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(977, 1, index % 977u, perm) * 954529u +
            invert(977, 1, (index / 977u) % 977u, perm) * 977u +
            invert(977, 1, (index / 954529u) % 977u, perm)) * float(0x1.fffffcp-1 / 932574833u);
    }

    float halton983(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(983, 1, index % 983u, perm) * 966289u +
            invert(983, 1, (index / 983u) % 983u, perm) * 983u +
            invert(983, 1, (index / 966289u) % 983u, perm)) * float(0x1.fffffcp-1 / 949862087u);
    }

    float halton991(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(991, 1, index % 991u, perm) * 982081u +
            invert(991, 1, (index / 991u) % 991u, perm) * 991u +
            invert(991, 1, (index / 982081u) % 991u, perm)) * float(0x1.fffffcp-1 / 973242271u);
    }

    float halton997(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(997, 1, index % 997u, perm) * 994009u +
            invert(997, 1, (index / 997u) % 997u, perm) * 997u +
            invert(997, 1, (index / 994009u) % 997u, perm)) * float(0x1.fffffcp-1 / 991026973u);
    }

    float halton1009(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1009, 1, index % 1009u, perm) * 1018081u +
            invert(1009, 1, (index / 1009u) % 1009u, perm) * 1009u +
            invert(1009, 1, (index / 1018081u) % 1009u, perm)) * float(0x1.fffffcp-1 / 1027243729u);
    }

    float halton1013(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1013, 1, index % 1013u, perm) * 1026169u +
            invert(1013, 1, (index / 1013u) % 1013u, perm) * 1013u +
            invert(1013, 1, (index / 1026169u) % 1013u, perm)) * float(0x1.fffffcp-1 / 1039509197u);
    }

    float halton1019(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1019, 1, index % 1019u, perm) * 1038361u +
            invert(1019, 1, (index / 1019u) % 1019u, perm) * 1019u +
            invert(1019, 1, (index / 1038361u) % 1019u, perm)) * float(0x1.fffffcp-1 / 1058089859u);
    }

    float halton1021(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1021, 1, index % 1021u, perm) * 1042441u +
            invert(1021, 1, (index / 1021u) % 1021u, perm) * 1021u +
            invert(1021, 1, (index / 1042441u) % 1021u, perm)) * float(0x1.fffffcp-1 / 1064332261u);
    }

    float halton1031(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1031, 1, index % 1031u, perm) * 1062961u +
            invert(1031, 1, (index / 1031u) % 1031u, perm) * 1031u +
            invert(1031, 1, (index / 1062961u) % 1031u, perm)) * float(0x1.fffffcp-1 / 1095912791u);
    }

    float halton1033(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1033, 1, index % 1033u, perm) * 1067089u +
            invert(1033, 1, (index / 1033u) % 1033u, perm) * 1033u +
            invert(1033, 1, (index / 1067089u) % 1033u, perm)) * float(0x1.fffffcp-1 / 1102302937u);
    }

    float halton1039(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1039, 1, index % 1039u, perm) * 1079521u +
            invert(1039, 1, (index / 1039u) % 1039u, perm) * 1039u +
            invert(1039, 1, (index / 1079521u) % 1039u, perm)) * float(0x1.fffffcp-1 / 1121622319u);
    }

    float halton1049(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1049, 1, index % 1049u, perm) * 1100401u +
            invert(1049, 1, (index / 1049u) % 1049u, perm) * 1049u +
            invert(1049, 1, (index / 1100401u) % 1049u, perm)) * float(0x1.fffffcp-1 / 1154320649u);
    }

    float halton1051(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1051, 1, index % 1051u, perm) * 1104601u +
            invert(1051, 1, (index / 1051u) % 1051u, perm) * 1051u +
            invert(1051, 1, (index / 1104601u) % 1051u, perm)) * float(0x1.fffffcp-1 / 1160935651u);
    }

    float halton1061(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1061, 1, index % 1061u, perm) * 1125721u +
            invert(1061, 1, (index / 1061u) % 1061u, perm) * 1061u +
            invert(1061, 1, (index / 1125721u) % 1061u, perm)) * float(0x1.fffffcp-1 / 1194389981u);
    }

    float halton1063(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1063, 1, index % 1063u, perm) * 1129969u +
            invert(1063, 1, (index / 1063u) % 1063u, perm) * 1063u +
            invert(1063, 1, (index / 1129969u) % 1063u, perm)) * float(0x1.fffffcp-1 / 1201157047u);
    }

    float halton1069(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1069, 1, index % 1069u, perm) * 1142761u +
            invert(1069, 1, (index / 1069u) % 1069u, perm) * 1069u +
            invert(1069, 1, (index / 1142761u) % 1069u, perm)) * float(0x1.fffffcp-1 / 1221611509u);
    }

    float halton1087(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1087, 1, index % 1087u, perm) * 1181569u +
            invert(1087, 1, (index / 1087u) % 1087u, perm) * 1087u +
            invert(1087, 1, (index / 1181569u) % 1087u, perm)) * float(0x1.fffffcp-1 / 1284365503u);
    }

    float halton1091(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1091, 1, index % 1091u, perm) * 1190281u +
            invert(1091, 1, (index / 1091u) % 1091u, perm) * 1091u +
            invert(1091, 1, (index / 1190281u) % 1091u, perm)) * float(0x1.fffffcp-1 / 1298596571u);
    }

    float halton1093(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1093, 1, index % 1093u, perm) * 1194649u +
            invert(1093, 1, (index / 1093u) % 1093u, perm) * 1093u +
            invert(1093, 1, (index / 1194649u) % 1093u, perm)) * float(0x1.fffffcp-1 / 1305751357u);
    }

    float halton1097(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1097, 1, index % 1097u, perm) * 1203409u +
            invert(1097, 1, (index / 1097u) % 1097u, perm) * 1097u +
            invert(1097, 1, (index / 1203409u) % 1097u, perm)) * float(0x1.fffffcp-1 / 1320139673u);
    }

    float halton1103(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1103, 1, index % 1103u, perm) * 1216609u +
            invert(1103, 1, (index / 1103u) % 1103u, perm) * 1103u +
            invert(1103, 1, (index / 1216609u) % 1103u, perm)) * float(0x1.fffffcp-1 / 1341919727u);
    }

    float halton1109(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1109, 1, index % 1109u, perm) * 1229881u +
            invert(1109, 1, (index / 1109u) % 1109u, perm) * 1109u +
            invert(1109, 1, (index / 1229881u) % 1109u, perm)) * float(0x1.fffffcp-1 / 1363938029u);
    }

    float halton1117(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1117, 1, index % 1117u, perm) * 1247689u +
            invert(1117, 1, (index / 1117u) % 1117u, perm) * 1117u +
            invert(1117, 1, (index / 1247689u) % 1117u, perm)) * float(0x1.fffffcp-1 / 1393668613u);
    }

    float halton1123(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1123, 1, index % 1123u, perm) * 1261129u +
            invert(1123, 1, (index / 1123u) % 1123u, perm) * 1123u +
            invert(1123, 1, (index / 1261129u) % 1123u, perm)) * float(0x1.fffffcp-1 / 1416247867u);
    }

    float halton1129(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1129, 1, index % 1129u, perm) * 1274641u +
            invert(1129, 1, (index / 1129u) % 1129u, perm) * 1129u +
            invert(1129, 1, (index / 1274641u) % 1129u, perm)) * float(0x1.fffffcp-1 / 1439069689u);
    }

    float halton1151(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1151, 1, index % 1151u, perm) * 1324801u +
            invert(1151, 1, (index / 1151u) % 1151u, perm) * 1151u +
            invert(1151, 1, (index / 1324801u) % 1151u, perm)) * float(0x1.fffffcp-1 / 1524845951u);
    }

    float halton1153(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1153, 1, index % 1153u, perm) * 1329409u +
            invert(1153, 1, (index / 1153u) % 1153u, perm) * 1153u +
            invert(1153, 1, (index / 1329409u) % 1153u, perm)) * float(0x1.fffffcp-1 / 1532808577u);
    }

    float halton1163(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1163, 1, index % 1163u, perm) * 1352569u +
            invert(1163, 1, (index / 1163u) % 1163u, perm) * 1163u +
            invert(1163, 1, (index / 1352569u) % 1163u, perm)) * float(0x1.fffffcp-1 / 1573037747u);
    }

    float halton1171(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1171, 1, index % 1171u, perm) * 1371241u +
            invert(1171, 1, (index / 1171u) % 1171u, perm) * 1171u +
            invert(1171, 1, (index / 1371241u) % 1171u, perm)) * float(0x1.fffffcp-1 / 1605723211u);
    }

    float halton1181(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1181, 1, index % 1181u, perm) * 1394761u +
            invert(1181, 1, (index / 1181u) % 1181u, perm) * 1181u +
            invert(1181, 1, (index / 1394761u) % 1181u, perm)) * float(0x1.fffffcp-1 / 1647212741u);
    }

    float halton1187(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1187, 1, index % 1187u, perm) * 1408969u +
            invert(1187, 1, (index / 1187u) % 1187u, perm) * 1187u +
            invert(1187, 1, (index / 1408969u) % 1187u, perm)) * float(0x1.fffffcp-1 / 1672446203u);
    }

    float halton1193(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1193, 1, index % 1193u, perm) * 1423249u +
            invert(1193, 1, (index / 1193u) % 1193u, perm) * 1193u +
            invert(1193, 1, (index / 1423249u) % 1193u, perm)) * float(0x1.fffffcp-1 / 1697936057u);
    }

    float halton1201(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1201, 1, index % 1201u, perm) * 1442401u +
            invert(1201, 1, (index / 1201u) % 1201u, perm) * 1201u +
            invert(1201, 1, (index / 1442401u) % 1201u, perm)) * float(0x1.fffffcp-1 / 1732323601u);
    }

    float halton1213(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1213, 1, index % 1213u, perm) * 1471369u +
            invert(1213, 1, (index / 1213u) % 1213u, perm) * 1213u +
            invert(1213, 1, (index / 1471369u) % 1213u, perm)) * float(0x1.fffffcp-1 / 1784770597u);
    }

    float halton1217(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1217, 1, index % 1217u, perm) * 1481089u +
            invert(1217, 1, (index / 1217u) % 1217u, perm) * 1217u +
            invert(1217, 1, (index / 1481089u) % 1217u, perm)) * float(0x1.fffffcp-1 / 1802485313u);
    }

    float halton1223(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1223, 1, index % 1223u, perm) * 1495729u +
            invert(1223, 1, (index / 1223u) % 1223u, perm) * 1223u +
            invert(1223, 1, (index / 1495729u) % 1223u, perm)) * float(0x1.fffffcp-1 / 1829276567u);
    }

    float halton1229(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1229, 1, index % 1229u, perm) * 1510441u +
            invert(1229, 1, (index / 1229u) % 1229u, perm) * 1229u +
            invert(1229, 1, (index / 1510441u) % 1229u, perm)) * float(0x1.fffffcp-1 / 1856331989u);
    }

    float halton1231(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1231, 1, index % 1231u, perm) * 1515361u +
            invert(1231, 1, (index / 1231u) % 1231u, perm) * 1231u +
            invert(1231, 1, (index / 1515361u) % 1231u, perm)) * float(0x1.fffffcp-1 / 1865409391u);
    }

    float halton1237(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1237, 1, index % 1237u, perm) * 1530169u +
            invert(1237, 1, (index / 1237u) % 1237u, perm) * 1237u +
            invert(1237, 1, (index / 1530169u) % 1237u, perm)) * float(0x1.fffffcp-1 / 1892819053u);
    }

    float halton1249(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1249, 1, index % 1249u, perm) * 1560001u +
            invert(1249, 1, (index / 1249u) % 1249u, perm) * 1249u +
            invert(1249, 1, (index / 1560001u) % 1249u, perm)) * float(0x1.fffffcp-1 / 1948441249u);
    }

    float halton1259(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1259, 1, index % 1259u, perm) * 1585081u +
            invert(1259, 1, (index / 1259u) % 1259u, perm) * 1259u +
            invert(1259, 1, (index / 1585081u) % 1259u, perm)) * float(0x1.fffffcp-1 / 1995616979u);
    }

    float halton1277(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1277, 1, index % 1277u, perm) * 1630729u +
            invert(1277, 1, (index / 1277u) % 1277u, perm) * 1277u +
            invert(1277, 1, (index / 1630729u) % 1277u, perm)) * float(0x1.fffffcp-1 / 2082440933u);
    }

    float halton1279(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1279, 1, index % 1279u, perm) * 1635841u +
            invert(1279, 1, (index / 1279u) % 1279u, perm) * 1279u +
            invert(1279, 1, (index / 1635841u) % 1279u, perm)) * float(0x1.fffffcp-1 / 2092240639u);
    }

    float halton1283(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1283, 1, index % 1283u, perm) * 1646089u +
            invert(1283, 1, (index / 1283u) % 1283u, perm) * 1283u +
            invert(1283, 1, (index / 1646089u) % 1283u, perm)) * float(0x1.fffffcp-1 / 2111932187u);
    }

    float halton1289(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1289, 1, index % 1289u, perm) * 1661521u +
            invert(1289, 1, (index / 1289u) % 1289u, perm) * 1289u +
            invert(1289, 1, (index / 1661521u) % 1289u, perm)) * float(0x1.fffffcp-1 / 2141700569u);
    }

    float halton1291(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1291, 1, index % 1291u, perm) * 1666681u +
            invert(1291, 1, (index / 1291u) % 1291u, perm) * 1291u +
            invert(1291, 1, (index / 1666681u) % 1291u, perm)) * float(0x1.fffffcp-1 / 2151685171u);
    }

    float halton1297(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1297, 1, index % 1297u, perm) * 1682209u +
            invert(1297, 1, (index / 1297u) % 1297u, perm) * 1297u +
            invert(1297, 1, (index / 1682209u) % 1297u, perm)) * float(0x1.fffffcp-1 / 2181825073u);
    }

    float halton1301(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1301, 1, index % 1301u, perm) * 1692601u +
            invert(1301, 1, (index / 1301u) % 1301u, perm) * 1301u +
            invert(1301, 1, (index / 1692601u) % 1301u, perm)) * float(0x1.fffffcp-1 / 2202073901u);
    }

    float halton1303(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1303, 1, index % 1303u, perm) * 1697809u +
            invert(1303, 1, (index / 1303u) % 1303u, perm) * 1303u +
            invert(1303, 1, (index / 1697809u) % 1303u, perm)) * float(0x1.fffffcp-1 / 2212245127u);
    }

    float halton1307(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1307, 1, index % 1307u, perm) * 1708249u +
            invert(1307, 1, (index / 1307u) % 1307u, perm) * 1307u +
            invert(1307, 1, (index / 1708249u) % 1307u, perm)) * float(0x1.fffffcp-1 / 2232681443u);
    }

    float halton1319(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1319, 1, index % 1319u, perm) * 1739761u +
            invert(1319, 1, (index / 1319u) % 1319u, perm) * 1319u +
            invert(1319, 1, (index / 1739761u) % 1319u, perm)) * float(0x1.fffffcp-1 / 2294744759u);
    }

    float halton1321(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1321, 1, index % 1321u, perm) * 1745041u +
            invert(1321, 1, (index / 1321u) % 1321u, perm) * 1321u +
            invert(1321, 1, (index / 1745041u) % 1321u, perm)) * float(0x1.fffffcp-1 / 2305199161u);
    }

    float halton1327(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1327, 1, index % 1327u, perm) * 1760929u +
            invert(1327, 1, (index / 1327u) % 1327u, perm) * 1327u +
            invert(1327, 1, (index / 1760929u) % 1327u, perm)) * float(0x1.fffffcp-1 / 2336752783u);
    }

    float halton1361(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1361, 1, index % 1361u, perm) * 1852321u +
            invert(1361, 1, (index / 1361u) % 1361u, perm) * 1361u +
            invert(1361, 1, (index / 1852321u) % 1361u, perm)) * float(0x1.fffffcp-1 / 2521008881u);
    }

    float halton1367(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1367, 1, index % 1367u, perm) * 1868689u +
            invert(1367, 1, (index / 1367u) % 1367u, perm) * 1367u +
            invert(1367, 1, (index / 1868689u) % 1367u, perm)) * float(0x1.fffffcp-1 / 2554497863u);
    }

    float halton1373(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1373, 1, index % 1373u, perm) * 1885129u +
            invert(1373, 1, (index / 1373u) % 1373u, perm) * 1373u +
            invert(1373, 1, (index / 1885129u) % 1373u, perm)) * float(0x1.fffffcp-1 / 2588282117u);
    }

    float halton1381(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1381, 1, index % 1381u, perm) * 1907161u +
            invert(1381, 1, (index / 1381u) % 1381u, perm) * 1381u +
            invert(1381, 1, (index / 1907161u) % 1381u, perm)) * float(0x1.fffffcp-1 / 2633789341u);
    }

    float halton1399(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1399, 1, index % 1399u, perm) * 1957201u +
            invert(1399, 1, (index / 1399u) % 1399u, perm) * 1399u +
            invert(1399, 1, (index / 1957201u) % 1399u, perm)) * float(0x1.fffffcp-1 / 2738124199u);
    }

    float halton1409(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1409, 1, index % 1409u, perm) * 1985281u +
            invert(1409, 1, (index / 1409u) % 1409u, perm) * 1409u +
            invert(1409, 1, (index / 1985281u) % 1409u, perm)) * float(0x1.fffffcp-1 / 2797260929u);
    }

    float halton1423(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1423, 1, index % 1423u, perm) * 2024929u +
            invert(1423, 1, (index / 1423u) % 1423u, perm) * 1423u +
            invert(1423, 1, (index / 2024929u) % 1423u, perm)) * float(0x1.fffffcp-1 / 2881473967u);
    }

    float halton1427(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1427, 1, index % 1427u, perm) * 2036329u +
            invert(1427, 1, (index / 1427u) % 1427u, perm) * 1427u +
            invert(1427, 1, (index / 2036329u) % 1427u, perm)) * float(0x1.fffffcp-1 / 2905841483u);
    }

    float halton1429(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1429, 1, index % 1429u, perm) * 2042041u +
            invert(1429, 1, (index / 1429u) % 1429u, perm) * 1429u +
            invert(1429, 1, (index / 2042041u) % 1429u, perm)) * float(0x1.fffffcp-1 / 2918076589u);
    }

    float halton1433(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1433, 1, index % 1433u, perm) * 2053489u +
            invert(1433, 1, (index / 1433u) % 1433u, perm) * 1433u +
            invert(1433, 1, (index / 2053489u) % 1433u, perm)) * float(0x1.fffffcp-1 / 2942649737u);
    }

    float halton1439(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1439, 1, index % 1439u, perm) * 2070721u +
            invert(1439, 1, (index / 1439u) % 1439u, perm) * 1439u +
            invert(1439, 1, (index / 2070721u) % 1439u, perm)) * float(0x1.fffffcp-1 / 2979767519u);
    }

    float halton1447(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1447, 1, index % 1447u, perm) * 2093809u +
            invert(1447, 1, (index / 1447u) % 1447u, perm) * 1447u +
            invert(1447, 1, (index / 2093809u) % 1447u, perm)) * float(0x1.fffffcp-1 / 3029741623u);
    }

    float halton1451(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1451, 1, index % 1451u, perm) * 2105401u +
            invert(1451, 1, (index / 1451u) % 1451u, perm) * 1451u +
            invert(1451, 1, (index / 2105401u) % 1451u, perm)) * float(0x1.fffffcp-1 / 3054936851u);
    }

    float halton1453(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1453, 1, index % 1453u, perm) * 2111209u +
            invert(1453, 1, (index / 1453u) % 1453u, perm) * 1453u +
            invert(1453, 1, (index / 2111209u) % 1453u, perm)) * float(0x1.fffffcp-1 / 3067586677u);
    }

    float halton1459(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1459, 1, index % 1459u, perm) * 2128681u +
            invert(1459, 1, (index / 1459u) % 1459u, perm) * 1459u +
            invert(1459, 1, (index / 2128681u) % 1459u, perm)) * float(0x1.fffffcp-1 / 3105745579u);
    }

    float halton1471(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1471, 1, index % 1471u, perm) * 2163841u +
            invert(1471, 1, (index / 1471u) % 1471u, perm) * 1471u +
            invert(1471, 1, (index / 2163841u) % 1471u, perm)) * float(0x1.fffffcp-1 / 3183010111u);
    }

    float halton1481(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1481, 1, index % 1481u, perm) * 2193361u +
            invert(1481, 1, (index / 1481u) % 1481u, perm) * 1481u +
            invert(1481, 1, (index / 2193361u) % 1481u, perm)) * float(0x1.fffffcp-1 / 3248367641u);
    }

    float halton1483(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1483, 1, index % 1483u, perm) * 2199289u +
            invert(1483, 1, (index / 1483u) % 1483u, perm) * 1483u +
            invert(1483, 1, (index / 2199289u) % 1483u, perm)) * float(0x1.fffffcp-1 / 3261545587u);
    }

    float halton1487(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1487, 1, index % 1487u, perm) * 2211169u +
            invert(1487, 1, (index / 1487u) % 1487u, perm) * 1487u +
            invert(1487, 1, (index / 2211169u) % 1487u, perm)) * float(0x1.fffffcp-1 / 3288008303u);
    }

    float halton1489(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1489, 1, index % 1489u, perm) * 2217121u +
            invert(1489, 1, (index / 1489u) % 1489u, perm) * 1489u +
            invert(1489, 1, (index / 2217121u) % 1489u, perm)) * float(0x1.fffffcp-1 / 3301293169u);
    }

    float halton1493(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1493, 1, index % 1493u, perm) * 2229049u +
            invert(1493, 1, (index / 1493u) % 1493u, perm) * 1493u +
            invert(1493, 1, (index / 2229049u) % 1493u, perm)) * float(0x1.fffffcp-1 / 3327970157u);
    }

    float halton1499(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1499, 1, index % 1499u, perm) * 2247001u +
            invert(1499, 1, (index / 1499u) % 1499u, perm) * 1499u +
            invert(1499, 1, (index / 2247001u) % 1499u, perm)) * float(0x1.fffffcp-1 / 3368254499u);
    }

    float halton1511(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1511, 1, index % 1511u, perm) * 2283121u +
            invert(1511, 1, (index / 1511u) % 1511u, perm) * 1511u +
            invert(1511, 1, (index / 2283121u) % 1511u, perm)) * float(0x1.fffffcp-1 / 3449795831u);
    }

    float halton1523(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1523, 1, index % 1523u, perm) * 2319529u +
            invert(1523, 1, (index / 1523u) % 1523u, perm) * 1523u +
            invert(1523, 1, (index / 2319529u) % 1523u, perm)) * float(0x1.fffffcp-1 / 3532642667u);
    }

    float halton1531(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1531, 1, index % 1531u, perm) * 2343961u +
            invert(1531, 1, (index / 1531u) % 1531u, perm) * 1531u +
            invert(1531, 1, (index / 2343961u) % 1531u, perm)) * float(0x1.fffffcp-1 / 3588604291u);
    }

    float halton1543(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1543, 1, index % 1543u, perm) * 2380849u +
            invert(1543, 1, (index / 1543u) % 1543u, perm) * 1543u +
            invert(1543, 1, (index / 2380849u) % 1543u, perm)) * float(0x1.fffffcp-1 / 3673650007u);
    }

    float halton1549(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1549, 1, index % 1549u, perm) * 2399401u +
            invert(1549, 1, (index / 1549u) % 1549u, perm) * 1549u +
            invert(1549, 1, (index / 2399401u) % 1549u, perm)) * float(0x1.fffffcp-1 / 3716672149u);
    }

    float halton1553(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1553, 1, index % 1553u, perm) * 2411809u +
            invert(1553, 1, (index / 1553u) % 1553u, perm) * 1553u +
            invert(1553, 1, (index / 2411809u) % 1553u, perm)) * float(0x1.fffffcp-1 / 3745539377u);
    }

    float halton1559(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1559, 1, index % 1559u, perm) * 2430481u +
            invert(1559, 1, (index / 1559u) % 1559u, perm) * 1559u +
            invert(1559, 1, (index / 2430481u) % 1559u, perm)) * float(0x1.fffffcp-1 / 3789119879u);
    }

    float halton1567(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1567, 1, index % 1567u, perm) * 2455489u +
            invert(1567, 1, (index / 1567u) % 1567u, perm) * 1567u +
            invert(1567, 1, (index / 2455489u) % 1567u, perm)) * float(0x1.fffffcp-1 / 3847751263u);
    }

    float halton1571(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1571, 1, index % 1571u, perm) * 2468041u +
            invert(1571, 1, (index / 1571u) % 1571u, perm) * 1571u +
            invert(1571, 1, (index / 2468041u) % 1571u, perm)) * float(0x1.fffffcp-1 / 3877292411u);
    }

    float halton1579(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1579, 1, index % 1579u, perm) * 2493241u +
            invert(1579, 1, (index / 1579u) % 1579u, perm) * 1579u +
            invert(1579, 1, (index / 2493241u) % 1579u, perm)) * float(0x1.fffffcp-1 / 3936827539u);
    }

    float halton1583(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1583, 1, index % 1583u, perm) * 2505889u +
            invert(1583, 1, (index / 1583u) % 1583u, perm) * 1583u +
            invert(1583, 1, (index / 2505889u) % 1583u, perm)) * float(0x1.fffffcp-1 / 3966822287u);
    }

    float halton1597(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1597, 1, index % 1597u, perm) * 2550409u +
            invert(1597, 1, (index / 1597u) % 1597u, perm) * 1597u +
            invert(1597, 1, (index / 2550409u) % 1597u, perm)) * float(0x1.fffffcp-1 / 4073003173u);
    }

    float halton1601(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1601, 1, index % 1601u, perm) * 2563201u +
            invert(1601, 1, (index / 1601u) % 1601u, perm) * 1601u +
            invert(1601, 1, (index / 2563201u) % 1601u, perm)) * float(0x1.fffffcp-1 / 4103684801u);
    }

    float halton1607(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1607, 1, index % 1607u, perm) * 2582449u +
            invert(1607, 1, (index / 1607u) % 1607u, perm) * 1607u +
            invert(1607, 1, (index / 2582449u) % 1607u, perm)) * float(0x1.fffffcp-1 / 4149995543u);
    }

    float halton1609(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1609, 1, index % 1609u, perm) * 2588881u +
            invert(1609, 1, (index / 1609u) % 1609u, perm) * 1609u +
            invert(1609, 1, (index / 2588881u) % 1609u, perm)) * float(0x1.fffffcp-1 / 4165509529u);
    }

    float halton1613(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1613, 1, index % 1613u, perm) * 2601769u +
            invert(1613, 1, (index / 1613u) % 1613u, perm) * 1613u +
            invert(1613, 1, (index / 2601769u) % 1613u, perm)) * float(0x1.fffffcp-1 / 4196653397u);
    }

    float halton1619(const unsigned index) const
    {
        unsigned int perm = rng->nextUInt();

        return (
            invert(1619, 1, index % 1619u, perm) * 2621161u +
            invert(1619, 1, (index / 1619u) % 1619u, perm) * 1619u +
            invert(1619, 1, (index / 2621161u) % 1619u, perm)) * float(0x1.fffffcp-1 / 4243659659u);
    }
};

}
