#ifndef NEAT_BASIC_NEURON_HPP
#define NEAT_BASIC_NEURON_HPP

#include "Neuron.hpp"
#include <vector>
#include "BasicNeuron/BasicNeuronUserDefinitions.hpp"
#include "Parameter.hpp"

namespace NEAT
{

class BasicNeuron : public Neuron
{
public:
	BasicNeuron( std::shared_ptr < BasicNeuronUserDefinitions > basicNeuronUserDefinitions );

	void mutate() override;
	void sumIncomingVoltage(float inputVoltage) override;
	float getDistance( const Neuron& neuron ) override;
	float eval() override;

private:
	std::shared_ptr < BasicNeuronUserDefinitions > basicNeuronUserDefinitions;
	std::unique_ptr < Parameter > bias;
	std::unique_ptr < Parameter > sigmoidConstant;
	float inputVoltageAccum;
	float lastOutputVoltage;
};

}

#endif