#include "Layer.hpp"
#include <iostream>
#include <fstream>
//Para simplificar la notacion se definira el preambulo completo en una notacion mas corta.

using namespace std;

namespace NEAT
{

Layer::~Layer()
{

}

Layer::Layer ( shared_ptr < Neuron > seedNeuron, unsigned int layerId) 
{
	this->seedNeuron = move( seedNeuron );
	this->layerId = layerId;
}


void Layer::spread()
{
	for (unsigned int i = 0; i < neurons.size(); ++i)
	{
		neurons.at(i)->spread();
	}
}

void Layer::addNewNeuron()
{
	neurons.push_back( seedNeuron->createNew()  ) ;
}


void Layer::addNeuron( shared_ptr < Neuron > neuron)
{
	neurons.push_back( move( neuron ) );
}


void Layer::printInfo() const
{
	cout << "LAYER ID: " << layerId << endl;
	for (unsigned int i = 0; i < neurons.size(); ++i)
	{
		cout << "Neuron place: " << i << endl;
		neurons.at(i)->printInfo();
	}
}


shared_ptr < Layer > Layer::crossOver( const Layer& other) const // has to be exact equals, not possible crossOver with another species
{
	auto result = make_unique <Layer>( move(seedNeuron->clone()), layerId );

	// 50% probabilidad de heredar la neurona de cualquiera de sus padres. neurona a neurona.
	for (uint i = 0; i < this->neurons.size(); ++i)
	{
		( rand()/(double)RAND_MAX > 0.5 ) ? result->neurons.push_back(move(this->neurons.at(i)->clone())) : result->neurons.push_back(move(other.neurons.at(i)->clone()));
	}	
	return move(result);
}


void Layer::mightMutate()
{
	for( auto& neuron : neurons  )
	{
		neuron->mightMutate();
	}
}

float Layer::getDistance( const Layer& other ) const
{
	auto result = 0.f;

	for (uint i = 0; i < this->neurons.size(); ++i)
	{
		result += this->neurons.at(i)->getDistance( other.neurons.at(i).get() );
	}	
	return move(result);
}

shared_ptr <Layer> Layer::clone()
{
	auto result = make_unique <Layer>( move(seedNeuron->clone()), layerId );

	// 50% probabilidad de heredar la neurona de cualquiera de sus padres. neurona a neurona.
	for (uint i = 0; i < this->neurons.size(); ++i)
	{
		result->neurons.push_back(move(this->neurons.at(i)->clone())) ;
	}	
	return move(result);
}

void Layer::save(const string path) const
{
	ofstream file;
	file.open(path);
	if(file.is_open())
	{
		uint n_id = 0;
		for( auto& neuron : neurons  )
		{
			neuron->save( path + "N" + to_string(n_id));
			//Adding to de table
			file << "N " << n_id << endl;
			n_id++;	
		}
	}
	else
	{
		cerr << "Layer::save::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}

	file.close();

}

void Layer::load(std::string path)
{
	ifstream file;
	file.open(path, ios::in);
	string line;
	if (file.is_open())
	{
		while ( getline (file,line, char(' ') ) )
		{
			if (line.compare("N") == 0)
			{
				getline (file,line, char('\n') );
				uint neuronId = stoul(line);
				auto neuron = seedNeuron->clone();
				neuron->load(path+"N"+to_string(neuronId));
				addNeuron(move(neuron));
			}
		}
		file.close();
	}
	else
	{
		cerr << "ERROR::Layer::load::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}

} // END NAMESPACE NEAT
