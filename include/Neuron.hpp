#ifndef NEATSPIKES_NEURON_HPP
#define NEATSPIKES_NEURON_HPP

#include "SynapticWeight.hpp"
#include <vector>
#include <memory>
#include <string>

namespace NEAT
{

class Neuron // Es una clase abstracta, no se puede crear instancias de esta sino que de las que implementen sus metodos abstractos ( de forma: virtual retunr-type function ( params ) = 0; )
{

public:
	// Primero las funciones que tienen que ser sobre escritas en cualquier instancia de neurona. (BasicNeuron o SpikeNeuron por ejemplo)
	Neuron(  );
	virtual ~Neuron();
	virtual void mightMutate() = 0;
	virtual float getDistance( const Neuron * neuron ) const = 0;
	virtual void spread() = 0;
	virtual std::shared_ptr < Neuron > clone() const = 0;
	virtual std::shared_ptr < Neuron > createNew() const = 0; 
	virtual void save( const std::string path) const = 0;

	// Las siguientes son opcionales si se desea o no sobre escribirlas
	virtual void printInfo() const {};
	virtual void sumIncomingVoltage(float inputVoltage);
	virtual float getOutput() const;
	virtual void receiveInccommingVoltage(); 
	virtual void sendVoltageToOutcomingSynapticWeights();
	virtual std::shared_ptr < Neuron > crossOver( const Neuron & other ) const;


	// Manejo de las conexiones entrantes y salientes.
	void addIncomingSynapticWeight( std::shared_ptr < SynapticWeight > incomingSynapticWeight );
	void addOutcomingSynapticWeight( std::shared_ptr < SynapticWeight > outcomingSynapticWeight );
	const std::vector < std::shared_ptr < SynapticWeight > >& getIncomingSynapticWeights();
	const std::vector < std::shared_ptr < SynapticWeight > >& getOutcomingSynapticWeights();

protected:

	float inputVoltageAccum;
	float output;	
	// Cada neurona tiene una coleccion de conexiones sinapticas entrantes y salientes
	std::vector< std::shared_ptr < SynapticWeight > > incomingSynapticWeights;
	std::vector< std::shared_ptr < SynapticWeight > > outcomingSynapticWeights;
	

};

}
#endif
