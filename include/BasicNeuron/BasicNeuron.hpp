#ifndef NEAT_BASIC_NEURON_HPP
#define NEAT_BASIC_NEURON_HPP

#include <vector>

#include "Neuron.hpp"
#include "BasicNeuronUserDefinitions.hpp"
#include "Parameter.hpp"

namespace NEAT
{

class BasicNeuron : public Neuron
{
public:
	BasicNeuron( const BasicNeuronUserDefinitions&  basicNeuronUserDefinitions );

	void mutate() override;
	void sumIncomingVoltage(float inputVoltage) override;
	float getDistance( const Neuron * neuron ) override;
	float eval() override;

	void printInfo();

private:
	std::unique_ptr < Parameter > bias;
	std::unique_ptr < Parameter > sigmoidConstant;
	float inputVoltageAccum;
	float lastOutputVoltage;
	float constantDistanceOfBias;
	float constantDistanceOfSigmoidConstant;
};

}

#endif