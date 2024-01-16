#!/bin/gnuplot 

  set terminal pngcairo enhanced font 'arial,10' size 800,600
	set output "histo.png"
	
	set title "Histogramme exemple"
	
	set style data histograms
	set style fill solid border -1
	set yrange [0:10]
	
	plot 'data.txt' using 2:xtic(1) with histogram title "Data"
	
