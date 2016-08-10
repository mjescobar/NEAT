#ifndef NEAT_ANN_HPP
#define NEAT_ANN_HPP

#include <vector>
#include <memory>
#include <map>
#include <tuple>
#include <random>

#include "Neuron.hpp"
#include "SynapticWeight.hpp"
#include "ANNUserDefinitions.hpp"
#include "Layer.hpp"

namespace NEAT
{

class ANN
{
public:
	ANN( const ANNUserDefinitions& userdef, std::shared_ptr <Neuron> seedNeuron, std::shared_ptr <SynapticWeight> seedSynapticWeihgt );
	~ANN();
	ANN(const ANN& other); // Usado para crossOver
	void setInputs( std::vector <float> inputs ) const;
	void spread();
	std::vector <float> getOutputs() const;
	void printInfo() const;
	std::unique_ptr < ANN > crossOver( const ANN& other ) const;
	float getDistance( const ANN& other) const;
	void mightMutate();

private:
	void newNeuronInlayer();
	void newSynapticWeight(); // puede crear una misma conexion dos veces.
	void newUniqueSynapticWeight(); //  Corrobora que no exista esta previamente
	void addSynapticWeight( const uint layerIn, const uint neuronIn, const uint layerOut, const uint neuronOut );
	void newLayer();
	std::tuple < uint, uint > findRandNeuron()const;
	std::tuple < uint, uint > findRandNeuronInAheadLayer(const uint layerBound)const;
	std::tuple < uint, uint > findRandNeuronBehindLayer(const uint layerBound)const;

	std::shared_ptr < Neuron > seedNeuron;
	std::shared_ptr < SynapticWeight > seedSynapticWeihgt;

	std::map < unsigned int, std::unique_ptr < Layer > > layersMap; // Se hace un mapa para mantener ordenado los layers siempre al momento de agregarlos en el mapa.
	std::vector < std::shared_ptr < SynapticWeight >  > synapticWeights;

	float probabilityNewNeuronInLayer;
	float probabilityOfNewSynapticWeight;
	float probabilityOfNewUniqueSynapticWeight;
	unsigned int inputsAmount;
	unsigned int outputsAmount;
	bool useBackwardConnections;
	float probabilityOfNewLayer;
	std::unique_ptr < std::default_random_engine > generator;

};
}
#endif