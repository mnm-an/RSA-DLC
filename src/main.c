#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "rsa.h"
#include <math_utils.h>
#include <prime_utils.h>
#include <time.h>

int main() {
    printf("\n[+] --------------- TEST DU PROJET RSA ---------------- [+]\n");
    /*** Test des fonctions RSA ***/


    mpz_t m, e, n,phi, d_priv, ch, p, q, signature;
    mpz_inits(m, e, n,phi, d_priv, ch, p, q, signature, NULL);

    printf("\n[+] --------------- Génération des clés RSA (1024 bits) ---------------- [+]\n");
    // Génération des clés

    generate_keys(n,e,d_priv,p,q,1024);

    // Affichage des clés
    printf("\n[+] Clés générées :\n\n");
    gmp_printf(" - p = %Zd\n - q = %Zd\n - n = %Zd\n - e = %Zd\n - d = %Zd\n", p, q, n, e, d_priv);

     /*** Test de conversion STRING ↔ MPZ ***/
    printf("\n[+] --------------- Test de conversion STRING ↔ MPZ ---------------- [+]\n");

    char msg_original[] = "Hello RSA!";
    char msg_recovered[100];

    // Convertir le message texte en nombre mpz
    str_to_mpz(m, msg_original);
    gmp_printf("[✓] Message converti en nombre (mpz) : %Zd\n", m);

    /*** Test du chiffrement et déchiffrement ***/
    printf("\n[+] --------------- Test du chiffrement et déchiffrement ---------------- [+]\n");

    encrypt(ch, m, e, n);
    gmp_printf("[✓] Message chiffré : %Zd\n", ch);

    // Déchiffrement standard
    decrypt(m, ch, d_priv, n);
    gmp_printf("[✓] Message déchiffré (standard, mpz) : %Zd\n", m);

    // Conversion mpz -> string
    mpz_to_str(m, msg_recovered);
    printf("[✓] Message déchiffré (récupéré en texte) : %s\n", msg_recovered);

    // Déchiffrement avec CRT
    decrypt_crt(m, ch, d_priv, p, q);
    gmp_printf("[✓] Message déchiffré (CRT, mpz) : %Zd\n", m);

    // Conversion mpz -> string après déchiffrement CRT
    mpz_to_str(m, msg_recovered);
    printf("[✓] Message déchiffré (CRT, récupéré en texte) : %s\n", msg_recovered);

    /*** Test de signature et vérification ***/
    printf("\n[+] --------------- Test de signature et vérification ---------------- [+]\n");

    // Signature du message
    mpz_set_str(m,"4562",10);
    sign(signature, m, d_priv, n);
    gmp_printf("\nSignature générée : %Zd\n", signature);

    // Vérification de la signature
    int valid = verify(signature, m, e, n);
    printf("\nRésultat de la vérification : %s\n", valid ? "VALIDÉE [V]" : "INVALIDE [X]");

    // Libération de la mémoire
    mpz_clears(m, e, n, phi,d_priv, ch, p, q, signature, NULL);

    printf("\n[+] ---------------- FIN DES TESTS ---------------- [+]\n");

    return 0;
}
