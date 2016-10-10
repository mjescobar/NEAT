#ifndef CPPNFunction_HPP
#define CPPNFunction_HPP

#include <memory>

namespace NEAT
{


class CPPNFunction
{
public:

CPPNFunction();
CPPNFunction(float bias, float amplifier);
CPPNFunction(const CPPNFunction & other);
float eval(const float input) const;
void changeFunction();
void printInfo() const;
std::unique_ptr <CPPNFunction> clone() const;

float bias;
float amplifier;
unsigned int functionId;

private:

unsigned int functionsAmount;
float sin_(const float input) const;
float cos_(const float input) const;
float gauss_(const float input) const;
float identity_(const float input) const;
float sigmoid_(const float input) const;
};

} // END NAMESPACE NEAT
#endif