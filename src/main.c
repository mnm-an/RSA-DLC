#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <rsa.h>
#include <math_utils.h>
#include <prime_utils.h>
#include <time.h>

int main() {
    printf("\n[+] --------------- TEST DU PROJET RSA ---------------- [+]\n");
    /*** Test des fonctions RSA ***/
    printf("\n[+] --------------- Test du chiffrement et déchiffrement ---------------- [+]\n");

    mpz_t m, e, n,phi, d_priv, ch, p, q, signature;
    mpz_inits(m, e, n,phi, d_priv, ch, p, q, signature, NULL);

    // Génération des clés
    get_prime(p, 1024);  // p de 1024 bits
    get_prime(q, 1024);  // q de 1024 bits
    mpz_mul(n, p, q);    // n = p * q
    calculate_phi(phi, p, q); // phi(n) = (p-1) * (q-1)
    mpz_set_ui(e, 65537); // Exposant public
    modular_inverse(d_priv, e, phi); // Calcul de d = e^(-1) mod phi(n)

    // Affichage des clés
    printf("\n[+] Clés générées :\n");
    gmp_printf(" - p = %Zd\n - q = %Zd\n - n = %Zd\n - e = %Zd\n - d = %Zd\n", p, q, n, e, d_priv);

    // Chiffrement d'un message
    mpz_set_str(m, "42", 10); // Message clair
    encrypt(ch, m, e, n);
    gmp_printf("\nMessage chiffré : %Zd\n", ch);

    // Déchiffrement standard
    decrypt(m, ch, d_priv, n);
    gmp_printf("\nMessage déchiffré (standard) : %Zd\n", m);

    // Déchiffrement avec CRT
    decrypt_crt(m, ch, d_priv, p, q);
    gmp_printf("\nMessage déchiffré (CRT) : %Zd\n", m);

    /*** Test de signature et vérification ***/
    printf("\n[+] --------------- Test de signature et vérification ---------------- [+]\n");

    // Signature du message
    sign(signature, m, d_priv, n);
    gmp_printf("\nSignature générée : %Zd\n", signature);

    // Vérification de la signature
    int valid = verify(signature, m, e, n);
    printf("\nRésultat de la vérification : %s\n", valid ? "VALIDÉE ✅" : "INVALIDE ❌");

    // Libération de la mémoire
    mpz_clears(m, e, n, phi,d_priv, ch, p, q, signature, NULL);

    printf("\n[+] ---------------- FIN DES TESTS ---------------- [+]\n");

    return 0;
}
