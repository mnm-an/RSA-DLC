#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include "math_utils.h"

// Liste de petits nombres premiers pour un test rapide
const unsigned long SMALL_PRIMES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283};
#define SMALL_PRIME_COUNT (sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]))


int is_prime_base(mpz_t n,mpz_t a){
	int s=0;
	mpz_t res,phi,r;

	mpz_inits(r,res,phi,NULL);

	mpz_sub_ui(phi,n,1);
	mpz_set(r, phi);

	//Ecrire phi sous la forme 2^s.r
	while(mpz_tstbit(r,0)==0){
		mpz_divexact_ui(r,r,2);
		s++;
	}

	rsa_powm(res,a,r,n);

	if(mpz_cmp_ui(res,1)==0){
		mpz_clears(r, res, phi, NULL);
		return 1; // Probablement premier
	}

	for(int i=1;i<s;i++){
		rsa_powm_ui(res,res,2,n);
		if(mpz_cmp(res,phi)==0){
			mpz_clears(r, res, phi, NULL);
			return 1; // Probablement premier
		}
	}

	mpz_clears(r, res, phi, NULL);
	return 0; // Composé
}


// Vérifie si n est divisible par un petit premier
int quick_prime_check(mpz_t n) {
    for (size_t i = 1; i < SMALL_PRIME_COUNT; i++) {  // Commence à 1 pour éviter 2 (on génère toujours impair)
        if (mpz_divisible_ui_p(n, SMALL_PRIMES[i])) {
            return 0;  // Pas premier
        }
    }
    return 1;
}

int is_prime(mpz_t n,int t){
	mpz_t rand;
	mpz_init(rand);
	if (!quick_prime_check(n)) return 0;

	gmp_randstate_t grain;
	gmp_randinit_default(grain);
	gmp_randseed_ui(grain,time(NULL));

	for(int i=0;i<t;i++){
		do{
			mpz_urandomm(rand,grain,n);
		}while(mpz_cmp_ui(rand, 2) < 0);

		if(is_prime_base(n,rand)==0){
			mpz_clear(rand);
			gmp_randclear(grain);
			return 0; // pas premier
		}
	}

	mpz_clear(rand);
	gmp_randclear(grain);

	return 1;
}

void get_prime(mpz_t prime, int k) {                   
    mpz_t min_val; // Variable pour stocker 2^(k-1)
    mpz_init(min_val);

    gmp_randstate_t grain; // État aléatoire GMP
    gmp_randinit_default(grain); // Initialisation de l'état aléatoire
    unsigned long seed = time(NULL) + rand(); // Génération d'une graine unique basée sur le temps avec un rand
    gmp_randseed_ui(grain, seed); // Initialisation de l'état aléatoire avec la graine

    // Calcul de 2^(k-1) pour garantir que les nombres générés sont de k bits
    rsa_ui_pow_ui(min_val, 2, k - 1);

    // Boucle pour générer un nombre premier
    while (1) {
        // Génération d'un nombre aléatoire < 2^(k-1)
        mpz_urandomb(prime, grain, k - 1);

        // Ajout de 2^(k-1) pour garantir que le nombre est de k bits
        mpz_add(prime, prime, min_val);

        // Forcer l'impairité (dernier bit à 1)
        mpz_setbit(prime, 0);

        // Vérification de la primalité
        if (is_prime(prime, 15)) {
            break; // Si le nombre est premier, on sort de la boucle
        }

        // Réinitialisation de la variable `prime` pour libérer la mémoire
        mpz_set_ui(prime, 0);
    }
    gmp_randclear(grain);
    mpz_clear(min_val);
}
