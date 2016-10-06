#include "ANN.hpp"
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
ANN::ANN( unique_ptr <Neuron> seedNeuron, unique_ptr <SynapticWeight> seedSynapticWeihgt)
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

ANN::ANN( const ANNUserDefinitions& userdef, unique_ptr <Neuron> seedNeuron, unique_ptr <SynapticWeight> seedSynapticWeihgt) : ANN()
{
	this->seedNeuron = move(seedNeuron);
	this->seedSynapticWeihgt = move(seedSynapticWeihgt);
	probabilityNewNeuronInLayer =  userdef.probabilityNewNeuronInLayer;
	probabilityOfNewSynapticWeight =  userdef.probabilityOfNewSynapticWeight;
	probabilityOfNewUniqueSynapticWeight =  userdef.probabilityOfNewUniqueSynapticWeight;
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
			addSynapticWeight( LAYER_INITIAL, in, LAYER_OUTPUT, out );
		}
	}
}

ANN::~ANN()
{
	layersMap.clear();
} 

unique_ptr < ANN > ANN::crossOver( const ANN&  mother ) const
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

void ANN::setInputs( vector <float> inputs ) const{
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
	for ( auto & layer: layersMap)
	{
		sum += layer.second->getDistance( *other.layersMap.at( layer.first ) );
	}
	for (uint i = 0; i < synapticWeights.size(); ++i)
	{
		sum += synapticWeights.at(i)->getDistance( other.synapticWeights.at(i).get() );
	}
	return move(sum);
}

unique_ptr <ANN> ANN::clone() const
{
	return move( make_unique <ANN>  (*this) );
} 

unique_ptr <ANN> ANN::createSimilar() const
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

}