#include "BasicNeuron.hpp"
#include "GlobalInformation.hpp"

namespace NEATSpikes
{	

	BasicNeuron::BasicNeuron(Neuron * prototype, int historicalMark_inicial_input, int historicalMark_inicial_output)
	{
		loadParametersFromPrototype( prototype );
		init();
		identificator = ++*id;
		historicalMarkNeuronInicialOut = historicalMark_inicial_output;
		historicalMarkNeuronInicialIn = historicalMark_inicial_input;
		historicalMark = globalInformation->getHistoricalMark(historicalMark_inicial_input, historicalMark_inicial_output);
		layer = globalInformation->getLayer(historicalMark);
	}
	// Este constructor debe ser llamado una sola vez en todo el tiempo
	BasicNeuron::BasicNeuron(GlobalInformation * information, std::string pathUserDefinitionsAboutBasicNeuron )
	{
		globalInformation = information;
		id = new int(0);
		init();
		SetParametersFromUserDefinitionsPath( pathUserDefinitionsAboutBasicNeuron );
	}
	
	BasicNeuron::BasicNeuron()
	{
	}

	BasicNeuron::~BasicNeuron()
	{
		// No hay nada que borrar.
	}

	// Recordar que se tiene la premisa de obtener un código modular y entendible antes que 100% óptimo en rendimiento, las expresiones usadas para lograr el objetivo podrían ser adhoc pero no lograrían el mejor entendimiento ni tampoco la mejor flexibilidad.
	void BasicNeuron::mutate()
	{
		// El procedimiento que se realiza es un procedimiento genérico el cual realiza 4 pasos importantes.
		// se da por supuesto que los valores finales después de la mutación deben pertenecer al intervalo [min, max]
		// Pasos
		// ====
		// 1) pasar los valores actuales que están en el intervalo [min,max] al intervalo [0,1] a través de una transformación afin.
		// 2) obtener valor random en intervalo [0,1]
		// 3) se ponderan ambos valores según ponderaciones se indican en el archivo de definiciones de usuario.
		// 4) el resultado es devuelto al intervalo [min, max] a través de la inverza a la función afin usada. 
		// OBSERVACIÓN: Existe la posibilidad que el valor deba ser obtenido de forma absolutamente aleatoria, ese caso es más sencillo.

		double max, min, random_normalized, sigmoidConstant_normalized, bias_normalized;

		// ===================== SIGMOID CONSTANT ==========================
		
		if (*useSigmoidConstantMutation)
		{
			max = *maxSigmoidConstant;
			min = *minSigmoidConstant;

			if( rand()/(double)RAND_MAX < *probabilityOfSigmoidConstantRandomMutation )
			{

				sigmoidConstant = (max - min)*(rand()/(double)RAND_MAX) + min;
			}
			else
			{
				// paso 1
				sigmoidConstant_normalized = (sigmoidConstant - min)/(max - min);
				// paso 2
				random_normalized = rand()/(double)RAND_MAX;
				// paso 3
				sigmoidConstant_normalized = sigmoidConstant_normalized *(1-*maximumSigmoidConstantVariationByMutation) + random_normalized*(*maximumSigmoidConstantVariationByMutation);
			 	// paso 4
			 	sigmoidConstant = (max - min) * sigmoidConstant_normalized + min;	
			}
		}
		


		// ========================BIAS==================================
	 	// los valores límites del bias son obtenidos y se usan las definiciones del usuario para calcularlos.
		if( *useBias )
		{

		 	max = *maxBias;
			min = *minBias;

			if( rand()/(double) RAND_MAX < *probabilityOfBiasRandomMutation )
			{
				bias = (max - min)*(rand()/(double) RAND_MAX) + min;
			}
			else
			{
				// paso 1
				bias_normalized = (bias - min)/(max - min);
				// paso 2
				random_normalized = (rand()/(double)RAND_MAX);
				// paso 3
				bias_normalized = bias_normalized *(1 - *maximumBiasVariationByMutation) + random_normalized*(*maximumBiasVariationByMutation);
				// paso 4
			 	bias = (max - min) * bias_normalized + min;
		 	}
	 	}
	 	else
	 	{
	 		bias = 0.0;
	 	}
	}

	double BasicNeuron::eval( )
	{
		double result = 2.0/(1.0 + exp( -(inputVoltage + bias)*sigmoidConstant ) ) - 1.0; // Está en el intervalo [-1,1] la salida de la neurona.
		inputVoltage = 0.0;
		lastOutputVoltage = result;
		return result;
	}

	/*
		Al momento de evaluar se calculan ordenadamente todas las entradas a la neurona, y se deben ir sumando las entradas para luego de finalizado ese proceso tener el voltaje total de entrada a la neurona.
	*/
	void BasicNeuron::sumIncomingConnectionsOutputs( double incomingOneConnectionOutput )
	{
		inputVoltage += incomingOneConnectionOutput;
	}

	/*
		Se guardan las caracteriscicas especiales de la neurona para luego poder volver a cargala en el mismo estado actual.
	*/
	void BasicNeuron::saveState(std::string pathToSave)
	{
		// Se crea un archivo con los datos de la conexión sináptica con formato BSW+numero de identificación, por ejemplo, BSW1234
		std::string finalArchive = pathToSave + "BN" + std::to_string(identificator);

		// Se agrega el organismo en el index
		std::ofstream index;
		index.open(pathToSave+"index", std::ios::out | std::ios::app);
		index << identificator << std::endl;
		index.close();

		// Se crea el archivo y se guardan los datos.
		std::ofstream finalFile;
		finalFile.open(finalArchive);
		finalFile << "identificator " << identificator << "\nsigmoidConstant " << sigmoidConstant << "\nbias "   << bias << "\nlastOutputVoltage " << lastOutputVoltage<< "\nlayer "   << layer << "\nhistoricalMark "   << historicalMark << "\nhistoricalMarkNeuronInicialIn " << historicalMarkNeuronInicialIn << "\nhistoricalMarkNeuronInicialOut " << historicalMarkNeuronInicialOut;
		finalFile.close();
	}


	// Recordar que la clase que carge neuronas debe setea correctamente la variable id.
	void BasicNeuron::load(std::string PathWhereIsSaved)
	{
		// Las variables que serán usadas en la función.
		FILE * archive; // El archivo es cargado en esta variable
		//Las siguientes variables son usadas para leer las lineas del archivo
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		// Las siguientes variables son usadas para obtener los valores de las lineas y guardarlo en el mapa loadData
		char * token_1; // Aqui se guardan los strings
		char * token_2; // y aquí el valor 
		char * saveptr; // variable para indicar el lugar donde quedó la ultima lectura del strtok_r
		std::map <std::string, double> loadData; 
		char delimiters[] = " \n\t"; // Los delimitadores usados.
		
		// Se abre el archivo donde está guardada la conexión sináptica
		//=========================================================================================
		archive = fopen(PathWhereIsSaved.c_str(),"r");
		while ((read = getline(&line, &len, archive)) != -1) 
		{
			token_1 = strtok_r(line, delimiters, &saveptr);
			token_2 = strtok_r(NULL, delimiters, &saveptr);
			loadData[token_1]=atof(token_2);
		}
		fclose(archive); // Ya no se requiere más de este recurso.
		//=========================================================================================

		// Ahora se le da el valor a las variables de usuario y se termina este método. Usando mapas se hace más sencillo y más robusto.
		//=========================================================================================
		identificator = loadData["identificator"];
		bias = loadData["bias"];
		lastOutputVoltage = loadData["lastOutputVoltage"];
		sigmoidConstant = loadData["sigmoidConstant"];
		layer = loadData["layer"];
		historicalMark = loadData["historicalMark"];
		historicalMarkNeuronInicialIn = loadData["historicalMarkNeuronInicialIn"];
		historicalMarkNeuronInicialOut = loadData["historicalMarkNeuronInicialOut"];
		//=========================================================================================
	}

	void BasicNeuron::printState()
	{
		std::cout << "BasicNeuron\t" << "identificator: " << identificator << "\tbias: " << bias << "\tsigmoidConstant: "<< sigmoidConstant  << "\tlastOutputVoltage: " << lastOutputVoltage << "\thistoricalMark: " << historicalMark << "\tlayer: " << layer << "\thistoricalMark_inicial_input: " << historicalMarkNeuronInicialIn <<  "\thistoricalMark_inicial_output: " << historicalMarkNeuronInicialOut << std::endl; 
	}

	void BasicNeuron::saveUserDefinitions(std::string pathToSave)
	{
		std::ofstream userDefinitions;
		userDefinitions.open(pathToSave+"userDefinitions", std::ios::out);
		userDefinitions << "Use_Bias " << *useBias << std::endl;
		
		userDefinitions << "Min_Bias " << *minBias << std::endl;
		userDefinitions << "Max_Bias " << *maxBias << std::endl;
		userDefinitions << "Maximum_Bias_Variation_By_Mutation " << *maximumBiasVariationByMutation << std::endl;
		userDefinitions << "Probability_Of_Bias_Random_Mutation " << *probabilityOfBiasRandomMutation << std::endl;
		userDefinitions << "ConstantDistanceOfBias " << *ConstantDistanceOfBias << std::endl;
		userDefinitions << "Predefined_Bias " << *PredefinedBias << std::endl;

		userDefinitions << "Use_Sigmoid_Constant_Mutation " << *useSigmoidConstantMutation << std::endl;
		userDefinitions << "Max_Sigmoid_Constant " << *maxSigmoidConstant << std::endl;
		userDefinitions << "Min_Sigmoid_Constant " << *minSigmoidConstant << std::endl;
		userDefinitions << "Maximum_Sigmoid_Constant_Variation_By_Mutation " << *maximumSigmoidConstantVariationByMutation << std::endl;
		userDefinitions << "Probability_Of_Sigmoid_Constant_Random_Mutation " << *probabilityOfSigmoidConstantRandomMutation << std::endl;
		userDefinitions << "ConstantDistanceOfSigmoidConstant " << *ConstantDistanceOfSigmoidConstant << std::endl;
		userDefinitions << "Predefined_Sigmoid_Constat " << *PredefinedSigmoidConstat << std::endl;

		
		userDefinitions.close();
	}

	void  BasicNeuron::setHistoricalMark(int _historicalMark)
	{
		historicalMark = _historicalMark;
	}

	int BasicNeuron::getHistoricalMark()
	{
		return historicalMark;
	}

	void BasicNeuron::setLayer(int _layer)
	{
		layer = _layer;
	}

	int BasicNeuron::getLayer()
	{
		return layer;
	}


	Neuron * BasicNeuron::createNew( int historicalMark_inicial_input, int historicalMark_inicial_output)
	{
		BasicNeuron * BN = new BasicNeuron(this, historicalMark_inicial_input, historicalMark_inicial_output);
		return BN;
	}
	

	int BasicNeuron::getInitialNeuronInHistoricalMark()
	{
		return historicalMarkNeuronInicialIn;
	}

	int BasicNeuron::getInitialNeuronOutHistoricalMark()
	{
		return historicalMarkNeuronInicialOut;
	}

	std::vector <int>BasicNeuron::getOutcomingConnections()
	{
		return outcomingConections;
	}

	std::vector <int> BasicNeuron::getIncomingConnections()
	{
		return incomingConections;
	}

	void BasicNeuron::addNewIncomingConection(int innovationOfIncomingConection)
	{
		incomingConections.push_back(innovationOfIncomingConection);
	}

	void BasicNeuron::addNewOutcomingConection(int innovationOfOutcomingConection)
	{
		incomingConections.push_back(innovationOfOutcomingConection);
	}

	double BasicNeuron::getLastOutput()
	{
		return lastOutputVoltage;
	}

	Neuron * BasicNeuron::duplicate()
	{
		// Sencillamente se crea una nueva neurona y se le dan los valores de ésta.
		BasicNeuron * BN = new BasicNeuron;
		BN->loadParametersFromPrototype(this);
		BN->lastOutputVoltage = lastOutputVoltage;
		BN->identificator = ++*id;
		BN->layer = layer;
		BN->historicalMark = historicalMark;
		BN->historicalMarkNeuronInicialIn = historicalMarkNeuronInicialIn;
		BN->historicalMarkNeuronInicialOut = historicalMarkNeuronInicialOut;
		BN->incomingConections.clear();
		BN->globalInformation = globalInformation;

		for (int i = 0; i < (int)incomingConections.size(); ++i)
		{
			BN->incomingConections.push_back( incomingConections.at( i ) );
		}
		BN->outcomingConections.clear();
		for (int i = 0; i < (int)outcomingConections.size(); ++i)
		{
			BN->outcomingConections.push_back( outcomingConections.at( i ) );
		}
		BN->inputVoltage = inputVoltage;
		BN->sigmoidConstant = sigmoidConstant;
		BN->bias = bias;
		return BN;
	}

	double BasicNeuron::getDistance( Neuron * neuron )
	{
		// Primero que todo como la entrada no se puede asegurar que sea en realidad un puntero a BasicNeuron, lo primero que se hace es asegurar que en realidad si sea puntero a BasicNeuron o sino se emite un error.
		BasicNeuron * BN = NULL;
		BN = dynamic_cast< BasicNeuron * > ( neuron );
		if(BN != NULL)
		{
			return fabs( sigmoidConstant - BN->sigmoidConstant )  * (*ConstantDistanceOfSigmoidConstant) +  fabs( bias - BN->bias )  * (*ConstantDistanceOfBias);
		}
		// Si entra aqui es porque neuron no es un puntero de BasicNeuron.
		std::cerr << "ERROR::BasicNeuron::getDistance::Input must to be a pointer to BasicNeuron wrapped like pointer of Neuron" << std::endl;
		neuron->printState();
		exit( EXIT_FAILURE );
		
	}

	void BasicNeuron::changeValuesRandomly()
	{
		double max,min;
		if(*useSigmoidConstantMutation)
		{
			max = *maxSigmoidConstant;
			min = *minSigmoidConstant;
			sigmoidConstant = (max - min)*(rand()/(double)RAND_MAX) + min;
		}
		else
		{
			sigmoidConstant = *PredefinedSigmoidConstat;
		}

		if(*useBias)
		{
			max = *maxBias;
			min = *minBias;
			bias = (max - min)*(rand()/(double)RAND_MAX) + min;
		}
		else
		{
			bias = *PredefinedBias;
		}
	}

	void BasicNeuron::saveId(std::string pathToSave)
	{
		std::string finalArchive = pathToSave + "BN_id" ;

		// Se crea el archivo y se guardan los datos.
		std::ofstream finalFile;
		finalFile.open(finalArchive);
		finalFile << "id " << *id << std::endl;
		finalFile.close();
	}
	
	void BasicNeuron::loadId(std::string PathWhereIsSaved)
	{
		FILE * archive; // El archivo es cargado en esta variable
		//Las siguientes variables son usadas para leer las lineas del archivo
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		// Las siguientes variables son usadas para obtener los valores de las lineas y guardarlo en el mapa loadData
		char * token_1; // Aqui se guardan los strings
		char * token_2; // y aquí el valor 
		char * saveptr; // variable para indicar el lugar donde quedó la ultima lectura del strtok_r
		std::map <std::string, double> loadData; 
		char delimiters[] = " \n\t"; // Los delimitadores usados.
		
		// Se abre el archivo donde está guardada la conexión sináptica
		//=========================================================================================
		archive = fopen(PathWhereIsSaved.c_str(),"r");
		while ((read = getline(&line, &len, archive)) != -1) 
		{
			token_1 = strtok_r(line, delimiters, &saveptr);
			token_2 = strtok_r(NULL, delimiters, &saveptr);
			loadData[token_1]=atof(token_2);
		}
		fclose(archive); // Ya no se requiere más de este recurso.
		//=========================================================================================

		id = new int( loadData["id"] );
	}

	void BasicNeuron::SetParametersFromUserDefinitionsPath(std::string pathUserDefinitionsAboutBasicNeuron)
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
		archive = fopen(pathUserDefinitionsAboutBasicNeuron.c_str(),"r");
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
		
		useBias = new bool( UserDefinitions["Use_Bias"] );
		useSigmoidConstantMutation = new bool( UserDefinitions["Use_Sigmoid_Constant_Mutation"] );
		maxBias = new double(UserDefinitions["Max_Bias"]);
		minBias = new double(UserDefinitions["Min_Bias"]);
		maximumBiasVariationByMutation = new double(UserDefinitions["Maximum_Bias_Variation_By_Mutation"]);
		probabilityOfBiasRandomMutation = new double(UserDefinitions["Probability_Of_Bias_Random_Mutation"]);
		ConstantDistanceOfBias = new double(UserDefinitions["ConstantDistanceOfBias"]);
		maxSigmoidConstant = new double(UserDefinitions["Max_Sigmoid_Constant"]);
		minSigmoidConstant = new double(UserDefinitions["Min_Sigmoid_Constant"]);
		probabilityOfSigmoidConstantRandomMutation = new double(UserDefinitions["Probability_Of_Sigmoid_Constant_Random_Mutation"]);
		maximumSigmoidConstantVariationByMutation = new double(UserDefinitions["Maximum_Sigmoid_Constant_Variation_By_Mutation"]);
		ConstantDistanceOfSigmoidConstant = new double(UserDefinitions["ConstantDistanceOfSigmoidConstant"]);
		PredefinedBias = new double(UserDefinitions["Predefined_Bias"]);
		PredefinedSigmoidConstat = new double(UserDefinitions["Predefined_Sigmoid_Constat"]);
			

		//===========================================================================================
		//Se procede a verificar errores

		if(*useBias)
		{
			if(*maxBias <= *minBias)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error maxBias <= minBias" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(*maximumBiasVariationByMutation > 1 || *maximumBiasVariationByMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error maximumBiasVariationByMutation must be on interval [0,1]" << std::endl;
				exit(EXIT_FAILURE);
			}
			if(*probabilityOfBiasRandomMutation > 1 || *probabilityOfBiasRandomMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfBiasRandomMutation must be on interval [0,1]" << std::endl;
				exit (EXIT_FAILURE);
			}
		}

		if(*useSigmoidConstantMutation)
		{
			if(*maximumSigmoidConstantVariationByMutation > 1 || *maximumSigmoidConstantVariationByMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error maximumSigmoidConstantVariationByMutation must be on interval [0,1]" << std::endl;
				exit (EXIT_FAILURE);
			}
			
			if(*probabilityOfSigmoidConstantRandomMutation > 1 || *probabilityOfSigmoidConstantRandomMutation < 0)
			{
				std::cerr << "Error::BasicNeuron::SetParametersFromUserDefinitionsPath::Error probabilityOfSigmoidConstantRandomMutation must be on interval [0,1]" << std::endl;
				exit (EXIT_FAILURE);
			}
		}
		//=========================================================================================
	}



	void BasicNeuron::init()
	{
		lastOutputVoltage=0.0;
		identificator = ++(*id);
	}

	void BasicNeuron::loadParametersFromPrototype(Neuron * prototype)
	{
		BasicNeuron * BN = NULL;
		BN = dynamic_cast < BasicNeuron * > ( prototype );
		if(BN != NULL)
		{
			id = BN->id;
			useBias = BN->useBias;
			maxBias = BN->maxBias;
			minBias = BN->minBias;
			probabilityOfBiasRandomMutation = BN->probabilityOfBiasRandomMutation;
			maximumBiasVariationByMutation = BN->maximumBiasVariationByMutation;
			ConstantDistanceOfBias = BN->ConstantDistanceOfBias;
			PredefinedBias = BN->PredefinedBias;
			useSigmoidConstantMutation = BN->useSigmoidConstantMutation;
			maxSigmoidConstant = BN->maxSigmoidConstant;
			minSigmoidConstant = BN->minSigmoidConstant;
			probabilityOfSigmoidConstantRandomMutation  = BN->probabilityOfSigmoidConstantRandomMutation;
			maximumSigmoidConstantVariationByMutation = BN->maximumSigmoidConstantVariationByMutation;
			ConstantDistanceOfSigmoidConstant = BN->ConstantDistanceOfSigmoidConstant;
			PredefinedSigmoidConstat = BN->PredefinedSigmoidConstat;
			globalInformation = BN->globalInformation;
		}
		else
		{
			std::cerr << "ERROR::BasicNeuron::loadParametersFromPrototype::Input must to be a pointer to BasicNeuron wrapped like pointer of Neuron" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	Neuron * BasicNeuron::createNewOutput()
	{
		BasicNeuron * BN =  new BasicNeuron;
		BN->loadParametersFromPrototype( this );
		BN->init();
		BN->historicalMarkNeuronInicialIn = OUTPUT_INICIAL_IN_OUT;
		BN->historicalMarkNeuronInicialOut = OUTPUT_INICIAL_IN_OUT;
		BN->historicalMark = (BN->globalInformation)->getNeuronOutputHistoricalMark();
		BN->layer = (BN->globalInformation)->getNeuronOutputLayer();
		BN->identificator = ++*id;
		return BN;
	}
	
	Neuron * BasicNeuron::createNewInput()
	{
		BasicNeuron * BN =  new BasicNeuron;
		BN->loadParametersFromPrototype( this );
		BN->init();
		BN->historicalMarkNeuronInicialIn = INPUT_INICIAL_IN_OUT;
		BN->historicalMarkNeuronInicialOut = INPUT_INICIAL_IN_OUT;
		BN->historicalMark = (BN->globalInformation)->getNeuronInputHistoricalMark();
		BN->layer = (BN->globalInformation)->getNeuronInputLayer();
		BN->identificator = ++*id;
		return BN;
	}


	void BasicNeuron::copyValues(Neuron * neuron)
	{
		BasicNeuron * BN = NULL;
		BN = dynamic_cast < BasicNeuron * > (neuron) ;
		if( BN != NULL )
		{
			this->bias = BN->bias;
			this->sigmoidConstant = BN->sigmoidConstant;
		}
		else
		{
			std::cerr << "ERROR::BasicNeuron::copyValues::The neuron is not BasicNeuron" << std::endl;
			exit (EXIT_FAILURE);
		}
		
	}

}