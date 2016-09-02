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
{
	getStatisticsOfCurrentGeneration();
	eliminateWorseOrganisms();
	deleteExtinctedSpicies();
	createDecendence();
	deleteExtinctedSpicies();
	createSpiciesFromOrganismCandidates();
	currentGeneration++;
}

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
	for (uint i = 0; i < spicies.size(); ++i)
	{
		childrensPerSpicie.push_back(0);
	}
	discrete_distribution<uint> obtainSpicie(fitnessVector.begin(), fitnessVector.end());
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

uint Life::getCurrentGeneration(){return currentGeneration;}

void Life::printInfo()
{
	cout << 
	"Spicies amount: " << spicies.size() << endl <<
	"maxAmountOfSpicies: " << maxAmountOfSpicies << endl <<
	"maxAmountOrganismInAllOldRaces: " << maxAmountOrganismInAllOldRaces << endl;
}

void Life::getStatisticsOfCurrentGeneration()
{
	mean = 0.f;
	variance = 0.f;
	min = 10000.f;
	max = 0.f;
	uint OrgmCounter = 0; 
	float fitness = 0.f;
	for( const auto& specie : spicies )
	{
		for(const auto& race: specie->youngRaces)
		{
			for(const auto& orgm : race->newOrganisms)
			{
				OrgmCounter++;
				fitness = orgm->getFitness();
				mean += fitness;
				if(min > fitness){min = fitness;}
				if(max < fitness){max = fitness;}
			}
		}
		for(const auto& race: specie->oldRaces)
		{
			for(const auto& orgm : race->newOrganisms)
			{
				OrgmCounter++;
				fitness = orgm->getFitness();
				mean += fitness;
				if(min > fitness){min = fitness;}
				if(max < fitness){max = fitness;}
			}
		}
	}

	mean = (1/(double)OrgmCounter)*mean;

	for( const auto& specie : spicies )
	{
		for(const auto& race: specie->youngRaces)
		{
			for(const auto& orgm : race->newOrganisms)
			{
				variance += (orgm->getFitness()-mean)*(orgm->getFitness()-mean);
			}
		}
		for(const auto& race: specie->oldRaces)
		{
			for(const auto& orgm : race->newOrganisms)
			{
				variance += (orgm->getFitness()-mean)*(orgm->getFitness()-mean);
			}
		}
	}
	variance = (1/(double)OrgmCounter)*variance;

	std::cout << "Mean: " << mean << "\tVariance: " << variance  << "\tmin: "<< min << "\tmax: "<< max<< std::endl;
}

void Life::eliminateWorseOrganisms() // the means is the limit
{
	if(min == max){return;} // caso raro
	for( const auto& specie : spicies )
	{
		for(const auto& race: specie->youngRaces)
		{
			race->newOrganisms.erase(  remove_if(race->newOrganisms.begin(), race->newOrganisms.end(),
		    [&](unique_ptr<Organism>& orgm)->bool { return orgm->getFitness() <= mean; }),
			race->newOrganisms.end());
		}
		for(const auto& race: specie->oldRaces)
		{
			race->newOrganisms.erase(  remove_if(race->newOrganisms.begin(), race->newOrganisms.end(),
		    [&](unique_ptr<Organism>& orgm)->bool { return orgm->getFitness() <= mean; }),
			race->newOrganisms.end());
		}
	}
}


} // end namespace NEAT