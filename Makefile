CPUS ?= $(shell cat /proc/cpuinfo |grep processor|wc -l || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

VPATH=include:src:objects

COMPILER=g++ -std=c++14

DEBUG= 

OPTMZ=-O3 

CFLAGS=$(DEBUG) -Wall -fPIC -I./include -I./objects -I./src $(OPTMZ)

OBJS = ./objects/Neuron.o ./objects/SynapticWeight.o ./objects/BasicNeuron.o ./objects/Parameter.o ./objects/BasicNeuronUserDefinitions.o ./objects/BasicSynapticWeight.o ./objects/BasicSynapticWeightUserDefinitions.o ./objects/Layer.o  ./objects/ANN.o ./objects/ANNUserDefinitions.o ./objects/Organism.o ./objects/OrganismUserDefinitions.o ./objects/Race.o ./objects/RaceUserDefinitions.o ./objects/ANNTools.o ./objects/RaceTools.o ./objects/SpiciesTools.o ./objects/Spicies.o ./objects/SpiciesUserDefinitions.o ./objects/Life.o ./objects/LifeUserDefinitions.o ./objects/LifeTools.o ./objects/TauSynapticWeight.o ./objects/TauSynapticWeightUserDefinitions.o ./objects/CPPNNeuron.o ./objects/CPPNNeuronUserDefinitions.o ./objects/LIFNeuron.o ./objects/LIFNeuronUserDefinitions.o ./objects/CPPNFunction.o ./objects/NEATStatistics.o 

.PHONY: all clean install git

all: Neuron.o SynapticWeight.o Parameter.o BasicNeuronUserDefinitions.o BasicNeuron.o BasicSynapticWeightUserDefinitions.o BasicSynapticWeight.o Layer.o ANNUserDefinitions.o ANN.o OrganismUserDefinitions.o Organism.o RaceUserDefinitions.o Race.o ANNTools.o RaceTools.o SpiciesTools.o Spicies.o SpiciesUserDefinitions.o Life.o LifeUserDefinitions.o LifeTools.o TauSynapticWeightUserDefinitions.o TauSynapticWeight.o CPPNNeuronUserDefinitions.o CPPNNeuron.o  LIFNeuronUserDefinitions.o LIFNeuron.o CPPNFunction.o NEATStatistics.o
	@echo All NEAT Compiled 

NEATStatistics.o: NEATStatistics.cpp 
	@echo Compiling NEATStatistics class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/NEATStatistics.o

CPPNFunction.o: CPPNFunction.cpp 
	@echo Compiling CPPNFunction class
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/CPPNFunction.o

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

TauSynapticWeightUserDefinitions.o: TauSynapticWeightUserDefinitions.cpp
	@echo Compiling TauSynapticWeightUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/TauSynapticWeightUserDefinitions.o

TauSynapticWeight.o: TauSynapticWeight.cpp
	@echo Compiling TauSynapticWeight
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/TauSynapticWeight.o

CPPNNeuronUserDefinitions.o: CPPNNeuronUserDefinitions.cpp
	@echo Compiling CPPNNeuronUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/CPPNNeuronUserDefinitions.o

CPPNNeuron.o: CPPNNeuron.cpp
	@echo Compiling CPPNNeuron
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/CPPNNeuron.o

ANNTools.o: ANNTools.cpp
	@echo Compiling ANNTools
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/ANNTools.o

OrganismUserDefinitions.o: OrganismUserDefinitions.cpp
	@echo Compiling OrganismUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/OrganismUserDefinitions.o

Organism.o: Organism.cpp
	@echo Compiling Organism
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Organism.o

RaceUserDefinitions.o: RaceUserDefinitions.cpp
	@echo Compiling RaceUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/RaceUserDefinitions.o

Race.o: Race.cpp
	@echo Compiling Race
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Race.o

SpiciesUserDefinitions.o: SpiciesUserDefinitions.cpp
	@echo Compiling SpiciesUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/SpiciesUserDefinitions.o

Spicies.o: Spicies.cpp
	@echo Compiling Spicies
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Spicies.o

LifeUserDefinitions.o: LifeUserDefinitions.cpp
	@echo Compiling LifeUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/LifeUserDefinitions.o

Life.o: Life.cpp
	@echo Compiling Life
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Life.o

LIFNeuron.o: LIFNeuron.cpp
	@echo Compiling LIFNeuron
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/LIFNeuron.o

LIFNeuronUserDefinitions.o: LIFNeuronUserDefinitions.cpp
	@echo Compiling LIFNeuronUserDefinitions
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/LIFNeuronUserDefinitions.o

LifeTools.o: LifeTools.cpp
	@echo Compiling LifeTools
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/LifeTools.o

RaceTools.o: RaceTools.cpp
	@echo Compiling RaceTools
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/RaceTools.o

SpiciesTools.o: SpiciesTools.cpp
	@echo Compiling SpiciesTools
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/SpiciesTools.o

Layer.o: Layer.cpp
	@echo Compiling Layer
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Layer.o

install:
	@g++ $(DEBUG) -shared -Wl,-soname,libneatspikes.so.1 -o libneatspikes.so.1.0 $(OBJS)
	@ln -sf libneatspikes.so.1.0 libneatspikes.so
	@ln -sf libneatspikes.so.1.0 libneatspikes.so.1
	@mv libneatspikes.so.1.0 libneatspikes.so libneatspikes.so.1 /usr/lib
	@rm -rf /usr/include/NEATSPIKES_include
	@cp -r ./include /usr/include/NEATSPIKES_include;
	@cp NEATSpikes /usr/include
	@chmod go+r --recursive /usr/include/NEATSPIKES_include/*
	@chmod go+r --recursive /usr/include/NEATSpikes
	@echo Installation was successful

clean:
	rm -rf objects
