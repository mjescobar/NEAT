#include "Life.hpp"


namespace NEATSpikes
{
	int Life::generationsThatDefineAnOldNiche=0;
	int Life::amountOfOrganismInANewNiche=0;
	int Life::MaxAmountOfNewNiches=0;
	int Life::AmountOfTotalOrganismFromAllOldNiches=0;
	double Life::probabilityOfInterspaceMating=0.0;
	int Life::generations=0;

	Life::Life(std::string path_Life_definitions, std::string path_Niche_definitions, Neuron * neuronInputPrototype,Neuron * neuronOutputPrototype,  Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * globalInformation)
	{

		setUserDefinitions(path_Life_definitions);

		// Se crea el primer organismo.
		ANN * seedANN = new ANN( neuronInputPrototype, neuronOutputPrototype, neuron, synapticWeight, path_ANN_definitions,globalInformation);
		// Se crea el primer nicho a traves del primer organismo
		Niche * initialNiche = new Niche(seedANN, path_Niche_definitions);

		new_niches_vector.push_back( initialNiche );

		//Dado que los nichos nuevos tienen un numero constante de organismos entonces se hace obligatorio iniciarlos asi tambien.
		//El primer loop es dinamico dado que new_niches_vector puede ir variando de largo mientras se van creando los nuevos hijos.
		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			//mientras el numero de organismos del nicho no alcance el numero constante de organismos en un nicho nuevo se deben crear hijos.
			while( new_niches_vector.at(i)->getAmountOfNewOrganisms() < amountOfOrganismInANewNiche )
			{
				//Se crea un nuevo organismo con las mismas caracteristicas del organismo semilla creado al inicio de este metodo
				Organism * newOrganism = seedANN->createNewWithSameTopologieButDiferentValues();	
				
				//Se verificara si el organismo pertenece a la misma raza que su padre.
				if( new_niches_vector.at(i)->IsAcepted(newOrganism) )
				{ // entonces es aceptado con el nicho padre.
					new_niches_vector.at(i)->addOrganism(newOrganism);
					continue;
				}
				else
				{
					// Si no pertence a la misma raza de su padre entonces se debe ver si pertenece a alguna otra de las razas que existan al momento. 
					unsigned int j ;
					//Se ira una por una verificando los nichos.
					for (j = 0; j < new_niches_vector.size(); ++j)
					{
						// Si es aceptado en este nicho hay que agregarlo o en caso que el nicho este lleno simplemente se descarta.
						if( new_niches_vector.at(j)->IsAcepted( newOrganism ) )
						{
							// Si es aceptado en este nicho hay que agregarlo o en caso que el nicho este lleno simplemente se descarta.
							if( new_niches_vector.at(j)->getAmountOfNewOrganisms() < amountOfOrganismInANewNiche )
							{
								new_niches_vector.at(j)->addOrganism( newOrganism );
								break;
							}
							else
							{
								delete newOrganism;
								break;
							}
						}
					}
					//Se verifica si no fue aceptado por algun nicho, en tal caso este organismo crea un nicho nuevo.
					if( j == new_niches_vector.size() )
					{
						// Si se llega a este punto es porque no fue aceptado en ningun otro nicho entonces debe crear uno propio.
						//En caso que aun no se hallan llenado la cantidad posible de nichos entonces se crea uno nuevo, en otro caso se pone en una lista FIFO de espera para crear nuevos nuchos.
						if( (int) new_niches_vector.size() < MaxAmountOfNewNiches)
						{
							new_niches_vector.push_back( new_niches_vector.at(0)->createNew( newOrganism ) );
						}
						else
						{
							organismFIFOWaitingForCreateNewNiches.push_back( newOrganism );
						}
					}
				}
			}
		}
	}

	// Es el metodo principal del algoritmo NEAT

	void Life::epoch()
	{
		// Se maduran todos los nichos, esto quiere decir que los organismos que eran nuevos pasan a ser viejos dentro de la raza especifica.

		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			new_niches_vector.at(i)->maturation();
		}
		
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			old_niches_vector.at(i)->maturation();
		}

		int totalOfChildrenInOldNiches = 0;


		//Ahora se comienza con la reproduccion tomando en cuenta la ley del mas fuerte, donde organismos con mas fitness tienen mejores probabilidades de tener desendencia. Tomar en cuenta que los nichos viejos se pelean la opcion de tener desendencia en cambio los nuevos tienen una cantidad de desendencia constante.

		double TotalFitnessAverageOfAllOldNiches = 0.0;
		
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			TotalFitnessAverageOfAllOldNiches += old_niches_vector.at( i )->getFitnessAverage();
		}
		// Se tiene la precausion de ver si existen nichos viejos puesto que inicialmente no existen.
		while(totalOfChildrenInOldNiches < AmountOfTotalOrganismFromAllOldNiches && old_niches_vector.size() >= 1)
		{
			// Se revisa si existe una crusa entre razas, la frecuencia de que esto ocurra es una eleccion del usuario quien puede manipular esta probabilidad.
			if( rand()/(double)RAND_MAX  < probabilityOfInterspaceMating && old_niches_vector.size() > 1 )
			{ 
				// Este caso será absolutamente random, es escojeran hijos de dos nichos aleatorios.
				int niche1;
				int niche2;
				//Si solo hay dos nichos entonces no hay nada que procesar, simplemente se toman organismos de cada uno.
				if(old_niches_vector.size() == 1)
				{
					niche1 = 0;
					niche2 = 1;
				}
				else
				{
					niche1 = rand()%old_niches_vector.size();
					niche2 = rand()%(old_niches_vector.size()-1); // Si no se hiciera el procedimiento para solo 2 nichos esta linea podria producir error porque modulo cero no existe.
					if ( niche2 >= niche1 )
					{
						niche2++;
					}
				}

				Organism * father = old_niches_vector.at( niche1 )->obtainOneOrganismToCrossoverWithAnotherNicheOrganism();
				Organism * mother = old_niches_vector.at( niche2 )->obtainOneOrganismToCrossoverWithAnotherNicheOrganism();
				// Se cruzan los organismos electos de ambos nichos diferentes.
				Organism * children = crossover( father , mother );

				// Se procede a buscar el nicho correspondiente para el hijo generado. 

				if( old_niches_vector.at(niche1)->IsAcepted( children ) )
				{ 
					// entonces es aceptado con el nicho padre.
					old_niches_vector.at(niche1)->addOrganism( children );
				}

				else if( old_niches_vector.at(niche2)->IsAcepted( children ) )
				{ 
					// entonces es aceptado con el nicho madre.
					old_niches_vector.at(niche2)->addOrganism( children );
				}

				else
				{
					
					unsigned int j ;
					// Se revizan todos los nichos para verificar si alguno lo acepta.
					for (j = 0; j < old_niches_vector.size(); ++j)
					{
						if( old_niches_vector.at(j)->IsAcepted( children ) )
						{
							totalOfChildrenInOldNiches++;
							old_niches_vector.at(j)->addOrganism( children );
							break;
						}
					}

					// En caso que ninguno lo acepte entonces se crea uno nuevo para el.
					if(j == old_niches_vector.size())
					{
						// Si se llega a este punto es porque no fue aceptado en ningun otro nicho entonces debe crear uno propio.
						organismFIFOWaitingForCreateNewNiches.push_back( children );
					}
				}
			}

			
			else 
			{ 
				// Este es el caso típico, donde todos los nichos se "pelean" la opción de tener un hijo. 


				// Primero se buscara un nicho.
				double random = rand()/(double)RAND_MAX;
				double sum = 0.0;
				Organism * children;
				unsigned int i ;
				for (i = 0; i < old_niches_vector.size(); ++i)
				{
					sum += old_niches_vector.at(i)->getFitnessAverage()/TotalFitnessAverageOfAllOldNiches; // Se divide por totalFitness para normalizar.
					if(sum >= random)
					{
						children = old_niches_vector.at(i)->obtainAChildren();
						break;
					}
				}
				if( i == old_niches_vector.size() )
				{
					std::cerr << "ERROR::Life::epoch::Not niche founded" << std::endl;
					exit(EXIT_FAILURE);
				}
				// Una vez tenido el hijo hay que buscarle un nicho al que pertenezca, se debe partir viendo si lo aceptan en el nicho del cual proviene.
				if(old_niches_vector.at(i)->IsAcepted(children) ) // Primero se prueba en su propio nicho.
				{ 
					// entonces es aceptado con el nicho padre.
					old_niches_vector.at(i)->addOrganism(children);
					totalOfChildrenInOldNiches++;
				}

				else
				{
					unsigned int j ;
					//Dado que no fue aceptado en el nicho del padre se prueba con todo el resto de los nichos a ver si queda aceptado, o en otro caso se crea su propio nicho.
					for (j = 0; j < old_niches_vector.size(); ++j)
					{
						if( old_niches_vector.at(j)->IsAcepted(children) )
						{
							totalOfChildrenInOldNiches++;
							old_niches_vector.at(j)->addOrganism(children);
							break;
						}
					}

					// Dado que no fue aceptado en ningun nicho se crea su propio nicho.
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
			if( old_niches_vector.size() > 0 )
			{
				new_niches_vector.push_back( old_niches_vector.at(0)->createNew( organismFIFOWaitingForCreateNewNiches.front() ) );
			}
			else
			{
				new_niches_vector.push_back( new_niches_vector.at(0)->createNew( organismFIFOWaitingForCreateNewNiches.front() ) );
			}
			organismFIFOWaitingForCreateNewNiches.erase( organismFIFOWaitingForCreateNewNiches.begin() );
		}

		// finalmente se llenan los nichos nuevos
		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{

			while( new_niches_vector.at(i)->getAmountOfNewOrganisms() < amountOfOrganismInANewNiche )
			{
				// Se obtiene un hijo desde el nicho.
				Organism * newOrganism = new_niches_vector.at(i)->obtainAChildren();	
				
				//Se prueba en su propio nicho primero a ver si pertenece en el nicho del que proviene.
				if( new_niches_vector.at(i)->IsAcepted(newOrganism) )
				{ 
					// entonces es aceptado con el nicho padre.
					new_niches_vector.at(i)->addOrganism(newOrganism);
					continue;
				}
				// Si no es aceptado en el nicho de sus padres entonces debe buscar en que nicho queda.
				else
				{
					unsigned int j ;
					// Se probara en todos los nichos.
					for (j = 0; j < new_niches_vector.size(); ++j)
					{
						if( new_niches_vector.at(j)->IsAcepted(newOrganism) )
						{
							// En caso de ser aceptado obviamente se debe agregar en tal nicho solo si no esta lleno ya.
							if( new_niches_vector.at(j)->getAmountOfNewOrganisms() < amountOfOrganismInANewNiche )
							{
								new_niches_vector.at(j)->addOrganism(newOrganism);
								break;
							}
							else
							{
								delete newOrganism;
								break;
							}
						}
					}
					// Si no encontro nicho entonces debe crear uno nuevo
					if(j == new_niches_vector.size())
					{
						// Si se llega a este punto es porque no fue aceptado en ningun otro nicho entonces debe crear uno propio.
						// Existen dos opciones, que pueda crear un nuevo nicho o que ya esten el limite de nichos nuevos por lo tanto hay que ponerlo en una FIFO de espera para crear nichos nuevos.
						if((int)new_niches_vector.size() < MaxAmountOfNewNiches && organismFIFOWaitingForCreateNewNiches.size()==0)
						{
							new_niches_vector.push_back( new_niches_vector.at(0)->createNew( newOrganism ) );
						}
						else
						{
							organismFIFOWaitingForCreateNewNiches.push_back( newOrganism );
						}
					}
				}
			}
		}


		// Se revizara si algun nicho pasa de ser nuevo a viejo segun la cantidad de epocas de resguardo que el usuario introdujiera en las definiciones de usuario.

		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			// Por cada nicho se pasa una epoca.
			new_niches_vector.at(i)->epoch();

			if( new_niches_vector.at(i)->getAge() >= generationsThatDefineAnOldNiche )
			{ 
				// Entonces debe ser pasado a los nichos viejos.
				old_niches_vector.push_back(new_niches_vector.at(i));
				new_niches_vector.erase(new_niches_vector.begin()+i);
				i--;
			}
		}

		// Todos los nichos nuevos pasan de epoca, lo que puede provocar la muerte de alguno de sus miembros segun la espectativa de vida que es introducida por el usuario
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			old_niches_vector.at(i)->epoch();
		}


		// Los nichos sin miembros nuevos ni miembros antiguos  son eliminados, esto es una especie de recogedor de basura como el de java.
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			if(old_niches_vector.at(i)->getAmountOfNewOrganisms() == 0 &&  old_niches_vector.at(i)->getAmountOfOldOrganisms()==0)
			{
				old_niches_vector.erase( old_niches_vector.begin() + i);
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
			new_niches_vector.at(i)->printState();
		}

		std::cout << "=============================Old Niches=========================================" << std::endl; 		
		
		for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			old_niches_vector.at(i)->printState();
		}
	
	}

	vector < Organism * >  Life::getAllNewOrganisms()
	{
		vector < Organism * >  final;
		


		for (unsigned int i = 0; i < new_niches_vector.size(); ++i)
		{
			vector < Organism * >  newOrgm = new_niches_vector.at(i)->getNewOrganismVector();
			final.insert(final.begin(), newOrgm.begin(), newOrgm.end() );
		}
		 for (unsigned int i = 0; i < old_niches_vector.size(); ++i)
		{
			vector < Organism * >  newOrgm = old_niches_vector.at(i)->getNewOrganismVector();
			final.insert(final.begin(), newOrgm.begin(), newOrgm.end() );
		}

		return final;
	}

	int Life::getGenerations()
	{
		return generations;
	}
}

