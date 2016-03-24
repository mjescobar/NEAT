#ifndef BASICSYNAPTICWEIGHT_HPP
#define BASICSYNAPTICWEIGHT_HPP

#include <cstdlib> // RAND_MAX, EXIT_FAILURE, srand, rand 
#include <map> // Para usar map.
#include <cstring> // strtok_r
#include <iostream> // cout, cerr, cin, clog
#include <stdio.h> // getline
#include <string> 
#include <fstream> // ofstream
#include "SynapticWeight.hpp"
#include <cmath>
#include "GlobalInformation.hpp"

namespace NEATSpikes{
	class BasicSynapticWeight: public SynapticWeight
	{
	// Primero los métodos y después las variables.
	// ==========================================================================================================================
	// ============================================ Métodos ========================================================================
	public:
		BasicSynapticWeight(SynapticWeight * prototype, int histoticalMark_Neuron_in , int histoticalMark_Neuron_out);	
		BasicSynapticWeight(SynapticWeight * prototype, int innovation, int histoticalMark_Neuron_in , int histoticalMark_Neuron_out);	
		/**
			\brief Se obtienen todos los parámetros necesarios a partir del archivo de definiciones de usuarios y se inicializa el peso con un valor random.
		*/
		BasicSynapticWeight(std::string pathUserDefinitionsAboutBasicSynapticWeight, GlobalInformation * Info);
		/**
			\brief Este constructor no hará  más que inicializar el peso sináptico porque se supone que las definiciones de usuario fueron ya guardadas en variables static.
		*/
		BasicSynapticWeight();
		/**
			\brief El constructor copia es usado simplemente para copiar el peso sináptico.
		*/
		BasicSynapticWeight(const BasicSynapticWeight & BSW);
		/**
			\brief No hay ninguna información que pudiera ser persistente así que es un método vacío.
		*/
		~BasicSynapticWeight();
		/**
			\brief Bajo cierta probabilidad pueden cambiar características de esta neurona.
		*/
		void mutate();
		/**
			\brief Se obtiene la salida de la conexión sináptica. Para eso se le debe entregar un valor de entrada.
		*/
		double eval(double incomingVoltage);	// Se aplica la identidad
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
			\brief se imprime toda la información de la conexión sináptica.
		*/
		void printState();
		/**
			\brief Se crea uno nuevo. Esto es necesario porque a priori desde la clase ANN no se sabe el tipo de conexión sináptica que se está usando.
		*/
		SynapticWeight * createNew(SynapticWeight * prototype, int histoticalMark_Neuron_in , int histoticalMark_Neuron_out);
		/**
			\brief Se guardan las definiciones de usuario relativo a esta clase.
		*/
		void saveUserDefinitions(std::string pathToSave);
		/**
			\brief Se establece valor para Innovation, innovation: Es el valor que representa la identificación de esta conexión sináptica respecto a la estructura de la red neuronal. Dos conexiones sinápticas con el mismo valor de innovation significa que conectan las mismas neuronas o entradas.
		*/
		void setInnovation(int _innovation);
		/**
			\brief Se obtiene el valor de innovation. innovation: Es el valor que representa la identificación de esta conexión sináptica respecto a la estructura de la red neuronal. Dos conexiones sinápticas con el mismo valor de innovation significa que conectan las mismas neuronas o entradas.
		*/
		int getInnovation();
		/**
			/brief Se deshabilita la conexión sináptica.
		*/
		void disable();
		/**
			\brief Se obtiene la marca histórica de la neurona que entra a esta conexión sináptica.
		*/
		int getHistoricalMarkOfNeuronIn();
		/**
			\brief Se obtiene la marca histórica de la neurona a la que entra esta conexión sináptica.
		*/
		int getHistoricalMarkOfNeuronOut();
		/**
			\brief Se obtiene la salida de la conexión sináptica.
		*/
		double getOutput();
		/**
			\brief Se deja configurada la entrada a esta conexión sináptica.
		*/
		void setInput(double input);
		/**
			\brief Se pasa la entrada a la salida.
		*/
		void spread();
		/**
			\brief Se duplica la conexión sináptica obteniendo otra similar a esta.
		*/
		SynapticWeight * duplicate();
		/**
			\brief Se obtiene la distancia según las diferencias en los pesos sinápticos de dos conexiones sinápticas.
		*/
		double getDistance(SynapticWeight * syn);
		/**
			\brief se ocambia el peso de forma aleatoria.
		*/
		void changeValuesRandomly();
		/**
			\brief Se guarda el valor de Id en disco duro
		*/
		void saveId(std::string pathToSave);
		/**
			\brief Se carga de disco duro el valor de Id.
		*/
		void loadId(std::string PathWhereIsSaved);
		/**
			\brief Se copian los valores relevantes de una coneccion sinaptica, esto ese realiza cuando ya teniendo una coneccion sinaptica se desea cambiar sus valores internos por los de otra coneccion sinaptica.
		*/
		void copyValues(SynapticWeight * sw);

		bool getEnable();
		/**
			\brief Se obtienen los datos desde el archivo con definiciones y se guarda en las variables estáticas correspondietes. 
		*/
		void SetParametersFromUserDefinitionsPath(std::string pathUserDefinitionsAboutBasicSynapticWeight);
	private:
		/**
			\brief Dado que no es 100% compatible llamar a un constructor desde otro constructor se crea el método init() el cual inicializa el peso sináptico.
		*/
		void init();
		
		/**
			\brief Se cargan los parametros de usuario a través de otra conexion sinaptica prototupo.
		*/
		void loadParametersFromPrototype(SynapticWeight * prototype);
	// =========================================================================================================================
	// ==========================================   Variables ========================================================================
		double output;
		double input;
		double synapticWeightValue;
		int identificator;
		int innovation; // Es el valor que representa la identificación de esta conexión sináptica respecto a la estructura de la red neuronal. Dos conexiones sinápticas con el mismo valor de innovation significa que conectan las mismas neuronas o entradas.
		bool enable;
		int historicalMarkOfNeuronIn;
		int historicalMarkOfNeuronOut;
		GlobalInformation * information; 

		// Recordar que, por modelo de programación, la probabilidad de que una conexión sináptica mute es parte de la estructura ANN (redes neuronales) por lo tanto aquí sólo se ven datos respecto a cómo efectuar esta mutación, no a la probabilidad misma de que mute. 
		int * id;
		double * maxWeightValue; //Maximo valor que puede tomar el peso sinamptico
		double * minWeightValue; //Minimo valor que puede tomar el peso sinamptico
		double * maximumWeightVariationByMutation; //Notar: que el valor debe ser descrito en el intervalo [0  1]. Existirá una máxima variación del peso sináptico permitido por mutación, o sea: peso nuevo = peso antiguo*(1-maximumWeightVariationByMutation) + random/RAND_MAX * maximumWeightVariationByMutation.  
		double * probabilityOfWeightRandomMutation; // Es la probabilidad de que se tenga un valor de peso sináptico completamente random.  Esto se muy útil porque si no se tiene este tipo de mecanismos entonces es más diifícil que el valor del bias logre llegar a los extremos dado que la ponderancia tiende a que los datos estén en el centro. 
		double * probabilityOfEnableADisabledConnection;
		double * ConstantDistanceOfSynapticWeightValue;
	};
}
#endif