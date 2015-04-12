#include "Niche.hpp"

namespace NEATSpikes
{
	double Niche::distanceThresshold=0.0;
	int Niche::organismLifeExpectation=0;
	int Niche::id=0;

	Niche::Niche()
	{
		identificator=id++;
		amountOfGenerationsAlive=0;	
		totalFitness=0;
	}
	Niche::Niche(std::string PathWhereIsSaved)
	{
		identificator=id++;
		amountOfGenerationsAlive=0;	
		totalFitness=0;
	}
	Niche::~Niche()
	{

	}
	Organism * Niche::obtainOneOrganismToCrossoverWithAnotherNicheOrganism()
	{
		double random = rand()/(double)RAND_MAX;
		double sum=0.0;
		for (unsigned int i = 0; i < organism_vector.size(); ++i)
		{
			sum += organism_vector.at(i)->getFitness()/totalFitness; // Se divide por totalFitness para normalizar.
			if(sum >= random)
			{
				return organism_vector.at(i);
			}
		}
		std::cerr << "ERROR::Niche::obtainOneOrganismToCrossoverWithAnotherNicheOrganism:: Could not find a organism" << std::endl;
		exit(EXIT_FAILURE);
	}




	Organism * Niche::obtainAChildren()
	{
		// Primero se encuentra al padre
		Organism * father=NULL;
		Organism * mother=NULL;
		unsigned int fatherPosition=-1;
		double random = rand()/(double)RAND_MAX;
		double sum=0.0;
		unsigned int i;

		// En caso de que no hayan más organismos en este nicho entonces se retorna el mismo organismo pero mutado.
		if(organism_vector.size() == 1)
		{
			ANN * children = new ANN(*organism_vector.at(0));
			children->mutate();
			return children;
		}
		// En caso de que no hayan organismos es porque puede ser que el nicho sea un nicho vacío en ese caso es un error porque no debería haber nichos vacíos que hayan sobrevivido hasta este momento o puede ser que sea un nicho nuevo en ese caso sólo hay organismos nuevos de donde obtener hijos.
		else if (organism_vector.size() == 0)
		{
			

			if (newOrganism_vector.size()==0)
			{
				std::cerr << "ERROR::Niche::obtainAChildren Niche have no organisms" << std::endl;
				exit(EXIT_FAILURE);
			}

			// Signfica que es un nicho nuevo, por lo tato tiene sólo organismos nuevos. Entonces se crea un nuevo organismo a través de éste.
			return newOrganism_vector.at(rand()%newOrganism_vector.size())->createOtherFromSeedOrganism();
		}


		// Una vez pasados los casos especiales queda encontrar al padre y a la madre.
		// Primero se encuentra al padre, para eso según el valor random obtenido se ontiene al padre además el fitness que posee cada organismo es proporcional a la posibilidad de ser el padre.
		
		for (i = 0; i < organism_vector.size(); ++i)
		{
			sum += organism_vector.at(i)->getFitness()/totalFitness; // Se divide por totalFitness para normalizar.
			if(sum >= random)
			{
				father = organism_vector.at(i);
				fatherPosition=i;
				break;
			}
		}
		if(i == organism_vector.size())
		{
			std::cerr << "ERROR::Niche::totalFitness is not correct" << std::endl;
			exit(EXIT_FAILURE);
		}


		// Ahora se encuentra a la madre
		double newTotalFitness = totalFitness- father->getFitness(); // Esto se calcula para que el próximo organismo electo (la madre) debe ser diferente al padre.
		random = rand()/(double)RAND_MAX; // Es importante obtener un nuevo valor random.

		sum=0.0;
		for (i = 0; i < organism_vector.size(); ++i)
		{
			if(i != fatherPosition) // Para que no pueda ser electo el padre.
			{
				sum += organism_vector.at(i)->getFitness()/newTotalFitness; // Se divide por totalFitness para normalizar.
				if(sum >= random)
				{
					mother = organism_vector.at(i);
					break;
				}
			}	
		}
		if( i == organism_vector.size() )
		{
			std::cerr << "ERROR::Niche::totalFitness is not correct" << std::endl;
			exit(EXIT_FAILURE);
		}
		ANN * children  = father->crossover( mother );
		children->mutate();

		return children;
	}

	void Niche::save(std::string pathToSave)
	{

	}

	void Niche::load(std::string PathWhereIsSaved)
	{

	}
	
	bool Niche::IsAcepted(Organism * orgm)
	{
		if (organismDistance(orgm,representant) < distanceThresshold)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void Niche::addOrganism(Organism * orgm)
	{
		newOrganism_vector.push_back(orgm);
	}

	void Niche::setUserDefinitions(std::string PathWhereIsSaved)
	{
		// Las variables que serán usadas en la función.
		FILE * archive; // El archivo es cargado en esta variable
		//Las siguientes variables son usadas para leer las lineas del archivo
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		// Las siguientes variables son usadas para obtener los valores de las lineas y guardarlo en el mapa UserDefinitions
		char * token_1; // Aqui se guardan los strings
		char * token_2; // y aquí el valor 
		char * saveptr; // variable para indicar el lugar donde quedó la ultima lectura del strtok_r
		std::map <std::string, double> UserDefinitions; 
		char delimiters[] = " \n\t"; // Los delimitadores usados.
		// Se abre el archivo donde están las definiciones de usuario.
		//=========================================================================================
		archive = fopen(PathWhereIsSaved.c_str(),"r");
		if (archive == NULL)
		{
			std::cerr << "ERROR::BasicNeuron::SetParametersFromUserDefinitionsPath::Could not read the file" << std::endl;
			exit(EXIT_FAILURE);
		}
		//=========================================================================================
		//Se lee linea a linea y se guarda en el mapa UserDefinitions
		//=========================================================================================
		while ((read = getline(&line, &len, archive)) != -1) 
		{
			token_1 = strtok_r(line, delimiters, &saveptr); // Por si acaso se desea hacer uso de hilos.
			token_2 = strtok_r(NULL, delimiters, &saveptr);
			UserDefinitions[token_1]=atof(token_2); // Se comienza a llenar el mapa.
		}
		fclose(archive); // Ya no se usa el recurso así que se cierra.
		//=========================================================================================
		// Ahora se le da el valor a las variables de usuario y se termina este método. Usando mapas se hace más sencillo y más robusto.
		//=========================================================================================
		organismLifeExpectation = (int)UserDefinitions["organismLifeExpectation"];
		distanceThresshold = UserDefinitions["distanceThresshold"];
		//=========================================================================================
	}
	Niche Niche::createNew(Organism * initialOrgm)
	{
		Niche * n = new Niche();
		n->champion = new ANN(*initialOrgm);
		n->representant=initialOrgm;
		n->newOrganism_vector.push_back(initialOrgm);
		return *n;

	}
	void Niche::maturation() // Se supone que todos los organismos nuevos son probados antes de hacer uso de este método
	{
		for (unsigned int i = 0; i < newOrganism_vector.size(); ++i)
		{
			double fitness =  newOrganism_vector.at(i)->getFitness();
			if(fitness > champion->getFitness())
			{
				delete(champion);
				champion = new ANN(*newOrganism_vector.at(i)); 
			}
			totalFitness += fitness;

			organism_vector.push_back(newOrganism_vector.at(i));
		}
		newOrganism_vector.clear();
		representant = organism_vector.at( rand()%organism_vector.size() );
	}

	void Niche::epoch() // Se supone que todos los organismos nuevos son probados antes de hacer uso de este método
	{
		for (unsigned int i = 0; i < organism_vector.size(); ++i)
		{
			double random = rand()/(double)RAND_MAX;
			if( random > organismLifeExpectation/(double)(1.0 + organismLifeExpectation) )
			{
				
				// Entonces el organismo muere.
				totalFitness-=organism_vector.at(i)->getFitness();
				delete(organism_vector.at(i));
				organism_vector.erase(organism_vector.begin()+i);
				i--;
			}
		}
		
		amountOfGenerationsAlive++;
	}
	
	Organism * Niche::getChampion()
	{
		return champion;
	}

	int Niche::getAmountOfOrganisms()  
	{
		return newOrganism_vector.size() + organism_vector.size();
	}
	int Niche::getAmountOfOldOrganisms()  
	{
		return organism_vector.size();
	}
	int Niche::getAmountOfNewOrganisms()  
	{
		return newOrganism_vector.size();
	}
	Niche Niche::createInitial(Organism * initialOrgm, std::string path_Niche_definitions)
	{
		Niche * n = new Niche();
		n->champion = new ANN(*initialOrgm);
		n->setUserDefinitions(path_Niche_definitions);
		n->representant=initialOrgm;
		n->newOrganism_vector.push_back(initialOrgm);
		return *n;
	}

	int Niche::getAge()
	{
		return amountOfGenerationsAlive;
	}

	double Niche::getTotalFitness()
	{
		return totalFitness;
	}
	void Niche::printState()
	{
		std::cout << "\t======================== Niche \t" << identificator << std::endl;
		std::cout << "\ttotalFitness\t" << totalFitness << std::endl;
		std::cout << "\taverageFitness\t" << getFitnessAverage() << std::endl;
		std::cout << "\tamount of organism\t" << getAmountOfOrganisms() << std::endl;

		std::cout << "\t======================== " << std::endl;
	}

	vector <Organism * > Niche::getOrganismVector()
	{
		return organism_vector;
	}
	vector <Organism * > Niche::getNewOrganismVector()
	{
		return newOrganism_vector;
	}

	double Niche::getFitnessAverage()
	{
		return getTotalFitness()/getAmountOfOldOrganisms();
	}
}