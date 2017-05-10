set encoding "utf8"
set terminal postscript eps enhanced color font 'Helvetica,20';
set output "../images/speedup.eps"

set style line 1 lt -1 lw 3 linecolor rgb "#6495ED" pi -1
set style line 4 lt -1 lw 3 linecolor rgb "#006400" pi -1
set style line 2 lt -1 pt 7 lw 1 linecolor rgb "#696969" pi -10
set style line 3 lt -1 pt 5 lw 1 linecolor rgb "#CD5C5C" pi -10
set style line 5 lt -1 pt 13 lw 1 linecolor rgb "#000080" pi -10

set zeroaxis;
set grid ytics

set key right top
set key font "0.5"
set key spacing 1
set boxwidth 1

set format xy "%g"

set ylabel 'Speedup'
set yrange [0:100]
set ytics 10

set xlabel "Processadores"
set xrange [0:32]
set xtics (1, 2, 4, 8, 16, 32)

plot "../data/" using 1:2 title 'Gustafson-Barsis' with linespoints ls 3, \
     "../data/" using 1:2 title 'Amdahl' with linespoints ls 2, \
     "../data/" using 1:2 title 'Implementação' with linespoints ls 5

