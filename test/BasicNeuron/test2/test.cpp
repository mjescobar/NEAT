// Se realizaran mediciones de distancia entre dos BasicnNeurons diferentes, varais veces.
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
	auto BN2 = std::make_unique < NEAT::BasicNeuron > ( *BNUD );

	for (int i = 0; i < 5; ++i)
	{
		std::cout << " BN1: " << std::endl;
		BN1->printInfo();
		std::cout << " BN2: " << std::endl;
		BN2->printInfo();
		std::cout << "Distance: " << BN2->getDistance( BN1.get() ) << std::endl;
		BN1->mightMutate();
		BN2->mightMutate();
	}

	return 0;

}