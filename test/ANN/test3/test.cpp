// Se desea probar el comportamiento de mightMutate
// Para analizar es necesario que usted vaya cambiando los user def, preferiblemente a probabilidades igual a 1 en diferentes casos.


#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 

using namespace NEAT;
using namespace std;





int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	
	auto BNUD = make_unique < BasicNeuronUserDefinitions > (  );
	auto BNseed = make_shared < BasicNeuron > ( *BNUD );  
	auto BSWUD = make_unique < BasicSynapticWeightUserDefinitions > (  );
	auto BSWseed = make_shared < BasicSynapticWeight > ( *BSWUD ); 
	auto ANNUD = make_unique < ANNUserDefinitions > ();
	auto ann1  = make_unique < ANN > ( *ANNUD, BNseed, BSWseed  );
	


	
	std::cout << "setInputs,getOutputs and spread TEST::" << std::endl;

	std::cout << "ann1::before anything" << std::endl;
	ann1->printInfo();
	std::cout << "MUTATING 10 times" << std::endl;
	for (uint i = 0; i < 10; ++i)
	{
		ann1->mightMutate();
	}

	std::cout << "====================================================" << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << "===================AFTER MIGHTMUTATES===============" << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << "====================================================" << std::endl;
	ann1->printInfo();

	return 0;
}