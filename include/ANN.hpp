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
#include <tuple>
#include <map>
#include <iostream>
#include <cstring>
#include <algorithm>    // std::copy

using namespace std;

namespace NEATSpikes{
	class ANN{
	// Primero los métodos y después las variables.
	// ===================================================================================================
	// ============================================ MÉTODOS  ================================================
	public:
		/**
			\brief no hace nada, es por formalidad.
		*/
		ANN();
		/**
			\brief Constructor copia.
		*/
		ANN(const ANN & ann);
		/**
			\brief El constructor de ANN debe generar toda la red neuronal del organismo inicial. Se debe realizar al comienzo de todo el entrenamiento, lueog se hacen copias de ANN al cruzarse los organismos.
		*/
		ANN(Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * information);
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
		ANN * crossover(ANN * mother);
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
		ANN * createOtherFromSeedOrganism();
		/**
			\brief Se obtiene la cantidad de salidas del organismo.
		*/
		static int getOrganismOutputSize();
		/**
			\brief Se carga una red neuronal a través de una anteriormente guardada.
		*/
		void load(std::string PathWhereIsSaved);
	private:
		
		/**
			\brief Se guarda toda la información necesario para definir esta red neuronal y poder así volver a cargarla perfectamente si se desease volverla a entrenar.
		*/
		void saveState(std::string pathToSave);
		
		/**
			\brief Se obtienen los datos desde el archivo con definiciones y se guarda en las variables estáticas correspondietes. 
		*/
		void SetParametersFromUserDefinitionsPath(std::string pathUserDefinitionsAboutANN);
		/**
			\brief Se crea una nueva neurona en la estructura de la red neuronal.
		*/
		void newNeuronMutation();
		/**
			\brief Cada layer sabe qué nodos son parte de él, así después el método eval puede ser realizado de forma ordenada las capas son evaluadas desde las entradas hasta las salidas en orden.
		*/
		void addNeuronToLayer(int layer, int historicalMark);
		/*
			\brief Se agrega una conexión sináptica a la red neurnal con todos los valores ya establecidos. 
		*/
		void addSynapticWeight(SynapticWeight * SW);
		/*
			\brief Se agrega una conexión sináptica a la red neurnal con todos los valores ya establecidos. 
		*/
		void addSynapticWeight(int historicalNeuronIn, int historicalNeuronOut);
		/**
			\brief Se agrega una neurona a la red neurnal con todos los valores ya establecidos. 
		*/
		void addNeuron(Neuron * N);
		/**
			\brief Se agrega una neurona a la red neurnal con todos los valores ya establecidos. 
		*/
		void addNeuron( int histoticalMarkNeuronIn, int histoticalMarkNeuronOut );
		/**
			\brief 
		*/	
		void addInitialStructureNeuron( Neuron * N );
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
		//ENCONTRAR UN MEJOR NOMBRE!!!!
		std::tuple < int,int >  localNeuronsInOut_TO_referencesNeurons(std:: vector<int> neuron_in_out); // Es muy complicado el nombre pero
		/**
			\brief 
		*/	
			//ENCONTRAR UN MEJOR NOMBRE!!!!
		std::tuple < int,int >  referencesNeurons_TO_localNeuronsInOut(std:: vector<int> vect_1_vect_2); // Es muy complicado el nombre pero
		/**
			\brief Se encuentran las posibles neuronas entre las cuales se generara la mutacion.
		*/
		std::tuple < int, int, int, int, int, int > findNeuronsForNewNeuronMutation();
		/**
			\brief Se encuentran las posibles neuronas entre las cuales se generara la mutacion.
		*/
		std::tuple < int, int > findNeuronsForNewSynapticMutation();

	// =====================================================================================================
	// ===========================================   VARIABLES  ==================================================
	private:
		
		int identificator;
		// ======RELATIVO A LA MUTACION PARA CREAR NUEVAS NEURONAS O CONEXIONES SINÁPTICAS===
			std::vector < std::vector <int> > neuronsReferencesForCreateNewNeurons; // Cada neurona creada tiene como referencia 2 neuronas a las cuales inicialmente unió, esto es necesario para luego adquirir una marca histórica. Ver capitulo relativo a mutaciones en el manual.
			std::vector <int> availableNumberOfNeuronMutationsInRelationToNeuron; // Esta variable posee todas cantidad de mutaciones disponibles que tengan relación con la neurona X, siendo X la entrada (o sea .at(X)), así al crear una nueva neurona se crea una nueva entrada a este vector con todas las cantidad de posibles mutaciones que se pueden tener con ésta, y al hacer una mutacion que la involucre encontes se baja en 1 la cantidad.
			std::vector < std::vector <int> > neuronsReferencesForCreateNewSynapticWeight;
			std::vector <int> availableNumberOfSynaptinWeightMutationsInRelationToNeuron;


			int amountOfPosiblyNeuronMutation;
			int amountOfPosiblySynapticWeightMutation;

			int amountOfNeurons;

			std::map <int, int> historicalMark_To_localNeuron; 
			std::map <int, int> innovarion_to_localSynapticWeightVectorPosition; 
			double fitness;
		//============================================================


		//=======================================================
		// 					RELATIVO A EVAL()
			std::vector < std::vector< int > > neuronsAtLayer; // Cada layer tiene una lista con las marcas históricas de las neuronas que pertenecen a aquel layer.
		//=======================================================

		std::vector <int> inputsInNeuron_vector;
		std::vector <int> outputsInNeuron_vector;
		std::vector < Neuron * > neuron_vector;
		std::vector <SynapticWeight * > synapticWeight_vector;
		GlobalInformation * generalInformation;
		

		Neuron * prototypeNeuron;
		SynapticWeight *  prototypeSynapticWeight;



		static int id; // Se usará para poder guardar las ANN de forma ordenada si se quisieran volver a cargar de nuevo.
		// ============== DEFINICIONES DE USUARIO =======================================
		static double probabilityOfSynapticWeightMutation; // Es para cada conexión sináptica existente en la red neuronal.
		static double probabilityOfNeuronMutation; // Es para cada neurona existente en la red neuonal.
		static double probabilityOfNewNeuronMutation; // Es la probabilidad de que por mutación se cree una nueva neurona
		static double probabilityOfNewSynapticWeightMutation; // Es la probabilidad de que por mutación se cree una nueva conexión sináptica
		static int inputsAmount; // Necesario para crear a la primera generación.
		static int outputsAmount;// Necesario para crear a la primera generación.
		static double ConstantOFDiferencesInStructureOfSynapticWeight;
		static double ConstantOFDiferencesInStructureOfNeurons;
		static bool ANNCanHaveConnectionsBack;
	};

	typedef ANN Organism;

}
#endif