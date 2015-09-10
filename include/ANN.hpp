#ifndef ANN_HPP
#define ANN_HPP	



/*
	En esta clase se desarrollan todos los metodos necesarios para calcular la salida de la red neuronal a través de las entradas que tenga en el tiempo.

	Además esta clase se encarga de administrar las mutaciones a cada neurona y a cada conexión sináptica.
*/
#include <vector>
#include "Neuron.hpp"
#include "SynapticWeight.hpp"
#include "GlobalInformation.hpp"
#include "Input.hpp"
#include "MutationControl.hpp"
#include <tuple>
#include <map>
#include <iostream>
#include <cstring>
#include <algorithm>    // std::copy
#include <sys/stat.h>
#include <fstream>

using namespace std;

namespace NEATSpikes
{
	class ANN
	{
	// Primero los métodos y después las variables.
	// ===================================================================================================
	// ============================================ MÉTODOS  ================================================
	public:
		/**
			\brief no hace nada, es por formalidad.
		*/
		ANN();
		/**
			\brief Se usa para crear nuevos organismos.
		*/
		ANN( GlobalInformation * information );
		/**
			\brief Constructor copia.
		*/
		ANN(ANN * ann);
		/**
			\brief El constructor de ANN debe generar toda la red neuronal del organismo inicial. Se debe realizar al comienzo de todo el entrenamiento, lueog se hacen copias de ANN al cruzarse los organismos.
		*/
		ANN(Neuron * inputPrototype, Neuron * outputPrototype, Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * globalInformation);

		/**
			\brief Se carga una ANN a partir del lugar donde fue guardada anteriormete
		*/
		ANN(std::string PathWhereIsSaved);
		/*	
			Se debe eliminar todo lo que pudiera quedar en memoria.
		*/
		~ANN();
		/**
			\brief Se imprime la red neuronal por terminal. Se imprimen primero los datos que son parte sólo del organismo luego se imprimen todas sus neuronas y finalmente todas las conexiones sinápticas.
		*/
		void printState();
		/**
			\brief Se hace el calculo de todas las redes  de neuronas y se pasa una iteración de tiempo. Está orientado a las conexiones sinápticas, eso quiere decir que  el método tomará en cuenta las salidas de las conexiones sinápticas y en caso de que éstas tengas tegan delays entonces se respetarán.
		*/
		vector <double> eval ( vector <double> inputs );
		/**
			\brief Debe incidar a cada  neurona y conexión sináptica que deben mutar. 
		*/
		void mutate();
		/**
			\brief Este método se encarga de cruzar dos redes neuronales para producir una nueva en memoria con las características de ambas anteriores.
		*/
		friend ANN * crossover(ANN * father, ANN * mother);
		/**
			\brief Se imprime la ANN de forma alternativa, se imprime sólo el neuronsReferencesForCreateNewNeurons el cuál muestra información importante de la neurona.
		*/
		void testPrint();
		/**
			\brief Se obtiene el fitness que tiene asignado el organismo.
		*/
		double getFitness();
		/**
			\brief Se la asigna un valor fitness al organismo. Este método se debe utilizar en el experimento.
		*/
		void setFitness(double fitness);
		/**
			\brief Se crea otra red neuronal con la misma estructura pero con todos los valores aleatorios.
		*/
		ANN * createNewWithSameTopologieButDiferentValues();
		/**
			\brief Se obtiene la cantidad de salidas del organismo.
		*/
		int getOrganismOutputSize();
		/**
			\brief Se carga una red neuronal a través de una anteriormente guardada.
		*/
		void load(std::string PathWhereIsSaved, Neuron * inputPrototype, Neuron * outputPrototype, Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * globalInformation );
		/**
			\brief Se crea una copia con sus variables en su propia memoria.
		*/
		ANN * duplicate();
		/**
			\brief Se guarda toda la información necesario para definir esta red neuronal y poder así volver a cargarla perfectamente si se desease volverla a entrenar.
		*/
		void saveState(std::string pathToSave);
		/**
			\brief Pasa una epoca y se debe saber si este organismo muere o no muere. 
			\return Si el organismo debe morir entonces retorna false, sino true.
		*/
		bool epoch();

		void saveUserDefinitions(std::string pathToSave);
	private:
		
		/**
			\brief Se obtienen los datos desde el archivo con definiciones y se guarda en las variables estáticas correspondietes. 
		*/
		void SetParametersFromUserDefinitionsPath(std::string pathUserDefinitionsAboutANN);
		/**
			\brief Se crea una nueva neurona en la estructura de la red neuronal.
		*/
		void newNeuronMutation();
		/*
			\brief Se agrega una conexión sináptica a la red neurnal con todos los valores ya establecidos. 
		*/
		void addSynapticWeight( SynapticWeight * SW );
		
		/**
			\brief Se agrega una neurona a la red neurnal con todos los valores ya establecidos. 
		*/
		void addNeuron( Neuron * N );
		/**
			\brief 
		*/
		void createInitialANN();
		/**
			\brief 
		*/
		void newSynapticWeightMutation();
		/**
			\brief 
		*/
		void updatePresentList(int layersAmount);
		/**
			\brief 
		*/
		friend double organismDistance(ANN * ann1, ANN * ann2);
		/*
			\brief Se propagan los voltajes a través de toda la red neuronal, desde la capa de entradas hasta la capa de las salidas (en caso normal donde la capa de las salidas es la última)
		*/
		void spreadVoltage();
		/*
			\brief
		*/
		std::tuple < int,int >  localNeuronsToReferencesNeurons(std:: vector<int> neuron_in_out); // Es muy complicado el nombre pero
		/**
			\brief 
		*/	
		std::tuple < int,int >  referencesNeuronsToLocalNeurons(std:: vector<int> vect_1_vect_2); // Es muy complicado el nombre pero
		/**
			\brief Se encuentran las posibles neuronas entre las cuales se generara la mutacion.
		*/
		std::tuple < int, int, int, int, int, int > findNeuronsForNewNeuronMutation();
		/**
			\brief Se encuentran las posibles neuronas entre las cuales se generara la mutacion.
		*/
		std::tuple < int, int > findNeuronsForNewSynapticMutation();
		/**
			\brief Se obtienen los parametros de usuario y globalinformation a traves de los que el prototipo tiene.
		*/
		void loadParametersFromPrototype(ANN * prototype);
		
	// ==============================================================================================
	// ==============================   VARIABLES  ==================================================
	private:
		double fitness; // Indicador del comportamiendo de la red neuronal, entre mejor realice el experimento mayor fitness debe tener.
		int identificator; // Es unico para cada red neuronal.

		//============================================================
		// ======RELATIVO A LA MUTACION PARA CREAR NUEVAS NEURONAS O CONEXIONES SINÁPTICAS===
			std::vector < int > historicalMarkToNeuron; 
			std::vector < int > innovationToSynapticWeight; 
			MutationControl * mutationControl;	
		//============================================================


		//=======================================================
		// 					RELATIVO A EVAL()
			std::vector < std::vector < int > > historicalMarkAtLayer; // Cada layer tiene una lista con las marcas históricas de las neuronas que pertenecen a aquel layer.
		//=======================================================

		std::vector < int > inputsInNeuron_vector; // Is a map to inputs in neuron vector
		std::vector < int > outputsInNeuron_vector; // Is a map to outputs in neuron vector
		std::vector < Neuron * > neuron_vector;
		std::vector < SynapticWeight * > synapticWeight_vector;
		GlobalInformation * globalInformation;


		Neuron * prototypeNeuron; // El prototipo de neurona corresponde a la neurona con la que se construiran el resto de las neuronas de la red, corresponden a las que se crearan por mutaciones.
		SynapticWeight *  prototypeSynapticWeight;

		Neuron * inputPrototype; // Este es el prototipo de neurona que sera usada como Input.
		Neuron * outputPrototype; // Este es el prototipo de neurona que sera usada como Output.



		static int id; // Se usara para poder guardar las ANN de forma ordenada si se quisieran volver a cargar de nuevo.
		// ============== DEFINICIONES DE USUARIO =======================================
		double * probabilityOfSynapticWeightMutation; // Es para cada conexión sináptica existente en la red neuronal.
		double * probabilityOfNeuronMutation; // Es para cada neurona existente en la red neuonal.
		double * probabilityOfNewNeuronMutation; // Es la probabilidad de que por mutación se cree una nueva neurona
		double * probabilityOfNewSynapticWeightMutation; // Es la probabilidad de que por mutación se cree una nueva conexión sináptica
		int * inputsAmount; // Necesario para crear a la primera generación.
		int * outputsAmount;// Necesario para crear a la primera generación.
		double * ConstantOFDiferencesInStructureOfSynapticWeight;
		double * ConstantOFDiferencesInStructureOfNeurons;
		bool * ANNCanHaveConnectionsBack;
		double * organismLifeExpectation;
		int age;
	};

	typedef ANN Organism;
}
#endif