#ifndef NEAT_BASICSYNAPTICWEIGHTUSERDEF_HPP
#define NEAT_BASICSYNAPTICWEIGHTUSERDEF_HPP

#include <string>
namespace NEAT
{
class BasicSynapticWeightUserDefinitions
{
public:
	BasicSynapticWeightUserDefinitions();
	BasicSynapticWeightUserDefinitions( std::string path );

	void printInfo();

	float maxWeightValue;
	float minWeightValue;
	float maximumWeightPercentVariation;
	float probabilityOfWeightRandomMutation;
	float probabilityOfEnableADisabledConnection;
	float constantDistanceOfSynapticWeightValue;
	float mutationProbability;
};
}
#endif