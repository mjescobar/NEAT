#include "Spicies.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

namespace NEAT
{

Spicies::~Spicies(){}

Spicies::Spicies( shared_ptr <Race> founderRace  ): Spicies(SpiciesUserDefinitions(), move(founderRace)){}

Spicies::Spicies( const SpiciesUserDefinitions& userdef, shared_ptr <Race> founderRace  )
{
	maxAmountOfRacesPerSpicie = userdef.maxAmountOfRacesPerSpicie;
	maxYears = userdef.maxYears;
	youngRaces.push_back(move(founderRace));
	generator = make_unique < default_random_engine > (chrono::system_clock::now().time_since_epoch().count());
	years = 0;
	extincted = false;
}

// Only old Races hava to fight for childrens the young races have not.
void Spicies::epoch()
{
	if(years >= maxYears){ extincted = true; return; }
	deleteExtinctedRaces();
	if(youngRaces.size() + oldRaces.size() == 0) {
		extincted = true; 
		return;
	}

	for (uint i = 0; i < oldRaces.size(); ++i)
	{
		oldRaces.at(i)->epoch( );
	}

	for (uint i = 0; i < youngRaces.size(); ++i)
	{
		youngRaces.at(i)->epoch( );
	}

	for (uint i = 0; i < youngRaces.size(); ++i)
	{
		if( youngRaces.at(i)->isYoung() == false  ) // Si la raza ha crecido entonces se tiene que cambiar de vector que lo maneja.
		{
			oldRaces.push_back( move(youngRaces.at(i)) );
			youngRaces.erase(youngRaces.begin()+i);
			i--;
		}
	}

	deleteExtinctedRaces();
	createRacesFromOrganismCandidates();
	years ++;
	deleteExtinctedRaces();
}

void Spicies::newRacesDecendece()
{
	for (uint i = 0; i < youngRaces.size(); ++i)
	{
		youngRaces.at(i)->newRaceDecendece( );
	}
}

shared_ptr <Organism> Spicies::getOrganismNewSpiciesCandidate()
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
			shared_ptr< Organism > result = move( race.newSpicieOrgmCandidate.at(selected) );
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
	uint protectedRacesAmount = max(1u, (uint)lround(0.5 * maxAmountOfRacesPerSpicie));
	if(  protectedRacesAmount < oldRaces.size() )
	{
		float min = oldRaces.at(0)->getFitnessMean();
		int min_position = -1;
		float fitness;
		float eraseWorseRaceRate = 0.5;

		if(rand()/(double)RAND_MAX < eraseWorseRaceRate)
		{
			for (uint i = 0; i < oldRaces.size(); ++i)
			{
				fitness = oldRaces.at(i)->getFitnessMean();
				if( fitness <= min )
				{
					min = fitness;
					min_position = i;
				}
			}	
			if(min_position < 0)
			{
				cerr << "ERROR::Spicies::eliminateWorseRaces:: position of worse specie is impossible. "  << min_position << endl;
				exit(EXIT_FAILURE);
			}
			oldRaces.erase (oldRaces.begin() + min_position);
		}
	}
}

} // end Namespace NEAT
