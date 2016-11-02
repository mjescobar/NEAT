#ifndef NEAT_CPPN_NEURON_HPP
#define NEAT_CPPN_NEURON_HPP

#include <vector>

#include "Neuron.hpp"
#include "CPPNNeuronUserDefinitions.hpp"
#include "Parameter.hpp"
#include "CPPNFunction.hpp"

namespace NEAT
{

class CPPNNeuron : public Neuron
{
public:
	virtual ~CPPNNeuron();
	CPPNNeuron();
	CPPNNeuron( const CPPNNeuronUserDefinitions&  cPPNNeuronUserDefinitions );
	CPPNNeuron( const CPPNNeuron & );


	void mightMutate() override;
	float getDistance( const Neuron * neuron ) const override;
	void spread() override;
	std::shared_ptr < Neuron > clone()  const override; //usado en cruzamiento
	void printInfo() const override;
	std::shared_ptr < Neuron > createNew() const override; // usado en mutacion
	void save( const std::string path) const;

private:
	std::shared_ptr < CPPNFunction > cppnFunction;
	std::shared_ptr < Parameter > bias;
	std::shared_ptr < Parameter > Amplifier;
	float constantDistanceOfBias;
	float constantDistanceOfAmplifier;
	float lastInputAccum;
	float mutationProbability;
	float changeFunctionRate;
	float constantDistanceOfFunction;
};

}

#endif
