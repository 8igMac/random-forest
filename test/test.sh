#! /bin/bash

echo testing $1 ...
g++ $1 ../src/decision_tree.cpp -I ../include -Wall 
time ./a.out ../data/$2.txt ../data/format/$2.format
rm a.out
