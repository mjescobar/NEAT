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
	LIFNeuron();
	LIFNeuron( const LIFNeuronUserDefinitions&  LIFNeuronUserDefinitions );
	LIFNeuron( const LIFNeuron & );

	void mightMutate() override;
	float getDistance( const Neuron * neuron ) const override;
	void spread() override;
	std::unique_ptr < Neuron > clone()  const override; //usado en cruzamiento
	void printInfo() const override;
	std::unique_ptr < Neuron > createNew() const override; // usado en mutacion
	void spike();
	void simulateNextCurrent();

private:
	std::unique_ptr < Parameter > bias;
	std::unique_ptr < Parameter > sigmoidConstant;
	float mutateProbability;
	float lastInputAccum;
	float samplingDelaTime;
	float resistence;
	float membraneTimeConstant; // tau_m 
	float outputCurrent;
	float membranVoltage;
	float spikeThreshold;
	float resetVoltage;
	float currentTimeConstant;
};

}

#endif