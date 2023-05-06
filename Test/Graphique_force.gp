set terminal pngcairo font "arial,12" size 1000, 800
set output 'force_Lennard_Jones.png'

set ylabel "Force de Lennard-Jones"
set xlabel "r (angstrom)"
set title "Graphique de la force de Lennard-Jones en fonction de la distence entre atomes"

set grid

plot 'Data_force_Lennard_Jones.dat' using 1:2 with linespoints title "froce" linecolor rgb "#0000FF"
