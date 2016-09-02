
#include "Neuron.hpp"

#include <cstdlib> //rand
#include <iostream>
namespace NEAT
{

Neuron::Neuron(  )
{
	
}

void Neuron::addIncomingSynapticWeight( std::shared_ptr < SynapticWeight > incomingSynapticWeight )
{
	incomingSynapticWeights.push_back(  incomingSynapticWeight ); // Ojo que se usa std::move dado que desde el parametro ya se esta haciendo una copia de la conexion sinaptica y no se necesita hacer una copia nuevamente.
}

void Neuron::addOutcomingSynapticWeight( std::shared_ptr < SynapticWeight > outcomingSynapticWeight )
{
	outcomingSynapticWeights.push_back(  outcomingSynapticWeight  ); // Ojo que se usa std::move dado que desde el parametro ya se esta haciendo una copia de la conexion sinaptica y no se necesita hacer una copia nuevamente.
}


const std::vector < std::shared_ptr < SynapticWeight > >& Neuron::getIncomingSynapticWeights()
{
	return incomingSynapticWeights;
}


const std::vector < std::shared_ptr < SynapticWeight > >& Neuron::getOutcomingSynapticWeights()
{
	return outcomingSynapticWeights;
}

void Neuron::sumIncomingVoltage(float inputVoltage)
{
	inputVoltageAccum += inputVoltage;
}

float Neuron::getOutput() const
{
	return output;
}


void Neuron::receiveInccommingVoltage()
{
	for ( auto SW : incomingSynapticWeights )
	{
		SW->spread();
		sumIncomingVoltage( SW->getOutput() );
	}
}

void Neuron::sendVoltageToOutcomingSynapticWeights()
{
	for ( auto SW : outcomingSynapticWeights )
	{
		SW->setInput( output );
	}
}

std::unique_ptr < Neuron > Neuron::crossOver( const Neuron & other ) const
{
	return std::move( ( rand()/(double)RAND_MAX > 0.5 ) ? this->clone(): other.clone()); // 50% de probabilidades
}


}