#include "Spicies.hpp"
#include <algorithm>
namespace NEAT
{

Spicies::Spicies( std::unique_ptr <Race> founderRace  ): Spicies(SpiciesUserDefinitions(), std::move(founderRace)){}

Spicies::Spicies( const SpiciesUserDefinitions& userdef, std::unique_ptr <Race> founderRace  )
{
	maxAmountOfRacesPerSpicie = userdef.maxAmountOfRacesPerSpicie;
	youngRaces.push_back(std::move(founderRace));
	generator = std::make_unique < std::default_random_engine > (std::chrono::system_clock::now().time_since_epoch().count());
}

// Only old Races hava to fight for childrens the young races have not.
void Spicies::epoch( const uint childrenAmount )
{
	createDecendence(childrenAmount);
	createRacesFromOrganismCandidates();
	deleteExtinctedRaces();
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
			return std::move (result);
		}
	}
	return nullptr; // in case was not posible to found a organism 
}

bool Spicies::isExtincted()
{
	oldRaces.erase(  std::remove_if(oldRaces.begin(), oldRaces.end(),
        [](std::unique_ptr<Race>& race)->bool { return race->isExtincted(); }),
    	oldRaces.end());
	youngRaces.erase(  std::remove_if(youngRaces.begin(), youngRaces.end(),
        [](std::unique_ptr<Race>& race)->bool { return race->isExtincted(); }),
    	youngRaces.end());
	return oldRaces.size() == 0 && youngRaces.size() == 0; // Si no quedan razas entonces esta extinta esta especie.
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

}