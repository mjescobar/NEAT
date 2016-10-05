#include "LIFNeuron.hpp"

#include <cmath> // exp fabs
#include <iostream>

using namespace std;

namespace NEAT
{

LIFNeuron::~LIFNeuron()
{
}

LIFNeuron::LIFNeuron(float samplingDelaTime) : LIFNeuron(  LIFNeuronUserDefinitions(), samplingDelaTime ){}

LIFNeuron::LIFNeuron( const LIFNeuronUserDefinitions& LIFNeuronUserDefinitions, float samplingDelaTime )
{
	outputCurrentAmpliude = 0.f;
	inputVoltageAccum = 0.f;
	output = 0.f;
	lastInputAccum = 0.f;
	this->samplingDelaTime = samplingDelaTime;

	resistence = make_unique < Parameter > ( LIFNeuronUserDefinitions.probabilityOfResistenceRandomMutation,
		LIFNeuronUserDefinitions.maximumResistencePercentVariation,
		LIFNeuronUserDefinitions.maxResistence,
		LIFNeuronUserDefinitions.minResistence
		 );
	
	membraneTimeConstant = make_unique < Parameter > ( LIFNeuronUserDefinitions.probabilityOfMembraneTimeConstantRandomMutation,
		LIFNeuronUserDefinitions.maximumMembraneTimeConstantPercentVariation,
		LIFNeuronUserDefinitions.maxMembraneTimeConstant,
		LIFNeuronUserDefinitions.minMembraneTimeConstant
		 );

	currentTimeConstant = make_unique < Parameter > ( LIFNeuronUserDefinitions.probabilityOfCurrentTimeConstantRandomMutation,
		LIFNeuronUserDefinitions.maximumCurrentTimeConstantPercentVariation,
		LIFNeuronUserDefinitions.maxCurrentTimeConstant,
		LIFNeuronUserDefinitions.minCurrentTimeConstant
		 );
	constantDistanceResistence = LIFNeuronUserDefinitions.constantDistanceResistence;
	constantDistanceMembraneTimeConstant = LIFNeuronUserDefinitions.constantDistanceMembraneTimeConstant;
	constantDistanceCurrentTimeConstant = LIFNeuronUserDefinitions.constantDistanceCurrentTimeConstant;
	resetVoltage =  LIFNeuronUserDefinitions.resetVoltage;
	spikeThreshold =  LIFNeuronUserDefinitions.spikeThreshold;
	mutationProbability = LIFNeuronUserDefinitions.mutationProbability;
	membranVoltage = 0.f;
	timeEnlacedFromLastSpike = 0.f;
}

LIFNeuron::LIFNeuron( const LIFNeuron & other) 
{
	this->samplingDelaTime = other.samplingDelaTime;

	inputVoltageAccum = 0.f;
	lastInputAccum = 0.f;
	output = 0.f;
	currentTimeConstant = other.currentTimeConstant->clone();
	membraneTimeConstant = other.membraneTimeConstant->clone();
	resistence = other.resistence->clone();
	constantDistanceResistence = other.constantDistanceResistence;
	constantDistanceMembraneTimeConstant = other.constantDistanceMembraneTimeConstant;
	constantDistanceCurrentTimeConstant = other.constantDistanceCurrentTimeConstant;
	resetVoltage = other.resetVoltage;
	spikeThreshold = other.spikeThreshold;
	mutationProbability = other.mutationProbability;
	outputCurrentAmpliude = 0.f;
	membranVoltage = 0.f;
	timeEnlacedFromLastSpike = 0.f;
}

void LIFNeuron::mightMutate()
{
	if( rand()/(double)RAND_MAX < mutationProbability)
	{
		resistence->mightMutate();
		membraneTimeConstant->mightMutate();
		currentTimeConstant->mightMutate();
	}
}

float LIFNeuron::getDistance( const Neuron * otherNeuron ) const
{
	const LIFNeuron * otherLIFNeuron = dynamic_cast < const LIFNeuron *  > (  otherNeuron );
	if(otherLIFNeuron == NULL)
	{
		cerr << "LIFNeuron::getDistance::otherNeuron is not LIFNeuron type." << endl;
		exit( EXIT_FAILURE );
	}

	return this->constantDistanceResistence * fabs(this->resistence->value - otherLIFNeuron->resistence->value) + this->constantDistanceMembraneTimeConstant *  fabs(this->membraneTimeConstant->value - otherLIFNeuron->membraneTimeConstant->value) + this->constantDistanceCurrentTimeConstant *  fabs(this->currentTimeConstant->value - otherLIFNeuron->currentTimeConstant->value)    ;
}

void LIFNeuron::spread()
{
	receiveInccommingVoltage(); // in father class

	float tmpResult  = samplingDelaTime / membraneTimeConstant->value;
	membranVoltage = membranVoltage * (1.f - tmpResult) + tmpResult * resistence->value * inputVoltageAccum;
	inputVoltageAccum = 0.0; // Se descarga la neurona y su potencial de entrada vuelve a cero.

	if( membranVoltage > spikeThreshold )
	{
		spike();
		membranVoltage = resetVoltage;
	}
	simulateNextCurrent();
	output = outputCurrent; // output is used in sendVoltageToOutcomingSynapticWeights by the abstract father class 'Neuron'
	sendVoltageToOutcomingSynapticWeights();
}

void LIFNeuron::printInfo() const
{
	cout << "samplingDelaTime: " << samplingDelaTime << endl
	<< "resistence: " << resistence->value << endl
	<< "membraneTimeConstant: " << membraneTimeConstant->value << endl
	<< "currentTimeConstant: " << currentTimeConstant->value << endl
	<< "constantDistanceResistence: " << constantDistanceResistence << endl
	<< "constantDistanceMembraneTimeConstant: " << constantDistanceMembraneTimeConstant << endl
	<< "constantDistanceCurrentTimeConstant: " << constantDistanceCurrentTimeConstant << endl
	<< "membranVoltage: " << membranVoltage << endl
	<< "resetVoltage: " << resetVoltage << endl
	<< "spikeThreshold: " << spikeThreshold << endl
	<< "mutationProbability: " << mutationProbability << endl
	<< "timeEnlacedFromLastSpike: " << timeEnlacedFromLastSpike << endl;
}

unique_ptr < Neuron > LIFNeuron::clone() const 
{
	return move( make_unique < LIFNeuron > (*this) );
}

unique_ptr < Neuron > LIFNeuron::createNew() const
{
	auto tmp = make_unique < LIFNeuron > (*this);
	tmp->resistence->random();
	tmp->membraneTimeConstant->random();
	tmp->currentTimeConstant->random();
	return move( tmp );
}

void LIFNeuron::spike()
{
	outputCurrent = outputCurrent + 1.f;
	outputCurrentAmpliude = outputCurrent;
	timeEnlacedFromLastSpike = 0.f;
}

void LIFNeuron::simulateNextCurrent()
{
	timeEnlacedFromLastSpike += samplingDelaTime;
	outputCurrent = outputCurrentAmpliude * exp( -timeEnlacedFromLastSpike/currentTimeConstant->value );
}

float LIFNeuron::getMembraneVoltage()
{
	return membranVoltage;
}




} // End namespace NEAT