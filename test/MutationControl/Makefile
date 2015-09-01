VPATH=../../include:../../src:.:../../objetcs:.
COMPILER=g++ -std=c++11 -O3
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I.

test: test.cpp ANN.o BasicSynapticWeight.o  BasicNeuron.o GlobalInformation.o
	rm -f save/*;
	$(COMPILER) $(CFLAGS) test.cpp ../../objetcs/ANN.o ../../objetcs/BasicSynapticWeight.o  ../../objetcs/BasicNeuron.o ../../objetcs/GlobalInformation.o   -o ./test
	

BasicNeuron.o: BasicNeuron.cpp BasicNeuron.hpp
	cd ../..; make BasicNeuron.o;

GlobalInformation.o: GlobalInformation.cpp GlobalInformation.hpp
	cd ../..; make GlobalInformation.o;

ANN.o: ANN.cpp ANN.hpp
	cd ../..; make ANN.o;

BasicSynapticWeight.o: BasicSynapticWeight.cpp BasicSynapticWeight.hpp 
	cd ../..; make BasicSynapticWeight.o;

clean:
	rm -f save/*
	rm -f test

run:
	mkdir -p save
	./test