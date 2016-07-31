
#include "Neuron.hpp"

namespace NEAT
{

Neuron::Neuron(  )
{
	
}

void Neuron::addIncomingSynapticWeight( std::shared_ptr < SynapticWeight > incomingSynapticWeight )
{
	incomingSynapticWeights.push_back( std::move( incomingSynapticWeight ) ); // Ojo que se usa std::move dado que desde el parametro ya se esta haciendo una copia de la conexion sinaptica y no se necesita hacer una copia nuevamente.
}

void Neuron::addOutcomingSynapticWeight( std::shared_ptr < SynapticWeight > outcomingSynapticWeight )
{
	outcomingSynapticWeights.push_back( std::move( outcomingSynapticWeight ) ); // Ojo que se usa std::move dado que desde el parametro ya se esta haciendo una copia de la conexion sinaptica y no se necesita hacer una copia nuevamente.
}


const std::vector < std::shared_ptr < SynapticWeight > >& Neuron::getIncomingSynapticWeights()
{
	return incomingSynapticWeights;
}


const std::vector < std::shared_ptr < SynapticWeight > >& Neuron::getOutcomingSynapticWeights()
{
	return outcomingSynapticWeights;
}

}