VPATH=../../include:.:../../src:../../objects
COMPILER=g++ -std=c++11
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I. -frtti

xor: xor.cpp Life.cpp  
	cd ../../; make Life.o
	$(COMPILER) $(CFLAGS)  xor.cpp ../../objects/Life.o ../../objects/Niche.o ../../objects/ANN.o ../../objects/Input.o ../../objects/BasicNeuron.o ../../objects/BasicSynapticWeight.o  ../../objects/GlobalInformation.o  -o ./xor

clean:
	rm -f save/*
	rm -f xor

run:
	./xor