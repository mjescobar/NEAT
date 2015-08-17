#include <iostream>
#include <vector>
#include "GlobalInformation.hpp"
#include "Neuron.h"
#include <cstdlib>
#include <algorithm>

#define NOT_CONNECTIONS_BACK -2
#define AVAIBLE_MUTATION -1
#define NOT_MUTATION_AVAIBLE -3

namespace NEATSpikes
{

	class ReferenceMapVector
	{
		public: 
			ReferenceMapVector()
			{

			}

			ReferenceMapVector(int position)
			{
				this->position = position;
				initialSize = position*2 + 1;  
				coordinates = new int[initialSize];
				amountOfAvaibleMutations = initialSize;
				std::fill_n(coordinates, initialSize, AVAIBLE_MUTATION);
			}

			ReferenceMapVector(int position, GlobalInformation * globalInformation, std::vector < Neuron * > * neurons, bool connectionsBack)
			{
				// Se crea este vector y se setean los valores.
				this->position = position;
				this->connectionsBack = connectionsBack;
				initialSize = position*2 + 1;  
				coordinates = new int[initialSize];
				amountOfAvaibleMutations = initialSize;
				this->neurons = neurons;
				this->globalInformation = globalInformation;
				std::fill_n(coordinates, initialSize, AVAIBLE_MUTATION);

				// En caso de que se prohiba conecciones recursivas se deben eliminar todas las opciones recursivas.
				if(connectionsBack)
				{
					amountOfAvaibleMutations -=  ((initialSize-1)/2 + 1) ; // se restan altiro todos los casos en que uno de los dos tienen un layer mas grande que el otro y el caso con sigo mismo, solo faltan los casos en que ambos tienen el mismo layer, en esos casos hay que disminuir en 1 este valor.
					coordinates[position] = NOT_CONNECTIONS_BACK;
					int thisNeuronLayerPlace = globalInformation->layerToPlace((*neurons).at(position)->getLayer());
					//Se mira la primera mitad nada mas pues la siguiente mitad es inverso.
					for (int i = 0; i < (initialSize-1)/2; ++i)
					{
						int otherNeuronLayerPlace = globalInformation->layerToPlace((*neurons).at(i)->getLayer()); 
						
						if( thisNeuronLayerPlace >= otherNeuronLayerPlace )
						{
							coordinates[i] = NOT_CONNECTIONS_BACK;
							if(thisNeuronLayerPlace == otherNeuronLayerPlace)
							{
								coordinates[initialSize - 1 -i] = NOT_CONNECTIONS_BACK;
								amountOfAvaibleMutations--;
							}
						}
						else
						{
							coordinates[initialSize -1 -i] = NOT_CONNECTIONS_BACK;
						}
					}
				}
			}

			~ReferenceMapVector()
			{
				delete[] coordinates;
			}

			int getAmountOfAvaibleMutations()
			{
				return amountOfAvaibleMutations;
			}

			int getAvaibleAtPlace (int place)
			{
				int count = 0; 
				for (int i = 0; i < initialSize; ++i)
				{
					if(coordinates[i] == AVAIBLE_MUTATION)
					{
						if(count == place)
						{
							return i;
						}
						count++;
					}
				}
			}

			void set ( int absolutePlace, int value )
			{
				coordinate [ absolutePlace ] = value;
				amountOfAvaibleMutations--;
			}

			int get ( int absolutePlace )
			{
				return coordinate [ absolutePlace ];
			}

			ReferenceMapVector * duplicate()
			{
				ReferenceMapVector * result = new ReferenceMapVector(position);
				for (int i = 0; i < position*2 + 1; ++i)
				{
					result->coordinates[i] =  this->coordinates[i];
				}
				result->amountOfAvaibleMutations = this->amountOfAvaibleMutations;
				result->connectionsBack = this->connectionsBack;
				result->neurons = this->neurons;
				return result;
			}

		private:
			int amountOfAvaibleMutations;
			int * coordinates;
			int position;
			bool connectionsBack;
			std::vector < Neuron * > * neurons;
			int initialSize;
			GlobalInformation * globalInformation;
	};
//============================================================================================================
//============================================================================================================
//============================================================================================================
//============================================================================================================
//============================================================================================================
	class ReferenceMap
	{
		public:
			ReferenceMap()
			{

			}

			ReferenceMap(GlobalInformation * globalInformation, std::vector < Neuron * > * neurons,  bool connectionsBack, int inputAmount, int outputAmount )
			{
				this->globalInformation = globalInformation;
				currentPosition = 0;
				this->connectionsBack = connectionsBack;
				this->neurons = neurons;

				for (int i = 0; i < inputAmount + outputAmount; ++i)
				{
					addNewReferenceMapVector();
				}
			}

			~ReferenceMap()
			{
				for (int i = 0; i < referenceNeurons.size(); ++i)
				{
					delete referenceNeurons.at(i);
				}
			}

			void addNewReferenceMapVector()
			{
				ReferenceMapVector * newReferenceMapVector = new ReferenceMapVector(currentPosition++, globalInformation, neurons, connectionsBack);
				amountOfAvaibleMutations += newReferenceMapVector->getAmountOfAvaibleMutations();
				referenceNeurons.push_back(newReferenceMapVector);
			}



			vector <int> obtainAvaibleMutationRandomly()
			{
				if(amountOfAvaibleMutations == 0)
				{
					return {NOT_MUTATION_AVAIBLE,NOT_MUTATION_AVAIBLE};
				}

				int randomIndex = rand()%amountOfAvaibleMutations;
				int count = -1;
				for (int i = 0; i < referenceNeurons.size(); ++i)
				{
					count += referenceNeurons.at(i).getAmountOfAvaibleMutations();
					if(count >= randomIndex)
					{
						return { i , referenceNeurons.at(i).getAvaibleAtPlace(count - randomIndex) };
					}
				}

				std::cerr << "ERROR::MutationControl::ReferenceMap::obtainAvaibleMutationRandomly:: Not mutation found " << std::endl;
				exit (EXIT_FAILURE);
			}

			void set( vector <int> place, int value )
			{
				amountOfAvaibleMutations--;
				referenceNeurons.at(place.at(0)).set(place.at(1), value);
			}

			int get ( vector <int> place)
			{
				return referenceNeurons.at(place.at(0)).get(place.at(1));
			}


			ReferenceMap * duplicate()
			{
				ReferenceMap * result-> = new ReferenceMap();
				for (int i = 0; i < referenceNeurons.size(); ++i)
				{
					result->referenceNeurons.push_back( this->referenceNeurons.at(i).duplicate() );
				}
				result->currentPosition = this->currentPosition;
				result->amountOfAvaibleMutations = this->amountOfAvaibleMutations;
				result->globalInformation = this->globalInformation;
				result->connectionsBack = this->connectionsBack;
				result->neurons = this->neurons;
			}
			 
			int getAmountOfAvaibleMutations()
			{
				return amountOfAvaibleMutations;
			}
			vector <int> getHistoricalMarkFromReferencePosition(vector <int> ReferencePosition)
			{
				int in, out, historicalMarkIn, historicalMarkOut;
				if( ReferencePosition.at(0) <= ReferencePosition.at(1) )
				{
					in = ReferencePosition.at(0);
					out = ReferencePosition.at(1);
				}
				else
				{
					in = 2*ReferencePosition.at(0) - ReferencePosition.at(1);
					out = ReferencePosition.at(0);
				}
				historicalMarkIn = (*neurons).at(in)->getHistoricalMark();
				historicalMarkOut = (*neurons).at(out)->getHistoricalMark();
				return { historicalMarkIn , historicalMarkOut };
			}

		private:
			int currentPosition;
			std::vector < ReferenceMapVector > referenceNeurons;
			int amountOfAvaibleMutations;//Cantidad Total de mutaciones.
			GlobalInformation * globalInformation;
			bool connectionsBack;
			std::vector < Neuron * > * neurons;
	};
}