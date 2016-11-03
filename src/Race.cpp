#include "Race.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace NEAT
{

Race::~Race()
{
}

Race::Race(shared_ptr <Organism>  founderOrganism ): Race( RaceUserDefinitions(), move(founderOrganism) ){}

Race::Race( const RaceUserDefinitions& userDef, shared_ptr <Organism>  founderOrganism )
{
	years = 0;
	maxYearsYoungRace = userDef.maxYearsYoungRace;
	maximumRaceDistance = userDef.maximumRaceDistance;
	youngRaceMaxPopulation = userDef.youngRaceMaxPopulation;
	maxStackNewRaceCandidates = userDef.maxStackNewRaceCandidates;
	maxStackNewSpiciesCandidates = userDef.maxStackNewSpiciesCandidates;
	maxYears = userDef.maxYears;
	raceTotalFitness = 0.f;
	youngRace = true;
	newOrganisms.push_back(move(founderOrganism));
	generator = make_unique < default_random_engine > (chrono::system_clock::now().time_since_epoch().count() + (uint)rand());
	// Se prodece a llenar la raza con youngRaceMaxPopulation organismos.
	populateFromCurrentsOrganisms( youngRaceMaxPopulation - newOrganisms.size() );
	extincted = false;
}

Race::Race( const Race& other, shared_ptr <Organism>  founderOrganism )
{
	years = 0;
	maxYearsYoungRace = other.maxYearsYoungRace;
	maximumRaceDistance = other.maximumRaceDistance;
	youngRaceMaxPopulation = other.youngRaceMaxPopulation;
	maxStackNewRaceCandidates = other.maxStackNewRaceCandidates;
	maxStackNewSpiciesCandidates = other.maxStackNewSpiciesCandidates;
	maxYears = other.maxYears;
	raceTotalFitness = 0.f;
	youngRace = true;
	newOrganisms.push_back(move(founderOrganism));
	generator = make_unique < default_random_engine > (chrono::system_clock::now().time_since_epoch().count() + (uint)rand());

	// Se prodece a llenar la raza con youngRaceMaxPopulation organismos.
	populateFromCurrentsOrganisms( youngRaceMaxPopulation - newOrganisms.size() );
	extincted = false;
}


void Race::epoch( ) // no se especifica en caso de ser young
{	
	if(extincted){cerr << "ERROR::Race::epoch::This race is extinct " << newOrganisms.size() << "\t" << oldOrganisms.size() << endl; exit(EXIT_FAILURE);}
	// Primero se pasa de epoca a cada organismo de la raza (pudiendo morir algunos de ellos en el camino)
	organismsGrowUp(); // Todos los nuevos organismos son pasados a viejos organismos
						// en este momento newOrganism esta vacio y sera vuelto a llenar al final
	if( ++years >= maxYearsYoungRace )	
	{	
		youngRace = false; 
	}
	// los que quedan tienen derecho a aparearse, tomando en cuenta la posibilidad que exista tan solo un organismo en la raza es que se chequea tal.
}

void Race::newRaceDecendece( ) // no se especifica en caso de ser young
{	
	if(oldOrganisms.size()  == 1) // No hay suficientes organismos como para realizar cruzamientos.
	{
		populateFromCurrentsOrganisms (youngRaceMaxPopulation); //todos los hijos son similares a su unico padre
		return ;
	}		
	// En otro caso proceden los cruzamientos.
	createNewRaceDecendence();
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

shared_ptr <Race> Race::createNew( shared_ptr <Organism> organism )
{
	return make_unique<Race>( *this, move(organism) );
}

bool Race::isExtinct()
{
	extincted = ( ( newOrganisms.size() + oldOrganisms.size() ) == 0 ) ; // Because can be extincted from the epoch method if have 0 childs mean that is a bad race and is extincted inmediatly
	return extincted;
}

bool Race::isYoung()
{
	return youngRace;
}

vector < shared_ptr< Organism> > & Race::getNewOrganisms_ref()
{
	return newOrganisms;
}

void Race::printInfo()const
{
	cout << "RACE USER DEF: " << endl
	<< "years: " << years << endl
	<< "maxYears: " << maxYears << endl
	<< "maxYearsYoungRace: " << maxYearsYoungRace << endl
	<< "maximumRaceDistance: " << maximumRaceDistance << endl
	<< "raceTotalFitness: " << raceTotalFitness << endl
	<< "isyoungRace: " << youngRace << endl
	<< "youngRaceMaxPopulation: " << youngRaceMaxPopulation << endl
	<< "maxStackNewRaceCandidates: " << maxStackNewRaceCandidates << endl
	<< "maxStackNewSpiciesCandidates: " << maxStackNewSpiciesCandidates << endl
	<< "Race structure: " << endl
	<< "newOrganisms size:" << newOrganisms.size() << endl 
	<< "oldOrganisms size:" << oldOrganisms.size() << endl
	<< "newRaceOrgmCandidate size:" << newRaceOrgmCandidate.size() << endl
	<< "newSpicieOrgmCandidate size: " << newSpicieOrgmCandidate.size() << endl; 
}

}
