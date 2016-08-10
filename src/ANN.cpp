#include "ANN.hpp"
#include <climits>
#include <tuple>
#include <iostream>
#include <chrono>
#define LAYER_INITIAL 0
#define LAYER_OUTPUT UINT_MAX



namespace NEAT
{

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

void ANN::newLayer()
{
	uint neuronIn, layerIn, neuronOut, layerOut;
	uint layerId = layersMap.size() - 1; 
	layersMap.emplace( layerId, std::make_unique< Layer >( seedNeuron, layerId ) );
	std::tie(layerIn, neuronIn) = findRandNeuronBehindLayer( layerId );
	std::tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( layerId );
	layersMap.at( layerId )->addNewNeuron();
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
  	std::cout << "NEW NEURON LAYER: " << randomLayerIdNewNeuron << std::endl;
  	layersMap.at( randomLayerIdNewNeuron )->addNewNeuron();
  	// Se deben crear dos conexiones que comuniquen esta neurona con la red neuronal, una conexion que llegue a esta neurona y una que salga de ella. Aqui es importante saber si se permiten las conexiones en ambas direcciones.
  	addSynapticWeight( layerIn, neuronIn, randomLayerIdNewNeuron, layersMap.at( randomLayerIdNewNeuron )->neurons.size()-1 );
  	addSynapticWeight( randomLayerIdNewNeuron, layersMap.at( randomLayerIdNewNeuron )->neurons.size() -1, layerOut, neuronOut );  	
}

void ANN::newSynapticWeight(){
	uint neuronIn, layerIn, neuronOut, layerOut;
	if( !useBackwardConnections ){
		std::tie(layerOut,neuronOut) = findRandNeuronInAheadLayer( 0 );
		std::tie(layerIn,neuronIn) = findRandNeuronBehindLayer( layerOut );
  	}else{
  		std::cerr << "YESS" << std::endl;
  		std::tie(layerIn,neuronIn) = findRandNeuron( );
		std::tie(layerOut,neuronOut) = findRandNeuron( );
  	}
  	std::cerr << "LO: " << layerOut  <<  "\tNO: " << neuronOut  << "\tLI: "<< layerIn <<"\tNI" << neuronIn << std::endl;
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

float ANN::getDistance( const ANN& other) const
{
	auto sum = 0.f;
	auto tmp = 0.f;
	//fisrt neurons
	for ( auto & layer: layersMap){
		tmp = layer.second->getDistance( *other.layersMap.at( layer.first ) );
		std::cout << "Layer " << layer.first << "\t: distance: " << tmp << std::endl;
		sum += tmp;
	}

	//Second SynapticWeights
	for (uint i = 0; i < synapticWeights.size(); ++i)
	{
		tmp = synapticWeights.at(i)->getDistance( other.synapticWeights.at(i).get() );
		std::cout << "synapticWeights " << i << "\t: distance: " << tmp << std::endl;
		sum += tmp;
	}

	return std::move(sum);
}


}