#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <random>
#include <ctime>

using namespace std;

// ------------------------------------------------------------
// DES Core Algorithm Implementation
// ------------------------------------------------------------

// Initial Permutation (IP) table
const int IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};

// Final Permutation (FP) - inverse of IP
const int FP[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25};

// Expansion table (E)
const int E[] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};

// Permutation table (P)
const int P[] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25};

// Permuted Choice 1 (PC-1) for key schedule
const int PC1[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};

// Permuted Choice 2 (PC-2) for key schedule
const int PC2[] = {
    14, 17, 11, 24, 1, 5, 3, 28,
    15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32};

// Number of left shifts per round for key schedule
const int SHIFTS[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// S-Boxes
const int S[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

// ------------------------------------------------------------
// Utility Functions
// ------------------------------------------------------------

void random_bytes(uint8_t *buffer, size_t size)
{
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<uint8_t> dist(0, 255);
    for (size_t i = 0; i < size; ++i)
        buffer[i] = dist(gen);
}

void des_set_parity(uint8_t *key)
{
    for (int i = 0; i < 8; ++i)
    {
        int bits = 0;
        for (int j = 0; j < 7; ++j)
            if (key[i] & (1 << j))
                bits++;
        if (bits % 2 == 0)
            key[i] ^= 0x01;
        else
            key[i] &= 0xFE;
    }
}

vector<uint8_t> add_pkcs7_padding(const vector<uint8_t> &data, size_t block_size)
{
    vector<uint8_t> padded = data;
    size_t pad_len = block_size - (data.size() % block_size);
    padded.insert(padded.end(), pad_len, static_cast<uint8_t>(pad_len));
    return padded;
}

bool remove_pkcs7_padding(vector<uint8_t> &data)
{
    if (data.empty())
        return false;
    uint8_t pad_len = data.back();
    if (pad_len == 0 || pad_len > 8)
        return false;
    for (size_t i = data.size() - pad_len; i < data.size(); ++i)
        if (data[i] != pad_len)
            return false;
    data.resize(data.size() - pad_len);
    return true;
}

// Print hex from vector
void print_hex(const vector<uint8_t> &data, const string &label)
{
    cout << label << ": ";
    for (auto byte : data)
        cout << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    cout << dec << endl;
}

// Print hex from raw pointer
void print_hex(const uint8_t *data, size_t len, const string &label)
{
    cout << label << ": ";
    for (size_t i = 0; i < len; ++i)
        cout << hex << setw(2) << setfill('0') << static_cast<int>(data[i]);
    cout << dec << endl;
}

// Print a section header
void print_header(const string &title)
{
    cout << "\n=============================================" << endl;
    cout << "  " << title << endl;
    cout << "=============================================" << endl;
}

// Count differing bits between two byte vectors (for avalanche)
int count_bit_differences(const vector<uint8_t> &a, const vector<uint8_t> &b)
{
    int diff = 0;
    size_t len = min(a.size(), b.size());
    for (size_t i = 0; i < len; i++)
    {
        uint8_t x = a[i] ^ b[i];
        while (x)
        {
            diff += x & 1;
            x >>= 1;
        }
    }
    return diff;
}

// ------------------------------------------------------------
// Core DES Operations
// ------------------------------------------------------------

uint64_t permute(uint64_t input, const int *table, int table_size, int input_bits)
{
    uint64_t output = 0;
    for (int i = 0; i < table_size; ++i)
    {
        int bit_pos = table[i] - 1;
        if (bit_pos < input_bits)
        {
            uint64_t bit = (input >> (input_bits - 1 - bit_pos)) & 1;
            output |= (bit << (table_size - 1 - i));
        }
    }
    return output;
}

uint32_t rotate_left(uint32_t value, int shifts, int bits)
{
    uint32_t mask = (bits == 28) ? 0x0FFFFFFF : 0xFFFFFFFF;
    return ((value << shifts) | (value >> (bits - shifts))) & mask;
}

void des_generate_keys(const uint8_t *key, uint64_t round_keys[16])
{
    uint64_t key64 = 0;
    for (int i = 0; i < 8; ++i)
        key64 = (key64 << 8) | key[i];

    uint64_t key56 = permute(key64, PC1, 56, 64);
    uint32_t C = (key56 >> 28) & 0x0FFFFFFF;
    uint32_t D = key56 & 0x0FFFFFFF;

    for (int round = 0; round < 16; ++round)
    {
        C = rotate_left(C, SHIFTS[round], 28);
        D = rotate_left(D, SHIFTS[round], 28);
        uint64_t combined = ((uint64_t)C << 28) | D;
        round_keys[round] = permute(combined, PC2, 48, 56);
    }
}

uint32_t des_f(uint32_t R, uint64_t K)
{
    uint64_t R_expanded = permute(R, E, 48, 32);
    R_expanded ^= K;
    uint32_t output = 0;
    for (int i = 0; i < 8; ++i)
    {
        uint8_t six_bits = (R_expanded >> (42 - i * 6)) & 0x3F;
        uint8_t row = ((six_bits & 0x20) >> 4) | (six_bits & 1);
        uint8_t col = (six_bits >> 1) & 0x0F;
        uint8_t s_value = S[i][row][col];
        output = (output << 4) | s_value;
    }
    return permute(output, P, 32, 32);
}

uint64_t des_process_block(uint64_t block, const uint64_t round_keys[16], bool encrypt)
{
    block = permute(block, IP, 64, 64);
    uint32_t L = (block >> 32) & 0xFFFFFFFF;
    uint32_t R = block & 0xFFFFFFFF;
    for (int round = 0; round < 16; ++round)
    {
        int key_idx = encrypt ? round : 15 - round;
        uint32_t temp = R;
        R = L ^ des_f(R, round_keys[key_idx]);
        L = temp;
    }
    block = ((uint64_t)R << 32) | L;
    return permute(block, FP, 64, 64);
}

// ------------------------------------------------------------
// DES Class
// ------------------------------------------------------------

class DES
{
private:
    uint64_t round_keys[16];

public:
    DES(const uint8_t *key)
    {
        des_generate_keys(key, round_keys);
    }

    void encrypt_block(const uint8_t *plaintext, uint8_t *ciphertext)
    {
        uint64_t block = 0;
        for (int i = 0; i < 8; ++i)
            block = (block << 8) | plaintext[i];
        uint64_t result = des_process_block(block, round_keys, true);
        for (int i = 0; i < 8; ++i)
        {
            ciphertext[7 - i] = result & 0xFF;
            result >>= 8;
        }
    }

    void decrypt_block(const uint8_t *ciphertext, uint8_t *plaintext)
    {
        uint64_t block = 0;
        for (int i = 0; i < 8; ++i)
            block = (block << 8) | ciphertext[i];
        uint64_t result = des_process_block(block, round_keys, false);
        for (int i = 0; i < 8; ++i)
        {
            plaintext[7 - i] = result & 0xFF;
            result >>= 8;
        }
    }
};

// ------------------------------------------------------------
// ECB Mode
// ------------------------------------------------------------

vector<uint8_t> des_ecb_encrypt(const vector<uint8_t> &plaintext, const uint8_t *key)
{
    DES des(key);
    vector<uint8_t> padded = add_pkcs7_padding(plaintext, 8);
    vector<uint8_t> ciphertext(padded.size(), 0);
    for (size_t i = 0; i < padded.size(); i += 8)
        des.encrypt_block(&padded[i], &ciphertext[i]);
    return ciphertext;
}

vector<uint8_t> des_ecb_decrypt(const vector<uint8_t> &ciphertext, const uint8_t *key)
{
    DES des(key);
    vector<uint8_t> plaintext(ciphertext.size(), 0);
    for (size_t i = 0; i < ciphertext.size(); i += 8)
        des.decrypt_block(&ciphertext[i], &plaintext[i]);
    if (!remove_pkcs7_padding(plaintext))
        cerr << "Warning: Padding error in ECB decryption" << endl;
    return plaintext;
}

// ------------------------------------------------------------
// CBC Mode
// ------------------------------------------------------------

vector<uint8_t> des_cbc_encrypt(const vector<uint8_t> &plaintext,
                                const uint8_t *key, const uint8_t *iv)
{
    DES des(key);
    vector<uint8_t> padded = add_pkcs7_padding(plaintext, 8);
    vector<uint8_t> ciphertext(padded.size(), 0);
    uint8_t previous[8];
    memcpy(previous, iv, 8);
    for (size_t i = 0; i < padded.size(); i += 8)
    {
        uint8_t block[8];
        for (int j = 0; j < 8; ++j)
            block[j] = padded[i + j] ^ previous[j];
        des.encrypt_block(block, &ciphertext[i]);
        memcpy(previous, &ciphertext[i], 8);
    }
    return ciphertext;
}

vector<uint8_t> des_cbc_decrypt(const vector<uint8_t> &ciphertext,
                                const uint8_t *key, const uint8_t *iv)
{
    DES des(key);
    vector<uint8_t> plaintext(ciphertext.size(), 0);
    uint8_t previous[8], current[8];
    memcpy(previous, iv, 8);
    for (size_t i = 0; i < ciphertext.size(); i += 8)
    {
        memcpy(current, &ciphertext[i], 8);
        des.decrypt_block(&ciphertext[i], &plaintext[i]);
        for (int j = 0; j < 8; ++j)
            plaintext[i + j] ^= previous[j];
        memcpy(previous, current, 8);
    }
    if (!remove_pkcs7_padding(plaintext))
        cerr << "Warning: Padding error in CBC decryption" << endl;
    return plaintext;
}

// ------------------------------------------------------------
// 3DES Class (EDE - Encrypt-Decrypt-Encrypt)
// ------------------------------------------------------------

class TripleDES
{
private:
    uint64_t k1_round_keys[16];
    uint64_t k2_round_keys[16];
    uint64_t k3_round_keys[16];

public:
    TripleDES(const uint8_t *key1, const uint8_t *key2, const uint8_t *key3)
    {
        des_generate_keys(key1, k1_round_keys);
        des_generate_keys(key2, k2_round_keys);
        des_generate_keys(key3, k3_round_keys);
    }

    void encrypt_block(const uint8_t *plaintext, uint8_t *ciphertext)
    {
        uint64_t block = 0;
        for (int i = 0; i < 8; ++i)
            block = (block << 8) | plaintext[i];
        block = des_process_block(block, k1_round_keys, true);  // Encrypt K1
        block = des_process_block(block, k2_round_keys, false); // Decrypt K2
        block = des_process_block(block, k3_round_keys, true);  // Encrypt K3
        for (int i = 0; i < 8; ++i)
        {
            ciphertext[7 - i] = block & 0xFF;
            block >>= 8;
        }
    }

    void decrypt_block(const uint8_t *ciphertext, uint8_t *plaintext)
    {
        uint64_t block = 0;
        for (int i = 0; i < 8; ++i)
            block = (block << 8) | ciphertext[i];
        block = des_process_block(block, k3_round_keys, false); // Decrypt K3
        block = des_process_block(block, k2_round_keys, true);  // Encrypt K2
        block = des_process_block(block, k1_round_keys, false); // Decrypt K1
        for (int i = 0; i < 8; ++i)
        {
            plaintext[7 - i] = block & 0xFF;
            block >>= 8;
        }
    }
};

// ------------------------------------------------------------
// 3DES ECB Mode
// ------------------------------------------------------------

vector<uint8_t> td_ecb_encrypt(const vector<uint8_t> &plaintext,
                               const uint8_t *key1, const uint8_t *key2, const uint8_t *key3)
{
    TripleDES td(key1, key2, key3);
    vector<uint8_t> padded = add_pkcs7_padding(plaintext, 8);
    vector<uint8_t> ciphertext(padded.size(), 0);
    for (size_t i = 0; i < padded.size(); i += 8)
        td.encrypt_block(&padded[i], &ciphertext[i]);
    return ciphertext;
}

vector<uint8_t> td_ecb_decrypt(const vector<uint8_t> &ciphertext,
                               const uint8_t *key1, const uint8_t *key2, const uint8_t *key3)
{
    TripleDES td(key1, key2, key3);
    vector<uint8_t> plaintext(ciphertext.size(), 0);
    for (size_t i = 0; i < ciphertext.size(); i += 8)
        td.decrypt_block(&ciphertext[i], &plaintext[i]);
    remove_pkcs7_padding(plaintext);
    return plaintext;
}

// ------------------------------------------------------------
// 3DES CBC Mode
// ------------------------------------------------------------

vector<uint8_t> td_cbc_encrypt(const vector<uint8_t> &plaintext,
                               const uint8_t *key1, const uint8_t *key2, const uint8_t *key3,
                               const uint8_t *iv)
{
    TripleDES td(key1, key2, key3);
    vector<uint8_t> padded = add_pkcs7_padding(plaintext, 8);
    vector<uint8_t> ciphertext(padded.size(), 0);
    uint8_t previous[8];
    memcpy(previous, iv, 8);
    for (size_t i = 0; i < padded.size(); i += 8)
    {
        uint8_t block[8];
        for (int j = 0; j < 8; ++j)
            block[j] = padded[i + j] ^ previous[j];
        td.encrypt_block(block, &ciphertext[i]);
        memcpy(previous, &ciphertext[i], 8);
    }
    return ciphertext;
}

vector<uint8_t> td_cbc_decrypt(const vector<uint8_t> &ciphertext,
                               const uint8_t *key1, const uint8_t *key2, const uint8_t *key3,
                               const uint8_t *iv)
{
    TripleDES td(key1, key2, key3);
    vector<uint8_t> plaintext(ciphertext.size(), 0);
    uint8_t previous[8], current[8];
    memcpy(previous, iv, 8);
    for (size_t i = 0; i < ciphertext.size(); i += 8)
    {
        memcpy(current, &ciphertext[i], 8);
        td.decrypt_block(&ciphertext[i], &plaintext[i]);
        for (int j = 0; j < 8; ++j)
            plaintext[i + j] ^= previous[j];
        memcpy(previous, current, 8);
    }
    remove_pkcs7_padding(plaintext);
    return plaintext;
}

// ============================================================
// MAIN
// ============================================================

int main()
{

    const string plaintext_str = "CRYPTOGRAPHY IS FUN";
    vector<uint8_t> plaintext(plaintext_str.begin(), plaintext_str.end());

    // --------------------------------------------------------
    // TASK 1: DES Encryption and Decryption — ECB Mode
    // --------------------------------------------------------
    print_header("TASK 1: DES Encryption & Decryption (ECB Mode)");

    // Step 1: Generate random 64-bit DES key
    uint8_t des_key[8];
    random_bytes(des_key, 8);
    des_set_parity(des_key);
    print_hex(des_key, 8, "Generated DES Key (64-bit)");

    cout << "Plaintext         : \"" << plaintext_str << "\"" << endl;
    cout << "Plaintext Length  : " << plaintext_str.size()
         << " bytes (padded to " << ((plaintext_str.size() / 8 + 1) * 8)
         << " bytes with PKCS7)" << endl;

    // Step 2: Encrypt using DES ECB
    vector<uint8_t> des_ecb_cipher = des_ecb_encrypt(plaintext, des_key);
    print_hex(des_ecb_cipher, "DES ECB Ciphertext");

    // Step 3: Decrypt ciphertext back
    vector<uint8_t> des_ecb_decrypted = des_ecb_decrypt(des_ecb_cipher, des_key);
    string des_ecb_decrypted_str(des_ecb_decrypted.begin(), des_ecb_decrypted.end());

    // Step 4: Print decrypted plaintext and verify
    cout << "DES ECB Decrypted : \"" << des_ecb_decrypted_str << "\"" << endl;
    cout << "Verification      : "
         << (des_ecb_decrypted_str == plaintext_str ? "PASS [OK]" : "FAIL [ERROR]") << endl;

    // --------------------------------------------------------
    // TASK 2: Compare ECB vs CBC Modes
    // --------------------------------------------------------
    print_header("TASK 2: CBC Mode & ECB vs CBC Comparison");

    // Generate random IV for CBC
    uint8_t iv[8];
    random_bytes(iv, 8);
    print_hex(iv, 8, "IV (Initialization Vector)");

    // Step 1: Encrypt using DES CBC
    vector<uint8_t> des_cbc_cipher = des_cbc_encrypt(plaintext, des_key, iv);
    print_hex(des_cbc_cipher, "DES CBC Ciphertext");

    // Step 2: Decrypt and verify
    vector<uint8_t> des_cbc_decrypted = des_cbc_decrypt(des_cbc_cipher, des_key, iv);
    string des_cbc_decrypted_str(des_cbc_decrypted.begin(), des_cbc_decrypted.end());
    cout << "DES CBC Decrypted : \"" << des_cbc_decrypted_str << "\"" << endl;
    cout << "Verification      : "
         << (des_cbc_decrypted_str == plaintext_str ? "PASS [OK]" : "FAIL [ERROR]") << endl;

    // Step 3: Side-by-side ciphertext comparison (explanation in report)
    cout << "\n--- ECB vs CBC Ciphertext Comparison ---" << endl;
    print_hex(des_ecb_cipher, "ECB Ciphertext");
    print_hex(des_cbc_cipher, "CBC Ciphertext");
    cout << "Ciphertexts match : "
         << (des_ecb_cipher == des_cbc_cipher ? "YES (unexpected!)" : "NO (expected)") << endl;

    // --------------------------------------------------------
    // TASK 3: Key Size Variation — DES vs 3DES
    // --------------------------------------------------------
    print_header("TASK 3: Key Size Variation - DES vs 3DES");

    // Generate 3 independent keys for 3DES
    uint8_t td_key1[8], td_key2[8], td_key3[8];
    random_bytes(td_key1, 8);
    des_set_parity(td_key1);
    random_bytes(td_key2, 8);
    des_set_parity(td_key2);
    random_bytes(td_key3, 8);
    des_set_parity(td_key3);

    print_hex(td_key1, 8, "3DES Key 1 (64-bit)");
    print_hex(td_key2, 8, "3DES Key 2 (64-bit)");
    print_hex(td_key3, 8, "3DES Key 3 (64-bit)");

    uint8_t td_iv[8];
    random_bytes(td_iv, 8);
    print_hex(td_iv, 8, "3DES IV");

    // 3DES ECB
    cout << "\n--- 3DES ECB ---" << endl;
    vector<uint8_t> td_ecb_cipher = td_ecb_encrypt(plaintext, td_key1, td_key2, td_key3);
    print_hex(td_ecb_cipher, "3DES ECB Ciphertext");
    vector<uint8_t> td_ecb_decrypted = td_ecb_decrypt(td_ecb_cipher, td_key1, td_key2, td_key3);
    string td_ecb_str(td_ecb_decrypted.begin(), td_ecb_decrypted.end());
    cout << "3DES ECB Decrypted: \"" << td_ecb_str << "\"" << endl;
    cout << "Verification      : "
         << (td_ecb_str == plaintext_str ? "PASS [OK]" : "FAIL [ERROR]") << endl;

    // 3DES CBC
    cout << "\n--- 3DES CBC ---" << endl;
    vector<uint8_t> td_cbc_cipher = td_cbc_encrypt(plaintext, td_key1, td_key2, td_key3, td_iv);
    print_hex(td_cbc_cipher, "3DES CBC Ciphertext");
    vector<uint8_t> td_cbc_decrypted = td_cbc_decrypt(td_cbc_cipher, td_key1, td_key2, td_key3, td_iv);
    string td_cbc_str(td_cbc_decrypted.begin(), td_cbc_decrypted.end());
    cout << "3DES CBC Decrypted: \"" << td_cbc_str << "\"" << endl;
    cout << "Verification      : "
         << (td_cbc_str == plaintext_str ? "PASS [OK]" : "FAIL [ERROR]") << endl;

    // --------------------------------------------------------
    // TASK 4: Avalanche Effect in DES (CBC Mode)
    // --------------------------------------------------------
    print_header("TASK 4 (Advanced): Avalanche Effect in DES-CBC");

    // Step 1: Modify plaintext by one character ('F' -> 'G')
    const string modified_str = "CRYPTOGRAPHY IS GUN";
    vector<uint8_t> modified_pt(modified_str.begin(), modified_str.end());

    cout << "Original Plaintext : \"" << plaintext_str << "\"" << endl;
    cout << "Modified Plaintext : \"" << modified_str << "\"" << endl;
    cout << "Change             : position 17 changed ('F' -> 'G') = 1 bit difference" << endl;

    // Step 2: Encrypt both in CBC mode using the same key and IV
    vector<uint8_t> cipher_orig = des_cbc_encrypt(plaintext, des_key, iv);
    vector<uint8_t> cipher_mod = des_cbc_encrypt(modified_pt, des_key, iv);

    print_hex(cipher_orig, "\nOriginal CBC Ciphertext");
    print_hex(cipher_mod, "Modified CBC Ciphertext");

    // Step 3: XOR difference and bit count
    vector<uint8_t> xor_diff(cipher_orig.size());
    int changed_bytes = 0;
    for (size_t i = 0; i < cipher_orig.size(); i++)
    {
        xor_diff[i] = cipher_orig[i] ^ cipher_mod[i];
        if (xor_diff[i] != 0)
            changed_bytes++;
    }
    print_hex(xor_diff, "XOR Difference     ");

    int bits_changed = count_bit_differences(cipher_orig, cipher_mod);
    int total_bits = (int)(cipher_orig.size() * 8);
    double percent = (100.0 * bits_changed) / total_bits;

    cout << "\nChanged bytes : " << changed_bytes << " / " << cipher_orig.size() << endl;
    cout << "Changed bits  : " << bits_changed << " / " << total_bits << endl;
    cout << "Avalanche %   : " << fixed << setprecision(1) << percent << "%" << endl;

    cout << "\n  All Tasks Completed." << endl;
    cout << endl;

    return 0;
}