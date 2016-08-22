#include "Life.hpp"

#include <chrono>

namespace NEAT
{


Life::Life(std::unique_ptr<ANN> annSeed )
{
	auto lud = std::make_unique <LifeUserDefinitions>();
	// Se crea la primera especie.
	auto  oud = std::make_unique<OrganismUserDefinitions> ();  
	auto firstOrganism = std::make_unique <Organism>( *oud, std::move(annSeed) );
	auto rud = std::make_unique<RaceUserDefinitions>();  
	auto firstRace = std::make_unique <Race>( *rud, std::move(firstOrganism) );
	auto sud = std::make_unique< SpiciesUserDefinitions > ();
	auto firstSpicie = std::make_unique<Spicies>(*sud, std::move(firstRace));
	spicies.push_back( std::move(firstSpicie) );
	maxAmountOfSpicies = lud->maxAmountOfSpicies;
	generator = std::make_unique < std::default_random_engine > (std::chrono::system_clock::now().time_since_epoch().count());
}

void Life::epoch()
{
	createDecendence();
	createSpiciesFromOrganismCandidates();
	deleteExtinctedRaces();
	currentGeneration++;
}

void Life::deleteExtinctedRaces()
{
	for (uint i = 0; i < spicies.size(); ++i)
	{
		if( spicies.at(i)->isExtincted() )
		{
			spicies.erase( spicies.begin() + i );
			i--;
		}
	}
}

void Life::createDecendence()
{
	std::vector <float> fitnessVector;
	fillFitnessVector (fitnessVector);
	std::vector <uint> childrensPerSpicie;
	// Se obtendran la cantidad de hijos por raza a traves de una distribuion discreta segun los fitness de cada especie
	for (uint i = 0; i < spicies.size(); ++i)
	{
		childrensPerSpicie.push_back(0);
	}
	std::discrete_distribution<uint> obtainSpicie(fitnessVector.begin(), fitnessVector.end());
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


void Life::fillFitnessVector (std::vector <float> & fitnessVector)
{
	fitnessVector.clear();
	for( const auto& spicie : spicies )
	{
		fitnessVector.push_back( spicie->getMeanFitnessOfOldRaces() );
	}
}

void Life::createSpiciesFromOrganismCandidates()
{
	if(spicies.size() >= maxAmountOfSpicies) return ;
	std::uniform_int_distribution<uint> randomSpicie(0, 
							spicies.size() -1);

	uint attemps = 2;
	for (uint i = 0; i < attemps; ++i)
	{
		uint spiciePos =  randomSpicie(*generator);
		auto organism = spicies.at(spiciePos)->getOrganismNewSpiciesCandidate();
		if( organism != nullptr ) // If is diferent than nullptr then one orgm was founded
		{

		}
	}
		
}


uint Life::getCurrentGeneration(){return currentGeneration;}


} // end namespace NEAT