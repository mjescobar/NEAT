VPATH=include:src:objects
COMPILER=g++ -std=c++11
CFLAGS=-Wall -fPIC -I./include -I./objects -I./src -frtti -O3

Life.o: Life.cpp Niche.o
	mkdir -p objects
	$(COMPILER) $(CFLAGS) -c $< -o ./objects/Life.o

ANN.o: ANN.cpp BasicSynapticWeight.o BasicNeuron.o Input.o GlobalInformation.o 
	mkdir -p objects
	$(COMPILER) $(CFLAGS) -c $< -o ./objects/ANN.o

BasicSynapticWeight.o: BasicSynapticWeight.cpp BasicSynapticWeight.hpp GlobalInformation.o 
	mkdir -p objects
	$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicSynapticWeight.o

BasicNeuron.o: BasicNeuron.cpp
	mkdir -p objects
	$(COMPILER) $(CFLAGS) -c $< -o ./objects/BasicNeuron.o

Input.o: Input.cpp
	mkdir -p objects
	$(COMPILER) $(CFLAGS) -c $< -o ./objects/Input.o

Niche.o: Niche.cpp ANN.o
	mkdir -p objects
	$(COMPILER) $(CFLAGS) -c $< -o ./objects/Niche.o


GlobalInformation.o: GlobalInformation.cpp GlobalInformation.hpp
	mkdir -p objects
	$(COMPILER) $(CFLAGS) -c $< -o ./objects/GlobalInformation.o

clean:
	rm -Rf objects

# Primero se borran todos los objetos. (no es estrictamente necesario porque se tiene en las reglas del .gitignore a los objetos pero pudiera ser que  los make clean borraran más que sólo los objetos)
# Luego se agregan los archivos.
gitAddAll:
	./clean.sh
	git add --all
	git status