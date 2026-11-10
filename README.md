# Malloc — École 42

Projet développé en **C** dans le cadre du cursus de l'école 42.

L'objectif est de réimplémenter les principales fonctions d'allocation dynamique de la bibliothèque standard C afin de comprendre le fonctionnement de la gestion mémoire à bas niveau.

Ce projet aborde notamment l'allocation de pages mémoire, la gestion des blocs, la réutilisation des espaces libérés et l'organisation des allocations.

## Fonctionnalités

- **`malloc()` :** allocation dynamique de mémoire.
- **`free()` :** libération des blocs précédemment alloués.
- **`realloc()` :** redimensionnement d'une allocation existante.
- **`show_alloc_mem()` :** affichage des informations sur les allocations mémoire.
- Gestion de différentes catégories d'allocations : `TINY`, `SMALL` et `LARGE`.
- Allocation de zones mémoire avec l'appel système `mmap()`.
- Alignement des blocs mémoire sur 16 octets.
- Gestion des blocs et des zones à l'aide de structures chaînées.
- Réutilisation des blocs libérés lorsque leur taille le permet.

## Architecture

L'allocateur repose sur une organisation en zones mémoire, chacune contenant des blocs pouvant être alloués ou libérés.

Les métadonnées associées aux zones et aux blocs permettent de suivre les allocations et de retrouver les espaces disponibles.

Le projet est organisé autour de plusieurs fichiers :

- `malloc.c` : gestion des allocations.
- `free.c` : libération de la mémoire.
- `realloc.c` : redimensionnement des allocations.
- `malloc_utils.c` : fonctions utilitaires de gestion mémoire.
- `show_alloc_mem.c` : visualisation des allocations.
- `test_stress_malloc.c` : programme de tests de l'allocateur.

L'implémentation est construite sous forme de bibliothèque partagée (`.so`).

## Compétences mises en œuvre

- Programmation système en C
- Gestion dynamique de la mémoire
- Manipulation des pointeurs et arithmétique des adresses
- Utilisation de `mmap()` et gestion des zones mémoire
- Structures de données chaînées
- Alignement mémoire et manipulation des tailles
- Création de bibliothèques partagées
- Tests et débogage de bas niveau

## Auteur

Charles-Louis Dutel — [GitHub](https
