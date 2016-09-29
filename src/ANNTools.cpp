#include "ANN.hpp"
#include <climits>
#include <tuple>
#include <iostream>
#include <chrono>

using namespace std;
namespace NEAT
{
void ANN::newLayer()
{
	uint neuronIn, layerIn, neuronOut, layerOut;
	uint layerId = layersMap.size() - 1; 
  	innovationMsg += "NL;";
	isNewSpecies = true;
	layersMap.emplace( layerId, make_unique< Layer >( move(seedNeuron->clone()), layerId ) );
	tie(layerIn, neuronIn) = findRandNeuronBehindLayer( layerId );
	tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( layerId );
	layersMap.at( layerId )->addNewNeuron();
	addSynapticWeight( layerIn, neuronIn, layerId, 0 );
  	addSynapticWeight( layerId, 0, layerOut, neuronOut );  
}

//Importante recordar que de este metodo y de los otros de mutacion depende las conexiones hacia atras
void ANN::newNeuronInlayer()
{
	if( layersMap.size() - 2 == 0 ){return ;}// En caso que no existan layers ocultos entonces no es posible agregar neuronas en ningun layer.
	uint randomLayerIdNewNeuron, neuronIn, layerIn, neuronOut, layerOut;
	
  	uniform_int_distribution<uint> distributionLayer(1, layersMap.size()-2 ); // valores entre 1 y size-2 dado que no pueden agregarse en la capa inicial ni en la final y por suerte el resto esta ordenada a partir del valor 1.
  	randomLayerIdNewNeuron = distributionLayer( *generator );

  	if( !useBackwardConnections ){
		tie(layerIn,neuronIn) = findRandNeuronBehindLayer( randomLayerIdNewNeuron );
		tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( randomLayerIdNewNeuron );
  	}else{
  		tie(layerIn,neuronIn) = findRandNeuron( );
		tie(layerOut,neuronOut) = findRandNeuron( );
  	}
  	// Una vez conseguidos todos los valores necesarios se procede a agregar la neurona y las conexiones, esto se hace posteriormente para que no se escoja este misma neurona como cantidata a neuron in o neuron out.
  	layersMap.at( randomLayerIdNewNeuron )->addNewNeuron();
  	// Se deben crear dos conexiones que comuniquen esta neurona con la red neuronal, una conexion que llegue a esta neurona y una que salga de ella. Aqui es importante saber si se permiten las conexiones en ambas direcciones.
  	innovationMsg += "nN(" +  to_string(randomLayerIdNewNeuron) + "  );";
	isNewSpecies = true;
  	addSynapticWeight( layerIn, neuronIn, randomLayerIdNewNeuron, layersMap.at( randomLayerIdNewNeuron )->neurons.size()-1 );
  	addSynapticWeight( randomLayerIdNewNeuron, layersMap.at( randomLayerIdNewNeuron )->neurons.size() -1, layerOut, neuronOut );  	
}

void ANN::newSynapticWeight(){
	uint neuronIn, layerIn, neuronOut, layerOut;
	if( !useBackwardConnections ){
		tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( 0 );
		tie(layerIn,neuronIn) = findRandNeuronBehindLayer( layerOut );
  	}else{
  		tie(layerIn,neuronIn) = findRandNeuron( );
		tie(layerOut,neuronOut) = findRandNeuron( );
  	}
	addSynapticWeight( layerIn, neuronIn, layerOut, neuronOut ); 
} 

void ANN::newUniqueSynapticWeight(){
	uint neuronIn, layerIn, neuronOut, layerOut;
	uint attempts = 20;
	bool unique = false;
	std::cerr << "anusw 1" << std::endl;
	for (uint i = 0; i < attempts; ++i)
	{
		if( useBackwardConnections ){
	std::cerr << "anusw 2" << std::endl;
			tie(layerIn,neuronIn) = findRandNeuron( );
			tie(layerOut,neuronOut) = findRandNeuron( );
	std::cerr << "anusw 3!!" << std::endl;
	  	}else{
	std::cerr << "anusw 4" << std::endl;
	  		tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( 0 );
			tie(layerIn,neuronIn) = findRandNeuronBehindLayer( layerOut );
	std::cerr << "anusw 5" << std::endl;
	  	}

	  	auto SWList =  layersMap.at(layerIn)->neurons.at(neuronIn)->getOutcomingSynapticWeights();
	  	unique = true;
	std::cerr << "anusw 6" << std::endl;
	  	for( const auto& SW : SWList ) // Se buscara si ya existe alguna conexion que conecte las mismas neuronas en la misma orientacion, en tal caso habra que intentarlo denuevo
	  	{
	  		uint swLayerIn, swNeuronIn, swLayerOut, swNeuronOut;
	std::cerr << "anusw 7" << std::endl;
	  		tie(swNeuronIn, swLayerIn, swNeuronOut, swLayerOut) = SW->getMark();
	std::cerr << "anusw 8" << std::endl;
	  		if( neuronOut == swNeuronOut && layerOut == swLayerOut ) // Como se estan revizando las conexiones de salida de la neurona solo basta con saber si ya existen conexiones que salgan al mismo destino
	  		{ 
	  			unique = false;
	  			break;
	  		}
	std::cerr << "anusw 9" << std::endl;
	  	}
	  	if(unique == true)
	  	{
	std::cerr << "anusw 10 Li: " << layerIn << "\tni: " << neuronIn << "\tlo: " << layerOut << "\tno: " << neuronOut  << std::endl;
	  		addSynapticWeight( layerIn, neuronIn, layerOut, neuronOut ); 
	  		break;
	std::cerr << "anusw 11" << std::endl;
	  	}
	std::cerr << "anusw 12" << std::endl;
	}	
}

void ANN::addSynapticWeight( const uint layerIn, const uint neuronIn, const uint layerOut, const uint neuronOut ){

	std::cerr << "asw 1" << std::endl;
	synapticWeights.push_back( seedSynapticWeihgt->createNew() );
	std::cerr << "asw 2" << std::endl;
	//Suena como al reves, pero la neurona in es la neurona de la cual sale la conexion y la neurona out es a la que llega la conexion. ha provocado problemas anteriores no entender este detalle. se debe revizar la notacion para otra que provoque menos confusion
	std::cerr << "asw 3" << std::endl;
	std::cerr << "layersMap.sz: " << layersMap.size()  << std::endl;

	for(  auto& layer : layersMap )
	{
		layer.second->printInfo();
	}

	std::cerr << "layerOut: " << layerOut  << std::endl;
	layersMap.at(layerOut)->printInfo();
	std::cerr << "neurons.sz: " << layersMap.at(layerOut)->neurons.size()  << std::endl;

	layersMap.at(layerOut)->neurons.at(neuronOut)->addIncomingSynapticWeight( synapticWeights.back() );
	std::cerr << "asw 4" << std::endl;
	layersMap.at(layerIn)->neurons.at(neuronIn)->addOutcomingSynapticWeight( synapticWeights.back() );
	std::cerr << "asw 5" << std::endl;
	synapticWeights.back()->setMark( neuronIn, layerIn, neuronOut, layerOut ); // HistorialMark
	std::cerr << "asw 6" << std::endl;
	innovationMsg += "SW(" + to_string(layerIn) + " ," + to_string(neuronIn )+ "," + to_string(layerOut) + "," + to_string(neuronOut) +  ");"; 
	std::cerr << "asw 7" << std::endl;
	isNewSpecies = true;
}

tuple < uint, uint > ANN::findRandNeuron() const  
{
	uniform_int_distribution<uint> layerDist(0, layersMap.size() - 1 );
	auto layer = layerDist( *generator );
	auto neuron = findRandNeuronInLayer(layer);
	return make_tuple ( move(layer), move(neuron) );
}

tuple < uint, uint > ANN::findRandNeuronInAheadLayer(const uint layerBound) const {
	if( layerBound == layersMap.size() || layerBound == layersMap.rbegin()->first ){
		cerr << "ERROR::ANN::findRandNeuronInAheadLayer::layerBound is the last layer." << endl;
		exit(EXIT_FAILURE);
	}
	uniform_int_distribution<uint> layerDist(layerBound + 1, layersMap.size() - 1 );
	auto layer = layerDist( *generator );
	auto neuron = findRandNeuronInLayer(layer);
	return make_tuple ( move(layer), move(neuron) );
}

tuple < uint, uint > ANN::findRandNeuronBehindLayer(const uint layerBound) const {
	if( layerBound == 0 ){
		cerr << "ERROR::ANN::findRandNeuronBehindLayer::layerBound is the initial layer." << endl;
		exit(EXIT_FAILURE);
	}
	uint layerBoundEffective = layerBound;
	if ( layerBound == layersMap.rbegin()->first ) // En caso que el layer sea UINT_MAX entonces simplemente cambiarlo por el valor del largo -1 para que tenga sentido el algoritmo.
	{
		layerBoundEffective = layersMap.size()-1;
	}
	uniform_int_distribution<uint> layerDist(0, layerBoundEffective -1 );
	auto layer = layerDist( *generator );
	auto neuron = findRandNeuronInLayer(layer);
	return make_tuple ( move(layer), move(neuron) );
}

void ANN::constructLayersFromParents(  const ANN& father, const ANN& mother ) 
{
	for( const auto& pair : father.layersMap )
	{
		this->layersMap.emplace ( pair.first, pair.second->crossOver(*mother.layersMap.at(pair.first) )  );
	}

}

void ANN::constructSynapsesFromParents( const ANN& father, const ANN& mother ) 
{	
	uint layerInput,layerOutput,neuronInput, neuronOutput;
	for ( uint i = 0; i < father.synapticWeights.size() ; ++i)
	{
		this->synapticWeights.push_back( move(father.synapticWeights.at(i)->crossOver( *mother.synapticWeights.at(i)) ) );
		tie(neuronInput,layerInput, neuronOutput, layerOutput) = this->synapticWeights.back()->getMark();
		// Se enlazan las neuronas correspondientes
		this->layersMap.at( layerOutput )->neurons.at(neuronOutput)->addIncomingSynapticWeight( this->synapticWeights.back() );
		this->layersMap.at(layerInput)->neurons.at(neuronInput)->addOutcomingSynapticWeight( this->synapticWeights.back() );
	}
}


void ANN::topologyMutations()
{
	if( rand()/(double)RAND_MAX  < probabilityOfNewLayer )
	{
		std::cerr << "atm 1" << std::endl;
		newLayer();
		std::cerr << "atm 2" << std::endl;
	}
	if( rand()/(double)RAND_MAX  < probabilityNewNeuronInLayer )
	{
		std::cerr << "atm 3" << std::endl;
		newNeuronInlayer();
		std::cerr << "atm 4" << std::endl;
	}
	if( rand()/(double)RAND_MAX  < probabilityOfNewSynapticWeight )
	{
		std::cerr << "atm 5" << std::endl;
		newSynapticWeight();
		std::cerr << "atm 6" << std::endl;
	}
	if( rand()/(double)RAND_MAX  < probabilityOfNewUniqueSynapticWeight )
	{
		std::cerr << "atm 7" << std::endl;
		newUniqueSynapticWeight();
		std::cerr << "atm 8" << std::endl;
	}
}

void ANN::nonTopologyMutations()
{
	for ( auto & layer: layersMap)
	{
		layer.second->mightMutate();
	}
	for ( auto & SW : synapticWeights){
		SW->mightMutate();
	}
}

void ANN::copyUserDefParameters( const ANN& other )
{
	this->seedNeuron = move(other.seedNeuron->clone());
	this->seedSynapticWeihgt = move(other.seedSynapticWeihgt->clone());
	this->probabilityNewNeuronInLayer = other.probabilityNewNeuronInLayer;
	this->probabilityOfNewSynapticWeight = other.probabilityOfNewSynapticWeight;
	this->probabilityOfNewUniqueSynapticWeight = other.probabilityOfNewUniqueSynapticWeight;
	this->inputsAmount = other.inputsAmount;
	this->outputsAmount = other.outputsAmount;
	this->useBackwardConnections = other.useBackwardConnections;
	this->probabilityOfNewLayer = other.probabilityOfNewLayer;
}

uint ANN::findRandNeuronInLayer( uint layer ) const
{
	if ( layer == layersMap.size()-1 )
	{		
		layer = layersMap.rbegin()->first;
	}  
	uniform_int_distribution<uint> neuronDist(0, layersMap.at(layer)->neurons.size() - 1 );
	return neuronDist( *generator );
}


}