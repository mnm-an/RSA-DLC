#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>


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

// Exponentiation modulaire square and multiply always
void rsa_powm(mpz_t rop, mpz_t base, mpz_t exp,mpz_t modulo){

	mpz_t a;
	mpz_inits(a,b,NULL);

	int l = (int)mpz_sizeinbase(exp,2); // Calculer le nombre des bits de la base 
	mpz_set_str(a,"1",10); // Initialiser a à 1
	mpz_set_str(b,"1",10); // Initialiser b à 1

	for(int i=l-1;i>=0;i--){
		mpz_mul(a,a,a);
		rsa_mod(a,a,modulo); // a^2 mod n

		if(mpz_tstbit(exp,i)){
			mpz_mul(a,a,base);
			rsa_mod(a,a,modulo); // a*m mod n
		}else{
			mpz_mul(b,a,base);
			rsa_mod(b,a,modulo); // b*m mod n   square and multiply always
		}
	}
	mpz_set(rop,a);
	mpz_clear(a);

}

void rsa_powm_ui(mpz_t rop, mpz_t base, unsigned long int exp,mpz_t modulo){

	mpz_t a,mpz_exp;
	mpz_inits(a,mpz_exp,NULL);

	mpz_set_ui(mpz_exp,exp);
	rsa_powm(rop,base,mpz_exp,modulo);
	mpz_clears(a,mpz_exp,NULL);

} 

void modular_inverse(mpz_t result, mpz_t a, mpz_t phi) {
    mpz_t r, r_prev, t, t_prev, q, temp, temp2;
    mpz_inits(r, r_prev, t, t_prev, q, temp, temp2, NULL);

    mpz_set(r, phi);        // r = phi
    mpz_set(r_prev, a);   // r_prev = a
    mpz_set_ui(t, 0);     // t = 0
    mpz_set_ui(t_prev, 1); // t_prev = 1

    while (mpz_cmp_ui(r, 0) != 0) {
        // Calcul de q = r_prev / r
        mpz_div(q, r_prev, r);

        // Mise à jour r
        mpz_mul(temp, q, r);
        mpz_sub(temp2, r_prev, temp);
        mpz_set(r_prev, r);
        mpz_set(r, temp2);

        // Mise à jour t
        mpz_mul(temp, q, t);
        mpz_sub(temp2, t_prev, temp);
        mpz_set(t_prev, t);
        mpz_set(t, temp2);
    }

    // Si r_prev != 1, l'inverse modulaire n'existe pas
    if (mpz_cmp_ui(r_prev, 1) != 0) {
        mpz_set_ui(result, 0); // Pas d'inverse modulaire
    } else {
        // Si t_prev est négatif, on l'ajuste
        if (mpz_sgn(t_prev) < 0) {
            mpz_add(t_prev, t_prev, phi);
        }
        mpz_set(result, t_prev);
    }

    mpz_clears(r, r_prev, t, t_prev, q, temp, temp2, NULL);
}

void calculate_phi(mpz_t phi, mpz_t p, mpz_t q) {
    mpz_t p_minus_1, q_minus_1;
    mpz_inits(p_minus_1, q_minus_1, NULL);

    // Calculer p-1 et q-1
    mpz_sub_ui(p_minus_1, p, 1); // p_minus_1 = p - 1
    mpz_sub_ui(q_minus_1, q, 1); // q_minus_1 = q - 1

    // Calculer phi = (p-1) * (q-1)
    mpz_mul(phi, p_minus_1, q_minus_1);

    mpz_clears(p_minus_1, q_minus_1, NULL);
}
