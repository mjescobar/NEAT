// Se desea probar el comportamiento de setInputs, get Outputs y spread
// Para lograr lo anterior se realizara un spread con una ANN recien creada en otro test se observara su comportamiento al inlcuir mutaciones.


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
	ann1->setInputs( {0.3f,0.1f,0.2f} );
	ann1->spread();
	std::vector <float> outputs = ann1->getOutputs();

	std::cout << "Before Spread:: inputs" << "{0.3f,0.1f,0.2f}" << "\toutputs: {" << outputs.at(0) << ","<< outputs.at(1) << ","<< outputs.at(2) << ","<< outputs.at(3) << ","<< outputs.at(4) << "}" << std::endl;
	std::cout << "ann1 printInfo::" << std::endl;
	ann1->printInfo();

	return 0;
}