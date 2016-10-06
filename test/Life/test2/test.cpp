// Se corrobora que la vida funcione correctamente en terminos simples, ver la cantidad de hijos, cantidad de razas y especies... si al final los organismos son mas que la cantidad maxima de organismos es pq algunas razas acaban de pasar de ser jovenes a adultas.
#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 

using namespace NEAT;
using namespace std;


void experiment( Organism& orgm )
{
	orgm.setFitness( (rand()/(double)RAND_MAX)*10 );
}

void sendAllOrganismToExperiment( Life& life );

int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	auto BNseed = make_unique < BasicNeuron > ( );  
	auto BSWseed = make_unique < BasicSynapticWeight > ( ); 
	auto ann1  = make_unique < ANN > ( move(BNseed), move(BSWseed) );
	auto life = make_unique <Life>( move(ann1) );

	life->printInfo();
	for (auto& spicie : life->spicies)
	{
		spicie->printInfo();
		for(auto& race : spicie->youngRaces)
		{
			race->printInfo();
		}

		for(auto& race : spicie->oldRaces)
		{
			race->printInfo();
		}
	}
	for (int i = 0; i < 100; ++i)
	{
		sendAllOrganismToExperiment(*life);
		life->epoch();
	}
	std::cout << "================================================" << std::endl;
	life->printInfo();
	uint sum = 0;
	for (auto& spicie : life->spicies)
	{
		spicie->printInfo();
		for(auto& race : spicie->oldRaces)
		{
			sum += race->newOrganisms.size();
			race->printInfo();
		}
	}

	std::cout << "oldRacesOrganisms: " << sum << std::endl;


	return 0;
}


void sendAllOrganismToExperiment( Life& life )
{
	for ( auto& spicie : life.spicies)
	{
		for( auto& race: spicie->youngRaces)
		{
			for( auto& orgm : race->newOrganisms )
			{
				experiment( *orgm );
			}
		}
		for( auto& race: spicie->oldRaces)
		{
			for( auto& orgm : race->newOrganisms )
			{
				experiment( *orgm );
			}
		}
	}
}