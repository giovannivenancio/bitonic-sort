set encoding "utf8"
set terminal postscript eps enhanced color font 'Helvetica,20';
set output "../images/speedup.eps"

set style line 1 lt -1 lw 3 linecolor rgb "#6495ED" pi -1
set style line 4 lt -1 lw 3 linecolor rgb "#006400" pi -1
set style line 2 lt -1 pt 7 lw 2 linecolor rgb "#696969" pi -6
set style line 3 lt -1 pt 5 lw 2 linecolor rgb "#CD5C5C" pi -6
set style line 5 lt -1 pt 13 lw 2 linecolor rgb "#000080" pi -6

set zeroaxis
set grid ytics xtics

set key right top
set key font "0.5"
set key spacing 1
set boxwidth 1

set format xy "%g"

set ylabel 'Speedup'
set yrange [0:12]
set ytics 1

set logscale x 2
set xlabel "Threads"
set xtics (1,2,4,8,16)

plot "../data/speedup/n_29.dat" using 1:2 title 'N = 2^{29}' with lines ls 3, \
     "../data/speedup/n_30.dat" using 1:2 title 'N = 2^{30}' with lines ls 2, \
     "../data/speedup/n_31.dat" using 1:2 title 'N = 2^{31}' with lines ls 5, \

