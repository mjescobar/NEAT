#include "GlobalInformation.hpp"

using namespace NEATSpikes;

GlobalInformation::GlobalInformation()
{
	LayersWithReferences.push_back({LAYER_INPUT,LAYER_INICIAL_INPUT,LAYER_FINAL_INPUT});
	LayersWithReferences.push_back({LAYER_OUTPUT,LAYER_INICIAL_OUTPUT,LAYER_FINAL_OUTPUT});
}


int GlobalInformation::getInnovation( int historicalMarkNeuronInput, int historicalMarkNeuronoutput )
{
	int vec_1,vec_2;
	std::tie(vec_1,vec_2) = historicalMarkToVector({historicalMarkNeuronInput,historicalMarkNeuronoutput});
	

	if(neuronsReferencesForCreateNewSynapticWeight.at(vec_1).at(vec_2) != -1)
	{
		return neuronsReferencesForCreateNewSynapticWeight.at(vec_1).at(vec_2);
	}
		
	int _innovation = innovation++;
	neuronsReferencesForCreateNewSynapticWeight.at(vec_1).at(vec_2) = _innovation;
	return _innovation;
}

// Se puede optimizar fácil, por tiempo lo dejo para después. Mas que nada se puede optimizar cuando sí existe el layer.
int GlobalInformation::getLayer( int layer_input_neuron, int layer_output_neuron )
{	
	if(layer_input_neuron == layer_output_neuron)
	{
		return layer_input_neuron;
	}

	// Lo primero es que para obtener simetría en conexiones hacia atrás estará pensado para layer menor y layer mayor sin pensar si es input u output
	// ==============================================================================
		int placeSmaller;

		if(layerToPlace(layer_input_neuron) > layerToPlace(layer_output_neuron) )
		{
			placeSmaller = layerToPlace(layer_output_neuron);
		}
		else
		{
			placeSmaller = layerToPlace(layer_input_neuron);
		}
	// ==============================================================================

	for (unsigned int i = 0; i < LayersWithReferences.size(); ++i)
	{
		if(LayersWithReferences.at(i).at(1) == layer_input_neuron && LayersWithReferences.at(i).at(2) == layer_output_neuron)
		{
			return LayersWithReferences.at(i).at(0);
		}

		if( layerToPlace( LayersWithReferences.at(i).at(0) ) > placeSmaller )
		{
			LayersWithReferences.insert(LayersWithReferences.begin() + i, {layer,layer_input_neuron,layer_output_neuron});
			return layer++;
		}
	}

	std::cerr << "ERROR::GlobalInformation::getLayer::Error with argumens of getLayer( int layer_input_neuron, int layer_output_neuron) " << "layer_input_neuron: "  << layer_input_neuron << "\tlayer_output_neuron: " << layer_output_neuron<< std::endl;
	exit(EXIT_FAILURE);
}

int GlobalInformation::getLayer( int historicalMark )
{
	if(historicalMark < this->historicalMark){
		return hytoricalMarkToLayer.at(historicalMark);
	}
	std::cerr << "ERROR::GlobalInformation::getLayer:: historicalMark dont exists" << std::endl; 
	exit(EXIT_FAILURE);
}



int GlobalInformation::getHistoricalMark( int historicalMarkNeuronInput, int historicalMarkNeuronoutput)
{
	//Primero se obtendra el historicalMark y luego se calculara el layer correspondiente y se gaurdara la conversion de uhno al otro en hytoricalMarkToLayer

	int _historicalMark;
	int vec_1,vec_2;
	// Se obtiene la posicion en el mapa de referencia de neuronas que corresponde a las historicalMark entrada y salida.
	std::tie(vec_1,vec_2) = historicalMarkToVector({historicalMarkNeuronInput,historicalMarkNeuronoutput});
	
	//Si ya existe una neurona que use la posicion en la coordenada entonces tienen el mismo historicalMark y simplemente se devuevle el valor, en otro caso habra que poner el valor en la coordenada correspondiente y agregar la nueva coordenas que dado esta nueva neurona existiran nuevas coordenadas.
	if(  neuronsReferencesForCreateNewNeurons.at(vec_1).at(vec_2) == -1  )
	{
		_historicalMark = historicalMark++;
		neuronsReferencesForCreateNewNeurons.at(vec_1).at(vec_2)=_historicalMark;

		std::vector <int> extension_1 ( 2*_historicalMark+1, -1);
		std::vector <int> extension_2 ( 2*_historicalMark+1, -1);

		neuronsReferencesForCreateNewSynapticWeight.push_back(extension_1);
		neuronsReferencesForCreateNewNeurons.push_back(extension_2);
	}
	else
	{
		return neuronsReferencesForCreateNewNeurons.at(vec_1).at(vec_2);
	}

	//Ahora se calcula el valor del layer.
	//==========================================================================
	int layerInputNeuron = hytoricalMarkToLayer.at(historicalMarkNeuronInput);
	int layerOutputNeuron = hytoricalMarkToLayer.at(historicalMarkNeuronoutput);
	int layer = getLayer(layerInputNeuron,layerOutputNeuron);
	hytoricalMarkToLayer.push_back(layer);

	return _historicalMark;
}

/*void GlobalInformation::initialize(int amountOfInputs,int amountOfOutputs )
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

}*/

void GlobalInformation::printLayers()
{
	std::cout << "Layer Order" << std::endl;
	for (int i = 0; i < (int)LayersWithReferences.size(); ++i)
	{
		std::cout << LayersWithReferences.at(i).at(0) << "\t"; 
	}
	std::cout << std::endl;
}

std::tuple < int,int > GlobalInformation::historicalMarkToVector(std:: vector<int> _historicalMark)
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


std::tuple < int,int >  GlobalInformation::vectorToHistoricalMark(std:: vector<int> vect_1_vect_2)
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

int GlobalInformation::getNeuronInputHistoricalMark()
{
	int historical = historicalMark;
	hytoricalMarkToLayer.push_back(LAYER_INPUT);


	//Se actualizan los mapas.
	std::vector <int> extension_1 ( 2*historicalMark+1, -1);
	std::vector <int> extension_2 ( 2*historicalMark+1, -1);
	neuronsReferencesForCreateNewSynapticWeight.push_back(extension_1);
	neuronsReferencesForCreateNewNeurons.push_back(extension_2);

	//Se agergan a los layers
	getLayer(LAYER_INICIAL_INPUT,LAYER_FINAL_INPUT);
	

	//Se actualizan los layers
	historicalMark++;
	return historical;
}

int GlobalInformation::getNeuronInputLayer()
{
	return LAYER_INPUT;
}


int GlobalInformation::getNeuronOutputHistoricalMark()
{
	int historical = historicalMark;
	hytoricalMarkToLayer.push_back(LAYER_OUTPUT);
	//Se actualizan los mapas.
	std::vector <int> extension_1 ( 2*historicalMark+1, -1);
	std::vector <int> extension_2 ( 2*historicalMark+1, -1);
	neuronsReferencesForCreateNewSynapticWeight.push_back(extension_1);
	neuronsReferencesForCreateNewNeurons.push_back(extension_2);

	//Se agergan a los layers
	getLayer(LAYER_INICIAL_OUTPUT,LAYER_FINAL_OUTPUT);
	

	//Se actualizan los layers
	historicalMark++;
	return historical;
}

int GlobalInformation::getNeuronOutputLayer()
{
	return LAYER_OUTPUT;
}