#ifndef NEATSTATISTICS_HPP
#define NEATSTATISTICS_HPP

#include <vector>
#include <string>
#include "Life.hpp"
namespace NEAT
{

class RaceStatistics
{
public:
	RaceStatistics(){
		organismsAmount = 0;
	}
	void addOrganism()
	{
		organismsAmount ++;
	}
	void addOrganism(unsigned int amount)
	{
		organismsAmount += amount;
	}
	unsigned int organismsAmount;
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
};

class NEATStatistics
{

public:
	NEATStatistics();
	void takeInformationOfTheCurrentGeneration(const Life& life );
	void printStatisticsToFile(std::string path) const;
	void printInfo() const;
private:
	std::vector< std::shared_ptr<GenerationsStatistics> > generationStatistics;
};

} // END NAMESPACE NEAT
#endif
