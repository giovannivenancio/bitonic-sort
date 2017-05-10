set encoding "utf8"
set terminal postscript eps enhanced color font 'Helvetica,20';
set output "../images/tempo.eps"

set style line 1 lt -1 lw 3 linecolor rgb "#6495ED" pi -1
set style line 4 lt -1 lw 3 linecolor rgb "#006400" pi -1
set style line 2 lt -1 pt 7 lw 3 linecolor rgb "#696969" pi -5
set style line 3 lt -1 pt 5 lw 3 linecolor rgb "#CD5C5C" pi -5
set style line 5 lt -1 pt 13 lw 1 linecolor rgb "#000080" pi -5

set zeroaxis;
set grid ytics

set key right top
set key font "0.5"
set key spacing 1
set boxwidth 1

set format y "%g"

set ylabel 'Tempo (s)'
set yrange [0:200]
set ytics 20

set logscale x 2
set format x '2^{%L}'
set xlabel "Tamanho do vetor"

plot "../data/time/sequential.dat" using (2**$1):2 title 'Bitonic Sort Sequencial' with lines ls 3, \
     "../data/time/parallel.dat" using (2**$1):2 title 'Bitonic Sort Paralelo' with lines ls 2

