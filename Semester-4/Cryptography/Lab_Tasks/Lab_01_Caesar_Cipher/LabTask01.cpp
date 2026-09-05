#include <iostream>
#include <string>
#include <cctype>  // For isalpha and toupper/tolower
#include <vector>  // For storing brute force results
#include <utility> // For pair

using namespace std;

class CaesarCipher
{
private:
    int shift;

public:
    // Constructor to set the shift value
    CaesarCipher(int s) : shift(s % 26) {} // Normalize shift to 0-25

    // Function to encrypt plaintext
    string encrypt(const string &text)
    {
        string result = "";
        for (char c : text)
        {
            if (isalpha(c))
            {
                // Convert to numeric value (0-25)
                char base = isupper(c) ? 'A' : 'a';
                int numeric = c - base;
                // Apply encryption formula: (numeric + shift) % 26
                int encryptedNumeric = (numeric + shift) % 26;
                // Convert back to character
                result += encryptedNumeric + base;
            }
            else
            {
                result += c; // Non-alphabetic unchanged
            }
        }
        return result;
    }

    // Function to decrypt ciphertext
    string decrypt(const string &text)
    {
        string result = "";
        for (char c : text)
        {
            if (isalpha(c))
            {
                // Convert to numeric value (0-25)
                char base = isupper(c) ? 'A' : 'a';
                int numeric = c - base;
                // Apply decryption formula: (numeric - shift + 26) % 26
                int decryptedNumeric = (numeric - shift + 26) % 26;
                // Convert back to character
                result += decryptedNumeric + base;
            }
            else
            {
                result += c; // Non-alphabetic unchanged
            }
        }
        return result;
    }

    // Function to perform brute force attack on ciphertext
    // Returns a vector of pairs: (key, decrypted_text)
    vector<pair<int, string>> bruteForceAttack(const string &ciphertext)
    {
        vector<pair<int, string>> results;
        // Try all possible keys from 1 to 25
        for (int s = 1; s <= 25; ++s)
        {
            CaesarCipher tempCipher(s);
            // Decrypt using each key
            string decrypted = tempCipher.decrypt(ciphertext);
            results.push_back({s, decrypted});
        }
        return results;
    }

    // Method to change shift
    void setShift(int s)
    {
        shift = s % 26;
    }

    // Method to get current shift
    int getShift() const
    {
        return shift;
    }
};

int main()
{
    int choice;
    do
    {
        cout << "\nMenu:" << endl;
        cout << "1. Encrypt" << endl;
        cout << "2. Decrypt" << endl;
        cout << "3. Brute Force Attack" << endl;
        cout << "4. Exit" << endl;
        cout << "Choose an option (1-4): ";
        cin >> choice;
        cin.ignore(); // Clear newline from input buffer

        if (choice == 1)
        {
            // Encryption
            string plaintext;
            int K;
            cout << "Enter plaintext: ";
            getline(cin, plaintext);
            cout << "Enter key K (1-25): ";
            cin >> K;
            cin.ignore(); // Clear newline
            CaesarCipher cipher(K);
            string encrypted = cipher.encrypt(plaintext);
            cout << "Ciphertext: " << encrypted << endl;
        }
        else if (choice == 2)
        {
            // Decryption
            string ciphertext;
            int K;
            cout << "Enter ciphertext: ";
            getline(cin, ciphertext);
            cout << "Enter key K (1-25): ";
            cin >> K;
            cin.ignore(); // Clear newline
            CaesarCipher cipher(K);
            string decrypted = cipher.decrypt(ciphertext);
            cout << "Plaintext: " << decrypted << endl;
        }
        else if (choice == 3)
        {
            // Brute Force Attack
            string ciphertext;
            cout << "Enter ciphertext: ";
            getline(cin, ciphertext);
            CaesarCipher cipher(0); // Dummy cipher, not used for brute force
            vector<pair<int, string>> results = cipher.bruteForceAttack(ciphertext);
            cout << "All possible plaintexts (identify the correct one manually):" << endl;
            for (const auto &result : results)
            {
                cout << "Key " << result.first << ": " << result.second << endl;
            }
        }

        else if (choice == 4)
        {
            cout << "Exiting program." << endl;
        }
        else
        {
            cout << "Invalid choice. Please select 1-4." << endl;
        }
    } while (choice != 4);

    return 0;
}