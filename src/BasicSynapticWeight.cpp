#include "BasicSynapticWeight.hpp"

namespace NEATSpikes
{

	//Este es el metodo con el que se debe comenzar la primera conexion sinaptica de todas, luego todas las que vienen son duplicas de esta.
	BasicSynapticWeight::BasicSynapticWeight(std::string pathUserDefinitionsAboutBasicSynapticWeight, GlobalInformation * Info)
	{
		id = new int(0);
		SetParametersFromUserDefinitionsPath(pathUserDefinitionsAboutBasicSynapticWeight);
		information = Info;
		init();
		changeValuesRandomly();
	}
	
	BasicSynapticWeight::BasicSynapticWeight(SynapticWeight * prototype, int histoticalMark_Neuron_in , int histoticalMark_Neuron_out)
	{
		loadParametersFromPrototype(prototype);
		init();
		historicalMarkOfNeuronIn=histoticalMark_Neuron_in;
		historicalMarkOfNeuronOut=histoticalMark_Neuron_out;
		innovation = information->getInnovation( histoticalMark_Neuron_in , histoticalMark_Neuron_out );		
		changeValuesRandomly();
	}
	BasicSynapticWeight::BasicSynapticWeight()
	{	
	}
	BasicSynapticWeight::~BasicSynapticWeight()
	{
	}
	void BasicSynapticWeight::init()
	{
		identificator = ++(*id);
		enable = true;
		input=0.0;
		output=0.0;
	}
	void BasicSynapticWeight::mutate()
	{
		// El procedimiento que se realiza es un procedimiento genérico el cual realiza 4 pasos importantes.
		//  se da por supuesto que los valores finales después de la mutación deben pertenecer al intervalo [min, max]
		// 1) pasar los valores al intervalo [0,1] a través de una transformación afin.
		// 2) obtener valor random en intervalo [0,1]
		// 3) se ponderan ambos valores según lo que el usuario manifesto en las definiciones de usuario
		// 4) el resultado es devuelto al intervalo [min, max] a través de la inverza a la función afin usada. 
		// En caso de que se tenga que el valor nuevo es completamente random se realiza la misma idea pero sin la ponderación.
		if(enable)
		{
			double max = *maxWeightValue;
			double min = *minWeightValue;
			if( rand()/(double)RAND_MAX < *probabilityOfWeightRandomMutation)
			{
				synapticWeightValue = (max - min)*(rand()/(double)RAND_MAX) + min;
			}
			else
			{
				double synapticWeightValueNormalized = (synapticWeightValue - min)/(max - min);
				double random_normalized = rand()/(double)RAND_MAX;
				synapticWeightValueNormalized = synapticWeightValueNormalized *(1-*maximumWeightVariationByMutation) + random_normalized*(*maximumWeightVariationByMutation);
			 	synapticWeightValue = (max - min) * synapticWeightValueNormalized + min;
		 	}	
		}
		else
		{
			if(rand()/(double)RAND_MAX  < *probabilityOfEnableADisabledConnection)
			{
				enable = true;
			}
		}	
	}
	void BasicSynapticWeight::printState()
	{
		std::cout << "BasicSynapticWeight\t" << "identificator: " << identificator << "\tsynapticWeightValue: " << synapticWeightValue << "\toutput " << output << "\thistoticalMark_Neuron_in: " << historicalMarkOfNeuronIn <<  "\thistoticalMark_Neuron_out: " << historicalMarkOfNeuronOut<< std::endl; 
	}
	SynapticWeight * BasicSynapticWeight::createNew( SynapticWeight * prototype, int histoticalMark_Neuron_in , int histoticalMark_Neuron_out )
	{
		BasicSynapticWeight * BSW = new BasicSynapticWeight( prototype, histoticalMark_Neuron_in, histoticalMark_Neuron_out );
		return BSW;
	}
	void BasicSynapticWeight::saveUserDefinitions(std::string pathToSave)
	{
		std::ofstream userDefinitions;
		userDefinitions.open(pathToSave + "userDefinitions", std::ios::out);
		userDefinitions << "Max_Weight_Value " << *maxWeightValue << std::endl;
		userDefinitions << "Min_Weight_Value " << *minWeightValue << std::endl;
		userDefinitions << "Maximum_Weight_Variation_By_Mutation " << *maximumWeightVariationByMutation << std::endl;
		userDefinitions << "Probability_Of_Weight_Random_Mutation " << *probabilityOfWeightRandomMutation << std::endl;
		userDefinitions << "probabilityOfEnableADisabledConnection " << *probabilityOfEnableADisabledConnection << std::endl;
		userDefinitions << "ConstantDistanceOfSynapticWeightValue " << *ConstantDistanceOfSynapticWeightValue << std::endl;
		userDefinitions.close();
	}
	// Se sabe que el formato es:
	//  Max_Weight_Value	0.123456
	// Maximum_Weight_Variation_By_Mutation 	0.123456
	// OBSERVACION IMPORTANTE:  Se usa strtok_r para así poder hacer multihebras en algún nivel superior sin tener problemas, además es más cómodo su uso.
	void BasicSynapticWeight::SetParametersFromUserDefinitionsPath(std::string pathUserDefinitionsAboutBasicSynapticWeight)
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
		archive = fopen(pathUserDefinitionsAboutBasicSynapticWeight.c_str(),"r");
		if (archive == NULL)
		{
			std::cerr << "ERROR::BasicSynapticWeight::SetParametersFromUserDefinitionsPath::Could not read the file" << std::endl;
			exit(EXIT_FAILURE);
		}
		//=========================================================================================
		//Se lee linea a linea y se guarda en el mapa UserDefinitions
		//=========================================================================================
		while ((read = getline(&line, &len, archive)) != -1) 
		{
			token_1 = strtok_r(line, delimiters, &saveptr);
			token_2 = strtok_r(NULL, delimiters, &saveptr);
			UserDefinitions [ token_1 ] = atof(token_2);
		}
		fclose(archive); // Ya no se hará uso de este archivo así que se cierra el recurso.
		//=========================================================================================
		// Ahora se le da el valor a las variables de usuario y se termina este método. Usando mapas se hace más sencillo y más robusto.
		//=========================================================================================
		maxWeightValue = new double ( UserDefinitions["Max_Weight_Value"] );
		minWeightValue = new double ( UserDefinitions["Min_Weight_Value"] );
		maximumWeightVariationByMutation = new double ( UserDefinitions["Maximum_Weight_Variation_By_Mutation"] );
		probabilityOfWeightRandomMutation = new double ( UserDefinitions["Probability_Of_Weight_Random_Mutation"] );
		probabilityOfEnableADisabledConnection = new double( UserDefinitions["probabilityOfEnableADisabledConnection"] );
		ConstantDistanceOfSynapticWeightValue = new double ( UserDefinitions["ConstantDistanceOfSynapticWeightValue"] );

		//=========================================================================================
		// Se revizan todos los posibles problemas
		if(*maxWeightValue <= *minWeightValue)
		{
			std::cerr << "Error::BasicSynapticWeight::SetParametersFromUserDefinitionsPath::Error maxWeightValue <= minWeightValue" << std::endl;
			exit( EXIT_FAILURE );
		}

		if(*probabilityOfWeightRandomMutation > 1 || *probabilityOfWeightRandomMutation < 0)
		{
			std::cerr << "Error::BasicSynapticWeight::SetParametersFromUserDefinitionsPath::Error probabilityOfWeightRandomMutation must be on interval [0,1]" << std::endl;
			exit( EXIT_FAILURE );
		}
		if(*probabilityOfEnableADisabledConnection > 1 || *probabilityOfEnableADisabledConnection < 0)
		{
			std::cerr << "Error::BasicSynapticWeight::SetParametersFromUserDefinitionsPath::Error probabilityOfEnableADisabledConnection must be on interval [0,1]" << std::endl;
			exit( EXIT_FAILURE );
		}
		if(*maximumWeightVariationByMutation > 1 || *maximumWeightVariationByMutation < 0)
		{
			std::cerr << "Error::BasicSynapticWeight::SetParametersFromUserDefinitionsPath::Error maximumWeightVariationByMutation must be on interval [0,1]" << std::endl;
			exit( EXIT_FAILURE );
		}
		//=========================================================================================
	}
	void BasicSynapticWeight::saveState(std::string pathToSave)
	{
		// Se crea un archivo con los datos de la conexión sináptica con formato BSW+numero de identificación, por ejemplo, BSW1234
		std::string finalArchive = pathToSave + "BSW"+ std::to_string(identificator);

		// Se agrega el organismo en el index
		std::ofstream index;
		index.open(pathToSave+"index", std::ios::out | std::ios::app);
		index << identificator << std::endl;
		index.close();

		// Se crea el archivo y se guardan los datos.
		std::ofstream finalFile;
		finalFile.open(finalArchive);
		finalFile << "identificator " << identificator << "\nsynapticWeightValue " << synapticWeightValue << "\noutput " << output << "\nenable "<< enable  << "\nhistoricalMarkOfNeuronIn "<< historicalMarkOfNeuronIn  << "\nhistoricalMarkOfNeuronOut "<< historicalMarkOfNeuronOut;
		finalFile.close();
	}
	void BasicSynapticWeight::load(std::string PathWhereIsSaved)
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
		//=========================================================================================

		// Ahora se le da el valor a las variables de usuario y se termina este método. Usando mapas se hace más sencillo y más robusto.
		//=========================================================================================
		identificator = loadData["identificator"];
		synapticWeightValue = loadData["synapticWeightValue"];
		output = loadData["output"];
		enable = loadData["enable"];
		historicalMarkOfNeuronIn = loadData["historicalMarkOfNeuronIn"];
		historicalMarkOfNeuronOut = loadData["historicalMarkOfNeuronOut"];
		//=========================================================================================
		fclose(archive);
	}
	void BasicSynapticWeight::setInnovation(int _innovation)
	{
		innovation = _innovation;
	}
	int BasicSynapticWeight::getInnovation()
	{
		return innovation;
	}
	void BasicSynapticWeight::disable()
	{
		enable=false;
	}

	int BasicSynapticWeight::getHistoricalMarkOfNeuronIn()
	{
		return historicalMarkOfNeuronIn;
	}
	int BasicSynapticWeight::getHistoricalMarkOfNeuronOut()
	{
		return historicalMarkOfNeuronOut;
	}
	double BasicSynapticWeight::getOutput()
	{
		return output;
	}
	void BasicSynapticWeight::setInput(double _input)
	{
		input = _input;
	}
	void BasicSynapticWeight::spread()
	{
		output = input * synapticWeightValue;
	}
	SynapticWeight * BasicSynapticWeight::duplicate()
	{
		BasicSynapticWeight * BSW = new BasicSynapticWeight;
		BSW->loadParametersFromPrototype( this );
		BSW->output = output;
		BSW->input = input;
		BSW->synapticWeightValue = synapticWeightValue;
		BSW->innovation = innovation; 
		BSW->enable = enable;
		BSW->historicalMarkOfNeuronIn = historicalMarkOfNeuronIn;
		BSW->historicalMarkOfNeuronOut = historicalMarkOfNeuronOut;
		BSW->identificator = ++(*id);
		BSW->information = information;
		return BSW;
	}

	double BasicSynapticWeight::getDistance(SynapticWeight * synapticWeight)
	{
		BasicSynapticWeight * BSW = NULL;
		BSW = dynamic_cast <BasicSynapticWeight *> ( synapticWeight );
		if(BSW != NULL)
		{
			return fabs( synapticWeightValue - BSW->synapticWeightValue ) * (*ConstantDistanceOfSynapticWeightValue);
		}
		else
		{
			std::cerr << "ERROR::BasicSynapticWeight::getDistance::Input must to be a pointer to BasicSynapticWeight wrapped like pointer of SynapticWeight" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void BasicSynapticWeight::changeValuesRandomly()
	{
		double max = *maxWeightValue;
		double min = *minWeightValue;
		synapticWeightValue = (max - min)*(rand()/(double)RAND_MAX) + min;
		std::cerr << "max " << max << "\tmin " << min << "\tsynapticWeightValue: " << synapticWeightValue << std::endl;
	}

	void BasicSynapticWeight::loadParametersFromPrototype(SynapticWeight * prototype)
	{
		BasicSynapticWeight * BSW = NULL;
		BSW = dynamic_cast < BasicSynapticWeight * > ( prototype );
		if(BSW != NULL)
		{
			id = BSW->id;
			information = BSW->information;
			maxWeightValue = BSW->maxWeightValue;
			minWeightValue = BSW->minWeightValue;
			maximumWeightVariationByMutation = BSW->maximumWeightVariationByMutation;
			probabilityOfWeightRandomMutation = BSW->probabilityOfWeightRandomMutation;
			probabilityOfEnableADisabledConnection = BSW->probabilityOfEnableADisabledConnection;
			ConstantDistanceOfSynapticWeightValue = BSW->ConstantDistanceOfSynapticWeightValue;
		}
		else
		{
			std::cerr << "ERROR::BasicSynapticWeight::loadParametersFromPrototype::Input must to be a pointer to BasicSynapticWeight wrapped like pointer of SynapticWeight" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void BasicSynapticWeight::saveId(std::string pathToSave)
	{
		std::string finalArchive = pathToSave + "BSW_id" ;
		// Se crea el archivo y se guardan los datos.
		std::ofstream finalFile;
		finalFile.open(finalArchive);
		finalFile << "id " << *id << std::endl;
		finalFile.close();
	}

	void BasicSynapticWeight::loadId(std::string PathWhereIsSaved)
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

	void BasicSynapticWeight::copyValues(SynapticWeight * sw)
	{
		BasicSynapticWeight * BSW = NULL;
		BSW = dynamic_cast < BasicSynapticWeight * > ( sw );
		if(BSW != NULL)
		{
			this->synapticWeightValue = BSW->synapticWeightValue;
		}
		else
		{
			std::cerr << "ERROR::BasicSynapticWeight::copyValues::Input must to be a pointer to BasicSynapticWeight wrapped like pointer of SynapticWeight" << std::endl;
			exit(EXIT_FAILURE);
		}
		
	}
}