#include "TauSynapticWeightUserDefinitions.hpp"

#include <fstream> // ofstream
#include <map> // map
#include <iostream>
#include <cstring>

namespace NEAT
{

TauSynapticWeightUserDefinitions::TauSynapticWeightUserDefinitions() : TauSynapticWeightUserDefinitions("./TSWUD")
{

}

TauSynapticWeightUserDefinitions::TauSynapticWeightUserDefinitions( std::string path )
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
	maxWeightValue = float( loadData["maxWeightValue"] );
	minWeightValue = float( loadData["minWeightValue"] );
	maximumWeightPercentVariation = float ( loadData["maximumWeightPercentVariation"] );
	probabilityOfWeightRandomMutation = float( loadData["probabilityOfWeightRandomMutation"] );
	probabilityOfEnableADisabledConnection = float( loadData["probabilityOfEnableADisabledConnection"] );
	constantDistanceOfSynapticWeightValue = float( loadData["constantDistanceOfSynapticWeightValue"] );
	mutationProbability = float( loadData["mutationProbability"] );
	maxBufferSize = uint(loadData["maxBufferSize"]);
	constantDistanceOfBufferSize = float(loadData["constantDistanceOfBufferSize"]);
	bufferSizeMutationRate = float(loadData["bufferSizeMutationRate"]);
}


void TauSynapticWeightUserDefinitions::printInfo()
{
	std::cout << 
	"maxWeightValue: "<< maxWeightValue << "\n" <<
	"minWeightValue: "<< minWeightValue << "\n" <<
	"maximumWeightPercentVariation: "<< maximumWeightPercentVariation << "\n" <<
	"probabilityOfWeightRandomMutation: "<< probabilityOfWeightRandomMutation << "\n" <<
	"probabilityOfEnableADisabledConnection: "<< probabilityOfEnableADisabledConnection << "\n" <<
	"mutationProbability: "<< mutationProbability << "\n" <<
	"maxBufferSize: "<< maxBufferSize << "\n" <<
	"constantDistanceOfBufferSize: "<< constantDistanceOfBufferSize << "\n" <<
	"bufferSizeMutationRate: "<< bufferSizeMutationRate << "\n" <<
	"constantDistanceOfSynapticWeightValue: "<< constantDistanceOfSynapticWeightValue << "\n";
}

}
