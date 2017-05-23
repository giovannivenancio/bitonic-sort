set encoding "utf8"
set terminal postscript eps enhanced color font 'Helvetica,20';
set output "../images/comp_eficiencia.eps"

set style line 1 lt -1 lw 3 linecolor rgb "#6495ED" pi -1
set style line 4 lt -1 lw 3 linecolor rgb "#006400" pi -1
set style line 2 lt -1 pt 7 lw 4 linecolor rgb "#696969" pi -6
set style line 3 lt -1 pt 5 lw 4 linecolor rgb "#CD5C5C" pi -6
set style line 5 lt -1 pt 13 lw 4 linecolor rgb "#000080" pi -6

set zeroaxis
set grid ytics

set key right top
set key font "0.5"
set key spacing 1
set boxwidth 1

set format xy "%g"

set ylabel 'Eficiência'
set yrange [0:1.3]
set ytics 0.1

set logscale x 2
set xlabel "Processadores"
set xtics (1,2,4,8)

plot "../data/eficiencia/amdahl.dat" using 1:2 title 'Amdahl ({/Symbol b} = 0.08)' with lines ls 3, \
     "../data/eficiencia/gustafson.dat" using 1:2 title 'Gustafson-Barsis ({/Symbol a} = 0.07)' with lines ls 2, \
     "../data/eficiencia/n_31.dat" using 1:2 title 'Aplicação' with lines ls 5, \

