#ifndef RSA_H
#define RSA_H

#include <gmp.h>

// Génération de clés RSA
void generate_keys(mpz_t n, mpz_t e, mpz_t d, mpz_t p, mpz_t q, unsigned int bits);

// Chiffrement RSA
void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n);

// Déchiffrement RSA
void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n);

// Déchiffrement RSA avec CRT (optimisé)
void decrypt_crt(mpz_t m, mpz_t c, mpz_t d, mpz_t p, mpz_t q);

// Signature RSA
void sign(mpz_t signature, mpz_t message, mpz_t d, mpz_t n);

// Vérification de signature RSA
int verify(mpz_t signature, mpz_t message, mpz_t e, mpz_t n);

// Conversion String ↔ mpz
void str_to_mpz(mpz_t m, char *msg);  
void mpz_to_str(mpz_t m, char* msg);   

#endif  // RSA_H
