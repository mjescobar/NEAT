#include "NEATStatistics.hpp"
#include <iostream>

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
			auto amount = race->newOrganisms.size();
			race_s->addOrganism(amount);
			spicie_s->addYoungRace(move(race_s));
		}
		for( auto& race: spicie->oldRaces)
		{
			auto race_s = make_unique <RaceStatistics>();
			auto amount = race->newOrganisms.size();
			race_s->addOrganism(amount);
			spicie_s->addOldRace(move(race_s));
		}
		generation_s->addSpecie(move(spicie_s));
	}
	generationStatistics.push_back( move(generation_s) );
}

void NEATStatistics::printStatisticsToFile(std::string path) const
{

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
				cout << "Young Race: " << k  << "\tOrganisms amount: " << generationStatistics.at(i)->spiciesStatistics.at(j)->youngRacesStatistics.at(k)->organismsAmount << endl;
			}

			for (unsigned int k = 0; k < generationStatistics.at(i)->spiciesStatistics.at(j)->oldRacesStatistics.size(); ++k)
			{
				cout << "Old Race: " << k  << "\tOrganisms amount: " << generationStatistics.at(i)->spiciesStatistics.at(j)->oldRacesStatistics.at(k)->organismsAmount << endl;
			}
		}
		cout << "========================= End Generation " << i << endl; 
	}
}
