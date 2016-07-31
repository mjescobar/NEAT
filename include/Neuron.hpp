#ifndef NEATSPIKES_NEURON_HPP
#define NEATSPIKES_NEURON_HPP

#include "PlatformDefinitions.hpp"
#include "SynapticWeight.hpp"
#include <vector>
#include <memory>

namespace NEAT
{

class Neuron
{

public:
	// Primero las funciones que tienen que ser sobre escritas en cualquier instancia de neurona. (BasicNeuron o SpikeNeuron por ejemplo)
	Neuron(  );
	virtual void mutate() = 0;
	virtual void sumIncomingVoltage(float inputVoltage) = 0;
	virtual float getDistance( const Neuron& neuron ) = 0;
	virtual float eval() = 0;

	// Manejo de las conexiones entrantes y salientes.
	void addIncomingSynapticWeight( std::shared_ptr < SynapticWeight > incomingSynapticWeight );
	void addOutcomingSynapticWeight( std::shared_ptr < SynapticWeight > outcomingSynapticWeight );
	const std::vector < std::shared_ptr < SynapticWeight > >& getIncomingSynapticWeights();
	const std::vector < std::shared_ptr < SynapticWeight > >& getOutcomingSynapticWeights();

private:
	
	// Cada neurona tiene una coleccion de conexiones sinapticas entrantes y salientes
	std::vector< std::shared_ptr < SynapticWeight > > incomingSynapticWeights;
	std::vector< std::shared_ptr < SynapticWeight > > outcomingSynapticWeights;



};

}
#endif