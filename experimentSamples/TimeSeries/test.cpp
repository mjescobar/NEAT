// Se prueba el benchmark XOR
#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 
#include <cmath>

using namespace NEAT;
using namespace std;

float maxFitness = 0.f;
float fitnessAcumm = 0.f;
uint contador = 0;
float maxGeneration = 0.f;

// Serie de tiempo 
// X[k+1] = r*X[k]*(1-X[k])
// X[0] = 0.1 ; r = 4

float x = 0.1f;
float r = 4.f;
unsigned int dataSetLarge = 1000;
unsigned int waitToFill = 50; // Time to wait while buffer filling

void experiment( Organism& orgm )
{
	float fitness = 0.f;
	float realOutput = x;
	vector <float> annOutput;

	float MSE = 0.f;
	for (unsigned int i = 0; i < waitToFill; ++i)
	{
		orgm.ann->setInputs({realOutput});
		realOutput = realOutput*(1-realOutput)*r;
		orgm.ann->spread();
		annOutput = orgm.ann->getOutputs();
	}
		
	for (unsigned int i = 0; i < dataSetLarge; ++i)
	{
		orgm.ann->setInputs({realOutput});
		realOutput = realOutput*(1-realOutput)*r;
		orgm.ann->spread();
		annOutput = orgm.ann->getOutputs();
		MSE += powf( realOutput - annOutput.at(0) ,2.f);
		//cout << "MSE: " <<MSE << endl;
	}	

	MSE = MSE/dataSetLarge;

	fitness = 1.f/MSE;

	if(fitness > maxFitness){
		maxFitness = fitness; 
	} 

	if(fitness > maxGeneration){
		maxGeneration = fitness; 
	}

	orgm.setFitness(fitness);
	fitnessAcumm += fitness;
	contador++;
}

void sendAllOrganismToExperiment( Life& life ); // function prototype

int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	auto BNseed = make_unique < BasicNeuron > ( );  
	auto BSWseed = make_unique < BasicSynapticWeight > ( ); 
	auto ann1  = make_unique < ANN > ( move(BNseed), move(BSWseed) );
	// auto TSWseed = make_unique <TauSynapticWeight>();
	// auto ann1 = make_unique <ANN> ( move(BNseed), move(TSWseed) );
	auto life = make_unique <Life>( move(ann1) );

	for (int i = 0; i < 1000; ++i)
	{
		sendAllOrganismToExperiment(*life);
		//std::cout << "Gen " << i << "\t" << fitnessAcumm/(float)contador  <<"\t" << contador << "\t" << life->spicies.size()<< std::endl;
		fitnessAcumm = 0.f;
		contador = 0;
		life->epoch();
		std::cout << "MG: " << maxGeneration << std::endl;
		maxGeneration = 0.f;

	}
	std::cout << "================================================" << std::endl;
	life->printInfo();
	std::cout << "maxFitness: " << maxFitness << std::endl;
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