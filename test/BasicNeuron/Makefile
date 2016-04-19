VPATH=../../include:.:../../src:../../objects
COMPILER=g++ -std=c++11
CFLAGS = -Wall -fPIC -I../../include   -I../../objects -I.
LIBFLAGSGL=-lGL -lGLU -lglut -lglfw -lGLEW

test: test.cpp BasicNeuron.o GlobalInformation.o shader.o OpenGLInitialization.o
	$(COMPILER) $(CFLAGS)  test.cpp ../../objects/BasicNeuron.o ../../objects/GlobalInformation.o objects/shader.o ../../objects/OpenGLInitialization.o -o ./test $(LIBFLAGSGL)
	rm -f save/*

shader.o: shader.cpp shader.hpp
	mkdir -p objects
	$(COMPILER) $(CFLAGS) -c shader.cpp -o objects/shader.o

BasicNeuron.o: BasicNeuron.cpp
	cd ../..; make BasicNeuron.o;

GlobalInformation.o: GlobalInformation.cpp
	cd ../..; make GlobalInformation.o;

OpenGLInitialization.o: OpenGLInitialization.cpp
	cd ../..; make OpenGLInitialization.o;

clean:
	rm -f save/*
	rm -f test

run:
	mkdir -p save
	./test