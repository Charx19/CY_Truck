#!/usr/bin/gnuplot --persist

# ARG1 = Input data file

set datafile separator ";"
set terminal pngcairo enhanced font 'arial,12' size 1200,1000

set output "images/img_t.png"
set title "Option T- The top 10 travelled towns" 
set title offset 5,0

set xlabel "TOWN NAMES" offset 0,-3
set ylabel "NB ROUTES" 

set style data histograms
set style histogram clustered
set style fill solid border -1

set xtic rotate by 45 right
set yrange [0:3500]

plot ARG1 using 2:xtic(1) lc rgb "#78e5ae" title "Total routes",\
        '' using 3 lc rgb "#5dca93" title "First Town"