#include "ANN.hpp"
#include <climits>
#include <tuple>
#include <iostream>
#include <chrono>

namespace NEAT
{
void ANN::newLayer()
{
	uint neuronIn, layerIn, neuronOut, layerOut;
	uint layerId = layersMap.size() - 1; 
	layersMap.emplace( layerId, std::make_unique< Layer >( seedNeuron, layerId ) );
	std::tie(layerIn, neuronIn) = findRandNeuronBehindLayer( layerId );
	std::tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( layerId );
	layersMap.at( layerId )->addNewNeuron();
  	innovationMsg += "NL;";
	addSynapticWeight( layerIn, neuronIn, layerId, 0 );
  	addSynapticWeight( layerId, 0, layerOut, neuronOut );  
}

//Importante recordar que de este metodo y de los otros de mutacion depende las conexiones hacia atras
void ANN::newNeuronInlayer()
{
	if( layersMap.size() - 2 == 0 ){return ;}// En caso que no existan layers ocultos entonces no es posible agregar neuronas en ningun layer.
	uint randomLayerIdNewNeuron, neuronIn, layerIn, neuronOut, layerOut;
	
  	std::uniform_int_distribution<uint> distributionLayer(1, layersMap.size()-2 ); // valores entre 1 y size-2 dado que no pueden agregarse en la capa inicial ni en la final y por suerte el resto esta ordenada a partir del valor 1.
  	randomLayerIdNewNeuron = distributionLayer( *generator );

  	if( !useBackwardConnections ){
		std::tie(layerIn,neuronIn) = findRandNeuronBehindLayer( randomLayerIdNewNeuron );
		std::tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( randomLayerIdNewNeuron );
  	}else{
  		std::tie(layerIn,neuronIn) = findRandNeuron( );
		std::tie(layerOut,neuronOut) = findRandNeuron( );
  	}
  	// Una vez conseguidos todos los valores necesarios se procede a agregar la neurona y las conexiones, esto se hace posteriormente para que no se escoja este misma neurona como cantidata a neuron in o neuron out.
  	layersMap.at( randomLayerIdNewNeuron )->addNewNeuron();
  	// Se deben crear dos conexiones que comuniquen esta neurona con la red neuronal, una conexion que llegue a esta neurona y una que salga de ella. Aqui es importante saber si se permiten las conexiones en ambas direcciones.
  	innovationMsg += "nN(" +  std::to_string(randomLayerIdNewNeuron) + "  );";
  	addSynapticWeight( layerIn, neuronIn, randomLayerIdNewNeuron, layersMap.at( randomLayerIdNewNeuron )->neurons.size()-1 );
  	addSynapticWeight( randomLayerIdNewNeuron, layersMap.at( randomLayerIdNewNeuron )->neurons.size() -1, layerOut, neuronOut );  	
}

void ANN::newSynapticWeight(){
	uint neuronIn, layerIn, neuronOut, layerOut;
	if( !useBackwardConnections ){
		std::tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( 0 );
		std::tie(layerIn,neuronIn) = findRandNeuronBehindLayer( layerOut );
  	}else{
  		std::tie(layerIn,neuronIn) = findRandNeuron( );
		std::tie(layerOut,neuronOut) = findRandNeuron( );
  	}
	addSynapticWeight( layerIn, neuronIn, layerOut, neuronOut ); 
} 

void ANN::newUniqueSynapticWeight(){
	uint attempts = 20;
	uint neuronIn, layerIn, neuronOut, layerOut;
	bool found = false;
	for (uint i = 0; i < attempts; ++i)
	{
		if( !useBackwardConnections ){
			std::tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( 0 );
			std::tie(layerIn,neuronIn) = findRandNeuronBehindLayer( layerOut );
	  	}else{
	  		std::tie(layerIn,neuronIn) = findRandNeuron( );
			std::tie(layerOut,neuronOut) = findRandNeuron( );
	  	}

	  	auto SWList =  layersMap.at(layerIn)->neurons.at(neuronIn)->getOutcomingSynapticWeights();
	  	found = true;
	  	for( const auto& SW : SWList ) // Se buscara si ya existe alguna conexion que conecte las mismas neuronas en la misma orientacion, en tal caso habra que intentarlo denuevo
	  	{
	  		auto mark = SW->getMark();
	  		if( neuronOut == std::get<2>(mark) && layerOut == std::get<3>(mark) )
	  		{ 
	  			found = false;
	  			break;
	  		}
	  	}
	  	if(found == true)
	  	{
	  		addSynapticWeight( layerIn, neuronIn, layerOut, neuronOut ); 
	  		break;
	  	}
	}	
}

void ANN::addSynapticWeight( const uint layerIn, const uint neuronIn, const uint layerOut, const uint neuronOut ){
	synapticWeights.push_back( seedSynapticWeihgt->createNew() );
	//Suena como al reves, pero la neurona in es la neurona de la cual sale la conexion y la neurona out es a la que llega la conexion. ha provocado problemas anteriores no entender este detalle. se debe revizar la notacion para otra que provoque menos confusion
	layersMap.at(layerOut)->neurons.at(neuronOut)->addIncomingSynapticWeight( synapticWeights.back() );
	layersMap.at(layerIn)->neurons.at(neuronIn)->addOutcomingSynapticWeight( synapticWeights.back() );
	synapticWeights.back()->setMark( neuronIn, layerIn, neuronOut, layerOut ); // HistorialMark
	innovationMsg += "SW(" + std::to_string(layerIn) + " ," + std::to_string(neuronIn )+ "," + std::to_string(layerOut) + "," + std::to_string(neuronOut) +  ");"; 
}

std::tuple < uint, uint > ANN::findRandNeuron() const  {
	std::uniform_int_distribution<uint> layerDist(0, layersMap.size() - 1 );
	auto layer = layerDist( *generator );
	if ( layer == layersMap.size()-1 ){		
		layer = layersMap.rbegin()->first;
	}  
	std::uniform_int_distribution<uint> neuronDist(0, layersMap.at(layer)->neurons.size() - 1 );
	auto neuron = neuronDist( *generator );
	return std::make_tuple ( std::move(layer), std::move(neuron) );
}

std::tuple < uint, uint > ANN::findRandNeuronInAheadLayer(const uint layerBound) const {
	if( layerBound == layersMap.size() || layerBound == layersMap.rbegin()->first ){
		std::cerr << "ERROR::ANN::findRandNeuronInAheadLayer::layerBound is the last layer." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::uniform_int_distribution<uint> layerDist(layerBound + 1, layersMap.size() - 1 );
	auto layer = layerDist( *generator );
	if ( layer == layersMap.size()-1 ){		
		layer = layersMap.rbegin()->first;
	}  
	std::uniform_int_distribution<uint> neuronDist(0, layersMap.at(layer)->neurons.size() - 1 );
	auto neuron = neuronDist( *generator );
	return std::make_tuple ( std::move(layer), std::move(neuron) );
}

std::tuple < uint, uint > ANN::findRandNeuronBehindLayer(const uint layerBound) const {
	if( layerBound == 0 ){
		std::cerr << "ERROR::ANN::findRandNeuronBehindLayer::layerBound is the initial layer." << std::endl;
		exit(EXIT_FAILURE);
	}
	uint layerBoundEffective = layerBound;
	if ( layerBound == layersMap.rbegin()->first ) // En caso que el layer sea UINT_MAX entonces simplemente cambiarlo por el valor del largo -1 para que tenga sentido el algoritmo.
	{
		layerBoundEffective = layersMap.size()-1;
	}

	std::uniform_int_distribution<uint> layerDist(0, layerBoundEffective -1 );
	auto layer = layerDist( *generator );
	if ( layer == layersMap.size()-1 ){		
		layer = layersMap.rbegin()->first;
	}  
	std::uniform_int_distribution<uint> neuronDist(0, layersMap.at(layer)->neurons.size() - 1 );
	auto neuron = neuronDist( *generator );
	return std::make_tuple ( std::move(layer), std::move(neuron) );
}


}