#ifndef NEAT_ORGANISM_HPP
#define NEAT_ORGANISM_HPP

#include "ANN.hpp"
#include "OrganismUserDefinitions.hpp"
namespace NEAT
{

class Organism
{
public:
	Organism( std::unique_ptr <ANN> annSeed );
	Organism( const OrganismUserDefinitions& userDef, std::unique_ptr <ANN> annSeed );
	Organism( const Organism& other );
	bool surviveNewEpoch(); 
	bool getIsNewSpicie() const;
	std::unique_ptr < Organism > crossOver( const Organism& other ) const;
	std::unique_ptr <Organism> createSimilar() const;
	void setFitness( const float fitness );
	float getFitness () const;
	float getDistance(const Organism& other ) const;

	std::unique_ptr <ANN> ann; // Para un uso mas entendible se deja esta variable en publico.
private:
	uint years;
	uint lifeExpectative;
	float fitness;
};

}
#endif