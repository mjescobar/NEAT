#include "Life.hpp"


namespace NEATSpikes
{
	int Life::generationsThatDefineAnOldNiche=0;
	int Life::amountOfOrganismInANewNiche=0;
	int Life::MaxAmountOfNewNiches=0;
	int Life::AmountOfTotalOrganismFromAllOldNiches=0;
	double Life::probabilityOfInterspaceMating=0.0;
	int Life::generations=0;

	Life::Life(std::string path_Life_definitions, std::string path_Niche_definitions, Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * information)
	{
		setUserDefinitions(path_Life_definitions);
		ANN * seedANN = new ANN(neuron, synapticWeight, path_ANN_definitions, information );
		new_niches_vector.push_back( Niche::createInitial(seedANN, path_Niche_definitions) );

		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			
			while( new_niches_vector.at(i).getAmountOfNewOrganisms() < amountOfOrganismInANewNiche )
			{
				
				Organism * newOrganism = seedANN->createOtherFromSeedOrganism();	
				
				if( new_niches_vector.at(i).IsAcepted(newOrganism) )
				{ // entonces es aceptado con el nicho padre.
					new_niches_vector.at(i).addOrganism(newOrganism);
					continue;
				}
				else
				{
					unsigned int j ;

					for (j = 0; j < new_niches_vector.size(); ++j)
					{
						if( new_niches_vector.at(j).IsAcepted(newOrganism) )
						{
							if( new_niches_vector.at(j).getAmountOfNewOrganisms() < amountOfOrganismInANewNiche )
							{
								new_niches_vector.at(j).addOrganism(newOrganism);
								break;
							}
							else
							{
								delete newOrganism;
								break;
							}
						}
					}

					if( j == new_niches_vector.size() )
					{
						// Si se llega a este punto es porque no fue aceptado en ningun otro nicho entonces debe crear uno propio.
						if( (int) new_niches_vector.size() < MaxAmountOfNewNiches)
						{
							new_niches_vector.push_back(Niche::createNew(newOrganism));
						}
						else
						{
							organismFIFOWaitingForCreateNewNiches.push_back(newOrganism);
						}
					}
				}
			}
		}
	}

	// Es el método principal del algoritmo NEAT

	void Life::epoch()
	{
		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			new_niches_vector.at(i).maturation();
		}
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			old_niches_vector.at(i).maturation();
		}

		int totalOfChildrenInOldNiches = 0;


		printState( );
		


		// Ahora comenzamos con los nichos viejos que se tienen que peliar la posibilidad de tener hijos.
		double TotalFitnessAverageOfAllOldNiches = 0.0;
		
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			TotalFitnessAverageOfAllOldNiches += old_niches_vector.at( i ).getFitnessAverage();
		}

		while(totalOfChildrenInOldNiches < AmountOfTotalOrganismFromAllOldNiches && old_niches_vector.size() >= 1)
		{
			if( rand()/(double)RAND_MAX  < probabilityOfInterspaceMating && old_niches_vector.size() > 1 )
			{ // Este caso será absolutamente random.

				int niche1 = rand()%old_niches_vector.size();
				int niche2 = rand()%(old_niches_vector.size()-1);
				if (niche2>= niche1)
				{
					niche2++;
				}
				Organism * father = old_niches_vector.at( niche1 ).obtainOneOrganismToCrossoverWithAnotherNicheOrganism();
				Organism * mother = old_niches_vector.at( niche2 ).obtainOneOrganismToCrossoverWithAnotherNicheOrganism();

				Organism * children = crossover( father , mother );
				if( old_niches_vector.at(niche1).IsAcepted( children ) )
				{ // entonces es aceptado con el nicho padre.
					old_niches_vector.at(niche1).addOrganism( children );
				}
				else if( old_niches_vector.at(niche2).IsAcepted( children ) )
				{ // entonces es aceptado con el nicho madre.
					old_niches_vector.at(niche2).addOrganism( children );
				}
				else
				{
					// No se revizará en los nuevos nichos porque están
					unsigned int j ;
					
					for (j = 0; j < old_niches_vector.size(); ++j)
					{
						if( old_niches_vector.at(j).IsAcepted( children ) )
						{
							totalOfChildrenInOldNiches++;
							old_niches_vector.at(j).addOrganism( children );
							break;
						}
					}
					
					if(j == old_niches_vector.size())
					{
						// Si se llega a este punto es porque no fue aceptado en ningun otro nicho entonces debe crear uno propio.
						organismFIFOWaitingForCreateNewNiches.push_back( children );
					}
				}
			}
			else 
			{ // Este es el caso típico, donde todos los nichos se "pelean" la opción de tener un hijo. 
				double random = rand()/(double)RAND_MAX;
				double sum=0.0;
				Organism * children;
				unsigned int i ;
				for (i = 0; i < old_niches_vector.size(); ++i)
				{
					sum += old_niches_vector.at(i).getFitnessAverage()/TotalFitnessAverageOfAllOldNiches; // Se divide por totalFitness para normalizar.
					if(sum >= random)
					{
						children = old_niches_vector.at(i).obtainAChildren();
						break;
					}
				}
				if(old_niches_vector.at(i).IsAcepted(children) ) // Primero se prueba en su propio nicho.
				{ // entonces es aceptado con el nicho padre.
					old_niches_vector.at(i).addOrganism(children);
					totalOfChildrenInOldNiches++;
				}
				else
				{
					// No se revizará en los nuevos nichos porque están
					unsigned int j ;
					
					for (j = 0; j < old_niches_vector.size(); ++j)
					{
						if( old_niches_vector.at(j).IsAcepted(children) )
						{
							totalOfChildrenInOldNiches++;
							old_niches_vector.at(j).addOrganism(children);
							break;
						}
					}
					if(j == old_niches_vector.size())
					{
						// Si se llega a este punto es porque no fue aceptado en ningun otro nicho entonces debe crear uno propio.
						organismFIFOWaitingForCreateNewNiches.push_back(children);
					}
				}
			}
		}


		// Si hay espacio para hacer un nuevo nicho y hay organismos esperando para hacer su nicho en la lista fifo entonces se hacen nuevos nichos.
		while(organismFIFOWaitingForCreateNewNiches.size()>=1 && (int)new_niches_vector.size() < MaxAmountOfNewNiches )
		{
			new_niches_vector.push_back( Niche::createNew( organismFIFOWaitingForCreateNewNiches.front() ) );
			organismFIFOWaitingForCreateNewNiches.erase(organismFIFOWaitingForCreateNewNiches.begin());
		}

		// finalmente se llenan los nuevos nichos


		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{

			while( new_niches_vector.at(i).getAmountOfNewOrganisms() < amountOfOrganismInANewNiche )
			{
				Organism * newOrganism = new_niches_vector.at(i).obtainAChildren();	
				if( new_niches_vector.at(i).IsAcepted(newOrganism) )
				{ // entonces es aceptado con el nicho padre.
					new_niches_vector.at(i).addOrganism(newOrganism);
					continue;
				}
				else
				{
					unsigned int j ;
					for (j = 0; j < new_niches_vector.size(); ++j)
					{
						if( new_niches_vector.at(j).IsAcepted(newOrganism) )
						{
							if( new_niches_vector.at(j).getAmountOfNewOrganisms() < amountOfOrganismInANewNiche )
							{
								new_niches_vector.at(j).addOrganism(newOrganism);
								break;
							}
							else
							{
								delete newOrganism;
								break;
							}
						}
					}
					if(j == new_niches_vector.size())
					{
						// Si se llega a este punto es porque no fue aceptado en ningun otro nicho entonces debe crear uno propio.
						if((int)new_niches_vector.size() < MaxAmountOfNewNiches && organismFIFOWaitingForCreateNewNiches.size()==0)
						{
							new_niches_vector.push_back(Niche::createNew(newOrganism));
						}
						else
						{
							organismFIFOWaitingForCreateNewNiches.push_back(newOrganism);
						}
					}
				}
			}
		}



		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			new_niches_vector.at(i).epoch();
			if( new_niches_vector.at(i).getAge() >= generationsThatDefineAnOldNiche )
			{ // Entonces debe ser pasado a los nichos viejos.
				old_niches_vector.push_back(new_niches_vector.at(i));
				new_niches_vector.erase(new_niches_vector.begin()+i);
				i--;
			}
		}
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			old_niches_vector.at(i).epoch();
		}


		// Los nichos sin miembros nuevos ni miembros antiguos  son eliminados.

		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			if(old_niches_vector.at(i).getAmountOfNewOrganisms() == 0 &&  old_niches_vector.at(i).getAmountOfOldOrganisms()==0)
			{
				old_niches_vector.erase(old_niches_vector.begin()+i);
				i--;
			}
		}

	}

	void Life::setUserDefinitions(std::string PathWhereUserDefIsSaved)
	{
		// Las variables que serán usadas en la función.
		FILE * archive; // El archivo es cargado enold_niches_vector esta variable
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
		archive = fopen(PathWhereUserDefIsSaved.c_str(),"r");
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
		generationsThatDefineAnOldNiche = (int) UserDefinitions [ "generationsThatDefineAnOldNiche" ];
		amountOfOrganismInANewNiche = (int) UserDefinitions [ "amountOfOrganismInANewNiche" ];
		MaxAmountOfNewNiches = (int) UserDefinitions [ "MaxAmountOfNewNiches" ];
		AmountOfTotalOrganismFromAllOldNiches = (int) UserDefinitions [ "AmountOfTotalOrganismFromAllOldNiches" ];
		probabilityOfInterspaceMating = UserDefinitions ["probabilityOfInterspaceMating"];
		generations = (int)UserDefinitions [ "generations" ];
		//=========================================================================================
	}
	Life::~Life()
	{

	}
	void Life::printState()
	{
		std::cout << "=============================User Defs=========================================" << std::endl; 
		std::cout << "\t generationsThatDefineAnOldNiche\t" << generationsThatDefineAnOldNiche << std::endl;
		std::cout << "\t amountOfOrganismInANewNiche\t" << amountOfOrganismInANewNiche << std::endl;
		std::cout << "\t MaxAmountOfNewNiches\t" << MaxAmountOfNewNiches << std::endl;
		std::cout << "\t AmountOfTotalOrganismFromAllOldNiches\t" << AmountOfTotalOrganismFromAllOldNiches << std::endl;
		std::cout << "\t probabilityOfInterspaceMating\t" << probabilityOfInterspaceMating << std::endl;

		std::cout << "=============================New Niches=========================================" << std::endl; 		
		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			new_niches_vector.at(i).printState();
		}

		std::cout << "=============================Old Niches=========================================" << std::endl; 		
		
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			old_niches_vector.at(i).printState();
		}
	
	}

	vector < Organism * >  Life::getAllNewOrganisms()
	{
		vector < Organism * >  final;
		


		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			vector < Organism * >  newOrgm = new_niches_vector.at(i).getNewOrganismVector();
			final.insert(final.begin(), newOrgm.begin(), newOrgm.end() );
		}
		 for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			vector < Organism * >  newOrgm = old_niches_vector.at(i).getNewOrganismVector();
			final.insert(final.begin(), newOrgm.begin(), newOrgm.end() );
		}

		return final;
	}

	int Life::getGenerations()
	{
		return generations;
	}
}

