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
	virtual ~BasicNeuron();
	BasicNeuron();
	BasicNeuron( const BasicNeuronUserDefinitions&  basicNeuronUserDefinitions );
	BasicNeuron( const BasicNeuron & );

	void mightMutate() override;
	float getDistance( const Neuron * neuron ) const override;
	void spread() override;
	std::shared_ptr < Neuron > clone()  const override; //usado en cruzamiento
	void printInfo() const override;
	std::shared_ptr < Neuron > createNew() const override; // usado en mutacion
	void save( const std::string path) const ;

private:
	std::shared_ptr < Parameter > bias;
	std::shared_ptr < Parameter > sigmoidConstant;
	float constantDistanceOfBias;
	float constantDistanceOfSigmoidConstant;
	float lastInputAccum;
	float mutationProbability;
};

}

#endif
