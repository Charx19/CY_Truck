#!/usr/bin/gnuplot --persist

# ARG1 = Data file
# ARG2 = Mode
# ARG3 = Y label
# ARG4 = X label
# ARG5 = Max range

set datafile separator ";"
set terminal pngcairo enhanced font 'arial,12' size 1200,1000

set output "images/img_".ARG2.".png"

set style fill solid
set style data histogram
set style histogram rowstacked
set boxwidth 0.8

set xlabel ARG4 rotate by 180
set ylabel "Option -".ARG2." : ".ARG3." = f(Driver)"
set y2label "NB ROUTES"

set xtic rotate by 90 right # Rotate the names to 90 degrees from the right
unset ytics # Remove the ytics (the tics on left side before being rotated)
set yrange [0:ARG5] # Set Y range for the bars
# y2tics = Values label #
set y2tics
set y2range [0:ARG5] # Set Y range for the label
set y2tic rotate by 90 center # Rotate the number from their center to fit

plot ARG1 using 2:xtic(1) with histogram notitle lc rgb "#79e6af"