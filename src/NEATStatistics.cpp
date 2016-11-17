#include "NEATStatistics.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace NEAT;

NEATStatistics::NEATStatistics()
{

}

void NEATStatistics::takeInformationOfTheCurrentGeneration(const Life& life )
{
	auto  generation_s = make_unique <GenerationsStatistics> ();

	for ( auto& spicie : life.spicies)
	{
		auto spicie_s = make_unique <SpicieStatistics>();

		for( auto& race: spicie->youngRaces)
		{
			auto race_s = make_unique <RaceStatistics>();
			for(auto& orgm: race->newOrganisms )
			{
				auto orgm_s = make_unique<OrganismStatistics>(orgm->getFitness());
				race_s->addOrganism(move(orgm_s));
			}
			spicie_s->addYoungRace(move(race_s));
		}
		for( auto& race: spicie->oldRaces)
		{
			auto race_s = make_unique <RaceStatistics>();
			for(auto& orgm: race->newOrganisms)
			{
				auto orgm_s = make_unique<OrganismStatistics>(orgm->getFitness());
				race_s->addOrganism(move(orgm_s));
			}
			spicie_s->addOldRace(move(race_s));
		}
		generation_s->addSpecie(move(spicie_s));
	}
	generationStatistics.push_back( move(generation_s) );
}

void NEATStatistics::printStatisticsToFile(std::string path) const
{
	ofstream file;
	file.open(path);
	file << "Complete tree of generation, species, races and organisms: " << endl;
	for (unsigned int i = 0; i < generationStatistics.size(); ++i)
	{
		auto spiciesAmount = generationStatistics.at(i)->spiciesStatistics.size(); 
		file << "Generation: " << i << "\t Spicies amount: " << spiciesAmount << endl;
		for (unsigned int j = 0; j < spiciesAmount; ++j)
		{
			file << "Specie: " << j << "\tYoungRaces: " << generationStatistics.at(i)->spiciesStatistics.at(j)->youngRacesStatistics.size() << "\tOldRaces: " << generationStatistics.at(i)->spiciesStatistics.at(j)->oldRacesStatistics.size() << endl;
			for (unsigned int k = 0; k < generationStatistics.at(i)->spiciesStatistics.at(j)->youngRacesStatistics.size(); ++k)
			{
				file << "Young Race: " << k  << "\tOrganisms amount: " << generationStatistics.at(i)->spiciesStatistics.at(j)->youngRacesStatistics.at(k)->organismsStatistics.size() << endl;
			}

			for (unsigned int k = 0; k < generationStatistics.at(i)->spiciesStatistics.at(j)->oldRacesStatistics.size(); ++k)
			{
				file << "Old Race: " << k  << "\tOrganisms amount: " << generationStatistics.at(i)->spiciesStatistics.at(j)->oldRacesStatistics.at(k)->organismsStatistics.size() << endl;
			}
		}
		file << "========================= End Generation " << i << endl; 
	}
	file.close();
}

void NEATStatistics::printInfo() const
{
	cout << "Complete tree of generation, species, races and organisms: " << endl;
	for (unsigned int i = 0; i < generationStatistics.size(); ++i)
	{
		auto spiciesAmount = generationStatistics.at(i)->spiciesStatistics.size(); 
		cout << "Generation: " << i << "\t Spicies amount: " << spiciesAmount << endl;
		for (unsigned int j = 0; j < spiciesAmount; ++j)
		{
			cout << "Specie: " << j << "\tYoungRaces: " << generationStatistics.at(i)->spiciesStatistics.at(j)->youngRacesStatistics.size() << "\tOldRaces: " << generationStatistics.at(i)->spiciesStatistics.at(j)->oldRacesStatistics.size() << endl;
			for (unsigned int k = 0; k < generationStatistics.at(i)->spiciesStatistics.at(j)->youngRacesStatistics.size(); ++k)
			{
				cout << "Young Race: " << k  << "\tOrganisms amount: " << generationStatistics.at(i)->spiciesStatistics.at(j)->youngRacesStatistics.at(k)->organismsStatistics.size() << endl;
			}

			for (unsigned int k = 0; k < generationStatistics.at(i)->spiciesStatistics.at(j)->oldRacesStatistics.size(); ++k)
			{
				cout << "Old Race: " << k  << "\tOrganisms amount: " << generationStatistics.at(i)->spiciesStatistics.at(j)->oldRacesStatistics.at(k)->organismsStatistics.size() << endl;
			}
		}
		cout << "========================= End Generation " << i << endl; 
	}
}


float NEATStatistics::getAverageFitnessOfGenerationAt(const uint place) const
{
	GenerationsStatistics * generation = generationStatistics.at(place).get();
	return generation->getAverageFitness();
}


void NEATStatistics::getAverageFitnessOfAllGenerationInFile(const string path) const
{
	ofstream file;
	file.open(path);

	for(auto& generation : generationStatistics)
	{
		file << generation->getAverageFitness() << endl;
	}

	file.close();
}


void NEATStatistics::getAverageFitnessAndVarianceOfAllGenerationInFile(const string path) const
{
	ofstream file;
	file.open(path);

	for(auto& generation : generationStatistics)
	{
		file << generation->getAverageFitness() << "\t" << generation->getVariance()  << endl;
	}

	file.close();
}


void NEATStatistics::getChampionFitnessOfAllGenerationInFile(const string path) const
{
	ofstream file;
	file.open(path);

	for(auto& generation : generationStatistics)
	{
		file << generation->getChampionFitness() << endl;
	}

	file.close();
}

