#ifndef NEAT_LAYER_HPP
#define NEAT_LAYER_HPP

#include <string>
#include <memory>
#include <vector>
#include "BasicNeuron.hpp"
#include "BasicNeuronUserDefinitions.hpp"

namespace NEAT
{

class Layer
{
public:
	Layer( std::unique_ptr < Neuron > seedNeuron  , unsigned int layerId );
	void addNewNeuron();
	void addNeuron( std::unique_ptr < Neuron > neuron); // se llama en cruzamientos
	void spread();
	void printInfo() const;
	std::unique_ptr < Layer > crossOver( const Layer& other) const;
	float getDistance( const Layer& other ) const;
	void mightMutate();
	std::unique_ptr <Layer> clone();

	std::vector < std::unique_ptr < Neuron > > neurons; // Por simplicidad y una mejor notacion se deje neurons en publico

private:
	std::unique_ptr < Neuron > seedNeuron;
	unsigned int layerId;
};

}


#endif