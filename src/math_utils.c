#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "../include/math_utils.h"

// modulo normal
void rsa_mod(mpz_t r, mpz_t d, mpz_t n)
{

    mpz_t temp;
    mpz_init(temp);

    mpz_fdiv_q(temp, d, n);
    mpz_mul(temp, temp, n);
    mpz_sub(r, d, temp);
}

void rsa_mod_ui(mpz_t r, mpz_t d, unsigned long int n)
{

    mpz_t temp;
    mpz_init(temp);

    mpz_fdiv_q_ui(temp, d, n);
    mpz_mul_ui(temp, temp, n);
    mpz_sub(r, d, temp);
}

// rop = base^(exp)
void rsa_pow_ui(mpz_t rop, mpz_t base, unsigned long int exp)
{

    mpz_set_ui(rop, 1); // Initialiser rop à 1 (base^0 = 1)
    mpz_t temp_base;
    mpz_init_set(temp_base, base); // Copie de base pour manipulation

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {                                 // Si le bit courant de exp est 1
            mpz_mul(rop, rop, temp_base); // rop *= temp_base
        }
        mpz_mul(temp_base, temp_base, temp_base); // temp_base = temp_base^2
        exp /= 2;                                 // Décaler exp vers la droite (division par 2)
    }

    mpz_clear(temp_base);
}

void rsa_ui_pow_ui(mpz_t rop, unsigned long int base, unsigned long int exp)
{
    mpz_t mpz_base;
    mpz_init_set_ui(mpz_base, base);
    rsa_pow_ui(rop, mpz_base, exp);
    mpz_clear(mpz_base);
}

// Exponentiation modulaire
void rsa_powm(mpz_t rop, mpz_t base, mpz_t exp, mpz_t modulo)
{

    mpz_t a;
    mpz_init(a);

    int l = (int)mpz_sizeinbase(exp, 2); // Calculer le nombre des bits de la base
    mpz_set_str(a, "1", 10);             // Initialiser a à 1

    for (int i = l - 1; i >= 0; i--)
    {
        mpz_mul(a, a, a);
        rsa_mod(a, a, modulo);

        if (mpz_tstbit(exp, i))
        {
            mpz_mul(a, a, base);
            rsa_mod(a, a, modulo);
        }
    }
    mpz_set(rop, a);
    mpz_clear(a);
}

void rsa_powm_ui(mpz_t rop, mpz_t base, unsigned long int exp, mpz_t modulo)
{

    mpz_t a, mpz_exp;
    mpz_inits(a, mpz_exp, NULL);

    mpz_set_ui(mpz_exp, exp);
    rsa_powm(rop, base, mpz_exp, modulo);
    mpz_clears(a, mpz_exp, NULL);
}

void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n)
{
    // Effectuer l'exponentiation modulaire pour chiffrer le message
    rsa_powm(c, m, e, n);
}

void decrypt_standard(mpz_t m, mpz_t c, mpz_t d, mpz_t n)
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

void rsa_sign(mpz_t signature, mpz_t message, mpz_t d, mpz_t n)
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


int main()
{
    // Test des fonctions de base
    mpz_t a, b, c, d;
    mpz_inits(a, b, c, d, NULL);
    mpz_set_str(a, "51", 10);
    mpz_set_str(b, "5", 10);
    mpz_set_str(c, "181", 10);

    rsa_mod(d, c, a);
    gmp_printf("Operation Modulo : \n 181 %% 51 = %Zd", d);

    rsa_mod_ui(d, c, 76);
    gmp_printf("\nOperation Modulo ui : \n 181 %% 76 = %Zd", d);

    rsa_powm(d, a, b, c);
    gmp_printf("\nOperation exponentiation Modulo : \n pow(51, 5, 181) = %Zd", d);

    rsa_powm_ui(d, a, 5, c);
    gmp_printf("\nOperation exponentiation Modulo ui : \n pow(51, 5, 181) = %Zd\n", d);

    mpz_clears(a, b, c, d, NULL);

    // Test des fonctions RSA
    mpz_t m, e, n, de, ch, p, q, signature;
    mpz_inits(m, e, n, de, ch, p, q, signature, NULL);

    // Données d'exemple
    mpz_set_str(m, "42", 10);    // Message clair
    mpz_set_str(e, "65537", 10); // Exposant public
    mpz_set_str(n, "3233", 10);  // Module public
    mpz_set_str(de, "2753", 10); // Exposant privé
    mpz_set_str(p, "61", 10);    // Premier facteur
    mpz_set_str(q, "53", 10);    // Second facteur

    // Chiffrement
    encrypt(ch, m, e, n);
    gmp_printf("Message chiffré : %Zd\n", ch);

    // Déchiffrement standard
    decrypt_standard(m, ch, de, n);
    gmp_printf("Message déchiffré en mode standard: %Zd\n", m);

    // Déchiffrement CRT
    decrypt_crt(m, ch, de, p, q);
    gmp_printf("Message déchiffré en mode CRT: %Zd\n", m);

    // Signature
    rsa_sign(signature, m, de, n);
    gmp_printf("Signature générée : %Zd\n", signature);

    // Vérification de la signature
    int is_valid = verify(signature, m, e, n);
    if (is_valid)
    {
        printf("La signature est valide.\n");
    }
    else
    {
        printf("La signature est invalide.\n");
    }

    mpz_clears(m, e, n, de, ch, p, q, signature, NULL);

    return 0;
}
