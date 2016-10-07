#ifndef NEAT_PARAMETER
#define NEAT_PARAMETER

#include <memory> // shared_ptr

namespace NEAT
{
class Parameter
{
public:
	Parameter( float defaultValue ); // In case that is not mutable
	Parameter( float probabilityOfRandomMutation, 
		float maximumPercentVariation,
		float maxAdmissibleValue,
		float minAdmissibleValue);
	Parameter ( const Parameter & other);
	virtual ~Parameter ();
	void mightMutate();
	std::shared_ptr < Parameter > clone();
	void random();
	float value;
private:

	bool isMutable;
	float probabilityOfRandomMutation;
	// Delta info
	float maximumPercentVariation;
	// value Limits
	float maxAdmissibleValue;
	float minAdmissibleValue;
};
}

#endif
