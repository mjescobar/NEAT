#include "Race.hpp"
#include <iostream>
#include <algorithm>

namespace NEAT
{

void Race::addOrganismCandidateToNewRace( std::unique_ptr <Organism> candidate )
{
	if( newRaceOrgmCandidate.size() >= maxStackNewRaceCandidates  )
	{
		return ;
	}
	newRaceOrgmCandidate.push_back(std::move(candidate));
}

void Race::addOrganismCandidateToNewSpicies( std::unique_ptr <Organism> candidate  )
{
	if( newSpicieOrgmCandidate.size() >= maxStackNewSpiciesCandidates  )
	{
		return ;
	}
	newSpicieOrgmCandidate.push_back(std::move(candidate));
}

void Race::updateTotalFitness()
{
	raceTotalFitness = 0.f;
	for( const auto& orgm : oldOrganisms )
	{
		raceTotalFitness += orgm->getFitness();
	}
}


void Race::fillFitnessVector (std::vector <float> & fitnessVector)
{
	fitnessVector.clear();
	for( const auto& orgm : oldOrganisms )
	{
		fitnessVector.push_back( orgm->getFitness() );
	}
}

void Race::populateFromCurrentsOrganisms( uint amountOfChildrens )
{
	const uint attempts = 2; // Solo dos intentos de que los organismos sean de la misma raza
	for (uint i = 0; i < amountOfChildrens; ++i) // Se intentara crear esta cantidad de hijos pero no es seguro dado que puede que aparezcan muchos hijos que no pertenezcan y para no parar el cpu solo hay dos intentos por ciclo de que se logre.
	{
		auto& fatherOrgm = getRandomOrganism_ref();
		for(uint j= 0; j< attempts; j++)
		{
			auto sonOrgm = fatherOrgm.createSimilar();
			if( fatherOrgm.getDistance( *sonOrgm ) <= maximumRaceDistance )
			{
				newOrganisms.push_back( std::move(sonOrgm) );
				break;
			}
			else{
				addOrganismCandidateToNewRace( std::move(sonOrgm) );
			}
		}
	}
}

Organism& Race::getRandomOrganism_ref()
{
	const uint amountOfOrganisms = newOrganisms.size() + oldOrganisms.size();
	std::uniform_int_distribution<uint> randomOrganism(0, amountOfOrganisms-1);
	const uint orgmSelct =randomOrganism(*generator);
	if(orgmSelct >= newOrganisms.size())
	{
		return *oldOrganisms.at( orgmSelct - newOrganisms.size() ).get();
	}
	return *newOrganisms.at(orgmSelct).get();
}


void Race::organismsGrowUp()
{

	for (uint i = 0; i < newOrganisms.size(); ++i)
	{
		oldOrganisms.push_back( std::move(newOrganisms.at(i)) );
	}
	newOrganisms.clear();
	oldOrganisms.erase(  std::remove_if(oldOrganisms.begin(), oldOrganisms.end(),
        [](std::unique_ptr<Organism>& orgm)->bool { return !orgm->surviveNewEpoch(); }),
    	oldOrganisms.end());
}


void Race::createDecendence(const uint amountOfChildrens )
{
	std::vector <float> fitnessVector;
	fillFitnessVector (fitnessVector); // Se llenaron los fitness en orden.
	std::discrete_distribution<uint> obtainOrganism(fitnessVector.begin(), fitnessVector.end());

	uint attempts = 2; // Si dos veces no se obtiene un organismo que sea de esta raza simplemente no se intenta de nuevo, para no dejar el cpu muy colapsado en esta operacion.
	for (uint i = 0; i < amountOfChildrens; ++i)
	{
		for (uint j = 0; j < attempts; ++j)
		{
			uint father = obtainOrganism(*generator);
			auto& fatherOrgm = *oldOrganisms.at(father).get();

			fitnessVector.at( father ) = 0.f; // Se va a crear una nueva distribucion pero sin el padre para que sea 
			std::discrete_distribution<uint> obtainDiferentOrganism(fitnessVector.begin(), fitnessVector.end());
			uint mother = obtainDiferentOrganism(*generator);
			auto& motherOrgm =  *oldOrganisms.at(mother).get();

			std::unique_ptr <Organism> sonOrgm = fatherOrgm.crossOver( motherOrgm );

			if( sonOrgm->getIsNewSpicie() )
			{
				addOrganismCandidateToNewSpicies( std::move(sonOrgm) );
			}else if( sonOrgm->getDistance( fatherOrgm ) < maximumRaceDistance   )
			{
				newOrganisms.push_back( std::move( sonOrgm ) );
				break; // Se obtuvo un nuevo organismo de la raza.
			}else
			{
				addOrganismCandidateToNewRace( std::move(sonOrgm) );
			}
		}
	}
}



}