#ifndef NEATSTATISTICS_HPP
#define NEATSTATISTICS_HPP

#include <vector>
#include <string>
#include "Life.hpp"
#include <cmath>
namespace NEAT
{

class OrganismStatistics
{
public:
	OrganismStatistics(float fitness)
	{
		this->fitness = fitness;
	}
	float fitness;
};

class RaceStatistics
{
public:
	RaceStatistics()
	{
	}
	void addOrganism(std::unique_ptr<OrganismStatistics> orgm_s)
	{
		organismsStatistics.push_back(move(orgm_s));
	}
	std::vector< std::shared_ptr<OrganismStatistics> > organismsStatistics;

};

class SpicieStatistics
{
public:
	void addYoungRace(std::shared_ptr <RaceStatistics> youngRace_s)
	{
		youngRacesStatistics.push_back(move(youngRace_s));

	}
	void addOldRace(std::shared_ptr <RaceStatistics> oldRace_s)
	{
		oldRacesStatistics.push_back(move(oldRace_s));
	}
	std::vector< std::shared_ptr<RaceStatistics> > oldRacesStatistics;
	std::vector< std::shared_ptr<RaceStatistics> > youngRacesStatistics;
};


class GenerationsStatistics
{
public:
	std::vector< std::shared_ptr<SpicieStatistics> > spiciesStatistics;
	void addSpecie( std::shared_ptr <SpicieStatistics> specie_s)
	{
		spiciesStatistics.push_back( move(specie_s) );
	}
	float getAverageFitness() const 
	{
		float allOrganismFitness = 0.0f;
		float onlySurvivorsAverageFitness = 0.0f;
		uint counter = 0;
		for (auto& specie_s : spiciesStatistics)
		{
			for (auto& oldRace_s : specie_s->oldRacesStatistics)
			{
				for (auto& orgm_s : oldRace_s->organismsStatistics)
				{
					counter++;
					allOrganismFitness += orgm_s->fitness;
				}	
			}
		}
		if (counter > 0)
		{
			allOrganismFitness = allOrganismFitness / (float)counter;
			uint counter_2 = 0;
			for (auto& specie_s : spiciesStatistics)
			{
				for (auto& oldRace_s : specie_s->oldRacesStatistics)
				{
					for (auto& orgm_s : oldRace_s->organismsStatistics)
					{
						if(orgm_s->fitness > allOrganismFitness)
						{
							counter_2++;
							onlySurvivorsAverageFitness += orgm_s->fitness;
						}
					}	
				}
			}
			if(counter_2 > 0)
			{			
				onlySurvivorsAverageFitness = onlySurvivorsAverageFitness / (float)counter_2;
				return onlySurvivorsAverageFitness;
			}
		}
		return 0.0f;
	}

	float getChampionFitness()
	{
		float championFitness = 0.f;
		for (auto& specie_s : spiciesStatistics)
		{
			for (auto& oldRace_s : specie_s->oldRacesStatistics)
			{
				for (auto& orgm_s : oldRace_s->organismsStatistics)
				{
					championFitness = std::max(championFitness, orgm_s->fitness);
				}	
			}
		}
		return championFitness;
	}
};

class NEATStatistics
{

public:
	NEATStatistics();
	void takeInformationOfTheCurrentGeneration(const Life& life );
	void printStatisticsToFile(std::string path) const;
	void printInfo() const;
	float getAverageFitnessOfGenerationAt(const uint place) const;
	void getAverageFitnessOfAllGenerationInFile(const std::string path) const;
	void getChampionFitnessOfAllGenerationInFile(const std::string path) const;
private:
	std::vector< std::shared_ptr<GenerationsStatistics> > generationStatistics;
};

} // END NAMESPACE NEAT
#endif
