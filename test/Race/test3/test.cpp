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
	race->printInfo();
	{
		auto& neworganisms = race->getNewOrganisms_ref();
		std::cout << "neworganisms size: " << neworganisms.size() << std::endl;
		for ( auto& orgm : neworganisms )
		{
			orgm->setFitness( (1 + 10)*1.0f );
			std::cout << "(i + 10)*1.0f" << (1 + 10)*1.0f << std::endl;
		}
		race->epoch(10);
	}
	{
		auto& neworganisms = race->getNewOrganisms_ref();
		std::cout << "neworganisms size: " << neworganisms.size() << std::endl;
		for ( auto& orgm : neworganisms )
		{
			orgm->setFitness( (2 + 10)*1.0f );
			std::cout << "(i + 10)*1.0f" << (2 + 10)*1.0f << std::endl;
		}
	}
	std::cout << "Fitness mean: " << race->getFitnessMean() << std::endl;
	race->printInfo();
	return 0;
}