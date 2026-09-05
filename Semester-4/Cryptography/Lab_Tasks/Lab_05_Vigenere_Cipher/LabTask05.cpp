#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Encrypts plaintext using the Vigenère cipher
string vigenereEncrypt(const string &plaintext, const string &key)
{
    string ciphertext = "";
    int keyIndex = 0;
    int keyLen = key.length();

    for (char c : plaintext)
    {
        if (isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            int shift = tolower(key[keyIndex % keyLen]) - 'a';
            ciphertext += (char)((c - base + shift) % 26 + base);
            keyIndex++;
        }
        else
        {
            ciphertext += c;
        }
    }

    return ciphertext;
}

// Decrypts ciphertext using the Vigenère cipher
string vigenereDecrypt(const string &ciphertext, const string &key)
{
    string plaintext = "";
    int keyIndex = 0;
    int keyLen = key.length();

    for (char c : ciphertext)
    {
        if (isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            int shift = tolower(key[keyIndex % keyLen]) - 'a';
            int decrypted = (c - base - shift + 26) % 26;
            plaintext += (char)(decrypted + base);
            keyIndex++;
        }
        else
        {
            plaintext += c;
        }
    }

    return plaintext;
}

int main()
{
    int choice;

    do
    {
        cout << "\n===== Vigenere Cipher Program =====" << endl;
        cout << "1. Encrypt" << endl;
        cout << "2. Decrypt" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice (1-3): ";
        cin >> choice;

        if (choice == 1)
        {
            // Encryption mode
            string plaintext, key;
            cout << "\n--- Encryption ---" << endl;
            cout << "Enter plaintext: ";
            cin.ignore(); // Clear newline from buffer
            getline(cin, plaintext);
            cout << "Enter key: ";
            getline(cin, key);

            if (key.empty())
            {
                cout << "Error: Key cannot be empty!" << endl;
                continue;
            }

            string ciphertext = vigenereEncrypt(plaintext, key);
            cout << "\nResult:" << endl;
            cout << "Plaintext: " << plaintext << endl;
            cout << "Key: " << key << endl;
            cout << "Ciphertext: " << ciphertext << endl;
        }
        else if (choice == 2)
        {
            // Decryption mode
            string ciphertext, key;
            cout << "\n--- Decryption ---" << endl;
            cout << "Enter ciphertext: ";
            cin.ignore(); // Clear newline from buffer
            getline(cin, ciphertext);
            cout << "Enter key: ";
            getline(cin, key);

            if (key.empty())
            {
                cout << "Error: Key cannot be empty!" << endl;
                continue;
            }

            string plaintext = vigenereDecrypt(ciphertext, key);
            cout << "\nResult:" << endl;
            cout << "Ciphertext: " << ciphertext << endl;
            cout << "Key: " << key << endl;
            cout << "Plaintext: " << plaintext << endl;
        }
        else if (choice == 3)
        {
            cout << "\nThank you for using Vigenere Cipher!" << endl;
            break;
        }
        else
        {
            cout << "Invalid choice! Please enter 1, 2, or 3." << endl;
        }

    } while (choice != 3);

    return 0;
}