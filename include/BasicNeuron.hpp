#ifndef BASICNEURON_HPP
#define BASICNEURON_HPP	
/*
	BasicNeuron es una neurona que realiza la función sigmoid a la entrada para dar la salida.
*/
#include "Neuron.hpp"
#include <cstdlib> // RAND_MAX, EXIT_FAILURE, srand, rand 
#include <map> // Para usar map.
#include <cstring> // strtok_r
#include <iostream> // cout, cerr, cin, clog
#include <stdio.h> // getline
#include <fstream> // ofstream
#include <cmath> // exp

namespace NEATSpikes{
	class BasicNeuron : public Neuron 
	{
		// Primero los métodos y después las variables.
		// ========================================================================================================
		// ====================================== Métodos ============================================================
		public:

			/**
				\brief Se crea una nueva neurona la cual tendrá seteados los valores historicalMark, historicalMark_inicial_input, historicalMark_inicial_output, layer; además tendrá valores aleatorios de bias y constante de la sigmoide.
			*/
			BasicNeuron(int historicalMark, int historicalMark_inicial_input, int historicalMark_inicial_output, int layer);
			/**
				\brief Se crea una neurona nueva y a además se cargan los valores de las definiciones de usuario.
			*/
			BasicNeuron(std::string pathUserDefinitionsAboutBasicNeuron);
			/**
				\brief El constructor básico simplemente inicializa los valores de forma random.
			*/
			BasicNeuron();
			/**
				\brief Se borra de memoria toda la información que pudiera persistir.
			*/
			~BasicNeuron();
			/**
				\brief Bajo cierta probabilidad pueden cambiar características de esta neurona.
			*/
			void mutate();
			/**
				\brief Después de tener calculado el potencial de entrada (la suma de las conexiones entrantes) se puede calcular la salida de la neurona, posteriormente se baja a el potencial de entrada o bien podría ser que hayan neuronas que no lo hagan así sino que con expresiones matemáticas.
			*/ 
			double eval(); // Recordar que al hacer la evaluación se debe dejar en cero el potencial de entrada para la siguiente iteración.
			/**
				\brief Antes de evaluar la salida de la neurona es necesario sumar todas las entradas que provienen de conexiones con otras neuronas.
				\param incomingOneConnectionOutput Suma una de las entradas que provienen de otras neuronas a la entrada total de esta iteración.
			*/
			void sumIncomingConnectionsOutputs(double incomingOneConnectionOutput);
			/**
				\brief Toda neurona debe ser capas de guardar toda la información necesaria para poder volver a realizarla.
				\param pathToSave es la ruta a donde se quiere guardar la neurona.
			*/
			void saveState(std::string pathToSave);
			/**
				\brief Se carga una neurona a través de una anteriormente guardada.
				\param Es la ruta donde está la neurona guardada que se quiere cargar.
			*/
			void load(std::string PathWhereIsSaved);
			/**
				\brief se imprime toda la información de la neurona.
			*/
			void printState();
			/**
				\brief Se guardan las definiciones de usuario relativo a esta clase.
			*/
			void saveUserDefinitions(std::string pathToSave);
			/**
				\brief Se establece un valor a la variable historicalMark también entendida como innovación; es el valor que define estructuralmente a la neurona, dos neuronas con mismo historicalMark es porque fueron creadas en la misma posición de la red neuronal.
			*/
			void setHistoricalMark(int _historicalMark);
			/**
				\brief Se retorna el valor de la variable historicalMark
			*/
			int getHistoricalMark();
			/**
				\brief Se establece un valor a la variable historicalMark también entendida como innovación; es el valor que define estructuralmente a la neurona, dos neuronas con mismo historicalMark es porque fueron creadas en la misma posición de la red neuronal.
			*/
			void setLayer(int _historicalMark);
			/**
				\brief Se retorna el valor de la variable historicalMark
			*/
			int getLayer();
			/**
				\brief Se crea uno nuevo. Esto es necesario porque a priori desde la clase ANN no se sabe el tipo neurona es la que se está usando.
			*/
			Neuron * createNew(int historicalMark, int historicalMark_inicial_input, int historicalMark_inicial_output, int layer);
			/**
				\brief  Se obtiene la neurona que inicialmente fue la entrada a esta neurona.
				\return La marca histórica de la neurona que inicialmente fue entrada.
			*/
			int getInitialNeuronInHistoricalMark();
			/**
				\brief  Se obtiene la neurona que inicialmente fue la salida a esta neurona.
				\return La marca histórica de la neurona que inicialmente fue salida.
			*/
			int getInitialNeuronOutHistoricalMark();
			/**
				\brief Se obtiene un vector que apunta a todas las conexiones que inicial en esta neurona.
			*/
			std::vector <int> getOutcomingConnections();
			/**
				\brief Se obtiene un vector que apunta a todas las conexiones que inicial en esta neurona.
			*/
			std::vector <int> getIncomingConnections();
			/**
				\brief Se agrega la innovación de una nueva conexion que termina en ésta neurona.
			*/
			void addNewIncomingConection(int innovationOfIncomingConection);
			/**
				\brief Se agrega la innovación de una nueva conexion que empieza en ésta neurona.
			*/
			void addNewOutcomingConection(int innovationOfOutcomingConection);
			/*
				\brief Se obtiene el último valor que de voltaje que esta neurona obtuvo como salida.
			*/
			double getLastOutput();
			/**
				\brief Se duplica esta neurona obtieniendo una nueva neurona identica a esta, pero en otro espacio de memoria.
			*/
			Neuron * duplicate();
			/**
				\brief Se calcula la distancia (idea de NEAT) entre una neurona y otra del mismo tipo.
			*/
			double getDistance(Neuron * neuron);
			/**
				\brief Se cambian todos los valores de la neurona de forma aleatoria.
			*/
			void changeValuesRandomly();
		private:
			/**
				\brief Se obtienen los datos desde el archivo con definiciones y se guarda en las variables estáticas correspondietes. 
			*/
			void SetParametersFromUserDefinitionsPath(std::string pathUserDefinitionsAboutBasicNeuron);
			/**
				\brief Se inicializa una neurona con sus variables de forma random.
			*/
			void init();
		// ======================================================================================================
		// ===========================================   Variables ====================================================
		private:
			double lastOutputVoltage;
			int identificator;
			int layer; // La capa en la que está.
			int historicalMark; // También entendida como innovación es el valor que define estructuralmente a la neurona, dos neuronas con mismo historicalMark es porque fueron creadas en la misma posición de la red neuronal.
			int historicalMarkNeuronInicialIn;
			int historicalMarkNeuronInicialOut;
			std::vector <int> incomingConections;
			std::vector <int> outcomingConections;
			//========================================================
			// 	RELATIVO A LA FUNCIÓN SIGMOIDE QUE REPRESENTA A LA NEURONA.
			// 	-----------------------------------------------------------------------------------------------------------
			// La función a usar por la neurona es la sigmoide la cual se expresa de la siguiente forma:
			//	sigmoide( inputVoltage ) = 2 / ( 1 + exp( (inputVoltage+bias)*constanteSigmoid ) ) - 1 
			// Esta sigmoide tiene como destino el intervalo [-1,1] por eso la altereración a la normal que tiene [0,1]
			double inputVoltage;
			double sigmoidConstant; 
			double bias;
			//========================================================
			// Las siguientes son las variables que el usuario introduce para toda la clase. Las definiciones de usuario para todas las neuronas básicas.
			static double maxBias;
			static int id;
			// Recordar que por modelo de programación la probabilidad de que una neurona mute es parte de la estructura ANN (redes neuronales) por lo tanto aquí sólo se ven datos respecto a cómo efectuar esta mutación, no a la probabilidad misma de que mute. 
			static double maximumBiasVariationByMutation; // Esta variable corresponde a cuánto pondera el valor antiguo del bías en el calculo del valor actual.
			static double maximumSigmoidConstantVariationByMutation; // Esta variable corresponde a cuánto pondera el valor antiguo de la constante de la sigmoide en el calculo del valor actual.
			static double probabilityOfBiasRandomMutation; // Es la probabilidad de que se tenga un valor BIAS completamente random.  Esto se muy útil porque si no se tiene este tipo de mecanismos entonces es más diifícil que el valor del bias logre llegar a los extremos dado que la ponderancia tiende a que los datos estén en el centro. 
			static double probabilityOfSigmoidConstantRandomMutation; // Es la probabilidad de que cambie absolutamente. Mismo caso que el comentario de la variable anterior.
			static double ConstantDistanceOfBias;
			static double ConstantDistanceOfSigmoidConstant;
	};
}
#endif