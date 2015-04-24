#include "GlobalInformation.hpp"

using namespace NEATSpikes;

int GlobalInformation::getInnovation( int historicalMarkNeuronInput, int historicalMarkNeuronoutput )
{
	int vec_1,vec_2;
	std::tie(vec_1,vec_2) = historicalMarkIN_OUT_TO_availableNumberOfMutationsInRelationToNeuron_Vect1_Vect2({historicalMarkNeuronInput,historicalMarkNeuronoutput});
	

	if(neuronsReferencesForCreateNewSynapticWeight.at(vec_1).at(vec_2) != -1)
	{
		return neuronsReferencesForCreateNewSynapticWeight.at(vec_1).at(vec_2);
	}
	else
	{
		int _innovation=innovation++;
		neuronsReferencesForCreateNewSynapticWeight.at(vec_1).at(vec_2)=_innovation;
		return _innovation;
	}

}

// Se puede optimizar fácil, por tiempo lo dejo para después. Mas que nada se puede optimizar cuando sí existe el layer.
int GlobalInformation::getLayer( int layer_input_neuron, int layer_output_neuron)
{	
	for (int i = 0; i < (int)LayersWithReferences.size(); ++i)
	{
		// primero se compara el layer input que tiene cada uno.
		if(  layerToPlace(LayersWithReferences.at(i).at(1) ) > layerToPlace(layer_input_neuron)) // Si el layer input de la referencia es mayor entonces el nuevo layer debe agregarse justo antes que este.
		{
			LayersWithReferences.insert(LayersWithReferences.begin() + i, {layer,layer_input_neuron,layer_output_neuron});
			return layer++;
		}
		else if(  layerToPlace(LayersWithReferences.at(i).at(1) ) == layerToPlace(layer_input_neuron)) // Si tienen el mismo, aún no se sabe nada, se deben comparar el layer output primero.
		{
			if( layerToPlace(LayersWithReferences.at(i).at(2) ) > layerToPlace(layer_output_neuron) ) // Si es mayor el layer de referencia entonces el nuevo layer debe agregarse antes que este.
			{
				LayersWithReferences.insert(LayersWithReferences.begin() + i, {layer,layer_input_neuron,layer_output_neuron});
				return layer++;
			}
			else if( layerToPlace(LayersWithReferences.at(i).at(2) ) == layerToPlace(layer_output_neuron) ) // Si tiene tanto el input como el output iguales es porque son el mismo
			{
				return LayersWithReferences.at(i).at(0);
			}
		}
	}
	std::cerr << "ERROR::GlobalInformation::getLayer::Error with argumens of getLayer( int layer_input_neuron, int layer_output_neuron) " << "layer_input_neuron: "  << layer_input_neuron << "\tlayer_output_neuron: " << layer_output_neuron<< std::endl;
	exit(EXIT_FAILURE);
}

int GlobalInformation::getHistoricalMark( int historicalMarkNeuronInput, int historicalMarkNeuronoutput)
{
	int _historicalMark;
	int vec_1,vec_2;
	std::tie(vec_1,vec_2) = historicalMarkIN_OUT_TO_availableNumberOfMutationsInRelationToNeuron_Vect1_Vect2({historicalMarkNeuronInput,historicalMarkNeuronoutput});
	
	if(  neuronsReferencesForCreateNewNeurons.at(vec_1).at(vec_2) == -1  )
	{
		_historicalMark = historicalMark++;
		neuronsReferencesForCreateNewNeurons.at(vec_1).at(vec_2)=_historicalMark;

		std::vector <int> extension_1 ( 2*_historicalMark+1, -1);
		std::vector <int> extension_2 ( 2*_historicalMark+1, -1);

		neuronsReferencesForCreateNewSynapticWeight.push_back(extension_1);
		neuronsReferencesForCreateNewNeurons.push_back(extension_2);


		return _historicalMark;
	}
	else{
		return neuronsReferencesForCreateNewNeurons.at(vec_1).at(vec_2);
	}
}
void GlobalInformation::initialize(int amountOfInputs,int amountOfOutputs )
{
	historicalMark = amountOfInputs+amountOfOutputs;
	innovation = 0;
	layer=2;

	for (int i = 0; i < amountOfInputs+amountOfOutputs; ++i)
	{
		std::vector <int> extension_1 ( 2*i+1, -1);
		std::vector <int> extension_2 ( 2*i+1, -1);

		neuronsReferencesForCreateNewSynapticWeight.push_back(extension_1);
		neuronsReferencesForCreateNewNeurons.push_back(extension_2);
	}

	LayersWithReferences.push_back({LAYER_INPUT,LAYER_INICIAL_INPUT,LAYER_FINAL_INPUT});
	LayersWithReferences.push_back({LAYER_OUTPUT,LAYER_INICIAL_OUTPUT,LAYER_FINAL_OUTPUT});

}

void GlobalInformation::printLayers()
{
	std::cout << "Layer Order" << std::endl;
	for (int i = 0; i < (int)LayersWithReferences.size(); ++i)
	{
		std::cout << LayersWithReferences.at(i).at(0) << "\t"; 
	}
	std::cout << std::endl;
}

std::tuple < int,int > GlobalInformation::historicalMarkIN_OUT_TO_availableNumberOfMutationsInRelationToNeuron_Vect1_Vect2(std:: vector<int> _historicalMark)
{
	if( _historicalMark.at(0) > _historicalMark.at(1) )
	{
		return std::make_tuple(_historicalMark.at(0),_historicalMark.at(1));
	}
	else
	{
		return std::make_tuple(_historicalMark.at(1),2*_historicalMark.at(1) - _historicalMark.at(0));
	}
}


std::tuple < int,int >  GlobalInformation::availableNumberOfMutationsInRelationToNeuron_Vect1_Vect2_TO_historicalMarkIN_OUT(std:: vector<int> vect_1_vect_2)
{
	if( vect_1_vect_2.at(0) > vect_1_vect_2.at(1) )
	{
		return std::make_tuple(vect_1_vect_2.at(0),vect_1_vect_2.at(1));
	}
	else
	{
		return std::make_tuple( 2*vect_1_vect_2.at(0) - vect_1_vect_2.at(1) , vect_1_vect_2.at(0));
	}
}


int GlobalInformation::layerToPlace(int layer)
{
	if(layer == LAYER_INICIAL_INPUT)
	{
		return -1;
	}
	else if(layer == LAYER_INICIAL_OUTPUT)
	{
		return LayersWithReferences.size() + 1;
	}

	int place=-1;

	for (int i = 0; i < (int)LayersWithReferences.size(); ++i)
	{
		if(LayersWithReferences.at(i).at(0) == layer )
		{
			place = i;
			break;
		}
	}

	return place;
}

std::vector <int> GlobalInformation::layerOrdererList()
{
	std::vector <int> _ordererList;
	for (int i = 0; i < (int)LayersWithReferences.size(); ++i)
	{
		_ordererList.push_back(LayersWithReferences.at(i).at(0));
	}
	return _ordererList;
}

int GlobalInformation::getAmountOfNeurons()
{
	return historicalMark;
}

int GlobalInformation::getAmountOfSynapticWeights()
{
	return innovation;
}