#!/bin/bash



gnuplot << EOF
	
	set datafile separator ";"
	set terminal pngcairo enhanced font 'arial,12' size 1200,1000
	set output "traitementL.png"
	set title "Option L- The longest distances" 
	set title offset 5,0
	set style data histograms
	set style fill solid border -1
	set xtic offset 0,-6 
	set yrange [0:3000]
	set ylabel " Distance " 
	set xlabel " Route ID " offset 0,-8 
	plot 'data.txt' using 2:xtic(1) with histogram notitle
	
	$data

EOF

xdg-open "traitementL.png" & 
