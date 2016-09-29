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
		cout << "MF: " << maxFitness << "\t{" << out1.at(0) <<", " << out2.at(0) <<", " << out3.at(0) << ", " << out4.at(0) << endl;
		//orgm.printInfo();
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
	auto life = make_unique <Life>( move(ann1) );

	for (int i = 0; i < 100; ++i)
	{
		cerr << "m1" << endl;
		sendAllOrganismToExperiment(*life);
		//cout << "Gen " << i << "\t" << fitnessAcumm/(float)contador  <<"\t" << contador << "\t" << life->spicies.size()<< endl;
		fitnessAcumm = 0.f;
		contador = 0;
		cerr << "m2" << endl;
		life->epoch();
		cerr << "m3" << endl;
		cout << "MG: " << maxGeneration << endl;
		maxGeneration = 0.f;

	}
	cout << "================================================" << endl;
	life->printInfo();
	cout << "maxFitness: " << maxFitness << endl;
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