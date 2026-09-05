#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <random>
#include <algorithm>
using namespace std;

//  AES CONSTANT
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

static const uint8_t RCON[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

//  GALOIS FIELD MULTIPLICATION
uint8_t gf_mul(uint8_t a, uint8_t b)
{
    uint8_t result = 0;
    while (b)
    {
        if (b & 1)
            result ^= a;
        bool hi = a & 0x80;
        a <<= 1;
        if (hi)
            a ^= 0x1B;
        b >>= 1;
    }
    return result;
}

//  AES STATE OPERATIONS
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

void addRoundKey(uint8_t s[4][4], const vector<uint32_t> &w, int round)
{
    for (int c = 0; c < 4; c++)
    {
        uint32_t word = w[round * 4 + c];
        s[0][c] ^= (word >> 24) & 0xFF;
        s[1][c] ^= (word >> 16) & 0xFF;
        s[2][c] ^= (word >> 8) & 0xFF;
        s[3][c] ^= (word) & 0xFF;
    }
}

//  KEY EXPANSION
vector<uint32_t> keyExpansion(const vector<uint8_t> &key)
{
    int Nk = key.size() / 4, Nr = Nk + 6, total = 4 * (Nr + 1);
    vector<uint32_t> w(total);
    for (int i = 0; i < Nk; i++)
        w[i] = ((uint32_t)key[4 * i] << 24) | ((uint32_t)key[4 * i + 1] << 16) | ((uint32_t)key[4 * i + 2] << 8) | ((uint32_t)key[4 * i + 3]);
    for (int i = Nk; i < total; i++)
    {
        uint32_t tmp = w[i - 1];
        if (i % Nk == 0)
        {
            tmp = ((tmp << 8) | (tmp >> 24));
            tmp = ((uint32_t)SBOX[(tmp >> 24) & 0xFF] << 24) | ((uint32_t)SBOX[(tmp >> 16) & 0xFF] << 16) | ((uint32_t)SBOX[(tmp >> 8) & 0xFF] << 8) | ((uint32_t)SBOX[(tmp) & 0xFF]);
            tmp ^= ((uint32_t)RCON[i / Nk] << 24);
        }
        else if (Nk > 6 && i % Nk == 4)
        {
            tmp = ((uint32_t)SBOX[(tmp >> 24) & 0xFF] << 24) | ((uint32_t)SBOX[(tmp >> 16) & 0xFF] << 16) | ((uint32_t)SBOX[(tmp >> 8) & 0xFF] << 8) | ((uint32_t)SBOX[(tmp) & 0xFF]);
        }
        w[i] = w[i - Nk] ^ tmp;
    }
    return w;
}

//  BLOCK ENCRYPT / DECRYPT
vector<uint8_t> aesEncryptBlock(const vector<uint8_t> &block,
                                const vector<uint32_t> &w, int Nr)
{
    uint8_t s[4][4];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            s[r][c] = block[r + 4 * c];
    addRoundKey(s, w, 0);
    for (int round = 1; round <= Nr; round++)
    {
        subBytes(s);
        shiftRows(s);
        if (round != Nr)
            mixColumns(s);
        addRoundKey(s, w, round);
    }
    vector<uint8_t> out(16);
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            out[r + 4 * c] = s[r][c];
    return out;
}

vector<uint8_t> aesDecryptBlock(const vector<uint8_t> &block,
                                const vector<uint32_t> &w, int Nr)
{
    uint8_t s[4][4];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            s[r][c] = block[r + 4 * c];
    addRoundKey(s, w, Nr);
    for (int round = Nr - 1; round >= 0; round--)
    {
        invShiftRows(s);
        invSubBytes(s);
        addRoundKey(s, w, round);
        if (round != 0)
            invMixColumns(s);
    }
    vector<uint8_t> out(16);
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            out[r + 4 * c] = s[r][c];
    return out;
}

//  PKCS#7 PADDING
vector<uint8_t> addPadding(const vector<uint8_t> &data)
{
    int pad = 16 - (data.size() % 16);
    vector<uint8_t> p = data;
    p.insert(p.end(), pad, (uint8_t)pad);
    return p;
}
vector<uint8_t> removePadding(const vector<uint8_t> &data)
{
    if (data.empty())
        return data;
    uint8_t pad = data.back();
    if (pad == 0 || pad > 16)
        return data;
    return vector<uint8_t>(data.begin(), data.end() - pad);
}

//  ECB MODE
vector<uint8_t> aesECBEncrypt(const vector<uint8_t> &pt,
                              const vector<uint8_t> &key)
{
    int Nr = key.size() / 4 + 6;
    auto w = keyExpansion(key);
    auto padded = addPadding(pt);
    vector<uint8_t> ct;
    for (size_t i = 0; i < padded.size(); i += 16)
    {
        auto enc = aesEncryptBlock({padded.begin() + i, padded.begin() + i + 16}, w, Nr);
        ct.insert(ct.end(), enc.begin(), enc.end());
    }
    return ct;
}
vector<uint8_t> aesECBDecrypt(const vector<uint8_t> &ct,
                              const vector<uint8_t> &key)
{
    int Nr = key.size() / 4 + 6;
    auto w = keyExpansion(key);
    vector<uint8_t> pt;
    for (size_t i = 0; i < ct.size(); i += 16)
    {
        auto dec = aesDecryptBlock({ct.begin() + i, ct.begin() + i + 16}, w, Nr);
        pt.insert(pt.end(), dec.begin(), dec.end());
    }
    return removePadding(pt);
}

//  CBC MODE
vector<uint8_t> aesCBCEncrypt(const vector<uint8_t> &pt,
                              const vector<uint8_t> &key,
                              const vector<uint8_t> &iv)
{
    int Nr = key.size() / 4 + 6;
    auto w = keyExpansion(key);
    auto padded = addPadding(pt);
    vector<uint8_t> ct, prev = iv;
    for (size_t i = 0; i < padded.size(); i += 16)
    {
        vector<uint8_t> block(padded.begin() + i, padded.begin() + i + 16);
        for (int j = 0; j < 16; j++)
            block[j] ^= prev[j];
        auto enc = aesEncryptBlock(block, w, Nr);
        ct.insert(ct.end(), enc.begin(), enc.end());
        prev = enc;
    }
    return ct;
}
vector<uint8_t> aesCBCDecrypt(const vector<uint8_t> &ct,
                              const vector<uint8_t> &key,
                              const vector<uint8_t> &iv)
{
    int Nr = key.size() / 4 + 6;
    auto w = keyExpansion(key);
    vector<uint8_t> pt;
    vector<uint8_t> prev = iv;
    for (size_t i = 0; i < ct.size(); i += 16)
    {
        vector<uint8_t> block(ct.begin() + i, ct.begin() + i + 16);
        auto dec = aesDecryptBlock(block, w, Nr);
        for (int j = 0; j < 16; j++)
            dec[j] ^= prev[j];
        pt.insert(pt.end(), dec.begin(), dec.end());
        prev = block;
    }
    return removePadding(pt);
}

//  UTILITIES
vector<uint8_t> randomBytes(int n)
{
    static mt19937 rng(random_device{}());
    static uniform_int_distribution<int> dist(0, 255);
    vector<uint8_t> buf(n);
    for (auto &b : buf)
        b = dist(rng);
    return buf;
}

void printHex(const string &label, const vector<uint8_t> &data)
{
    cout << label;
    for (auto b : data)
        cout << hex << setw(2) << setfill('0') << (int)b;
    cout << dec << "\n";
}

vector<uint8_t> toBytes(const string &s)
{
    return vector<uint8_t>(s.begin(), s.end());
}
string toString(const vector<uint8_t> &v)
{
    return string(v.begin(), v.end());
}

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

void printSeparator(const string &title)
{
    cout << "\n=============================================\n";
    cout << "  " << title << "\n";
    cout << "=============================================\n";
}

//  MAIN
int main()
{
    const string plaintext = "CRYPTOGRAPHY IS FUN";
    vector<uint8_t> pt = toBytes(plaintext);

    // ── TASK 1: AES-128 ECB ────────────────────────────────
    printSeparator("TASK 1: AES-128 ECB Encryption & Decryption");

    vector<uint8_t> key128 = randomBytes(16);
    vector<uint8_t> ecb_ct = aesECBEncrypt(pt, key128);
    vector<uint8_t> ecb_dec = aesECBDecrypt(ecb_ct, key128);

    printHex("Key (128-bit)  : ", key128);
    cout << "Plaintext      : \"" << plaintext << "\"\n";
    cout << "Plaintext Len  : " << pt.size()
         << " bytes padded to "
         << ((pt.size() / 16) + 1) * 16 << " bytes (PKCS#7)\n";
    printHex("ECB Ciphertext : ", ecb_ct);
    cout << "ECB Decrypted  : \"" << toString(ecb_dec) << "\"\n";
    cout << "Verification   : "
         << (toString(ecb_dec) == plaintext ? "PASS [OK]" : "FAIL") << "\n";

    // ── TASK 2: AES-128 CBC vs ECB ─────────────────────────
    printSeparator("TASK 2: AES-128 CBC Mode & ECB vs CBC Comparison");

    vector<uint8_t> iv128 = randomBytes(16);
    vector<uint8_t> cbc_ct = aesCBCEncrypt(pt, key128, iv128);
    vector<uint8_t> cbc_dec = aesCBCDecrypt(cbc_ct, key128, iv128);

    printHex("IV  (128-bit)  : ", iv128);
    printHex("CBC Ciphertext : ", cbc_ct);
    cout << "CBC Decrypted  : \"" << toString(cbc_dec) << "\"\n";
    cout << "Verification   : "
         << (toString(cbc_dec) == plaintext ? "PASS [OK]" : "FAIL") << "\n";

    cout << "\n--- ECB vs CBC Ciphertext Comparison ---\n";
    printHex("ECB            : ", ecb_ct);
    printHex("CBC            : ", cbc_ct);
    cout << "Match          : "
         << (ecb_ct == cbc_ct ? "YES (unexpected!)" : "NO  (expected)") << "\n";

    // ── TASK 3: AES-192 & AES-256 ──────────────────────────
    printSeparator("TASK 3: Key Size Variation");

    // AES-192
    vector<uint8_t> key192 = randomBytes(24);
    vector<uint8_t> iv192 = randomBytes(16);
    vector<uint8_t> ct192_ecb = aesECBEncrypt(pt, key192);
    vector<uint8_t> ct192_cbc = aesCBCEncrypt(pt, key192, iv192);
    vector<uint8_t> dc192_ecb = aesECBDecrypt(ct192_ecb, key192);
    vector<uint8_t> dc192_cbc = aesCBCDecrypt(ct192_cbc, key192, iv192);

    cout << "\n[ AES-192 ]\n";
    printHex("Key (192-bit)  : ", key192);
    printHex("ECB Ciphertext : ", ct192_ecb);
    cout << "ECB Decrypted  : \"" << toString(dc192_ecb) << "\"\n";
    cout << "ECB Verify     : "
         << (toString(dc192_ecb) == plaintext ? "PASS [OK]" : "FAIL") << "\n";
    printHex("IV  (128-bit)  : ", iv192);
    printHex("CBC Ciphertext : ", ct192_cbc);
    cout << "CBC Decrypted  : \"" << toString(dc192_cbc) << "\"\n";
    cout << "CBC Verify     : "
         << (toString(dc192_cbc) == plaintext ? "PASS [OK]" : "FAIL") << "\n";

    // AES-256
    vector<uint8_t> key256 = randomBytes(32);
    vector<uint8_t> iv256 = randomBytes(16);
    vector<uint8_t> ct256_ecb = aesECBEncrypt(pt, key256);
    vector<uint8_t> ct256_cbc = aesCBCEncrypt(pt, key256, iv256);
    vector<uint8_t> dc256_ecb = aesECBDecrypt(ct256_ecb, key256);
    vector<uint8_t> dc256_cbc = aesCBCDecrypt(ct256_cbc, key256, iv256);

    cout << "\n[ AES-256 ]\n";
    printHex("Key (256-bit)  : ", key256);
    printHex("ECB Ciphertext : ", ct256_ecb);
    cout << "ECB Decrypted  : \"" << toString(dc256_ecb) << "\"\n";
    cout << "ECB Verify     : "
         << (toString(dc256_ecb) == plaintext ? "PASS [OK]" : "FAIL") << "\n";
    printHex("IV  (128-bit)  : ", iv256);
    printHex("CBC Ciphertext : ", ct256_cbc);
    cout << "CBC Decrypted  : \"" << toString(dc256_cbc) << "\"\n";
    cout << "CBC Verify     : "
         << (toString(dc256_cbc) == plaintext ? "PASS [OK]" : "FAIL") << "\n";

    // Key size summary table
    cout << "\n--- Key Size Comparison ---\n";
    cout << "+-------------+----------+-----------+--------+\n";
    cout << "| Variant     | Key Bits | Key Words | Rounds |\n";
    cout << "+-------------+----------+-----------+--------+\n";
    cout << "| AES-128     |   128    |     4     |   10   |\n";
    cout << "| AES-192     |   192    |     6     |   12   |\n";
    cout << "| AES-256     |   256    |     8     |   14   |\n";
    cout << "+-------------+----------+-----------+--------+\n";

    // ── TASK 4: Avalanche Effect ────────────────────────────
    printSeparator("TASK 4: Avalanche Effect in AES-CBC");

    string modified = "CRYPTOGRAPHY IS GUN"; // 'F' → 'G' : 1 bit change
    vector<uint8_t> mod = toBytes(modified);

    vector<uint8_t> orig_ct = aesCBCEncrypt(pt, key128, iv128);
    vector<uint8_t> mod_ct = aesCBCEncrypt(mod, key128, iv128);

    vector<uint8_t> xorDiff(orig_ct.size());
    for (size_t i = 0; i < orig_ct.size(); i++)
        xorDiff[i] = orig_ct[i] ^ mod_ct[i];

    int changedBits = countDiffBits(orig_ct, mod_ct);
    int totalBits = (int)orig_ct.size() * 8;
    int changedBytes = 0;
    for (auto b : xorDiff)
        if (b != 0)
            changedBytes++;

    cout << "Original       : \"" << plaintext << "\"\n";
    cout << "Modified       : \"" << modified << "\"\n";
    cout << "Change         : 'F' -> 'G' at position 17  (1 bit difference)\n";
    printHex("Original CT    : ", orig_ct);
    printHex("Modified CT    : ", mod_ct);
    printHex("XOR Difference : ", xorDiff);
    cout << "Changed bytes  : " << changedBytes << " / " << orig_ct.size() << "\n";
    cout << "Changed bits   : " << changedBits << " / " << totalBits << "\n";
    cout << fixed << setprecision(1)
         << "Avalanche %    : " << (100.0 * changedBits / totalBits) << "%\n";

    printSeparator("All Tasks Complete");
    return 0;
}