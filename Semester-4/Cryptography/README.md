# Cryptography

A collection of cryptography laboratory tasks covering classical ciphers, symmetric and asymmetric cryptography, cryptographic hash functions, steganography, and practical security applications.

The coursework progresses from foundational encryption techniques to modern cryptographic algorithms and OpenSSL-based implementations, with emphasis on understanding algorithm design, key management, cryptanalysis, and security limitations.

## Topics Covered

* **Classical Cryptography:** Caesar, Vigenère, and Playfair ciphers
* **Symmetric Cryptography:** S-DES, DES, 3DES, and AES
* **Modes of Operation:** ECB and CBC
* **Asymmetric Cryptography:** RSA and DSA
* **Hash Functions:** MD5, SHA-256, and SHA-512
* **Cryptanalysis:** Brute-force, linear, and differential analysis
* **Steganography:** LSB-based image data hiding
* **Cryptographic Concepts:** Confusion, diffusion, S-boxes, Feistel networks, SPN, key scheduling, and avalanche effect
* **Practical Cryptography:** Encryption, decryption, digital signatures, and integrity verification

## Tools & Technologies

* **C++** — Cryptographic algorithm implementations
* **CImg** — Image processing for LSB steganography
* **OpenSSL** — AES, RSA, DSA, and hash implementations
* **CryptoTool** — Cryptographic experimentation
* **DES Simulator** — DES analysis and experimentation

## Coursework

| Task        | Focus Area                   |
| ----------- | ---------------------------- |
| Lab Task 01 | Caesar Cipher Implementation |
| Lab Task 03 | Simplified DES (S-DES)       |
| Lab Task 04 | S-DES Brute-Force Attack     |
| Lab Task 05 | Vigenère Cipher              |
| Lab Task 06 | DES, 3DES & CBC/ECB Modes    |
| Lab Task 07 | AES-128, AES-192 & AES-256   |
| Lab Task 08 | Playfair Cipher              |
| Lab Task 09 | AES vs DES Comparative Study |
| Lab Task 10 | AES Encryption using OpenSSL |
| Lab Task 11 | LSB Steganography            |
| Lab Task 12 | RSA using OpenSSL            |
| Lab Task 12 | DSA using OpenSSL            |
| Hash Lab    | MD5, SHA-256 & SHA-512       |

## Key Implementations

### Classical Ciphers

Implemented and analyzed:

* Caesar Cipher with encryption, decryption, and brute-force functionality
* Vigenère Cipher with repeating-key encryption/decryption
* Playfair Cipher using a 5×5 digraph matrix

### Symmetric Cryptography

Implemented:

* S-DES with permutations, S-boxes, Feistel rounds, and key generation
* S-DES brute-force key search across all 1024 possible keys
* DES with 16 Feistel rounds
* 3DES using the EDE construction
* AES supporting 128-, 192-, and 256-bit keys
* ECB and CBC modes with PKCS#7 padding

### Asymmetric Cryptography

Implemented using OpenSSL:

* RSA key generation, encryption/decryption, and digital signatures
* DSA key generation and digital signatures

### Hash Functions

Generated and compared:

* MD5
* SHA-256
* SHA-512

The hash lab demonstrated the avalanche effect by showing how a small input modification produces a substantially different hash.

### Steganography

Implemented LSB image steganography to:

* Embed secret messages into RGB image pixels
* Extract hidden messages from stego images
* Demonstrate practical data hiding techniques

## Security Analysis

The coursework also examined important security limitations and design considerations, including:

* Brute-force attacks against small key spaces
* Linear and differential cryptanalysis
* DES's insecure 56-bit effective key size
* ECB pattern leakage
* Secure use of CBC with an appropriate IV
* MD5's cryptographic weaknesses
* Importance of strong key sizes
* Avalanche effect and diffusion
* Need for cryptographically secure random number generation

## Learning Outcomes

Through these labs, I developed practical understanding of:

* Classical and modern cryptographic algorithms
* Symmetric vs asymmetric cryptography
* Feistel networks and substitution-permutation networks
* Key generation and key scheduling
* S-box substitution and permutation operations
* Encryption modes and padding
* Brute-force and cryptanalytic techniques
* Digital signatures and non-repudiation
* Cryptographic hashing and integrity verification
* Steganographic data hiding
* Practical use of OpenSSL for cryptographic operations

## Coursework Timeline

**February – May 2026**

The coursework progressed from classical cipher implementations to symmetric and asymmetric cryptography, followed by practical hashing, steganography, and OpenSSL-based security operations.

---

**Course:** Cryptography
**Focus:** Cryptographic Algorithms, Secure Encryption, Cryptanalysis, Hashing & Practical Security
