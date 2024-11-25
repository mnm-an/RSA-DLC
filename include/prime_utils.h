#ifndef PRIME_UTILS_H
#define PRIME_UTILS_H

#include <gmp.h>

// Test de primalité de base avec Miller-Rabin
// Vérifie si `n` est probablement premier en utilisant une base `a`.
// Renvoie 1 si probablement premier, sinon 0.
int is_prime_base(mpz_t n, mpz_t a);

// Test de primalité complet avec Miller-Rabin
// Vérifie si `n` est probablement premier avec `t` itérations.
// Renvoie 1 si probablement premier, sinon 0.
int is_prime(mpz_t n, int t);

// Génération d'un nombre premier
// Génère un nombre premier de `bits` bits et le stocke dans `prime`.
void get_prime(mpz_t prime, int bits);

#endif

