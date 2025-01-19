#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <rsa.h>
#include <math_utils.h>
#include <prime_utils.h>

// Fonction pour lire un fichier texte
int read_message_from_file(const char *filename, mpz_t message) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), file)) {
        mpz_set_str(message, buffer, 10); // Lire le message en tant que nombre
    }
    fclose(file);
    return 1;
}

// Fonction pour écrire un message dans un fichier texte
void write_message_to_file(const char *filename, const mpz_t message) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erreur : Impossible d'écrire dans le fichier %s\n", filename);
        return;
    }
    gmp_fprintf(file, "%Zd\n", message);
    fclose(file);
    printf("Message enregistré dans le fichier %s\n", filename);
}

void decrypt_from_file(const char *filename, mpz_t d, mpz_t n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Lire le contenu chiffré
    char buffer[1024];
    if (!fgets(buffer, sizeof(buffer), file)) {
        perror("Erreur lors de la lecture du fichier");
        fclose(file);
        return;
    }

    fclose(file);

    // Convertir le texte chiffré en nombre
    mpz_t ciphertext, plaintext;
    mpz_inits(ciphertext, plaintext, NULL);
    mpz_set_str(ciphertext, buffer, 10);

    // Déchiffrer
    decrypt(plaintext, ciphertext, d, n);

    // Afficher le résultat déchiffré
    gmp_printf("Message déchiffré : %Zd\n", plaintext);

    mpz_clears(ciphertext, plaintext, NULL);
}

// Fonction principale
int main() {
    mpz_t m, e, n, de, ch, p, q, signature;
    mpz_inits(m, e, n, de, ch, p, q, signature, NULL);

    // Générer les clés RSA
    printf("Génération des clés RSA...\n");
    get_prime(p, 256); // Premier facteur
    get_prime(q, 256); // Second facteur
    mpz_mul(n, p, q);  // Modulus
    mpz_t phi;
    mpz_init(phi);
    calculate_phi(phi, p, q);
    mpz_set_ui(e, 65537);         // Exposant public standard
    modular_inverse(de, e, phi); // Exposant privé

    printf("Clés générées avec succès !\n");
    gmp_printf("Clé publique (n, e) : (%Zd, %Zd)\n", n, e);
    gmp_printf("Clé privée (n, d) : (%Zd, %Zd)\n", n, de);

    int choice;
    char filename[256];

    do {
        printf("\nOptions disponibles :\n");
        printf("1. Saisir un message depuis le clavier\n");
        printf("2. Charger un message depuis un fichier\n");
        printf("3. Enregistrer un message chiffré dans un fichier\n");
        printf("4. Déchiffrer un message chiffré\n");
        printf("5. Déchiffrer un message à partir d'un fichier\n");
        printf("6. Quitter\n");
        printf("Choisissez une option : ");
        scanf("%d", &choice);
        getchar(); // Consommer le caractère de nouvelle ligne

        switch (choice) {
        case 1:
            printf("Entrez un message (nombre) à chiffrer : ");
            char buffer[1024];
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Supprimer le '\n'
            mpz_set_str(m, buffer, 10);
            encrypt(ch, m, e, n);
            gmp_printf("Message chiffré : %Zd\n", ch);
            break;

        case 2:
            printf("Entrez le nom du fichier contenant le message : ");
            scanf("%s", filename);
            if (read_message_from_file(filename, m)) {
                encrypt(ch, m, e, n);
                gmp_printf("Message chiffré : %Zd\n", ch);
            }
            break;

        case 3:
            printf("Entrez le nom du fichier pour enregistrer le message chiffré : ");
            scanf("%s", filename);
            write_message_to_file(filename, ch);
            break;

        case 4:
            printf("Entrez un message chiffré à déchiffrer : ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Supprimer le '\n'
            mpz_set_str(ch, buffer, 10);
            decrypt(m, ch, de, n);
            gmp_printf("Message déchiffré : %Zd\n", m);
            break;

        case 5:
            printf("Entrez le chemin du fichier contenant le message chiffré : ");
            char filepath[256];
            scanf("%s", filepath);

            printf("Entrez l'exposant privé (d) : ");
            gmp_scanf("%Zd", de);

            printf("Entrez le module public (n) : ");
            gmp_scanf("%Zd", n);

            decrypt_from_file(filepath, de, n);
            break;

        case 6:
            printf("Quitter le programme...\n");
            break;

        default:
            printf("Option invalide, veuillez réessayer.\n");
        }
    } while (choice != 6);

    // Libérer la mémoire
    mpz_clears(m, e, n, de, ch, p, q, signature, phi, NULL);

    return 0;
}
