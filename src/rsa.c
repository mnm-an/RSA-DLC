#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "math_utils.h"

void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n)
{
    // Effectuer l'exponentiation modulaire pour chiffrer le message
    rsa_powm(c, m, e, n);
}

void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n)
{
    // Déchiffrement : m = c^d mod n
    rsa_powm(m, c, d, n);
}

void decrypt_crt(mpz_t m, mpz_t c, mpz_t d, mpz_t p, mpz_t q)
{
    mpz_t dp, dq, qinv, m1, m2, h, temp1, temp2;
    mpz_inits(dp, dq, qinv, m1, m2, h, temp1, temp2, NULL);

    // Calcul de dp = d mod (p-1) et dq = d mod (q-1)
    mpz_sub_ui(temp1, p, 1);
    rsa_mod(dp, d, temp1);

    mpz_sub_ui(temp1, q, 1);
    rsa_mod(dq, d, temp1);

    // Calcul de qinv (inverse de q mod p)
    mpz_invert(qinv, q, p);

    // Calcul de m1 = c^dp mod p et m2 = c^dq mod q
    rsa_powm(m1, c, dp, p);
    rsa_powm(m2, c, dq, q);

    // h = qinv * (m1 - m2) mod p
    mpz_sub(h, m1, m2);
    rsa_mod(h, h, p); // Assurer que (m1 - m2) est positif
    mpz_mul(h, h, qinv);
    rsa_mod(h, h, p);

    // Calcul final m = m2 + h * q
    mpz_mul(temp1, h, q);
    mpz_add(m, m2, temp1);

    // Libération des variables temporaires
    mpz_clears(dp, dq, qinv, m1, m2, h, temp1, temp2, NULL);
}

void sign(mpz_t signature, mpz_t message, mpz_t d, mpz_t n)
{
    // Calcul de la signature : signature = message^d mod n
    rsa_powm(signature, message, d, n);
}

int verify(mpz_t signature, mpz_t message, mpz_t e, mpz_t n) {
    mpz_t recovered_message, difference;
    mpz_init(recovered_message);
    mpz_init(difference);

    // Calculer le message récupéré à partir de la signature : recovered_message =
    // signature^e mod n


    rsa_powm(recovered_message, signature, e, n);

    // Calculer la différence entre le message récupéré et le message original
    mpz_sub(difference, recovered_message, message);

    // Vérifier si la différence est égale à zéro
    unsigned long int diff_val = mpz_get_ui(difference);

    // Si la différence est égale à zéro, alors les deux entiers sont égaux
    int is_valid = (diff_val == 0);

    mpz_clear(recovered_message);
    mpz_clear(difference);

    return is_valid;
}
