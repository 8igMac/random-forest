#!/bin/bash

OPTICAL=optical-digits
IRIS=iris

## iput mode
#time ./bin/main data/$1.txt data/format/$1.format

## feed data: optical-digits
time ./bin/main data/$OPTICAL.txt data/format/$OPTICAL.format

## feed data: iris
#time ./bin/main data/$IRIS.txt data/format/$IRIS.format
