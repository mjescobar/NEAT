#include "ANN.hpp"
#include <climits>
#include <tuple>
#include <iostream>
#include <chrono>
#define LAYER_INITIAL 0
#define LAYER_OUTPUT UINT_MAX



namespace NEAT
{

// En caso de no definir ANNUserDefinitions, entonces se usa el constructor por defecto de ANNUSERDEF
ANN::ANN( std::shared_ptr <Neuron> seedNeuron, std::shared_ptr <SynapticWeight> seedSynapticWeihgt)
:ANN(ANNUserDefinitions(), seedNeuron, seedSynapticWeihgt){}


ANN::ANN(const ANN& other){ // no se copian los layers ni las conexiones porque son creados al momento del cruzamiento nuevamente
	this->seedNeuron = other.seedNeuron;
	this->seedSynapticWeihgt = other.seedSynapticWeihgt;
	this->probabilityNewNeuronInLayer = other.probabilityNewNeuronInLayer;
	this->probabilityOfNewSynapticWeight = other.probabilityOfNewSynapticWeight;
	this->probabilityOfNewUniqueSynapticWeight = other.probabilityOfNewUniqueSynapticWeight;
	this->inputsAmount = other.inputsAmount;
	this->outputsAmount = other.outputsAmount;
	this->useBackwardConnections = other.useBackwardConnections;
	this->probabilityOfNewLayer = other.probabilityOfNewLayer;
	generator = std::make_unique < std::default_random_engine > (std::chrono::system_clock::now().time_since_epoch().count() + (uint)rand());
	innovationMsg = "";
	isNewSpecies = false;
}

ANN::ANN( const ANNUserDefinitions& userdef, std::shared_ptr <Neuron> seedNeuron, std::shared_ptr <SynapticWeight> seedSynapticWeihgt){
	this->seedNeuron = seedNeuron;
	this->seedSynapticWeihgt = seedSynapticWeihgt;
	probabilityNewNeuronInLayer =  userdef.probabilityNewNeuronInLayer;
	probabilityOfNewSynapticWeight =  userdef.probabilityOfNewSynapticWeight;
	probabilityOfNewUniqueSynapticWeight =  userdef.probabilityOfNewUniqueSynapticWeight;
	inputsAmount =  userdef.inputsAmount;
	outputsAmount =  userdef.outputsAmount;
	useBackwardConnections =  userdef.useBackwardConnections;
	probabilityOfNewLayer = userdef.probabilityOfNewLayer; 
	// start the initial topology
	//Primero las capas y sus neuronas
	layersMap.emplace(LAYER_INITIAL, std::move(std::make_unique<Layer>( seedNeuron ,LAYER_INITIAL)) );
	layersMap.emplace(LAYER_OUTPUT, std::move(std::make_unique<Layer>( seedNeuron,LAYER_OUTPUT)) );

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
	generator = std::make_unique < std::default_random_engine > (std::chrono::system_clock::now().time_since_epoch().count() + (uint)rand());
	isNewSpecies = false;
	innovationMsg = "";
}

ANN::~ANN()
{
	layersMap.clear();
} 

std::unique_ptr < ANN > ANN::crossOver( const ANN&  other ) const
{
	auto result = std::make_unique <ANN> ( *this ); 
	// Primero se crean las neuronas y luego las conexiones synapticas
	for( const auto& pair : layersMap ){
		result->layersMap.emplace ( pair.first, pair.second->crossOver(*other.layersMap.at(pair.first) )  );
	}

	for ( uint i = 0; i < this->synapticWeights.size() ; ++i){
		result->synapticWeights.push_back( this->synapticWeights.at(i)->crossOver( *other.synapticWeights.at(i) ) );
		auto historicalMark = result->synapticWeights.back()->getMark();
		// Se enlazan las neuronas correspondientes
		result->layersMap.at( std::get<1>(historicalMark) )->neurons.at(std::get<0>(historicalMark))->addIncomingSynapticWeight( result->synapticWeights.back() );
		result->layersMap.at(std::get<3>(historicalMark))->neurons.at(std::get<2>(historicalMark))->addIncomingSynapticWeight( result->synapticWeights.back() );
	}
	return std::move( result );
}

void ANN::printInfo() const{
	for( const auto& layer : layersMap ){
		layer.second->printInfo();
	}
	for( auto SW : synapticWeights){
		SW->printInfo();
	}
}

void ANN::setInputs( std::vector <float> inputs ) const{
	auto initialLayer = layersMap.begin()->second.get();
	for (uint i = 0; i < initialLayer->neurons.size(); ++i)	{
		initialLayer->neurons.at(i)->sumIncomingVoltage(inputs.at(i));
	}
}

std::vector <float> ANN::getOutputs() const {
	std::vector <float> result;
	auto finalLayer = layersMap.rbegin()->second.get();
	for (uint i = 0; i < finalLayer->neurons.size(); ++i)
	{
		result.push_back(finalLayer->neurons.at(i)->getOutput());
	}
	return std::move(result);
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
		isNewSpecies = true;
	}
	if( rand()/(double)RAND_MAX  < probabilityNewNeuronInLayer){
		newNeuronInlayer();
		isNewSpecies = true;
	}
	if( rand()/(double)RAND_MAX  < probabilityOfNewSynapticWeight){
		newSynapticWeight();
		isNewSpecies = true;
	}
	if( rand()/(double)RAND_MAX  < probabilityOfNewUniqueSynapticWeight){
		newUniqueSynapticWeight();
		isNewSpecies = true;
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
	return std::move(sum);
}


std::unique_ptr <ANN> ANN::clone() const
{
	auto result = std::make_unique <ANN>  (*this); //  copy constructor

	for( const auto& pair : layersMap ){
		result->layersMap.emplace ( pair.first, pair.second->clone()  );
	}

	for ( uint i = 0; i < this->synapticWeights.size() ; ++i){
		result->synapticWeights.push_back( this->synapticWeights.at(i)->clone() );
		auto historicalMark = result->synapticWeights.back()->getMark();
		// Se enlazan las neuronas correspondientes
		result->layersMap.at( std::get<1>(historicalMark) )->neurons.at(std::get<0>(historicalMark))->addIncomingSynapticWeight( result->synapticWeights.back() );
		result->layersMap.at(std::get<3>(historicalMark))->neurons.at(std::get<2>(historicalMark))->addIncomingSynapticWeight( result->synapticWeights.back() );
	}
	return std::move( result );
} 

std::unique_ptr <ANN> ANN::createSimilar() const
{
	auto result = this->clone();
	for ( auto & layer: result->layersMap){
		layer.second->mightMutate();
	}
	for ( auto & SW : result->synapticWeights){
		SW->mightMutate();
	}
	return std::move(result);
}

bool ANN::getIsNewSpicie() const 
{
	return isNewSpecies;
}

}