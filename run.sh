#!/bin/sh
mkdir build
rm -rf bin
cd build/ && cmake .. && make && cd ../bin/ && ./main