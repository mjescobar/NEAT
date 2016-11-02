#ifndef NEAT_TAUYNAPTICWEIGHT_HPP
#define NEAT_TAUYNAPTICWEIGHT_HPP


#include "SynapticWeight.hpp"
#include "TauSynapticWeightUserDefinitions.hpp"
#include "Parameter.hpp"
#include <deque>

namespace NEAT
{
class TauSynapticWeight : public SynapticWeight
{
public:
	TauSynapticWeight();
	TauSynapticWeight( const TauSynapticWeightUserDefinitions& userdef );
	TauSynapticWeight( const TauSynapticWeight& other );
	virtual ~TauSynapticWeight();
	void mightMutate() override;
	void spread() override;
	float getDistance( const SynapticWeight * sw ) const override;
	std::shared_ptr < SynapticWeight > clone() const override;
	std::shared_ptr < SynapticWeight > createNew() const override;
	void printInfo() const;
	void save( const std::string path) const;
private:
	std::deque <float> voltageQueue;
	std::shared_ptr < Parameter > weight;
	uint maxBufferSize;
	float constantDistanceOfSynapticWeightValue;
	float mutationProbability;
	uint bufferSize;
	float constantDistanceOfBufferSize;
	float bufferSizeMutationRate;
};
}
#endif
