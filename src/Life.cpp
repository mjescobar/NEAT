#include "Life.hpp"

#include <chrono>
#include <iostream>
#include <algorithm>

using namespace std;

namespace NEAT
{
	
Life::~Life()
{
}

Life::Life(shared_ptr<ANN> annSeed ) : 
Life(make_unique<Spicies>( move(make_unique <Race>( move(make_unique <Organism>(  move(annSeed) )) ))), make_unique <LifeUserDefinitions>()) 
{
}

Life::Life( shared_ptr<Spicies> specie, shared_ptr<LifeUserDefinitions> lud)
{
	spicies.push_back( move(specie) );
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
	for (uint i = 0; i < spicies.size(); ++i)
	{
		spicies.at(i)->epoch();
	}
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
