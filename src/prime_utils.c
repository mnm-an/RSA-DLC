#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include "math_utils.h"

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
		return 1;
	}
	for(int i=1;i<s;i++){
		rsa_powm_ui(res,res,2,n);
		if(mpz_cmp(res,phi)==0){
			mpz_clears(r, res, phi, NULL);
			return 1;
		}
	}

	mpz_clears(r, res, phi, NULL);
	return 0; // Probablement Premier
}

int is_prime(mpz_t n,int t){
	mpz_t rand;
	mpz_init(rand);

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
			return 0;
		}
	}

	mpz_clear(rand);
	gmp_randclear(grain);

	return 1;
}

void get_prime(mpz_t prime, int k){                   
    mpz_t inf; // Variable pour stocker 2^(k-1)
    mpz_init(inf);

    gmp_randstate_t grain; // État aléatoire GMP
    gmp_randinit_default(grain); // Initialisation de l'état aléatoire
    unsigned long seed = time(NULL) + rand(); // Génération d'une graine unique basée sur le temps avec un rand
    gmp_randseed_ui(grain, seed); // Initialisation de l'état aléatoire avec la graine

    // Calcul de 2^(k-1) pour garantir que les nombres générés sont de k bits
    rsa_ui_pow_ui(inf, 2, k - 1);

    // Boucle pour générer un nombre premier
    while (1) {
        // Génération d'un nombre aléatoire < 2^(k-1)
        mpz_urandomb(prime, grain, k - 1);

        // Ajout de 2^(k-1) pour garantir que le nombre est de k bits
        mpz_add(prime, prime, inf);

        // Vérification de la primalité
        if (is_prime(prime, 15)) {
            break; // Si le nombre est premier, on sort de la boucle
        }

        // Réinitialisation de la variable `prime` pour libérer la mémoire
        mpz_set_ui(prime, 0);
    }
    gmp_randclear(grain);
    mpz_clear(inf);
}

