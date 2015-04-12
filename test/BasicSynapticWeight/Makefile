VPATH=../../include  .
COMPILER=g++ -std=c++11
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I.

test: test1.cpp ../../src/BasicSynapticWeight.cpp
	cd ../../; make BasicSynapticWeight.o
	$(COMPILER) $(CFLAGS)  test1.cpp ../../objects/BasicSynapticWeight.o -o ./test

clean:
	rm -f save/*
	rm -f test

run:
	./test