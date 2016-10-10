// Test de funcionalidad de TauSynapticWeight

#include <NEATSpikes> 
#include <memory>
#include <iostream>
#include <ctime>       // time 
#include <cstdlib>     // srand, rand 
using namespace NEAT;
using namespace std;


int main()
{
	srand(time(0));
	auto cppnNeuron = make_unique<CPPNNeuron>( );
	auto cppn2Neuron = cppnNeuron->clone();
	cppnNeuron->printInfo();
	cppnNeuron->sumIncomingVoltage(1.0);
	cppnNeuron->spread();
	cout << "input: 1  out:  " << cppnNeuron->getOutput()  << endl << "================================"<< endl;

	cppnNeuron->mightMutate();
	cppnNeuron->printInfo();
	cppnNeuron->sumIncomingVoltage(1.0);
	cppnNeuron->spread();
	cout << "input: 1  out:  " << cppnNeuron->getOutput()  << endl << "================================"<< endl;
	cppnNeuron->mightMutate();
	cppnNeuron->printInfo();
	cppnNeuron->sumIncomingVoltage(1.0);
	cppnNeuron->spread();
	cout << "input: 1  out:  " << cppnNeuron->getOutput()  << endl << "================================"<< endl;
	cppnNeuron->mightMutate();
	cppnNeuron->printInfo();
	cppnNeuron->sumIncomingVoltage(1.0);
	cppnNeuron->spread();
	cout << "input: 1  out:  " << cppnNeuron->getOutput()  << endl << "================================"<< endl;
	cppnNeuron->mightMutate();
	cppnNeuron->printInfo();
	cppnNeuron->sumIncomingVoltage(1.0);
	cppnNeuron->spread();
	cout << "input: 1  out:  " << cppnNeuron->getOutput()  << endl << "================================"<< endl;
	cppnNeuron->mightMutate();
	cppnNeuron->printInfo();
	cppnNeuron->sumIncomingVoltage(1.0);
	cppnNeuron->spread();
	cout << "input: 1  out:  " << cppnNeuron->getOutput()  << endl << "================================"<< endl;

	cout << "Distance between first and last cppnNeurons: " << cppnNeuron->getDistance(cppn2Neuron.get()) << endl;
	cppn2Neuron->printInfo();
	return 0;
}