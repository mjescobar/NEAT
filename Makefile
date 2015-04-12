VPATH=include:src:objects
COMPILER=g++ -std=c++11
CFLAGS=-Wall -fPIC -I./include -I./objects -I./src -frtti -O3
OBJS = ./objects/Life.o ./objects/Niche.o ./objects/ANN.o ./objects/BasicSynapticWeight.o ./objects/BasicNeuron.o ./objects/Input.o ./objects/GlobalInformation.o

Life.o: Life.cpp Niche.cpp
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Life.o
	@echo Compiling Life 

ANN.o: ANN.cpp BasicSynapticWeight.cpp BasicNeuron.cpp Input.cpp GlobalInformation.cpp 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/ANN.o
	@echo Compiling ANN 

BasicSynapticWeight.o: BasicSynapticWeight.cpp BasicSynapticWeight.hpp GlobalInformation.cpp 
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicSynapticWeight.o
	@echo Compiling BasicSynapticWeight 

BasicNeuron.o: BasicNeuron.cpp
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicNeuron.o
	@echo Compiling BasicSynapticWeight 

Input.o: Input.cpp
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Input.o
	@echo Compiling BasicSynapticWeight 

Niche.o: Niche.cpp ANN.cpp
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/Niche.o
	@echo Compiling BasicSynapticWeight 

   
GlobalInformation.o: GlobalInformation.cpp GlobalInformation.hpp
	@mkdir -p objects
	@$(COMPILER) $(CFLAGS) -c $< -o ./objects/GlobalInformation.o
	@echo Compiling BasicSynapticWeight 

clean:
	@rm -Rf objects
	@echo Compiling BasicSynapticWeight 

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