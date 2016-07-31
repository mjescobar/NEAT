
VPATH=include:src:objects
COMPILER=g++ -std=c++14
CFLAGS=-Wall -fPIC -I./include -I./objects -I./src -O3 
OBJS = ./objects/Life.o ./objects/Niche.o ./objects/ANN.o ./objects/BasicSynapticWeight.o ./objects/BasicNeuron.o ./objects/Input.o ./objects/GlobalInformation.o

# .PHONY: all clean install git

all: Neuron.o Parameter.o BasicNeuronUserDefinitions.o BasicNeuron.o
	@echo All NEAT Compiled 


Neuron.o: Neuron.cpp 
	@echo Compiling Neuron class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Neuron.o

Parameter.o: Parameter.cpp 
	@echo Compiling Parameter class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Parameter.o

BasicNeuronUserDefinitions.o: ./src/BasicNeuron/BasicNeuronUserDefinitions.cpp
	@echo Compiling BasicNeuronUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicNeuronUserDefinitions.o

BasicNeuron.o: ./src/BasicNeuron/BasicNeuron.cpp
	@echo Compiling BasicNeuron
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicNeuron.o
