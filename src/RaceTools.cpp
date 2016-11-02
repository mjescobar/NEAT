#include "Race.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

namespace NEAT
{

void Race::addOrganismCandidateToNewRace( shared_ptr <Organism> candidate )
{
	if( newRaceOrgmCandidate.size() >= maxStackNewRaceCandidates  )
	{
		return ;
	}
	newRaceOrgmCandidate.push_back(move(candidate));
}

void Race::addOrganismCandidateToNewSpicies( shared_ptr <Organism> candidate  )
{
	if( newSpicieOrgmCandidate.size() >= maxStackNewSpiciesCandidates  )
	{
		return ;
	}
	newSpicieOrgmCandidate.push_back(move(candidate));
}

void Race::updateTotalFitness()
{
	raceTotalFitness = 0.f;
	for( const auto& orgm : newOrganisms )
	{
		raceTotalFitness += orgm->getFitness();
	}
	for( const auto& orgm : oldOrganisms )
	{
		raceTotalFitness += orgm->getFitness();
	}
}

void Race::fillFitnessVector (vector <float> & fitnessVector)
{
	fitnessVector.clear();
	for( const auto& orgm : oldOrganisms )
	{
		fitnessVector.push_back( orgm->getAdjustedFitness() );
	}
}

void Race::populateFromCurrentsOrganisms( uint amountOfChildrens )
{
	const uint attempts = 4; // Solo dos intentos de que los organismos sean de la misma raza
	for (uint i = 0; i < amountOfChildrens; ++i) // Se intentara crear esta cantidad de hijos pero no es seguro dado que puede que aparezcan muchos hijos que no pertenezcan y para no parar el cpu solo hay dos intentos por ciclo de que se logre.
	{
		auto& fatherOrgm = getRandomOrganism_ref();
		for(uint j= 0; j< attempts; j++)
		{
			auto sonOrgm = fatherOrgm.createSimilar(); // no puede ser de otra especie el resultado dado que es de la misma topologia.
			if( fatherOrgm.getDistance( *sonOrgm ) <= maximumRaceDistance )
			{
				newOrganisms.push_back( move(sonOrgm) );
				break;
			}
			else{
				addOrganismCandidateToNewRace( move(sonOrgm) );
			}
		}
	}
}

Organism& Race::getRandomOrganism_ref()
{
	const uint amountOfOrganisms = newOrganisms.size() + oldOrganisms.size();
	if( amountOfOrganisms == 0 ) {cerr << "ERROR::getRandomOrganism_ref:: no Organisms to select" << endl;}
	uniform_int_distribution<uint> randomOrganism(0, amountOfOrganisms-1);
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
		oldOrganisms.push_back( move(newOrganisms.at(i)) );
	}
	newOrganisms.clear();
	oldOrganisms.erase(  remove_if(oldOrganisms.begin(), oldOrganisms.end(),
        [](shared_ptr<Organism>& orgm)->bool { return !orgm->surviveNewEpoch(); }),
    	oldOrganisms.end());
}

void Race::getChildFromParentAt(const uint & placeOfFather)
{
	auto& fatherOrgm = *oldOrganisms.at(placeOfFather).get();
	vector <float> fitnessVector;
	fillFitnessVector (fitnessVector); // Se llenaron los fitness en orden.
	fitnessVector.at( placeOfFather ) = 0.f;
	discrete_distribution<uint> obtainDiferentOrganism(fitnessVector.begin(), fitnessVector.end());
	uint attempts = 4; // Si cuatro veces no se obtiene un organismo que sea de esta raza simplemente no se intenta de nuevo, para no dejar el cpu muy colapsado en esta operacion.
	for (uint j = 0; j < attempts; ++j)
	{
		uint mother = obtainDiferentOrganism(*generator);
		auto& motherOrgm =  *oldOrganisms.at(mother).get();
		shared_ptr <Organism> sonOrgm = fatherOrgm.crossOver( motherOrgm );
		if( sonOrgm->getIsNewSpicie() )
		{
			addOrganismCandidateToNewSpicies( move(sonOrgm) );
		}else if( sonOrgm->getDistance( fatherOrgm ) < maximumRaceDistance   )
		{
			newOrganisms.push_back( move( sonOrgm ) );
			break; // Se obtuvo un nuevo organismo de la raza.
		}else
		{
			addOrganismCandidateToNewRace( move(sonOrgm) );
		}
	}
}
void Race::createNewRaceDecendence()
{
	if(oldOrganisms.size() == 0)
	{
		return;
	}

	vector <float> fitnessVector;
	fillFitnessVector (fitnessVector); // Se llenaron los fitness en orden.
	discrete_distribution<uint> obtainOrganism(fitnessVector.begin(), fitnessVector.end());

	uint attempts = 4; // Si dos veces no se obtiene un organismo que sea de esta raza simplemente no se intenta de nuevo, para no dejar el cpu muy colapsado en esta operacion.
	for (uint i = 0; i < youngRaceMaxPopulation; ++i)
	{
		for (uint j = 0; j < attempts; ++j)
		{
			uint father = obtainOrganism(*generator);
			auto& fatherOrgm = *oldOrganisms.at(father).get();

			fitnessVector.at( father ) = 0.f; // Se va a crear una nueva distribucion pero sin el padre para que sea 
			discrete_distribution<uint> obtainDiferentOrganism(fitnessVector.begin(), fitnessVector.end());
			uint mother = obtainDiferentOrganism(*generator);
			auto& motherOrgm =  *oldOrganisms.at(mother).get();

			shared_ptr <Organism> sonOrgm = fatherOrgm.crossOver( motherOrgm );


			if( sonOrgm->getIsNewSpicie() )
			{
				addOrganismCandidateToNewSpicies( move(sonOrgm) );
			}else if( sonOrgm->getDistance( fatherOrgm ) < maximumRaceDistance   )
			{
				newOrganisms.push_back( move( sonOrgm ) );
				break; // Se obtuvo un nuevo organismo de la raza.
			}else
			{
				addOrganismCandidateToNewRace( move(sonOrgm) );
			}
		}
	}
}

void Race::eliminateWorseOrganisms()
{
	float worseOrganismEliminationRate = 0.5;
	float minSafeOrganismsInRace = 5;

	if(newOrganisms.size() > minSafeOrganismsInRace)
	{
		auto amountOrganismsToEliminate = round(worseOrganismEliminationRate*newOrganisms.size());
		if(newOrganisms.size() -amountOrganismsToEliminate < minSafeOrganismsInRace)
		{
			amountOrganismsToEliminate = newOrganisms.size() - minSafeOrganismsInRace; // To assure that at least minSafeOrganismsInRace will survive.
		}

		while(amountOrganismsToEliminate > 0)
		{
			eliminateWorseOrganism();
			amountOrganismsToEliminate--;
		}
	}
}

void Race::eliminateWorseOrganism()
{
	float minFitness = newOrganisms.at(0)->getFitness();
	uint minPlace = 0;
	for (uint i = 0; i < newOrganisms.size(); ++i)
	{
		if(newOrganisms.at(i)->getFitness() <= minFitness)
		{
			minFitness = newOrganisms.at(i)->getFitness() ;
			minPlace = i;
		}
	}
	newOrganisms.erase(newOrganisms.begin() + minPlace);
}

} // END NAMESPACE NEAT
