#include "Spicies.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

namespace NEAT
{

Spicies::~Spicies()
{
	
}

Spicies::Spicies( unique_ptr <Race> founderRace  ): Spicies(SpiciesUserDefinitions(), move(founderRace)){}

Spicies::Spicies( const SpiciesUserDefinitions& userdef, unique_ptr <Race> founderRace  )
{
	maxAmountOfRacesPerSpicie = userdef.maxAmountOfRacesPerSpicie;
	maxYears = userdef.maxYears;
	youngRaces.push_back(move(founderRace));
	generator = make_unique < default_random_engine > (chrono::system_clock::now().time_since_epoch().count());
	years = 0;
	extincted = false;
}

// Only old Races hava to fight for childrens the young races have not.
void Spicies::epoch( const uint childrenAmount )
{
	if(childrenAmount == 0 && oldRaces.size() >= 1){ 
		extincted = true; 
		return; 
	}
	if(years >= maxYears){ extincted = true; return; }
	createDecendence(childrenAmount);
	deleteExtinctedRaces();
	if(youngRaces.size() + oldRaces.size() == 0) {
		extincted = true; 
		return;
	}
	createRacesFromOrganismCandidates();
	years ++;
}

unique_ptr <Organism> Spicies::getOrganismNewSpiciesCandidate()
{

	uint attempts = 3;
	for (uint i = 0; i < attempts; ++i)
	{
		auto& race = getRandomRace_ref();
		if(race.newSpicieOrgmCandidate.size() >= 1)
		{
			uniform_int_distribution<uint> randomOrganism(0, 
							race.newSpicieOrgmCandidate.size() -1);
			auto selected = randomOrganism(*generator);
			unique_ptr< Organism > result = move( race.newSpicieOrgmCandidate.at(selected) );
			race.newSpicieOrgmCandidate.erase( race.newSpicieOrgmCandidate.begin() + selected );
			if(!detectRepeatedInnovation(*result))
			{
				return move (result);
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
	return move(mean);
}

void Spicies::printInfo() const
{
	cout <<
	"years: " << years <<  endl <<
	"oldRaces size: " << oldRaces.size() << endl <<
	"young races size: " << youngRaces.size() << endl <<
	"maxAmountOfRacesPerSpicie: " << maxAmountOfRacesPerSpicie << endl <<
	"INNOV MAPs: " << endl;
	for( auto& pair : innovMsgMap )
	{
		cout << "Key: " << pair.first << "  Value: " << pair.second << endl;
	}
}

void Spicies::eliminateWorseOrganisms()
{
	for(auto& race : oldRaces)
	{
		race->eliminateWorseOrganisms();
	}
	for(auto& race : youngRaces)
	{
		race->eliminateWorseOrganisms();
	}
}

void Spicies::eliminateWorseRaces()
{
	float racesMean = 0.f; 
	float min = -1.f;
	float max = 0.f;
	float fitness;
	for(auto& race : oldRaces)
	{
		fitness = race->getFitnessMean();
		if( fitness > min )
		{
			min = fitness;
		}
		if(fitness < max)
		{
			max = fitness;
		}
		racesMean += fitness;
	}
	racesMean = racesMean/(float)oldRaces.size();

	if(min == max) {return;} // some rare case;

	// ToDo: Mejorar el modelo tal que, por ejemplo, la probabilidad de supervivencia sea una gaussiana con la misma media y promedio que las especies (solo para las especies de fitness menor que la media)
	oldRaces.erase(  remove_if(oldRaces.begin(), oldRaces.end(),
    [&](unique_ptr<Race>& race)->bool 
    { 
		if( race->getFitnessMean() < racesMean )
		{
			return true;
		}
		return false;
     }),oldRaces.end());
}

} // end Namespace NEAT