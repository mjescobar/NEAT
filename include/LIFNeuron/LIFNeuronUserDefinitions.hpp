#ifndef NEAT_LIFNEURONUSERDEFNITIONS_HPP
#define NEAT_LIFNEURONUSERDEFNITIONS_HPP

#include <string>


namespace NEAT
{

class LIFNeuronUserDefinitions
{
public:
	LIFNeuronUserDefinitions();
	LIFNeuronUserDefinitions( std::string path );
	void printInfo();

	//Se mantienen publicos por simplicidad en su uso.
	float mutateProbability;
	float maxBias;
	float minBias;
	float maximumBiasPercentVariation;
	float probabilityOfBiasRandomMutation;
	float constantDistanceOfBias;
	float maxSigmoidConstant;
	float minSigmoidConstant;
	float probabilityOfSigmoidConstantRandomMutation;
	float maximumSigmoidConstantPercentVariation;
	float constantDistanceOfSigmoidConstant;
};

}

#endif