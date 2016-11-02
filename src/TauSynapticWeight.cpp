#include "TauSynapticWeight.hpp"

#include <iostream>
#include <cmath> //fabs

using namespace std;

namespace NEAT
{

TauSynapticWeight::~TauSynapticWeight()
{
}

TauSynapticWeight::TauSynapticWeight():TauSynapticWeight(TauSynapticWeightUserDefinitions()){}

TauSynapticWeight::TauSynapticWeight(const TauSynapticWeightUserDefinitions& userdef )
{
	// Se crean los parametros, luego se otorgan los valores iniciales del resto de las variables importantes.
	weight = make_unique < Parameter > (userdef.probabilityOfWeightRandomMutation,
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
	if(tsw == NULL){cerr << "TauSynapticWeight::getDistance::sw is not type TauSynapticWeight" << endl; exit(EXIT_FAILURE);}
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
	cout << "weight: " << weight->value << "\tbufferSize: " << bufferSize << "\tHin(L,N): {" << layerInput << "," << neuronPlaceInLayerVector_IN <<"}" << "\tHout(L,N): {" << layerOutput << "," << neuronPlaceInLayerVector_OUT <<"}"  << "\tinput: " << input << "\toutput: " << output << endl;
}

shared_ptr < SynapticWeight > TauSynapticWeight::clone() const
{
	return move( make_unique < TauSynapticWeight > ( *this )  );
}

shared_ptr < SynapticWeight > TauSynapticWeight::createNew() const
{
	auto tmp = make_unique < TauSynapticWeight > ( *this );
	tmp->weight->random();
	tmp->bufferSize = round(maxBufferSize*( rand()/(double)RAND_MAX ) );
	return move( tmp );
}

void TauSynapticWeight::save( const string path) const
{
	ofstream file(path);
	if(file.is_open())
	{
		file << 
			"weight " << weight->value  << endl <<
			"bufferSize " << bufferSize << endl;
		saveBaseData(file);	

		file.close();
	}
	else
	{
		cerr << "BasicNeuron::save::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}

void TauSynapticWeight::load( const std::string path )
{
	ifstream file;
	file.open(path, ios::in);
	string line;
	if (file.is_open())
	{
		while ( getline (file,line, char(' ') ) )
		{
			if (line.compare("weight") == 0)
			{
				getline (file,line, char('\n') );
				weight->value =  stof(line);
			}
			else if (line.compare("bufferSize") == 0)
			{
				getline (file,line, char('\n') );
				bufferSize =  stof(line);
			}
			else if (line.compare("neuronPlaceInLayerVector_IN") == 0)
			{
				getline (file,line, char('\n') );
				neuronPlaceInLayerVector_IN =  stoul(line);
			}
			else if (line.compare("layerInput") == 0)
			{
				getline (file,line, char('\n') );
				layerInput =  stoul(line);
			}
			else if (line.compare("neuronPlaceInLayerVector_OUT") == 0)
			{
				getline (file,line, char('\n') );
				neuronPlaceInLayerVector_OUT =  stoul(line);
			}
			else if (line.compare("layerOutput") == 0)
			{
				getline (file,line, char('\n') );
				layerOutput =  stoul(line);
			}
		}
		file.close();
	}
	else
	{
		cerr << "ERROR::TauSynapticWeight::load::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}

} // end namespace NEAT
