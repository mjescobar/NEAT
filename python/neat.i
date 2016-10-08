// pair.i - SWIG interface
%include <std_shared_ptr.i>
%shared_ptr(NEAT::BasicNeuron)
%shared_ptr(NEAT::Neuron)
%shared_ptr(NEAT::BasicSynapticWeight)
%shared_ptr(NEAT::TauSynapticWeight)
%shared_ptr(NEAT::SynapticWeight)
%shared_ptr(NEAT::LIFNeuron)
//%shared_ptr(NEAT::Life)
%shared_ptr(NEAT::ANN)
%shared_ptr(NEAT::Spicies)
%shared_ptr(NEAT::Organism)


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
%include <std_vector.i>

namespace std
{
    %template(FloatVector) vector<float>;
}


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
