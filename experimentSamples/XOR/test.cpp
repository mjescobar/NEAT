// Se prueba el benchmark XOR
#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 
#include <cmath>
#include <string>

using namespace NEAT;
using namespace std;

float maxFitness = 0.f;
float fitnessAcumm = 0.f;
uint contador = 0;
float maxGeneration = 0.f;
Organism champion;
unique_ptr <NEATStatistics> neatStats ;
map< float, Organism* > map_fitness_organism; 


void exiting();

void experiment( Organism& orgm )
{
	//XOR {0,0}->{0}
	orgm.ann->setInputs({0.f,0.f}); 
	orgm.ann->spread();
	vector<float> out1 = orgm.ann->getOutputs();
	float error = fabs(fabs(out1.at(0))- 0.f);
	//XOR {0,1} -> {1}
	orgm.ann->setInputs({0.f,1.f}); 
	orgm.ann->spread();
	vector<float> out2 = orgm.ann->getOutputs();
	error += fabs(fabs(out2.at(0)) - 1.f);
	//XOR {1,0} -> {1}
	orgm.ann->setInputs({1.f,0.f}); 
	orgm.ann->spread();
	vector<float> out3 = orgm.ann->getOutputs();
	error += fabs(fabs(out3.at(0)) - 1.f);
	//XOR {1,1} -> {0}
	orgm.ann->setInputs({1.f,1.f}); 
	orgm.ann->spread();
	vector<float> out4 = orgm.ann->getOutputs();
	error += fabs(fabs(out4.at(0)) - 0.f);

	//cerr << "error: " << error << "\t{" << out1.at(0) <<", " << out2.at(0) <<", " << out3.at(0) << ", " << out4.at(0) <<"} " << endl; 
	float error_MAX = 4.0;
	float fitness = (error_MAX - error)*(error_MAX - error);
	if(fitness > maxFitness){
		maxFitness = fitness; 
		champion = orgm;
		cout << "MF: " << maxFitness << "\t{" << out1.at(0) <<", " << out2.at(0) <<", " << out3.at(0) << ", " << out4.at(0) <<  "}" << endl;
		//orgm.printInfo();
	} 

	if(fitness > maxGeneration){
		maxGeneration = fitness; 
	}

	orgm.setFitness(fitness);
	fitnessAcumm += fitness;
	contador++;
}

void elitism( Life& life );
void sendAllOrganismToExperiment( Life& life ); // function prototype
void sendAllOrganismToElitism( Life& life ); // function prototype


int main( int argc, char ** argv )
{
	atexit(exiting);
	uint seedNumber;
	if (argc == 2)
	{
		string seedNumb_str ((const char *)argv[1]);
		seedNumber = stoul(seedNumb_str);
	}
	else
	{
		seedNumber = time(0);
	}
	cout << "seedNumber: " << seedNumber << endl;


	srand(seedNumber); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	//auto BNseed = make_unique < BasicNeuron > ( );  
	auto cppnNeuron = make_unique < CPPNNeuron > ( ); 
	auto BSWseed = make_unique < BasicSynapticWeight > ( ); 
	auto ann1  = make_unique < ANN > ( move(cppnNeuron), move(BSWseed) );
	auto life = make_unique <Life>( move(ann1) );
	neatStats = make_unique <NEATStatistics>();

	for (int i = 0; i < 50; ++i)
	{

		sendAllOrganismToExperiment(*life);
		elitism(*life);
		//cout << "Gen " << i << "\t" << fitnessAcumm/(float)contador  <<"\t" << contador << "\t" << life->spicies.size()<< endl;
		fitnessAcumm = 0.f;
		contador = 0;

		neatStats->takeInformationOfTheCurrentGeneration(*life);
		life->epoch();
		//cout << "MG: " << maxGeneration << endl;
		maxGeneration = 0.f;

	}
	cout << "================================================" << endl;
	life->printInfo();
	cout << "maxFitness: " << maxFitness << endl;
	cout << "================================================" << endl;
	neatStats->printStatisticsToFile("./save/NEATStatistics");
	neatStats->getAverageFitnessOfAllGenerationInFile("./save/fitessPerGeneration");
	neatStats->getChampionFitnessOfAllGenerationInFile("./save/championPerGeneration");
	champion.ann->save("./save/champion");
	cout << "===LOADING==="<< endl;

	Organism orgm_load;
	auto ccpn_seed = make_unique <CPPNNeuron> ();
	auto syn_seed = make_unique <BasicSynapticWeight>();
	orgm_load.ann->load("./save/champion", move (ccpn_seed), move (syn_seed) );
	cout << "ANN PRINT INFO ==================================" << endl;
	orgm_load.ann->printInfo();

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


void exiting()
{
	//neatStats->printInfo();
}
