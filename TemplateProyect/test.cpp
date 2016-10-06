// Se prueba el benchmark XOR
#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 
#include <cmath>

using namespace NEAT;
using namespace std;

float maxFitnessAllTimes = 0.f;
float fitnessAcumm = 0.f;
uint contador = 0;
float maxGenerationFitness = 0.f;

void experiment( Organism& orgm )
{
	float fitness = 0.f;




	// Crea tu experimento aqui.



	// El codigo siguiente es para mostrar el avance del fitness y mostrar asi como esta aprendiendo a solucionar el problema.
	if(fitness > maxFitnessAllTimes){
		maxFitnessAllTimes = fitness; 
		cout << "MF: " << maxFitnessAllTimes  << endl;
	} 
	if(fitness > maxGenerationFitness){
		maxGenerationFitness = fitness; 
	}
	orgm.setFitness(fitness);
	fitnessAcumm += fitness;
	contador++;
}




//===========================================================================================

// Si eres principiante en el uso de NEAT y no has leido completamente la guia de usuario se recomienda no examinar en detalle el codigo de mas abajo.
//===========================================================================================




void sendAllOrganismToExperiment( Life& life ); // function prototype

int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	auto BNseed = make_unique < BasicNeuron > ( );  
	auto BSWseed = make_unique < BasicSynapticWeight > ( ); 
	auto ann1  = make_unique < ANN > ( move(BNseed), move(BSWseed) );
	auto life = make_unique <Life>( move(ann1) );

	for (int i = 0; i < 100; ++i)
	{
		sendAllOrganismToExperiment(*life);
		//cout << "Gen " << i << "\t" << fitnessAcumm/(float)contador  <<"\t" << contador << "\t" << life->spicies.size()<< endl;
		fitnessAcumm = 0.f;
		contador = 0;
		life->epoch();
		cout << "MG: " << maxGenerationFitness << endl;
		maxGenerationFitness = 0.f;

	}
	cout << "================================================" << endl;
	life->printInfo();
	cout << "maxFitnessAllTimes: " << maxFitnessAllTimes << endl;
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