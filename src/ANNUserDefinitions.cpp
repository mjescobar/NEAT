#include <fstream> // ofstream
#include <map> // map
#include <iostream>
#include <cstring>


#include "ANNUserDefinitions.hpp"



namespace NEAT
{

ANNUserDefinitions::~ANNUserDefinitions(  ){}

ANNUserDefinitions::ANNUserDefinitions(  ): ANNUserDefinitions( "./ANNUD" )
{

}
ANNUserDefinitions::ANNUserDefinitions( std::string path )
{
	std::map <std::string, double> loadData; // Se guardara la informacion en el mapa primero y luego se pasara a las variables.
	std::ifstream file;
	std::string line;
	char * piece1; // Aqui se guardan los strings
	char * piece2; // y aquí el valor 
	char * saveptr; // variable para indicar el lugar donde quedó la ultima lectura del strtok_r
	char delimiters[] = " \n\t"; // Los delimitadores usados.
	// Obtener toda la informacion del archivo
	file.open(  path , std::ios::in );
	if( file.is_open() )
	{
		while ( std::getline(file, line) )
	    {
	    	char * dup = strdup(line.c_str());
			piece1 = strtok_r( dup, delimiters, &saveptr); // Por si acaso se desea hacer uso de hilos.
			piece2 = strtok_r(NULL, delimiters, &saveptr);	    	
	    	loadData.emplace( std::make_pair( piece1, atof( piece2 ) ) );
	    	free(dup);
	    }
	    file.close();
	}

	// Se cargan todas las variables con los datos.


	probabilityNewNeuronInLayer = float( loadData["probabilityNewNeuronInLayer"] );
	probabilityOfNewSynapticWeight = float( loadData["probabilityOfNewSynapticWeight"] );
	probabilityOfNewUniqueSynapticWeight = float( loadData["probabilityOfNewUniqueSynapticWeight"] );
	inputsAmount = uint( loadData["inputsAmount"] );
	outputsAmount = uint( loadData["outputsAmount"] );
	useBackwardConnections = bool( loadData["useBackwardConnections"]);
	probabilityOfNewLayer = float (loadData["probabilityOfNewLayer"]);
	try
	{
		initialConectionProbability = float (loadData.at("initialConectionProbability"));
	}
	catch(std::out_of_range e)
	{
		initialConectionProbability = 1.0;
	}
	loadData.clear();
}

void ANNUserDefinitions::printInfo()
{
	std::cout << 
	 "initialConectionProbability: " << initialConectionProbability << std::endl << 
	 "probabilityNewNeuronInLayer: " << probabilityNewNeuronInLayer << std::endl << 
	 "probabilityOfNewSynapticWeight: " << probabilityOfNewSynapticWeight << std::endl << 
	 "probabilityOfNewUniqueSynapticWeight: " << probabilityOfNewUniqueSynapticWeight << std::endl << 
	"inputsAmount: " << inputsAmount << std::endl << 
	 "outputsAmount: " << outputsAmount << std::endl << 
	 "probabilityOfNewLayer: " << probabilityOfNewLayer << std::endl << 
	 "useBackwardConnections: " << useBackwardConnections << std::endl;
}

}