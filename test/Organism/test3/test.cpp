// Se prueba la funcionalidad de createSimilar y surviveNewEpoch.
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
	auto BNseed = make_shared < BasicNeuron > ( );  
	auto BSWseed = make_shared < BasicSynapticWeight > ( ); 
	auto ann1  = make_unique < ANN > ( BNseed, BSWseed );
	auto orgm = make_unique < Organism > ( std::move(ann1) );
	std::cout << "ORGM 1 \n==========" << std::endl;
	orgm->printInfo();
	std::cout << "ORGM 2 \n==========" << std::endl;
	auto orgm2 = orgm->createSimilar();
	orgm2->printInfo();

	auto orgm3 = orgm->crossOver( *orgm2 );
	std::cout << "ORGM 3 \n==========" << std::endl;
	orgm3->printInfo();
	return 0;
}