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

// En caso de no definir ANNUserDefinitions, entonces se usa el constructor por defecto de ANNUSERDEF
ANN::ANN( unique_ptr <Neuron> seedNeuron, unique_ptr <SynapticWeight> seedSynapticWeihgt)
:ANN(ANNUserDefinitions(), move(seedNeuron), move(seedSynapticWeihgt))
{

}


ANN::ANN(const ANN& other){ // no se copian los layers ni las conexiones porque son creados al momento del cruzamiento nuevamente
	this->seedNeuron = move(other.seedNeuron->clone());
	this->seedSynapticWeihgt = move(other.seedSynapticWeihgt->clone());
	this->probabilityNewNeuronInLayer = other.probabilityNewNeuronInLayer;
	this->probabilityOfNewSynapticWeight = other.probabilityOfNewSynapticWeight;
	this->probabilityOfNewUniqueSynapticWeight = other.probabilityOfNewUniqueSynapticWeight;
	this->inputsAmount = other.inputsAmount;
	this->outputsAmount = other.outputsAmount;
	this->useBackwardConnections = other.useBackwardConnections;
	this->probabilityOfNewLayer = other.probabilityOfNewLayer;
	generator = make_unique < default_random_engine > (chrono::system_clock::now().time_since_epoch().count() + (uint)rand());
	innovationMsg = "";
	isNewSpecies = false;
}

ANN::ANN( const ANNUserDefinitions& userdef, unique_ptr <Neuron> seedNeuron, unique_ptr <SynapticWeight> seedSynapticWeihgt){
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
	
	generator = make_unique < default_random_engine > (chrono::system_clock::now().time_since_epoch().count() + (uint)rand());
	isNewSpecies = false;
	innovationMsg = "";
}

ANN::~ANN()
{
	layersMap.clear();
} 

unique_ptr < ANN > ANN::crossOver( const ANN&  other ) const
{
	auto result = make_unique <ANN> ( *this ); 
	// Primero se crean las neuronas y luego las conexiones synapticas
	for( const auto& pair : layersMap ){
		result->layersMap.emplace ( pair.first, pair.second->crossOver(*other.layersMap.at(pair.first) )  );
	}

	for ( uint i = 0; i < this->synapticWeights.size() ; ++i){
		result->synapticWeights.push_back( move(this->synapticWeights.at(i)->crossOver( *other.synapticWeights.at(i)) ) );
		uint layerInput,layerOutput,neuronInput, neuronOutput;
		
		tie(neuronInput,layerInput, neuronOutput, layerOutput) = result->synapticWeights.back()->getMark();
		// Se enlazan las neuronas correspondientes
		result->layersMap.at( layerOutput )->neurons.at(neuronOutput)->addIncomingSynapticWeight( result->synapticWeights.back() );
		result->layersMap.at(layerInput)->neurons.at(neuronInput)->addOutcomingSynapticWeight( result->synapticWeights.back() );
	}
	// ========================MIGHT MUTATE=======================
	result->mightMutate(); // Este paso es de vital importancia y produce la complexificacion de la red
	//============================================================
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
	auto initialLayer = layersMap.begin()->second.get();
	for (uint i = 0; i < initialLayer->neurons.size(); ++i)	{
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
	for ( auto & layer: layersMap){
		layer.second->spread();
	}
}

void ANN::mightMutate(){
	//primero las mutaciones de las neuronas y conexiones synapticas que son las mas basicas
	for ( auto & layer: layersMap){
		layer.second->mightMutate();
	}
	for ( auto & SW : synapticWeights){
		SW->mightMutate();
	}
	//Segundo las mutaciones de topologia
	if( rand()/(double)RAND_MAX  < probabilityOfNewLayer){
		newLayer();
	}
	if( rand()/(double)RAND_MAX  < probabilityNewNeuronInLayer){
		newNeuronInlayer();
	}
	if( rand()/(double)RAND_MAX  < probabilityOfNewSynapticWeight){
		newSynapticWeight();
	}
	if( rand()/(double)RAND_MAX  < probabilityOfNewUniqueSynapticWeight){
		newUniqueSynapticWeight();
	}
}

float ANN::getDistance( const ANN& other) const
{
	auto sum = 0.f;
	for ( auto & layer: layersMap){
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
	auto result = make_unique <ANN>  (*this); //  copy constructor

	for( const auto& pair : layersMap ){
		result->layersMap.emplace ( pair.first, pair.second->clone()  );
	}

	for ( uint i = 0; i < this->synapticWeights.size() ; ++i){
		result->synapticWeights.push_back( this->synapticWeights.at(i)->clone() );
		uint layerInput,layerOutput,neuronInput, neuronOutput;
		tie(neuronInput,layerInput, neuronOutput, layerOutput) = result->synapticWeights.back()->getMark();
		// Se enlazan las neuronas correspondientes
		result->layersMap.at( layerOutput )->neurons.at(neuronOutput)->addIncomingSynapticWeight( result->synapticWeights.back() );
		result->layersMap.at(layerInput)->neurons.at(neuronInput)->addOutcomingSynapticWeight( result->synapticWeights.back() );
	}
	return move( result );
} 

unique_ptr <ANN> ANN::createSimilar() const
{
	auto result = this->clone();
	for ( auto & layer: result->layersMap){
		layer.second->mightMutate();
	}
	for ( auto & SW : result->synapticWeights){
		SW->mightMutate();
	}
	return move(result);
}

bool ANN::getIsNewSpicie() const 
{
	return isNewSpecies;
}

}