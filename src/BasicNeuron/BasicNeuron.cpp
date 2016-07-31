#include "BasicNeuron/BasicNeuron.hpp"

#include <cmath> // exp

namespace NEAT
{

BasicNeuron::BasicNeuron( std::shared_ptr < BasicNeuronUserDefinitions > basicNeuronUserDefinitions )
{
	inputVoltageAccum = 0.f;
	this->basicNeuronUserDefinitions = std::move( basicNeuronUserDefinitions );
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

float BasicNeuron::getDistance( const Neuron& neuron )
{
	return 0;
}

float BasicNeuron::eval()
{
	float result = 2.0 / ( 1.0 + exp( -( inputVoltageAccum + bias->value ) * sigmoidConstant->value ) ) - 1.0; // Esta en el intervalo [-1,1] la salida de la neurona.
	inputVoltageAccum = 0.0; // Se descarga la neurona y su potencial de entrada vuelve a cero.
	lastOutputVoltage = result; 
	return result;
}

}