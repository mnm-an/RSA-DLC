#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include "../include/math_utils.h"

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

void get_prime(mpz_t prime, int k) {                   
    mpz_t inf;
    mpz_init(inf);

    gmp_randstate_t grain;
    gmp_randinit_default(grain);
    unsigned long seed = time(NULL) + rand();
    gmp_randseed_ui(grain, seed);

    rsa_ui_pow_ui(inf, 2, k - 1); // Calcul 2^(k-1)

    do {
        mpz_urandomb(prime, grain, k - 1); // Générer un nombre aléatoire < 2^(k-1)
        mpz_add(prime, prime, inf);       // Ajouter 2^(k-1) pour obtenir un nombre de k bits
    } while (!is_prime(prime, 25)); // Vérifier si le nombre est probablement premier

    gmp_randclear(grain);
    mpz_clear(inf);
}
