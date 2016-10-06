// Se probara solo la carga de los datos y el funcionamiento de mutate


#include <NEATSpikes>
#include <memory>
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 

int main()
{
	std::srand(std::time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	
	auto BSWUD = std::make_unique < NEAT::BasicSynapticWeightUserDefinitions > ( "./BSWUD" );
	auto BSW1 = std::make_unique < NEAT::BasicSynapticWeight > ( *BSWUD );

	BSWUD->printInfo();

	for (int i = 0; i < 10; ++i)
	{
		BSW1->printInfo();
		BSW1->mightMutate();
	}

	return 0;
}