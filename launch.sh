#!/bin/bash

###### M1 CHPS - PPN - DYNAMIQUE MOLECULAIRE ######

#### Création d'un fichier d'entrée avec positions de points aléatoire ####

# Unités
## 1 Å = 10^-10 m = 10^-9 dm
## 1 Å^3 = (10^-9)^-3 dm^3 = 10^-27 dm^3 = 10^-27 L

# Conditions de densité
## volume_boite = b_x x b_y x b_z (Å^3)
## masse_volumique_argon = 1,7837 g/L = 1,7837 × 10^−3 kg/L = 1,7837 × 10^−30 kg/A à T = 0°C et p = 1 atm = 10^5 Pa = 1 bar
## masse_atomique_argon = 39,949 u = 39,949 x 1,660 538 921 × 10−27 kg = 6,6337 × 10−26 kg
## N x masse_atomique_argon / (b_x x b_y x b_z) = masse_volumique_argon
## b_x x b_y x b_z = N x masse_atomique_argon / masse_volumique_argon

# Proches des standards ? si N=10000 et b_x = b_y = b_z :
## b_x = (N x masse_atomique_argon / masse_volumique_argon)^(1/3) = (10000 x 6,6337 × 10−26 / (1,7837 × 10^−30))^(1/3) = 719,14 Å

# atomsk --create <lattice> <a> [<c>] <sp1> [<sp2> <sp3>...] [orient (hkl)X (hkl)Y (hkl)Z] [options] <outputfile> [<formats>]
# -add-atom <species> random <N>
# <lattice> ==> sc : simple cubic
# <a> : maille en ångströms (Å) ===> 3,405 Å pour l'argon
# <sp1> ==> Ar : argon

# -cell <add|rm|set> <d> <H1|H2|H3|x|y|z|xy|xz|yx|yz|zx|zy|xyz>
# set <d> <H1|H2|H3|x|y|z|xy|xz|yx|yz|zx|zy|xyz> : change the length of <H1|H2|H3|x|y|z|xy|xz|yx|yz|zx|zy|xyz> to <d> Å

mkdir Entree 

# 5000 atomes :
atomsk --create sc 3.405 Ar orient 100 010 001 -cell set 719,14 H1 -cell set 719,14 H2 -cell set 719,14 H3 Entree/initial5000.cfg
atomsk Entree/initial5000.cfg -add-atom Ar random 5000 Entree/source5000.cfg
atomsk Entree/source5000.cfg xyz

# 10000 atomes (version proche des standards) :
atomsk --create sc 3.405 Ar orient 100 010 001 -cell set 719,14 H1 -cell set 719,14 H2 -cell set 719,14 H3 Entree/initial10000.cfg
atomsk Entree/initial10000.cfg -add-atom Ar random 10000 Entree/source10000.cfg
atomsk Entree/source10000.cfg xyz

# 15000 atomes :
atomsk --create sc 3.405 Ar orient 100 010 001 -cell set 719,14 H1 -cell set 719,14 H2 -cell set 719,14 H3 Entree/initial15000.cfg
atomsk Entree/initial15000.cfg -add-atom Ar random 15000 Entree/source15000.cfg
atomsk Entree/source15000.cfg xyz

#### Exécution du makefile, exécution du programme ####

mkdir Sortie

make
./simulation

#### Nettoyage ####

make clean_o # Suppression automatique des fichiers objet (.o)

