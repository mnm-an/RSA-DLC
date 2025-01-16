// #include <stdio.h>
// #include <gmp.h>
// #include <stdlib.h>
// #include <time.h>
// #include <../include/prime_utils.h>
// #include <math_utils.h>
// #include <gmp.h>


int main() {
// 	mpz_t p,q,n,phi,a,inv;
// 	mpz_inits(p,q,n,phi,a,inv,NULL);

// 	get_prime(p,256);
// 	get_prime(q,256);

// 	mpz_mul(n,p,q);

// 	calculate_phi(phi,p,q);

// 	mpz_set_ui(a,16549);

// 	modular_inverse(inv,a,phi);

// 	gmp_printf("\n[+] Test : Géneration des premier \n\np = %Zd\nq = %Zd\nn = %Zd",p,q,n);
// 	gmp_printf("\n\n[+] Test de L'inverse modulaire \n\ninverse of %Zd mod n = %Zd",a,inv);
// 	mpz_clears(p,q,n,phi,a,inv,NULL);
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
