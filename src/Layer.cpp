#include "Layer.hpp"
#include <iostream>

//Para simplificar la notacion se definira el preambulo completo en una notacion mas corta.


namespace NEAT
{

Layer::Layer ( std::shared_ptr < Neuron > seedNeuron, unsigned int layerId) 
{
	this->seedNeuron = std::move( seedNeuron );
	this->layerId = layerId;
}


void Layer::spread()
{
	for (unsigned int i = 0; i < neurons.size(); ++i)
	{
		neurons.at(i)->spread();
	}
}

void Layer::addNewNeuron()
{
	neurons.push_back( seedNeuron->createNew()  ) ;
}


void Layer::addNeuron( std::unique_ptr < Neuron > neuron)
{
	neurons.push_back( std::move( neuron ) );
}


void Layer::printInfo() const
{
	std::cout << "LAYER ID: " << layerId << std::endl;
	for (unsigned int i = 0; i < neurons.size(); ++i)
	{
		std::cout << "Neuron place: " << i << std::endl;
		neurons.at(i)->printInfo();
	}
}


std::unique_ptr < Layer > Layer::crossOver( const Layer& other) const // has to be exact equals, not possible crossOver with another species
{
	auto result = std::make_unique <Layer>( seedNeuron, layerId );

	// 50% probabilidad de heredar la neurona de cualquiera de sus padres. neurona a neurona.
	for (uint i = 0; i < this->neurons.size(); ++i)
	{
		( rand()/(double)RAND_MAX > 0.5 ) ? result->neurons.push_back(std::move(this->neurons.at(i)->clone())) : result->neurons.push_back(std::move(other.neurons.at(i)->clone()));
	}	
	return std::move(result);
}


void Layer::mightMutate()
{
	for( auto& neuron : neurons  )
	{
		neuron->mightMutate();
	}
}

float Layer::getDistance( const Layer& other ) const
{
	auto result = 0.f;

	for (uint i = 0; i < this->neurons.size(); ++i)
	{
		result += this->neurons.at(i)->getDistance( other.neurons.at(i).get() );
	}	
	return std::move(result);
}


}
