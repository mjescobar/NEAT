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
	auto ann2 = make_unique < ANN > ( *ANNUD, BNseed, BSWseed  );
	
	ann1->printInfo();
	ann2->printInfo();


	std::cout << "Distance:   " << ann1->getDistance(  *ann2  ) << std::endl; 

	return 0;
}