#include "BasicSynapticWeight.hpp"

#include <iostream>
#include <cmath> //fabs
#include <iomanip>

namespace NEAT
{

BasicSynapticWeight::~BasicSynapticWeight()
{
	
}

BasicSynapticWeight::BasicSynapticWeight():BasicSynapticWeight(BasicSynapticWeightUserDefinitions()){};

BasicSynapticWeight::BasicSynapticWeight(const BasicSynapticWeightUserDefinitions& userdef )
{
	// Se crean los parametros, luego se otorgan los valores iniciales del resto de las variables importantes.
	weight = std::make_unique < Parameter > (userdef.probabilityOfWeightRandomMutation,
		userdef.maximumWeightPercentVariation,
		userdef.maxWeightValue,
		userdef.minWeightValue);
	constantDistanceOfSynapticWeightValue = userdef.constantDistanceOfSynapticWeightValue;
	mutationProbability = userdef.mutationProbability;
	
	input = 0.f;
	output = 0.f;
}
BasicSynapticWeight::BasicSynapticWeight( const BasicSynapticWeight& other )
{
	cloneBaseData( other );
	constantDistanceOfSynapticWeightValue = other.constantDistanceOfSynapticWeightValue;
	mutationProbability = other.mutationProbability;
	input = 0.f;
	output = 0.f;
	weight = other.weight->clone();
}

float BasicSynapticWeight::getDistance( const SynapticWeight * sw ) const
{
	const BasicSynapticWeight *  bsw = dynamic_cast < const BasicSynapticWeight * > ( sw );
	if(bsw == NULL){std::cerr << "BasicSynapticWeight::getDistance::sw is not type BasicSynapticWeight" << std::endl; exit(EXIT_FAILURE);}
	return fabs(bsw->weight->value - this->weight->value) * constantDistanceOfSynapticWeightValue;
}

void BasicSynapticWeight::mightMutate()
{
	if( rand()/(double)RAND_MAX < mutationProbability)
	{
		weight->mightMutate();
	}
}

void BasicSynapticWeight::spread()
{ 
	output = input * weight->value; 
}

void BasicSynapticWeight::printInfo() const
{
	std::cout << "weight: " << weight->value << std::setprecision(6) << "\tHin(L,N): {" << layerInput << "," << neuronPlaceInLayerVector_IN <<"}" << "\tHout(L,N): {" << layerOutput << "," << neuronPlaceInLayerVector_OUT <<"}"  << "\tinput: " << input << "\toutput: " << output << std::endl;
}

std::shared_ptr < SynapticWeight > BasicSynapticWeight::clone() const
{
	return std::move( std::make_unique < BasicSynapticWeight > ( *this )  );
}

std::shared_ptr < SynapticWeight > BasicSynapticWeight::createNew() const
{
	auto tmp = std::make_unique < BasicSynapticWeight > ( *this );
	tmp->weight->random();
	return std::move( tmp );
}

} // end namespace NEAT
