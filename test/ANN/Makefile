VPATH=../../include:../../objects:.
COMPILER=g++ -std=c++11
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I. -frtti -O3

test: test.cpp ../../objects/ANN.o  
	cd ../../; make ANN.o
	$(COMPILER) $(CFLAGS)  test.cpp ../../objects/ANN.o ../../objects/Input.o ../../objects/BasicNeuron.o ../../objects/BasicSynapticWeight.o ../../objects/GlobalInformation.o  -o ./test

clean:
	rm -f save/*
	rm -f test

run:
	./test BNUserDef BSWUserDef  userDefANN 