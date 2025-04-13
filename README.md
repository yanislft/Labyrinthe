# Labyrinthe – Mini-Projet L1 Informatique

## Objectif du projet

Ce projet a pour but de concevoir et d’implémenter un labyrinthe interactif comportant plusieurs fonctionnalités, notamment :

- La génération aléatoire de labyrinthes parfaits (chaque point est relié par un chemin unique),
- L'exploration manuelle ou automatique d’un chemin optimal,
- Une interaction en temps réel avec l'utilisateur à travers une interface en mode texte.

## Fonctionnalités principales

### Génération de labyrinthe

- Génération dynamique d’un labyrinthe parfait à partir d’une grille de dimensions p × q.
- Utilisation d’un algorithme basé sur une pile pour construire le labyrinthe en marquant les cases visitées.

### Parcours et résolution

- Mode manuel : l’utilisateur peut se déplacer à l’aide des flèches directionnelles.
- Mode automatique : recherche d’un chemin optimal entre l’entrée (0, 0) et la sortie (p-1, q-1) à l’aide d’un algorithme de parcours en profondeur (DFS).

### Affichage et interaction

- Visualisation graphique du labyrinthe à l’aide de la bibliothèque `ncurses`.
- Affichage des chemins explorés et des solutions disponibles.

## Structure du projet

- `votre_prog.cpp` : Fichier principal contenant la logique de lecture et d’exécution.
- `labIO.cpp` / `labIO.h` : Fonctions d'entrée/sortie liées au labyrinthe.
- `lab.h` : Définitions des structures de données (labyrinthe, pile, etc.).
- `laby.txt` : Exemple de fichier de configuration pour un labyrinthe.
- `output/` : Dossier contenant les résultats générés (labyrinthes explorés, par exemple).

## Fonctionnalité de jeu interactif

Le programme permet de :

- Générer un labyrinthe aléatoire,
- Explorer le labyrinthe en mode manuel,
- Afficher automatiquement une solution si l’utilisateur abandonne (touche F1).

## Configuration de l’environnement

- Langage utilisé : C++
- Bibliothèque utilisée : `ncurses` (pour l’affichage terminal)
- Commande de compilation :
  ```
  g++ votre_prog.cpp labIO.cpp -o votre_prog -lncurses
  ```

## Exemple d’utilisation

L’utilisateur lance le programme, choisit la taille du labyrinthe, puis peut :
- Le parcourir manuellement avec les flèches directionnelles,
- Ou lancer la résolution automatique,
- Visualiser les chemins explorés ainsi que les solutions trouvées.

## Auteur

Yanis Laftimi
Ce projet a été réalisé dans le cadre du cours de Bases de la programmation – Licence 1 Informatique.
