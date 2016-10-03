#ifndef NEAT_BASICNEURONUSERDEFNITIONS_HPP
#define NEAT_BASICNEURONUSERDEFNITIONS_HPP

#include <string>


namespace NEAT
{

class BasicNeuronUserDefinitions
{
public:
	BasicNeuronUserDefinitions();
	BasicNeuronUserDefinitions( std::string path );
	void printInfo();

	//Se mantienen publicos por simplicidad en su uso.
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