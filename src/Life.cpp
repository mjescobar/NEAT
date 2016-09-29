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
	std::cerr << "le 1" << std::endl;
	eliminateWorseOrganisms();
	std::cerr << "le 2" << std::endl;
	eliminateWorseRaces();
	std::cerr << "le 3" << std::endl;
	eliminateWorseSpecies();
	std::cerr << "le 4" << std::endl;
	deleteExtinctedSpicies();
	std::cerr << "le 5" << std::endl;
	createDecendence();
	std::cerr << "le 6" << std::endl;
	deleteExtinctedSpicies();
	std::cerr << "le 7" << std::endl;
	createSpiciesFromOrganismCandidates();
	std::cerr << "le 8" << std::endl;
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