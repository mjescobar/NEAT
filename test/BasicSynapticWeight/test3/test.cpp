// Se probara el funcionamiento de la funcion getDistance



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
	auto BSW2 = std::make_unique < NEAT::BasicSynapticWeight > ( *BSWUD );


	BSWUD->printInfo();
	for (int i = 0; i < 10; ++i)
	{
		std::cout << "BSW1 info::::" << std::endl;
		BSW1->printInfo();
		std::cout << "BSW2 info::::" << std::endl;
		BSW2->printInfo();
		std::cout << "Distancia: " << BSW1->getDistance( BSW2.get() ) << std::endl;
		BSW2->mightMutate();
		BSW1->mightMutate();
	}

	return 0;
}