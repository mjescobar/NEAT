#include "Organism.hpp"
#include <iostream>

namespace NEAT
{

Organism::Organism(std::unique_ptr <ANN> annSeed ) : Organism( OrganismUserDefinitions(), std::move(annSeed) ){}

Organism::Organism( const OrganismUserDefinitions& userDef, std::unique_ptr <ANN> annSeed )
{
	years = 0;
	lifeExpectative = userDef.lifeExpectative;
	fitness = 0.f;
	ann = annSeed->createSimilar();
}

Organism::Organism( const Organism& other ) // copy constructor
{ 
	years = 0;
	lifeExpectative = other.lifeExpectative;
	fitness = 0.f;
	ann = other.ann->clone();
}

Organism::Organism( std::unique_ptr <ANN> ann,  uint lifeExpectative  ) // used in crossOver
{
	years = 0;
	this->lifeExpectative = lifeExpectative;
	fitness = 0.f;
	this->ann = std::move(ann);
}

Organism::Organism( const ANN& ann, uint lifeExpectative ) // Used in create similar
{
	years = 0;
	this->lifeExpectative = lifeExpectative;
	fitness = 0.f;
	this->ann = ann.createSimilar();
}

bool Organism::surviveNewEpoch() 
{
	// Si es su primera epoca definitivamente se le deja vivir 
	if(years == 0)
	{
		years++;
		return true;
	}
	
	double random = rand()/(double)RAND_MAX;
	// Se usa una desigualdad tal que la esperanza de vida sea en probabilidad la que el usuario puso.
	if( random > lifeExpectative/(double)(1.0 + lifeExpectative) )
	{
		return false;
	}

	years++;
	return true;
} 

bool Organism::getIsNewSpicie() const
{
	return ann->getIsNewSpicie();
}

std::unique_ptr <Organism> Organism::createSimilar() const
{
	return std::move( std::make_unique <Organism>(*ann, lifeExpectative) );
}

void Organism::setFitness( const float fitness )
{
	this->fitness = fitness;
}
float Organism::getFitness () const
{
	return fitness;
}

float Organism::getDistance(const Organism& other ) const
{
	return this->ann->getDistance(*other.ann);
}

std::unique_ptr <Organism> Organism::crossOver( const Organism& other ) const
{
	return std::move( std::make_unique<Organism>( std::move(this->ann->crossOver(*other.ann)), lifeExpectative  ) );
}

void Organism::printInfo()
{
	std::cout << "years: " << years <<  "\tlifeExpectative: " << lifeExpectative << "\tfitness: " <<  fitness << std::endl;
	std::cout << "Internal ANN" << std::endl;
	ann->printInfo(); 
}


}