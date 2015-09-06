#ifndef INFORMATIONBETWEENLIFEANDORGANISM_HPP
#define INFORMATIONBETWEENLIFEANDORGANISM_HPP


#define LAYER_INPUT 0
#define LAYER_OUTPUT 1
#define LAYER_INICIAL_INPUT -1
#define LAYER_FINAL_INPUT -1
#define LAYER_INICIAL_OUTPUT -2
#define LAYER_FINAL_OUTPUT -2
#define AVAIBLE_MUTATION -1
#define BACK_CONNECTION -2


#include <iostream>
#include <vector>
#include <tuple>
#include <cstdlib> // RAND_MAX, EXIT_FAILURE, srand, rand 

#define INPUT_INICIAL_IN_OUT -2
#define OUTPUT_INICIAL_IN_OUT -3

namespace NEATSpikes{
	class GlobalInformation{
	public:
		GlobalInformation();
		int getInnovation( int historicalMarkNeuronInput, int historicalMarkNeuronoutput );
		int getLayer( int layer_input_neuron, int layer_output_neuron );
		int getLayer( int historicalMark );
		int getHistoricalMark( int historicalMarkNeuronInput, int historicalMarkNeuronoutput);
		void printLayers();
		std::vector <int> layerOrdererList();
		int layerToPlace(int layer);
		int getAmountOfNeurons();
		int getAmountOfSynapticWeights();
		int getNeuronInputHistoricalMark();
		int getNeuronInputLayer();
		int getNeuronOutputHistoricalMark();
		int getNeuronOutputLayer();

	private:
		/*
			\brief
		*/
		std::tuple < int,int >  historicalMarkToVector(std:: vector<int> neuron_in_out); // Es muy complicado el nombre pero
		/**
			\brief 
		*/	
		std::tuple < int,int >  vectorToHistoricalMark(std:: vector<int> vect_1_vect_2); // Es muy complicado el nombre pero

		std::vector < std::vector <int> > newNeuronPosibilities;
		std::vector < std::vector <int> > newSynapticWeightPosibilities;
		int historicalMark;
		int innovation;
		int layer;
		std::vector < std::vector <int> > neuronsReferencesForCreateNewSynapticWeight;
		std::vector < std::vector <int> > neuronsReferencesForCreateNewNeurons;
		std::vector < std::vector <int> > LayersWithReferences;
		std::vector < int > hytoricalMarkToLayer; // En este vector almacena la conversion desde historical mark a layer
	};
}


#endif