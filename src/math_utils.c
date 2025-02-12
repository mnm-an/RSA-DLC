#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "prime_utils.h"

// modulo normal
void rsa_mod(mpz_t r, mpz_t d, mpz_t n){

	mpz_t temp;
	mpz_init(temp);

	mpz_fdiv_q(temp,d,n);
	mpz_mul(temp,temp,n);
	mpz_sub(r,d,temp);

}

// rop = base^(exp)
void rsa_pow_ui(mpz_t rop, mpz_t base, unsigned long int exp){

    mpz_set_ui(rop, 1); // Initialiser rop à 1 (base^0 = 1)
    mpz_t temp_base;
    mpz_init_set(temp_base, base); // Copie de base pour manipulation

    while (exp > 0) {
        if (exp % 2 == 1) { // Si le bit courant de exp est 1
            mpz_mul(rop, rop, temp_base); // rop *= temp_base
        }
        mpz_mul(temp_base, temp_base, temp_base); // temp_base = temp_base^2
        exp /= 2; // Décaler exp vers la droite (division par 2)
    }

    mpz_clear(temp_base);

} 

void rsa_ui_pow_ui(mpz_t rop, unsigned long int base, unsigned long int exp) {
    mpz_t mpz_base;
    mpz_init_set_ui(mpz_base, base); 
    rsa_pow_ui(rop, mpz_base, exp);  
    mpz_clear(mpz_base);            
}

// Exponentiation modulaire
void rsa_powm(mpz_t rop, mpz_t base, mpz_t exp, mpz_t modulo) {
    mpz_t a, b;
    // Initialisation de a et b à 1
    mpz_inits(a, b, NULL);
    mpz_set_str(a, "1", 10);
    mpz_set_str(b, "1", 10);

    int l = (int)mpz_sizeinbase(exp, 2); // Nombre de bits dans exp

    for (int i = l - 1; i >= 0; i--) {
        // Élever a au carré et réduire modulo modulo
        mpz_mul(a, a, a);
        rsa_mod(a, a, modulo); // a = a^2 mod modulo

        if (mpz_tstbit(exp, i)) {
            // Si le bit est 1 : multiplication effective par base
            mpz_mul(a, a, base);
            rsa_mod(a, a, modulo); // a = (a * base) mod modulo
        } else {
            // Si le bit est 0 : multiplication fictive sur b
            mpz_mul(b, a, base);
            rsa_mod(b, b, modulo); // Opération dummy pour masquer la branche
        }
    }
    mpz_set(rop, a);
    mpz_clears(a, b, NULL);
}

void rsa_powm_ui(mpz_t rop, mpz_t base, unsigned long int exp,mpz_t modulo){

	mpz_t a,mpz_exp;
	mpz_inits(a,mpz_exp,NULL);

	mpz_set_ui(mpz_exp,exp);
	rsa_powm(rop,base,mpz_exp,modulo);
	mpz_clears(a,mpz_exp,NULL);

} 

void extended_euclide(mpz_t gcd, mpz_t u, mpz_t v, mpz_t a, mpz_t b) {
    mpz_t tmp_a, tmp_b, tmp_u, tmp_v, q, r, tmp_q;
    mpz_inits(tmp_a, tmp_b, tmp_u, tmp_v, q, r, tmp_q, NULL);

    mpz_set(tmp_a, a); // Copie a
    mpz_set(tmp_b, b); // Copie b

    mpz_set_ui(u, 1);
    mpz_set_ui(v, 0);
    mpz_set_ui(tmp_u, 0);
    mpz_set_ui(tmp_v, 1);

    while (mpz_cmp_ui(tmp_b, 0) != 0) {
        mpz_fdiv_q(q, tmp_a, tmp_b); // q = tmp_a // tmp_b
        mpz_set(r, tmp_b);           // r = tmp_b
        mpz_mod(tmp_b, tmp_a, tmp_b); // tmp_b = tmp_a % tmp_b
        mpz_set(tmp_a, r);           // tmp_a = r

        // Update u and tmp_u
        mpz_set(r, tmp_u);          // r = tmp_u
        mpz_mul(tmp_q, q, tmp_u);   // tmp_q = q * tmp_u
        mpz_sub(tmp_u, u, tmp_q);   // tmp_u = u - tmp_q

        mpz_set(u, r);              // u = r  l'inverse de a

        // Update v and tmp_v
        mpz_set(r, tmp_v);          // r = tmp_v
        mpz_mul(tmp_q, q, tmp_v);   // tmp_q = q * tmp_v
        mpz_sub(tmp_v, v, tmp_q);   // tmp_v = v - tmp_q

        mpz_set(v, r);              // v = r  l'inverse de b
    }

    mpz_set(gcd, tmp_a); // gcd = tmp_a  (a*u + b*v = gcd)

    mpz_clears(tmp_a, tmp_b, tmp_u, tmp_v, q, r, tmp_q, NULL);
}

void modular_inverse(mpz_t result,mpz_t a,mpz_t phi){
	mpz_t inv,k,gcd;
	mpz_inits(inv,k,gcd,NULL);

	// Calculer les termes de bezout
	extended_euclide(gcd, inv, k, a, phi);

	if(mpz_cmp_ui(gcd,1)==0){
		mpz_set(result,inv);
		mpz_mod(result, inv, phi);

	}else{
		// Si le pgcd différent de 1
		mpz_set_ui(result,0);
		return;
	}

	mpz_clears(inv,k,gcd,NULL);
}

void calculate_phi(mpz_t phi,mpz_t p,mpz_t q){

	mpz_t tmp_p,tmp_q;
	mpz_inits(tmp_p,tmp_q,NULL);

	mpz_sub_ui(tmp_p,p,1);
	mpz_sub_ui(tmp_q,q,1);

	mpz_mul(phi,tmp_p,tmp_q);

	mpz_clears(tmp_p,tmp_q,NULL);
}
