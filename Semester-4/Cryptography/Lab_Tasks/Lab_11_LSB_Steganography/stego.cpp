#include "CImg.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace cimg_library;

//  Steganography — LSB Embed & Extract
//  Cover image : cover.bmp  (736 x 397 RGB)
//  Capacity    : 109,572 characters
class Steganography
{

    string text;
    CImg<unsigned char> img;

public:
    // Load image from file
    Steganography(string imgFile)
    {
        img.load(imgFile.c_str());
        cout << "Image loaded  : " << imgFile << "\n";
        cout << "Dimensions    : " << img.width()
             << " x " << img.height()
             << " x " << img.spectrum() << " channels\n";
        cout << "Max capacity  : "
             << (img.width() * img.height() * img.spectrum()) / 8
             << " characters\n";
    }

    // Read secret message from secret.txt
    void readSecret()
    {
        ifstream f("secret.txt");
        if (!f.is_open())
        {
            cout << "Error: secret.txt not found!\n";
            return;
        }
        getline(f, text);
        f.close();
        cout << "\nSecret message : \"" << text << "\"\n";
        cout << "Length         : " << text.length() << " characters\n";
        cout << "Bits needed    : " << text.length() * 8 << " bits\n";
    }

    // Return specific bit of a character
    // charIdx = which character, bitPos = which bit (0=LSB to 7=MSB)
    int getBit(int charIdx, int bitPos)
    {
        return (text[charIdx] >> bitPos) & 1;
    }

    // Set the LSB of a pixel byte to our secret bit
    // (pixelVal & ~1) clears bit 0, then OR sets it to our bit
    unsigned char setLSB(unsigned char pixelVal, int bit)
    {
        return (pixelVal & ~1) | bit;
    }

    // EMBED: hide secret message bits into image LSBs
    void embed()
    {
        int totalChars = text.length();
        int totalBitsNeeded = totalChars * 8;
        int capacity = img.width() * img.height() * img.spectrum();

        cout << "\n--- Embedding ---\n";
        cout << "Image capacity : " << capacity << " bits\n";
        cout << "Bits needed    : " << totalBitsNeeded << " bits\n";

        if (totalBitsNeeded > capacity)
        {
            cout << "Error: Image too small for this message!\n";
            return;
        }

        int bitPos = 0;
        int charIdx = 0;

        for (int x = 0; x < img.width(); x++)
        {
            for (int y = 0; y < img.height(); y++)
            {
                for (int c = 0; c < img.spectrum(); c++)
                {
                    if (charIdx < totalChars)
                    {
                        int secretBit = getBit(charIdx, bitPos);
                        unsigned char original = img(x, y, 0, c);
                        unsigned char modified = setLSB(original, secretBit);
                        img(x, y, 0, c) = modified;
                        bitPos++;
                        if (bitPos == 8)
                        {
                            bitPos = 0;
                            charIdx++;
                        }
                    }
                }
            }
        }

        cout << "Characters hidden : " << totalChars << "\n";
        cout << "Bits used         : " << totalBitsNeeded << "\n";
    }

    // Save the stego image
    void saveImage()
    {
        img.save("stego.bmp");
        cout << "Stego image saved : stego.bmp\n";
    }

    // EXTRACT: recover secret message from stego image
    // Reads LSB of each pixel channel in same order as embedding
    // Assembles bits into characters 8 at a time
    string extract(string stegoFile, int knownLength)
    {
        CImg<unsigned char> stego;
        stego.load(stegoFile.c_str());

        string recovered = "";
        int currentByte = 0;
        int bitCount = 0;
        int charsRead = 0;

        for (int x = 0; x < stego.width(); x++)
        {
            for (int y = 0; y < stego.height(); y++)
            {
                for (int c = 0; c < stego.spectrum(); c++)
                {
                    if (charsRead < knownLength)
                    {
                        int lsb = stego(x, y, 0, c) & 1;
                        currentByte |= (lsb << bitCount);
                        bitCount++;
                        if (bitCount == 8)
                        {
                            recovered += (char)currentByte;
                            currentByte = 0;
                            bitCount = 0;
                            charsRead++;
                        }
                    }
                }
            }
        }
        return recovered;
    }

    // Run full embed workflow
    void runEmbed()
    {
        readSecret();
        embed();
        saveImage();
    }
};

//  MAIN — Menu
int main()
{
    int choice;

    do
    {
        cout << "\n===== LSB Steganography Program =====\n";
        cout << "Cover image : cover.bmp (736 x 397 RGB)\n";
        cout << "-------------------------------------\n";
        cout << "1. Embed secret message\n";
        cout << "2. Extract secret message\n";
        cout << "3. Exit\n";
        cout << "Enter choice (1-3): ";
        cin >> choice;

        if (choice == 1)
        {

            Steganography s("cover.bmp");
            s.runEmbed();
        }
        else if (choice == 2)
        {

            int len;
            cout << "\nEnter number of characters to extract: ";
            cin >> len;

            cout << "\n--- Extraction ---\n";
            Steganography s("stego.bmp");
            string msg = s.extract("stego.bmp", len);

            cout << "Extracted message : \"" << msg << "\"\n";

            ofstream out("extracted.txt");
            out << msg;
            out.close();
            cout << "Saved to extracted.txt\n";
        }
        else if (choice == 3)
        {
            cout << "Goodbye!\n";
        }
        else
        {
            cout << "Invalid choice! Enter 1, 2, or 3.\n";
        }

    } while (choice != 3);

    return 0;
}