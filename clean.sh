#!/bin/sh
# El objetivo de éste script es recursivamente llamar a la regla make clean de todo los Makefiles que se encuentren para así agregar tranquiliamente  todo al repositorio github.
dir=`pwd`/
find . -name Makefile | while read line
do
   tmp=$(dirname $line) 
   finalDir=$dir$tmp
   cd $finalDir; make clean
done