#include "Spicies.hpp"
#include <algorithm>
#include <iostream>
namespace NEAT
{

Spicies::Spicies( std::unique_ptr <Race> founderRace  ): Spicies(SpiciesUserDefinitions(), std::move(founderRace)){}

Spicies::Spicies( const SpiciesUserDefinitions& userdef, std::unique_ptr <Race> founderRace  )
{
	maxAmountOfRacesPerSpicie = userdef.maxAmountOfRacesPerSpicie;
	maxYears = userdef.maxYears;
	youngRaces.push_back(std::move(founderRace));
	generator = std::make_unique < std::default_random_engine > (std::chrono::system_clock::now().time_since_epoch().count());
	years = 0;
	extincted = false;
}

// Only old Races hava to fight for childrens the young races have not.
void Spicies::epoch( const uint childrenAmount )
{
	if(childrenAmount == 0 && oldRaces.size() >= 1){ extincted = true; return; }
	if(years >= maxYears){ extincted = true; return; }
	createDecendence(childrenAmount);
	deleteExtinctedRaces();
	if(youngRaces.size() + oldRaces.size() == 0) {extincted = true; return;}
	createRacesFromOrganismCandidates();
	years ++;
}

std::unique_ptr <Organism> Spicies::getOrganismNewSpiciesCandidate()
{

	uint attempts = 3;
	for (uint i = 0; i < attempts; ++i)
	{
		auto& race = getRandomRace_ref();
		if(race.newSpicieOrgmCandidate.size() >= 1)
		{
			std::uniform_int_distribution<uint> randomOrganism(0, 
							race.newSpicieOrgmCandidate.size() -1);
			auto selected = randomOrganism(*generator);
			std::unique_ptr< Organism > result = std::move( race.newSpicieOrgmCandidate.at(selected) );
			race.newSpicieOrgmCandidate.erase( race.newSpicieOrgmCandidate.begin() + selected );
			if(!detectRepeatedInnovation(*result))
			{
				return std::move (result);
			}
		}
	}
	return nullptr; // in case was not posible to found a organism 
}

bool Spicies::isExtinct()
{
	extincted = extincted || (oldRaces.size() == 0 && youngRaces.size() == 0);
	return extincted; // Si no quedan razas entonces esta extinta esta especie.
}

float Spicies::getMeanFitnessOfOldRaces()
{
	if(oldRaces.size() == 0) return 0.f;
	auto mean = 0.f;
	for( const auto& race : oldRaces )
	{
		mean += race->getFitnessMean();
	}
	mean = mean/(float)oldRaces.size();
	return std::move(mean);
}

void Spicies::printInfo() const
{
	std::cout <<
	"years: " << years <<  std::endl <<
	"oldRaces size: " << oldRaces.size() << std::endl <<
	"young races size: " << youngRaces.size() << std::endl <<
	"maxAmountOfRacesPerSpicie: " << maxAmountOfRacesPerSpicie << std::endl <<
	"INNOV MAPs: " << std::endl;
	for( auto& pair : innovMsgMap )
	{
		std::cout << "Key: " << pair.first << "  Value: " << pair.second << std::endl;
	}
}

} // end Namespace NEAT