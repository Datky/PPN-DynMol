#!/bin/bash

# M1 CHPS - PPN - DYNAMIQUE MOLECULAIRE
# Création d'un fichier d'entrée avec positions de points aléatoire

# atomsk --create <lattice> <a> [<c>] <sp1> [<sp2> <sp3>...] [orient (hkl)X (hkl)Y (hkl)Z] [options] <outputfile> [<formats>]
# -add-atom <species> random <N>
# <lattice> ==> sc : simple cubic
# <a> : maille en ångströms (Å) ===> 3,405 Å pour l'argon
# <sp1> ==> Ar : argon

# 100 atomes :
atomsk --create sc 3.405 Ar orient 100 010 001 -cell set 1000 H1 -cell set 1000 H2 -cell set 1000 H3 initial100.cfg
atomsk initial100.cfg -add-atom Ar random 100 source100.cfg
atomsk source100.cfg xyz

# 1000 atomes :
atomsk --create sc 3.405 Ar orient 100 010 001 -cell set 1000 H1 -cell set 1000 H2 -cell set 1000 H3 initial1000.cfg
atomsk initial1000.cfg -add-atom Ar random 1000 source1000.cfg
atomsk source1000.cfg xyz

# 10000 atomes :
atomsk --create sc 3.405 Ar orient 100 010 001 -cell set 1000 H1 -cell set 1000 H2 -cell set 1000 H3 initial10000.cfg
atomsk initial10000.cfg -add-atom Ar random 10000 source10000.cfg
atomsk source10000.cfg xyz

