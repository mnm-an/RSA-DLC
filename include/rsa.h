#ifndef RSA_H
#define RSA_H

#ifndef RSA_H
#define RSA_H

#include <gmp.h>

// Génération de clés RSA
// Génère les clés publique et privée RSA (n, e, d) en fonction des tailles de p et q.
// - `n`: clé publique (modulus)
// - `e`: exposant public
// - `d`: exposant privé
// - `p` et `q`: deux nombres premiers distincts
void generate_keys(mpz_t n, mpz_t e, mpz_t d, mpz_t p, mpz_t q, unsigned int bits);

// Chiffrement RSA
// Chiffre un message avec la clé publique (e, n).
// - `cipher`: résultat chiffré
// - `message`: message à chiffrer
// - `e`: exposant public
// - `n`: modulus public
void encrypt(mpz_t cipher, const mpz_t message, const mpz_t e, const mpz_t n);

// Déchiffrement RSA
// Déchiffre un message avec la clé privée (d, n).
// - `message`: message déchiffré
// - `cipher`: texte chiffré
// - `d`: exposant privé
// - `n`: modulus public
void decrypt(mpz_t message, const mpz_t cipher, const mpz_t d, const mpz_t n);

// Déchiffrement RSA avec CRT
// Utilise le théorème des restes chinois pour accélérer le déchiffrement.
// - `message`: message déchiffré
// - `cipher`: texte chiffré
// - `p`: premier facteur de n
// - `q`: deuxième facteur de n
// - `d_p`: d mod (p-1)
// - `d_q`: d mod (q-1)
// - `q_inv`: q^{-1} mod p
void decrypt_crt(mpz_t message, const mpz_t cipher, const mpz_t p, const mpz_t q, const mpz_t d_p, const mpz_t d_q, const mpz_t q_inv);

// Génération des composantes CRT
// Calcule les composantes nécessaires pour le déchiffrement CRT.
// - `d_p`: d mod (p-1)
// - `d_q`: d mod (q-1)
// - `q_inv`: q^{-1} mod p
void generate_crt_components(mpz_t d_p, mpz_t d_q, mpz_t q_inv, const mpz_t d, const mpz_t p, const mpz_t q);

// Signature RSA
// Génère une signature pour un message avec la clé privée (d, n).
// - `signature`: résultat signé
// - `message`: message à signer
// - `d`: exposant privé
// - `n`: modulus public
void sign(mpz_t signature, const mpz_t message, const mpz_t d, const mpz_t n);

// Vérification de signature RSA
// Vérifie si une signature est valide pour un message donné avec la clé publique (e, n).
// - `message`: message original
// - `signature`: signature à vérifier
// - `e`: exposant public
// - `n`: modulus public
// Retourne 1 si valide, sinon 0.
int verify(const mpz_t message, const mpz_t signature, const mpz_t e, const mpz_t n);

#endif


#endif
