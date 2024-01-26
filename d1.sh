#!/bin/bash



gnuplot << EOF
	
	set datafile separator ";"
	set terminal pngcairo enhanced font 'arial,12' size 1200,1000
	set output "histo.png"
	set title "Option d1-Nb routes per Driver" rotate by 90
	set title offset 5,0
	set style data histograms
	set style fill solid border -1
	set xtic offset 0,-6 rotate by 90 scale 0
	set ytic rotate by 90 scale 0
	set yrange [0:250]
	set ylabel " NB routes" rotate by 90
	set xlabel " Drivers ID " offset 0,-8 rotate by 180
	plot 'data.txt' using 2:xtic(1) with histogram notitle
	
	$data

EOF
convert -rotate 90 histo.png d1.png
xdg-open "d1.png" & 
