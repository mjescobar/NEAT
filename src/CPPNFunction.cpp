#include "CPPNFunction.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace NEAT;
using namespace std;



CPPNFunction::CPPNFunction(): CPPNFunction(0.0, 0.0)
{

}

CPPNFunction::CPPNFunction(float bias, float amplifier)
{
	this->bias = bias;
	this->amplifier = amplifier;
	functionsAmount = 5;
	functionId = round((rand()/(double)RAND_MAX)*functionsAmount - 0.5);
}

CPPNFunction::CPPNFunction(const CPPNFunction & other)
{
	this->bias = other.bias;
	this->amplifier = other.amplifier;
	this->functionsAmount = other.functionsAmount;
	functionId = other.functionId;
}


float CPPNFunction::eval(const float input) const
{
	switch(functionId)
	{
		case 0:
			return this->sin_(input);
			break;
		case 1:
			return this->cos_(input);
			break;
		case 2:
			return this->gauss_(input);
			break;
		case 3:
			return this->identity_(input);
			break;
		case 4:
			return this->sigmoid_(input);
			break;
		default:
			cerr << "CPPNFUNCTION::EVAL::ERROR:: functionId out of range" << endl;
			exit(EXIT_FAILURE);
			break;
	}
}

void CPPNFunction::changeFunction() 
{
	functionId = round((rand()/(double)RAND_MAX)*functionsAmount - 0.5);
}

float CPPNFunction::sigmoid_(const float input) const
{
	return sin( amplifier*(input + bias) );
}

float CPPNFunction::sin_(const float input) const
{
	return sin( amplifier*(input + bias) );
}
float CPPNFunction::cos_(const float input) const
{
	return cos( amplifier*(input + bias) );
}
float CPPNFunction::gauss_(const float input) const
{
	return 0.5*((1/(2.50663*0.1))*exp(-0.5*input*input*10)) -1;
}
float CPPNFunction::identity_(const float input) const
{
	return ((input >= 1.0)?  1.0:  input);
}


void CPPNFunction::printInfo() const 
{
	cout << 
	"bias: " << bias << endl <<
	"amplifier: " << amplifier << endl <<
	"functionId: " << functionId << endl ;
}

unique_ptr <CPPNFunction> CPPNFunction::clone() const
{
	return move( std::make_unique<CPPNFunction>(*this) );
}
