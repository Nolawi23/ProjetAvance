# Projet TSP - Problème du Voyageur de Commerce
## Licence Informatique - Projet Avancé

---

## Membres de l'équipe 11

- **Nolawi GEBREKIRSTOS** 
- **Yoakin HAILSELASSIEA**
- **Anteneh AMDEBERHAN**  
- **Tidiane MICHEL** 

---

## Description du projet

Ce projet implémente plusieurs algorithmes de résolution du **Problème du Voyageur de Commerce (TSP)** en langage C, conformément aux spécifications du sujet de projet avancé.

### Objectifs

- Implémenter différentes méthodes de résolution du TSP (exactes et heuristiques)
- Comparer les performances des différents algorithmes
- Travailler avec le format de fichiers TSPLIB standard
- Valider les résultats avec des scripts Python utilisant la bibliothèque `tsplib95`

### Problème du TSP

Étant donnés n points (villes) répartis sur un plan et les distances les séparant, trouver un chemin de longueur totale minimale qui passe exactement une fois par chaque point et revienne au point de départ.

---

## Structure du projet

```
projet-tsp/
├── README.md                   # Ce fichier
├── Makefile                    # Fichier de compilation
│
├── Headers (.h)
│   ├── tsp.h                   # Structures principales
│   ├── parser.h                # Lecture fichiers TSPLIB
│   ├── distance.h              # Fonctions de distance
│   ├── matrice.h               # Matrice des distances
│   ├── tsp_utils.h             # Utilitaires
│   ├── force_brute.h           # Force brute
│   ├── heuristiques.h          # Heuristiques (NN, RW, 2-opt)
│   └── ga_generique.h          # Algorithme génétique
│
├── Sources (.c)
│   ├── tsp.c                   # Programme principal
│   ├── parser.c                # Lecture TSPLIB
│   ├── distance.c              # Distance EUCL_2D, GEO, ATT
│   ├── matrice.c               # Matrice triangulaire
│   ├── tsp_utils.c             # Fonctions utilitaires
│   ├── force_brute.c           # Algorithme de force brute
│   ├── plus_proche_voisin.c    # Nearest Neighbor
│   ├── marche_aleatoire.c      # Random Walk
│   ├── 2opt.c                  # 2-optimisation
│   └── ga_generique.c          # Algorithme génétique
│
├── Tests Python
│   ├── test_tsp_c.py           # Script de validation
│   ├── tsp_tools.py            # Outils Python
│   └── instances.txt           # Liste des instances de test
│
└── Instances TSPLIB (.tsp)
    ├── att10.tsp
    ├── att48.tsp
    ├── burma14.tsp
    ├── a280.tsp
    └── ...
```

---

## Compilation

### Compilation du projet

```bash
# Compilation standard
make

# Nettoyage
make clean

# Recompilation complète
make rebuild
```

---

## Installation des dépendances

### Dépendances C

Le projet utilise uniquement la bibliothèque standard C et la bibliothèque mathématique :
- `stdio.h`, `stdlib.h`, `string.h`, `stdbool.h`,`unistd.h`
- `math.h` (nécessite l'option `-lm` à la compilation)
- `time.h` pour les mesures de performance
- `signal.h` pour la gestion des interruptions (Ctrl+C)

**Aucune bibliothèque externe n'est nécessaire pour la compilation du programme C.**

### Dépendances Python (pour les tests)

Pour exécuter les scripts de validation Python :

```bash
# Installation avec pip
pip install tsplib95 pyproj matplotlib

```

**Packages Python requis :**
- `tsplib95` : Lecture et validation des fichiers TSPLIB
- `pyproj` : Calculs géométriques pour distances GEO
- `matplotlib` : Visualisation des résultats

---

## Utilisation

### Synopsis

```bash
./tsp -f <fichier.tsp> [-m <méthode> | -c]
```

### Options de ligne de commande

| Option | Description |
|--------|-------------|
| `-h` | Affiche l'aide |
| `-f <fichier>` | Fichier TSPLIB à charger (obligatoire) |
| `-c` | Calcule la tournée canonique [1,2,3,...,n] |
| `-m <méthode>` | Méthode de résolution (voir ci-dessous) |

### Méthodes disponibles

| Méthode | Description | Paramètres |
|---------|-------------|------------|
| `bf` | Force brute (exact) | Aucun |
| `nn` | Plus proche voisin (nearest neighbor) | Aucun |
| `rw` | Marche aléatoire (random walk) | Aucun |
| `2optnn` | 2-opt avec initialisation NN | Aucun |
| `2optrw` | 2-opt avec initialisation RW | Aucun |
| `ga` | Algorithme génétique | Aucun (utilise valeurs par défaut) |
| `gadpx` | Algorithme génétique avec DPX | Aucun (utilise valeurs par défaut) |
| `bft` | test avec et sans matrice de force brute(incompatible avec le script python test_tsp_c.py). A Compiler apart| Aucun |


---

## Méthodes implémentées

### 1. Force Brute (bf)

**Principe** : Énumération exhaustive de toutes les permutations possibles.

**Caractéristiques :**
- Solution optimale garantie
- Complexité : O((n-1)!)
- Utilisable uniquement pour n ≤ 12-13 villes
- Gestion des interruptions (Ctrl+C)
- Calcule aussi la pire tournée

**Optimisations :**
- Fixation de la première ville (division par n)
- Génération de permutations lexicographiques (économie mémoire)
- Matrice de distances précalculée (optionnel)

**Exemple :**
```bash
./tsp -f ../test/att10.tsp -m bf
```

---

### 2. Plus Proche Voisin (nn)

**Principe** : Heuristique gloutonne qui sélectionne à chaque étape la ville non visitée la plus proche.

**Caractéristiques :**
- Complexité : O(n²)
- Solution approchée (généralement 10-30% de l'optimum)
- Très rapide (< 1 seconde pour n=1000)

**Algorithme :**
1. Partir de la ville 1
2. Répéter n-1 fois :
   - Trouver la ville non visitée la plus proche
   - Se déplacer vers cette ville
3. Retourner à la ville de départ

**Exemple :**
```bash
./tsp -f ../test/att48.tsp -m nn
```

---

### 3. Marche Aléatoire (rw)

**Principe** : Sélection aléatoire des villes à visiter.

**Caractéristiques :**
- Complexité : O(n)
- Solution aléatoire (généralement médiocre)
- Utilisé pour initialisation de l'algorithme génétique

**Utilisation typique :**
- Baseline pour comparaison
- Génération de population initiale (AG)
- Diversification

**Exemple :**
```bash
./tsp -f ../test/burma14.tsp -m rw
```

---

### 4. 2-Optimisation (2opt)

**Principe** : Amélioration locale d'une tournée existante en inversant des segments.

**Théorème** : Si deux arêtes d'une tournée se croisent, les décroiser diminue la longueur totale.

**Caractéristiques :**
- Amélioration itérative
- Complexité : O(n²) par itération
- Amélioration typique : 10-40%

**Algorithme :**
1. Partir d'une tournée initiale (NN ou RW)
2. Pour chaque paire d'arêtes :
   - Calculer le gain d'un échange 2-opt
   - Si gain > 0, appliquer l'échange
3. Répéter jusqu'à convergence

**Exemples :**
```bash
# 2-opt avec initialisation NN
./tsp -f ../test/att48.tsp -m 2optnn

# 2-opt avec initialisation RW
./tsp -f ../test/att48.tsp -m 2optrw
```

---

### 5. Algorithme Génétique (ga)

**Principe** : Méta-heuristique inspirée de l'évolution biologique.

**Caractéristiques :**
- Population d'individus (tournées)
- Croisement ordonné (OX)
- Mutation par échange
- Élitisme (conservation du meilleur)

**Paramètres par défaut :**
- Taille de population : 30
- Nombre de générations : 1000
- Taux de mutation : 0.1 (10%)
- Croisements par génération : 15

**Opérateurs génétiques :**

1. **Sélection** : Aléatoire (deux parents)
2. **Croisement ordonné (OX)** :
   - Copier un segment du parent 1
   - Compléter avec parent 2 dans l'ordre
3. **Mutation** : Échange de deux villes
4. **Remplacement** : Le pire individu

**Algorithme :**
```
1. Initialiser population (marche aléatoire)
2. Évaluer la population
3. Pour chaque génération :
   a. Pour nb_croisements fois :
      - Sélectionner 2 parents
      - Créer enfant par croisement OX
      - Muter avec probabilité p
      - Remplacer le pire si meilleur
   b. Mettre à jour meilleur global
   c. Élitisme : insérer meilleur global
4. Retourner meilleur global
```

**Exemple :**
```bash
./tsp -f ../test/att48.tsp -m ga
```

---

## Tests et validation

### Script Python de validation

Le script `test_tsp_c.py` permet de :
- Valider les calculs du programme C
- Comparer avec la bibliothèque `tsplib95`
- Vérifier la validité des tournées
- Mesurer les performances

### Exécution des tests

```bash
# Tester une instance spécifique
python3 test_tsp_c.py

# Tester plusieurs instances
# (modifier instances.txt avec la liste des fichiers)
python3 test_tsp_c.py
```

### Format de sortie

Le programme C produit une sortie normalisée compatible avec le parsing Python :

```
Tour : <instance> <méthode> <temps_cpu> <longueur> [chemin]
```

**Exemple :**
```
Tour : att48 nn 0.000234 33523.000000 [1,9,38,31,44,18,7,28,6,37,...]
```

### Validation automatique

Le script Python vérifie :
1. **Validité de la tournée** : Toutes les villes visitées une fois
2. **Cohérence des longueurs** : C vs Python
3. **Temps d'exécution** : Performance

---

## Performances et résultats

### Résultats sur instances de référence

#### att10 (10 villes)
| Méthode | Distance | Temps |
|---------|----------|-------|
| bf | 6178 | 53.759ms |
| nn | 6704 | 0.004ms |
| rw | 13211 | 0.005ms |
| 2optnn | **6178** | 0.013ms |
| 2optrw | **6178** | 0.011ms |
| ga | **6178** | 6.666ms |
| gadpx | 6273 | 18.949ms |

#### att15 (15 villes)
| Méthode | Distance | Temps |
|---------|----------|-------|
| nn | 7798 | 0.007ms |
| rw | 15366 | 0.005ms |
| 2optnn | **6443** | 0.020ms |
| 2optrw | 6549 | 0.019ms |
| ga | **6443** | 9.082ms |
| gadpx | **6443** | 30.539ms |


#### burma14 (14 villes)
| Méthode | Distance | Temps |
|---------|----------|-------|
| nn | 4048 | 0.005ms |
| rw | 6669 | 0.005ms |
| 2optnn | 3371 | 0.021ms |
| 2optrw | 3336 | 0.020ms |
| ga | **3323** | 8.169ms |
| gadpx | 3336 | 27.123ms |


#### att48 (48 villes)
| Méthode | Distance | Temps |
|---------|----------|-------|
| nn | 12861 | 0.031ms |
| rw | 46568 | 0.022ms |
| 2optnn | **10959** | 0.183ms |
| 2optrw | **10752** | 0.323ms |
| ga | 11202 | 33.209ms |
| gadpx | 11138 | 108.218ms |


#### bier127 (127 villes)
| Méthode | Distance | Temps |
|---------|----------|-------|
| nn | 135751 | 0.131ms |
| rw | 612594 | 0.130ms |
| 2optnn | **124973** | 2.150ms |
| 2optrw | 129708 | 1.879ms |
| ga | 133696 | 137.716ms |
| gadpx | **127471** | 426.283ms |


#### a280 (280 villes)
| Méthode | Distance | Temps |
|---------|----------|-------|
| nn | 3148 | 0.449ms |
| rw | 35717 | 0.334ms |
| 2optnn | **2840** | 6.281ms |
| 2optrw | 2965 | 6.397ms |
| ga | **2905** | 535.287ms |
| gadpx | 2996 | 1647.908ms |


### Résumé des performances

#### Qualité des solutions
- **2-opt** : Champion sur instances moyennes/grandes
- **AG** : Trouve l'optimum exact sur burma14 (3323)
- **NN** : Rapide mais qualité moyenne
- **RW** : Très variable, qualité médiocre

#### Temps d'exécution
- **NN** : Ultra-rapide (< 0.001s pour toutes instances)
- **2-opt** : Excellent compromis (< 0.01s pour n < 150)
- **AG** : Lent sur grandes instances (0.5s pour n=280)

### Analyse comparative

**Force brute (bf) :**
- Solution optimale garantie
- Limité à ~12 villes (temps exponentiel)
- Utile pour validation sur petites instances

**Plus proche voisin (nn) :**
- Très rapide
- Solutions raisonnables (4-15% de l'optimum)
- Excellent choix pour initialisation

**2-opt + NN :**
- Meilleur rapport qualité/temps
- Solutions à 1-5% de l'optimum
- **Recommandé pour usage général**

**Algorithme génétique (ga) :**
- Bonnes solutions (3-10% de l'optimum)
- Plus lent que 2-opt
- Intéressant pour instances complexes

---

## Limitations

### Limitations algorithmiques

1. **Force brute**
   - Impraticable au-delà de 12-13 villes
   - Temps : (n-1)! opérations
   - Consommation mémoire raisonnable (génération à la volée)

2. **Heuristiques constructives (NN, RW)**
   - Pas de garantie d'optimalité
   - Sensibles au point de départ (NN)
   - Qualité variable (RW très aléatoire)

3. **2-optimisation**
   - Optimum local uniquement
   - Dépend de la qualité de l'initialisation
   - Peut converger prématurément

4. **Algorithme génétique**
   - Paramètres à ajuster selon l'instance
   - Pas de garantie de convergence
   - Temps de calcul significatif

---

## Exemples d'utilisation

### Cas d'usage typiques

#### 1. Trouver la solution optimale (petite instance)

```bash
./tsp -f att10.tsp -m bf
```

**Sortie attendue :**
```
Tour : att10 bf 0.012345 123456.789 [1,2,3,4,5,6,7,8,9,10]
```

---

#### 2. Solution rapide et efficace

```bash
./tsp -f att48.tsp -m 2optnn
```

**Avantages :**
- Très rapide (< 0.1s)
- Bonne qualité (≈1% de l'optimum)
- **Recommandé pour usage général**

---

#### 3. Validation avec Python

```bash
# Modifier test_tsp_c.py pour spécifier l'instance
python3 test_tsp_c.py
```

**Sortie Python :**
```
Instance ; algo ; long (C) ; long (Python) ; temps ; tour ; valid ; mêmes longueurs
att48 ; nn ; 33523 ; 33523 ; 0.001 ; [1,9,38,...] ; True ; True
```

---

#### 4. Test avec interruption (Force brute)

```bash
./tsp -f ../test/att10.tsp -m bft
# Appuyer sur Ctrl+C pendant l'exécution
# Le programme demande si on veut continuer
```

**Interaction :**
```
========== INTERRUPTION DÉTECTÉE ==========
Meilleure tournée trouvée jusqu'à présent :
  Longueur : 12345.67
  Chemin : [1,5,3,7,...]

Voulez-vous continuer les calculs ? (o/n) : n
```

---

#### 5. Calcul de la tournée canonique

```bash
./tsp -f att48.tsp -c
```

**Utilité :**
- Validation des fonctions de distance
- Baseline de comparaison
- Test de parsing TSPLIB

---

## Format des fichiers TSPLIB

### Structure d'un fichier .tsp

```
NAME : att48
COMMENT : 48 villes (att)
TYPE : TSP
DIMENSION : 48
EDGE_WEIGHT_TYPE : ATT
NODE_COORD_SECTION
1 6734 1453
2 2233 10
3 5530 1424
...
48 7462 2604
EOF
```

### Types de distance supportés

| Type | Description | Formule |
|------|-------------|---------|
| EUCL_2D | Distance euclidienne 2D | √((x₁-x₂)² + (y₁-y₂)²) |
| GEO | Distance géographique | Formule géodésique |
| ATT | Distance pseudo-euclidienne | Formule ATT spéciale |

---

## Licence et crédits

### Projet académique

Ce projet a été réalisé dans le cadre du cours de **Projet Avancé** en Licence Informatique à l'Université Paul Sabatier (Toulouse III).

**Enseignant responsable :** Vincent Dugat  
**Année universitaire :** 2025-2026  
**Date :** Septembre-Décembre 2025

### Fichiers TSPLIB

Les instances de test proviennent de la bibliothèque TSPLIB95, maintenue par l'Université de Heidelberg.

---

## Checklist de validation

- [x] Compilation sans erreur ni warning
- [x] Toutes les méthodes implémentées et fonctionnelles
- [x] Tests Python passent avec succès
- [x] Format de sortie normalisé respecté
- [x] Gestion des interruptions (Ctrl+C) pour force brute
- [x] Matrice des distances optimisée (triangulaire)
- [x] Documentation (README) complète
- [x] Code commenté et propre
- [x] Tests sur plusieurs instances TSPLIB

---

**Dernière mise à jour :** Décembre 2025
