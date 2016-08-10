// Este test esta pensado simplemente en observar la carga de los datos de usuario y el funcionamiento de mutate.
// Observar el comportamiento de los datos segun los valores de usuario entregados y determinar si el comportamiento posee
// anomalias.


#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 


int main()
{
	std::srand(std::time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	auto BNUD = std::make_unique < NEAT::BasicNeuronUserDefinitions > ( "./BN_userdef" );
	auto BN = std::make_unique < NEAT::BasicNeuron > ( *BNUD );
	BNUD->printInfo();

	for (int i = 0; i < 30; ++i)
	{
		BN->printInfo();
		BN->mightMutate();
	}

	return 0;

}