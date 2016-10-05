#ifndef NEAT_SPICIESUSERDEF_HPP
#define NEAT_SPICIESUSERDEF_HPP



#include <string>

namespace NEAT
{

class SpiciesUserDefinitions
{
public:
	virtual ~SpiciesUserDefinitions();
	SpiciesUserDefinitions();
	SpiciesUserDefinitions(  std::string path  );

	void printInfo();
	
	uint maxAmountOfRacesPerSpicie;	
	uint maxYears;
};

}


#endif