#ifndef NEAT_ANNUSERDEF_HPP
#define NEAT_ANNUSERDEF_HPP



#include <string>

namespace NEAT
{

class ANNUserDefinitions
{
public:
	virtual ~ANNUserDefinitions();
	ANNUserDefinitions();
	ANNUserDefinitions(  std::string path  );

	void printInfo();

	float probabilityNewNeuronInLayer;
	float probabilityOfNewSynapticWeight;
	float probabilityOfNewUniqueSynapticWeight;
	unsigned int inputsAmount;
	unsigned int outputsAmount;
	bool useBackwardConnections;
	float probabilityOfNewLayer;
	

};

}


#endif