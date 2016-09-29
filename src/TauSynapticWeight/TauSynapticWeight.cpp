#include "TauSynapticWeight.hpp"

#include <iostream>
#include <cmath> //fabs

namespace NEAT
{

TauSynapticWeight::TauSynapticWeight():TauSynapticWeight(TauSynapticWeightUserDefinitions()){};

TauSynapticWeight::TauSynapticWeight(const TauSynapticWeightUserDefinitions& userdef )
{
	// Se crean los parametros, luego se otorgan los valores iniciales del resto de las variables importantes.
	weight = std::make_unique < Parameter > (userdef.probabilityOfWeightRandomMutation,
		userdef.maximumWeightPercentVariation,
		userdef.maxWeightValue,
		userdef.minWeightValue);

	constantDistanceOfSynapticWeightValue = userdef.constantDistanceOfSynapticWeightValue;
	mutationProbability = userdef.mutationProbability;
	maxBufferSize = userdef.maxBufferSize;
	constantDistanceOfBufferSize = userdef.constantDistanceOfBufferSize;
	bufferSizeMutationRate = userdef.bufferSizeMutationRate;
	
	bufferSize = round(maxBufferSize*( rand()/(double)RAND_MAX ) );

	for(uint i= 0; i< bufferSize; i++) // Se inicializan los buffers con ceros.
	{
		voltageQueue.push_back(0.f);
	}
  
	input = 0.f;
	output = 0.f;
}

TauSynapticWeight::TauSynapticWeight( const TauSynapticWeight& other )
{
	cloneBaseData( other );
	constantDistanceOfSynapticWeightValue = other.constantDistanceOfSynapticWeightValue;
	mutationProbability = other.mutationProbability;
	input = 0.f;
	output = 0.f;
	weight = other.weight->clone();
	maxBufferSize = other.maxBufferSize;
	constantDistanceOfBufferSize = other.constantDistanceOfBufferSize;
	bufferSizeMutationRate = other.bufferSizeMutationRate;
	bufferSize = other.bufferSize;

}

float TauSynapticWeight::getDistance( const SynapticWeight * sw ) const
{
	const TauSynapticWeight *  tsw = dynamic_cast < const TauSynapticWeight * > ( sw );
	if(tsw == NULL){std::cerr << "TauSynapticWeight::getDistance::sw is not type TauSynapticWeight" << std::endl; exit(EXIT_FAILURE);}
	return fabs(tsw->weight->value - this->weight->value) * constantDistanceOfSynapticWeightValue + fabs(tsw->bufferSize - this->bufferSize) * constantDistanceOfBufferSize;
}

void TauSynapticWeight::mightMutate()
{
	if( rand()/(double)RAND_MAX < mutationProbability)
	{
		weight->mightMutate();
	}

	if( rand()/(double)RAND_MAX < bufferSizeMutationRate)
	{
		bufferSize = round(maxBufferSize*( rand()/(double)RAND_MAX ) );
	}

}

void TauSynapticWeight::spread()
{ 
	voltageQueue.push_back(input);
	output = voltageQueue.front()* weight->value;
	voltageQueue.pop_front();
}

void TauSynapticWeight::printInfo() const
{
	std::cout << "weight: " << weight->value << "\tbufferSize: " << bufferSize << "\tHin(L,N): {" << layerInput << "," << neuronPlaceInLayerVector_IN <<"}" << "\tHout(L,N): {" << layerOutput << "," << neuronPlaceInLayerVector_OUT <<"}"  << "\tinput: " << input << "\toutput: " << output << std::endl;
}

std::unique_ptr < SynapticWeight > TauSynapticWeight::clone() const
{
	return std::move( std::make_unique < TauSynapticWeight > ( *this )  );
}

std::unique_ptr < SynapticWeight > TauSynapticWeight::createNew() const
{
	auto tmp = std::make_unique < TauSynapticWeight > ( *this );
	tmp->weight->random();
	tmp->bufferSize = round(maxBufferSize*( rand()/(double)RAND_MAX ) );
	return std::move( tmp );
}

} // end namespace NEAT