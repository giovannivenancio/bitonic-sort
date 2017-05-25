set encoding "utf8"
set terminal postscript eps enhanced color font 'Helvetica,20';
set output "../images/desempenho.eps"

set style line 1 lt -1 lw 2 linecolor rgb "#6495ED" pi -1
set style line 4 lt -1 lw 2 linecolor rgb "#006400" pi -1
set style line 2 lt -1 lw 2 linecolor rgb "#696969" pi -6
set style line 3 lt -1 lw 2 linecolor rgb "#CD5C5C" pi -6
set style line 5 lt -1 lw 2 linecolor rgb "#000080" pi -6
set style line 5 lt -1 lw 2 linecolor rgb "#FFD700" pi -6

set zeroaxis
set grid ytics xtics

set key left top
set key font "0.5"
set key spacing 1
set boxwidth 1

set format y "%g"

set ylabel 'Tempo (ms)'
set yrange [0:2000]
set ytics 200

set logscale x 2
set format x '2^{%L}'
set xlabel "Tamanho do grupo"
set xrange [1:2147483648]

plot "../data/desempenho/n_26.dat" using (2**$1):2 title ' N = 2^{26}' with lines ls 3, \
     "../data/desempenho/n_27.dat" using (2**$1):2 title 'N = 2^{27}' with lines ls 2, \
     "../data/desempenho/n_28.dat" using (2**$1):2 title 'N = 2^{28}' with lines ls 5, \
     "../data/desempenho/n_29.dat" using (2**$1):2 title 'N = 2^{29}' with lines ls 1, \
     "../data/desempenho/n_30.dat" using (2**$1):2 title 'N = 2^{30}' with lines ls 4, \
     "../data/desempenho/n_31.dat" using (2**$1):2 title 'N = 2^{31}' with lines ls 2, \


