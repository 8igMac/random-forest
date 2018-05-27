#! /bin/bash

echo testing $1 ...
g++ $1 ../src/decision_tree.cpp -I ../include -Wall 
./a.out ../data/iris.txt
rm a.out
