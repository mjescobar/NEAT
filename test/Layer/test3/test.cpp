// Se prueba spread
// Observar si hay algun mal comportamiento.


#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 


int main()
{
	std::srand(std::time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	auto BNUD = std::make_unique < NEAT::BasicNeuronUserDefinitions > ( "./BN_userdef" );
	auto BN1 = std::make_shared < NEAT::BasicNeuron > ( *BNUD );
	auto L1 = std::make_unique < NEAT::Layer> ( BN1 , 0 );

	BN1->printInfo();
	std::cout << "=======" << std::endl;

	for (uint i = 0; i < 5; ++i)
	{
		L1->addNewNeuron();
	}

	L1->printInfo();

	for (uint i = 0; i < L1->neurons.size(); ++i)
	{
		L1->neurons.at(i)->sumIncomingVoltage( rand()/(double)RAND_MAX );
	}

	L1->spread();
	L1->printInfo();


	return 0;

}