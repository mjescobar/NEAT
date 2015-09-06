#ifndef REFERENCE_MAP_VECTOR_HPP
#define REFERENCE_MAP_VECTOR_HPP

#include <iostream>
#include <vector>
#include "GlobalInformation.hpp"
#include "Neuron.hpp"
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

				std::cerr << "ERROR::ReferenceMapVector::getAvaibleAtPlace::Not avaivle place found" << std::endl;
				exit(EXIT_FAILURE);
			}

			void set ( int absolutePlace, int value )
			{
				coordinates [ absolutePlace ] = value;
				amountOfAvaibleMutations--;
			}

			int get ( int absolutePlace )
			{
				return coordinates [ absolutePlace ];
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

			void print()
			{
				for (int i = 0; i < initialSize-1; ++i)
				{
					std::cout << coordinates[i] << "  "; 
				}
				std::cout << coordinates[initialSize-1];
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
}
#endif