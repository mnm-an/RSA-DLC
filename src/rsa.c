#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "math_utils.h"
#include "prime_utils.h"

void generate_keys(mpz_t n, mpz_t e, mpz_t d, mpz_t p, mpz_t q, int bits){
    mpz_t phi;
    mpz_init(phi);

    // 1 - Génération de deux nombres premiers distincts
    get_prime(p, bits);
    do {
        get_prime(q, bits);
    } while (mpz_cmp(p, q) == 0);  // Vérifier que p ≠ q

    // 2 - Calcul de n = p * q
    mpz_mul(n, p, q);    // n = p * q

    // 3 - Calcul de φ(n) = (p - 1) * (q - 1)
    calculate_phi(phi, p, q);    

    // 4 - Choisir e et vérifier qu'il est inversible mod φ(n)
    mpz_set_ui(e, 65537);  // Premier choix : 4ème nombre de Fermat

    while (1) {
        modular_inverse(d, e, phi);
        if (mpz_cmp_ui(d, 0) != 0) {
            break;  // d a été trouvé, e est correct
        }
        mpz_add_ui(e, e, 2);  // Essayer un autre e impair
    }


    mpz_clear(phi);
}

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
    modular_inverse(qinv, q, p);

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

void str_to_mpz(mpz_t m, char *msg) {
    mpz_set_str(m, "0", 10);  // Initialiser m=0

    while (*msg != '\0') {  // parcourir le msg
        mpz_mul_ui(m, m, 256);  // Shift left 8 bits 
        mpz_add_ui(m, m, (unsigned char)*msg);  // ajouter la valeur Ascci au m
        msg++; 
    }
}

void mpz_to_str(mpz_t m, char* msg) {
    int i = 0;
    unsigned char byte;
    
    // Décoder le nombre
    while (mpz_cmp_ui(m, 0) > 0) {
        byte = mpz_get_ui(m) % 256;  // Extraire le dernier octet (caractère)
        msg[i++] = (char)byte;  // Stocker cet octet comme caractère
        mpz_div_ui(m, m, 256);  // Right Shift (diviser par 2^8)
    }
    
    msg[i] = '\0';  // End of string
    
    // Inverser la chaîne pour restaurer l'ordre correct des caractères
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = msg[start];
        msg[start] = msg[end];
        msg[end] = temp;

        start++;
        end--;
    }
}
