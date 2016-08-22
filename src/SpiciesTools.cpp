#include "Spicies.hpp"
#include <iostream>
namespace NEAT
{

Race& Spicies::getRandomRace_ref()
{
	uint racesAmount = youngRaces.size() + oldRaces.size();
	std::uniform_int_distribution<uint> randomRace(0, racesAmount-1);
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
	catch ( const std::out_of_range& exception )
	{
		innovMsgMap.emplace( orgm.ann->innovationMsg , true );
		return false;
	}
	std::cerr << "ERROR::detectRepeatedInnovation::This line must not be achieved by construction, some error happened" << std::endl;
	exit(1);
}

void Spicies::fillFitnessVector (std::vector <float> & fitnessVector)
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
				std::uniform_int_distribution<uint> randomOrganism(0, raceSelct.newRaceOrgmCandidate.size()-1);
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

				for( auto& raceIt : youngRaces  )
				{
					if(raceIt->belongsAtThisRace( ormgSelct ))
					{
						raceSelct.newRaceOrgmCandidate.erase( raceSelct.newRaceOrgmCandidate.begin() +  orgmPos);
						flagIsNewRace = false;
						break;
					}
				}
				
				//Crear nueva raza.
				if(flagIsNewRace)
				{
					youngRaces.push_back( raceSelct.createNew(std::move( raceSelct.newRaceOrgmCandidate.at(orgmPos) )) );
					raceSelct.newRaceOrgmCandidate.erase( raceSelct.newRaceOrgmCandidate.begin() +  orgmPos);
					break;
				}
			}
		}
	}
}

void Spicies::createDecendence(const uint childrenAmount )
{
	std::vector <float> fitnessVector;
	fillFitnessVector (fitnessVector);
	std::vector <uint> childrensPerRace;
	for (uint i = 0; i < oldRaces.size(); ++i)
	{
		childrensPerRace.push_back(0);
	}
	std::discrete_distribution<uint> obtainOrganism(fitnessVector.begin(), fitnessVector.end());
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
	for (uint i = 0; i < youngRaces.size(); ++i)
	{
		youngRaces.at(i)->epoch();
		if( youngRaces.at(i)->isYoung() == false  ) // Si la raza ha crecido entonces se tiene que cambiar de vector que lo maneja.
		{
			oldRaces.push_back( std::move(youngRaces.at(i)) );
			youngRaces.erase(youngRaces.begin()+i);
			i--;
		}
	}
}

void Spicies::deleteExtinctedRaces()
{
	for (uint i = 0; i < oldRaces.size(); ++i)
	{
		if( oldRaces.at(i)->isExtincted() )
		{
			oldRaces.erase( oldRaces.begin() + i );
			i--;
		}
	}
}


}