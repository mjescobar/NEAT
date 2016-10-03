#include "LIFNeuron.hpp"

#include <cmath> // exp fabs
#include <iostream>

namespace NEAT
{

LIFNeuron::LIFNeuron(float samplingDelaTime) : LIFNeuron(  LIFNeuronUserDefinitions(), samplingDelaTime ){}

LIFNeuron::LIFNeuron( const LIFNeuronUserDefinitions& LIFNeuronUserDefinitions, float samplingDelaTime )
{
	outputCurrentAmpliude = 0.f;
	inputVoltageAccum = 0.f;
	output = 0.f;
	lastInputAccum = 0.f;
	this->samplingDelaTime = samplingDelaTime;

	resistence = std::make_unique < Parameter > ( LIFNeuronUserDefinitions.probabilityOfResistenceRandomMutation,
		LIFNeuronUserDefinitions.maximumResistencePercentVariation,
		LIFNeuronUserDefinitions.maxResistence,
		LIFNeuronUserDefinitions.minResistence
		 );
	
	membraneTimeConstant = std::make_unique < Parameter > ( LIFNeuronUserDefinitions.probabilityOfMembraneTimeConstantRandomMutation,
		LIFNeuronUserDefinitions.maximumMembraneTimeConstantPercentVariation,
		LIFNeuronUserDefinitions.maxMembraneTimeConstant,
		LIFNeuronUserDefinitions.minMembraneTimeConstant
		 );

	currentTimeConstant = std::make_unique < Parameter > ( LIFNeuronUserDefinitions.probabilityOfCurrentTimeConstantRandomMutation,
		LIFNeuronUserDefinitions.maximumCurrentTimeConstantPercentVariation,
		LIFNeuronUserDefinitions.maxCurrentTimeConstant,
		LIFNeuronUserDefinitions.minCurrentTimeConstant
		 );
	constantDistanceResistence = LIFNeuronUserDefinitions.constantDistanceResistence;
	constantDistanceMembraneTimeConstant = LIFNeuronUserDefinitions.constantDistanceMembraneTimeConstant;
	constantDistanceCurrentTimeConstant = LIFNeuronUserDefinitions.constantDistanceCurrentTimeConstant;
	membranVoltage = 0.f;
	timeEnlacedFromLastSpike = 0.f;
}

LIFNeuron::LIFNeuron( const LIFNeuron & other) 
{
	this->samplingDelaTime = other.samplingDelaTime;

	inputVoltageAccum = 0.f;
	lastInputAccum = 0.f;
	output = 0.f;
	currentTimeConstant = other.currentTimeConstant->clone();
	membraneTimeConstant = other.membraneTimeConstant->clone();
	resistence = other.resistence->clone();
	constantDistanceResistence = other.constantDistanceResistence;
	constantDistanceMembraneTimeConstant = other.constantDistanceMembraneTimeConstant;
	constantDistanceCurrentTimeConstant = other.constantDistanceCurrentTimeConstant;
	outputCurrentAmpliude = 0.f;
	membranVoltage = 0.f;
	timeEnlacedFromLastSpike = 0.f;
}

void LIFNeuron::mightMutate()
{
	resistence->mightMutate();
	membraneTimeConstant->mightMutate();
	currentTimeConstant->mightMutate();
}

float LIFNeuron::getDistance( const Neuron * otherNeuron ) const
{
	const LIFNeuron * otherLIFNeuron = dynamic_cast < const LIFNeuron *  > (  otherNeuron );
	if(otherLIFNeuron == NULL)
	{
		std::cerr << "LIFNeuron::getDistance::otherNeuron is not LIFNeuron type." << std::endl;
		exit( EXIT_FAILURE );
	}

	return this->constantDistanceResistence * fabs(this->resistence->value - otherLIFNeuron->resistence->value) + this->constantDistanceMembraneTimeConstant *  fabs(this->membraneTimeConstant->value - otherLIFNeuron->membraneTimeConstant->value) + this->constantDistanceCurrentTimeConstant *  fabs(this->currentTimeConstant->value - otherLIFNeuron->currentTimeConstant->value)    ;
}

void LIFNeuron::spread()
{
	receiveInccommingVoltage(); // in father class

	float tmpResult  = samplingDelaTime / membraneTimeConstant->value;
	membranVoltage = membranVoltage * (1.f - tmpResult) + tmpResult * resistence->value * inputVoltageAccum;
	inputVoltageAccum = 0.0; // Se descarga la neurona y su potencial de entrada vuelve a cero.

	if( membranVoltage > spikeThreshold )
	{
		spike();
		membranVoltage = resetVoltage;
	}
	simulateNextCurrent();
	output = outputCurrent; // output is used in sendVoltageToOutcomingSynapticWeights by the abstract father class 'Neuron'
	sendVoltageToOutcomingSynapticWeights();
}

void LIFNeuron::printInfo() const
{
	
}

std::unique_ptr < Neuron > LIFNeuron::clone() const 
{
	return std::move( std::make_unique < LIFNeuron > (*this) );
}

std::unique_ptr < Neuron > LIFNeuron::createNew() const
{
	auto tmp = std::make_unique < LIFNeuron > (*this);
	tmp->resistence->random();
	tmp->membraneTimeConstant->random();
	tmp->currentTimeConstant->random();
	return std::move( tmp );
}

void LIFNeuron::spike()
{
	outputCurrent = outputCurrent + 1.f;
	outputCurrentAmpliude = outputCurrent;
	timeEnlacedFromLastSpike = 0.f;
}

void LIFNeuron::simulateNextCurrent()
{
	timeEnlacedFromLastSpike += samplingDelaTime;
	outputCurrent = outputCurrentAmpliude * exp( -timeEnlacedFromLastSpike/currentTimeConstant->value );
}

} // End namespace NEAT