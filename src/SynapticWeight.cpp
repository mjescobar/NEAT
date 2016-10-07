#include "SynapticWeight.hpp"
#include <cstdlib> //rand
#include <iostream>
namespace NEAT
{

SynapticWeight::SynapticWeight()
{

}

SynapticWeight::~SynapticWeight()
{
	
}

void SynapticWeight::cloneBaseData( const SynapticWeight& other )
{
	this->neuronPlaceInLayerVector_IN =  other.neuronPlaceInLayerVector_IN;
	this->layerInput = other.layerInput;
	this->neuronPlaceInLayerVector_OUT = other.neuronPlaceInLayerVector_OUT;
	this->layerOutput = other.layerOutput;
}

void SynapticWeight::setMark( uint neuronPlaceInLayerVector_IN, 
		uint layerInput,
		uint neuronPlaceInLayerVector_OUT,
		uint layerOutput  
			)
{
	this->neuronPlaceInLayerVector_IN =  neuronPlaceInLayerVector_IN;
	this->layerInput = layerInput;
	this->neuronPlaceInLayerVector_OUT = neuronPlaceInLayerVector_OUT;
	this->layerOutput = layerOutput;
}

std::tuple < uint, uint, uint, uint > SynapticWeight::getMark() const
{
	return std::make_tuple(neuronPlaceInLayerVector_IN, layerInput, neuronPlaceInLayerVector_OUT, layerOutput);
}

std::shared_ptr < SynapticWeight > SynapticWeight::crossOver( const SynapticWeight& other ) const
{
	return std::move( ( rand()/(double)RAND_MAX > 0.5 ) ? this->clone(): other.clone()); // 50% de probabilidades
}

float SynapticWeight::getOutput() const
{
	return output;
}

void SynapticWeight::setInput( float inputVoltage )
{
	input = inputVoltage;
} 


}
