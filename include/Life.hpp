#ifndef LIFE_HPP
#define LIFE_HPP	
/*
	La clase Life representa al manejo de todos las razas que se van creando a través del entrenamiento, es la estructura más grande que existe y es la que implementa los algoritmos de NEAT.
*/
#include "ANN.hpp"
#include "GlobalInformation.hpp"
#include "Niche.hpp"
#include "Neuron.hpp"
#include "SynapticWeight.hpp"
#include <string>

using namespace std;

namespace NEATSpikes{
	class Life{
	// Primero los métodos y después las variables.
	// ====================================================================================================
	// ============================================ Métodos  ==================================================
	public:
		Life();
		Life(std::string path_Life_definitions, std::string path_Niche_definitions, Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * information);
		/**
			\brief Se carga la vida a partir del lugar donde fue guardada anteriormete.
			\param PathWhereIsSaved es la ruta al archivo donde está guarada la vida que se desea cargar.
		*/
		Life(std::string PathWhereIsSaved);
		/**
			\brief Se borra de memoria todos los datos que pudieran persistir.
		*/
		~Life();
		/**
			\brief Epoch es el método motor del algoritmo NEAT en donde se pasa de generación, esto quiere decir que nace una nueva generación a partir de las anteriores y mueren algunos organismos de generaciones anteriores otros quedan vivos (se tiene una esperanza de vida), además nuevas razas pueden ser creadas haciendo una busqueda del optimo a través de diferentes estructuras neuronales. 
		*/
		void epoch();
		/**
			\brief El mejor organismo de todos los tiempos puede es guardado por Life y se puede llamar a él a través de este método.
			\return Se devuelve al organismo campeón de todos los tiempo, esto quiere decir el que tiene un mejor fitness dado que ha tenido el mejor desempeño en el experimento.
		*/
		Organism getChampion();
		/**
			\brief Se devuelve la cantidad de generaciones que han pasado.
			\return La cantidad de generaciones de entrenamiento.
		*/
		int getCurrentGeneration();

		/**
			\brief Se imprime la red neuronal por terminal.
		*/
		void printState();

		vector < Organism * > getAllNewOrganisms();
		/**
			\brief Se obtiene la cantidad de generaciones.
		*/
		int getGenerations();

	private:
				
		/**
			\brief Se guarda toda la información necesario para definir esta red neuronal y poder así volver a cargarla perfectamente si se desease volverla a entrenar.
		*/
		void saveState ( std::string pathToSave );
		/**
			\brief Se carga una red neuronal a través de una anteriormente guardada.
		*/
		void load(std::string PathWhereIsSaved);

		void setUserDefinitions(std::string PathWhereUserDefIsSaved);

	// =========================================================================================================
	// ===========================================   Variables =======================================================
	private:
		vector <Organism *> organismFIFOWaitingForCreateNewNiches;
		vector <Niche>  old_niches_vector;
		vector <Niche> new_niches_vector;
		static int generationsThatDefineAnOldNiche;
		static int amountOfOrganismInANewNiche;
		static int MaxAmountOfNewNiches;
		static int AmountOfTotalOrganismFromAllOldNiches;
		static double probabilityOfInterspaceMating;
		static int generations;
	};
}
#endif