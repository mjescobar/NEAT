#ifndef NEAT_SYNAPTICWEIGHT_HPP
#define NEAT_SYNAPTICWEIGHT_HPP

#include <memory>
#include <tuple>

namespace NEAT
{
class SynapticWeight // Es una clase abstracta, no se puede crear instancias de esta sino que de las que implementen sus metodos abstractos ( de forma: virtual retunr-type function ( params ) = 0; )
{
public:
	// Primero las funciones que deben ser sobre escritas.
	SynapticWeight();
	virtual ~SynapticWeight();
	virtual void mightMutate() = 0;
	virtual void spread() = 0;
	virtual float getDistance( const SynapticWeight * sw ) const = 0;
	virtual void printInfo() const = 0;
	virtual std::shared_ptr < SynapticWeight > clone() const = 0;
	virtual std::shared_ptr < SynapticWeight > createNew() const = 0;


	//Las que pueden ser sobre escritas
	virtual float getOutput() const ;
	virtual void setInput( float inputVoltage );
	virtual std::shared_ptr < SynapticWeight > crossOver( const SynapticWeight& other ) const;


	void cloneBaseData( const SynapticWeight& other );

	void setMark( uint neuronPlaceInLayerVector_IN, 
			uint layerInput,
			uint neuronPlaceInLayerVector_OUT,
			uint layerOutput  
				);

	std::tuple < uint, uint, uint, uint > getMark() const;

	uint neuronPlaceInLayerVector_IN;
	uint layerInput;
	uint neuronPlaceInLayerVector_OUT;
	uint layerOutput;

protected:
	float input;
	float output;


};
}
#endif
