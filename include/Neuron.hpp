#ifndef NEURON_HPP
#define NEURON_HPP	

#include <string>
#include <vector>
#include <iostream>
/*
	Esta clase es abstracta, la  idea es poder probar diferentes tipos de neuronas de forma sencilla.
*/
namespace NEATSpikes
{
	class Neuron
	{
	public:
		virtual ~Neuron(){};

		virtual void mutate()=0;
		/**
			\brief Después de tener calculado el potencial de entrada (la suma de las conexiones entrantes) se puede calcular la salida de la neurona, posteriormente se baja a el potencial de entrada o bien podría ser que hayan neuronas que no lo hagan así sino que con expresiones matemáticas.
		*/ 
		virtual double eval()=0; // Recordar que al hacer la evaluación se debe dejar en cero el potencial de entrada para la siguiente iteración.
		/**
			\brief Antes de evaluar la salida de la neurona es necesario sumar todas las entradas.
		*/
		virtual void sumIncomingConnectionsOutputs(double incomingOneConnectionOutput)=0;
		/**
			\brief Toda neurona debe ser capas de guardar toda la información necesaria para poder volver a realizarla.
		*/
		virtual void saveState(std::string pathToSave)=0;
		/**
			\brief Se carga una neurona a través de una anteriormente guardada.
		*/
		virtual void load(std::string PathWhereIsSaved)=0;
		/**
			\brief se imprime toda la información de la neurona.
		*/
		virtual void printState()=0;
		/**
			\brief Se crea una nueva neurona.  Esto es necesario porque a priori desde la clase ANN no se sabe el tipo de neurona que se está usando. 
		*/
		virtual Neuron * createNew( int historicalMark_inicial_input, int historicalMark_inicial_output){std::cerr << "ERROR::Neuron::createNew::Method is called in Neuron class that have no implementation." << std::endl; exit(EXIT_FAILURE);};
		/**
			\brief Se crea una nueva neurona.  Esto es necesario porque a priori desde la clase ANN no se sabe el tipo de neurona que se está usando. 
		*/
		virtual Neuron * createNewInput(){std::cerr << "ERROR::Neuron::createNewInput::Method is called in Neuron class that have no implementation." << std::endl; exit(EXIT_FAILURE);};
		/**
			\brief Se crea una nueva neurona.  Esto es necesario porque a priori desde la clase ANN no se sabe el tipo de neurona que se está usando. 
		*/
		virtual Neuron * createNewOutput(){std::cerr << "ERROR::Neuron::createNewOutput::Method is called in Neuron class that have no implementation." << std::endl; exit(EXIT_FAILURE);};
		/**
			\brief Devuelve el valor de marca histórica asociada a la neurona.
		*/
		virtual int getHistoricalMark()=0;

		virtual int getInitialNeuronInHistoricalMark()=0;

		virtual int getInitialNeuronOutHistoricalMark()=0;

		virtual std::vector <int> getOutcomingConnections()=0;

		virtual std::vector <int> getIncomingConnections()=0;

		virtual void addNewIncomingConection(int innovationOfIncomingConection)=0;

		virtual void addNewOutcomingConection(int innovationOfOutcomingConection)=0;

		virtual int getLayer()=0;

		virtual double getLastOutput()=0;

		virtual Neuron * duplicate()=0;

		virtual double getDistance(Neuron * neuron)=0;

		virtual void changeValuesRandomly()=0;

		virtual void copyValues(Neuron * neuron){std::cerr << "ERROR::Neuron::copyValues::Method is called in Neuron class that have no implementation." << std::endl; exit(EXIT_FAILURE);};

		virtual void SetParametersFromUserDefinitionsPath(std::string pathUserDefinitionsAboutBasicNeuron)=0; 

		virtual void saveUserDefinitions(std::string pathToSave)=0;

	};

}
#endif