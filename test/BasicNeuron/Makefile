VPATH=../../include:../../src:.
COMPILER=g++ -std=c++11 -O3
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I.

test: test.cpp
	cd ../../; make BasicNeuron.o
	$(COMPILER) $(CFLAGS)  test.cpp ../../objects/BasicNeuron.o -o ./test

clean:
	rm -f save/*
	rm -f test

run:
	./test