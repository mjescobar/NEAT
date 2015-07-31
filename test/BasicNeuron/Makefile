VPATH=../../include:../../src:.:../../objetcs
COMPILER=g++ -std=c++11 -O3
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I.

test: test.cpp BasicNeuron.o
	$(COMPILER) $(CFLAGS)  test.cpp ../../objects/BasicNeuron.o -o ./test

BasicNeuron.o: BasicNeuron.cpp
	cd ../..; make BasicNeuron.o;


clean:
	rm -f save/*
	rm -f test

run:
	mkdir -p save
	./test