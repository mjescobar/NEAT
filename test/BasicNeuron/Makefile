VPATH=../../include:.:../../src:../../objects
COMPILER=g++ -std=c++11
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I.

test: test.cpp BasicNeuron.o GlobalInformation.o
	$(COMPILER) $(CFLAGS)  test.cpp ../../objects/BasicNeuron.o ../../objects/GlobalInformation.o -o ./test
	rm -f save/*

BasicNeuron.o: BasicNeuron.cpp
	cd ../..; make BasicNeuron.o;

GlobalInformation.o: GlobalInformation.cpp
	cd ../..; make GlobalInformation.o;


clean:
	rm -f save/*
	rm -f test

run:
	mkdir -p save
	./test