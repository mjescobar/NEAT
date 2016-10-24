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
	// Existen dos tipos de razas, las nuevas que tienen asegurado una cantidad de hijos (establecido por el usuario) y las viejas.

	// Primero las razas viejas se reparten los hijos.

	// Se pasan todos los organismos a un vector llamado organisms y se mantiene un mapeo entre este vector y su posicion en la especie y raza correspondiente.
	map <uint, vector <uint> > organismsToStructure_map;
	vector < Organism *  > organisms;
	for ( uint i = 0; i < spicies.size(); i++){ 
		for( uint j = 0; j < spicies.at(i)->oldRaces.size(); j++){
			for( uint k = 0; k < spicies.at(i)->oldRaces.at(j)->oldOrganisms.size(); k++){
				organisms.push_back( spicies.at(i)->oldRaces.at(j)->oldOrganisms.at(k).get() );
				vector <uint> structurePosition = {i,j,k};
				organismsToStructure_map.emplace(organisms.size()-1,structurePosition);
	}	}	}

	// Se crea un vector con los fitness de cada uno de los organismos para ser usado como distribucion de probabilidad discreta mas adelante.
	vector <float> fitnessVector;
	for (uint i = 0; i < organisms.size(); ++i)
	{
		fitnessVector.push_back( organisms.at(i)->getAdjustedFitness() );
	}
	// se crea la funcion de probabilidad asociada a los fitness.
	discrete_distribution<uint> obtainFatherOrganism(fitnessVector.begin(), fitnessVector.end());


	//Segun la probabilidad anterior se obtienen los padres para los nuevos hijos.
	for (uint i = 0; i < maxAmountOrganismInAllOldRaces; ++i)
	{
		uint father = obtainFatherOrganism(*generator);
		vector <uint> structurePositionFather = organismsToStructure_map.at(father);
		uint specieOfFather = structurePositionFather.at(0);
		uint raceOfFather = structurePositionFather.at(1);
		uint organismPositionOfFather = structurePositionFather.at(2);
		spicies.at(specieOfFather)->oldRaces.at(raceOfFather)->getChildFromParentAt(organismPositionOfFather);
	}	

	// Luego las razas nuevas pasaran de epoca
	for( auto& specie : spicies ){
		specie->newRacesDecendece(); // tienen asegurado una cantidad de hijos
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
