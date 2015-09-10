#ifndef NICHE_HPP
#define NICHE_HPP

#include "ANN.hpp"
#include <string>

using namespace std;

namespace NEATSpikes{
	class Niche{
	// Primero los métodos y después las variables.
	// ============================================================================================================
	// ============================================ Métodos ==========================================================
	public:
		Niche();
		/**	
			\brief Se carga el nicho a partir del lugar donde fue guardado anteriormete
		*/
		Niche(std::string PathWhereIsSaved);
		/**
			\brief Se borran todos los datos que pudieran quedar en la memoria.
		*/
		~Niche();
		/**
			\brief Se cra un nuevo organismo a través de un organismo de este nicho y uno de otro nicho. Para simplificar el proceso este método sólo devuelve un organismo de este nicho para que en la clase life se busque otro organismo de otro nicho y 

		*/
		Organism * obtainOneOrganismToCrossoverWithAnotherNicheOrganism(); 
		/**
			\brief Se cra un nuevo organismo a través de un organismo de este nicho y uno de otro nicho. Para simplificar el proceso este método sólo devuelve un organismo de este nicho para que en la clase life se busque otro organismo de otro nicho y 

		*/
		Organism * obtainAChildren(); 
		/**
			\brief Se imprime todo lo necesario para representar a este nicho completamente
		*/
		void printState();
		/**
			\brief Se guarda toda la información necesaria para volver a obtener este mismo nicho
		*/
		void save(std::string pathToSave);	
		/**
			\brief Se carga el nicho desde una ruta donde antes fue guardado un nicho.
		*/
		void load(std::string PathWhereIsSaved);
		/**
			\brief Se pregunta si el organismo puede pertenecer al nicho.
			\return Si es acpetado se devuelve un true, en otro caso se devuelve un false.
		*/
		bool IsAcepted(Organism * orgm);


		void setUserDefinitions(std::string PathWhereUserDefIsSaved);

		static Niche createNew(Organism * initialOrgm);
		static Niche createInitial(Organism * initialOrgm, std::string path_Niche_definitions);

		void maturation();

		void epoch();

		Organism * getChampion();

		int getAmountOfOrganisms();
		int getAmountOfNewOrganisms();
		int getAmountOfOldOrganisms();  


		void addOrganism(Organism * orgm);

		int getAge();

		double getTotalFitness();

		vector <Organism * > getOrganismVector();
		vector <Organism * > getNewOrganismVector();
		double getFitnessAverage();
	// ==========================================================================================================
	// ===========================================   Variables ========================================================
	private:
		vector <Organism *> organism_vector;
		vector <Organism *> newOrganism_vector;
		int amountOfOrganism;
		int amountOfGenerationsAlive;
		double totalFitness;
		Organism * representant; // Will be selected randomly each generation from organism_vector.
		Organism * champion; // De todos los tiempos. Se guarda con new.
		int identificator;
		

		double * distanceThresshold;
		int * organismLifeExpectation;
		static int id;
	};

}


#endif