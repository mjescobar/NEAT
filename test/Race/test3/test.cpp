//
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
	auto race = make_unique < Race > (std::move(orgm));

	for (int i = 0; i < 2; ++i)
	{
		race->epoch(10);
		auto& neworganisms = race->getNewOrganisms_ref();
		std::cout << "neworganisms size: " << neworganisms.size() << std::endl;
		for ( auto& orgm : neworganisms )
		{
			orgm->setFitness( i*1.0f );
		}
	}
	std::cout << "Fitness mean: " << race->getFitnessMean() << std::endl;

	return 0;
}