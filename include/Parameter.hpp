#ifndef NEAT_PARAMETER
#define NEAT_PARAMETER

#include "PlatformDefinitions.hpp"
#include <cstdlib> //random

namespace NEAT
{
class Parameter
{
public:

	Parameter( float defaultValue ); // In case that is not mutable
	Parameter( float probabilityOfRandomMutation, 
		float probabilityOfDeltaMutation,
		float maximumVariation,
		float maxAdmissibleValue,
		float minAdmissibleValue);
	void mutate();

	float value;

private:

	bool isMutable;

	float probabilityOfRandomMutation;
	float probabilityOfDeltaMutation;

	// Delta info
	float maximumVariation;

	// value Limits
	float maxAdmissibleValue;
	float minAdmissibleValue;
};
}

#endif