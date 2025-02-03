# Implémentation Complète de l'Algorithme RSA en C avec la Bibliothèque GMP

Ce projet est une implémentation complète de l'algorithme RSA en C, utilisant la bibliothèque GMP (GNU Multiple Precision Arithmetic Library). Il comprend la génération des clés, le chiffrement, le déchiffrement, la signature numérique et la vérification.

## Structure du Projet
_rsa_project/  
├── src/  _(Contient les fichiers source (.c))_  
│   ├── main.c _(Point d’entrée du programme)_  
│   ├── rsa.c _(Implémentation des fonctions liées à RSA)_  
│   ├── test.c _(Tests des fonctionnalités)_  
│   ├── math_utils.c _(Fonctions mathématiques : GCD, exponentiation modulaire)_  
│   ├── prime_utils.c _(Fonctions de génération et de vérification des nombres premiers)_  
├── include/  _(Headers pour abstraction matérielle)_  
│   ├── rsa.h _(Déclarations des fonctions RSA)_  
│   ├── math_utils.h _(Déclarations des fonctions utilitaires mathématiques)_  
│   ├── prime_utils.h _(Déclarations des fonctions liées aux nombres premiers)_  
├── Makefile _(Script de compilation)_  
└── README.md _(Documentation et workflow CI/CD)_  

## Fonctionnalités Implémentées

### 1️⃣ Génération de Clés RSA

- **Fonction : `generate_keys()`**
  - Génère une paire de clés RSA (publique et privée) avec des nombres premiers sécurisés.

### 2️⃣ Chiffrement et Déchiffrement

- **Fonctions : `encrypt()` et `decrypt()`**
  - **Chiffrement RSA** : Convertit un message clair en texte chiffré.
  - **Déchiffrement RSA** : Récupère le message clair à partir du texte chiffré.
  - **Déchiffrement CRT** : Optimisé avec le théorème des restes chinois (`decrypt_crt()`).

### 3️⃣ Signature Numérique et Vérification

- **Fonctions : `sign()` et `verify()`**
  - **Signature RSA** : Génère une signature numérique pour garantir l'authenticité.
  - **Vérification** : Confirme si une signature est valide ou non.

### 4️⃣ Optimisation : Génération de Nombres Premiers Sécurisée

- **Fonction : `get_prime()`**
  - **Miller-Rabin** : Test de primalité probabiliste.
  - **Élimination des nombres divisibles par les petits nombres premiers** avant Miller-Rabin pour accélérer la génération.

### 5️⃣ Optimisation : Exponentiation Modulaire Sécurisée

- **Fonction : `rsa_powm()`**
  - **Square-and-Multiply Always** : Résiste aux attaques SPA (Simple Power Analysis).
  - **Temps constant** : Empêche les attaques par observation temporelle.
  - **Protège l’exposant `d`** pendant l'exponentiation.
