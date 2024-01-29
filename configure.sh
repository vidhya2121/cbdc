#!/bin/bash
cd CBDC_F22
rm CMakeCache.txt
cmake .
make
cd ..