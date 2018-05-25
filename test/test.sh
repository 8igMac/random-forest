#! /bin/bash

echo testing $1 ...
g++ $1 ../src/decision_tree.cpp -I ../include
./a.out ../data/iris.txt
rm a.out
