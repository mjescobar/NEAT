#include "ANN.hpp"


namespace NEATSpikes
{
	int ANN::id=0; // Se tiene que inicializar así porque es una variable estática
	double ANN::probabilityOfSynapticWeightMutation=0.0;  // Se tiene que inicializar así porque es una variable estática
	double ANN::probabilityOfNeuronMutation=0.0;  // Se tiene que inicializar así porque es una variable estática
	double ANN::probabilityOfNewNeuronMutation=0.0; // Se tiene que inicializar así porque es una variable estática
	double ANN::probabilityOfNewSynapticWeightMutation=0.0; // Se tiene que inicializar así porque es una variable estática
	int ANN::inputsAmount=0; // La cantidad de entradas que posee la red neuronal
	int ANN::outputsAmount=0; // La cantidad de salidas que posee la red neuronal
	double ANN::ConstantOFDiferencesInStructureOfSynapticWeight=0.0; // Multiplica a la diferencia de conexiones sinápticas que están en sólo uno de los organismos al calcular su distancia
	double ANN::ConstantOFDiferencesInStructureOfNeurons=0.0; //Multiplica a la diferencia de neuronas que están en sólo uno de los organismos al calcular su distancia
	bool ANN::ANNCanHaveConnectionsBack=false; // Si al mutar se puede obtener conexiones recursivas.
	Neuron * ANN::prototypeNeuron=NULL; // El prototipo de las neuronas es usado para crear las neuronas nuevas del mismo tipo
	SynapticWeight *  ANN::prototypeSynapticWeight=NULL; // El prototipo de las conexiones synapticas es usado para crear nuevas conexiones sinápticas del mismo tipo siempre.

	
	ANN::ANN()
	{
	
	}

	// Este constructor es creado para nuevos ANN
	ANN::ANN(GlobalInformation * information)
	{
		generalInformation = information; // Así siempre se tiene una copia de la información de todas las redes neuronales.
		identificator = id++;
		amountOfPosiblyNeuronMutation=0;
		amountOfPosiblySynapticWeightMutation=0;
	}

	// Constructor copia.
	// Aquí simplemente se copia toda la información de ANN. No hay ningún algoritmo que entender.
	ANN::ANN(const ANN & ann)
	{

		for (int i = 0; i < (int)ann.neuron_vector.size(); ++i)
		{
			neuron_vector.push_back( ann.neuron_vector.at(i)->duplicate() );
		}

		for (int i = 0; i < (int)ann.synapticWeight_vector.size(); ++i)
		{
			synapticWeight_vector.push_back( ann.synapticWeight_vector.at(i)->duplicate() );
		}

		fitness=ann.fitness;

		generalInformation = ann.generalInformation;
		inputsInNeuron_vector = ann.inputsInNeuron_vector;
		outputsInNeuron_vector = ann.outputsInNeuron_vector;
		
		identificator = id++;
		amountOfPosiblyNeuronMutation = ann.amountOfPosiblyNeuronMutation;
		amountOfPosiblySynapticWeightMutation = ann.amountOfPosiblySynapticWeightMutation;
		amountOfNeurons = ann.amountOfNeurons;


		innovarion_to_localSynapticWeightVectorPosition.insert(ann.innovarion_to_localSynapticWeightVectorPosition.begin(), ann.innovarion_to_localSynapticWeightVectorPosition.end());
		historicalMark_To_localNeuron.insert(ann.historicalMark_To_localNeuron.begin(), ann.historicalMark_To_localNeuron.end());

		neuronsReferencesForCreateNewNeurons = ann.neuronsReferencesForCreateNewNeurons;
		neuronsReferencesForCreateNewSynapticWeight = ann.neuronsReferencesForCreateNewSynapticWeight;

		for (unsigned int i = 0; i < ann.availableNumberOfNeuronMutationsInRelationToNeuron.size(); ++i)
		{
			availableNumberOfNeuronMutationsInRelationToNeuron.push_back(ann.availableNumberOfNeuronMutationsInRelationToNeuron.at(i));
		}
		
		for (unsigned int i = 0; i < ann.availableNumberOfSynaptinWeightMutationsInRelationToNeuron.size(); ++i)
		{
			availableNumberOfSynaptinWeightMutationsInRelationToNeuron.push_back(ann.availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at(i));
		}
		
		for (unsigned int i = 0; i < ann.neuronsAtLayer.size(); ++i)
		{
			neuronsAtLayer.push_back(ann.neuronsAtLayer.at(i));
		} 
	}


	/**
		Esto debe crearse  una sola vez en el experimento en si.
	*/
	ANN::ANN(Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * information)
	{
		prototypeNeuron = neuron;
		prototypeSynapticWeight = synapticWeight;
		generalInformation = information; // Así siempre se tiene una copia de la información de todas las redes neuronales.
		SetParametersFromUserDefinitionsPath( path_ANN_definitions ); // Acá se determinan los valores de las definiciones de usurio.
		identificator = id++;
		createInitialANN( );
	}

	ANN::~ANN()
	{
		for (unsigned int i = 0; i < neuron_vector.size(); ++i)
		{
			delete(neuron_vector.at(i));
		}
		
		for (unsigned int i = 0; i < synapticWeight_vector.size(); ++i)
		{
			delete(synapticWeight_vector.at(i));
		}

		neuronsReferencesForCreateNewNeurons.clear(); 
		availableNumberOfNeuronMutationsInRelationToNeuron.clear();
		neuronsReferencesForCreateNewSynapticWeight.clear();
		availableNumberOfSynaptinWeightMutationsInRelationToNeuron.clear();
		historicalMark_To_localNeuron.clear();
		innovarion_to_localSynapticWeightVectorPosition.clear();
		neuronsAtLayer.clear();
		inputsInNeuron_vector.clear();
		outputsInNeuron_vector.clear();
	}

	void ANN::printState()
	{ 
		// Al imprimir el estado de un organismo, por completitud, se imprime el estado de todas las neuronas y todas las conexiones sinápticas de forma ordenada.

		// Primero se imprimen los datos  propios de este organismo.
		//============================================================================
			std::cout << "ANN\t" << "identificator: " << identificator << "\aamountOfNeurons: " << neuron_vector.size()  << std::endl; 
			std::cout << "Layers" << std::endl;
			generalInformation->printLayers();
		//=============================================================================

		// Segundo se imprimen todas las neuronas que son parte de este organismo.
		//=============================================================================
			std::cout << "\nNeurons\n=======" << std::endl;
			for (int i = 0; i < (int)neuron_vector.size(); ++i)
			{
				std::cout << "Neuron: " << i << "\n========="  <<  std::endl;
				neuron_vector.at(i)->printState();
			}
		//=============================================================================

		// Tercero se imprimen todas las conexiones sinápticas que son parte de este organismo.
		//=============================================================================
			std::cout << "SynapticWeight\n=======" << std::endl;
			for (int i = 0; i < (int)synapticWeight_vector.size(); ++i)
			{
				std::cout << "SynapticWeight: " << i << "\n========="  <<  std::endl;
				synapticWeight_vector.at(i)->printState();
			}
		//=============================================================================
	}

	vector <double> ANN::eval(vector <double> inputs)
	{

		// Este método es uno de los métodos escenciales de NEAT, el método que el usuario finalmente utilizar en sus experimentos.
		std::vector <double> final; // la variable que finalmente será devuelta.
		// En caso que se ingresen incorrectamente  la cantidad de entradas entonces el programa da alarma y termina.
		if(inputs.size() != inputsInNeuron_vector.size())
		{	
			std::cerr << "ERROR::ANN::eval::Incorrect amount of inputs" << std::endl;
			exit(EXIT_FAILURE);
		}

		// Lo primero es ingresar las entradas que el usuario de a la red neuronal a las neuronas inputs correspondientes. 
		// Se cargan las entradas a los inputs

		// Paso 1: Sumar las entradas a los inputs correspondientes.
		//=====================================================================
			for (unsigned int i = 0; i < inputsInNeuron_vector.size(); ++i)
			{
				int input_i = historicalMark_To_localNeuron[ inputsInNeuron_vector.at( i ) ];
				neuron_vector.at( input_i )->sumIncomingConnectionsOutputs( inputs.at( i ) );
			}
		//=====================================================================

		// PASO 2: propagar la red. (esta es la parte más importante del método)
		//=====================================================================
			spreadVoltage();
		//=====================================================================


		// PASO 3: Finalmente se compone la salida de la red neuronal.
		//=====================================================================
			for (unsigned int i = 0; i < outputsInNeuron_vector.size(); ++i)
			{
				int localNeuronOutput = historicalMark_To_localNeuron[ outputsInNeuron_vector.at( i ) ];
				final.push_back( neuron_vector.at( localNeuronOutput )->getLastOutput( ) );
			}
		//=====================================================================

		return final;
	}



	void ANN::mutate()
	{
		std::cerr << "Entro a mutate" << std::endl;
		// 1) Primero se revizará neurona a neurona si es que debe o no mutar.
		// 2) Se revizarán todas las conexiones sinápticas para ver si deben mutar.
		// 3) Se reviza si hay mutación que cree una nueva neurona
		// 4) Se reviza si hay mutación que cree una nueva conexión sináptica.

		// Paso 1: se revizará neurona a neurona si es que debe o no mutar
		//=============================================
			for (unsigned int i = 0; i < neuron_vector.size() ; ++i)
			{
				if ( rand()/(double)RAND_MAX <=  probabilityOfNeuronMutation)
				{	
		std::cerr << "Entro a mutate2" << std::endl;
					neuron_vector.at(i)->mutate();
				}
			}	
		//=============================================

		// 2) Se revizarán todas las conexiones sinápticas para ver si deben mutar.
		//=============================================
			for (unsigned int i = 0; i < synapticWeight_vector.size() ; ++i)
			{
				if ( rand()/(double)RAND_MAX <=  probabilityOfSynapticWeightMutation)
				{
		std::cerr << "Entro a mutate3" << std::endl;
					synapticWeight_vector.at(i)->mutate();
				}
			}
		//=============================================

		// 3) Se reviza si hay mutación que cree una nueva neurona
		//=============================================
			if ( rand()/(double)RAND_MAX <=  probabilityOfNewNeuronMutation )
			{
		std::cerr << "Entro a mutate4" << std::endl;
				newNeuronMutation();
			}
		//=============================================
		// 4) Se reviza si hay mutación que cree una nueva conexión sináptica.
		//=============================================
			if ( rand()/(double)RAND_MAX <=  probabilityOfNewSynapticWeightMutation)
			{
		std::cerr << "Entro a mutate5" << std::endl;
				newSynapticWeightMutation();
			}
		//=============================================
		std::cerr << "salió de mutate" << std::endl;
	}


	// ANN * ANN::crossover(ANN * mother)
	// {

	// 	// El procedimiento que se realiza es:
	// 	// 1) El hijo copia todo del padre
	// 	// 2) Se recorren todas las neuronas de la madre, de haber coincidencia de neuronas entonces con 50% de probabilidades la neurona se cambia por la versión de la madre en otro caso se conserva el del padre. 
	// 	// 3) Lo mismo pero con las conexiones sinápticas.
	// 	ANN * children = new ANN( *this );

	// 	// Primero se agregan todas las neuronas que posea la madre al padre si este no lo tiene, y si lo tiene 50% de probabilidad de cambiar el existente por el de la madre
	// 	//==================================================================================
	// 		for (unsigned int i = 0; i < mother->neuron_vector.size(); ++i)
	// 		{
				
	// 			int historicalMarkMotherNeuron = mother->neuron_vector.at( i )->getHistoricalMark();

	// 			if( historicalMark_To_localNeuron.count( historicalMarkMotherNeuron ) > 0 ) // Si historicalMarkMotherNeuron está en el mapa entonces:
	// 			{
					
	// 				int localNeuronPosition = historicalMark_To_localNeuron.at( historicalMarkMotherNeuron );
	// 				if( localNeuronPosition == 0 && historicalMarkMotherNeuron != 0 )
	// 				{
	// 					std::cerr << "EROR:::::::::: " << historicalMarkMotherNeuron << "\t" << historicalMark_To_localNeuron.count( historicalMarkMotherNeuron ) << std::endl;
	// 					testPrint();
	// 					exit( EXIT_FAILURE );
	// 				}

	// 				if( rand()/(double)RAND_MAX > 0.5 )
	// 				{ // Entonces cambiar el existente por el de la madre.
	// 					Neuron * toDelete = children->neuron_vector.at( localNeuronPosition );
	// 					children->neuron_vector.at( localNeuronPosition ) = mother->neuron_vector.at( i )->duplicate( );
	// 					delete( toDelete );
	// 				}
	// 			}	
	// 			else
	// 			{
	// 				children->addNeuron( mother->neuron_vector.at( i )->duplicate() );
	// 			}
	// 		}
	// 	//==================================================================================


	// 	// Se agregan todas las conexiones sinápticas que están sólo en la madre, y las conexiones sinápticas que están en ambos se tiene un 50% de probabilidades que adquiera el del padre o la madre.
	// 	//==================================================================================
	// 		for ( unsigned int i = 0; i < mother->synapticWeight_vector.size( ); ++i )
	// 		{
	// 			int innovationMother = mother->synapticWeight_vector.at( i )->getInnovation( );

	// 			if( innovarion_to_localSynapticWeightVectorPosition.count( innovationMother ) )
	// 			{
	// 				int localInnovationPosition = innovarion_to_localSynapticWeightVectorPosition.at( innovationMother );

	// 				if( rand()/(double)RAND_MAX > 0.5 )
	// 				{ // Entonces cambiar el existente por el de la madre.
	// 					SynapticWeight * toDelete;
	// 					toDelete = children->synapticWeight_vector.at( localInnovationPosition );
	// 					children->synapticWeight_vector.at( localInnovationPosition )= mother->synapticWeight_vector.at( i )->duplicate();
	// 					delete( toDelete );
	// 				}
	// 			}
	// 			else // Si el dato no está en el mapa de referencias entocnes es porque no existe en el padre y hay que agregarlo sin más
	// 			{
	// 				children->addSynapticWeight( mother->synapticWeight_vector.at( i )->duplicate( ) );
	// 			}
	// 		}
	// 	//==================================================================================

	// 	return children;
	// }


	/*
		Las neuronas del hijo se agregaran en orden, esto quiere decir que primero se les agregarán las neuronas con marcas históricas de menor magnitud, similarmente con las conexiones sinápticas y sus innovaciones.
	*/
	ANN * crossover(ANN * father, ANN * mother)
	{
		std::cerr << "crossover entra" << std::endl;
		// Lo primero es crear un hijo vacío. 
		ANN * children = new ANN( father->generalInformation );
		// Se agregan todas las neuronas iniciales.
		//==================================================
			// INPUTS
			for (int i = 0; i < ANN::inputsAmount; ++i)
			{
				children->inputsInNeuron_vector.push_back(i);
				children->addInitialStructureNeuron( new Input( i ,-1,-1, LAYER_INPUT ) );
			}

			// OUTPUTS: Con 50% es del padre y 50% es de la madre.
			for (int i = 0; i < ANN::outputsAmount; ++i)
			{
				int position = i+ANN::inputsAmount;
				children->outputsInNeuron_vector.push_back(position);
				if ( rand() > RAND_MAX/2.0 )
				{
					children->addInitialStructureNeuron( father->neuron_vector.at(position)->duplicate() );
				}
				else
				{
					children->addInitialStructureNeuron( mother->neuron_vector.at(position)->duplicate() );
				}
			}
		//==================================================

		// Se agergan todas las neuronas restantes que se encuentren en alguno de los progenitores, si se enceuntra en ambos entonces se hereda cualquiera con 50% de probabilidades.
		//==================================================
			//Hacerlo de forma ordenada teóricamente no debería atraer problemas porque se suponque que una nurona de historical mark más alto sólo puede conectar dos neuronas de historical mark mas bajo que se suponque que ya fueron creadas.
			for (int i = ANN::inputsAmount+ANN::outputsAmount; i < children->generalInformation->getAmountOfNeurons() +1 ; ++i)
			{
				int fatherLocalNeuron = father->historicalMark_To_localNeuron[ i ];
				int motherLocalNeuron = mother->historicalMark_To_localNeuron[ i ];

				if( fatherLocalNeuron || motherLocalNeuron )
				{
					bool father_flag = false; // Si es true entonces la neurona elejida es del padre.
					if( fatherLocalNeuron && motherLocalNeuron )
					{
						if(rand() > RAND_MAX/2.0) 
						{
							father_flag = true;
						}
					}
					else if( fatherLocalNeuron )
					{
						father_flag = true;
					}

					if( father_flag )
					{
						children->addNeuron( ( father->neuron_vector.at( fatherLocalNeuron ) )->duplicate()  );
					}
					else
					{
						children->addNeuron( ( mother->neuron_vector.at( motherLocalNeuron ) )->duplicate()  );
					}
				}
			}
			std::cerr << "crossover entra 2" << std::endl;

		//Se agregan las conexiones sinápticas.
		//==================================================
			// Se agrega la conexión 0 manualmente dado que si o si está en ambos organismos y así se simplifica el loop siguiente.

			if(rand() > RAND_MAX/2.0)
			{
				children->addSynapticWeight( ( father->synapticWeight_vector.at( 0 ) )->duplicate() );
			}
			else
			{
				children->addSynapticWeight( ( mother->synapticWeight_vector.at( 0 ) )->duplicate() );
			}

			for (int i = 1; i < children->generalInformation->getAmountOfSynapticWeights() +1 ; ++i)
			{
				int fatherLocalSynapses = father->innovarion_to_localSynapticWeightVectorPosition[ i ];
				int motherLocalSynapses = mother->innovarion_to_localSynapticWeightVectorPosition[ i ];

				if( fatherLocalSynapses || motherLocalSynapses )
				{
					bool father_flag = false; // Si es true entonces la conexión elejida es del padre.
					if( fatherLocalSynapses && motherLocalSynapses )
					{
						if(rand() > RAND_MAX/2.0) 
						{
							father_flag = true;
						}
					}
					else if( fatherLocalSynapses )
					{
						father_flag = true;
					}

					if( father_flag )
					{
						children->addSynapticWeight( ( father->synapticWeight_vector.at( fatherLocalSynapses ) )->duplicate()  );
					}
					else
					{
						children->addSynapticWeight( ( mother->synapticWeight_vector.at( motherLocalSynapses ) )->duplicate()  );
					}
				}
			}
		//==================================================

		std::cerr << "crossover sale" << std::endl;

		return children;

	}

	void ANN::testPrint()
	{
		
		std::cout << "===============================================================\n";
		std::cout << "Se imprimen neuronsReferencesForCreateNewNeurons \n";
		for (unsigned int i = 0; i < neuronsReferencesForCreateNewNeurons.size(); ++i)
		{
			for (unsigned int j = 0; j < neuronsReferencesForCreateNewNeurons.at( i ).size(); ++j)
			{
				std::cout << neuronsReferencesForCreateNewNeurons.at( i ).at( j ) << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << "\nSe imprimen neuronsReferencesForCreateNewSynapticWeight \n";
		for (unsigned int i = 0; i < neuronsReferencesForCreateNewSynapticWeight.size(); ++i)
		{
			for (unsigned int j = 0; j < neuronsReferencesForCreateNewSynapticWeight.at( i ).size(); ++j)
			{
				std::cout << neuronsReferencesForCreateNewSynapticWeight.at( i ).at( j ) << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << "Total de mutaciones de neurona posible: " << amountOfPosiblyNeuronMutation <<"\tTotal de mutaciones de conexion posible: " << amountOfPosiblySynapticWeightMutation  << std::endl;
		std::cout << "===============================================================\n";
	}

	double ANN::getFitness()
	{
		return fitness;
	}

	void ANN::setFitness(double _fitness)
	{
		fitness = _fitness;
	}

	ANN * ANN::createOtherFromSeedOrganism()
	{
		 ANN* newAnn = new ANN( *this );
		
		for (unsigned int i = 0; i < newAnn->neuron_vector.size(); ++i)
		{
			newAnn->neuron_vector.at( i )->changeValuesRandomly();	
		}
		
		for (unsigned int i = 0; i < newAnn->synapticWeight_vector.size(); ++i)
		{
			newAnn->synapticWeight_vector.at( i )->changeValuesRandomly();	
		}

		return newAnn;
	}

	void ANN::saveState(std::string pathToSave)
	{
		// Bastante largo, se deja para posterior trabajo.
	}

	void ANN::load(std::string PathWhereIsSaved)
	{

	}

	void ANN::SetParametersFromUserDefinitionsPath( std::string pathUserDefinitionsAboutANN )
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
			archive = fopen( pathUserDefinitionsAboutANN.c_str() , "r" );
			if ( archive == NULL )
			{
				std::cerr << "ERROR::BasicNeuron::SetParametersFromUserDefinitionsPath::Could not read the file" << std::endl;
				exit( EXIT_FAILURE );
			}
		//=========================================================================================
		//Se lee linea a linea y se guarda en el mapa UserDefinitions
		//=========================================================================================
			while ( ( read = getline(&line, &len, archive) ) != -1) 
			{
				token_1 = strtok_r(line, delimiters, &saveptr); // Por si acaso se desea hacer uso de hilos.
				token_2 = strtok_r(NULL, delimiters, &saveptr);
				UserDefinitions[token_1]=atof(token_2); // Se comieenza a llenar el mapa.
			}
			fclose( archive ); // Ya no se usa el recurso así que se cierra.

		//=========================================================================================
		// Ahora se le da el valor a las variables de usuario y se termina este método. Usando mapas se hace más sencillo y más robusto.
		//=========================================================================================
		
			probabilityOfSynapticWeightMutation = UserDefinitions[ "Probability_Of_Synaptic_Weight_Mutation" ];
			probabilityOfNeuronMutation = UserDefinitions[ "Probability_Of_Neuron_Mutation" ];
			probabilityOfNewNeuronMutation = UserDefinitions[ "Probability_Of_New_Neuron_Mutation" ];
			probabilityOfNewSynapticWeightMutation = UserDefinitions["Probability_Of_New_Synaptic_Weight_Mutation"];
			inputsAmount = UserDefinitions[ "inputsAmount" ];
			outputsAmount= UserDefinitions[ "outputsAmount" ];
			ConstantOFDiferencesInStructureOfSynapticWeight=UserDefinitions[ "ConstantOFDiferencesInStructureOfSynapticWeight" ];
			ConstantOFDiferencesInStructureOfNeurons=UserDefinitions[ "ConstantOFDiferencesInStructureOfNeurons" ];
			ANNCanHaveConnectionsBack = UserDefinitions[ "ANNCanHaveConnectionsBack" ];
		//=========================================================================================
		
		// =========================================================================================
		// Se reviza si estan todos los datos en sus correspondientes intervalos
			if(probabilityOfNewSynapticWeightMutation > 1 || probabilityOfNewSynapticWeightMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfNewSynapticWeightMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(probabilityOfNewNeuronMutation > 1 || probabilityOfNewNeuronMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfNewNeuronMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(probabilityOfNeuronMutation > 1 || probabilityOfNeuronMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfNeuronMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(probabilityOfSynapticWeightMutation > 1 || probabilityOfSynapticWeightMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfSynapticWeightMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}
		//=========================================================================================
	}


	void ANN::newNeuronMutation()
	{		
		int	localNeuronIn, 
			localNeuronOut,
			neuronReference_1,
			neuronReference_2,
			historicalMark,
			histoticalMarkNeuronIn,
			histoticalMarkNeuronOut;

		// Primero se obtienen las neuronas inicial input e inicial output de forma aleatoria, además la función devuelve todas las formas en que estos datos son guardados; en mapa de referencias, en las listas locales(localNeuron) y en las listas globales(HistoricalMarks)
		tie( histoticalMarkNeuronIn, histoticalMarkNeuronOut, localNeuronIn, localNeuronOut, neuronReference_1, neuronReference_2 ) = findNeuronsForNewNeuronMutation();


		// Se debe deshabilitar la conexión sináptica que conectaba diréctamente las neuronas que antes estaban diréctamente conectadas, SI EXISTE porque no tiene por qué existir. Si no existe, el procedimiento que debería ser es crear una conexión sináptica que esté en la posición dicha y deshabilitarla.
		if(  neuronsReferencesForCreateNewSynapticWeight.at( neuronReference_1 ).at( neuronReference_2 ) == -1 ) // Si no existe
		{
			// Entonces se crea en la red neuronal.
			addSynapticWeight( histoticalMarkNeuronIn, histoticalMarkNeuronOut );
		}
		// Se deshabilita la conexión sináptica.
		synapticWeight_vector.at(   innovarion_to_localSynapticWeightVectorPosition[   neuronsReferencesForCreateNewSynapticWeight.at( neuronReference_1 ).at( neuronReference_2 )  ]  )->disable();	
		
		// Luego,  se agrega la neurona creada a la red neuronal.
		addNeuron(  histoticalMarkNeuronIn, histoticalMarkNeuronOut );
		
		// Se obtiene su marca histórica.
		historicalMark = neuronsReferencesForCreateNewNeurons.at( neuronReference_1 ).at( neuronReference_2 ); 

		// Ahora se deben crear 2 conexiones sinápticas, la primera desde la neurona que inicialmente es la input hacia la neurona nueva y otro desde la neurona haica la neurona que es inicialmente la salida.
		// Ahora sí se agregan las conexiones sinápticas.
		std::cerr << "OPCIÓN 1" << "  neuronReference_1: " << neuronReference_1 << "\tneuronReference_2: "<< neuronReference_2 << std::endl;
		generalInformation->printLayers();
		std::cerr << neuron_vector.at(localNeuronIn)->getLayer() << " , " << neuron_vector.at(localNeuronOut)->getLayer() << std::endl;

		addSynapticWeight( histoticalMarkNeuronIn , historicalMark );
		std::cerr << "OPCIÓN 2\t" << "" << std::endl;
		addSynapticWeight( historicalMark , histoticalMarkNeuronOut );
		std::cerr << "salio de OPCIÓN" << std::endl;
	}

	void ANN::addNeuronToLayer(int layer, int historicalMark)
	{
		if( (int)neuronsAtLayer.size() <= layer )
		{ // El layer no existe.
			while( (int)neuronsAtLayer.size() <= layer )
			{
				std::vector <int> extension (1,-1);
				neuronsAtLayer.push_back( extension);
			}
			neuronsAtLayer.at( layer ).at( 0 ) = historicalMark;
		}
		else
		{	
			if( neuronsAtLayer.at( layer ).at(0) == -1 )
			{// En caso que fue creada sin valores entonces el primer dato será -1 lo cual es sólo para definir que no tiene datos, en otro caso este valor debiera ser cambiado.
				neuronsAtLayer.at( layer ).at(0) = historicalMark;
			}
			else
			{
				neuronsAtLayer.at( layer ).push_back( historicalMark );
			}
		}
	}


	void ANN::addSynapticWeight( SynapticWeight * SW )
	{

		
		


		// Paso 1: Se obtienen todos los datos necesarios.
		//================================================================
			// Se obtiene la innovación de la conexión sináptica que se va a agregar.
			int innovation = SW->getInnovation();

			// Se obtienen las marcas históricas de la neurona entrante
			int historicalMarkNeuronIn =  SW->getHistoricalMarkOfNeuronIn();
			int historicalMarkNeuronOut =  SW->getHistoricalMarkOfNeuronOut();

			// Se obtiene la posición de ambos en el vector local de neuronas.
			int localNeuronIn = historicalMark_To_localNeuron[ historicalMarkNeuronIn ];
			int localNeuronOut = historicalMark_To_localNeuron[ historicalMarkNeuronOut ];

			// Se obtiene la posición de éstos en  neuronsReferencesForCreateNewSynapticWeight
			int NeuronReference1;
			int NeuronReference2;

			tie( NeuronReference1, NeuronReference2 ) = localNeuronsInOut_TO_referencesNeurons( {localNeuronIn,localNeuronOut} );
		//================================================================

		// Si no se ha agregado antes entonces en el mapa neuronsReferencesForCreateNewSynapticWeight en su posición debería haber un valor -1
		// Paso 2: se analiza un posible caso de error:	
		//================================================================
			if( neuronsReferencesForCreateNewSynapticWeight.at( NeuronReference1 ).at( NeuronReference2 ) !=  -1) 
			{
				std::cerr << "ANN::addSynapticWeight::SynapticWeight already exist    " << neuronsReferencesForCreateNewSynapticWeight.at( NeuronReference1 ).at( NeuronReference2 ) << std::endl;
				cerr << "NeuronReference1: " << NeuronReference1 << "\tNeuronReference2: " << NeuronReference2 << std::endl;
				generalInformation->printLayers();
		std::cerr << neuron_vector.at(localNeuronIn)->getLayer() << " , " << neuron_vector.at(localNeuronOut)->getLayer() << std::endl;
				testPrint();
				exit( EXIT_FAILURE );
			}
		//================================================================


		// Ahora en la posición estará el valor de la innovación en el mapa de referencias para creaciones de innovaciones.
		//================================================================
			neuronsReferencesForCreateNewSynapticWeight.at( NeuronReference1 ).at( NeuronReference2 ) = innovation;		
			innovarion_to_localSynapticWeightVectorPosition[ innovation ] = synapticWeight_vector.size();// Notar que synapticWeight_vector.size() es la posición que tendrá esta conexión sináptica al agregarse al vector synapticWeight_vector
		//================================================================


		// Se debe agregar esta conexion a la lista de conexiones entrantes y salientes de las neuronas correspondientes.
		//===============================================================
			neuron_vector.at( localNeuronOut )->addNewIncomingConection( innovation ); 
			neuron_vector.at( localNeuronIn )->addNewOutcomingConection( innovation ); 
		//================================================================


		// Se deben ajustar la cantidad de mutaciones de conexion disponibles y la cantidad de mutaciones disponibles con relacion a la neuronade referencia.
		//===============================================================
			// Se debe disminuir en 1 el valor de cantidad de posibles mutaciones de conexiones sinápticas.
			amountOfPosiblySynapticWeightMutation--;
			// La cantidad de posibles mutaciones de conexión sináptica disminuye en uno en relación a esa neurona de referencia.
			availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( NeuronReference1 )--; 

		// Finalmente se agrega la conexión sináptica al vector de conexiones sinápticas.
		//===============================================================
			synapticWeight_vector.push_back( SW );
		//===============================================================
	}

	void ANN::addSynapticWeight(int historicalNeuronIn, int historicalNeuronOut)
	{
		SynapticWeight * newSynapticWeight = prototypeSynapticWeight->createNew( historicalNeuronIn , historicalNeuronOut );
		addSynapticWeight(newSynapticWeight);
	}

	void ANN::addNeuron( Neuron * N )
	{
		// Paso 1: Se obtienen todos los datos necesarios.
		//================================================================
			int historicalMark= N->getHistoricalMark();
			int layer = N->getLayer();
			 // Se debe agregar la neurona a su layer correspondiente para fácilitar la evaluación después.
			addNeuronToLayer( layer, historicalMark );

			// Se obtienen las marcas históricas de la neurona entrante
			int historicalMarkNeuronIn =  N->getInitialNeuronInHistoricalMark();
			int historicalMarkNeuronOut =  N->getInitialNeuronOutHistoricalMark();

			// Se obtiene la posición de ambos en el vector local de neuronas.
			int localNeuronIn = historicalMark_To_localNeuron[ historicalMarkNeuronIn ];
			int localNeuronOut = historicalMark_To_localNeuron[ historicalMarkNeuronOut ];

			// Se obtiene la posición de éstos en  neuronsReferencesForCreateNewNeurons
			int NeuronReference1;
			int NeuronReference2;
			tie(NeuronReference1, NeuronReference2) = localNeuronsInOut_TO_referencesNeurons({localNeuronIn,localNeuronOut});
		//================================================================

		// Paso 2: se analiza un posible caso de error:	
		//================================================================
			if( neuronsReferencesForCreateNewNeurons.at( NeuronReference1 ).at( NeuronReference2 ) != -1) 
			{
				std::cerr << "ANN::addNeuron::Neuron already exist, Neuron->printState():" << std::endl;
				N->printState();
				std::cerr << "ANN->testPrint() for see neuron reference map:" <<std::endl; 
				testPrint();
				exit( EXIT_FAILURE );
			}
		//================================================================
		
		neuronsReferencesForCreateNewNeurons.at( NeuronReference1 ).at( NeuronReference2 ) = historicalMark;
		
		
		// Paso 3: se agrega la neurona al vector de neuronas y se asigna en el mapa
		//================================================================
			neuron_vector.push_back( N );
			int positionNewNeuron = neuron_vector.size()-1;
			historicalMark_To_localNeuron[ historicalMark ] = positionNewNeuron;
		//================================================================

		
		// Finalmente se debe re calcular la cantidad de posibles mutaciones.
		//================================================================
			availableNumberOfNeuronMutationsInRelationToNeuron.at(NeuronReference1)--; // Ahora hay una opción menos de mutación con relación a la neurona i (según el if de arriba se desifra la neurona i)
			 amountOfPosiblyNeuronMutation += (2*positionNewNeuron + 1) - 1;  // donde (2*positionNewNeuron + 1): es la cantidad de nuevas mutaciones por agregar una neurona y -1 es por haber usado una opción.
			 amountOfPosiblySynapticWeightMutation += (2*positionNewNeuron + 1);
			availableNumberOfSynaptinWeightMutationsInRelationToNeuron.push_back( 2*positionNewNeuron +1);
			// Al agregar la nueva neurona neuronsReferencesForCreateNewSynapticWeight y neuronsReferencesForCreateNewNeurons deben agrandarse correspondientemente.
			std::vector <int> extension_1 ( 2*positionNewNeuron +1, -1 ); // donde (2*positionNewNeuron + 1) son todas las convinaciones con la nueva neurona que pueden haber.
			std::vector <int> extension_2 ( 2*positionNewNeuron +1, -1 );// donde (2*positionNewNeuron + 1) son todas las combinaciones con la nueva neurona que pueden haber.
			availableNumberOfNeuronMutationsInRelationToNeuron.push_back( 2*positionNewNeuron +1 );
			neuronsReferencesForCreateNewSynapticWeight.push_back( extension_1 );
			neuronsReferencesForCreateNewNeurons.push_back( extension_2 );
		//================================================================

		// En caso de que no se permitan conexiones hacia atrás 
		// Se recorrerán todas las neuronas y se verán si son de un layer mayor, menor o igual. De ser de un layer mayor entonces se aceptan las conexiones hacia adelante y no las hacia atrás, de ser un layer menor se aceptan las conexiones hacia atrás pero no hacia adelante y de ser del mismo layer no se aceptan conexiones. (simil con neuronas)
		//================================================================
			if( !ANNCanHaveConnectionsBack )
			{
				for ( unsigned int i = 0; i < neuron_vector.size() ; ++i )
				{
					int layerOtherNeuron = neuron_vector.at( i )->getLayer();

					// Primero se reviza si es exactamente la misma neurona, entonces se elimina la conexión consigo misma.
					if(positionNewNeuron == (int)i)
					{
						// En el caso de conexiones desde la neurona nueva hacia la neurona i no se aceptan las conexiones (-2)
						neuronsReferencesForCreateNewNeurons.at( i ).at( i ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( i ).at( i ) = -2;
						
						// Se quitan las posibilidades de conexión.
						availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( positionNewNeuron ) -= 1; 
						availableNumberOfNeuronMutationsInRelationToNeuron.at( positionNewNeuron ) -= 1; 

						//Y se elimina esa opción de la cantidad de mutaciones posibles.
						amountOfPosiblySynapticWeightMutation -= 1;
						amountOfPosiblyNeuronMutation -= 1;
					}

					// En caso de que sean el mismo layer se eliminan todas las posibles conexiones/neuronas entre estas dos neuronas (sea de dirección ida o vuelta).
					else if( generalInformation->layerToPlace( layer ) == generalInformation->layerToPlace(layerOtherNeuron) )
					{
						int input = positionNewNeuron;
						int output= i;
						int ref1,ref2;
						// En el caso de conexiones desde la neurona nueva hacia la neurona i no se aceptan las conexiones (-2)
						neuronsReferencesForCreateNewNeurons.at( input ).at( output ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( input ).at( output ) = -2;

						// En el caso de conexiones desde la neurona i hacia la neurona nueva no se aceptan las conexiones (-2)
						// Primero se deben saber los índices en el mapa de referencia cuando los inputs y outputs son cambiados
						tie( ref1, ref2 )=localNeuronsInOut_TO_referencesNeurons( {output,input} );
						neuronsReferencesForCreateNewNeurons.at( ref1 ).at( ref2 ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( ref1 ).at( ref2 ) = -2;
						
						// Se quitan las posibilidades de conexión.
						availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( positionNewNeuron ) -= 2; 
						availableNumberOfNeuronMutationsInRelationToNeuron.at( positionNewNeuron) -= 2; 
						amountOfPosiblySynapticWeightMutation -= 2;
						amountOfPosiblyNeuronMutation -= 2;

					}
					// En caso de que el layer actual sea menor que el layer de la neurona i entonces se anulan todas las conexiones/neuronas que comiencen en la neurona i y terminen en la nueva.
					else if( generalInformation->layerToPlace( layer ) < generalInformation->layerToPlace( layerOtherNeuron ) )
					{
						int input = positionNewNeuron;
						int output= i;
						int ref1,ref2;
						// En el caso de conexiones desde la neurona i hacia la neurona nueva no se aceptan las conexiones (-2)
						// Primero se deben saber los índices en el mapa de referencia cuando los inputs y outputs son cambiados
						tie( ref1, ref2 )=localNeuronsInOut_TO_referencesNeurons({output,input});
						neuronsReferencesForCreateNewNeurons.at( ref1 ).at( ref2 ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( ref1 ).at( ref2 ) = -2;

						// Se quitan las posibilidades de conexión
						availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( positionNewNeuron ) -=1; 
						availableNumberOfNeuronMutationsInRelationToNeuron.at( positionNewNeuron ) -=1; 
						amountOfPosiblySynapticWeightMutation -= 1;
						amountOfPosiblyNeuronMutation -= 1;
					}
					// En el otro caso: se tiene que la neurona actual tiene un layer mayor que la neurona i, entonces se niegan las conexiones desde la neurona actual a la neurona i.
					else
					{
						int input = positionNewNeuron;
						int output= i;
						// En el caso de conexiones desde la neurona nueva hacia la neurona i no se aceptan las conexiones (-2)
						neuronsReferencesForCreateNewNeurons.at( input ).at( output ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( input ).at( output ) = -2;

						// Se quitan las posibilidades de conexión
						availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( positionNewNeuron ) -=1; 
						availableNumberOfNeuronMutationsInRelationToNeuron.at( positionNewNeuron ) -=1; 
						amountOfPosiblySynapticWeightMutation -= 1;
						amountOfPosiblyNeuronMutation -= 1;
					}
				}
			}
		//================================================================	
	}


	void ANN::addNeuron(int histoticalMarkNeuronIn, int histoticalMarkNeuronOut)
	{
		int layer, historicalMark, localNeuronIn, localNeuronOut;

		localNeuronIn = historicalMark_To_localNeuron.at(histoticalMarkNeuronIn);
		localNeuronOut = historicalMark_To_localNeuron.at(histoticalMarkNeuronOut);
		//Se obtiene la marca histórica de la neurona
		historicalMark = generalInformation->getHistoricalMark( histoticalMarkNeuronIn, histoticalMarkNeuronOut  ); // Si ya tiene innovation no importa prque el valor no cambiaría, si no tiene es impotante
		// Se obtiene el layer correspondiente de la neurona. 
		layer = generalInformation->getLayer( neuron_vector.at( localNeuronIn )->getLayer() , neuron_vector.at( localNeuronOut )->getLayer() );
		// A estas alturas ya se sabe el valor de la neurona inicial input y la neurona inicial output. 
		// Se debe obtener el valor del historical mark de esta neurona, también a qué capa (layer) que corresponde.
		Neuron * newNeuron = prototypeNeuron->createNew( historicalMark, histoticalMarkNeuronIn, histoticalMarkNeuronOut, layer );
		// Finalmente se agrega a la red neuronal.
		addNeuron(newNeuron);
	}

	// Ha diferencia de las otras neuronas, al agregar una neurona de la estructura inicial esta no tiene neurona inicial input o no tiene neurona inicial output, así que en el mapa de referencia de neuronas se debe agregar diferentemente.
	void ANN::addInitialStructureNeuron( Neuron * N )
	{
		// Paso 1: Se obtienen todos los datos necesarios.
		//================================================================
			
			int historicalMark= N->getHistoricalMark();
			int layer = N->getLayer();
			 // Se debe agregar la neurona a su layer correspondiente para fácilitar la evaluación después.
			addNeuronToLayer( layer, historicalMark );
		//================================================================

		
		// Paso 2: se agrega la neurona al vector de neuronas y se asigna en el mapa
		//================================================================
			neuron_vector.push_back( N );
			int positionNewNeuron = neuron_vector.size()-1;
			historicalMark_To_localNeuron[ historicalMark ] = positionNewNeuron;
		//================================================================

		
		// Finalmente se debe re calcular la cantidad de posibles mutaciones.
		//================================================================
			amountOfPosiblyNeuronMutation += 2*positionNewNeuron+1;  // donde 2*positionNewNeuron: es la cantidad de nuevas mutaciones por agregar una neurona y el +1 es consigo mismo
			amountOfPosiblySynapticWeightMutation += 2*positionNewNeuron+1;
			availableNumberOfSynaptinWeightMutationsInRelationToNeuron.push_back( 2*positionNewNeuron +1);
			// Al agregar la nueva neurona neuronsReferencesForCreateNewSynapticWeight y neuronsReferencesForCreateNewNeurons deben agrandarse correspondientemente.
			std::vector <int> extension_1 ( 2*positionNewNeuron +1, -1 ); // donde 2*positionNewNeuron +1  son todas las combinaciones con la nueva neurona que pueden haber (incluida si misma).
			std::vector <int> extension_2 ( 2*positionNewNeuron +1, -1 );// donde 2*positionNewNeuron +1 son todas las convinaciones con la nueva neurona que pueden haber.
			availableNumberOfNeuronMutationsInRelationToNeuron.push_back( 2*positionNewNeuron+1 );
			neuronsReferencesForCreateNewSynapticWeight.push_back( extension_1 );
			neuronsReferencesForCreateNewNeurons.push_back( extension_2 );
		//================================================================


		// En caso de que no se permitan conexiones hacia atrás 
		// Se recorrerán todas las neuronas y se verán si son de un layer mayor, menor o igual. De ser de un layer mayor entonces se aceptan las conexiones hacia adelante y no las hacia atrás, de ser un layer menor se aceptan las conexiones hacia atrás pero no hacia adelante y de ser del mismo layer no se aceptan conexiones. (simil con neuronas)
		//================================================================
			if( !ANNCanHaveConnectionsBack )
			{
				for ( unsigned int i = 0; i < neuron_vector.size() ; ++i )
				{
					int layerOtherNeuron = neuron_vector.at( i )->getLayer();
					// En caso de que se trate de la misma neurona entonces hay que eliminar la opción
					if(positionNewNeuron == (int)i)
					{
						// En el caso de conexiones desde la neurona nueva hacia la neurona i no se aceptan las conexiones (-2)
						neuronsReferencesForCreateNewNeurons.at( i ).at( i ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( i ).at( i ) = -2;
						
						// Se quitan las posibilidades de conexión.
						availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( positionNewNeuron ) -= 1; 
						availableNumberOfNeuronMutationsInRelationToNeuron.at( positionNewNeuron ) -= 1; 

						//Y se elimina esa opción de la cantidad de mutaciones posibles.
						amountOfPosiblySynapticWeightMutation -= 1;
						amountOfPosiblyNeuronMutation -= 1;
					}
					// En caso de que sean el mismo layer se eliminan todas las posibles conexiones/neuronas entre estas dos neuronas (sea de dirección ida o vuelta).
					else if( generalInformation->layerToPlace( layer ) == generalInformation->layerToPlace( layerOtherNeuron ) )
					{

						int input = positionNewNeuron;
						int output= i;
						int ref1,ref2;
						// En el caso de conexiones desde la neurona nueva hacia la neurona i no se aceptan las conexiones (-2)
						neuronsReferencesForCreateNewNeurons.at( input ).at( output ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( input ).at( output ) = -2;

						// En el caso de conexiones desde la neurona i hacia la neurona nueva no se aceptan las conexiones (-2)
						// Primero se deben saber los índices en el mapa de referencia cuando los inputs y outputs son cambiados
						tie( ref1, ref2 )=localNeuronsInOut_TO_referencesNeurons( {output,input} );
						neuronsReferencesForCreateNewNeurons.at( ref1 ).at( ref2 ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( ref1 ).at( ref2 ) = -2;
						
						// Se quitan las posibilidades de conexión.
						availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( positionNewNeuron ) -= 2; 
						availableNumberOfNeuronMutationsInRelationToNeuron.at( positionNewNeuron ) -= 2; 
						amountOfPosiblySynapticWeightMutation -= 2;
						amountOfPosiblyNeuronMutation -= 2;
					}

					// En caso de que el layer actual sea menor que el layer de la neurona i entonces se anulan todas las conexiones/neuronas que comiencen en la neurona i y terminen en la nueva.
					else if( generalInformation->layerToPlace( layer ) < generalInformation->layerToPlace( layerOtherNeuron ) )
					{
						int input = positionNewNeuron;
						int output= i;
						int ref1,ref2;
						// En el caso de conexiones desde la neurona i hacia la neurona nueva no se aceptan las conexiones (-2)
						// Primero se deben saber los índices en el mapa de referencia cuando los inputs y outputs son cambiados
						tie( ref1, ref2 )=localNeuronsInOut_TO_referencesNeurons({output,input});
						neuronsReferencesForCreateNewNeurons.at( ref1 ).at( ref2 ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( ref1 ).at( ref2 ) = -2;

						// Se quitan las posibilidades de conexión
						availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( positionNewNeuron ) -=1; 
						availableNumberOfNeuronMutationsInRelationToNeuron.at( positionNewNeuron ) -=1; 
						amountOfPosiblySynapticWeightMutation -= 1;
						amountOfPosiblyNeuronMutation -= 1;
					}
					// En el otro caso: se tiene que la neurona actual tiene un layer mayor que la neurona i, entonces se niegan las conexiones desde la neurona actual a la neurona i.
					else
					{
						int input = positionNewNeuron;
						int output= i;
						// En el caso de conexiones desde la neurona nueva hacia la neurona i no se aceptan las conexiones (-2)
						neuronsReferencesForCreateNewNeurons.at( input ).at( output ) = -2;
						neuronsReferencesForCreateNewSynapticWeight.at( input ).at( output ) = -2;

						// Se quitan las posibilidades de conexión
						availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( positionNewNeuron ) -=1; 
						availableNumberOfNeuronMutationsInRelationToNeuron.at( positionNewNeuron ) -=1; 
						amountOfPosiblySynapticWeightMutation -= 1;
						amountOfPosiblyNeuronMutation -= 1;
					}
				}
			}
		//================================================================	
	}

	void ANN::createInitialANN()
	{
		// Se sabe la cantidad de input y outputs lo que es suficiente para crear una red neuronal inicial.
		// La cantindad total de neuronas al finalizar el proceso será:
		amountOfPosiblyNeuronMutation=0;
		amountOfPosiblySynapticWeightMutation=0;

		// Se inicializan las listas de mutaciones de informationBetweenOrganismAndLife
		generalInformation->initialize(inputsAmount,outputsAmount);

		// Las primeras neuronas en ser agregadas serán los inputs, que rigurosamente hablando no son neuronas, pero en la implementación derivan de neurona para simplificar la programación.
		// Estarán en el layer 0;
		for (int i = 0; i < inputsAmount; ++i)
		{
			int position = neuron_vector.size();
			inputsInNeuron_vector.push_back(position);
			addInitialStructureNeuron( new Input( position,-1,-1, LAYER_INPUT ) );
		}

		//Ahora se agregan las neuronas output
		for (int i = 0; i < outputsAmount; ++i)
		{
			int position = neuron_vector.size();
			outputsInNeuron_vector.push_back(position);
			addInitialStructureNeuron( prototypeNeuron->createNew(position , -1, -1, LAYER_OUTPUT ) );
		}
		
		// SE AGREGAN LAS CONEXIONES
		for (int i = 0; i < outputsAmount; ++i)
		{
			for (int j = 0; j < inputsAmount; ++j)
			{
				addSynapticWeight(  inputsInNeuron_vector.at(j), outputsInNeuron_vector.at(i) );
			}
		}

	}


	void ANN::newSynapticWeightMutation( )
	{
		// A diferencia del caso de agregar neuronas, agregar conexiones sinápticas no siempre es posible un caso de ejemplo es una neurona inicial la cual no permite conexiones recursivas.

		// Paso 1: resolver caso en que no hay posibles mutaciones.
		//=============================================================
			if( amountOfPosiblySynapticWeightMutation == 0 )
			{ // Esto sólo puede ocurrir con mutaciones de conexión sináptica.
				std::cout << "amountOfPosiblySynapticWeightMutation is 0 then newSynapticWeightMutation do not do anything" << std::endl;
				return;
			}
		//=============================================================

		// Paso 2: se obtiene aleatoriamente las marcas históricas de las neuronas inicial input e inicial ourput
		//=============================================================
			int historicalMarkNeuronIn;
			int historicalMarkNeuronOut;
			tie(historicalMarkNeuronIn,historicalMarkNeuronOut) = findNeuronsForNewSynapticMutation();
		//=============================================================

		// Se crea la conexión sináptica.
		// Finalmente se agrega la conexión sináptica a la ANN
		//=============================================================
			addSynapticWeight(historicalMarkNeuronIn,historicalMarkNeuronOut );
		//=============================================================
	}



	
	void ANN::updatePresentList(int layersAmount)
	{
		while((int)neuronsAtLayer.size()<=layersAmount)
		{
			std::vector <int> extension (1,-1);
			neuronsAtLayer.push_back(extension);
		}	
	}

	
	double organismDistance(ANN * ann1, ANN * ann2)
	{
		
		double DN=0.0;
		double DW=0.0;
		double W=0.0;
		double N=0.0;
	
		// NEURONAS
		//========================================================================
		for ( int i = 0; i < (int)ann2->neuron_vector.size() ; ++i )
		{
			int historicalMarkMotherNeuron = ann2->neuron_vector.at( i )->getHistoricalMark();
			int localNeuronPositionInFather = ann1->historicalMark_To_localNeuron[ historicalMarkMotherNeuron ];
			if(  localNeuronPositionInFather == 0 && historicalMarkMotherNeuron != 0) // entonces no existe en el padre.
			{
				// Hay una diferencia
				DN++;
			}
			else
			{ 
				//ann2->neuron_vector.at( i )->printState();
				//std::cerr << "localNeuronPositionInFather: " << localNeuronPositionInFather << "\thistoricalMarkMotherNeuron: "<<historicalMarkMotherNeuron << std::endl;
				//ann1->neuron_vector.at( localNeuronPositionInFather )->printState();
				N += ann1->neuron_vector.at( localNeuronPositionInFather )->getDistance( ann2->neuron_vector.at( i ) );
			}
		}

		// Hay que hacer lo inverso para contabilizar los casos en que la neurona pertenece al padre nada mas.
		for (int i = 0; i < (int)ann1->neuron_vector.size(); ++i)
		{
			int historicalMarkFatherNeuron = ann1->neuron_vector.at( i )->getHistoricalMark();
			int localNeuronPositionInMother = ann2->historicalMark_To_localNeuron[ historicalMarkFatherNeuron ];
			if(  localNeuronPositionInMother == 0 && historicalMarkFatherNeuron != 0) // entonces no existe en el padre.
			{
				// Hay una diferencia
				DN++;
			}
		}

		// CONEXIONES SINÁPTICAS
		//========================================================================
		for (int i = 0; i < (int)ann2->synapticWeight_vector.size(); ++i)
		{
			int innovationMother = ann2->synapticWeight_vector.at(i)->getInnovation();
			int localInnovationPositionFather = ann1->innovarion_to_localSynapticWeightVectorPosition[innovationMother];

			if(  localInnovationPositionFather == 0 && innovationMother !=0 ) // entonces no existe en el padre.
			{
				DW++;
			}
			else
			{ // Existe uno en el padre.
				W += ann1->synapticWeight_vector.at(localInnovationPositionFather)->getDistance(ann2->synapticWeight_vector.at(i));
			}
		}
		for (int i = 0; i < (int)ann1->synapticWeight_vector.size(); ++i)
		{
			int innovationFather = ann1->synapticWeight_vector.at(i)->getInnovation();
			int localInnovationPositionMother = ann2->innovarion_to_localSynapticWeightVectorPosition[innovationFather];

			if(  localInnovationPositionMother == 0 && innovationFather !=0 ) // entonces no existe en el padre.
			{
				DW++;
			}
		}
	
		double distance =W + N + DW * ann1->ConstantOFDiferencesInStructureOfSynapticWeight + DN * ann1->ConstantOFDiferencesInStructureOfNeurons;
		return distance;
	}

	void ANN::spreadVoltage()
	{
		// Aunque los inputs ya tienen cargadas sus entradas puede ser que existan conexiones hacia atrás que alimenten también a las entradas así que el proceso igualmente comienza en el layer 0.
		//Layer a layer se iran obtienendo las neuronas de cada layer y se les ira sumando las entradas de todas las conexiones de las otra capas que llegan a ellas, finalmente se obtiene su salida y esta es pasada a las conexiones salientes de ella, así se tiene que toda la red se conecta. Notar que al finalizar una capa recién se actualizan los valores de las conexiones sinápticas que comienzan en esa capa, eso es para lograr simetría en caso de que existan conexiones cruzadas en un mismo layer.
		
		
		// Antes de comenzar se actualizan el orden de las capas según el orden de todas las capas en NEAT.
		std::vector <int> layerOrdererList =  generalInformation->layerOrdererList(); // Acá están los layer ordenadamente según la distancia entre los input y los output.
		// Se actualiza la lista de ordenes en este organismo, esto se realiza para no tener problemas ante la posibilidad de que un layer no esté dentro de los layes del organismo.
		updatePresentList( layerOrdererList.size() );

		
		for (unsigned int i = 0; i < layerOrdererList.size() ; ++i)
		{

			// Se corrobora que el layer sea un layer existente en esta ANN
			//=======================================================================
				int currentLayer = layerOrdererList.at( i );
				if(neuronsAtLayer.at( currentLayer ).at( 0 ) == -1)//Si es -1 es porque no hay neuronas en aquella capa.
				{
					continue;
				}
			//=======================================================================

			std::vector <int> synapticWeightThatStartInThisLayer; 

			// Se obtiene neurona a neurona cada una del layer actual.
			//=======================================================================
				for (unsigned int neuronIterator = 0 ;  neuronIterator < neuronsAtLayer.at( currentLayer ).size() ; ++neuronIterator )
				{
					int historicalMarkOfSelectedNeuron = neuronsAtLayer.at( currentLayer ).at( neuronIterator );
					int positionInNeuronVector = historicalMark_To_localNeuron[ historicalMarkOfSelectedNeuron ] ;


					// Se obtienen todas las conexiones sinápticas que terminan en esta neurona y se le suman sus voltajes de salida a la entrada de la neurona.
					//=====================================================================
						std::vector <int> incomingConnections = neuron_vector.at( positionInNeuronVector )->getIncomingConnections();
						for ( unsigned int k = 0; k < incomingConnections.size(); ++k )
						{
							int positionInSynapticWeightVector = innovarion_to_localSynapticWeightVectorPosition[ incomingConnections.at( k ) ];
							// Por cada conexión entrante se suma al voltaje entrante a la neurona.
							neuron_vector.at( positionInNeuronVector )->sumIncomingConnectionsOutputs( synapticWeight_vector.at( positionInSynapticWeightVector )->getOutput( ) );
						}
					//=====================================================================

					// Dado que ya están todas las conexiones entrantes ya evaluadas se puede proceder a evaluar la salida de la neurona.
					double outputOfNeuron = neuron_vector.at( positionInNeuronVector )->eval();


					// La salida de la neurona es pasada a la entrada de todas las conexiones sinápticas que comienzan de esta neurona.
					//=====================================================================
						std::vector <int> outcomingConnections = neuron_vector.at(positionInNeuronVector)->getOutcomingConnections();
						for (unsigned int k = 0; k < outcomingConnections.size(); ++k)
						{
							int positionInSynapticWeightVector = innovarion_to_localSynapticWeightVectorPosition[ outcomingConnections.at( k ) ];
							synapticWeightThatStartInThisLayer.push_back( positionInSynapticWeightVector );
							synapticWeight_vector.at(positionInSynapticWeightVector)->setInput( outputOfNeuron );
						}
					//=====================================================================
				}
			//=======================================================================


			//  Se actualiza el valor de todas las conexiones sinápticas que comienzan desde esta capa.
			// Esto se realiza al final para mantener la simetria en la red neuronal, imaginar caso en que existan dos neuronas en la misma capa que tienen conexiones entre ellas, no es correcto que  una conexion actualice su salida antes que la segunda neurona calcule su entrada porque no pasaría lo mismo con la primera neurona y sería un problema sólo de orden.
			//=======================================================================
				for (unsigned int k = 0; k < synapticWeightThatStartInThisLayer.size(); ++k)
				{
					synapticWeight_vector.at( synapticWeightThatStartInThisLayer.at( k ) )->spread();
				}
			//=======================================================================
		}
	}


	std::tuple < int,int > ANN::localNeuronsInOut_TO_referencesNeurons(std:: vector<int> neuron_in_out)
	{
		if( neuron_in_out.at(0) > neuron_in_out.at(1) )
		{
			return make_tuple(neuron_in_out.at(0),neuron_in_out.at(1));
		}
		else
		{
			return make_tuple(neuron_in_out.at(1), 2*neuron_in_out.at(1)  - neuron_in_out.at(0));
		}
	}


	std::tuple < int,int >  ANN::referencesNeurons_TO_localNeuronsInOut(std:: vector<int> vect_1_vect_2)
	{
		if( vect_1_vect_2.at(0) > vect_1_vect_2.at(1) )
		{
			return make_tuple( vect_1_vect_2.at(0) , vect_1_vect_2.at(1) );
		}

		else
		{
			return make_tuple( 2*vect_1_vect_2.at(0) - vect_1_vect_2.at(1) , vect_1_vect_2.at(0) );
		}
	}

	std::tuple < int, int, int, int, int, int > ANN::findNeuronsForNewNeuronMutation()
	{
		int 	sum_prev,
			sum,
			diference,
			newNeuronIndicator;

		newNeuronIndicator =  rand()%amountOfPosiblyNeuronMutation + 1; // Con esto definimos completamente cuál será la neurona nueva. 
		unsigned int j = 0;
		unsigned int i = 0;
		unsigned int counter=0;
		sum=0;
		sum_prev=0;
		for ( i = 0; i < neuronsReferencesForCreateNewNeurons.size(); ++i )
		{
			sum += availableNumberOfNeuronMutationsInRelationToNeuron.at( i );
			if( sum >= newNeuronIndicator ) 
			{		
				break;
			}
			sum_prev = sum;
		}

		if( i == neuronsReferencesForCreateNewNeurons.size() )
		{
			testPrint();
			std::cerr << "ANN::findNeuronsForNewNeuronMutation:: 1st Component not found" << std::endl;
			std::cerr << "amountOfPosiblyNeuronMutation: "<< amountOfPosiblyNeuronMutation << std::endl;
			for ( i = 0; i < neuronsReferencesForCreateNewNeurons.size(); ++i )
			{
				sum += availableNumberOfNeuronMutationsInRelationToNeuron.at( i );
				std::cerr << "sum: " << sum << std::endl;
			}
			exit( EXIT_FAILURE );
		}
		// Ahora que se sabe la primera componente se debe buscar por la segunda para definir completamente la mutación. 
		diference = newNeuronIndicator - sum_prev;
		
		for ( j = 0; j < neuronsReferencesForCreateNewNeurons.at( i ).size(); ++j ) 
		{
			if( neuronsReferencesForCreateNewNeurons.at( i ).at( j ) == -1 )
			{
				if( (int)++counter == diference )
				{
					break;
				}
			}
		}
		if( j == neuronsReferencesForCreateNewNeurons.at( i ).size() )
		{
			std::cerr << "ANN::findNeuronsForNewNeuronMutation:: 2nd Component not found" << std::endl;
			exit( EXIT_FAILURE );
		}
		int NeuronRef_1 = i;
		int NeuronRef_2 = j;
		int localNeuronIn;
		int localNeuronOut;
		

		tie( localNeuronIn , localNeuronOut ) = referencesNeurons_TO_localNeuronsInOut( {NeuronRef_1 , NeuronRef_2 } );

		int histoticalMarkNeuronIn = neuron_vector.at( localNeuronIn )->getHistoricalMark( );
		int histoticalMarkNeuronOut = neuron_vector.at( localNeuronOut )->getHistoricalMark( );


		return make_tuple (histoticalMarkNeuronIn, histoticalMarkNeuronOut, localNeuronIn, localNeuronOut, NeuronRef_1, NeuronRef_2 );
	}



	std::tuple < int, int > ANN::findNeuronsForNewSynapticMutation()
	{
		int sum_prev,
			sum,
			localNeuronOut,   
			localNeuronIn,
			diference,
			newSynapticWeightIndicator,
			counter;

		unsigned int i,j;
		std:: cerr << "amountOfPosiblySynapticWeightMutation: " << amountOfPosiblySynapticWeightMutation << std::endl;
		newSynapticWeightIndicator =  ( rand()%amountOfPosiblySynapticWeightMutation ) + 1; // Con esto definimos completamente cuál será la neurona nueva. 
		j=0;
		counter=0;
		sum=0;
		sum_prev=0;

		// Primero se debe encontrar cuales serán las neuronas inicial input e inicial output, para eso se busca primero alguna neurona con la que esta mutación va a tener relación.
		// Ojo que aun encontrado el primer vector no se sabe si "i" corresponderá a la neurona inicial input o inicial output dado como se pensó, eso se sabrá posteriormente.
		for (i = 0; i < neuronsReferencesForCreateNewSynapticWeight.size(); ++i)
		{
			sum += availableNumberOfSynaptinWeightMutationsInRelationToNeuron.at( i ); 
			if(sum >= newSynapticWeightIndicator) 
			{
				break;
			}
			sum_prev=sum;
		}

		if( i == neuronsReferencesForCreateNewSynapticWeight.size() )
		{

			std::cerr << "ERROR::ANN::findNeuronsForNewSynapticMutation::1:: No synapticWeight mutation available" << std::endl;
			std::cerr << amountOfPosiblySynapticWeightMutation << std::endl;
			testPrint();
			exit(EXIT_FAILURE);
		}
		// Ahora que se sabe la primera componente se debe buscar por la segunda para definir completamente la mutación. 
		
		diference = newSynapticWeightIndicator -  sum_prev; // El -1 es importante
		
		for (j = 0; j < neuronsReferencesForCreateNewSynapticWeight.at( i ).size(); ++j) 
		{
			if( neuronsReferencesForCreateNewSynapticWeight.at( i ).at( j ) == -1 )
			{
				if(++counter == diference)
				{
					break;
				}
			}
		}

		if( j == neuronsReferencesForCreateNewSynapticWeight.at( i ).size() )
		{
			std::cerr << "ERROR::ANN::findNeuronsForNewSynapticMutation::2:: No synapticWeight mutation available" << std::endl;
			exit(EXIT_FAILURE);
		}


		// Ahora a través de i y j se obtendrán las neuronas inicial input e inicial output que fueron elejidas.
		tie(localNeuronIn, localNeuronOut) =  referencesNeurons_TO_localNeuronsInOut( { (int)i , (int)j } );


		// Además será bastante usada la marca histórica de ambas neuronas en lo que queda del método
		int histoticalMarkNeuronIn = neuron_vector.at( localNeuronIn )->getHistoricalMark();
		int histoticalMarkNeuronOut = neuron_vector.at( localNeuronOut )->getHistoricalMark();

		return make_tuple( histoticalMarkNeuronIn , histoticalMarkNeuronOut );
	}

	int ANN::getOrganismOutputSize()
	{
		return outputsAmount;
	}
}