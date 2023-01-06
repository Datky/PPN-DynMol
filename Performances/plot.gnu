set output 'AosvSoA.png'
red = "#FF0000"; blue = "#0000FF"
set yrange [0:5000000000]

set xtics format""
set grid ytics


set xlabel "Nombre d'atomes simulés"
set ylabel "Cycles CPU pour une itération"
set title "Comparaison de performance entre AoS et SoA"
plot "perf.dat" using 2:xtic(1) with linespoints title "AoS" linecolor rgb red, \
     "perf.dat" using 3 with linespoints title "SoA" linecolor rgb blue, 
