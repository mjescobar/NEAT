#ifndef REFERENCE_MAP_HPP
#define REFERENCE_MAP_HPP

#include <iostream>
#include <vector>
#include "GlobalInformation.hpp"
#include "Neuron.hpp"
#include <cstdlib>
#include <algorithm>
#include "ReferenceMapVector.hpp"

namespace NEATSpikes
{
	class ReferenceMap
	{
		public:
			ReferenceMap()
			{

			}

			ReferenceMap(GlobalInformation * globalInformation, std::vector < Neuron * > * neurons,  bool connectionsBack )
			{
				this->globalInformation = globalInformation;
				currentPosition = 0;
				this->connectionsBack = connectionsBack;
				this->neurons = neurons;
			}

			~ReferenceMap()
			{
				for (int i = 0; i < (int)referenceNeurons.size(); ++i)
				{
					//referenceNeurons.erase (referenceNeurons.begin(),referenceNeurons.end());
					delete referenceNeurons.at(i);
				}
			}

			void addNewReferenceMapVector()
			{
				ReferenceMapVector * newReferenceMapVector = new ReferenceMapVector(currentPosition++, globalInformation, neurons, connectionsBack);
				amountOfAvaibleMutations += newReferenceMapVector->getAmountOfAvaibleMutations();
				referenceNeurons.push_back(newReferenceMapVector);
			}



			std::vector <int> obtainAvaibleMutationRandomly()
			{

				if(amountOfAvaibleMutations == 0)
				{
					return {NOT_MUTATION_AVAIBLE,NOT_MUTATION_AVAIBLE};
				}

				int randomIndex = rand()%amountOfAvaibleMutations;
				int count = -1;
				for (int i = 0; i < (int)referenceNeurons.size(); ++i)
				{
					count += referenceNeurons.at(i)->getAmountOfAvaibleMutations();
					if(count >= randomIndex)
					{
						return { i , referenceNeurons.at(i)->getAvaibleAtPlace(count - randomIndex) };
					}
				}

				std::cerr << "ERROR::MutationControl::ReferenceMap::obtainAvaibleMutationRandomly:: Not mutation found " << std::endl;
				exit (EXIT_FAILURE);
			}

			void set( std::vector <int> place, int value )
			{
				amountOfAvaibleMutations--;
				referenceNeurons.at(place.at(0))->set(place.at(1), value);
				while(  value >= (int)valueToPosition.size() )
				{
					valueToPosition.push_back({});
				}
				valueToPosition.at(value) = place;
			}

			int get ( std::vector <int> place)
			{
				return referenceNeurons.at(place.at(0))->get(place.at(1));
			}


			ReferenceMap * duplicate()
			{
				ReferenceMap * result = new ReferenceMap();
				for (int i = 0; i < (int)referenceNeurons.size(); ++i)
				{
					result->referenceNeurons.push_back( this->referenceNeurons.at(i)->duplicate() );
				}
				result->currentPosition = this->currentPosition;
				result->amountOfAvaibleMutations = this->amountOfAvaibleMutations;
				result->globalInformation = this->globalInformation;
				result->connectionsBack = this->connectionsBack;
				result->neurons = this->neurons;
				return result;
			}
			 
			int getAmountOfAvaibleMutations()
			{
				return amountOfAvaibleMutations;
			}
			
			std::vector <int> getHistoricalMarkFromReferencePosition(std::vector <int> ReferencePosition)
			{
				int in, out, historicalMarkIn, historicalMarkOut;
				if( ReferencePosition.at(0) <= ReferencePosition.at(1) )
				{
					in = 2*ReferencePosition.at(0) - ReferencePosition.at(1);
					out = ReferencePosition.at(0);
				}
				else
				{
					in = ReferencePosition.at(0);
					out = ReferencePosition.at(1);
				}
				historicalMarkIn = (*neurons).at(in)->getHistoricalMark();
				historicalMarkOut = (*neurons).at(out)->getHistoricalMark();
				return { historicalMarkIn , historicalMarkOut };
			}

			std::vector <int> getPlaceOfValue(int value)
			{
				if((int)valueToPosition.size() > value)
				{
					if(valueToPosition.at(value).size() != 0)
					{
						return valueToPosition.at(value);
					}
				}
				
				std::cerr << "ERROR::ReferenceMap::getPlaceOfValue::Value has not been stored in the map before this call." << std::endl;
				exit(EXIT_FAILURE);
			}
			void print()
			{
				for (unsigned int i = 0; i < referenceNeurons.size(); ++i)
				{
					std::cout << "Row " << i  << ":\t";
					referenceNeurons.at(i)->print();
					std::cout << std::endl;
				}
			}
		private:
			int currentPosition;
			std::vector < ReferenceMapVector * > referenceNeurons;
			int amountOfAvaibleMutations;//Cantidad Total de mutaciones.
			GlobalInformation * globalInformation;
			bool connectionsBack;
			std::vector < Neuron * > * neurons;
			std::vector < std::vector <int> > valueToPosition; // Dado que los valores deben, por obligacion ser diferentes cada vez entonces solo existe una posicion para un valor dado.
	};
}
#endif