set encoding "utf8"
set terminal pngcairo enhanced color font 'Helvetica,20';
set output "../images/speedup.png"

set style line 1 lt -1 lw 2 linecolor rgb "#6495ED" pi -1
set style line 4 lt -1 lw 2 linecolor rgb "#006400" pi -1
set style line 2 lt -1 pt 7 lw 2 linecolor rgb "#696969" pi -6
set style line 3 lt -1 pt 5 lw 2 linecolor rgb "#CD5C5C" pi -6
set style line 5 lt -1 pt 13 lw 2 linecolor rgb "#000080" pi -6
set style line 6 lt -1 pt 13 lw 2 linecolor rgb "#DAA520" pi -6

set zeroaxis
set grid ytics xtics

set key right top
set key font "0.25"
set key spacing 0.75
set boxwidth 1

set format xy "%g"

set ylabel 'Speedup'
set yrange [0:0.02]
set ytics 0.005

set logscale x 2
set xlabel "Processos"
set xtics (2,4,8,16)
set xrange [2:16]

plot "../data/speedup/n_18.dat" using 1:2 title 'N = 2^{18}' with lines ls 3, \
     "../data/speedup/n_19.dat" using 1:2 title 'N = 2^{19}' with lines ls 2, \
     "../data/speedup/n_20.dat" using 1:2 title 'N = 2^{20}' with lines ls 5, \
     "../data/speedup/n_21.dat" using 1:2 title 'N = 2^{21}' with lines ls 1, \
     "../data/speedup/n_22.dat" using 1:2 title 'N = 2^{22}' with lines ls 4, \
     "../data/speedup/n_23.dat" using 1:2 title 'N = 2^{23}' with lines ls 6

