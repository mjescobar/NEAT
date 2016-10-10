#ifndef NEAT_CPPNEURONUSERDEFNITIONS_HPP
#define NEAT_CPPNEURONUSERDEFNITIONS_HPP

#include <string>


namespace NEAT
{

class CPPNNeuronUserDefinitions
{
public:
	virtual ~CPPNNeuronUserDefinitions();
	CPPNNeuronUserDefinitions();
	CPPNNeuronUserDefinitions( std::string path );
	void printInfo();

	//Se mantienen publicos por simplicidad en su uso.
	float maxBias;
	float minBias;
	float maximumBiasPercentVariation;
	float probabilityOfBiasRandomMutation;
	float constantDistanceOfBias;
	float maxAmplifier;
	float minAmplifier;
	float probabilityOfAmplifierRandomMutation;
	float maximumAmplifierPercentVariation;
	float constantDistanceOfAmplifier;
	float mutationProbability;
	float changeFunctionRate;
	float constantDistanceOfFunction;
};

}

#endif