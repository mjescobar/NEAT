#ifndef NEAT_ANN_HPP
#define NEAT_ANN_HPP

#include <vector>
#include <memory>
#include <map>
#include <tuple>
#include <random>
#include <string>

#include "Neuron.hpp"
#include "SynapticWeight.hpp"
#include "ANNUserDefinitions.hpp"
#include "Layer.hpp"

namespace NEAT
{

class ANN
{
/**
	\class ANN  ANN.hpp
	\brief {Represent a Artificial Neural networks, composed by Layers of Neurons and Synaptic Weights}
*/

public:
	ANN();
	ANN(std::unique_ptr <Neuron> seedNeuron, std::unique_ptr <SynapticWeight> seedSynapticWeihgt);
	ANN(const ANN& other); // Usado para crossOver, para crear una copia usar metodo clone()
	ANN( const ANNUserDefinitions& userdef, std::unique_ptr <Neuron> seedNeuron, std::unique_ptr <SynapticWeight> seedSynapticWeihgt );
	virtual ~ANN();
	/**
		\brief {setInput is the only way to manually introduce input voltage to the inputs neurons, if in the user definitions file is set n number of inputs then this method acepts only a vector float of this size exactly.}
		\param[in] {inputs: Is a vector with equal size than the inputs neurons that represent a inncoming voltage of each input neuron.}
	*/
	void setInputs( std::vector <float> inputs ) const;
	/**
		\brief {This is the process to spread voltage from the first layer until the last layer (the output layer)}
	*/
	void spread();
	/**
		\brief { To obtain the last output voltage of the output neurons.  }
		\return { The last output voltage of the output neurons trought a float vector with the correct size. }
	*/
	std::vector <float> getOutputs() const;
	/**
		\brief { print relevant information }
	*/
	void printInfo() const;
	/**
		\brief { represent the process of reproduction between this (father) and other(mother), the son is a mixture of them }
		\param[in] { mother is a reference of a another ANN that represent the mother. }
		\return { the return is a new ANN pointer that represent the children. }
	*/
	std::unique_ptr < ANN > crossOver( const ANN& mother ) const;
	/**
		\brief {Obtain the distance in between two organism of the same topology. Is calculated neuron by neuron and conection by conection and depends on how is implemented in the neuron and in the conection}
		\param[in] {other is another ANN}
		\return {the return value is the distance between this and tue input ANN (other)}
	*/
	float getDistance( const ANN& other) const;
	/**
		\brief { Exist two diferents mutation that culd ocur, a topologie mutation that is add a neuron, layer or synapses or a non topolofie mutation that is change internal parameters values of Neurons or Synapses. The topologie mutation probabilities are part of the ANN user definitions, and the internal parameter mutation is in Neuron and SynapticWeight user definitions.

		This method is the motor of evolution, the way to explore diferent topologies and change internal values. This function provoke the innovations in the search of better organisms}
	*/
	void mightMutate();
	/**
		\brief { Create a ANN with same topology and internal parameters. }
		\return { Return the ANN with same topology and internal parameters. }
	*/
	std::unique_ptr <ANN> clone() const; 
	/**
		\brief { Create a ANN with same topology but with diferent Neural and Synaptic parameters }
		\return { A ANN with same topology but diferent Neural and Synaptic parameters  }
	*/
	std::unique_ptr <ANN> createSimilar() const; // Para poblar una nueva especie con otros con misma topología.
	/**
		\brief { ask if is a new spicie (because have diferent topology)  }
		\return { return true if is not from father specie, false in other case. } 
	*/
	bool getIsNewSpicie() const;
	
	std::string innovationMsg; // Is used to prevent produce two similar species with the same topologie. (this could ocur but from diferent species and is unlikely and is not dangerous but cpu cost because find two times the same topology and could be two diferent in the same time.)
	bool isNewSpecies; // Is set to true if have a topologie mutation
private:
	
	/**
		\private
		\brief { Create a new layer with one neuron inside and inncomming and outcomming synaptic weight from this neuron.   }
	*/
	// Por mejor vizualisacion los metodos privados estan implementados en ANNTools.cpp 
	void newNeuronInlayer();
	/**
		\private
		\brief { Add a new synaptic weight between two random neurons, but this synaptic weight could not be unique. }
	*/
	void newSynapticWeight(); // puede crear una misma conexion dos veces.
	/**
		\private
		\brief { Try to add a new Unique synaptic weight between two neurons. }
	*/
	void newUniqueSynapticWeight(); //  Corrobora que no exista esta previamente
	/**
		\private
		\brief { add a specific synaptic weight }
		\input { LayerIn and NeuronIn are the pair to specify the neuron that init the conection and LayerOut and NeuronOut specify the outcomming neuron. }
	*/
	void addSynapticWeight( const uint layerIn, 
		const uint neuronIn, 
		const uint layerOut, 
		const uint neuronOut );
	/**
		\private
		\brief {create a new layer just before the output layer.}
	*/
	void newLayer();
	/**
		\private
		\brief { Find a random layer place and neuron place }
		\return { tie( Layer, Neuron ) where Layer is a random layer place in layersMap map and Neuron is a random neuron place in neurons vector. }
	*/
	std::tuple < uint, uint > findRandNeuron() const;
	/**
		\private
		\brief { Find a random pair layer/neuron from a layer ahead than layerbound.}
		\input {layerBound is the layer that is the lower limit (not included) of the posible layer to return.}
		\return { tie( Layer, Neuron ) where Layer is a random layer place in layersMap map and Neuron is a random neuron place in neurons vector. }
	*/
	std::tuple < uint, uint > findRandNeuronInAheadLayer(const uint layerBound) const;
	/**
		\private
		\brief { Find a random pair layer/neuron from a layer behind than layerbound.}
		\input {layerBound is the layer that is the upper limit (not included) of the posible layer to return.}
		\return { tie( Layer, Neuron ) where Layer is a random layer place in layersMap map and Neuron is a random neuron place in neurons vector. }
	*/
	std::tuple < uint, uint > findRandNeuronBehindLayer(const uint layerBound) const;
	/**
		\private
		\brief { Create the layer and neuron topology with neurons fromm both parents. }
		\input { father and mother are the ANN that represent the father and the mother. }
	*/
	void constructLayersFromParents( const ANN& father, const ANN& mother );
	/**
		\private
		\brief { Create all the synaptic weight of the topology each one from the parents. }
		\inputs { father and mother are the ANN that represent the father and the mother. }
	*/
	void constructSynapsesFromParents( const ANN& father, const ANN& mother );
	/**
		\private
		\brief { Copy user def parameter from other ANN }
	*/
	void copyUserDefParameters( const ANN& other );
	/**
		\private
		\brief {  }
	*/
	void topologyMutations();
	/**
		\private
		\brief {  }
	*/
	void nonTopologyMutations();
	/**
		\private
		\brief { Find a random neuron in a specific layer. }
	*/
	uint findRandNeuronInLayer( uint layer )const; 

	std::unique_ptr < Neuron > seedNeuron;
	std::unique_ptr < SynapticWeight > seedSynapticWeihgt;
	std::map < unsigned int, std::unique_ptr < Layer > > layersMap; // Se hace un mapa para mantener ordenado los layers siempre al momento de agregarlos en el mapa.
	std::unique_ptr < std::default_random_engine > generator;
	std::vector < std::shared_ptr < SynapticWeight >  > synapticWeights;
	float probabilityNewNeuronInLayer;
	float probabilityOfNewSynapticWeight;
	float probabilityOfNewUniqueSynapticWeight;
	unsigned int inputsAmount;
	unsigned int outputsAmount;
	bool useBackwardConnections;
	float probabilityOfNewLayer;
};
}
#endif