VPATH=../../include:.:../../src:../../objects
COMPILER=g++ -std=c++11
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I. -frtti -g

xor: xor.cpp Life.cpp  ANN.cpp 
	mkdir -p bin
	cd ../../; make
	$(COMPILER) $(CFLAGS)  xor.cpp ../../objects/Life.o ../../objects/Niche.o ../../objects/ANN.o ../../objects/Input.o ../../objects/BasicNeuron.o ../../objects/BasicSynapticWeight.o  ../../objects/GlobalInformation.o  -o ./bin/xor

clean:
	rm -f save/*
	rm -rf bin

run:
	./bin/xor