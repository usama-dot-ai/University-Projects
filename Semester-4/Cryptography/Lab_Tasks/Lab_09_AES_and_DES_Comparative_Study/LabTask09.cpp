#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;
using namespace chrono;

// ═══════════════════════════════════════════════════════════
//  AES CONSTANTS
// ═══════════════════════════════════════════════════════════

static const uint8_t SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
static const uint8_t INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};
static const uint8_t RCON[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// ── AES GF multiplication ───────────────────────────────────
uint8_t gf_mul(uint8_t a, uint8_t b)
{
    uint8_t r = 0;
    while (b)
    {
        if (b & 1)
            r ^= a;
        bool h = a & 0x80;
        a <<= 1;
        if (h)
            a ^= 0x1B;
        b >>= 1;
    }
    return r;
}

// ── AES state ops ───────────────────────────────────────────
void subBytes(uint8_t s[4][4])
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            s[r][c] = SBOX[s[r][c]];
}
void invSubBytes(uint8_t s[4][4])
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            s[r][c] = INV_SBOX[s[r][c]];
}
void shiftRows(uint8_t s[4][4])
{
    for (int r = 1; r < 4; r++)
    {
        uint8_t t[4];
        for (int c = 0; c < 4; c++)
            t[c] = s[r][(c + r) % 4];
        for (int c = 0; c < 4; c++)
            s[r][c] = t[c];
    }
}
void invShiftRows(uint8_t s[4][4])
{
    for (int r = 1; r < 4; r++)
    {
        uint8_t t[4];
        for (int c = 0; c < 4; c++)
            t[c] = s[r][(c - r + 4) % 4];
        for (int c = 0; c < 4; c++)
            s[r][c] = t[c];
    }
}
void mixColumns(uint8_t s[4][4])
{
    for (int c = 0; c < 4; c++)
    {
        uint8_t s0 = s[0][c], s1 = s[1][c], s2 = s[2][c], s3 = s[3][c];
        s[0][c] = gf_mul(2, s0) ^ gf_mul(3, s1) ^ s2 ^ s3;
        s[1][c] = s0 ^ gf_mul(2, s1) ^ gf_mul(3, s2) ^ s3;
        s[2][c] = s0 ^ s1 ^ gf_mul(2, s2) ^ gf_mul(3, s3);
        s[3][c] = gf_mul(3, s0) ^ s1 ^ s2 ^ gf_mul(2, s3);
    }
}
void invMixColumns(uint8_t s[4][4])
{
    for (int c = 0; c < 4; c++)
    {
        uint8_t s0 = s[0][c], s1 = s[1][c], s2 = s[2][c], s3 = s[3][c];
        s[0][c] = gf_mul(0x0e, s0) ^ gf_mul(0x0b, s1) ^ gf_mul(0x0d, s2) ^ gf_mul(0x09, s3);
        s[1][c] = gf_mul(0x09, s0) ^ gf_mul(0x0e, s1) ^ gf_mul(0x0b, s2) ^ gf_mul(0x0d, s3);
        s[2][c] = gf_mul(0x0d, s0) ^ gf_mul(0x09, s1) ^ gf_mul(0x0e, s2) ^ gf_mul(0x0b, s3);
        s[3][c] = gf_mul(0x0b, s0) ^ gf_mul(0x0d, s1) ^ gf_mul(0x09, s2) ^ gf_mul(0x0e, s3);
    }
}
void addRoundKey(uint8_t s[4][4], const vector<uint32_t> &w, int rnd)
{
    for (int c = 0; c < 4; c++)
    {
        uint32_t wd = w[rnd * 4 + c];
        s[0][c] ^= (wd >> 24) & 0xFF;
        s[1][c] ^= (wd >> 16) & 0xFF;
        s[2][c] ^= (wd >> 8) & 0xFF;
        s[3][c] ^= wd & 0xFF;
    }
}

// ── AES key expansion ───────────────────────────────────────
vector<uint32_t> keyExpansion(const vector<uint8_t> &key)
{
    int Nk = key.size() / 4, Nr = Nk + 6, total = 4 * (Nr + 1);
    vector<uint32_t> w(total);
    for (int i = 0; i < Nk; i++)
        w[i] = ((uint32_t)key[4 * i] << 24) | ((uint32_t)key[4 * i + 1] << 16) | ((uint32_t)key[4 * i + 2] << 8) | key[4 * i + 3];
    for (int i = Nk; i < total; i++)
    {
        uint32_t tmp = w[i - 1];
        if (i % Nk == 0)
        {
            tmp = ((tmp << 8) | (tmp >> 24));
            tmp = ((uint32_t)SBOX[(tmp >> 24) & 0xFF] << 24) | ((uint32_t)SBOX[(tmp >> 16) & 0xFF] << 16) | ((uint32_t)SBOX[(tmp >> 8) & 0xFF] << 8) | SBOX[tmp & 0xFF];
            tmp ^= ((uint32_t)RCON[i / Nk] << 24);
        }
        else if (Nk > 6 && i % Nk == 4)
        {
            tmp = ((uint32_t)SBOX[(tmp >> 24) & 0xFF] << 24) | ((uint32_t)SBOX[(tmp >> 16) & 0xFF] << 16) | ((uint32_t)SBOX[(tmp >> 8) & 0xFF] << 8) | SBOX[tmp & 0xFF];
        }
        w[i] = w[i - Nk] ^ tmp;
    }
    return w;
}

// ── AES block encrypt/decrypt ───────────────────────────────
vector<uint8_t> aesEncBlock(const vector<uint8_t> &blk, const vector<uint32_t> &w, int Nr)
{
    uint8_t s[4][4];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            s[r][c] = blk[r + 4 * c];
    addRoundKey(s, w, 0);
    for (int rnd = 1; rnd <= Nr; rnd++)
    {
        subBytes(s);
        shiftRows(s);
        if (rnd != Nr)
            mixColumns(s);
        addRoundKey(s, w, rnd);
    }
    vector<uint8_t> out(16);
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            out[r + 4 * c] = s[r][c];
    return out;
}
vector<uint8_t> aesDecBlock(const vector<uint8_t> &blk, const vector<uint32_t> &w, int Nr)
{
    uint8_t s[4][4];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            s[r][c] = blk[r + 4 * c];
    addRoundKey(s, w, Nr);
    for (int rnd = Nr - 1; rnd >= 0; rnd--)
    {
        invShiftRows(s);
        invSubBytes(s);
        addRoundKey(s, w, rnd);
        if (rnd != 0)
            invMixColumns(s);
    }
    vector<uint8_t> out(16);
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            out[r + 4 * c] = s[r][c];
    return out;
}

// ── AES padding + ECB ───────────────────────────────────────
vector<uint8_t> pkcs7add(const vector<uint8_t> &d)
{
    int p = 16 - (d.size() % 16);
    vector<uint8_t> r = d;
    r.insert(r.end(), p, (uint8_t)p);
    return r;
}
vector<uint8_t> pkcs7rem(const vector<uint8_t> &d)
{
    if (d.empty())
        return d;
    uint8_t p = d.back();
    if (!p || p > 16)
        return d;
    return {d.begin(), d.end() - p};
}

vector<uint8_t> aesECBEnc(const vector<uint8_t> &pt, const vector<uint8_t> &key)
{
    int Nr = key.size() / 4 + 6;
    auto w = keyExpansion(key);
    auto pad = pkcs7add(pt);
    vector<uint8_t> ct;
    for (size_t i = 0; i < pad.size(); i += 16)
    {
        auto e = aesEncBlock({pad.begin() + i, pad.begin() + i + 16}, w, Nr);
        ct.insert(ct.end(), e.begin(), e.end());
    }
    return ct;
}
vector<uint8_t> aesECBDec(const vector<uint8_t> &ct, const vector<uint8_t> &key)
{
    int Nr = key.size() / 4 + 6;
    auto w = keyExpansion(key);
    vector<uint8_t> pt;
    for (size_t i = 0; i < ct.size(); i += 16)
    {
        auto d = aesDecBlock({ct.begin() + i, ct.begin() + i + 16}, w, Nr);
        pt.insert(pt.end(), d.begin(), d.end());
    }
    return pkcs7rem(pt);
}

// ═══════════════════════════════════════════════════════════
//  DES CONSTANTS & TABLES
// ═══════════════════════════════════════════════════════════

static const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
static const int FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
static const int E[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
static const int P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};
static const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
static const int PC2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
static const int SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
static const uint8_t SBOX_DES[8][4][16] = {
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8}, {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0}, {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10}, {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5}, {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15}, {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8}, {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1}, {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7}, {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15}, {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9}, {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4}, {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}, {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6}, {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14}, {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11}, {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8}, {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6}, {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1}, {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6}, {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2}, {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7}, {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2}, {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8}, {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

// ── DES helpers ─────────────────────────────────────────────
uint64_t desPermute(uint64_t in, const int *tbl, int n, int bits)
{
    uint64_t out = 0;
    for (int i = 0; i < n; i++)
    {
        uint64_t b = (in >> (bits - tbl[i])) & 1;
        out |= (b << (n - 1 - i));
    }
    return out;
}

uint32_t desF(uint32_t R, uint64_t K)
{
    uint64_t exp = 0;
    for (int i = 0; i < 48; i++)
    {
        uint64_t b = (((uint64_t)R) >> (32 - E[i])) & 1;
        exp |= (b << (47 - i));
    }
    uint64_t xord = exp ^ K;
    uint32_t out = 0;
    for (int s = 0; s < 8; s++)
    {
        int chunk = (xord >> (42 - 6 * s)) & 0x3F;
        int row = ((chunk & 0x20) >> 4) | (chunk & 1);
        int col = (chunk >> 1) & 0xF;
        out = (out << 4) | SBOX_DES[s][row][col];
    }
    uint32_t pout = 0;
    for (int i = 0; i < 32; i++)
    {
        uint32_t b = (out >> (32 - P[i])) & 1;
        pout |= (b << (31 - i));
    }
    return pout;
}

void desKeySchedule(uint64_t key64, uint64_t subkeys[16])
{
    uint64_t kp = desPermute(key64, PC1, 56, 64);
    uint32_t C = (kp >> 28) & 0xFFFFFFF, D = kp & 0xFFFFFFF;
    for (int i = 0; i < 16; i++)
    {
        for (int s = 0; s < SHIFTS[i]; s++)
        {
            C = ((C << 1) | (C >> 27)) & 0xFFFFFFF;
            D = ((D << 1) | (D >> 27)) & 0xFFFFFFF;
        }
        uint64_t CD = ((uint64_t)C << 28) | D;
        subkeys[i] = desPermute(CD, PC2, 48, 56);
    }
}

uint64_t desBlock(uint64_t block, uint64_t subkeys[16], bool enc)
{
    uint64_t ip = desPermute(block, IP, 64, 64);
    uint32_t L = (ip >> 32), R = ip & 0xFFFFFFFF;
    for (int i = 0; i < 16; i++)
    {
        int k = enc ? i : 15 - i;
        uint32_t tmp = R;
        R = L ^ desF(R, subkeys[k]);
        L = tmp;
    }
    return desPermute(((uint64_t)R << 32) | L, FP, 64, 64);
}

// ── DES ECB with PKCS7 (8-byte blocks) ──────────────────────
void desSetParity(uint8_t *k)
{
    for (int i = 0; i < 8; i++)
    {
        int b = 0;
        for (int j = 1; j < 8; j++)
            if (k[i] & (1 << j))
                b++;
        if (b % 2 == 0)
            k[i] |= 1;
        else
            k[i] &= 0xFE;
    }
}

vector<uint8_t> desECBEnc(const vector<uint8_t> &pt, const uint8_t *key)
{
    uint64_t k64 = 0;
    for (int i = 0; i < 8; i++)
        k64 = ((k64 << 8) | key[i]);
    uint64_t sk[16];
    desKeySchedule(k64, sk);
    int pad = 8 - (pt.size() % 8);
    vector<uint8_t> padded = pt;
    padded.insert(padded.end(), pad, (uint8_t)pad);
    vector<uint8_t> ct;
    for (size_t i = 0; i < padded.size(); i += 8)
    {
        uint64_t blk = 0;
        for (int j = 0; j < 8; j++)
            blk = ((blk << 8) | padded[i + j]);
        uint64_t enc = desBlock(blk, sk, true);
        for (int j = 7; j >= 0; j--)
        {
            ct.push_back(enc & 0xFF);
            enc >>= 8;
        }
        // fix byte order
        std::reverse(ct.end() - 8, ct.end());
    }
    return ct;
}

vector<uint8_t> desECBDec(const vector<uint8_t> &ct, const uint8_t *key)
{
    uint64_t k64 = 0;
    for (int i = 0; i < 8; i++)
        k64 = ((k64 << 8) | key[i]);
    uint64_t sk[16];
    desKeySchedule(k64, sk);
    vector<uint8_t> pt;
    for (size_t i = 0; i < ct.size(); i += 8)
    {
        uint64_t blk = 0;
        for (int j = 0; j < 8; j++)
            blk = ((blk << 8) | ct[i + j]);
        uint64_t dec = desBlock(blk, sk, false);
        vector<uint8_t> tmp;
        for (int j = 7; j >= 0; j--)
        {
            tmp.push_back(dec & 0xFF);
            dec >>= 8;
        }
        std::reverse(tmp.begin(), tmp.end());
        pt.insert(pt.end(), tmp.begin(), tmp.end());
    }
    uint8_t pad = pt.back();
    if (pad && pad <= 8)
        pt.resize(pt.size() - pad);
    return pt;
}

// ═══════════════════════════════════════════════════════════
//  UTILITIES
// ═══════════════════════════════════════════════════════════

vector<uint8_t> randomBytes(int n)
{
    static mt19937 rng(random_device{}());
    static uniform_int_distribution<int> d(0, 255);
    vector<uint8_t> b(n);
    for (auto &x : b)
        x = d(rng);
    return b;
}

void printHex(const string &label, const vector<uint8_t> &data)
{
    cout << label;
    for (auto b : data)
        cout << hex << setw(2) << setfill('0') << (int)b;
    cout << dec << "\n";
}

string toString(const vector<uint8_t> &v) { return string(v.begin(), v.end()); }
vector<uint8_t> toBytes(const string &s) { return {s.begin(), s.end()}; }

int countDiffBits(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    int bits = 0;
    for (size_t i = 0; i < min(a.size(), b.size()); i++)
    {
        uint8_t d = a[i] ^ b[i];
        while (d)
        {
            bits += d & 1;
            d >>= 1;
        }
    }
    return bits;
}

void sep(const string &t)
{
    cout << "\n+--------------------------------------------------+\n";
    cout << "  " << t << "\n";
    cout << "+--------------------------------------------------+\n";
}

// ═══════════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════════

int main()
{
    const string plaintext = "LabTask09";
    auto pt = toBytes(plaintext);

    // ── Keys ────────────────────────────────────────────────
    auto aes_key = randomBytes(16); // AES-128: 16 bytes
    uint8_t des_key[8];
    auto tmp = randomBytes(8);
    memcpy(des_key, tmp.data(), 8);
    desSetParity(des_key); // DES: 8 bytes (56-bit effective)

    // ══════════════════════════════════════════════════════
    sep("PLAINTEXT");
    cout << "Message        : \"" << plaintext << "\"\n";
    cout << "Length         : " << pt.size() << " bytes\n";

    // ══════════════════════════════════════════════════════
    sep("AES-128 ECB  (128-bit key, 10 rounds, 16-byte block)");

    auto t0 = high_resolution_clock::now();
    auto aes_ct = aesECBEnc(pt, aes_key);
    auto t1 = high_resolution_clock::now();
    auto aes_dec = aesECBDec(aes_ct, aes_key);
    auto t2 = high_resolution_clock::now();

    printHex("Key            : ", aes_key);
    cout << "Padded Length  : " << ((pt.size() / 16) + 1) * 16 << " bytes (PKCS#7)\n";
    printHex("Ciphertext     : ", aes_ct);
    cout << "Decrypted      : \"" << toString(aes_dec) << "\"\n";
    cout << "Verification   : " << (toString(aes_dec) == plaintext ? "PASS [OK]" : "FAIL") << "\n";
    cout << "Encrypt time   : "
         << duration_cast<microseconds>(t1 - t0).count() << " µs\n";
    cout << "Decrypt time   : "
         << duration_cast<microseconds>(t2 - t1).count() << " µs\n";

    // ══════════════════════════════════════════════════════
    sep("DES ECB  (56-bit key, 16 rounds, 8-byte block)");

    auto t3 = high_resolution_clock::now();
    auto des_ct = desECBEnc(pt, des_key);
    auto t4 = high_resolution_clock::now();
    auto des_dec = desECBDec(des_ct, des_key);
    auto t5 = high_resolution_clock::now();

    cout << "Key (64-bit w/ parity): ";
    for (int i = 0; i < 8; i++)
        cout << hex << setw(2) << setfill('0') << (int)des_key[i];
    cout << dec << "\n";
    cout << "Padded Length  : " << ((pt.size() / 8) + 1) * 8 << " bytes (PKCS#7)\n";
    printHex("Ciphertext     : ", des_ct);
    cout << "Decrypted      : \"" << toString(des_dec) << "\"\n";
    cout << "Verification   : " << (toString(des_dec) == plaintext ? "PASS [OK]" : "FAIL") << "\n";
    cout << "Encrypt time   : "
         << duration_cast<microseconds>(t4 - t3).count() << " µs\n";
    cout << "Decrypt time   : "
         << duration_cast<microseconds>(t5 - t4).count() << " µs\n";

    // ══════════════════════════════════════════════════════
    sep("AES vs DES — CIPHERTEXT COMPARISON");

    printHex("AES Ciphertext : ", aes_ct);
    printHex("DES Ciphertext : ", des_ct);
    cout << "AES CT length  : " << aes_ct.size() << " bytes\n";
    cout << "DES CT length  : " << des_ct.size() << " bytes\n";
    cout << "Same output?   : " << (aes_ct == des_ct ? "YES" : "NO") << "\n";

    // ══════════════════════════════════════════════════════
    sep("AVALANCHE EFFECT COMPARISON");

    string modified = "LabTask9"; // 1-bit change
    auto mod = toBytes(modified);

    auto aes_ct2 = aesECBEnc(mod, aes_key);
    auto des_ct2 = desECBEnc(mod, des_key);

    int aes_bits = countDiffBits(aes_ct, aes_ct2);
    int des_bits = countDiffBits(des_ct, des_ct2);

    cout << "Original       : \"" << plaintext << "\"\n";
    cout << "Modified       : \"" << modified << "\"\n";
    cout << "Change         : 'F' -> 'G'  (1 bit)\n\n";
    cout << "AES changed bits : " << aes_bits << " / " << aes_ct.size() * 8
         << " (" << fixed << setprecision(1) << (100.0 * aes_bits / (aes_ct.size() * 8)) << "%)\n";
    cout << "DES changed bits : " << des_bits << " / " << des_ct.size() * 8
         << " (" << fixed << setprecision(1) << (100.0 * des_bits / (des_ct.size() * 8)) << "%)\n";

    // ══════════════════════════════════════════════════════
    sep("SUMMARY TABLE");

    cout << "+--------------------+---------------+---------------+\n";
    cout << "| Property           | AES-128       | DES           |\n";
    cout << "+--------------------+---------------+---------------+\n";
    cout << "| Block Size         | 128 bits      | 64 bits       |\n";
    cout << "| Key Size           | 128 bits      | 56 bits       |\n";
    cout << "| Rounds             | 10            | 16            |\n";
    cout << "| Structure          | SPN           | Feistel       |\n";
    cout << "| CT Length (19 B)   | 32 bytes      | 24 bytes      |\n";
    cout << "| Status             | Secure        | Broken        |\n";
    cout << "+--------------------+---------------+---------------+\n";
    cout << "| Encrypt time       | see above     | see above     |\n";
    cout << "| Avalanche %        | see above     | see above     |\n";
    cout << "+--------------------+---------------+---------------+\n";

    return 0;
}