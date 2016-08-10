#include "BasicNeuron.hpp"

#include <cmath> // exp fabs
#include <iostream>

namespace NEAT
{

BasicNeuron::BasicNeuron( const BasicNeuronUserDefinitions& basicNeuronUserDefinitions )
{
	inputVoltageAccum = 0.f;
	output = 0.f;
	constantDistanceOfBias = basicNeuronUserDefinitions.constantDistanceOfBias;
	constantDistanceOfSigmoidConstant = basicNeuronUserDefinitions.constantDistanceOfSigmoidConstant;
	mutateProbability = basicNeuronUserDefinitions.mutateProbability;

	bias = std::make_unique < Parameter > ( basicNeuronUserDefinitions.probabilityOfBiasRandomMutation,
		basicNeuronUserDefinitions.maximumBiasPercentVariation,
		basicNeuronUserDefinitions.maxBias,
		basicNeuronUserDefinitions.minBias
		 );
	
	sigmoidConstant = std::make_unique < Parameter > ( basicNeuronUserDefinitions.probabilityOfSigmoidConstantRandomMutation,
		basicNeuronUserDefinitions.maximumSigmoidConstantPercentVariation,
		basicNeuronUserDefinitions.maxSigmoidConstant,
		basicNeuronUserDefinitions.minSigmoidConstant
		 );
	lastInputAccum = 0.f;
}


BasicNeuron::BasicNeuron( const BasicNeuron & other) 
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



void BasicNeuron::mightMutate()
{
	if( random()/(double)RAND_MAX < mutateProbability )
	{
		bias->mightMutate();
		sigmoidConstant->mightMutate();
	}
}

float BasicNeuron::getDistance( const Neuron * otherNeuron ) const
{
	const BasicNeuron * otherBasicNeuron = dynamic_cast < const BasicNeuron *  > (  otherNeuron );
	if(otherBasicNeuron == NULL)
	{
		std::cerr << "BasicNeuron::getDistance::otherNeuron is not BasicNeuron type." << std::endl;
		exit( EXIT_FAILURE );
	}

	return this->constantDistanceOfBias * ( fabs(this->bias->value - otherBasicNeuron->bias->value)  )  + this->constantDistanceOfSigmoidConstant * ( fabs(this->sigmoidConstant->value - otherBasicNeuron->sigmoidConstant->value)  ) ;
}

void BasicNeuron::spread()
{
	receiveInccommingVoltage(); // in father class
	
	output = 2.0 / ( 1.0 + exp( -( inputVoltageAccum + bias->value ) * sigmoidConstant->value ) ) - 1.0; // Esta en el intervalo [-1,1] la salida de la neurona.

	lastInputAccum = inputVoltageAccum; // solo para mostrar en informaciones.
	inputVoltageAccum = 0.0; // Se descarga la neurona y su potencial de entrada vuelve a cero.
	sendVoltageToOutcomingSynapticWeights();
}

void BasicNeuron::printInfo() const
{
	std::cout << "Bias: " << bias->value << "\tSigmoidConstant: " << sigmoidConstant->value << "\tlastInputAccum: "<< lastInputAccum << "\tinputVoltageAccum: " << inputVoltageAccum << "\toutput: " << output << std::endl;
}



std::unique_ptr < Neuron > BasicNeuron::clone() const 
{
	return std::move( std::make_unique < BasicNeuron > (*this) );
}

std::unique_ptr < Neuron > BasicNeuron::createNew() const
{
	auto tmp = std::make_unique < BasicNeuron > (*this);
	tmp->bias->random();
	tmp->sigmoidConstant->random();
	return std::move( tmp );
}

}