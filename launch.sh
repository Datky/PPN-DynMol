#!/bin/bash

###### M1 CHPS - PPN - DYNAMIQUE MOLECULAIRE ######

#### INFORMATIONS PHYSIQUES ####

# Unités
## 1 Å = 10^-10 m = 10^-9 dm
## 1 Å^3 = (10^-9)^-3 dm^3 = 10^-27 dm^3 = 10^-27 L

# Conditions de densité
## volume_boite = b_x x b_y x b_z (Å^3)
## masse_volumique_argon = 1,7837 g/L = 1,7837 × 10^−3 kg/L = 1,7837 × 10^−30 kg/A à T = 0°C et p = 1 atm = 10^5 Pa = 1 bar
## masse_atomique_argon = 39,949 u = 39,949 x 1,660 538 921 × 10−27 kg = 6,6337 × 10−26 kg
## N x masse_atomique_argon / (b_x x b_y x b_z) = masse_volumique_argon
## b_x x b_y x b_z = N x masse_atomique_argon / masse_volumique_argon
# Proches des standards ? si N=10000 et b_x = b_y = b_z :
## b_x = (N x masse_atomique_argon / masse_volumique_argon)^(1/3) = (10000 x 6,6337 × 10−26 / (1,7837 × 10^−30))^(1/3) = 719,14 Å

#### INFORMATIONS ATOMSK ####
# atomsk --create <lattice> <a> [<c>] <sp1> [<sp2> <sp3>...] [orient (hkl)X (hkl)Y (hkl)Z] [options] <outputfile> [<formats>]
# -add-atom <species> random <N>
# <lattice> ==> sc : simple cubic
# <a> : maille en ångströms (Å) ===> 3,405 Å pour l'argon
# <sp1> ==> Ar : argon
# -cell <add|rm|set> <d> <H1|H2|H3|x|y|z|xy|xz|yx|yz|zx|zy|xyz>
# set <d> <H1|H2|H3|x|y|z|xy|xz|yx|yz|zx|zy|xyz> : change the length of <H1|H2|H3|x|y|z|xy|xz|yx|yz|zx|zy|xyz> to <d> Å

#### DEBUT ####
echo ""; echo -n "BONJOUR ET BIENVENUE DANS L'EXECUTION DU PROGRAMME DE DYNAMIQUE MOLECULAIRE !" ; echo "";

# Performance :
echo ""; echo -n "FREQUENCE DU PROCESSEUR EN MODE PERFORMANCE ?" ; echo "";
read -p "Souhaitez-vous exécuter la commande 'sudo cpupower -c all frequency-set -g performance' ? (y/n)" answer
if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
    sudo cpupower -c all frequency-set -g performance
    echo "Mode super-utilisateur activé."
else
    echo "Mode super-utilisateur désactivé."
fi

# Répertoires :
mkdir Entree
mkdir Sortie Sortie_omp Sortie_mpi Terminal Resultats Courbes
mkdir Bin

#### TESTS ####
echo ""; echo -n "! TESTS !" ; echo "";
echo -n "Compilations" ; echo "";
make Bin/TEST
echo -n "Exécution des tests :" ; echo "";
./Bin/TEST
diff "Test/TEST_source.xyz" "Test/TEST_cible.xyz" >> Test/diff_test.txt
make clean_test
echo ""; echo -n "! FIN TESTS !" ; echo "";

# Saisie des arguments du main() en ligne de commande :
echo ""; echo -n "SAISIE DES PARAMETRES !" ; echo "";
echo -n "Merci d'entrer le nombre d'atomes d'argon 'N' (exemple : 10000): "
read N
echo -n "Merci d'entrer le nombre d'itérations 'nb_itérations', i.e. autant d'applications de l'algorithme de Verlet-vitesses (exemple : 20): "
read nb_iteration
echo -n "Merci d'entrer le pas de temps 'dt' en fs (exemple : 10 fs): "
read dt
echo -n "Merci d'entrer les dimensions (supposées égales) selon x, y, z du maillage 3D de cellules 'c_x, c_y, c_z' (exemple : 5x(x5x5) cellules): "
read c_x
c_y="$c_x"
c_z="$c_x"

# Dimensions de la boîte de simulation :
taille_10000=719
b_x=$(echo "$taille_10000*$N/10000" | bc)
b_y=$(echo "$taille_10000*$N/10000" | bc)
b_z=$(echo "$taille_10000*$N/10000" | bc)
echo -n "Dimensions de la boîte de simulation ajustées en fonction du nombre de particules selon les densités standard (en ångströms): "
echo "$b_x x $b_y x $b_z"

echo ""; echo -n "GENERATION DU FICHIER .XYZ INITIAL PAR ATOMSK !" ; echo "";
#1) Création d'une structure cristalline cubique simple d'atomes d'argon avec une maille de 3,405 Å, orientée selon les vecteurs (100), (010) et (001), définition des dimensions de la boîte, coordonnées initiales des atomes dans "Entree/initial$N.cfg" :
atomsk --create sc 3.405 Ar orient 100 010 001 -cell set $b_x H1 -cell set $b_y H2 -cell set $b_z H3 Entree/initial$N.cfg 

#2) Ajout de $N atomes d'argon aux positions aléatoires dans "Entree/source$N.cfg" :
atomsk Entree/initial$N.cfg -add-atom Ar random $N Entree/source$N.cfg

#3) Dans un fichier .xyz pour future visualisation par Ovito (par exemple) :
atomsk Entree/source$N.cfg xyz 

#### COMPILATIONS ####
echo ""; echo -n "COMPILATIONS !" ; echo "";
make

#### EXECUTIONS ####
echo ""; echo -n "EXECUTIONS !" ;
echo ""; read -p "Souhaitez-vous exécuter la commande taskset -c 2 ...' pour limiter l'exécution au coeur N°2 (recommandé si parallélisme recherche : non) ? (y/n)" answer ; echo "";
if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
    echo ""; echo "Exécution sur le coeur N°2." ; echo "";
    echo ""; echo -n "EXECUTION DE LA VERSION DE BASE SUR 1 COEUR !" ; echo "";
    # taskset -c 2 ./Bin/simulation $N $nb_iteration $dt $b_x $b_y $b_z $c_x $c_y $c_z
    # taskset -c 2 ./Bin/simulation $N $nb_iteration $dt $b_x $b_y $b_z $c_x $c_y $c_z | tee Terminal.txt > /dev/null
    taskset -c 2 ./Bin/simulation $N $nb_iteration $dt $b_x $b_y $b_z $c_x $c_y $c_z | tee Terminal/terminal.txt
    sleep 1
    echo ""; echo -n "EXECUTION DE LA VERSION MPI SUR 1 COEUR EN SUSPENS !" ; echo "";
    #taskset -c 2 ./Bin/simulation_mpi $N $nb_iteration $dt $b_x $b_y $b_z $c_x $c_y $c_z | tee Terminal/terminal_mpi.txt
    sleep 1
    echo ""; echo -n "EXECUTION DE LA VERSION OPEN MP SUR 1 COEUR !" ; echo "";
    taskset -c 2 ./Bin/simulation_omp $N $nb_iteration $dt $b_x $b_y $b_z $c_x $c_y $c_z | tee Terminal/terminal_omp.txt
else
    echo ""; echo "Exécution sans affectation de coeur spécifique." ; echo "";
    echo ""; echo -n "EXECUTION DE LA VERSION DE BASE !" ; echo "";
    ./Bin/simulation $N $nb_iteration $dt $b_x $b_y $b_z $c_x $c_y $c_z | tee Terminal/terminal.txt
    sleep 1
    echo ""; echo -n "EXECUTION DE LA VERSION MPI EN SUSPENS !" ; echo "";
    #./Bin/simulation_mpi $N $nb_iteration $dt $b_x $b_y $b_z $c_x $c_y $c_z | tee Terminal/terminal_mpi.txt
    sleep 1
    for i in 1 2 4 6 8
        do
            echo ""; echo -n "EXECUTION DE LA VERSION OPEN MP SUR $i MODULO 8 COEURS !" ; echo "";
            export OMP_NUM_THREADS=$i
            ./Bin/simulation_omp $N $nb_iteration $dt $b_x $b_y $b_z $c_x $c_y $c_z | tee Terminal/terminal_omp_${i}.txt
        done
fi

#### PROFILAGE ET DEBUGAGE ####
# valgrind ./Bin/simulation -s $N $nb_iteration $dt $b_x $b_y $b_z

# Profilage de l'utilisation du processeur dans le fichier de sortie callgrind.out :
# valgrind --tool=callgrind ./Bin/simulation $N $nb_iteration $dt $b_x $b_y $b_z
# valgrind ./Bin/simulation $N $nb_iteration $dt $b_x $b_y $b_z -s --leak-check=full

# Affichage des résultats et envoie vers un fichier .txt
# callgrind_annotate callgrind.out.* >> valgrind.txt

# Profilage GDB :
# gdb ./Bin/simulation $N $nb_iteration $dt $b_x $b_y $b_z

#### TEST DES FICHIER DE SORTIE ####
echo ""; echo -n "! TEST DES FICHIER DE SORTIE !" ; echo "";
echo -n "Confer répertoire /Test" ; echo "";
if [ -f "Sortie/simulation"$N"_iteration01.xyz" ]; then
    if [ -f "Sortie_omp/simulation"$N"_iteration01.xyz" ]; then
        for (( i=1; i <= $nb_iteration; i++ ))
        do
            if [ $i -lt 10 ]; then
                diff "Sortie/simulation"$N"_iteration0"$i".xyz" "Sortie_omp/simulation"$N"_iteration0"$i".xyz" >> Test/diff_omp.txt
            else
                diff "Sortie/simulation"$N"_iteration"$i".xyz" "Sortie_omp/simulation"$N"_iteration"$i".xyz" >> Test/diff_omp.txt
            fi
        done
    fi

    if [ -f "Sortie_mpi/simulation"$N"_iteration01.xyz" ]; then
        for i in 'seq 1 $nb_iteration'
        do
            if [ $i -lt 10 ]; then
                diff "Sortie/simulation"$N"_iteration0"$i".xyz" "Sortie_mpi/simulation"$N"_iteration0"$i".xyz" >> Test/diff_mpi.txt
            else
                diff "Sortie/simulation"$N"_iteration"$i".xyz" "Sortie_mpi/simulation"$N"_iteration"$i".xyz" >> Test/diff_mpi.txt
            fi
        done
    fi
fi
echo ""; echo -n "! FIN TEST !" ; echo "";

#### AFFICHAGES OVITO ####
echo ""; echo -n "AFFICHAGE OVITO ?" ; echo "";
read -p "Souhaitez-vous lancer Ovito (supposé installé ; supposé sourcé dans votre répertoire actuel) ? (réponse recommandée : n) (y/n)" answer
if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
    echo "Lancement de Ovito."
    ovito
else
    echo "Ovito n'a pas été lancé." ; echo "";
fi

#### EXPLOITATION DES RESULTATS ####
paste Resultats/omp_max_threads.dat Resultats/omp_capacite_par_thread.dat > Resultats/omp_strong_scaling.dat
paste Resultats/omp_max_threads.dat Resultats/omp_capacite.dat > Resultats/omp_weak_scaling.dat

#### COURBES GNUPLOT ####
gnuplot -p <<- EOF
    set term png
    set output "Courbes/omp_strong_scaling.png"
    set title "STRONG SCALING"
    set xlabel "Nombre de threads"
    set ylabel "Capacité (nombre d'atomes * nombre d'itérations) par thread"
    set grid
    set key left top
    plot "Resultats/omp_strong_scaling.dat" using 1:2 with lines linestyle 1 linecolor "red" title "Capacité par threads = f(nombre de threads)"
EOF

gnuplot -p <<- EOF
    set term png
    set output "Courbes/omp_weak scaling.png"
    set title "WEAK SCALING"
    set xlabel "Nombre de threads"
    set ylabel "Capacité totale (nombre d'atomes * nombre d'itérations)"
    set grid
    set key left top
    plot "Resultats/omp_weak_scaling.dat" using 1:2 with lines linestyle 1 linecolor "red" title "Capacité = f(nombre de threads)"
EOF

#### NETTOYAGE ####
rm -rf Entree Bin
make clean_o # Suppression automatique des fichiers objet (.o) 

