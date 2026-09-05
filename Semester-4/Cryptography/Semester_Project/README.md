# Secure File Encryption and Digital Signatures

A hybrid cryptographic file security project implemented in **C++ using OpenSSL**. The system combines symmetric encryption, asymmetric key protection, digital signatures, and SHA-256 integrity verification to provide confidentiality, authenticity, and integrity for different types of files.

## Project Overview

The project implements a hybrid cryptographic framework in which:

* **AES-256-CBC** and **DES-CBC** provide symmetric file encryption.
* **RSA-2048** securely encrypts and transports symmetric session keys using OAEP padding.
* **DSA-2048** generates digital signatures for encrypted files.
* **SHA-256** provides file integrity verification.
* Random session keys and IVs are generated for symmetric encryption.
* Text, image, and PDF files are tested to verify successful encryption, decryption, and structural integrity.

## Architecture

```text
                    ┌─────────────────────┐
                    │      Input File     │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │ AES-256-CBC / DES   │
                    │   File Encryption   │
                    └──────────┬──────────┘
                               │
                 ┌─────────────┴─────────────┐
                 ▼                           ▼
        ┌─────────────────┐         ┌─────────────────┐
        │ RSA-2048 + OAEP │         │   DSA-2048      │
        │ Session Key     │         │ Digital         │
        │ Protection      │         │ Signature       │
        └─────────────────┘         └─────────────────┘
                 │                           │
                 └─────────────┬─────────────┘
                               ▼
                    ┌─────────────────────┐
                    │ SHA-256 Integrity   │
                    │     Verification    │
                    └─────────────────────┘
```

The design uses symmetric cryptography for bulk data and asymmetric cryptography for session-key protection and signatures, combining the performance advantages of symmetric encryption with public-key security mechanisms.

## Cryptographic Components

| Component       | Purpose                                       |
| --------------- | --------------------------------------------- |
| AES-256-CBC     | Primary high-performance file encryption      |
| DES-CBC         | Comparative symmetric encryption              |
| RSA-2048        | Secure symmetric session-key encryption       |
| OAEP            | RSA encryption padding                        |
| DSA-2048        | Digital signature generation and verification |
| SHA-256         | File integrity hashing                        |
| OpenSSL EVP API | Symmetric encryption/decryption               |
| C++             | Core implementation                           |

## Workflow

### 1. Key Generation

RSA and DSA key pairs are generated using OpenSSL.

### 2. Symmetric Encryption

A random session key and IV are generated using `RAND_bytes()`. The selected file is encrypted using AES-256-CBC or DES-CBC through the OpenSSL EVP API.

### 3. Session-Key Protection

The symmetric session key is encrypted with the RSA-2048 public key using OAEP padding.

### 4. Digital Signature

A SHA-256 digest of the encrypted file is generated and used to create a DSA digital signature.

### 5. Integrity & Tamper Verification

The signature is verified against the encrypted file. Any modification to the encrypted data causes signature verification to fail.

## Project Features

* AES-256-CBC file encryption/decryption
* DES-CBC file encryption/decryption
* RSA-2048 session-key protection
* RSA-OAEP padding
* DSA digital signatures
* SHA-256 integrity verification
* Random key and IV generation
* Binary file processing
* Support for text, image, and PDF files
* AES vs DES performance benchmarking
* Tamper detection through signature verification

## Testing

The project was tested with multiple file formats and encryption configurations.

| File Type | Algorithm   | Signature | Tamper Detection | Integrity |
| --------- | ----------- | --------- | ---------------- | --------- |
| Text      | AES-256-CBC | PASS      | YES              | PASS      |
| Text      | DES-CBC     | PASS      | YES              | PASS      |
| Image     | AES-256-CBC | PASS      | YES              | PASS      |
| PDF       | AES-256-CBC | PASS      | YES              | PASS      |

The report also documents successful SHA-256 comparison between the original and decrypted image, confirming that the byte streams matched.

## Performance

The project benchmarked AES-256-CBC and DES-CBC across different file sizes.

| File  |    Size | AES-256-CBC |  DES-CBC |
| ----- | ------: | ----------: | -------: |
| Text  |   45 KB |     0.42 ms |  0.68 ms |
| Image |  2.4 MB |     8.12 ms | 34.56 ms |
| PDF   | 18.5 MB |    44.21 ms |      N/A |

For a separate 10 MB benchmark, AES-256 recorded **16 ms encryption / 9 ms decryption**, while DES recorded **118 ms encryption / 110 ms decryption**.

## Security Demonstration

The project demonstrates several important cryptographic security principles:

* **Confidentiality** through symmetric encryption
* **Secure key management** through RSA key wrapping
* **Authenticity** through digital signatures
* **Integrity** through SHA-256 and signature verification
* **Tamper detection** through verification failure after modifying encrypted data
* **Performance considerations** when selecting cryptographic algorithms

The project also demonstrates why DES is unsuitable for modern secure systems due to its limited effective key size, while AES-256 provides substantially stronger security and performance characteristics.

## Implementation Highlights

The C++ implementation includes functions for:

* SHA-256 file hashing
* AES/DES encryption and decryption through the EVP API
* RSA session-key encryption
* DSA signature generation
* DSA signature verification
* Performance measurement
* Automated tamper testing

## Technologies

* **Language:** C++
* **Cryptographic Library:** OpenSSL
* **Environment:** Kali Linux
* **Algorithms:** AES-256-CBC, DES-CBC, RSA-2048, DSA-2048, SHA-256
* **RSA Padding:** OAEP

## Course Information

**Course:** Cryptography (Lab)
**Project:** Secure File Encryption and Digital Signatures
**Student:** Usama Arshad — 247141
**Program:** BS Cyber Security — Fall 2024
**Instructor:** Mr. Omer Ali
**Submission Date:** May 24, 2026
**Academic Year:** 2024–2028

## Key Learning Outcomes

* Practical implementation of hybrid cryptographic systems
* Symmetric and asymmetric encryption
* Secure session-key management
* Digital signature generation and verification
* Cryptographic hashing and integrity checking
* File-level encryption and decryption
* Cryptographic performance benchmarking
* Detection of unauthorized data modification

---

**Project Focus:** Hybrid Cryptography, Secure File Encryption, Key Management, Digital Signatures & Integrity Verification
