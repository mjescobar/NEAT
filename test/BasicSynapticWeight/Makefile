VPATH=../../include:.:../../src:../../onjects
COMPILER=g++ -std=c++11
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I.

test: test1.cpp GlobalInformation.o BasicSynapticWeight.o 
	$(COMPILER) $(CFLAGS)  test1.cpp ../../objects/BasicSynapticWeight.o ../../objects/GlobalInformation.o -o ./test

BasicSynapticWeight.o: BasicSynapticWeight.cpp
	cd ../../; make BasicSynapticWeight.o

GlobalInformation.o: GlobalInformation.cpp
	cd ../../; make GlobalInformation.o

clean:
	rm -f save/*
	rm -f test

run:
	mkdir -p save
	./test