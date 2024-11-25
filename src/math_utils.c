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

void rsa_mod_ui(mpz_t r, mpz_t d, unsigned long int n){

	mpz_t temp;
	mpz_init(temp);

	mpz_fdiv_q_ui(temp,d,n);
	mpz_mul_ui(temp,temp,n);
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
void rsa_powm(mpz_t rop, mpz_t base, mpz_t exp,mpz_t modulo){

	mpz_t a;
	mpz_init(a);

	int l = (int)mpz_sizeinbase(exp,2); // Calculer le nombre des bits de la base 
	mpz_set_str(a,"1",10); // Initialiser a à 1

	for(int i=l-1;i>=0;i--){
		mpz_mul(a,a,a);
		rsa_mod(a,a,modulo);

		if(mpz_tstbit(exp,i)){
			mpz_mul(a,a,base);
			rsa_mod(a,a,modulo);
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

int main(){
	// Pour tester les fonctions
	mpz_t a,b,c,d;
	mpz_inits(a,b,c,d,NULL);
	mpz_set_str(a,"51",10);
	mpz_set_str(b,"5",10);
	mpz_set_str(c,"181",10);
	
	rsa_mod(d,c,a);
	gmp_printf("\n Operation Modulo : \n 181 % 51 = %Zd",d);

	rsa_mod_ui(d,c,76);
	gmp_printf("\n Operation Modulo ui : \n 181 % 76 = %Zd",d);

	rsa_powm(d,a,b,c);
	gmp_printf("\n Operation exponentiation Modulo : \n pow(51,5,181) = %Zd",d);

	rsa_powm_ui(d,a,5,c);
	gmp_printf("\n Operation exponentiation Modulo ui : \n pow(51,5,181) = %Zd",d);

	mpz_clears(a,b,c,d,NULL);
}