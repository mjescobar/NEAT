// Se probara el funcionamiento de entrada respecto a la salida de la synapticweight
// Observar si la salida es la multiplicacion de la entrada por el peso sinaptico en casos aleatorios


#include <NEATSpikes>
#include <memory>
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 
#include <iostream> //cout

int main()
{
	std::srand(std::time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	
	auto BSWUD = std::make_unique < NEAT::BasicSynapticWeightUserDefinitions > ( "./BSWUD" );
	auto BSW1 = std::make_unique < NEAT::BasicSynapticWeight > ( *BSWUD );

	BSWUD->printInfo();
	float random;
	for (int i = 0; i < 10; ++i)
	{
		random = rand() / (double)RAND_MAX;
		
		BSW1->setInput(random);
		BSW1->spread();
		BSW1->printInfo();
		BSW1->mightMutate();
	}

	return 0;
}