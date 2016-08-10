// Se desea probar el comportamiento de mightMutate
// Para analizar es necesario que usted vaya cambiando los user def, preferiblemente a probabilidades igual a 1 en diferentes casos.

// Se recomienda cambiar la funcion spread de BN a una que sea output = inputAccum
// y que los pesos tengan valor 1 (a traves de parametros de ususario) 
// con lo anterior se hara mas facil la observacion.


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
	

	ANNUD->printInfo();

	std::cout << "setInputs,getOutputs and spread TEST::" << std::endl;

	std::cout << "ann1::before anything" << std::endl;
	
	for (uint i = 0; i < 2; ++i)
	{
		ann1->mightMutate();
	}
	ann1->setInputs( {1.f,1.f} );
	ann1->spread();
	ann1->printInfo();
	ann1->setInputs( {2.f,2.f} );
	ann1->spread();
	ann1->printInfo();
	return 0;
}