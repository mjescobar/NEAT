#include "TimeDelayedSynapticWeight.hpp"

namespace NEATSpikes
{
	//Dado que son variables estáticas DEBEN ser inicializadas siempre en el cpp.
	int TimeDelayedSynapticWeight::id=0;
	int TimeDelayedSynapticWeight::MaxAmountOfTimeDelayed=0;
	double TimeDelayedSynapticWeight::maxWeightValue=-1.0;
	double TimeDelayedSynapticWeight::maximumWeightVariationByMutation=-1.0;
	double TimeDelayedSynapticWeight::probabilityOfWeightRandomMutation=-1.0;
	double TimeDelayedSynapticWeight::probabilityOfEnableADisabledConnection=-1.0;
	double TimeDelayedSynapticWeight::ConstantDistanceOfSynapticWeightValue=0.0;
	GlobalInformation * TimeDelayedSynapticWeight::information = NULL;

	TimeDelayedSynapticWeight::TimeDelayedSynapticWeight(std::string pathUserDefinitionsAboutTimeDelayedSynapticWeight, GlobalInformation * Info)
	{
		SetParametersFromUserDefinitionsPath(pathUserDefinitionsAboutTimeDelayedSynapticWeight);
		information = Info;
		init();
	}

	TimeDelayedSynapticWeight::TimeDelayedSynapticWeight(int _innovation,int histoticalMark_Neuron_in , int histoticalMark_Neuron_out)
	{
		historicalMarkOfNeuronIn=histoticalMark_Neuron_in;
		historicalMarkOfNeuronOut=histoticalMark_Neuron_out;
		innovation=_innovation;
		init();
	}

	TimeDelayedSynapticWeight::TimeDelayedSynapticWeight(int histoticalMark_Neuron_in , int histoticalMark_Neuron_out)
	{
		historicalMarkOfNeuronIn=histoticalMark_Neuron_in;
		historicalMarkOfNeuronOut=histoticalMark_Neuron_out;
		innovation= information->getInnovation( histoticalMark_Neuron_in , histoticalMark_Neuron_out );
		init();
	}

	TimeDelayedSynapticWeight::TimeDelayedSynapticWeight()
	{	init();
	}

	TimeDelayedSynapticWeight::TimeDelayedSynapticWeight(const TimeDelayedSynapticWeight & BSW)
	{
		synapticWeightValue = BSW.synapticWeightValue;
		output = BSW.output;
		identificator = BSW.identificator; 
	}

	TimeDelayedSynapticWeight::~TimeDelayedSynapticWeight()
	{
		// No hay información que deba ser borrada.
	}

	void TimeDelayedSynapticWeight::init()
	{
		synapticWeightValue = ( 2 * random()/(double)RAND_MAX - 1 ) * maxWeightValue;
		identificator = id++;
		enable = true;
		input=0.0;
		output=0.0;
		bufferSize = rand() % MaxAmountOfTimeDelayed;
	}

	void TimeDelayedSynapticWeight::mutate()
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
			double max = maxWeightValue;
			double min = -maxWeightValue;
			if( rand()/(double)RAND_MAX < probabilityOfWeightRandomMutation)
			{
				synapticWeightValue = (max - min)*(rand()/(double)RAND_MAX) + min;
			}
			else
			{
				double synapticWeightValueNormalized = (synapticWeightValue - min)/(max - min);
				double random_normalized = rand()/(double)RAND_MAX;
				synapticWeightValueNormalized = synapticWeightValueNormalized *(1-maximumWeightVariationByMutation) + random_normalized*maximumWeightVariationByMutation;
			 	synapticWeightValue = (max - min) * synapticWeightValueNormalized + min;
		 	}	
		}
		else
		{
			if(rand()/(double)RAND_MAX  < probabilityOfEnableADisabledConnection)
			{
				enable=true;
			}
		}	
	}

	void TimeDelayedSynapticWeight::printState()
	{
		std::cout << "TimeDelayedSynapticWeight\t" << "identificator: " << identificator << "\tsynapticWeightValue: " << synapticWeightValue << "\toutput " << output << "\thistoticalMark_Neuron_in: " << historicalMarkOfNeuronIn <<  "\thistoticalMark_Neuron_out: " << historicalMarkOfNeuronOut<< std::endl; 
	}

	SynapticWeight * TimeDelayedSynapticWeight::createNew( int histoticalMark_Neuron_in , int histoticalMark_Neuron_out )
	{
		TimeDelayedSynapticWeight * BSW = new TimeDelayedSynapticWeight( histoticalMark_Neuron_in, histoticalMark_Neuron_out );
		return BSW;
	}

	void TimeDelayedSynapticWeight::saveUserDefinitions(std::string pathToSave)
	{
		std::ofstream userDefinitions;
		userDefinitions.open(pathToSave+"userDefinitions", std::ios::out);
		userDefinitions << "Max_Weight_Value " << maxWeightValue << std::endl;
		userDefinitions << "Maximum_Weight_Variation_By_Mutation " << maximumWeightVariationByMutation << std::endl;
		userDefinitions << "Probability_Of_Weight_Random_Mutation " << probabilityOfWeightRandomMutation << std::endl;
		userDefinitions << "probabilityOfEnableADisabledConnection " << probabilityOfEnableADisabledConnection << std::endl;
		userDefinitions << "ConstantDistanceOfSynapticWeightValue " << ConstantDistanceOfSynapticWeightValue << std::endl;
		userDefinitions << "MaxAmountOfTimeDelayed " << MaxAmountOfTimeDelayed << std::endl;
		userDefinitions.close();
	}




	// Se sabe que el formato es:
	//  Max_Weight_Value	0.123456
	// Maximum_Weight_Variation_By_Mutation 	0.123456
	// OBSERVACION IMPORTANTE:  Se usa strtok_r para así poder hacer multihebras en algún nivel superior sin tener problemas, además es más cómodo su uso.
	void TimeDelayedSynapticWeight::SetParametersFromUserDefinitionsPath(std::string pathUserDefinitionsAboutTimeDelayedSynapticWeight)
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
		archive = fopen(pathUserDefinitionsAboutTimeDelayedSynapticWeight.c_str(),"r");
		if (archive == NULL)
		{
			std::cerr << "ERROR::TimeDelayedSynapticWeight::SetParametersFromUserDefinitionsPath::Could not read the file" << std::endl;
			exit(EXIT_FAILURE);
		}
		//=========================================================================================
		//Se lee linea a linea y se guarda en el mapa UserDefinitions
		//=========================================================================================
		while ((read = getline(&line, &len, archive)) != -1) 
		{
			token_1 = strtok_r(line, delimiters, &saveptr);
			token_2 = strtok_r(NULL, delimiters, &saveptr);
			UserDefinitions [ token_1 ]=atof(token_2);
		}
		fclose(archive); // Ya no se hará uso de este archivo así que se cierra el recurso.
		//=========================================================================================
		// Ahora se le da el valor a las variables de usuario y se termina este método. Usando mapas se hace más sencillo y más robusto.
		//=========================================================================================
		maxWeightValue = UserDefinitions["Max_Weight_Value"];
		MaxAmountOfTimeDelayed = UserDefinitions["MaxAmountOfTimeDelayed"];
		maximumWeightVariationByMutation = UserDefinitions["Maximum_Weight_Variation_By_Mutation"];
		probabilityOfWeightRandomMutation = UserDefinitions["Probability_Of_Weight_Random_Mutation"];
		probabilityOfEnableADisabledConnection = UserDefinitions["probabilityOfEnableADisabledConnection"];
		ConstantDistanceOfSynapticWeightValue = UserDefinitions["ConstantDistanceOfSynapticWeightValue"];
		if(maximumWeightVariationByMutation > 1 || maximumWeightVariationByMutation < 0)
		{
			std::cerr << "Error::TimeDelayedSynapticWeight::SetParametersFromUserDefinitionsPath::Error maximumWeightVariationByMutation must be on interval [0,1]" << std::endl;
			exit( EXIT_FAILURE );
		}
		if(probabilityOfEnableADisabledConnection > 1 || probabilityOfEnableADisabledConnection < 0)
		{
			std::cerr << "Error::TimeDelayedSynapticWeight::SetParametersFromUserDefinitionsPath::Error probabilityOfEnableADisabledConnection must be on interval [0,1]" << std::endl;
			exit( EXIT_FAILURE );
		}
		if(probabilityOfWeightRandomMutation > 1 || probabilityOfWeightRandomMutation < 0)
		{
			std::cerr << "Error::TimeDelayedSynapticWeight::SetParametersFromUserDefinitionsPath::Error probabilityOfWeightRandomMutation must be on interval [0,1]" << std::endl;
			exit( EXIT_FAILURE );
		}
		//=========================================================================================
	}


	void TimeDelayedSynapticWeight::saveState(std::string pathToSave)
	{
		// Se crea un archivo con los datos de la conexión sináptica con formato BSW+numero de identificación, por ejemplo, BSW1234
		std::string finalArchive = pathToSave + "TDSW"+ std::to_string(identificator);

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

	void TimeDelayedSynapticWeight::load(std::string PathWhereIsSaved)
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
			token_1 = strtok_r( line, delimiters, &saveptr );
			token_2 = strtok_r( NULL, delimiters, &saveptr );
			loadData[token_1]=atof( token_2 );
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

	void TimeDelayedSynapticWeight::setInnovation(int _innovation)
	{
		innovation = _innovation;
	}

	int TimeDelayedSynapticWeight::getInnovation()
	{
		return innovation;
	}


	void TimeDelayedSynapticWeight::disable()
	{
		enable=false;
	}

	int TimeDelayedSynapticWeight::getHistoricalMarkOfNeuronIn()
	{
		return historicalMarkOfNeuronIn;
	}
	int TimeDelayedSynapticWeight::getHistoricalMarkOfNeuronOut()
	{
		return historicalMarkOfNeuronOut;
	}
	double TimeDelayedSynapticWeight::getOutput()
	{
		return output;
	}
	void TimeDelayedSynapticWeight::setInput(double _input)
	{
		input = _input;
	}
	void TimeDelayedSynapticWeight::spread()
	{
		
		output = input * synapticWeightValue;
	}
	SynapticWeight * TimeDelayedSynapticWeight::duplicate()
	{
		TimeDelayedSynapticWeight * BSW = new TimeDelayedSynapticWeight;
		BSW->output=output;
		BSW->input=input;
		BSW->synapticWeightValue=synapticWeightValue;
		BSW->identificator=identificator;
		BSW->innovation=innovation; 
		BSW->enable=enable;
		BSW->historicalMarkOfNeuronIn=historicalMarkOfNeuronIn;
		BSW->historicalMarkOfNeuronOut=historicalMarkOfNeuronOut;
		return BSW;
	}

	double TimeDelayedSynapticWeight::getDistance(SynapticWeight * synapticWeight)
	{
		TimeDelayedSynapticWeight * BSW = NULL;
		BSW = dynamic_cast < TimeDelayedSynapticWeight * > ( synapticWeight );
		if(BSW != NULL)
		{
			return fabs( synapticWeightValue - BSW->synapticWeightValue ) * ConstantDistanceOfSynapticWeightValue;
		}
		else
		{
			std::cerr << "ERROR::TimeDelayedSynapticWeight::getDistance::Input must to be a pointer to TimeDelayedSynapticWeight wrapped like pointer of SynapticWeight" << std::endl;
			exit( EXIT_FAILURE );
		}
	}

	void TimeDelayedSynapticWeight::changeValuesRandomly()
	{
		double max = maxWeightValue;
		double min = -maxWeightValue;
		synapticWeightValue = (max - min)*(rand()/(double)RAND_MAX) + min;
	}
}