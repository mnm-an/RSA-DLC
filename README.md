# Implémentation Complète de l'Algorithme RSA en C avec la Bibliothèque GMP

Ce projet est une implémentation complète de l'algorithme RSA en C, utilisant la bibliothèque GMP (GNU Multiple Precision Arithmetic Library). Il comprend la génération des clés, le chiffrement, le déchiffrement, la signature numérique et la vérification.

# 📁 Structure du Projet

Le projet **RSA** est organisé comme suit :

rsa_project/ ├── 📂 src/ # Contient les fichiers source (.c) │ ├── 📄 main.c # Point d'entrée du programme (tester les fonctionnalités) │ ├── 📄 rsa.c # Implémentation des fonctions liées à RSA │ ├── 📄 math_utils.c # Fonctions utilitaires mathématiques (ex: GCD, modular exponentiation) │ └── 📄 prime_utils.c # Fonctions de génération et de vérification des nombres premiers ├── 📂 include/ # Contient les fichiers d'en-tête (.h) │ ├── 📄 rsa.h # Déclarations des fonctions RSA │ ├── 📄 math_utils.h # Déclarations des fonctions utilitaires mathématiques │ └── 📄 prime_utils.h # Déclarations des fonctions liées aux nombres premiers ├── 📜 Makefile # Script de compilation └── 📜 README.md # Documentation du projet


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
