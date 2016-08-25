#ifndef NEAT_LIFE_HPP
#define NEAT_LIFE_HPP

#include "LifeUserDefinitions.hpp"
#include "Neuron.hpp"
#include "SynapticWeight.hpp"
#include "Spicies.hpp"
#include "Race.hpp"
#include "Organism.hpp"
#include <vector>
#include <memory>
#include <random>

namespace NEAT
{


class Life
{
public:
	Life( std::unique_ptr<ANN> annSeed );
	void epoch();
	uint getCurrentGeneration();
private:
	void deleteExtinctedSpicies();
	void createSpiciesFromOrgmCandidates();
	void createDecendence();
	void fillFitnessVector (std::vector <float> & fitnessVector);
	void createSpiciesFromOrganismCandidates();

	uint maxAmountOfSpicies;
	uint maxAmountOrganismInAllOldRaces;
	uint currentGeneration;
	std::vector < std::unique_ptr<Spicies> > spicies;
	std::unique_ptr < std::default_random_engine > generator;

};


}// end namspace NEAT


#endif