#!/bin/bash

# enter the dataset file name in ./data
OPTICAL=optical-digits
IRIS=iris

## feed data: optical-digits
time ./bin/main data/$OPTICAL.txt data/format/$OPTICAL.format

## feed data: iris
#time ./bin/main data/$IRIS.txt data/format/$IRIS.format
