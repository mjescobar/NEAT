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
	virtual ~Life();
	Life( std::shared_ptr<Spicies> speci, std::shared_ptr<LifeUserDefinitions> lud); 
	Life( std::shared_ptr<ANN> annSeed );
	void epoch();
	uint getCurrentGeneration();
	void printInfo();
	
	std::vector < std::shared_ptr<Spicies> > spicies;
	std::shared_ptr<Organism> getOrganism(int i);
	int getSizeOrganism();
private:
	void deleteExtinctedSpicies();
	void createSpiciesFromOrgmCandidates();
	void createDecendence();
	void fillFitnessVector (std::vector <float> & fitnessVector);
	void createSpiciesFromOrganismCandidates();
	//void getStatisticsOfCurrentGeneration();
	void eliminateWorseSpecies();
	void eliminateWorseRaces();
	void eliminateWorseOrganisms();
	uint maxAmountOfSpicies;
	uint maxAmountOrganismInAllOldRaces;
	uint currentGeneration;
	std::shared_ptr < std::default_random_engine > generator;
	float mean;
	float variance;
	float min;
	float max;
};


}// end namspace NEAT


#endif
