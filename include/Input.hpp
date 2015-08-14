#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>
#include <string>
#include <iostream>
#include "Neuron.hpp"
#include <fstream> // ofstream
#include <map>
#include <cstring> // strtok_r

namespace NEATSpikes
{
	class Input : public Neuron
	{
		public:
			Input();
			~Input();
			Input(const Input & in);
			Input(int _historicalMark, int historicalMark_inicial_input, int historicalMark_inicial_output, int _layer);
			/**
				\brief Bajo cierta probabilidad pueden cambiar características de esta neurona.
			*/
			void mutate();
			/**
				\brief Después de tener calculado el potencial de entrada (la suma de las conexiones entrantes) se puede calcular la salida de la neurona, posteriormente se baja a el potencial de entrada o bien podría ser que hayan neuronas que no lo hagan así sino que con expresiones matemáticas.
			*/ 
			double eval(); // Recordar que al hacer la evaluación se debe dejar en cero el potencial de entrada para la siguiente iteración.
			/**
				\brief Antes de evaluar la salida de la neurona es necesario sumar todas las entradas.
			*/
			void sumIncomingConnectionsOutputs(double incomingOneConnectionOutput);
			/**
				\brief Toda neurona debe ser capas de guardar toda la información necesaria para poder volver a realizarla.
			*/
			void saveState(std::string pathToSave);
			/**
				\brief Se carga una neurona a través de una anteriormente guardada.
			*/
			void load(std::string PathWhereIsSaved);
			/**
				\brief se imprime toda la información de la neurona.
			*/
			void printState();
			/**
				\brief  Los inputs no pueden ser creados a través de este método. El uso de la función no está permitido.
			*/
			Neuron * createNew(Neuron * prototype, int historicalMark, int historicalMark_inicial_input, int historicalMark_inicial_output, int layer);

			int getHistoricalMark();

			int getInitialNeuronInHistoricalMark();

			int getInitialNeuronOutHistoricalMark();

			std::vector <int> getOutcomingConnections();

			std::vector <int> getIncomingConnections();

			void addNewIncomingConection(int innovationOfIncomingConection);

			void addNewOutcomingConection(int innovationOfOutcomingConection);

			double getLastOutput();

			int getLayer();
			
			Neuron * duplicate();

			double getDistance(Neuron * neuron) { return 0; };

			void changeValuesRandomly() { };

		private:
			double inputVoltage;
			double lastOutput;
			int identificator;
			int layer;
			int historicalMark;
			int historicalMarkNeuronInicialIn;
			int historicalMarkNeuronInicialOut;
			std::vector <int> incomingConections;
			std::vector <int> outcomingConections;
			static int id;
	};
}


#endif