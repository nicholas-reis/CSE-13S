#!/usr/bin/env bash

make clean && make

# Split each line using comma as the delimiter.
# Then extract x, the computed value, and the library value from each line.
# Output to `sine()` results to `/tmp/computed.dat`.
# Output to `sin()` results to `/tmp/library.dat`.
./soorting -i -e -s -q | awk -F, '{
    split($1, heap, " ");
    split($2, shell, " ");
    split($3, insert, " ");
    split($4, quick, " ");
    split($5, size, " ");
    print size[1], heap[1] > "/tmp/heap.dat"
    print size[1], shell[1] > "/tmp/shell.dat"
    print size[1], insert[1] > "/tmp/insert.dat"
    print size[1], quick[1] > "/tmp/quick.dat"   
}'

echo -n "Plotting... "
gnuplot <<EOF
set terminal pdf
set key outside
set zeroaxis
set xlabel "Elements"
set ylabel "Moves"
set style data lines
set xrange [ 10.0 : 10000.0 ]
set yrange [ 10.0 : 100000000.0 ]

set log x
set log y

set output "Moves.pdf"
set title "Moves Performed"
plot "/tmp/heap.dat" with lines title "Heap Sort", \
    "/tmp/shell.dat" with lines title "Shell Sort", \
    "/tmp/insert.dat" with lines title "Insertion Sort", \
    "/tmp/quick.dat" with lines title "Quick Sort"
EOF
echo "done."
