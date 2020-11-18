#!/bin/sh

# 全局变量默认是对外可见改为默认不可见
g++ -fPIC libA.cpp -shared -fvisibility=hidden -o libA.so
g++ -fPIC libB.cpp -shared -fvisibility=hidden -o libB.so

g++ main.cpp libA.so libB.so -o main

export LD_LIBRARY_PATH=.

./main
