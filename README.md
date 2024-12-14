# Projet de Système d'Exploitation

## Description du Projet

Ce projet consiste à développer un système d'exploitation minimaliste avec des fonctionnalités essentielles telles que l'affichage, la gestion de timer, l'ordonnancement, et la communication via UART. 

### Fonctionnalités Réalisées
- **Affichage** : Mise en place d'un système de gestion de l'affichage en mode texte, incluant le support des caractères ASCII.
- **Timer** : Configuration et gestion d'interruptions pour afficher une horloge au format `HH:MM:SS` en haut de l'écran.
- **UART** : Implémentation d'une interface de communication UART pour les transmissions RX/TX.
- **Ordonnancement** : Un mécanisme d'ordonnancement minimal a été développé.

### Extensions Réalisées/Tests
- **UART/Clavier** : Tentative de développement de la fonction `int getchar()` pour capturer les entrées clavier avec gestion des interruptions RX et TX. Cependant, des interférences ont été rencontrées entre le timer et les interruptions clavier.
- **Tests** : Création de fonctions de test pour :
  - **Timer** : Valider le fonctionnement de l'horloge et des interruptions.
  - **Affichage** : Vérifier l'affichage des caractères et la gestion du curseur.
  - **Ordonnancement** : Tester les mécanismes de gestion des processus.

**Note importante** : Pour tester uniquement le timer, commentez l'appel à `ordonnance()` dans le `trap_handler` du timer.

---

## Extensions Futures

Je prévois d'améliorer et d'étendre ce projet avec :
1. **Ordonnancement par Priorité** : Développer un mécanisme d'ordonnancement plus avancé pour une gestion optimisée des processus.
2. **Mini-shell** : Implémenter une interface en ligne de commande pour interagir avec le système.
3. **Système de Fichiers** : Découvrir et intégrer les notions de gestion des fichiers dans un système d'exploitation.


