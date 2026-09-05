#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

// 5×5 Playfair Key Matrix
// Maps keyword + alphabet (I/J merged) to grid
char matrix[5][5];
// Row/col positions for each letter (I/J merged)
int rowOf[26], colOf[26];

void buildMatrix(const string &keyword)
{
    // Initialize used array, merge J into I
    bool used[26] = {};
    used['J' - 'A'] = true;

    string seq = "";
    // Build seq from unique keyword letters
    for (char c : keyword)
    {
        c = toupper(c);
        if (!isalpha(c))
            continue;
        if (c == 'J')
            c = 'I';
        if (!used[c - 'A'])
        {
            used[c - 'A'] = true;
            seq += c;
        }
    }
    // Append remaining letters A-Z
    for (char c = 'A'; c <= 'Z'; c++)
    {
        if (!used[c - 'A'])
            seq += c;
    }

    // Fill matrix row-major and update positions
    for (int i = 0; i < 25; i++)
    {
        int r = i / 5, col = i % 5;
        matrix[r][col] = seq[i];
        rowOf[seq[i] - 'A'] = r;
        colOf[seq[i] - 'A'] = col;
    }
}

void printMatrix()
{
    cout << "\n  Playfair Key Matrix:\n";
    cout << "  +---+---+---+---+---+\n";
    // Print each row with borders
    for (int r = 0; r < 5; r++)
    {
        cout << "  | ";
        // Print each column
        for (int c = 0; c < 5; c++)
            cout << matrix[r][c] << " | ";
        cout << "\n  +---+---+---+---+---+\n";
    }
}

// Prepare plaintext into digraphs: clean, uppercase, I/J merged, pad doubles/single with X
string prepareText(const string &text)
{
    string clean = "";
    // Clean to uppercase alpha only, merge J=I
    for (char c : text)
    {
        if (!isalpha(c))
            continue;
        c = toupper(c);
        if (c == 'J')
            c = 'I';
        clean += c;
    }

    string result = "";
    int i = 0;
    // Form digraphs, pad with X if double or odd length
    while (i < (int)clean.size())
    {
        char a = clean[i];
        if (i + 1 == (int)clean.size())
        {
            // Odd length: pad with X
            result += a;
            result += 'X';
            i++;
        }
        else
        {
            char b = clean[i + 1];
            if (a == b)
            {
                // Same letters: insert X
                result += a;
                result += 'X';
                i++;
            }
            else
            {
                // Valid digraph
                result += a;
                result += b;
                i += 2;
            }
        }
    }
    return result;
}

// Prepare ciphertext for decryption: uppercase alpha only, I/J merged, no padding removal
string prepareCiphertext(const string &text)
{
    string result = "";
    // Clean to uppercase alpha only, merge J=I
    for (char c : text)
    {
        if (!isalpha(c))
            continue;
        c = toupper(c);
        if (c == 'J')
            c = 'I';
        result += c;
    }
    return result;
}

// Process single digraph: row shift, col shift, or rectangle swap based on mode (+1 encrypt, -1 decrypt)
pair<char, char> processDigraph(char a, char b, int mode)
{
    int rA = rowOf[a - 'A'], cA = colOf[a - 'A'];
    int rB = rowOf[b - 'A'], cB = colOf[b - 'A'];

    if (rA == rB)
    {
        // Same row: shift columns
        cA = (cA + mode + 5) % 5;
        cB = (cB + mode + 5) % 5;
    }
    else if (cA == cB)
    {
        // Same column: shift rows
        rA = (rA + mode + 5) % 5;
        rB = (rB + mode + 5) % 5;
    }
    else
    {
        // Rectangle: swap columns
        swap(cA, cB);
    }
    return {matrix[rA][cA], matrix[rB][cB]};
}

string processText(const string &text, int mode)
{
    string result = "";
    // Process each digraph pair
    for (int i = 0; i < (int)text.size(); i += 2)
    {
        auto [a, b] = processDigraph(text[i], text[i + 1], mode);
        result += a;
        result += b;
    }
    return result;
}

// Remove padding X's from decrypted text (end/trailing doubles)
string removePadding(const string &text)
{
    string result = "";
    // Remove terminal X or X between same letters
    for (int i = 0; i < (int)text.size(); i++)
    {
        if (i == (int)text.size() - 1 && text[i] == 'X')
            break; // Skip final pad X
        if (text[i] == 'X' && i > 0 && i < (int)text.size() - 1 && text[i - 1] == text[i + 1])
            continue; // Skip pad X between doubles
        result += text[i];
    }
    return result;
}

// Menu handler functions
void handleEncryption()
{
    string keyword, plaintext;

    cout << "\n--- Encryption ---\n";
    cout << "Enter keyword   : ";
    cin.ignore();
    getline(cin, keyword);

    if (keyword.empty())
    {
        cout << "Error: Keyword cannot be empty!\n";
        return;
    }

    cout << "Enter plaintext : ";
    getline(cin, plaintext);

    if (plaintext.empty())
    {
        cout << "Error: Plaintext cannot be empty!\n";
        return;
    }

    buildMatrix(keyword);
    printMatrix();

    string prepared = prepareText(plaintext);
    string encrypted = processText(prepared, +1);

    cout << "\nResult:\n";
    cout << "Keyword          : " << keyword << "\n";
    cout << "Original Text    : " << plaintext << "\n";
    cout << "Prepared Digraphs: ";
    // Print digraphs
    for (int i = 0; i < (int)prepared.size(); i += 2)
        cout << prepared.substr(i, 2) << " ";
    cout << "\nCiphertext       : " << encrypted << "\n";
}

void handleDecryption()
{
    string keyword, ciphertext;

    cout << "\n--- Decryption ---\n";
    cout << "Enter keyword   : ";
    cin.ignore();
    getline(cin, keyword);

    if (keyword.empty())
    {
        cout << "Error: Keyword cannot be empty!\n";
        return;
    }

    cout << "Enter ciphertext: ";
    getline(cin, ciphertext);

    if (ciphertext.empty())
    {
        cout << "Error: Ciphertext cannot be empty!\n";
        return;
    }

    // Validate even length for digraphs
    string cleaned = prepareCiphertext(ciphertext);
    if (cleaned.size() % 2 != 0)
    {
        cout << "Error: Ciphertext length must be even (Playfair encrypts in pairs)!\n";
        return;
    }

    buildMatrix(keyword);
    printMatrix();

    string decrypted = removePadding(processText(cleaned, -1));

    cout << "\nResult:\n";
    cout << "Keyword          : " << keyword << "\n";
    cout << "Ciphertext       : " << ciphertext << "\n";
    cout << "Decrypted Text   : " << decrypted << "\n";
}

// Main program: menu loop for encrypt/decrypt/exit
int main()
{
    int choice;

    // Loop until exit
    do
    {
        cout << "\n===== Playfair Cipher Program =====\n";
        cout << "1. Encrypt\n";
        cout << "2. Decrypt\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1-3): ";
        cin >> choice;

        if (choice == 1)
        {
            handleEncryption();
        }
        else if (choice == 2)
        {
            handleDecryption();
        }
        else if (choice == 3)
        {
            cout << "\nThank you for using Playfair Cipher!\n";
        }
        else
        {
            cout << "Invalid choice! Please enter 1, 2, or 3.\n";
        }

    } while (choice != 3);

    return 0;
}
