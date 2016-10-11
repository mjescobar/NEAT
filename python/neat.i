%module neat

%include <std_shared_ptr.i>
%shared_ptr(NEAT::CPPNNeuron)
%shared_ptr(NEAT::BasicNeuron)
%shared_ptr(NEAT::Neuron)
%shared_ptr(NEAT::BasicSynapticWeight)
%shared_ptr(NEAT::TauSynapticWeight)
%shared_ptr(NEAT::SynapticWeight)
%shared_ptr(NEAT::LifeUserDefinitions)
%shared_ptr(NEAT::LIFNeuron)
//%shared_ptr(NEAT::Life)
%shared_ptr(NEAT::ANN)
%shared_ptr(NEAT::Spicies)
%shared_ptr(NEAT::SpiciesUserDefinitions)
%shared_ptr(NEAT::Organism)
%shared_ptr(NEAT::Race)
%shared_ptr(NEAT::RaceUserDefinitions)


%{
#include "../include/SynapticWeight.hpp"
#include "../include/Neuron.hpp"
#include "../include/BasicNeuronUserDefinitions.hpp"
#include "../include/BasicNeuron.hpp"
#include "../include/CPPNNeuronUserDefinitions.hpp"
#include "../include/CPPNNeuron.hpp"
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
#include "../include/LifeUserDefinitions.hpp"
#include "../include/Life.hpp"
#include "../include/TauSynapticWeightUserDefinitions.hpp"
#include "../include/TauSynapticWeight.hpp"
#include "../include/LIFNeuronUserDefinitions.hpp"
#include "../include/LIFNeuron.hpp"
#include "../include/NEATStatistics.hpp"
%}


%include <std_vector.i>

namespace std
{
    %template(FloatVector) vector<float>;
}


%include "../include/SynapticWeight.hpp"
%include "../include/Neuron.hpp"
%include "../include/BasicNeuronUserDefinitions.hpp"
%include "../include/BasicNeuron.hpp"
%include "../include/CPPNNeuronUserDefinitions.hpp"
%include "../include/CPPNNeuron.hpp"
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
%include "../include/LifeUserDefinitions.hpp"
%include "../include/Life.hpp"
%include "../include/TauSynapticWeightUserDefinitions.hpp"
%include "../include/TauSynapticWeight.hpp"
%include "../include/LIFNeuronUserDefinitions.hpp"
%include "../include/LIFNeuron.hpp"
%include "../include/NEATStatistics.hpp"


/*
%{
/* This function matches the prototype of the normal C callback
   function for our widget. However, we use the clientdata pointer
   for holding a reference to a Python callable object. * /

static double PythonCallBack(double a, void *clientdata)
{
   PyObject *func, *arglist;
   PyObject *result;
   double    dres = 0;
   
   func = (PyObject *) clientdata;               // Get Python function
   arglist = Py_BuildValue("(d)",a);             // Build argument list
   result = PyEval_CallObject(func,arglist);     // Call Python
   Py_DECREF(arglist);                           // Trash arglist
   if (result) {                                 // If no errors, return double
     dres = PyFloat_AsDouble(result);
   }
   Py_XDECREF(result);
   return dres;
}
%}

// Attach a new method to our plot widget for adding Python functions
%addmethods PlotWidget {
   // Set a Python function object as a callback function
   // Note : PyObject *pyfunc is remapped with a typempap
   void set_pymethod(PyObject *pyfunc) {
     self->set_method(PythonCallBack, (void *) pyfunc);
     Py_INCREF(pyfunc);
   }
}
*/

