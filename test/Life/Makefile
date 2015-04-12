VPATH=../../include  .
COMPILER=g++ -std=c++11
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I. -frtti

test: test.cpp ../../src/Life.cpp  
	cd ../../; make Life
	$(COMPILER) $(CFLAGS)  test.cpp ../../objects/Life.o ../../objects/Niche.o ../../objects/ANN.o ../../objects/Input.o ../../objects/BasicNeuron.o ../../objects/BasicSynapticWeight.o ../../objects/Neuron.o  ../../objects/InformationBetweenLifeAndOrganism.o  ../../objects/SynapticWeight.o -o ./test

clean:
	rm -f save/*
	rm -f test

run:
	./test BNUserDef BSWUserDef  userDefANN 