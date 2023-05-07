#!/bin/bash

#### SUPERPOSITION DES COURBES ####
gnuplot -p <<- EOF
    set term png
    set output "Courbes/omp_scaling.png"
    set title "OpenMP Scaling"
    set xlabel "Nombre de threads"
    set grid
    set key left top
    plot "Resultats/omp_weak_scaling.dat" using 1:2 with lines linestyle 1 linecolor "red" title "Capacité par threads (Weak Scaling)", \
         "Resultats/omp_strong_scaling.dat" using 1:2 with lines linestyle 1 linecolor "blue" title "Capacité totale (Strong Scaling)"
EOF

