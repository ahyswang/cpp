#!/bin/sh

g++ -fPIC libA.cpp -shared -o libA.so
g++ -fPIC libB.cpp -shared -o libB.so

g++ main.cpp libA.so libB.so -o main

export LD_LIBRARY_PATH=.

./main
