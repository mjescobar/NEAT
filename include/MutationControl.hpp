
#ifndef MUTATION_CONTROL_HPP
#define MUTATION_CONTROL_HPP

#include <iostream>
#include <vector>
#include "GlobalInformation.hpp"
#include "Neuron.hpp"
#include "SynapticWeight.hpp"
#include "ReferenceMap.hpp"
#include <cstdlib>
#include <algorithm>
//Se encarga de encontrar las mutaciones de una red neuronal que desea mutar para crear una nueva neurona o una nueva coneccion sinaptica dado la complejidad de encontrarles una posicion que sea absolutamente aleatoria y equiprobable. Hay que tomar en cuenta la diferencia cuando se pueden hacer conecciones recursivas y cuando no se pueden lo que le agrerga un grado de complejidad al algoritmo.

namespace NEATSpikes
{

	class MutationControl
	{
		public:
			MutationControl();
			MutationControl(GlobalInformation * globalInformation, std::vector < Neuron * > * neurons,  bool connectionsBack, std::vector < SynapticWeight * > * synapticWeights, std::vector <int> * innovationToSynapticWeight, std::vector <int> * historicalMarkToNeuron, Neuron * neuronPrototype, SynapticWeight * synapticWeightPrototype,std::vector < std::vector< int > > * historicalMarkAtLayer );

			~MutationControl();
			/**
				\brief Se crea una nueva neurona por mutacion.
			*/
			void mutateNewNeuron();
			/**
				\brief Se crea una nueva conneccion sinaptica por mutacion.
			*/
			void mutateNewSynapticWeight();
			/**
				\brief Al momento de crear una nueva red neuronal debe poseer su propia copia de control de mutaciones.
			*/
			MutationControl * duplicate(std::vector < Neuron * > * neurons, std::vector < SynapticWeight * > * synapticWeights, std::vector<int> * innovationToSynapticWeight, std::vector <int> * historicalMarkToNeuron, std::vector < std::vector< int > > * historicalMarkAtLayer); // Esas entradas tienen que ser dadas al momento de crear la copia pues no pueden ser obtenidas desde una instancia de MutationControl para funcionar.
			/**
				\brief Devuelve la cantidad de mutaciones de neurona posibles.
			*/
			int getAmountOfNeuronMutationAvaible();
			/**
				\brief Devuelve la cantidad de mutaciones de coneccion sinaptica posibles.
			*/
			int getAmountOfSynapticWeightMutationAvaible();
			/**
				\brief Se agrega una neurona a la estructura neuronal. Ojo que no crea una neurona nueva a partir de la que se le de, se debe dar una neurona nueva desde la clase que llame a este metodo.
			*/
			void addNeuron(Neuron * neuron);
			/**
				\brief Se agrega una coneccion sinaptica a la estructura neuronal. Ojo que no crea una coneccion nueva a partir de la que se le de, se debe dar una coneccion sinaptica nueva desde la clase que llame a este metodo.
			*/
			void addSynapticWeight( SynapticWeight * synapticWeight );
			/**
				\brief Se imprime la informacion de MutationControl.
			*/
			void print();

			void AddInitialNeuron(Neuron * neuron);
		private:
			ReferenceMap * referenceMapForSynapticsWeight;
			ReferenceMap * referenceMapForNeurons;
			GlobalInformation * globalInformation;
			std::vector < Neuron * > * neurons;
			std::vector < SynapticWeight * > * synapticWeights;
			std::vector <int> * innovationToSynapticWeight;
			std::vector <int> * historicalMarkToNeuron;
			Neuron * neuronPrototype;
			SynapticWeight * synapticWeightPrototype; 
			std::vector < std::vector< int > > * historicalMarkAtLayer;
	};
}

#endif