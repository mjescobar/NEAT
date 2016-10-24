#include "Life.hpp"

#include <chrono>
#include <iostream>
#include <algorithm>

using namespace std;

namespace NEAT
{

void Life::deleteExtinctedSpicies()
{
	spicies.erase(  remove_if(spicies.begin(), spicies.end(),
    [](shared_ptr<Spicies>& specie)->bool { return specie->isExtinct(); }),
	spicies.end());
}

void Life::createDecendence()
{
	vector <float> fitnessVector;
	fillFitnessVector (fitnessVector);
	vector <uint> childrensPerSpicie;
	// Se obtendran la cantidad de hijos por raza a traves de una distribuion discreta segun los fitness de cada especie

	for (uint i = 0; i < spicies.size(); ++i)
	{
		childrensPerSpicie.push_back(0);
	}
	discrete_distribution<uint> obtainSpicie(fitnessVector.begin(), fitnessVector.end());
	uint selected = 0;
	for (uint i = 0; i < maxAmountOrganismInAllOldRaces; ++i)
	{
		selected = obtainSpicie(*generator);
		childrensPerSpicie.at(selected) += 1;
	}	
	// Ahora dado que ya se sabe la cantidad de hijos se procede a asignarlos a cada especie.
	for (uint i = 0; i < spicies.size(); ++i)
	{
		spicies.at(i)->epoch( childrensPerSpicie.at(i) );
	}
}
void Life::fillFitnessVector (vector <float> & fitnessVector)
{
	fitnessVector.clear();
	for( const auto& spicie : spicies )
	{
		fitnessVector.push_back( spicie->getMeanFitnessOfOldRaces() );
	}
}
void Life::createSpiciesFromOrganismCandidates()
{
	if(spicies.size() >= maxAmountOfSpicies) {return ;}
	uniform_int_distribution<uint> randomSpicie(0, 
							spicies.size() -1);
	for (uint k = spicies.size(); k < maxAmountOfSpicies; ++k)
	{
		uint attemps = 2;
		for (uint i = 0; i < attemps; ++i)
		{
			uint spiciePos =  randomSpicie(*generator);
			shared_ptr <Organism> organism = spicies.at(spiciePos)->getOrganismNewSpiciesCandidate();
			if( organism != nullptr ) // If is diferent than nullptr then one orgm was founded
			{
				organism->ann->isNewSpecies = false;
				auto race_tmp = make_unique <Race>( move(organism) );
				auto spicie_tmp = make_unique<Spicies>( move(race_tmp));
				spicies.push_back( move(spicie_tmp) );
				break;
			}
		}
	}
}

void Life::eliminateWorseSpecies()
{  // Only old species could be eliminated
	uint protectedSpeciesAmount = round(0.5*maxAmountOfSpicies);
	if(protectedSpeciesAmount == 0) 
	{
		protectedSpeciesAmount = 1;
	}
	if(  protectedSpeciesAmount < spicies.size() )
	{
		float min = spicies.at(0)->getMeanFitnessOfOldRaces();;
		int min_position = -1;
		float fitness;
		float eraseWorseSpicieRate = 0.5;

		if(rand()/(double)RAND_MAX < eraseWorseSpicieRate)
		{
			for (uint i = 0; i < spicies.size(); ++i)
			{
				fitness = spicies.at(i)->getMeanFitnessOfOldRaces();
				if( fitness <= min )
				{
					min = fitness;
					min_position = i;
				}
			}	
			if(min_position < 0)
			{
				cerr << "ERROR::Life::eliminateWorseSpecies:: position of worse specie is impossible."  << min_position<< endl;
				exit(EXIT_FAILURE);
			}
			spicies.erase (spicies.begin() + min_position);
		}
	}
}

void Life::eliminateWorseRaces()
{
	for(auto& specie : spicies)
	{
		specie->eliminateWorseRaces();
	}
}

void Life::eliminateWorseOrganisms()
{
	for(auto& specie : spicies)
	{
		specie->eliminateWorseOrganisms();
	}
}

int Life::getSizeOrganism() {
	int count = 0;
	for ( auto& spicie_ : this->spicies)
	{
		for( auto& race: spicie_->youngRaces)
		{
			count += race->newOrganisms.size();
		}
		for( auto& race: spicie_->oldRaces)
		{
			count += race->newOrganisms.size();
		}
	}
	return count;

}

std::shared_ptr<Organism> Life::getOrganism(int i) {
	int count = 0;
	for ( auto& spicie_ : this->spicies)
	{
		for( auto& race: spicie_->youngRaces)
		{
			for( auto& orgm : race->newOrganisms )
			{
				if(count == i)
					return orgm;
				count++;
			}
		}
		for( auto& race: spicie_->oldRaces)
		{
			for( auto& orgm : race->newOrganisms )
			{
				if(count == i)
					return orgm;
				count++;
			}
		}
	}
	return nullptr;

}


} // end namespace NEAT
