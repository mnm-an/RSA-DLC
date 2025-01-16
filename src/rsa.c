void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    // Effectuer l'exponentiation modulaire pour chiffrer le message
    rsa_powm(c, m, e, n);
}

void decrypt_standard(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    // Déchiffrement : m = c^d mod n
    rsa_powm(m, c, d, n);
}

void decrypt_crt(mpz_t m, mpz_t c, mpz_t d, mpz_t p, mpz_t q) {
    mpz_t dp, dq, qinv, m1, m2, h, temp1, temp2;
    mpz_inits(dp, dq, qinv, m1, m2, h, temp1, temp2, NULL);

    // Calcul de dp = d mod (p-1) et dq = d mod (q-1)
    mpz_sub_ui(temp1, p, 1);
    rsa_mod(dp, d, temp1);

    mpz_sub_ui(temp1, q, 1);
    rsa_mod(dq, d, temp1);

    // Calcul de qinv (inverse de q mod p)
    mpz_invert(qinv, q, p);

    // Calcul de m1 = c^dp mod p et m2 = c^dq mod q
    rsa_powm(m1, c, dp, p);
    rsa_powm(m2, c, dq, q);

    // h = qinv * (m1 - m2) mod p
    mpz_sub(h, m1, m2);
    rsa_mod(h, h, p); // Assurer que (m1 - m2) est positif
    mpz_mul(h, h, qinv);
    rsa_mod(h, h, p);

    // Calcul final m = m2 + h * q
    mpz_mul(temp1, h, q);
    mpz_add(m, m2, temp1);

    // Libération des variables temporaires
    mpz_clears(dp, dq, qinv, m1, m2, h, temp1, temp2, NULL);
}

void sign(mpz_t signature, const mpz_t message, const mpz_t d, const mpz_t n){
    // Signature RSA 
    rsa_powm(signature, m, d, n);
}

int verify(const mpz_t message, const mpz_t signature, const mpz_t e, const mpz_t n) {
    mpz_t tmp;
    mpz_init(tmp);

    // Compute m' = signature^e mod n
    rsa_powm(tmp, signature, e, n);

    // Compare m' with the original message
    int result = (mpz_cmp(tmp, message) == 0);

    mpz_clear(tmp); // Free allocated memory
    return result; // Return 1 if valid, 0 otherwise
}
