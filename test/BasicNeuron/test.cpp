#include "BasicNeuron.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace NEATSpikes;

int main()
{
	srand( time( 0 ) ); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.

	cout << "Basic Neuron Test initialized" << endl;
	// Lo primero que se prueba es la carga correcta de los parámetros de usuario.
	BasicNeuron * BN = new BasicNeuron("./BasicNeuronUserDefinitions");
	cout << "First Neuron created by constructor method:" << endl;
	BN->printState(); // Se imprime el estado buscando ver que están correctamente inicializadas todas las variables importantes.

	cout << "Now 100 mutations " << endl;
	// Se prueban 5 mutaciones.
	for (int i = 0; i < 100; ++i)
	{
		BN->mutate();
		BN->printState();	
	}

	cout << "Testing the last one with a random input voltage" << endl;

	double random = rand()/(double)RAND_MAX;

	BN->sumIncomingConnectionsOutputs(random);
	cout << "input voltage: " << random << "\t Neuron output: " << BN->eval() << endl;


	cout << "Saving data" << endl;
	BN->saveState("./save/");
	BN->saveUserDefinitions("./save/");
	BN->saveId("./save/");


	cout << "Loading UserDef" << endl;
	BasicNeuron * BNPrototype = new BasicNeuron("./save/userDefinitions");
	cout << "Loading Id" << endl;
	BNPrototype->loadId("./save/BN_id");	


	cout << "Loading data" << endl;
	BasicNeuron * BN2 = dynamic_cast< BasicNeuron * > ( BNPrototype->duplicate());
	BN2->load("./save/BN0");

	cout << "Basic neuron loaded" << endl;
	BN2->printState();

	BN2->mutate();
	BN2->mutate();
	BN2->mutate();
	BN2->mutate();

	cout << "BN " << endl;
	BN->printState();

	cout << "BN2" << endl;
	BN2->printState();

	cout << " Diference between BN2 and BN: "<< BN2->getDistance(BN) << endl;



	return EXIT_SUCCESS;
}