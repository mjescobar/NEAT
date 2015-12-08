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
	vector <double> output;

	double error_sum = 0;
	// XOR(0,0) -> 0
	//cerr << "========================================" << endl;
	output.push_back( organism->eval({0,0}).at(0) );
	// XOR(0,1) -> 1
	output.push_back( organism->eval({0,1}).at(0) );
	// XOR(1,0) -> 1
	output.push_back( organism->eval({1,0}).at(0) );
	// XOR(1,1) -> 0
	output.push_back( organism->eval({1,1}).at(0) );

	error_sum =  fabs( output.at( 0 ) )+ fabs( output.at( 3 ) ) + fabs( 1 - output.at( 1 ) ) + fabs( 1 - output.at( 2 ) )  ;

	cout  << "\tfitness: " << pow(4 - error_sum, 2) << "\terror_sum: " <<fabs( output.at( 0 ) )+ fabs( output.at( 3 ) ) + fabs(1 - output.at( 1 ) ) + fabs( 1 - output.at( 2 ) ) << "\t eval( 0 , 0 ): " <<  output.at(0)<< "\t eval( 0 , 1 ):" << output.at(1)<< "\t eval( 1 , 0 ):" <<  output.at(2) << "\t eval( 1 , 1 ):" <<  output.at(3)  << endl;
	output.clear();

	if ( std::isnan(pow(4 - error_sum, 2)+ 0.0000001) ) // isnan es parte de c++11
	{
		cerr << "ERROR::fitness:: fitness is not a number. The organism will be printed bellow."<< endl;	
		organism->printState();
		organism->saveState("./");
		exit(EXIT_FAILURE);
	}

	return pow(4 - error_sum, 2)+ 0.0000001;
}


int main(int argc, char** argv){

	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	GlobalInformation * ilo = new GlobalInformation();
	BasicNeuron * BN = new BasicNeuron(ilo, "./BasicNeuronUserDefinitions");
	BasicSynapticWeight * BSW = new BasicSynapticWeight("./BasicSynapticUserDefinitions" , ilo);
	Life NeatPopulation = Life("./LifeUserDef", "./NicheUserDef", BN, BN, BN, BSW, "./userDefANN", ilo );

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
	return 0 ;
}