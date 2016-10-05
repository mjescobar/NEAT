#ifndef NEAT_ORGANISMUSERDEF_HPP
#define NEAT_ORGANISMUSERDEF_HPP



#include <string>

namespace NEAT
{

class OrganismUserDefinitions
{
public:
	OrganismUserDefinitions();
	virtual ~OrganismUserDefinitions();
	OrganismUserDefinitions(  std::string path  );

	void printInfo();
	float lifeExpectative;
	

};

}


#endif