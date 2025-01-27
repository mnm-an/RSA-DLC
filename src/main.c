#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <rsa.h>
#include <math_utils.h>
#include <prime_utils.h>
#include <time.h>

int main() {
	/**

    printf("\n[+] ---------------- Test des fonctions de base ---------------------[+]\n");
    mpz_t a, b, c, d;
    mpz_inits(a, b, c, d, NULL);
    mpz_set_str(a, "51", 10);
    mpz_set_str(b, "5", 10);
    mpz_set_str(c, "181", 10);

    rsa_mod(d, c, a);
    printf("\n[-]--------------------------------------------------[-]\n");
    printf("Test de quelques Foncionnalités :\n");
    gmp_printf("\nOperation Modulo : \n 181 %% 51 = %Zd", d);

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
    printf("\n[-]--------------------------------------------------[-]\n");
    printf("Test de chiffrement et déchiffrement :\n");
    gmp_printf("\nMessage chiffré : %Zd\n", ch);

    // Déchiffrement standard
    decrypt(m, ch, de, n);
    gmp_printf("\nMessage déchiffré en mode standard: %Zd\n", m);

    // Déchiffrement CRT
    decrypt_crt(m, ch, de, p, q);
    gmp_printf("\nMessage déchiffré en mode CRT: %Zd\n", m);

    printf("\n[-]--------------------------------------------------[-]\n");
    printf("Test de signature :\n");

    // Signature
    sign(signature, m, de, n);
    gmp_printf("\nSignature générée : %Zd\n", signature);

    // Vérification de la signature
    int is_valid = verify(signature, m, e, n);
    if (is_valid)
    {
        printf("\nLa signature est valide.\n");
    }
    else
    {
        printf("La signature est invalide.\n");
    }

    mpz_clears(m, e, n, de, ch, p, q, signature, NULL);

    printf("\n[+] ---------------- Test des géneration des clés ---------------------[+]\n");

 	mpz_t priv_p,priv_q,pub_n,phi,pub_e,inv;
 	mpz_inits(priv_p,priv_q,pub_n,phi,pub_e,inv,NULL);

 	get_prime(priv_p,256);
 	get_prime(priv_q,256);

 	mpz_mul(pub_n,priv_p,priv_q);

 	calculate_phi(phi,priv_q,priv_p);

 	mpz_set_ui(pub_e,65537);

 	modular_inverse(inv,pub_e,phi);

 	gmp_printf("\nTest : Géneration des premier \n\np = %Zd (256 bits)\nq = %Zd (256 bits)\nn = %Zd (512 bits)",priv_p,priv_q,pub_n);
	gmp_printf("\n\n[+] Test de L'inverse modulaire \n\ninverse of %Zd mod n = %Zd",pub_e,inv);
	**/
 	mpz_t priv_p,priv_q,pub_n,phi,pub_e,inv;
 	mpz_inits(priv_p,priv_q,pub_n,phi,pub_e,inv,NULL);

	int time_f = time(NULL);

	get_prime(priv_p,1024);

	int time_l = time(NULL);

 	gmp_printf("\n\n[+] Géneration de premier de 1024 bits : \n\np = %Zd (1024 bits)",priv_p);

  	printf("\ntime : %d sec",time_l-time_f);

	mpz_clears(priv_p,priv_q,pub_n,phi,pub_e,inv,NULL);

    return 0;
}
