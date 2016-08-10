// Se realizaran pruebas de entradas versus salidas.
// Observar el comportamiento de la salida segun las entradas y ver que todo este bien.


#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 


int main()
{
	std::srand(std::time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	auto BNUD = std::make_unique < NEAT::BasicNeuronUserDefinitions > ( "./BN_userdef" );
	auto BN1 = std::make_unique < NEAT::BasicNeuron > ( *BNUD );

	float random;
	for (int i = 0; i < 20; ++i)
	{
		random = (2.0 * rand()/(double)RAND_MAX -1.0) * 1.1;
		BN1->sumIncomingVoltage( random );
		BN1->spread();
		std::cout << "BN1 ::" << std::endl;
		BN1->printInfo();
		std::cout << "input: " << random <<  "\toutput: " << BN1->getOutput() << std::endl;
		BN1->mightMutate();
	}

	return 0;

}