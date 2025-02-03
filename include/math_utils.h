#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <gmp.h>

// Calcul de modulo normal : r = d % n
void rsa_mod(mpz_t r, mpz_t d, mpz_t n);

// Calcul de modulo avec un entier non GMP : r = d % n (où n est unsigned long int)
void rsa_mod_ui(mpz_t r, mpz_t d, unsigned long int n);

// Calcul de puissance : rop = base ^ exp
void rsa_pow_ui(mpz_t rop, mpz_t base, unsigned long int exp);

// Calcul de puissance avec base et exposant non GMP : rop = base ^ exp
void rsa_ui_pow_ui(mpz_t rop, unsigned long int base, unsigned long int exp);

// Exponentiation modulaire square and multiply always
void rsa_powm(mpz_t rop, mpz_t base, mpz_t exp, mpz_t modulo);

// Calcul de puissance modulaire avec exposant non GMP : rop = (base ^ exp) % modulo
void rsa_powm_ui(mpz_t rop, mpz_t base, unsigned long int exp, mpz_t modulo);

void gcd(mpz_t result, const mpz_t a, const mpz_t b);

// Calcul la relation de bezout : Donnant a et b il retourne u,v tel que a*u + b*v = pgcd
void extended_euclide(mpz_t gcd, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b);

// Calcul de l'inverse modulaire
void modular_inverse(mpz_t result, mpz_t a, mpz_t phi);

// Calcul de l'indicatrice d'euler
void calculate_phi(mpz_t phi, mpz_t p, mpz_t q);


#endif
