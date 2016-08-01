#include "BasicNeuron.hpp"

#include <cmath> // exp fabs
#include <iostream>

namespace NEAT
{

BasicNeuron::BasicNeuron( const BasicNeuronUserDefinitions& basicNeuronUserDefinitions )
{
	inputVoltageAccum = 0.f;
	lastOutputVoltage = 0.f;
	constantDistanceOfBias = basicNeuronUserDefinitions.constantDistanceOfBias;
	constantDistanceOfSigmoidConstant = basicNeuronUserDefinitions.constantDistanceOfSigmoidConstant;

	if ( basicNeuronUserDefinitions.useBias )
	{
			bias = std::make_unique < Parameter > ( basicNeuronUserDefinitions.probabilityOfBiasRandomMutation,
				basicNeuronUserDefinitions.maximumBiasPercentVariation,
				basicNeuronUserDefinitions.maxBias,
				basicNeuronUserDefinitions.minBias
				 ) ;
	}
	else
	{
		bias = std::make_unique < Parameter > ( basicNeuronUserDefinitions.predefinedBias );
	}

	if ( basicNeuronUserDefinitions.useSigmoidConstant )
	{
			sigmoidConstant = std::make_unique < Parameter > ( basicNeuronUserDefinitions.probabilityOfSigmoidConstantRandomMutation,
				basicNeuronUserDefinitions.maximumSigmoidConstantPercentVariation,
				basicNeuronUserDefinitions.maxSigmoidConstant,
				basicNeuronUserDefinitions.minSigmoidConstant
				 ) ;
	}
	else
	{
		sigmoidConstant = std::make_unique < Parameter > ( basicNeuronUserDefinitions.predefinedSigmoidConstant );
	}
}


void BasicNeuron::mutate()
{
	// Recordar que quien decide si una neurona muta o no muta no es la misma neurona sino que la clase ANN
	// Por lo mismo es quien calcula la probabilidad de mutar de esta neurona.
	bias->mutate();
	sigmoidConstant->mutate();

}

void BasicNeuron::sumIncomingVoltage( float inputVoltage )
{
	inputVoltageAccum += inputVoltage;
}

float BasicNeuron::getDistance( const Neuron * otherNeuron )
{
	const BasicNeuron * otherBasicNeuron = dynamic_cast < const BasicNeuron *  > (  otherNeuron );
	if(otherBasicNeuron == NULL)
	{
		std::cerr << "BasicNeuron::getDistance::otherNeuron is not BasicNeuron type." << std::endl;
		exit( EXIT_FAILURE );
	}

	return this->constantDistanceOfBias * ( fabs(this->bias->value - otherBasicNeuron->bias->value)  )  + this->constantDistanceOfSigmoidConstant * ( fabs(this->sigmoidConstant->value - otherBasicNeuron->sigmoidConstant->value)  ) ;
}

float BasicNeuron::eval()
{
	float result = 2.0 / ( 1.0 + exp( -( inputVoltageAccum + bias->value ) * sigmoidConstant->value ) ) - 1.0; // Esta en el intervalo [-1,1] la salida de la neurona.
	inputVoltageAccum = 0.0; // Se descarga la neurona y su potencial de entrada vuelve a cero.
	lastOutputVoltage = result; 
	return result;
}

void BasicNeuron::printInfo()
{
	std::cout << "Bias: " << bias->value << "\tSigmoidConstant: " << sigmoidConstant->value << std::endl;
}

}