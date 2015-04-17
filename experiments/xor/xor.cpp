//======================
//	XOR
//======================
#include "Life.hpp"
#include "BasicNeuron.hpp"
#include "BasicSynapticWeight.hpp"

#include <ctime>

using namespace NEATSpikes;
using namespace std;

double fitness(Organism * organism){
	vector<double> output;
	vector<double> input;

	double error_sum = 0;
	//cout << organism << endl;

	// XOR(0,0) -> 0
	input.push_back(0);
	input.push_back(0);
	//cerr << "========================================" << endl;
	output = organism->eval(input);
	//cerr << "\toutput 1: (0,0)->" << output.at(0) << endl;
	error_sum += abs(output.at(0));
	
	input.clear();
	// XOR(0,1) -> 1
	input.push_back(0);
	input.push_back(1);
	output = organism->eval(input);
	//cerr << "\toutput 2: (0,1)->" << output.at(0) << endl;
	error_sum += abs(1 - abs(output.at(0)) );
	input.clear();
	// XOR(1,0) -> 1
	input.push_back(1);
	input.push_back(0);
	output = organism->eval(input);
	//cerr << "\toutput 3: (1,0)->" << output.at(0) << endl;
	error_sum += abs(1 - abs(output.at(0)) );
	input.clear();
	// XOR(1,1) -> 0
	input.push_back(1);
	input.push_back(1);
	output = organism->eval(input);
	//cerr << "\toutput 4: (1,1)->" << output.at(0) << endl;
	error_sum += abs(output.at(0));
	input.clear();
	//cerr << "\tfitness: " << pow(4 - error_sum, 2) << endl;
	return pow(4 - error_sum, 2);
}


int main(int argc, char** argv){
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	GlobalInformation * ilo = new GlobalInformation();
	BasicSynapticWeight * BSW = new BasicSynapticWeight("./BSWUserDef", ilo);
	BasicNeuron * BN = new BasicNeuron("./BNUserDef");
	Life NeatPopulation = Life("./LifeUserDef", "./NicheUserDef",BN,BSW,"./ANNUserDef",ilo );

	for (int j = 0; j < NeatPopulation.getGenerations(); ++j)
	{
		cerr << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n====================GENERACION " << j+1  << " ====================="<< endl;

		vector <Organism *>  AllNewOrganismToCalculateFiteness = NeatPopulation.getAllNewOrganisms();

		for (unsigned int i = 0; i < AllNewOrganismToCalculateFiteness.size(); ++i)
		{
			AllNewOrganismToCalculateFiteness.at(i)->setFitness( fitness( AllNewOrganismToCalculateFiteness.at( i ) )  );
		}
		
		NeatPopulation.epoch();
	}
	return 0;
}