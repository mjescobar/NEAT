COMPILER=g++ -std=c++14
CFLAGS=-Wall -fPIC -I./include -I./objects -I./src -O3 

all: testapp

testapp: test.cpp
	@echo Compiling a testapp aplication of BasicNeuron
	@$(COMPILER) $(CFLAGS) ./test.cpp -o testapp -lneatspikes	
	@echo to use simply call make run

compileNEATSpikes:
	cd ../../..; make all

installNEATSpikes:
	cd ../../../; sudo make install

run: 
	@mkdir -p save
	@rm -rf /save/*
	@./testapp

clean:
	rm testapp