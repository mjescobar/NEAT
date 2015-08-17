VPATH=include:src:objects
COMPILER=g++ -std=c++11
CFLAGS=-Wall -fPIC -I./include -I./objects -I./src -frtti -O3 -g
OBJS = ./objects/Life.o ./objects/Niche.o ./objects/ANN.o ./objects/BasicSynapticWeight.o ./objects/BasicNeuron.o ./objects/Input.o ./objects/GlobalInformation.o

.PHONY: all clean install git

all: Life.o Niche.o ANN.o BasicSynapticWeight.o BasicNeuron.o Input.o GlobalInformation.o MutationControl.o
	@echo All NEAT Compiled 


Life.o: Life.cpp Niche.cpp Niche.o GlobalInformation.o
	@echo Compiling Life 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Life.o

ANN.o: ANN.cpp BasicSynapticWeight.cpp BasicNeuron.cpp Input.cpp GlobalInformation.cpp BasicSynapticWeight.o BasicNeuron.o Input.o GlobalInformation.o 
	@echo Compiling ANN 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/ANN.o
	
BasicSynapticWeight.o: BasicSynapticWeight.cpp BasicSynapticWeight.hpp GlobalInformation.cpp GlobalInformation.o 
	@echo Compiling BasicSynapticWeight 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicSynapticWeight.o

BasicNeuron.o: BasicNeuron.cpp GlobalInformation.o
	@echo Compiling BasicNeuron 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicNeuron.o

Input.o: Input.cpp GlobalInformation.o
	@echo Compiling Input 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Input.o

Niche.o: Niche.cpp ANN.cpp
	@echo Compiling Niche 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Niche.o

   
GlobalInformation.o: GlobalInformation.cpp GlobalInformation.hpp
	@echo Compiling GlobalInformation 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/GlobalInformation.o

MutationControl.o: MutationControl.cpp MutationControl.hpp GlobalInformation.o BasicNeuron.o
	@echo Compiling MutationControl 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/MutationControl.o
	

clean:
	@rm -Rf objects
	@echo Clean --- Ok

# Primero se borran todos los objetos. (no es estrictamente necesario porque se tiene en las reglas del .gitignore a los objetos pero pudiera ser que  los make clean borraran más que sólo los objetos)
# Luego se agregan los archivos.
git:
	@./clean.sh
	@git add --all; git status; git commit -e; git push  
	
install:
	@g++ -shared -Wl,-soname,libneatspikes.so.1 -o libneatspikes.so.1.0 $(OBJS)
	@ln -sf libneatspikes.so.1.0 libneatspikes.so
	@ln -sf libneatspikes.so.1.0 libneatspikes.so.1
	@mv libneatspikes.so.1.0 libneatspikes.so libneatspikes.so.1 /usr/lib
	@mkdir -p /usr/include/NEATSPIKES_include/
	@cp ./include/* /usr/include/NEATSPIKES_include/
	@cp NEATSPIKES /usr/include
	@chmod go+r /usr/include/NEATSPIKES_include/*
	@chmod go+r /usr/include/NEATSPIKES
	@echo Installation was successful