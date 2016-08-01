#ifndef NEAT_SYNAPTICWEIGHT_HPP
#define NEAT_SYNAPTICWEIGHT_HPP


namespace NEAT
{
class SynapticWeight
{
public:
	// Primero las funciones que deben ser sobre escritas.
	SynapticWeight();
	~SynapticWeight();
	virtual void mutate() = 0;
	virtual void spread() = 0;
	virtual float getOutput() = 0;
	virtual float getDistance( const SynapticWeight * sw ) = 0;

	int id;
};
}
#endif