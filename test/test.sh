#! /bin/bash

echo testing $1 ...
g++ -Wall $1 -I ../include
./a.out ../data/iris.txt
rm a.out
