#!/bin/bash

#### SUPERPOSITION DES COURBES ####
gnuplot -p <<- EOF
    set term png
    set output "Courbes/omp_strong_scaling.png"
    set title "OpenMP Scaling"
    set xlabel "Nombre de threads"
    set grid
    set key left top
    plot "Resultats/omp_strong_scaling_20000at_5it_5fs_25c_V10.dat" using 1:2 with lines linestyle 1 linecolor "red" title "Capacité totale (Strong Scaling)", \
         "Resultats/omp_strong_scaling_20000at_5it_5fs_25c_V08.dat" using 1:2 with lines linestyle 1 linecolor "blue" title "Capacité totale (Strong Scaling)"
EOF

gnuplot -p <<- EOF
    set term png
    set output "Courbes/omp_weak_scaling.png"
    set title "OpenMP Scaling"
    set xlabel "Nombre de threads"
    set grid
    set key left top
    plot "Resultats/omp_weak_scaling_20000at_5it_5fs_25c_V10.dat" using 1:2 with lines linestyle 1 linecolor "red" title "Capacité par threads (Weak Scaling)", \
         "Resultats/omp_weak_scaling_20000at_5it_5fs_25c_V08.dat" using 1:2 with lines linestyle 1 linecolor "blue" title "Capacité par threads (Weak Scaling)"
EOF
