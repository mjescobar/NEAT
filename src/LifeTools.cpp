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
    [](unique_ptr<Spicies>& specie)->bool { return specie->isExtinct(); }),
	spicies.end());
}

void Life::createDecendence()
{
	vector <float> fitnessVector;
	fillFitnessVector (fitnessVector);
	vector <uint> childrensPerSpicie;
	// Se obtendran la cantidad de hijos por raza a traves de una distribuion discreta segun los fitness de cada especie
	std::cerr << "lcd 1" << std::endl;

	for (uint i = 0; i < spicies.size(); ++i)
	{
		childrensPerSpicie.push_back(0);
	}
	std::cerr << "lcd 2" << std::endl;
	discrete_distribution<uint> obtainSpicie(fitnessVector.begin(), fitnessVector.end());
	uint selected = 0;
	for (uint i = 0; i < maxAmountOrganismInAllOldRaces; ++i)
	{
		selected = obtainSpicie(*generator);
		childrensPerSpicie.at(selected) += 1;
	}	
	// Ahora dado que ya se sabe la cantidad de hijos se procede a asignarlos a cada especie.
	std::cerr << "lcd 3" << std::endl;
	for (uint i = 0; i < spicies.size(); ++i)
	{
		spicies.at(i)->epoch( childrensPerSpicie.at(i) );
	}
	std::cerr << "lcd 4" << std::endl;
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
			unique_ptr <Organism> organism = spicies.at(spiciePos)->getOrganismNewSpiciesCandidate();
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
	float speciesMean = 0.f; 
	float min = -1.f;
	float max = 0.f;
	float fitness;
	for(auto& specie : spicies)
	{
		fitness = specie->getMeanFitnessOfOldRaces();
		if( fitness > min )
		{
			min = fitness;
		}
		if(fitness < max)
		{
			max = fitness;
		}
		speciesMean += fitness;
	}
	speciesMean = speciesMean/(float)spicies.size();

	if(min == max) {return;} // some rare case;

	// ToDo: Mejorar el modelo tal que, por ejemplo, la probabilidad de supervivencia sea una gaussiana con la misma media y promedio que las especies (solo para las especies de fitness menor que la media)
	spicies.erase(  remove_if(spicies.begin(), spicies.end(),
    [&](unique_ptr<Spicies>& specie)->bool 
    { 
    	if( specie->oldRaces.size() > 0 )
		{
			if( specie->getMeanFitnessOfOldRaces() < speciesMean )
			{
				return true;
			}
			return false;
		}
		return false;
     }),spicies.end());
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


} // end namespace NEAT