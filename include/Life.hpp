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
	void printInfo();
	
	std::vector < std::unique_ptr<Spicies> > spicies;
private:
	void deleteExtinctedSpicies();
	void createSpiciesFromOrgmCandidates();
	void createDecendence();
	void fillFitnessVector (std::vector <float> & fitnessVector);
	void createSpiciesFromOrganismCandidates();
	void getStatisticsOfCurrentGeneration();
	void eliminateWorseOrganisms();
	uint maxAmountOfSpicies;
	uint maxAmountOrganismInAllOldRaces;
	uint currentGeneration;
	std::unique_ptr < std::default_random_engine > generator;
	float mean;
	float variance;
	float min;
	float max;
};


}// end namspace NEAT


#endif