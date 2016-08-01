#ifndef NEAT_BASICSYNAPTICWEIGHT_HPP
#define NEAT_BASICSYNAPTICWEIGHT_HPP

#include "SynapticWeight.hpp"
#include "BasicSynapticWeightUserDefinitions.hpp"
#include "Parameter.hpp"
#include <memory>

namespace NEAT
{
class BasicSynapticWeight : public SynapticWeight
{
public:
	BasicSynapticWeight( BasicSynapticWeightUserDefinitions& userdef );
	void mutate() override;
	void spread() override;
	float getOutput() override;
	float getDistance( const SynapticWeight * sw ) override;

	void printInfo();

private:

	std::unique_ptr < Parameter > weight;
	float input;
	float output;
	float constantDistanceOfSynapticWeightValue;

};
}
#endif