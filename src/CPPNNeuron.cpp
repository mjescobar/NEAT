#include "CPPNNeuron.hpp"

#include <cmath> // exp fabs
#include <iostream>
#include <memory>
#include <fstream>
using namespace std;
namespace NEAT
{

CPPNNeuron::~CPPNNeuron()
{

}

CPPNNeuron::CPPNNeuron() : CPPNNeuron(  CPPNNeuronUserDefinitions() ){}

CPPNNeuron::CPPNNeuron( const CPPNNeuronUserDefinitions& cPPNNeuronUserDefinitions )
{
	inputVoltageAccum = 0.f;
	output = 0.f;
	constantDistanceOfBias = cPPNNeuronUserDefinitions.constantDistanceOfBias;
	constantDistanceOfAmplifier = cPPNNeuronUserDefinitions.constantDistanceOfAmplifier;
	changeFunctionRate = cPPNNeuronUserDefinitions.changeFunctionRate;
	bias = make_unique < Parameter > ( cPPNNeuronUserDefinitions.probabilityOfBiasRandomMutation,
		cPPNNeuronUserDefinitions.maximumBiasPercentVariation,
		cPPNNeuronUserDefinitions.maxBias,
		cPPNNeuronUserDefinitions.minBias
		 );
	
	Amplifier = make_unique < Parameter > ( cPPNNeuronUserDefinitions.probabilityOfAmplifierRandomMutation,
		cPPNNeuronUserDefinitions.maximumAmplifierPercentVariation,
		cPPNNeuronUserDefinitions.maxAmplifier,
		cPPNNeuronUserDefinitions.minAmplifier
		 );
	lastInputAccum = 0.f;

	mutationProbability = cPPNNeuronUserDefinitions.mutationProbability;
	cppnFunction = make_unique<CPPNFunction>(bias->value, Amplifier->value);
	constantDistanceOfFunction = cPPNNeuronUserDefinitions.constantDistanceOfFunction;

}

CPPNNeuron::CPPNNeuron( const CPPNNeuron & other) 
{
	inputVoltageAccum = 0.f;
	lastInputAccum = 0.f;
	output = 0.f;
	constantDistanceOfBias = other.constantDistanceOfBias;
	constantDistanceOfAmplifier = other.constantDistanceOfAmplifier;
	changeFunctionRate = other.changeFunctionRate;
	mutationProbability = other.mutationProbability;
	constantDistanceOfFunction = other.constantDistanceOfFunction;
	bias = other.bias->clone();
	Amplifier = other.Amplifier->clone();
	cppnFunction = other.cppnFunction->clone();
	cppnFunction->bias = bias->value;
	cppnFunction->amplifier = Amplifier->value;
}

void CPPNNeuron::mightMutate()
{
	if( rand()/(double)RAND_MAX < mutationProbability)
	{
		bias->mightMutate();
		Amplifier->mightMutate();
		cppnFunction->bias = this->bias->value;
		cppnFunction->amplifier = this->Amplifier->value;
		if(rand()/(double)RAND_MAX  < changeFunctionRate){
			cppnFunction->changeFunction();
		}
	}
}

//Falta la distancia por funcion
float CPPNNeuron::getDistance( const Neuron * otherNeuron ) const
{
	const CPPNNeuron * otherCPPNNeuron = dynamic_cast < const CPPNNeuron *  > (  otherNeuron );
	if(otherCPPNNeuron == NULL)
	{
		cerr << "CPPNNeuron::getDistance::otherNeuron is not CPPNNeuron type." << endl;
		exit( EXIT_FAILURE );
	}

	return this->constantDistanceOfBias * ( fabs(this->bias->value - otherCPPNNeuron->bias->value)  )  + this->constantDistanceOfAmplifier * ( fabs(this->Amplifier->value - otherCPPNNeuron->Amplifier->value)  )  + this->constantDistanceOfFunction * ( (this->cppnFunction->functionId != otherCPPNNeuron->cppnFunction->functionId)?1.0:0.0 ) ;
}

void CPPNNeuron::spread()
{
	receiveInccommingVoltage(); // in father class
	
	output = cppnFunction->eval(inputVoltageAccum);

	lastInputAccum = inputVoltageAccum; // solo para mostrar en informaciones.
	inputVoltageAccum = 0.0; // Se descarga la neurona y su potencial de entrada vuelve a cero.
	sendVoltageToOutcomingSynapticWeights();
}

void CPPNNeuron::printInfo() const
{
	cout << "Bias: " << bias->value << "\tAmplifier: " << Amplifier->value << "\tlastInputAccum: "<< lastInputAccum << "\tinputVoltageAccum: " << inputVoltageAccum << "\toutput: " << output << "\tmutationProbability: "<< mutationProbability<< endl;
	cppnFunction->printInfo();

}

shared_ptr < Neuron > CPPNNeuron::clone() const 
{
	return move( make_unique < CPPNNeuron > (*this) );
}

shared_ptr < Neuron > CPPNNeuron::createNew() const
{
	auto tmp = make_unique < CPPNNeuron > (*this);
	tmp->bias->random();
	tmp->Amplifier->random();
	tmp->cppnFunction->changeFunction();
	tmp->cppnFunction->bias = tmp->bias->value;
	tmp->cppnFunction->amplifier = tmp->Amplifier->value;
	return move( tmp );
}


void CPPNNeuron::save( const std::string path ) const 
{
	ofstream file;
	file.open(path);
	if(file.is_open())
	{
		file << 
		"bias " << bias->value <<  endl << 
		"Amplifier " << Amplifier->value <<  endl << 
		"functionId " << cppnFunction->functionId <<  endl;
		file.close();
	}
	else
	{
		cerr << "CPPNNeuron::save::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}

void CPPNNeuron::load( const string path)
{
	ifstream file;
	file.open(path, ios::in);
	string line;
	if (file.is_open())
	{
		while ( getline (file,line, char(' ') ) )
		{
			if (line.compare("bias") == 0)
			{
				getline (file,line, char('\n') );
				bias->value =  stof(line);
				cppnFunction->bias = bias->value ;

			}
			else if(line.compare("Amplifier") == 0)
			{
				getline (file,line, char('\n') );
				Amplifier->value =  stof(line);
				cppnFunction->amplifier = Amplifier->value ;
			}
			else if(line.compare("functionId") == 0)
			{
				getline (file,line, char('\n') );
				cppnFunction->functionId =  stoul(line);
			}
		}
		file.close();
	}
	else
	{
		cerr << "ERROR::CPPNNeuron::load::File could not be opened" << endl;
		exit(EXIT_FAILURE);
	}
}

} // End namespace NEAT
