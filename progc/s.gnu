#!/usr/bin/gnuplot --persist

set datafile separator ";"
set terminal pngcairo enhanced font 'arial,12' size 1200,1000

set output "images/img_s.png"
set title "Option -s : Distance = f(Route)"

set xlabel "ROUTE ID"
set ylabel "DISTANCE (Km)"

set xtic rotate by 45 right
set ytics 100
set yrange [0:1000]

plot ARG1 using ($1-1):3:5:xtic(2) with filledcurve fc rgb "#6ad7a1" title "Distance Max/Min (Km)",\
    '' using 4 smooth mcspline lw 2 lc rgb "#019e73" title "Distance average (Km)"