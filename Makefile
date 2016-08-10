
VPATH=include:src:objects:include/BasicNeuron:src/BasicNeuron:include/BasicSynapticWeight:src/BasicSynapticWeight
COMPILER=g++ -std=c++14
CFLAGS=-Wall -fPIC -I./include -I./objects -I./src -O3 -I./include/BasicNeuron -I./src/BasicNeuron -I./include/BasicSynapticWeight -I./src/BasicSynapticWeight

OBJS = ./objects/Neuron.o ./objects/SynapticWeight.o ./objects/BasicNeuron.o ./objects/Parameter.o ./objects/BasicNeuronUserDefinitions.o ./objects/BasicSynapticWeight.o ./objects/BasicSynapticWeightUserDefinitions.o ./objects/Layer.o  ./objects/ANN.o ./objects/ANNUserDefinitions.o

.PHONY: all clean install git

all: Neuron.o SynapticWeight.o Parameter.o BasicNeuronUserDefinitions.o BasicNeuron.o BasicSynapticWeightUserDefinitions.o BasicSynapticWeight.o Layer.o ANNUserDefinitions.o ANN.o
	@echo All NEAT Compiled 


Neuron.o: Neuron.cpp 
	@echo Compiling Neuron class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Neuron.o


SynapticWeight.o: SynapticWeight.cpp 
	@echo Compiling SynapticWeight class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/SynapticWeight.o

Parameter.o: Parameter.cpp 
	@echo Compiling Parameter class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Parameter.o

BasicNeuronUserDefinitions.o: BasicNeuronUserDefinitions.cpp
	@echo Compiling BasicNeuronUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicNeuronUserDefinitions.o

BasicNeuron.o: BasicNeuron.cpp
	@echo Compiling BasicNeuron
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicNeuron.o

BasicSynapticWeight.o: BasicSynapticWeight.cpp
	@echo Compiling BasicSynapticWeight
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicSynapticWeight.o

BasicSynapticWeightUserDefinitions.o: BasicSynapticWeightUserDefinitions.cpp
	@echo Compiling BasicSynapticWeightUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicSynapticWeightUserDefinitions.o

ANNUserDefinitions.o: ANNUserDefinitions.cpp
	@echo Compiling ANNUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/ANNUserDefinitions.o

ANN.o: ANN.cpp
	@echo Compiling ANN
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/ANN.o

Layer.o: Layer.cpp
	@echo Compiling Layer
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Layer.o

install:
	@g++ -shared -Wl,-soname,libneatspikes.so.1 -o libneatspikes.so.1.0 $(OBJS)
	@ln -sf libneatspikes.so.1.0 libneatspikes.so
	@ln -sf libneatspikes.so.1.0 libneatspikes.so.1
	@mv libneatspikes.so.1.0 libneatspikes.so libneatspikes.so.1 /usr/lib
	@mkdir -p /usr/include/NEATSPIKES_include/
	@find ./include -type f -exec cp {} /usr/include/NEATSPIKES_include/ \;
	@cp NEATSpikes /usr/include
	@chmod go+r --recursive /usr/include/NEATSPIKES_include/*
	@chmod go+r --recursive /usr/include/NEATSpikes
	@echo Installation was successful

clean:
	rm -rf objects