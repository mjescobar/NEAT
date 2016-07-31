#ifndef NEAT_BASICNEURONUSERDEFNITIONS_HPP
#define NEAT_BASICNEURONUSERDEFNITIONS_HPP

#include <string>

namespace NEAT
{

class BasicNeuronUserDefinitions
{
public:
	BasicNeuronUserDefinitions( std::string path );

	//Se mantienen publicos por simplicidad en su uso.
	bool useBias;
	bool useSigmoidConstant;
	float maxBias;
	float minBias;
	float maximumBiasVariation;
	float probabilityOfBiasRandomMutation;
	float constantDistanceOfBias;
	float maxSigmoidConstant;
	float minSigmoidConstant;
	float probabilityOfSigmoidConstantRandomMutation;
	float maximumSigmoidConstantVariation;
	float constantDistanceOfSigmoidConstant;
	float predefinedBias;
	float predefinedSigmoidConstat;
};

}

#endif