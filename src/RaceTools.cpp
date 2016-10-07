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
		fitnessVector.push_back( orgm->getFitness() );
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


void Race::createDecendence(const uint amountOfChildrens )
{
	if(amountOfChildrens == 0){return;}
	vector <float> fitnessVector;
	fillFitnessVector (fitnessVector); // Se llenaron los fitness en orden.
	discrete_distribution<uint> obtainOrganism(fitnessVector.begin(), fitnessVector.end());

	uint attempts = 4; // Si dos veces no se obtiene un organismo que sea de esta raza simplemente no se intenta de nuevo, para no dejar el cpu muy colapsado en esta operacion.
	for (uint i = 0; i < amountOfChildrens; ++i)
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
	float fitnessMean = 0.f; 
	float min = -1.f;
	float max = 0.f;
	float fitness;
	for(auto& orgm : newOrganisms)
	{
		fitness = orgm->getFitness();
		if( fitness > min )
		{
			min = fitness;
		}
		if(fitness < max)
		{
			max = fitness;
		}
		fitnessMean += fitness;
	}
	fitnessMean = fitnessMean/(float)newOrganisms.size();

	if(min == max) {return;} // some rare case;

	// ToDo: Mejorar el modelo tal que, por ejemplo, la probabilidad de supervivencia sea una gaussiana con la misma media y promedio que las especies (solo para las especies de fitness menor que la media)
	newOrganisms.erase(  remove_if(newOrganisms.begin(), newOrganisms.end(),
    [&](shared_ptr<Organism>& orgm)->bool 
    { 
		if( orgm->getFitness() < fitnessMean )
		{
			return true;
		}
		return false;
     }),newOrganisms.end());
}

}
