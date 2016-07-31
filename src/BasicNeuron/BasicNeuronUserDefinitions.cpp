#include "BasicNeuron/BasicNeuronUserDefinitions.hpp"

#include <fstream> // ofstream
#include <map> // map
#include <iostream>
#include <algorithm>    // For std::remove()
#include <cstring>

namespace NEAT
{

BasicNeuronUserDefinitions::BasicNeuronUserDefinitions( std::string path )
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
	useBias = bool( loadData["useBias"] );
	useSigmoidConstant = bool( loadData["useSigmoidConstant"] );
	maxBias = float( loadData["maxBias"] );
	minBias = float( loadData["minBias"] );
	maximumBiasVariation = float( loadData["maximumBiasVariation"] );
	probabilityOfBiasRandomMutation = float( loadData["probabilityOfBiasRandomMutation"] );
	constantDistanceOfBias = float( loadData["constantDistanceOfBias"] );
	maxSigmoidConstant = float( loadData["maxSigmoidConstant"] );
	minSigmoidConstant = float( loadData["minSigmoidConstant"] );
	probabilityOfSigmoidConstantRandomMutation = float( loadData["probabilityOfSigmoidConstantRandomMutation"] );
	maximumSigmoidConstantVariation = float( loadData["maximumSigmoidConstantVariation"] );
	constantDistanceOfSigmoidConstant = float( loadData["constantDistanceOfSigmoidConstant"] );
	predefinedBias = float( loadData["predefinedBias"] );
	predefinedSigmoidConstat = float( loadData["predefinedSigmoidConstat"] );

	loadData.clear();
}

}