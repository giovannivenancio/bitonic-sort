#!/usr/bin/env bash

mode=$1

BIN_SEQ=../src/bitonic_seq
BIN_PAR=../src/bitonic_par

if [ "$mode" == "sequential" ]; then
    echo "*** Running experiments: Sequential Bitonic Sort"
    echo
    BIN=$BIN_SEQ

elif [ "$mode" == "parallel" ]; then
    echo "*** Running experiments: Parallel Bitonic Sort"
    echo
    BIN=$BIN_PAR
else
    echo "Invalid mode!"
    exit 0
fi

for i in `seq 26 30`;
do
    time $BIN $((2**$i)) | grep 'N'
    echo
done
