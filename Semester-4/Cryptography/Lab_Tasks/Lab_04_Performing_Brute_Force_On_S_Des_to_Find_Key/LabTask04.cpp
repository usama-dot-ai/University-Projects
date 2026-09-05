#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>
#include <vector>

using namespace std;

// Permutation Tables
// All indices are 1‑based (as in the SDES specification)
const int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_INV[8] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[8] = {4, 1, 2, 3, 2, 3, 4, 1}; // Expansion/Permutation
const int P4[4] = {2, 4, 3, 1};

// S‑boxes (4×4)
const int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}};

const int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}};

// ==================== Utility Functions ====================

// Check if a string contains only '0' and '1'
bool isBinary(const string &s)
{
    for (char c : s)
        if (c != '0' && c != '1')
            return false;
    return true;
}

// Read a binary string of exactly 'bits' characters, with validation
string readBinary(int bits)
{
    string input;
    while (true)
    {
        cin >> input;
        if (input.length() == bits && isBinary(input))
            return input;
        cout << "Invalid input. Enter exactly " << bits << " bits (0/1): ";
    }
}

// Apply a permutation table to an input value.
uint16_t permute(uint16_t input, int in_len, const int perm[], int out_len)
{
    uint16_t output = 0;
    for (int i = 0; i < out_len; i++)
    {
        // Source bit position (0‑based from the left)
        int src = perm[i] - 1;
        // Extract that bit (counting from MSB)
        uint16_t bit = (input >> (in_len - 1 - src)) & 1;
        // Place it in the output (MSB first)
        output |= (bit << (out_len - 1 - i));
    }
    return output;
}

// Circular left shift on a 5‑bit value
uint8_t leftShift5(uint8_t val, int shifts)
{
    shifts %= 5;
    val &= 0x1F; // keep only lower 5 bits
    return ((val << shifts) | (val >> (5 - shifts))) & 0x1F;
}

// ==================== Key Schedule ====================
void generateSubkeys(uint16_t key, uint8_t &k1, uint8_t &k2)
{
    // Step 1: P10
    uint16_t permuted = permute(key, 10, P10, 10);

    // Split into two 5‑bit halves
    uint8_t left = (permuted >> 5) & 0x1F;
    uint8_t right = permuted & 0x1F;

    // Round 1 (shift 1)
    left = leftShift5(left, 1);
    right = leftShift5(right, 1);
    uint16_t combined = (left << 5) | right;
    k1 = permute(combined, 10, P8, 8); // first subkey

    // Round 2 (shift 2 more → total 3)
    left = leftShift5(left, 2);
    right = leftShift5(right, 2);
    combined = (left << 5) | right;
    k2 = permute(combined, 10, P8, 8); // second subkey
}

// ==================== S‑box Lookup ====================
// Input: 4 bits (packed in low nibble of val)
// Output: 2 bits (result of S‑box)
uint8_t sbox(uint8_t val, const int box[4][4])
{
    // Row: bits 1 and 4 (1‑based) → bits 4 and 1 (0‑based)
    uint8_t row = ((val & 0x8) >> 2) | (val & 0x1);
    // Column: bits 2 and 3
    uint8_t col = (val >> 1) & 0x3;
    return box[row][col] & 0x3;
}

// ==================== Feistel Round Function ====================
// f(R, SK) : R is 4 bits, SK is 8 bits (subkey)
uint8_t f(uint8_t r, uint8_t sk)
{
    // Expansion/Permutation (4 → 8 bits)
    uint8_t expanded = permute(r, 4, EP, 8);

    // XOR with subkey
    uint8_t xored = expanded ^ sk;

    // Split into two 4‑bit halves for the S‑boxes
    uint8_t left = (xored >> 4) & 0x0F;
    uint8_t right = xored & 0x0F;

    // Apply S‑boxes (each gives 2 bits)
    uint8_t s0_out = sbox(left, S0);
    uint8_t s1_out = sbox(right, S1);

    // Combine (4 bits: s0_out as high 2, s1_out as low 2)
    uint8_t combined = (s0_out << 2) | s1_out;

    // Apply P4 permutation
    return permute(combined, 4, P4, 4);
}

// ==================== Encryption ====================
uint8_t encrypt(uint8_t plain, uint8_t k1, uint8_t k2)
{
    // Initial Permutation
    uint8_t ip = permute(plain, 8, IP, 8);

    // Split into left and right halves (4 bits each)
    uint8_t left = (ip >> 4) & 0x0F;
    uint8_t right = ip & 0x0F;

    // Round 1
    uint8_t temp = right;
    right = left ^ f(right, k1);
    left = temp;

    // Round 2
    temp = right;
    right = left ^ f(right, k2);
    left = temp;

    // After the two rounds, swap the halves (as required by Feistel)
    uint8_t combined = (right << 4) | left;

    // Inverse Initial Permutation
    return permute(combined, 8, IP_INV, 8);
}

// ==================== Decryption ====================
// Same encryption function but with subkeys reversed
uint8_t decrypt(uint8_t cipher, uint8_t k1, uint8_t k2)
{
    return encrypt(cipher, k2, k1);
}

// ==================== Brute Force Function ====================
void bruteForce(const string &plainStr, const string &cipherStr, uint16_t originalKey)
{
    uint8_t plain = stoi(plainStr, nullptr, 2);
    uint8_t targetCipher = stoi(cipherStr, nullptr, 2);

    cout << "\nStarting brute force over all 1024 possible keys...\n\n";

    vector<uint16_t> matchingKeys;

    for (uint16_t key = 0; key < 1024; ++key)
    {
        // Generate subkeys for this key
        uint8_t k1, k2;
        generateSubkeys(key, k1, k2);

        // Encrypt the plaintext
        uint8_t cipher = encrypt(plain, k1, k2);

        // Check if it matches the target ciphertext
        bool match = (cipher == targetCipher);

        // Display the attempt
        cout << "Key " << key << " (binary " << bitset<10>(key) << ") -> ";
        cout << "Ciphertext: " << bitset<8>(cipher);
        if (match)
        {
            cout << "  <--- MATCH";
            if (key == originalKey)
                cout << " (ORIGINAL KEY)";
            matchingKeys.push_back(key);
        }
        cout << endl;
    }

    // Report results
    cout << "\n===== BRUTE FORCE COMPLETE =====\n";
    if (matchingKeys.empty())
    {
        cout << "No key found that maps the given plaintext to the given ciphertext.\n";
    }
    else
    {
        cout << "Found " << matchingKeys.size() << " matching key(s):\n";
        for (uint16_t k : matchingKeys)
        {
            cout << "  " << bitset<10>(k) << " (decimal " << k << ")";
            if (k == originalKey)
                cout << " <--- ORIGINAL KEY";
            cout << "\n";
        }
    }
}

// ==================== Main Program ====================
int main()
{
    cout << "\n=== SIMPLIFIED DES (SDES) ===\n";

    // Read plaintext and key
    cout << "Enter 8-bit plaintext: ";
    string plainStr = readBinary(8);
    cout << "Enter 10-bit key: ";
    string keyStr = readBinary(10);

    // Convert to integers
    uint8_t plain = stoi(plainStr, nullptr, 2);
    uint16_t key = stoi(keyStr, nullptr, 2);

    // Generate subkeys
    uint8_t k1, k2;
    generateSubkeys(key, k1, k2);

    // Encrypt
    uint8_t cipher = encrypt(plain, k1, k2);
    uint8_t decrypted = decrypt(cipher, k1, k2);

    // Display encryption results
    cout << "\nSubkey K1: " << bitset<8>(k1);
    cout << "\nSubkey K2: " << bitset<8>(k2);

    cout << "\n\nPlaintext : " << plainStr;
    cout << "\nCiphertext: " << bitset<8>(cipher);
    cout << "\nDecrypted : " << bitset<8>(decrypted);

    if (plain == decrypted)
        cout << "\n\nDecryption successful - original plaintext recovered.\n";
    else
        cout << "\n\nDecryption failed - something went wrong.\n";

    // Now perform brute force using the same plaintext and resulting ciphertext
    bruteForce(plainStr, bitset<8>(cipher).to_string(), key);

    cout << endl;
    return 0;
}