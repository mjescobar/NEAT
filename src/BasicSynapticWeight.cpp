#include "BasicSynapticWeight.hpp"

#include <iostream>
#include <cmath> //fabs
#include <iomanip>

using namespace std;
namespace NEAT
{

BasicSynapticWeight::~BasicSynapticWeight()
{
	
}

BasicSynapticWeight::BasicSynapticWeight():BasicSynapticWeight(BasicSynapticWeightUserDefinitions()){};

BasicSynapticWeight::BasicSynapticWeight(const BasicSynapticWeightUserDefinitions& userdef )
{
	// Se crean los parametros, luego se otorgan los valores iniciales del resto de las variables importantes.
	weight = make_unique < Parameter > (userdef.probabilityOfWeightRandomMutation,
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
	if(bsw == NULL){cerr << "BasicSynapticWeight::getDistance::sw is not type BasicSynapticWeight" << endl; exit(EXIT_FAILURE);}
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
	cout << "weight: " << weight->value << setprecision(6) << "\tHin(L,N): {" << layerInput << "," << neuronPlaceInLayerVector_IN <<"}" << "\tHout(L,N): {" << layerOutput << "," << neuronPlaceInLayerVector_OUT <<"}"  << "\tinput: " << input << "\toutput: " << output << endl;
}

shared_ptr < SynapticWeight > BasicSynapticWeight::clone() const
{
	return move( make_unique < BasicSynapticWeight > ( *this )  );
}

shared_ptr < SynapticWeight > BasicSynapticWeight::createNew() const
{
	auto tmp = make_unique < BasicSynapticWeight > ( *this );
	tmp->weight->random();
	return move( tmp );
}

void BasicSynapticWeight::save( const string path) const
{
	ofstream file;
	file.open(path);
	if(file.is_open())
	{
		file << 
		"weight " << weight->value  << endl;
		saveBaseData(file);
		file.close();
	}
	else
	{
		cerr << "BasicNeuron::save::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}

void BasicSynapticWeight::load( const std::string path )
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
		cerr << "ERROR::BasicSynapticWeight::load::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}

} // end namespace NEAT
