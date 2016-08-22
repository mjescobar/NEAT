#ifndef NEAT_SPICIESUSERDEF_HPP
#define NEAT_SPICIESUSERDEF_HPP



#include <string>

namespace NEAT
{

class SpiciesUserDefinitions
{
public:
	SpiciesUserDefinitions();
	SpiciesUserDefinitions(  std::string path  );

	void printInfo();
	
	uint maxAmountOfRacesPerSpicie;	

};

}


#endif