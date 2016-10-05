#ifndef NEAT_RAcEUSERDEF_HPP
#define NEAT_RAcEUSERDEF_HPP



#include <string>

namespace NEAT
{

class RaceUserDefinitions
{
public:
	virtual ~RaceUserDefinitions();
	RaceUserDefinitions();
	RaceUserDefinitions(  std::string path  );

	void printInfo();
	uint maxYears;
	uint maxYearsYoungRace;
	float maximumRaceDistance;
	uint youngRaceMaxPopulation;
	uint maxStackNewRaceCandidates; 
	uint maxStackNewSpiciesCandidates; 

};

}


#endif