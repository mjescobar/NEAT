#ifndef NEAT_LIF_NEURON_HPP
#define NEAT_LIF_NEURON_HPP

#include <vector>

#include "Neuron.hpp"
#include "LIFNeuronUserDefinitions.hpp"
#include "Parameter.hpp"

namespace NEAT
{

class LIFNeuron : public Neuron
{
public:
	LIFNeuron( float samplingDelaTime);
	LIFNeuron( const LIFNeuronUserDefinitions&  LIFNeuronUserDefinitions, float samplingDelaTime );
	LIFNeuron( const LIFNeuron & );
	virtual ~LIFNeuron();

	void mightMutate() override;
	float getDistance( const Neuron * neuron ) const override;
	void spread() override;
	std::shared_ptr < Neuron > clone()  const override; //usado en cruzamiento
	void printInfo() const override;
	std::shared_ptr < Neuron > createNew() const override; // usado en mutacion
	void spike();
	void simulateNextCurrent();
	float getMembraneVoltage();
	void save( const std::string path) const;


private:
	std::shared_ptr < Parameter > resistence;
	std::shared_ptr < Parameter > membraneTimeConstant; // tau_m 
	std::shared_ptr < Parameter > currentTimeConstant; // tau_m 
	float lastInputAccum;
	float samplingDelaTime;
	float outputCurrent;
	float membranVoltage;
	float spikeThreshold;
	float resetVoltage;
	float outputCurrentAmpliude;
	float constantDistanceResistence;
	float constantDistanceMembraneTimeConstant;
	float constantDistanceCurrentTimeConstant;
	float timeEnlacedFromLastSpike;
	float mutationProbability;
};

}

#endif
