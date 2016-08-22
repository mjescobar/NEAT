#ifndef NEAT_LIFEUSERDEF_HPP
#define NEAT_LIFEUSERDEF_HPP



#include <string>

namespace NEAT
{

class LifeUserDefinitions
{
public:
	LifeUserDefinitions();
	LifeUserDefinitions(  std::string path  );

	void printInfo();
	float maxAmountOfSpicies;
	uint maxAmountOrganismInAllOldRaces;
};

}


#endif