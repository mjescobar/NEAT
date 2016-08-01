#include "BasicSynapticWeight.hpp"

#include <iostream>

namespace NEAT
{

BasicSynapticWeight::BasicSynapticWeight( BasicSynapticWeightUserDefinitions& userdef )
{
	// Se crean los parametros, luego se otorgan los valores iniciales del resto de las variables importantes.
	weight = std::make_unique < Parameter > (userdef.probabilityOfWeightRandomMutation,
		userdef.maximumWeightPercentVariation,
		userdef.maxWeightValue,
		userdef.minWeightValue);
	constantDistanceOfSynapticWeightValue = userdef.constantDistanceOfSynapticWeightValue;

	input = 0.f;
	output = 0.f;
}

float BasicSynapticWeight::getDistance( const SynapticWeight * sw )
{
	const BasicSynapticWeight *  bsw = dynamic_cast < const BasicSynapticWeight * > ( sw );
	if(bsw == NULL){std::cerr << "BasicSynapticWeight::getDistance::sw is not type BasicSynapticWeight" << std::endl; exit(EXIT_FAILURE);}
	return (bsw->weight->value - this->weight->value) * constantDistanceOfSynapticWeightValue;
}

void BasicSynapticWeight::mutate()
{
	weight->mutate();
}

void BasicSynapticWeight::spread()
{ 
	output = input * weight->value; 
}

float BasicSynapticWeight::getOutput() 
{
	return output;
}

void BasicSynapticWeight::printInfo()
{
	std::cout << "weight: " << weight->value << "\tinput: " << input << "\toutput: " << output << std::endl;
}

}

