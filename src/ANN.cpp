#include "ANN.hpp"
#include <fstream>
#include <climits>
#include <tuple>
#include <iostream>
#include <chrono>
#define LAYER_INITIAL 0
#define LAYER_OUTPUT UINT_MAX

using namespace std;
namespace NEAT
{

ANN::ANN()
{
	generator = make_unique < default_random_engine > (chrono::system_clock::now().time_since_epoch().count() + (uint)rand());
	innovationMsg = "";
	isNewSpecies = false;
}

// En caso de no definir ANNUserDefinitions, entonces se usa el constructor por defecto de ANNUSERDEF
ANN::ANN( shared_ptr <Neuron> seedNeuron, shared_ptr <SynapticWeight> seedSynapticWeihgt)
:ANN(ANNUserDefinitions(), move(seedNeuron), move(seedSynapticWeihgt))
{

}

ANN::ANN(const ANN& other) : ANN()
{ // no se copian los layers ni las conexiones porque son creados al momento del cruzamiento nuevamente
	copyUserDefParameters(other);

	for( const auto& pair : other.layersMap )
	{
		this->layersMap.emplace ( pair.first, pair.second->clone()  );
	}

	for ( uint i = 0; i < other.synapticWeights.size() ; ++i)
	{
		this->synapticWeights.push_back( other.synapticWeights.at(i)->clone() );
		uint layerInput,layerOutput,neuronInput, neuronOutput;
		tie(neuronInput,layerInput, neuronOutput, layerOutput) = this->synapticWeights.back()->getMark();
		// Se enlazan las neuronas correspondientes
		this->layersMap.at( layerOutput )->neurons.at(neuronOutput)->addIncomingSynapticWeight( this->synapticWeights.back() );
		this->layersMap.at(layerInput)->neurons.at(neuronInput)->addOutcomingSynapticWeight( this->synapticWeights.back() );
	}
}

ANN::ANN( const ANNUserDefinitions& userdef, shared_ptr <Neuron> seedNeuron, shared_ptr <SynapticWeight> seedSynapticWeihgt) : ANN()
{
	this->seedNeuron = move(seedNeuron);
	this->seedSynapticWeihgt = move(seedSynapticWeihgt);
	probabilityNewNeuronInLayer =  userdef.probabilityNewNeuronInLayer;
	probabilityOfNewSynapticWeight =  userdef.probabilityOfNewSynapticWeight;
	probabilityOfNewUniqueSynapticWeight =  userdef.probabilityOfNewUniqueSynapticWeight;
	initialConectionProbability = userdef.initialConectionProbability;
	inputsAmount =  userdef.inputsAmount;
	outputsAmount =  userdef.outputsAmount;
	useBackwardConnections =  userdef.useBackwardConnections;
	probabilityOfNewLayer = userdef.probabilityOfNewLayer; 
	// start the initial topology
	//Primero las capas y sus neuronas
	layersMap.emplace(LAYER_INITIAL, move(make_unique<Layer>( move(this->seedNeuron->clone()) ,LAYER_INITIAL)) );
	layersMap.emplace(LAYER_OUTPUT, move(make_unique<Layer>( move(this->seedNeuron->clone()),LAYER_OUTPUT)) );

	for (uint i = 0; i < inputsAmount; ++i){
		layersMap.at(LAYER_INITIAL)->addNewNeuron();
	}

	for (uint i = 0; i < outputsAmount; ++i){
		layersMap.at(LAYER_OUTPUT)->addNewNeuron();
	}
	// Segundo las conexiones sinapticas.
	for (unsigned int in = 0; in < inputsAmount; ++in){
		for (unsigned int out = 0; out < outputsAmount; ++out){
			if(rand()/(double)RAND_MAX < initialConectionProbability)
			{
				addSynapticWeight( LAYER_INITIAL, in, LAYER_OUTPUT, out );
			}
		}
	}
}

ANN::~ANN()
{
	layersMap.clear();
} 

shared_ptr < ANN > ANN::crossOver( const ANN&  mother ) const
{
	auto result = make_unique <ANN> ();
	result->copyUserDefParameters(*this); 
	result->constructLayersFromParents(*this, mother );
	result->constructSynapsesFromParents(*this, mother );
	result->mightMutate(); // Este paso es de vital importancia y produce la complexificacion de la red
	return move( result );
}

void ANN::printInfo() const{
	cout << "Innov  MSG" << endl;
	cerr << innovationMsg << endl;

	for( const auto& layer : layersMap ){
		layer.second->printInfo();
	}
	for( auto SW : synapticWeights){
		SW->printInfo();
	}
}

void ANN::setInputs( const vector<float>& inputs ) const{
	if( inputs.size() !=  inputsAmount )
	{
		cerr << "Error::ANN::setInputs:: input size is not correct." << endl;
		exit(EXIT_FAILURE);
	}
	auto initialLayer = layersMap.begin()->second.get(); // Always the first layer is the input layer
	for (uint i = 0; i < initialLayer->neurons.size(); ++i)	
	{
		initialLayer->neurons.at(i)->sumIncomingVoltage(inputs.at(i));
	}
}

vector <float> ANN::getOutputs() const {
	vector <float> result;
	auto finalLayer = layersMap.rbegin()->second.get();
	for (uint i = 0; i < finalLayer->neurons.size(); ++i)
	{
		result.push_back(finalLayer->neurons.at(i)->getOutput());
	}
	return move(result);
}

void ANN::spread(){
	for (auto & layer: layersMap)
	{
		layer.second->spread();
	}
}

void ANN::mightMutate()
{
	topologyMutations();
	nonTopologyMutations();
}

float ANN::getDistance( const ANN& other) const
{
	auto sum = 0.f;
	uint counter = 0;
	for ( auto & layer: layersMap)
	{
		sum += layer.second->getDistance( *other.layersMap.at( layer.first ) );
		counter += layer.second->neurons.size();
	}
	for (uint i = 0; i < synapticWeights.size(); ++i)
	{
		sum += synapticWeights.at(i)->getDistance( other.synapticWeights.at(i).get() );
		counter ++;
	}

	return move(sum / (float)counter); // Distance relative to the amount of neurons and synaptics weights
}

shared_ptr <ANN> ANN::clone() const
{
	return move( make_unique <ANN>  (*this) );
} 

shared_ptr <ANN> ANN::createSimilar() const
{
	auto result = this->clone();
	for (unsigned int i = 0; i < 5; ++i)
	{
		result->nonTopologyMutations();
	}
	return move(result);
}

bool ANN::getIsNewSpicie() const 
{
	return isNewSpecies;
}


void ANN::save(const string path) const
{
	uint sw_id = 0;
	ofstream file;
	file.open(path);
	if(file.is_open())
	{
		for(auto const& key_layer : layersMap )
		{
			key_layer.second->save(path +"L"+ to_string(key_layer.first) );
			//Saving in table
			file << "L " << to_string(key_layer.first) << endl;
		}
		for(auto const& sw : synapticWeights)
		{
			sw->save(path + "S" + to_string(sw_id) );
			// Saving in table.
			file << "S " << to_string(sw_id)  << endl;
			sw_id++;
		}
		file.close();
	}
	else
	{
		cerr << "ERROR::ANN::save::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}

void ANN::load(const string path, unique_ptr<Neuron> seedNeuron, unique_ptr <SynapticWeight> seedSynapticWeihgt )
{
	uint layerIn, layerOut, neuronOut, neuronIn;
	ifstream file;
	file.open(path, ios::in);
	string line;
	if (file.is_open())
	{
		while ( getline (file,line, char(' ') ) )
		{
			if (line.compare("L") == 0)
			{
				getline (file,line, char('\n') );
				uint layerId = stoul(line);
				auto layer = make_unique<Layer>(move(seedNeuron->clone()), layerId);
				layer->load(path+"L"+to_string(layerId));
				layersMap.emplace ( layerId, move(layer)  );
			}
			else if (line.compare("S") == 0)
			{
				getline (file,line, char('\n') );
				uint sw_id = stoul(line);
				auto SW = seedSynapticWeihgt->clone();
				SW->load(path+"S"+to_string(sw_id));
				tie(neuronIn,layerIn, neuronOut, layerOut) = SW->getMark();
				synapticWeights.push_back(move(SW));
				layersMap.at(layerOut)->neurons.at(neuronOut)->addIncomingSynapticWeight( synapticWeights.back() );
				layersMap.at(layerIn)->neurons.at(neuronIn)->addOutcomingSynapticWeight( synapticWeights.back() );
			}
		}
		inputsAmount = layersMap.at(0)->neurons.size();
		file.close();
	}
	else
	{
		cerr << "ERROR::ANN::load::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}


} // END NAMESPACE NEAT


