#include "Race.hpp"
#include <iostream>
#include <algorithm>

namespace NEAT
{
Race::Race(std::unique_ptr <Organism>  founderOrganism ): Race( RaceUserDefinitions(), std::move(founderOrganism) ){}

Race::Race( const RaceUserDefinitions& userDef, std::unique_ptr <Organism>  founderOrganism )
{
	years = 0;
	maxYearsYoungRace = userDef.maxYearsYoungRace;
	maximumRaceDistance = userDef.maximumRaceDistance;
	youngRaceMaxPopulation = userDef.youngRaceMaxPopulation;
	maxStackNewRaceCandidates = userDef.maxStackNewRaceCandidates;
	maxStackNewSpiciesCandidates = userDef.maxStackNewSpiciesCandidates;
	raceTotalFitness = 0.f;
	youngRace = true;
	newOrganisms.push_back(std::move(founderOrganism));
	generator = std::make_unique < std::default_random_engine > (std::chrono::system_clock::now().time_since_epoch().count() + (uint)rand());

	// Se prodece a llenar la raza con youngRaceMaxPopulation organismos.
	populateFromCurrentsOrganisms( youngRaceMaxPopulation - newOrganisms.size() );
}

Race::Race( const Race& other, std::unique_ptr <Organism>  founderOrganism )
{
	years = 0;
	maxYearsYoungRace = other.maxYearsYoungRace;
	maximumRaceDistance = other.maximumRaceDistance;
	youngRaceMaxPopulation = other.youngRaceMaxPopulation;
	maxStackNewRaceCandidates = other.maxStackNewRaceCandidates;
	maxStackNewSpiciesCandidates = other.maxStackNewSpiciesCandidates;
	raceTotalFitness = 0.f;
	youngRace = true;
	newOrganisms.push_back(std::move(founderOrganism));
	generator = std::make_unique < std::default_random_engine > (std::chrono::system_clock::now().time_since_epoch().count() + (uint)rand());

	// Se prodece a llenar la raza con youngRaceMaxPopulation organismos.
	populateFromCurrentsOrganisms( youngRaceMaxPopulation - newOrganisms.size() );
}

void Race::epoch(){
	epoch(youngRaceMaxPopulation);
}

void Race::epoch( uint amountOfChildrens ) // no se especifica en caso de ser young
{	
	// Primero se pasa de epoca a cada organismo de la raza (pudiendo morir algunos de ellos en el camino)
	organismsGrowUp(); // Todos los nuevos organismos son pasados a viejos organismos
						// en este momento newOrganism esta vacio y sera vuelto a llenar al final
	if(oldOrganisms.size() == 0){return;}
	if( ++years >= maxYearsYoungRace ){	youngRace = false; }
	// los que quedan tienen derecho a aparearse, tomando en cuenta la posibilidad que exista tan solo un organismo en la raza es que se chequea tal.
	if(oldOrganisms.size()  == 1) // No hay suficientes organismos como para realizar cruzamientos.
	{
		populateFromCurrentsOrganisms (amountOfChildrens); //todos los hijos son similares a su unico padre
		return ;
	}		
	// En otro caso proceden los cruzamientos.
	createDecendence(amountOfChildrens);
}

float Race::getFitnessMean()
{
	this->updateTotalFitness();
	return raceTotalFitness/(float)(oldOrganisms.size() + newOrganisms.size());
}

bool Race::belongsAtThisRace(const Organism& orgm ) 
{
	auto& randOrgm = getRandomOrganism_ref();
	return randOrgm.getDistance(orgm) < maximumRaceDistance;
}

std::unique_ptr <Race> Race::createNew( std::unique_ptr <Organism> organism )
{
	return std::make_unique<Race>( *this, std::move(organism) );
}

bool Race::isExtincted()
{
	return ( ( newOrganisms.size() + oldOrganisms.size() == 0 )? true : false );
}

bool Race::isYoung()
{
	return youngRace;
}

std::vector < std::unique_ptr< Organism> > & Race::getNewOrganisms_ref()
{
	return newOrganisms;
}

void Race::printInfo()const
{
	std::cout << "RACE USER DEF: " << std::endl
	<< "years: " << years << std::endl
	<< "maxYearsYoungRace: " << maxYearsYoungRace << std::endl
	<< "maximumRaceDistance: " << maximumRaceDistance << std::endl
	<< "raceTotalFitness: " << raceTotalFitness << std::endl
	<< "youngRace: " << youngRace << std::endl
	<< "youngRaceMaxPopulation: " << youngRaceMaxPopulation << std::endl
	<< "maxStackNewRaceCandidates: " << maxStackNewRaceCandidates << std::endl
	<< "maxStackNewSpiciesCandidates: " << maxStackNewSpiciesCandidates << std::endl
	<< "Race structure: " << std::endl
	<< "newOrganisms size:" << newOrganisms.size() << std::endl 
	<< "oldOrganisms size:" << oldOrganisms.size() << std::endl
	<< "newRaceOrgmCandidate size:" << newRaceOrgmCandidate.size() << std::endl
	<< "newSpicieOrgmCandidate size: " << newSpicieOrgmCandidate.size() << std::endl; 
}

}