#ifndef NEAT_TAUYNAPTICWEIGHTUSERDEF_HPP
#define NEAT_TAUYNAPTICWEIGHTUSERDEF_HPP

#include <string>
namespace NEAT
{
class TauSynapticWeightUserDefinitions
{
public:
	virtual ~TauSynapticWeightUserDefinitions();
	TauSynapticWeightUserDefinitions();
	TauSynapticWeightUserDefinitions( std::string path );

	void printInfo();

	float maxWeightValue;
	float minWeightValue;
	float maximumWeightPercentVariation;
	float probabilityOfWeightRandomMutation;
	float probabilityOfEnableADisabledConnection;
	float constantDistanceOfSynapticWeightValue;
	float mutationProbability;
	uint maxBufferSize;
	float constantDistanceOfBufferSize;
	float bufferSizeMutationRate;
};
}
#endif