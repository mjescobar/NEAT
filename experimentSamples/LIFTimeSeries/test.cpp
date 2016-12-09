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

map< float, Organism* > map_fitness_organism; 
unique_ptr <NEATStatistics> neatStats ;


float vectorSum( const vector <float> & vec )
{
	float result = 0.f;
	for (uint i = 0; i < vec.size(); ++i)
	{
		result += vec.at(i);
	}
	return result;
}

void experiment( Organism& orgm )
{
	float fitness = 0.f;
	float realOutput = x;
	vector <float> annOutput;
	vector <float> inputs;
	for (int i = 0; i < 20; ++i)
	{
		inputs.push_back(0.f);
	}
	float MSE = 0.f;
	for (unsigned int i = 0; i < waitToFill; ++i)
	{
		for (int i = 0; i < 20; ++i)
		{
			inputs.at(i) = realOutput;
		}
		orgm.ann->setInputs(inputs);
		realOutput = realOutput*(1-realOutput)*r;
		orgm.ann->spread();
	}
		
	for (unsigned int i = 0; i < dataSetLarge; ++i)
	{
		for (int i = 0; i < 20; ++i)
		{
			inputs.at(i) = realOutput;
		}
		orgm.ann->setInputs(inputs);
		realOutput = realOutput*(1-realOutput)*r;
		orgm.ann->spread();
		annOutput = orgm.ann->getOutputs();
		MSE += powf(realOutput -  vectorSum(annOutput) ,2.f);
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
void elitism( Life& life );

int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	float sampleTime = 0.001f;
	auto lifn = make_unique < LIFNeuron > ( sampleTime );  
	auto TSWseed = make_unique <BasicSynapticWeight>();
	auto ann1 = make_unique <ANN> ( move(lifn), move(TSWseed) );
	auto life = make_unique <Life>( move(ann1) );
	neatStats = make_unique <NEATStatistics>();

	for (int i = 0; i < 1000; ++i)
	{
		sendAllOrganismToExperiment(*life);
		elitism(*life);
		//std::cout << "Gen " << i << "\t" << fitnessAcumm/(float)contador  <<"\t" << contador << "\t" << life->spicies.size()<< std::endl;
		fitnessAcumm = 0.f;
		contador = 0;
		neatStats->takeInformationOfTheCurrentGeneration(*life);
		life->epoch();
		std::cout << "MG: " << maxGeneration << std::endl;
		maxGeneration = 0.f;

	}
	std::cout << "================================================" << std::endl;
	life->printInfo();
	neatStats->printStatisticsToFile("./save/NEATStatistics");
	neatStats->getAverageFitnessOfAllGenerationInFile("./save/fitessPerGeneration");
	neatStats->getChampionFitnessOfAllGenerationInFile("./save/championPerGeneration");
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


void elitism( Life& life )
{
	uint counter = 0;
	uint counter_1 = 0;
	uint counter_2 = 0;
	map_fitness_organism.clear();
	for ( auto& spicie : life.spicies)
	{
		for( auto& race: spicie->youngRaces)
		{
			for( auto& orgm : race->newOrganisms )
			{
	 			counter++;
	 			counter_1++;
	 			float new_fitness = orgm->getFitness() + (rand()/(double)RAND_MAX)/100.f;
	 			while( map_fitness_organism.find(new_fitness) != map_fitness_organism.end() )
	 			{
	 				new_fitness = orgm->getFitness() + (rand()/(double)RAND_MAX)/100.f;	
	 				cout << "new_fitness exists " << new_fitness << endl;
	 			}
	 			map_fitness_organism.emplace( new_fitness,  orgm.get()); // el rand es para minimizar probabilidad de colision en el mapa
			}
		}
		for( auto& race: spicie->oldRaces)
		{
			for( auto& orgm : race->newOrganisms )
			{
	 			counter++;
	 			counter_2++;
	 			float new_fitness = orgm->getFitness() + (rand()/(double)RAND_MAX)/100.f;
	 			while( map_fitness_organism.find(new_fitness) != map_fitness_organism.end() )
	 			{
	 				new_fitness = orgm->getFitness() + (rand()/(double)RAND_MAX)/100.f;	
	 				cout << "new_fitness exists " << new_fitness << endl;
	 			}
	 			map_fitness_organism.emplace(orgm->getFitness() + (rand()/(double)RAND_MAX)/100.f,  orgm.get()); // el rand es para minimizar probabilidad de colision en el mapa
			}
		}
	}

	uint large = map_fitness_organism.size();
	if(counter == large)
	{	
		uint counter = 0; // Este vive solo en este scope fijarse.
		for( auto& item : map_fitness_organism )
		{
			item.second->setFitness( pow(counter/(float)large*4.0,2.0) );
			counter++;
		}
	}
	else
	{
		cout << "WARNING::elitism was not posible; "  << large <<  "\t" << counter<< endl; 
		cout << "c1: " << counter_1 << "\t" << "c2: " << counter_2 << endl;
	}
	map_fitness_organism.clear();
		
}

