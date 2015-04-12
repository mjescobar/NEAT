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


namespace NEATSpikes{
	class GlobalInformation{
	public:
		int getInnovation( int historicalMarkNeuronInput, int historicalMarkNeuronoutput );
		int getLayer( int layer_input_neuron, int layer_output_neuron);
		int getHistoricalMark( int historicalMarkNeuronInput, int historicalMarkNeuronoutput);
		void initialize(int amountOfInputs,int amountOfOutputs );
		void printLayers();
		std::vector <int> layerOrdererList();
		int layerToPlace(int layer);
	private:
			//ENCONTRAR UN MEJOR NOMBRE!!!!
		/*
			\brief
		*/
		std::tuple < int,int >  historicalMarkIN_OUT_TO_availableNumberOfMutationsInRelationToNeuron_Vect1_Vect2(std:: vector<int> neuron_in_out); // Es muy complicado el nombre pero
		/**
			\brief 
		*/	
			//ENCONTRAR UN MEJOR NOMBRE!!!!
		std::tuple < int,int >  availableNumberOfMutationsInRelationToNeuron_Vect1_Vect2_TO_historicalMarkIN_OUT(std:: vector<int> vect_1_vect_2); // Es muy complicado el nombre pero


		


		std::vector < std::vector <int> > newNeuronPosibilities;
		std::vector < std::vector <int> > newSynapticWeightPosibilities;

		int historicalMark;
		int innovation;
		int layer;
		std::vector < std::vector <int> > neuronsReferencesForCreateNewSynapticWeight;
		std::vector < std::vector <int> > neuronsReferencesForCreateNewNeurons;


		std::vector < std::vector <int> > LayersWithReferences;

	};
}


#endif