#include "ANN.hpp"

using namespace std;

namespace NEATSpikes
{
	int ANN::id = 0;

	ANN::ANN()
	{
		identificator = id++;
		age = 0;
	}

	//Este metodo se llama una sola vez por todo NEAT
	ANN::ANN(Neuron * inputPrototype, Neuron * outputPrototype, Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * globalInformation)
	{ 
		this->inputPrototype = inputPrototype;
		this->outputPrototype = outputPrototype;
		this->prototypeNeuron = neuron;
		this->prototypeSynapticWeight = synapticWeight;
		this->globalInformation = globalInformation; // Así siempre se tiene una copia de la información de todas las redes neuronales.

		SetParametersFromUserDefinitionsPath( path_ANN_definitions ); // Acá se determinan los valores de las definiciones de usurio.

		identificator = id++;

		mutationControl = new MutationControl(globalInformation, &neuron_vector,  *ANNCanHaveConnectionsBack, &synapticWeight_vector, &innovationToSynapticWeight, &historicalMarkToNeuron, neuron, synapticWeight , &historicalMarkAtLayer );

		createInitialANN();
		age = 0;
	}

	ANN::~ANN()
	{
		for (unsigned int i = 0; i < (neuron_vector).size(); ++i)
		{
			delete((neuron_vector).at(i));
		}
		
		for (unsigned int i = 0; i < (synapticWeight_vector).size(); ++i)
		{
			delete((synapticWeight_vector).at(i));
		}

		(historicalMarkToNeuron).clear();
		(innovationToSynapticWeight).clear();
		(historicalMarkAtLayer).clear();
		inputsInNeuron_vector.clear();
		outputsInNeuron_vector.clear();
	}

	

	void ANN::printState()
	{ 
		std::cout << "ANN id: " << identificator << "\nFitness: " << fitness <<  "\nAge: " << age << std::endl;
		for (unsigned int i = 0; i < neuron_vector.size(); ++i)
		{
			neuron_vector.at(i)->printState();
		}
		for (unsigned int i = 0; i < synapticWeight_vector.size(); ++i)
		{
			synapticWeight_vector.at(i)->printState();
		}
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
				int input_i = (historicalMarkToNeuron).at( inputsInNeuron_vector.at( i ) );
				(neuron_vector).at( input_i )->sumIncomingConnectionsOutputs( inputs.at( i ) );
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
				int localNeuronOutput = (historicalMarkToNeuron).at( outputsInNeuron_vector.at( i ) );
				final.push_back( (neuron_vector).at( localNeuronOutput )->getLastOutput( ) );
			}
		//=====================================================================

		return final;
	}



	void ANN::mutate()
	{
		// 1) Primero se revizara neurona a neurona si es que debe o no mutar.
		// 2) Se revizaran todas las conexiones sinápticas para ver si deben mutar.
		// 3) Se reviza si hay mutacion que cree una nueva neurona.
		// 4) Se reviza si hay mutacion que cree una nueva conexión sináptica.

		// Paso 1: se revizara neurona a neurona si es que debe o no mutar
		//=============================================
			for (unsigned int i = 0; i < (neuron_vector).size() ; ++i)
			{
				if ( rand()/(double)RAND_MAX <=  *probabilityOfNeuronMutation)
				{	
					(neuron_vector).at(i)->mutate();
				}
			}	
		//=============================================

		// 2) Se revizarán todas las conexiones sinapticas para ver si deben mutar.
		//=============================================
			for (unsigned int i = 0; i < (synapticWeight_vector).size() ; ++i)
			{
				if ( rand()/(double)RAND_MAX <=  *probabilityOfSynapticWeightMutation)
				{
				(synapticWeight_vector).at(i)->mutate();
				}
			}
		//=============================================

		// 3) Se reviza si hay mutacion que cree una nueva neurona
		//=============================================
			if ( rand()/(double)RAND_MAX <=  *probabilityOfNewNeuronMutation )
			{
				newNeuronMutation();
			}
		//=============================================
		// 4) Se reviza si hay mutacion que cree una nueva conexion sinaptica.
		//=============================================
			if ( rand()/(double)RAND_MAX <=  *probabilityOfNewSynapticWeightMutation)
			{
				newSynapticWeightMutation();
			}
		//=============================================
		
	}

	
	/*
		Notar que hay que tomar en cuenta que primero se deben agergar las neuronas que tanto sus neuronas inicial input como outputs ya existan en la red neuronal, y luego iterativamente se agergaran las que ya posean ambas (su inicial input y inicial output) en la red dada la iteracion anterior.
	*/
	ANN * crossover(ANN * father, ANN * mother)
	{
		// Lo primero sera crear a un hijo en semejanza de su padre.
		//==========================================================
		ANN * children = father->duplicate();

		//Luego hay que ir agregando iterativamente las neuronas de la madre. Notar que hay que tomar en cuenta que primero se deben agergar las neuronas que tanto sus neuronas inicial input como outputs ya existan en la red neuronal, y luego iterativamente se agergaran las que ya posean ambas (su inicial input y inicial output) en la red dada la iteracion anterior.
		//============================================================
		int neuronsAmount = (int)(mother->neuron_vector).size();
		int flagIsAdded [neuronsAmount]; // un arreglo del mismo tamano que la cantidad de neuronas de la madre pero lleno de ceros, se iran poniendo unos mediante se vayan agregando las neuronas de la madre que esten en el mismo lugar en su vector de neuronas.
		std::fill_n(flagIsAdded, neuronsAmount, 0);
		int counter = 0;// Cantidad de neuronas de la madre ya agregadas.


		int i = 0;
		while(true)
		{
			if(counter == neuronsAmount) // Ya se han agregado todas las neuronas de la madre
			{
				break; // Se sale del loop while(true)
			}

			if(flagIsAdded[i] == 0)
			{

				int historicalOfNeuronIn = (mother->neuron_vector).at(i)->getInitialNeuronInHistoricalMark();
				int historicalOfNeuronOut = (mother->neuron_vector).at(i)->getInitialNeuronOutHistoricalMark();

				//Primero se verificara si son input o output
				if(historicalOfNeuronIn == INPUT_INICIAL_IN_OUT || historicalOfNeuronIn == OUTPUT_INICIAL_IN_OUT) 
				{
					flagIsAdded[i] = 1; // En cualquiera de los dos casos ya se sabe que esta neurona ya fue procesada.
					
					//Con 50% se usa los valores de la madre.
					if( rand()/(double)RAND_MAX > 0.5 )
					{
						//Se copian los valores de la neurona de la madre en la neurona que ya existe (heredada del padre).
						int neuronPosition = (children->historicalMarkToNeuron).at((mother->neuron_vector).at(i)->getHistoricalMark());
						(children->neuron_vector).at(neuronPosition)->copyValues((mother->neuron_vector).at(i));
					}
					++counter;
					++i;
					continue;

				}

				//Primero se verificara que el historical mark al menos es menor que el mas grande de los que existen en la red neuronal del hijo (que proviene del padre), en otro caso no tiene sentido pensar que existan en esta red neuronal.
				if((int)(children->historicalMarkToNeuron).size() > historicalOfNeuronIn &&  (int)(children->historicalMarkToNeuron).size() > historicalOfNeuronOut )
				{

					//Ahora se verificara si existen ambos en la red neuronal. 
					if((children->historicalMarkToNeuron).at(historicalOfNeuronIn) != -1 && (children->historicalMarkToNeuron).at(historicalOfNeuronOut) != -1)
					{
						// En este punto se sabe que la red neuronal posee ambas neuronas, la input y la output de esta neurona de la madre, ahora hay una ultima prueba que hacer, es necesario saber si esta nerona existe en el hijo o si solo existe en la madre, en el primer caso existe una proibabilidad de 0.5 en que se use el de la madre, en el segundo caso se debe agregar directamente sin mas.
						flagIsAdded[i] = 1; // En cualquiera de los dos casos ya se sabe que esta neurona ya fue procesada.
						++counter;
						//primero se revizara si el hitoricalMark al menos es menor que el del mayor historicalMark del hijo actualmente.
						if( (int)(children->historicalMarkToNeuron).size() > (mother->neuron_vector).at(i)->getHistoricalMark() )
						{
							//Si el historical mark esta en la red neuronal entonces debe existir una transformacion entre el historicalMark y el vector de neuronas
							if( (children->historicalMarkToNeuron).at((mother->neuron_vector).at(i)->getHistoricalMark()) != -1)
							{
								//En este punto hay un 0.5 de probabildiad de que la neurona finalmente sea de la madre.
								if( rand()/(double)RAND_MAX > 0.5 )
								{
									//Se copian los valores de la neurona de la madre en la neurona que ya existe (heredada del padre).
									int neuronPosition = (children->historicalMarkToNeuron).at((mother->neuron_vector).at(i)->getHistoricalMark());
									(children->neuron_vector).at(neuronPosition)->copyValues((mother->neuron_vector).at(i));
								}
							}
							else
							{ // Dado que no existe en la red del hijo se agrega
								Neuron * newNeuron = (mother->neuron_vector).at(i)->duplicate();
								children->addNeuron ( newNeuron );
							}
						}
						else
						{	// Dado que no existe en la red del hijo se agrega
							Neuron * newNeuron = (mother->neuron_vector).at(i)->duplicate();
							children->addNeuron ( newNeuron );
						}

							
					}
						
				}
			}
			++i;
		}

		//Ahora simplemente se agregan las connecciones sinapticas de la madre
		//Se sabe a priori que dado que estan todas las neuronas deben estar las neuronas que son conectadas por las connecciones sinapticas que se van a introducir asi que no deberia haber problemas de esa indole, lo que hay que hacer es identificar las conecciones que pertenecen solo a uno de los dos padres y en otro caso otorgar un 0.5 de probabilidad de que se quede con alguno de los dos (recordar que ya es una copia del padre por lo tanto lo unico que cabe preguntarse es si debe cambiarse los valores por los de la madre.)
		//===============================================================================================
		for (unsigned int i = 0; i < (mother->synapticWeight_vector).size() ; ++i)
		{
			//Primero se revizara si el innovation es menor que el maximo innovation que existe al momento.
			if(  (int)(children->innovationToSynapticWeight).size() > (mother->synapticWeight_vector).at(i)->getInnovation() )
			{


				//Ahora se verificara si existe tal innovacion en la red neuronal actual, de existir entoces deberia existir un mapa desde innovationToSynapticWeight con valor diferente de -1.
				if( (children->innovationToSynapticWeight).at((mother->synapticWeight_vector).at(i)->getInnovation()) != -1 )
				{
					int synapticPosition = (children->innovationToSynapticWeight).at((mother->synapticWeight_vector).at(i)->getInnovation());
					//Si en alguno esta deshabilitado se mantendra deshabilitado
					if( !(children->synapticWeight_vector).at(synapticPosition)->getEnable() )
					{
						continue;
					}
					else if(!(mother->synapticWeight_vector).at(i)->getEnable())
					{
						int synapticPosition = (children->innovationToSynapticWeight).at((mother->synapticWeight_vector).at(i)->getInnovation());
						(children->synapticWeight_vector).at(synapticPosition)->copyValues((mother->synapticWeight_vector).at(i));
					}

					//Dado que existe la coneccion tanto en el padre como en la madre, existira un 0.5 de probabiliad de que los valores sean de la madre (sino se mantiene como esta porque ya es una copia del padre)
					if (rand()/(double)RAND_MAX > 0.5)
					{
						(children->synapticWeight_vector).at(synapticPosition)->copyValues((mother->synapticWeight_vector).at(i));
					}
				}
				else
				{
					//Dado que no existe en el hijo (copai del padre) se debe agregar.
					SynapticWeight * newSynaptic = (mother->synapticWeight_vector).at(i)->duplicate();
				children->addSynapticWeight(newSynaptic);
				}
			}
			else
			{//Si el maximo inovation que se tiene es menor que el que se esta agregando es porque definitivamente no se posee tal coneccion en la red actual
				SynapticWeight * newSynaptic = (mother->synapticWeight_vector).at(i)->duplicate();
				children->addSynapticWeight(newSynaptic);
			}
		}

		return children;
	}

	void ANN::testPrint()
	{
		globalInformation->printLayers();
		mutationControl->print();

	}

	double ANN::getFitness()
	{
		return fitness;
	}

	void ANN::setFitness(double _fitness)
	{
		fitness = _fitness;
	}

	ANN * ANN::createNewWithSameTopologieButDiferentValues()
	{
		 ANN * newAnn = this->duplicate();
		
		// Cada neurona obtiene valores aleatorios
		for (unsigned int i = 0; i < ((newAnn->neuron_vector)).size(); ++i)
		{
			((newAnn->neuron_vector)).at( i )->changeValuesRandomly();	
		}
		// Cada peso sinaptico obtiene valores aleatorios
		for (unsigned int i = 0; i < ((newAnn->synapticWeight_vector)).size(); ++i)
		{
			((newAnn->synapticWeight_vector)).at( i )->changeValuesRandomly();	
		}

		return newAnn;
	}

	void ANN::saveState(std::string pathToSave)
	{
		// Bastante largo, se deja para posterior trabajo.


		//=============================================
		// Neuronas
		mkdir( (pathToSave + "Neurons/" ).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );

		for (unsigned int i = 0; i < neuron_vector.size(); ++i)
		{
			neuron_vector.at(i)->saveState(pathToSave + "Neurons/" );
		}
		neuron_vector.at(0)->saveUserDefinitions(pathToSave + "Neurons/" );



		//=============================================
		// Conecciones sinapticas
		mkdir( (pathToSave + "Synapses/" ).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );

		for (unsigned int i = 0; i < synapticWeight_vector.size(); ++i)
		{
			synapticWeight_vector.at(i)->saveState(pathToSave + "Synapses/" );
		}
		synapticWeight_vector.at(0)->saveUserDefinitions(pathToSave + "Synapses/" );

		// Se guarda la edad.

		std::ofstream parameters;
		parameters.open(pathToSave + "ANN", std::ios::out);
		parameters << "age " << age << std::endl;
		parameters.close();

	}

	void ANN::load(std::string PathWhereIsSaved, Neuron * inputPrototype, Neuron * outputPrototype, Neuron * neuron, SynapticWeight * synapticWeight, std::string path_ANN_definitions, GlobalInformation * globalInformation )
	{

		FILE * archive; // El archivo es cargado en esta variable
		//Las siguientes variables son usadas para leer las lineas del archivo
		char * lineRd = NULL;
		size_t len = 0;
		ssize_t read;
		
		char * token_1; // Aqui se guardan los strings
		char * token_2; // y aquí el valor 
		char * saveptr; // variable para indicar el lugar donde quedó la ultima lectura del strtok_r
		std::map <std::string, double> parameters; 
		char delimiters[] = " \n\t"; // Los delimitadores usados.
		// Se abre el archivo donde están las definiciones de usuario.
		//=============================saves============================================================
			archive = fopen( (PathWhereIsSaved + "ANN").c_str() , "r" );
			if ( archive == NULL )
			{
				std::cerr << "ERROR::BasicNeuron::load::Could not read the file" << std::endl;
				exit( EXIT_FAILURE );
			}
		//=========================================================================================
		//Se lee linea a linea y se guarda en el mapa parameters
		//=========================================================================================
			while ( ( read = getline(&lineRd, &len, archive) ) != -1) 
			{
				token_1 = strtok_r(lineRd, delimiters, &saveptr); // Por si acaso se desea hacer uso de hilos.
				token_2 = strtok_r(NULL, delimiters, &saveptr);
				parameters[token_1]=atof(token_2); // Se comieenza a llenar el mapa.
			}
			fclose( archive ); // Ya no se usa el recurso así que se cierra.

		age = int(parameters["age"]);


		this->inputPrototype = inputPrototype;
		this->outputPrototype = outputPrototype;
		this->prototypeNeuron = neuron;
		this->prototypeSynapticWeight = synapticWeight;
		this->globalInformation = globalInformation; // Así siempre se tiene una copia de la información de todas las redes neuronales.

		SetParametersFromUserDefinitionsPath( path_ANN_definitions ); // Acá se determinan los valores de las definiciones de usurio.

		identificator = id++;

		mutationControl = new MutationControl(globalInformation, &neuron_vector,  *ANNCanHaveConnectionsBack, &synapticWeight_vector, &innovationToSynapticWeight, &historicalMarkToNeuron, neuron, synapticWeight , &historicalMarkAtLayer );


		std::ifstream infile( PathWhereIsSaved + "Neurons/index" );
		std::string line;
		//Se procede a cargar las neuronas.
		//Primero las inputs, luego las outputs y finalmente las hidden, pues ese es el orden natural que poseen y en el que naturalmente tambien fueron guardados.

		for (int i = 0; i < *inputsAmount; ++i)
		{
			infile >> line;
			int position = (neuron_vector).size();
			inputsInNeuron_vector.push_back(position);
			Neuron * newNeuron = inputPrototype->createNewInput();
			newNeuron->load( PathWhereIsSaved + "Neurons/" + "BN" + line ); // OJO E BN, no deberia ir.
			mutationControl->AddInitialNeuron( newNeuron );
		}

		 
		for (int i = 0; i < *outputsAmount; ++i)
		{
			infile >> line;
			int position = (neuron_vector).size();
			outputsInNeuron_vector.push_back(position);
			Neuron * newNeuron = outputPrototype->createNewOutput();
			newNeuron->load( PathWhereIsSaved + "Neurons/" + "BN" + line ); // OJO E BN, no deberia ir.
			mutationControl->AddInitialNeuron( newNeuron );
		}

		while ((infile >> line).good())
		{
				Neuron * newNeuron = prototypeNeuron->createNew(0,0);
				newNeuron->load( PathWhereIsSaved + "Neurons/" + "BN" + line ); // OJO E BN, no deberia ir.
				mutationControl->addNeuron( newNeuron );
		}

		infile.close();


		std::ifstream infileSynapses( PathWhereIsSaved + "Synapses/index" );

		while ((infileSynapses >> line).good())
		{
				SynapticWeight * newNeuron = prototypeSynapticWeight->createNew(prototypeSynapticWeight,0,0);
				newNeuron->load( PathWhereIsSaved + "Synapses/" + "BSW" + line ); // OJO E BN, no deberia ir.
				mutationControl->addSynapticWeight( newNeuron );
		}


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
		//=============================saves============================================================
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
			probabilityOfSynapticWeightMutation = new double(UserDefinitions[ "Probability_Of_Synaptic_Weight_Mutation" ]);
			probabilityOfNeuronMutation = new double(UserDefinitions[ "Probability_Of_Neuron_Mutation" ]);
			probabilityOfNewNeuronMutation = new double(UserDefinitions[ "Probability_Of_New_Neuron_Mutation" ]);
			probabilityOfNewSynapticWeightMutation = new double(UserDefinitions["Probability_Of_New_Synaptic_Weight_Mutation"]);
			inputsAmount = new int(UserDefinitions[ "inputsAmount" ]);
			outputsAmount= new int(UserDefinitions[ "outputsAmount" ]);
			ConstantOFDiferencesInStructureOfSynapticWeight = new double(UserDefinitions[ "ConstantOFDiferencesInStructureOfSynapticWeight" ]);
			ConstantOFDiferencesInStructureOfNeurons = new double(UserDefinitions[ "ConstantOFDiferencesInStructureOfNeurons" ]);
			ANNCanHaveConnectionsBack = new bool(UserDefinitions[ "ANNCanHaveConnectionsBack" ]);
			organismLifeExpectation = new double(UserDefinitions[ "organismLifeExpectation" ]);
		//=========================================================================================
		
		// =========================================================================================
		// Se reviza si estan todos los datos en sus correspondientes intervalos
			if(*probabilityOfNewSynapticWeightMutation > 1 || *probabilityOfNewSynapticWeightMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfNewSynapticWeightMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(*probabilityOfNewNeuronMutation > 1 || *probabilityOfNewNeuronMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfNewNeuronMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(*probabilityOfNeuronMutation > 1 || *probabilityOfNeuronMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfNeuronMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(*probabilityOfSynapticWeightMutation > 1 || *probabilityOfSynapticWeightMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfSynapticWeightMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}

			if( *organismLifeExpectation < 0.0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error organismLifeExpectation must be greater than 0" << std::endl;
				exit(EXIT_FAILURE);
			}
		//=========================================================================================
	}

	void ANN::saveUserDefinitions(std::string pathToSave)
	{
		std::ofstream userDefinitions;
		userDefinitions.open(pathToSave+"userDefinitions", std::ios::out);
		userDefinitions << "Probability_Of_Synaptic_Weight_Mutation " << *probabilityOfSynapticWeightMutation  << std::endl;
		userDefinitions << "Probability_Of_Neuron_Mutation " << *probabilityOfNeuronMutation  << std::endl;
		userDefinitions << "Probability_Of_New_Neuron_Mutation " << *probabilityOfNewNeuronMutation  << std::endl;
		userDefinitions << "Probability_Of_New_Synaptic_Weight_Mutation " << *probabilityOfNewSynapticWeightMutation << std::endl;
		userDefinitions << "inputsAmount " << *inputsAmount  << std::endl;
		userDefinitions << "outputsAmount " << *outputsAmount  << std::endl;
		userDefinitions << "ConstantOFDiferencesInStructureOfSynapticWeight " << *ConstantOFDiferencesInStructureOfSynapticWeight  << std::endl;
		userDefinitions << "ConstantOFDiferencesInStructureOfNeurons " << *ConstantOFDiferencesInStructureOfNeurons  << std::endl;
		userDefinitions << "ANNCanHaveConnectionsBack " << *ANNCanHaveConnectionsBack  << std::endl;
		userDefinitions << "organismLifeExpectation " << *organismLifeExpectation  << std::endl;
		userDefinitions.close();
	}

	void ANN::newNeuronMutation()
	{
		mutationControl->mutateNewNeuron();
	}

	void ANN::newSynapticWeightMutation( )
	{
		mutationControl->mutateNewSynapticWeight();
	}

	void ANN::addSynapticWeight( SynapticWeight * SW )
	{
		mutationControl->addSynapticWeight(SW);
	}


	void ANN::addNeuron( Neuron * N )
	{
		mutationControl->addNeuron(N);
	}



	void ANN::createInitialANN()
	{
		// Se sabe la cantidad de input y outputs lo que es suficiente para crear una red neuronal inicial.

		// Las primeras neuronas en ser agregadas serán los inputs, que rigurosamente hablando no son neuronas, pero en la implementación derivan de neurona para simplificar la programación.
		// Estarán en el layer 0;

		for (int i = 0; i < *inputsAmount; ++i)
		{
			int position = (neuron_vector).size();
			inputsInNeuron_vector.push_back(position);

			mutationControl->AddInitialNeuron( inputPrototype->createNewInput() );
		
		}

		//Ahora se agregan las neuronas output
		for (int i = 0; i < *outputsAmount; ++i)
		{
			int position = (neuron_vector).size();
			outputsInNeuron_vector.push_back(position);
			mutationControl->AddInitialNeuron( outputPrototype->createNewOutput(  ) );
		}
		
		// SE AGREGAN LAS CONEXIONES
		for (int i = 0; i < *outputsAmount; ++i)
		{
			for (int j = 0; j < *inputsAmount; ++j)
			{
				addSynapticWeight(  prototypeSynapticWeight->createNew(prototypeSynapticWeight, inputsInNeuron_vector.at(j), outputsInNeuron_vector.at(i)) );
			}
		}

	}






	
	void ANN::updatePresentList(int layersAmount)
	{
		while((int)(historicalMarkAtLayer).size() -1 <= layersAmount)
		{
			(historicalMarkAtLayer).push_back({});
		}	
	}

	
	double organismDistance(ANN * ann1, ANN * ann2)
	{
		
		double DN = 0.0;
		double DW = 0.0;
		double W = 0.0;
		double N = 0.0;
	
		// NEURONAS
		//========================================================================
		for ( int i = 0; i < (int)(ann2->neuron_vector).size() ; ++i )
		{
			int historicalMarkMotherNeuron = (ann2->neuron_vector).at( i )->getHistoricalMark();
			int localNeuronPositionInFather = (ann1->historicalMarkToNeuron).at( historicalMarkMotherNeuron );
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
				N += (ann1->neuron_vector).at( localNeuronPositionInFather )->getDistance( (ann2->neuron_vector).at( i ) );
			}
		}

		// Hay que hacer lo inverso para contabilizar los casos en que la neurona pertenece al padre nada mas.
		for (int i = 0; i < (int)(ann1->neuron_vector).size(); ++i)
		{
			int historicalMarkFatherNeuron = (ann1->neuron_vector).at( i )->getHistoricalMark();
			int localNeuronPositionInMother = (ann2->historicalMarkToNeuron).at( historicalMarkFatherNeuron );
			if(  localNeuronPositionInMother == 0 && historicalMarkFatherNeuron != 0) // entonces no existe en el padre.
			{
				// Hay una diferencia
				DN++;
			}
		}

		// CONEXIONES SINÁPTICAS
		//========================================================================
		for (int i = 0; i < (int)(ann2->synapticWeight_vector).size(); ++i)
		{
			int innovationMother = (ann2->synapticWeight_vector).at(i)->getInnovation();
			int localInnovationPositionFather = (ann1->innovationToSynapticWeight).at(innovationMother);

			if(  localInnovationPositionFather == 0 && innovationMother !=0 ) // entonces no existe en el padre.
			{
				DW++;
			}
			else
			{ // Existe uno en el padre.
				W += (ann1->synapticWeight_vector).at(localInnovationPositionFather)->getDistance((ann2->synapticWeight_vector).at(i));
			}
		}
		for (int i = 0; i < (int)(ann1->synapticWeight_vector).size(); ++i)
		{
			int innovationFather = (ann1->synapticWeight_vector).at(i)->getInnovation();
			int localInnovationPositionMother = (ann2->innovationToSynapticWeight).at(innovationFather);

			if(  localInnovationPositionMother == 0 && innovationFather !=0 ) // entonces no existe en el padre.
			{
				DW++;
			}
		}
	
		double distance =W + N + DW * (* (ann1->ConstantOFDiferencesInStructureOfSynapticWeight)) + DN * (* (ann1->ConstantOFDiferencesInStructureOfNeurons));
		return distance;
	}

	void ANN::spreadVoltage()
	{
		// Aunque los inputs ya tienen cargadas sus entradas puede ser que existan conexiones hacia atrás que alimenten también a las entradas así que el proceso igualmente comienza en el layer 0.
		//Layer a layer se iran obtienendo las neuronas de cada layer y se les ira sumando las entradas de todas las conexiones de las otra capas que llegan a ellas, finalmente se obtiene su salida y esta es pasada a las conexiones salientes de ella, así se tiene que toda la red se conecta. Notar que al finalizar una capa recién se actualizan los valores de las conexiones sinápticas que comienzan en esa capa, eso es para lograr simetría en caso de que existan conexiones cruzadas en un mismo layer.
		
		
		// Antes de comenzar se actualizan el orden de las capas según el orden de todas las capas en NEAT.
		std::vector <int> layerOrdererList =  globalInformation->layerOrdererList(); // Acá están los layer ordenadamente según la distancia entre los input y los output.
		// Se actualiza la lista de ordenes en este organismo, esto se realiza para no tener problemas ante la posibilidad de que un layer no esté dentro de los layes del organismo.
		updatePresentList( layerOrdererList.size() );

		
		for (unsigned int i = 0; i < layerOrdererList.size() ; ++i)
		{

			// Se corrobora que el layer sea un layer existente en esta ANN
			//=======================================================================
				int currentLayer = layerOrdererList.at( i );
				if((historicalMarkAtLayer).at( currentLayer ).size() == 0)//Si es 0 es porque no hay neuronas en aquella capa.
				{
					continue;
				}
			//=======================================================================

			std::vector <int> synapticWeightThatStartInThisLayer; 

			// Se obtiene neurona a neurona cada una del layer actual.
			//=======================================================================
				for (unsigned int neuronIterator = 0 ;  neuronIterator < (historicalMarkAtLayer).at( currentLayer ).size() ; ++neuronIterator )
				{
					int historicalMarkOfSelectedNeuron = (historicalMarkAtLayer).at( currentLayer ).at( neuronIterator );
					int positionInNeuronVector = (historicalMarkToNeuron).at (historicalMarkOfSelectedNeuron ) ;


					// Se obtienen todas las conexiones sinápticas que terminan en esta neurona y se le suman sus voltajes.
					//=====================================================================
						std::vector <int> incomingConnections = (neuron_vector).at( positionInNeuronVector )->getIncomingConnections();
						for ( unsigned int k = 0; k < incomingConnections.size(); ++k )
						{
							int positionInSynapticWeightVector = (innovationToSynapticWeight).at( incomingConnections.at( k ) );
							// Por cada conexión entrante se suma al voltaje entrante a la neurona.
							(neuron_vector).at( positionInNeuronVector )->sumIncomingConnectionsOutputs( (synapticWeight_vector).at( positionInSynapticWeightVector )->getOutput( ) );
						}
					//=====================================================================

					// Dado que ya están todas las conexiones entrantes ya evaluadas se puede proceder a evaluar la salida de la neurona.
					double outputOfNeuron = (neuron_vector).at( positionInNeuronVector )->eval();


					// La salida de la neurona es pasada a la entrada de todas las conexiones sinápticas que comienzan de esta neurona.
					//=====================================================================
						std::vector <int> outcomingConnections = (neuron_vector).at(positionInNeuronVector)->getOutcomingConnections();
						for (unsigned int k = 0; k < outcomingConnections.size(); ++k)
						{
							int positionInSynapticWeightVector = (innovationToSynapticWeight).at( outcomingConnections.at( k ) );
							synapticWeightThatStartInThisLayer.push_back( positionInSynapticWeightVector );
							(synapticWeight_vector).at(positionInSynapticWeightVector)->setInput( outputOfNeuron );
						}
					//=====================================================================
				}
			//=======================================================================


			//  Se actualiza el valor de todas las conexiones sinápticas que comienzan desde esta capa.
			// Esto se realiza al final para mantener la simetria en la red neuronal, imaginar caso en que existan dos neuronas en la misma capa que tienen conexiones entre ellas, no es correcto que  una conexion actualice su salida antes que la segunda neurona calcule su entrada porque no pasaría lo mismo con la primera neurona y sería un problema sólo de orden.
			//=======================================================================
				for (unsigned int k = 0; k < synapticWeightThatStartInThisLayer.size(); ++k)
				{
					(synapticWeight_vector).at( synapticWeightThatStartInThisLayer.at( k ) )->spread();
				}
			//=======================================================================
		}
	}

	int ANN::getOrganismOutputSize()
	{
		return *outputsAmount;
	}

	void ANN::loadParametersFromPrototype(ANN * prototype)
	{
		globalInformation = prototype->globalInformation;
		probabilityOfSynapticWeightMutation  = prototype->probabilityOfSynapticWeightMutation ;
		probabilityOfNeuronMutation = prototype->probabilityOfNeuronMutation;
		probabilityOfNewNeuronMutation = prototype->probabilityOfNewNeuronMutation;
		probabilityOfNewSynapticWeightMutation  = prototype->probabilityOfNewSynapticWeightMutation ;
		inputsAmount = prototype->inputsAmount;
		outputsAmount = prototype->outputsAmount;
		ConstantOFDiferencesInStructureOfSynapticWeight  = prototype->ConstantOFDiferencesInStructureOfSynapticWeight ;
		ConstantOFDiferencesInStructureOfNeurons = prototype->ConstantOFDiferencesInStructureOfNeurons;
		ANNCanHaveConnectionsBack = prototype->ANNCanHaveConnectionsBack;
		organismLifeExpectation = prototype->organismLifeExpectation;
	}

	ANN * ANN::duplicate()
	{
		ANN * final = new ANN();
		final->loadParametersFromPrototype(this);
		final->mutationControl = this->mutationControl->duplicate( &final->neuron_vector, &final->synapticWeight_vector, &final->innovationToSynapticWeight, &final->historicalMarkToNeuron, &final->historicalMarkAtLayer);
		for (unsigned int i = 0; i < (this->neuron_vector).size(); ++i)
		{
			(final->neuron_vector).push_back( (this->neuron_vector).at(i)->duplicate());
		}

		for (unsigned int i = 0; i < (this->synapticWeight_vector).size(); ++i)
		{
			(final->synapticWeight_vector).push_back((this->synapticWeight_vector).at(i)->duplicate());
		}
		
		for (unsigned int i = 0; i < (this->historicalMarkToNeuron).size(); ++i)
		{
			(final->historicalMarkToNeuron).push_back( (this->historicalMarkToNeuron).at( i ) ); 
		}

		for (unsigned int i = 0; i < (this->innovationToSynapticWeight).size(); ++i)
		{
			(final->innovationToSynapticWeight).push_back( (this->innovationToSynapticWeight).at( i ) ); 
		}

		for (unsigned int i = 0; i < (this->historicalMarkAtLayer).size(); ++i)
		{
			std::vector < int > currentLayer;
			if ((this->historicalMarkAtLayer).at(i).size() > 0)
			{
				for (unsigned int j = 0; j < (this->historicalMarkAtLayer).at(i).size(); ++j)
				{
					currentLayer.push_back((this->historicalMarkAtLayer).at(i).at(j));
				}
				(final->historicalMarkAtLayer).push_back(currentLayer);
			}
			else
			{
				(final->historicalMarkAtLayer).push_back({});
			}
				
		}

		for (unsigned int i = 0; i < this->inputsInNeuron_vector.size(); ++i)
		{
			final->inputsInNeuron_vector.push_back( this->inputsInNeuron_vector.at( i ) ) ;
		}

		for (unsigned int i = 0; i < this->outputsInNeuron_vector.size(); ++i)
		{
			final->outputsInNeuron_vector.push_back( this->outputsInNeuron_vector.at( i ) ) ;
		}

		final->prototypeNeuron = this->prototypeNeuron;
		final->prototypeSynapticWeight = this->prototypeSynapticWeight;
		final->inputPrototype = this->inputPrototype;
		final->outputPrototype = this->outputPrototype;
		

		final->age = 0;

		return final;
	}


	bool ANN::epoch()
	{
		std::cerr << "1" << std::endl;
		// Si es su primera epoca definitivamente se le deja vivir 
		if(age == 0)
		{
			age++;
			return true;
		}
		std::cerr << "2: "  << organismLifeExpectation << std::endl;
		
		double random = rand()/(double)RAND_MAX;
		// Se usa una desigualdad tal que la esperanza de vida sea en probabilidad la que el usuario puso.
		if( random > *organismLifeExpectation/(double)(1.0 + *organismLifeExpectation) )
		{
			return false;
		}
		std::cerr << "3" << std::endl;
		age++;
		std::cerr << "4" << std::endl;
		return true;
	}
}