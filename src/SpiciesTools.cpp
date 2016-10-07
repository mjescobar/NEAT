#include "Spicies.hpp"
#include <iostream>
#include <algorithm>

using namespace std;
namespace NEAT
{

Race& Spicies::getRandomRace_ref()
{
	const uint racesAmount = youngRaces.size() + oldRaces.size();
	if( racesAmount == 0 ) {cerr << "ERROR::getRandomRace_ref:: no race to select" << endl;exit(EXIT_FAILURE);}	
	uniform_int_distribution<uint> randomRace(0, racesAmount-1);
	uint raceSelected = randomRace( *generator );

	if(raceSelected >= youngRaces.size())
	{
		return *(oldRaces.at(raceSelected-youngRaces.size()).get());
	}
	return *(youngRaces.at(raceSelected).get());
}

bool Spicies::detectRepeatedInnovation( const Organism & orgm )
{
	try
	{
		if( innovMsgMap.at( orgm.ann->innovationMsg ) == true )
		{
			return true;
		}
	}
	catch ( const out_of_range& exception )
	{
		innovMsgMap.emplace( orgm.ann->innovationMsg , true );
		return false;
	}
	cerr << "ERROR::detectRepeatedInnovation::This line must not be achieved by construction, some error happened" << endl;
	exit(1);
}

void Spicies::fillFitnessVector (vector <float> & fitnessVector)
{
	fitnessVector.clear();
	for( const auto& race : oldRaces )
	{
		fitnessVector.push_back( race->getFitnessMean() );
	}
}

void Spicies::createRacesFromOrganismCandidates()
{
	if( oldRaces.size() + youngRaces.size() > maxAmountOfRacesPerSpicie ){return;} // Nunca superar el limite de razas admisibles.
	uint attempts = 3;
	bool flagIsNewRace;
	for (uint k = oldRaces.size() + youngRaces.size(); k < maxAmountOfRacesPerSpicie; ++k)
	{
		for (uint i = 0; i < attempts; ++i)
		{
			auto& raceSelct = getRandomRace_ref();

			if(raceSelct.newRaceOrgmCandidate.size() >= 1)
			{
				// Select a organism of this stack randomly
				uniform_int_distribution<uint> randomOrganism(0, raceSelct.newRaceOrgmCandidate.size()-1);
				uint orgmPos = randomOrganism(*generator);
				auto& ormgSelct = *raceSelct.newRaceOrgmCandidate.at(orgmPos);

				// Chequear que no pertenezca a ninguna raza existente
				flagIsNewRace = true;
				for( auto& raceIt : oldRaces  )
				{
					if(raceIt->belongsAtThisRace( ormgSelct ))
					{
						raceSelct.newRaceOrgmCandidate.erase( raceSelct.newRaceOrgmCandidate.begin() +  orgmPos);
						flagIsNewRace = false;
						break;
					}
				}
				if(flagIsNewRace == false){	break;	}
				for( auto& raceIt : youngRaces  )
				{
					if(raceIt->belongsAtThisRace( ormgSelct ))
					{
						raceSelct.newRaceOrgmCandidate.erase( raceSelct.newRaceOrgmCandidate.begin() +  orgmPos);
						flagIsNewRace = false;
						break;
					}
				}
				if(flagIsNewRace == false){	break;	}
				//Crear nueva raza.
				if(flagIsNewRace)
				{
					youngRaces.push_back( raceSelct.createNew(move( raceSelct.newRaceOrgmCandidate.at(orgmPos) )) );
					raceSelct.newRaceOrgmCandidate.erase( raceSelct.newRaceOrgmCandidate.begin() +  orgmPos);
					break;
				}
			}
		}
	}
}

void Spicies::createDecendence(const uint childrenAmount )
{
	if(oldRaces.size() >= 1 )
	{
		vector <float> fitnessVector;
		fillFitnessVector (fitnessVector);
		vector <uint> childrensPerRace;
		for (uint i = 0; i < oldRaces.size(); ++i)
		{
			childrensPerRace.push_back(0);
		}
		discrete_distribution<uint> obtainOrganism(fitnessVector.begin(), fitnessVector.end());
		uint selected = 0;
		for (uint i = 0; i < childrenAmount; ++i)
		{
			selected = obtainOrganism(*generator);
			childrensPerRace.at(selected) += 1;
		}	
		for (uint i = 0; i < oldRaces.size(); ++i)
		{
			oldRaces.at(i)->epoch( childrensPerRace.at(i) );
		}

	}
	for (uint i = 0; i < youngRaces.size(); ++i)
	{
		youngRaces.at(i)->epoch();
		if( youngRaces.at(i)->isYoung() == false  ) // Si la raza ha crecido entonces se tiene que cambiar de vector que lo maneja.
		{
			oldRaces.push_back( move(youngRaces.at(i)) );
			youngRaces.erase(youngRaces.begin()+i);
			i--;
		}
	}
}

void Spicies::deleteExtinctedRaces()
{
	oldRaces.erase(  remove_if(oldRaces.begin(), oldRaces.end(),
    [](shared_ptr<Race>& race)->bool { return race->isExtinct(); }),
	oldRaces.end());
	youngRaces.erase(  remove_if(youngRaces.begin(), youngRaces.end(),
    [](shared_ptr<Race>& race)->bool { return race->isExtinct(); }),
	youngRaces.end());
}


}
