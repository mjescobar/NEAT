#ifndef NEAT_LIFNEURONUSERDEFNITIONS_HPP
#define NEAT_LIFNEURONUSERDEFNITIONS_HPP

#include <string>


namespace NEAT
{

class LIFNeuronUserDefinitions
{
public:
	LIFNeuronUserDefinitions();
	virtual ~LIFNeuronUserDefinitions();
	LIFNeuronUserDefinitions( std::string path );
	void printInfo();

	//Se mantienen publicos por simplicidad en su uso.
	float probabilityOfResistenceRandomMutation;
	float maximumResistencePercentVariation;
	float maxResistence;
	float minResistence;
	float probabilityOfMembraneTimeConstantRandomMutation;
	float maximumMembraneTimeConstantPercentVariation;
	float maxMembraneTimeConstant;
	float minMembraneTimeConstant;
	float probabilityOfCurrentTimeConstantRandomMutation;
	float maximumCurrentTimeConstantPercentVariation;
	float maxCurrentTimeConstant;
	float minCurrentTimeConstant;
	float constantDistanceResistence;
	float constantDistanceMembraneTimeConstant;
	float constantDistanceCurrentTimeConstant;
	float resetVoltage;
	float spikeThreshold;
	float mutationProbability;
};

}

#endif