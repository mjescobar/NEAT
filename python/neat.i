// pair.i - SWIG interface
%module neat
%{
#include "../include/SynapticWeight.hpp"
#include "../include/Neuron.hpp"
#include "../include/BasicNeuronUserDefinitions.hpp"
#include "../include/BasicNeuron.hpp"
#include "../include/Parameter.hpp"
#include "../include/BasicSynapticWeightUserDefinitions.hpp"
#include "../include/BasicSynapticWeight.hpp"
#include "../include/Layer.hpp"
#include "../include/ANNUserDefinitions.hpp"
#include "../include/ANN.hpp"
#include "../include/OrganismUserDefinitions.hpp"
#include "../include/Organism.hpp"
#include "../include/RaceUserDefinitions.hpp"
#include "../include/Race.hpp"
#include "../include/SpiciesUserDefinitions.hpp"
#include "../include/Spicies.hpp"
#include "../include/Life.hpp"
#include "../include/TauSynapticWeightUserDefinitions.hpp"
#include "../include/TauSynapticWeight.hpp"
#include "../include/LIFNeuronUserDefinitions.hpp"
#include "../include/LIFNeuron.hpp"
%}

// Ignore the default constructor
//%ignore std::pair::pair();      

// Parse the original header file
%include "../include/SynapticWeight.hpp"
%include "../include/Neuron.hpp"
%include "../include/BasicNeuronUserDefinitions.hpp"
%include "../include/BasicNeuron.hpp"
%include "../include/Parameter.hpp"
%include "../include/BasicSynapticWeightUserDefinitions.hpp"
%include "../include/BasicSynapticWeight.hpp"
%include "../include/Layer.hpp"
%include "../include/ANNUserDefinitions.hpp"
%include "../include/ANN.hpp"
%include "../include/OrganismUserDefinitions.hpp"
%include "../include/Organism.hpp"
%include "../include/RaceUserDefinitions.hpp"
%include "../include/Race.hpp"
%include "../include/SpiciesUserDefinitions.hpp"
%include "../include/Spicies.hpp"
%include "../include/Life.hpp"
%include "../include/TauSynapticWeightUserDefinitions.hpp"
%include "../include/TauSynapticWeight.hpp"
%include "../include/LIFNeuronUserDefinitions.hpp"
%include "../include/LIFNeuron.hpp"

// Instantiate some templates
//%template(pairii) std::pair<int,int>;
//%template(pairdi) std::pair<double,int>;
