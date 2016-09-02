#ifndef NEAT_RAcE_HPP
#define NEAT_RAcE_HPP

#include <random>
#include <vector>
#include <memory>
#include <chrono>
#include "Organism.hpp"
#include "RaceUserDefinitions.hpp"

#define SPICE_NOT_FOUNDED 1


namespace NEAT
{

class Race
{
public:
	Race(std::unique_ptr <Organism> organism);
	Race( const RaceUserDefinitions& userDef, std::unique_ptr <Organism> organism );
	Race( const Race& other, std::unique_ptr <Organism>  founderOrganism );
	void populate( );
	void epoch( uint childrensAmount ); 
	void epoch(); // for new races
	float getFitnessMean();
	bool belongsAtThisRace(const Organism& orgm ); // Distancia entre organismo y raza
	std::unique_ptr <Race> createNew( std::unique_ptr <Organism> organism );
	bool isExtinct();
	bool isYoung();
	std::vector < std::unique_ptr< Organism > > & getNewOrganisms_ref();
	void printInfo()const;
	
	std::vector <std::unique_ptr<Organism>> newOrganisms;
	std::vector <std::unique_ptr<Organism>> newRaceOrgmCandidate;
	std::vector <std::unique_ptr<Organism>> newSpicieOrgmCandidate;

	std::vector <float> fitnessPerYear;

private:
	Organism& getRandomOrganism_ref();
	void addOrganismCandidateToNewRace( std::unique_ptr <Organism> candidate );
	void addOrganismCandidateToNewSpicies( std::unique_ptr <Organism> candidate  );
	void updateTotalFitness();
	void fillFitnessVector (std::vector <float> & fitnessVector);
	void populateFromCurrentsOrganisms( uint amountOfChildrens );
	void organismsGrowUp();
	void createDecendence(const uint amountOfChildrens );

	std::vector <std::unique_ptr<Organism>> oldOrganisms;

	bool extincted;
	uint years;
	uint maxYears;
	uint maxYearsYoungRace;
	float maximumRaceDistance;
	float raceTotalFitness;
	bool youngRace;
	uint youngRaceMaxPopulation;
	uint maxStackNewRaceCandidates; 
	uint maxStackNewSpiciesCandidates; 
	std::unique_ptr < std::default_random_engine > generator;
};

}

#endif