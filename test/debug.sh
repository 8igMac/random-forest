#! /bin/bash

echo debugging $1 ...
g++ $1 -I ../include -Wall -g
gdb a.out


