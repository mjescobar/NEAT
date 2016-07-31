#ifndef NEAT_SYNAPTICWEIGHT_HPP
#define NEAT_SYNAPTICWEIGHT_HPP


namespace NEAT
{
class SynapticWeight
{
public:
	// Primero las funciones que deben ser sobre escritas.
	virtual void mutate() = 0;
	virtual void spread() = 0;

};
}
#endif