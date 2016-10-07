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
	orgm.ann->setInputs({1.f,1.f});
	orgm.ann->spread();
	vector <float> out = orgm.ann->getOutputs();
	orgm.printInfo();
	orgm.setFitness(1.f);
}

void sendAllOrganismToExperiment( Life& life );

int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	std::cerr << "P-21: "<<std::endl;
	auto BNseed = make_unique < BasicNeuron > ( );  
	std::cerr << "P-1: "<<std::endl;
	auto BSWseed = make_unique < BasicSynapticWeight > ( ); 
	std::cerr << "P-0.5: "<<std::endl;
	auto ann1  = make_unique < ANN > ( move(BNseed), move(BSWseed) );
	std::cerr << "P0: "<<std::endl;
	auto life = make_unique <Life>( move(ann1) );

	std::cerr << "P1: "<<std::endl;
	for (int i = 0; i < 1; ++i)
	{
	std::cerr << "P2: "<<std::endl;
		sendAllOrganismToExperiment(*life);
	std::cerr << "P3: "<<std::endl;
		life->epoch();
	std::cerr << "P4: "<<std::endl;
	}
	



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