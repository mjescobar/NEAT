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
	virtual ~Layer();
	Layer( std::shared_ptr < Neuron > seedNeuron  , unsigned int layerId );
	void addNewNeuron();
	void addNeuron( std::shared_ptr < Neuron > neuron); // se llama en cruzamientos
	void spread();
	void printInfo() const;
	std::shared_ptr < Layer > crossOver( const Layer& other) const;
	float getDistance( const Layer& other ) const;
	void mightMutate();
	std::shared_ptr <Layer> clone();


	void save(const std::string path) const;

	std::vector < std::shared_ptr < Neuron > > neurons; // Por simplicidad y una mejor notacion se deje neurons en publico

private:
	std::shared_ptr < Neuron > seedNeuron;
	unsigned int layerId;
};

}


#endif
