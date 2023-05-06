set terminal pngcairo enhanced size 800,600
set output "output.png"

set xlabel "Number"
set ylabel "Cycles CPU"
set key outside;

set title "Voisinlist and Sans Voisinlist Comparison"

plot "/home/yubowen/M1S2/fina/ppn/PPN-DynMol/data.txt" using 1:2 with linespoints title "Voisinlist" lt 1 lw 2 lc rgb "red", \
     "/home/yubowen/M1S2/fina/ppn/1/PPN-DynMol/data.txt" using 1:2 with linespoints title "Sans Voisinlist" lt 1 lw 2 lc rgb "blue"
