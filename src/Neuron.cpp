
#include "Neuron.hpp"

#include <cstdlib> //rand
#include <iostream>

using namespace std;
namespace NEAT
{

Neuron::Neuron(  )
{
	
}

Neuron::~Neuron(  )
{
}

void Neuron::addIncomingSynapticWeight( shared_ptr < SynapticWeight > incomingSynapticWeight )
{
	incomingSynapticWeights.push_back(  incomingSynapticWeight ); // Ojo que se usa move dado que desde el parametro ya se esta haciendo una copia de la conexion sinaptica y no se necesita hacer una copia nuevamente.
}

void Neuron::addOutcomingSynapticWeight( shared_ptr < SynapticWeight > outcomingSynapticWeight )
{
	outcomingSynapticWeights.push_back(  outcomingSynapticWeight  ); // Ojo que se usa move dado que desde el parametro ya se esta haciendo una copia de la conexion sinaptica y no se necesita hacer una copia nuevamente.
}


const vector < shared_ptr < SynapticWeight > >& Neuron::getIncomingSynapticWeights()
{
	return incomingSynapticWeights;
}


const vector < shared_ptr < SynapticWeight > >& Neuron::getOutcomingSynapticWeights()
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

shared_ptr < Neuron > Neuron::crossOver( const Neuron & other ) const
{
	return move( ( rand()/(double)RAND_MAX > 0.5 ) ? this->clone(): other.clone()); // 50% de probabilidades
}


}
