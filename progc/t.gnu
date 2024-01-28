#!/usr/bin/gnuplot --persist

set datafile separator ";"
set terminal pngcairo enhanced font 'arial,12' size 1200,1000

set output "traitementT.png"
set title "Option T- The top 10 travelled towns" 
set title offset 5,0

set ylabel " NB routes " 
set xlabel " Town Names" offset 0,-8 

set style data histograms
set style histogram clustered
set style fill solid border -1
set xtic offset 0,-6 
set yrange [0:4000]

plot 'dataT.txt' using 2:xtic(1) title "Total routes",\
        'dataT.txt' using 3 title "First Town"