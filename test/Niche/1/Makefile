VPATH=../../../include:../../../src:.:../../../objects
COMPILER=g++ -std=c++11 -O3
CFLAGS = -Wall -fPIC -I../../../include   -I../../../objects -I.

test: test.cpp ANN.o BasicSynapticWeight.o  BasicNeuron.o GlobalInformation.o MutationControl.o Niche.o
	rm -Rf save/*;
	$(COMPILER) $(CFLAGS) test.cpp ../../../objects/ANN.o ../../../objects/BasicSynapticWeight.o  ../../../objects/BasicNeuron.o ../../../objects/GlobalInformation.o ../../../objects/MutationControl.o ../../../objects/Niche.o   -o ./test
	
Niche.o: Niche.cpp Niche.hpp
	cd ../../..; make Niche.o;

BasicNeuron.o: BasicNeuron.cpp BasicNeuron.hpp
	cd ../../..; make BasicNeuron.o;

GlobalInformation.o: GlobalInformation.cpp GlobalInformation.hpp
	cd ../../..; make GlobalInformation.o;

ANN.o: ANN.cpp ANN.hpp 
	cd ../../..; make ANN.o;

MutationControl.o: MutationControl.cpp MutationControl.hpp ReferenceMap.hpp ReferenceMapVector.hpp
	cd ../../..; make MutationControl.o;

BasicSynapticWeight.o: BasicSynapticWeight.cpp BasicSynapticWeight.hpp 
	cd ../../..; make BasicSynapticWeight.o;

clean:
	rm -Rf save/*
	rm -f test
cleanALL:
	make clean
	cd ../../..; make clean
run:
	rm -Rf save/*
	mkdir -p save
	./test