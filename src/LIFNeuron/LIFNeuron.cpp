#include "LIFNeuron.hpp"

#include <cmath> // exp fabs
#include <iostream>

namespace NEAT
{

LIFNeuron::LIFNeuron() : LIFNeuron(  LIFNeuronUserDefinitions() ){}

LIFNeuron::LIFNeuron( const LIFNeuronUserDefinitions& LIFNeuronUserDefinitions )
{
	inputVoltageAccum = 0.f;
	output = 0.f;
	constantDistanceOfBias = LIFNeuronUserDefinitions.constantDistanceOfBias;
	constantDistanceOfSigmoidConstant = LIFNeuronUserDefinitions.constantDistanceOfSigmoidConstant;
	mutateProbability = LIFNeuronUserDefinitions.mutateProbability;

	bias = std::make_unique < Parameter > ( LIFNeuronUserDefinitions.probabilityOfBiasRandomMutation,
		LIFNeuronUserDefinitions.maximumBiasPercentVariation,
		LIFNeuronUserDefinitions.maxBias,
		LIFNeuronUserDefinitions.minBias
		 );
	
	sigmoidConstant = std::make_unique < Parameter > ( LIFNeuronUserDefinitions.probabilityOfSigmoidConstantRandomMutation,
		LIFNeuronUserDefinitions.maximumSigmoidConstantPercentVariation,
		LIFNeuronUserDefinitions.maxSigmoidConstant,
		LIFNeuronUserDefinitions.minSigmoidConstant
		 );
	lastInputAccum = 0.f;
}

LIFNeuron::LIFNeuron( const LIFNeuron & other) 
{
	inputVoltageAccum = 0.f;
	lastInputAccum = 0.f;
	output = 0.f;
	constantDistanceOfBias = other.constantDistanceOfBias;
	constantDistanceOfSigmoidConstant = other.constantDistanceOfSigmoidConstant;
	mutateProbability = other.mutateProbability;
	bias = other.bias->clone();
	sigmoidConstant = other.sigmoidConstant->clone();
}

void LIFNeuron::mightMutate()
{
	if( random()/(double)RAND_MAX < mutateProbability )
	{
		bias->mightMutate();
		sigmoidConstant->mightMutate();
	}
}

float LIFNeuron::getDistance( const Neuron * otherNeuron ) const
{
	const LIFNeuron * otherLIFNeuron = dynamic_cast < const LIFNeuron *  > (  otherNeuron );
	if(otherLIFNeuron == NULL)
	{
		std::cerr << "LIFNeuron::getDistance::otherNeuron is not LIFNeuron type." << std::endl;
		exit( EXIT_FAILURE );
	}

	return this->constantDistanceOfBias * ( fabs(this->bias->value - otherLIFNeuron->bias->value)  )  + this->constantDistanceOfSigmoidConstant * ( fabs(this->sigmoidConstant->value - otherLIFNeuron->sigmoidConstant->value)  ) ;
}

void LIFNeuron::spread()
{
	receiveInccommingVoltage(); // in father class

	float tmpResult  = samplingDelaTime/membraneTimeConstant;
	membranVoltage = membranVoltage * (1.f - tmpResult) + tmpResult * resistence * inputVoltageAccum;
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
	tmp->bias->random();
	tmp->sigmoidConstant->random();
	return std::move( tmp );
}

void LIFNeuron::spike()
{
	outputCurrent = outputCurrent + 1.f;
	outputCurrentAmpliude = outputCurrent;
	timeEnlacedFromLastSpike = 0.f;
}

void simulateNextCurrent()
{
	timeEnlacedFromLastSpike += samplingDelaTime;
	outputCurrent = outputCurrentAmpliude * exp( -timeEnlacedFromLastSpike/currentTimeConstant );
}

} // End namespace NEAT