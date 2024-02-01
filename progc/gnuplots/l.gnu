#!/usr/bin/gnuplot --persist

# ARG1 = Input data file

set datafile separator ";"
set terminal pngcairo enhanced font 'arial,12' size 1200,1000

set output "images/img_l.png"
set title "Option -l : Distance = f(Route)" 
set title offset 5,0

set style fill solid
set style data histogram
set style histogram rowstacked
set boxwidth 0.8

set ylabel " DISTANCE (Km) " 
set xlabel " ROUTE ID "

set xtic
set yrange [0:3000]

plot ARG1 using 2:xtic(1) with histogram notitle lc rgb "#79e6af"