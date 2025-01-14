#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include <prime_utils.h>
#include <math_utils.h>
#include <gmp.h>


int main() {
	mpz_t p,q,n,phi,a,inv;
	mpz_inits(p,q,n,phi,a,inv,NULL);

	get_prime(p,256);
	get_prime(q,256);

	mpz_mul(n,p,q);

	calculate_phi(phi,p,q);

	mpz_set_ui(a,16549);

	modular_inverse(inv,a,phi);

	gmp_printf("\n[+] Test : Géneration des premier \n\np = %Zd\nq = %Zd\nn = %Zd",p,q,n);
	gmp_printf("\n\n[+] Test de L'inverse modulaire \n\ninverse of %Zd mod n = %Zd",a,inv);
	mpz_clears(p,q,n,phi,a,inv,NULL);
    return 0;
}