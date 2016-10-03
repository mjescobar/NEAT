#include "Life.hpp"

#include <chrono>
#include <iostream>
#include <algorithm>

using namespace std;

namespace NEAT
{
Life::Life(unique_ptr<ANN> annSeed )
{
	auto lud = make_unique <LifeUserDefinitions>();
	// Se crea la primera especie.
	auto firstOrganism = make_unique <Organism>(  move(annSeed) );
	auto firstRace = make_unique <Race>( move(firstOrganism) );
	auto firstSpicie = make_unique<Spicies>( move(firstRace));
	spicies.push_back( move(firstSpicie) );
	maxAmountOfSpicies = lud->maxAmountOfSpicies;
	maxAmountOrganismInAllOldRaces = lud->maxAmountOrganismInAllOldRaces;
	generator = make_unique < default_random_engine > (chrono::system_clock::now().time_since_epoch().count());
}

void Life::epoch()
{ // private function method of Life class in LifeTooks.cpp.
	eliminateWorseOrganisms();
	eliminateWorseRaces();
	eliminateWorseSpecies();
	deleteExtinctedSpicies();
	createDecendence();
	deleteExtinctedSpicies();
	createSpiciesFromOrganismCandidates();
	currentGeneration++;
}

uint Life::getCurrentGeneration()
{
	return currentGeneration;
}

void Life::printInfo()
{
	cout << 
	"Spicies amount: " << spicies.size() << endl <<
	"maxAmountOfSpicies: " << maxAmountOfSpicies << endl <<
	"maxAmountOrganismInAllOldRaces: " << maxAmountOrganismInAllOldRaces << endl;
}

} // end namespace NEAT