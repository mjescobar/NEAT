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
	auto specie = make_unique <Spicies> (std::move(race));


	for (int i = 0; i < 100; ++i)
	{
		for( auto& race : specie->oldRaces )
		{
			for(auto& orgm : race->newOrganisms)
			{
				orgm->setFitness(	(rand()/(double)RAND_MAX)*10 );
			}
		}

		for( auto& race : specie->youngRaces )
		{
			for(auto& orgm : race->newOrganisms)
			{
				orgm->setFitness(	(rand()/(double)RAND_MAX)*10 );
			}
		}
		specie->epoch(1000);
	}
	
	


	//specie->printInfo(); 
	
	// for( auto& race : specie->oldRaces )
	// {
	// 	race->printInfo();
	// }

	// for( auto& race : specie->youngRaces )
	// {
	// 	race->printInfo();
	// }

	return 0;
}