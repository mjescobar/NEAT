#include "CPPNNeuronUserDefinitions.hpp"

#include <fstream> // ofstream
#include <map> // map
#include <iostream>
#include <cstring>

namespace NEAT
{

CPPNNeuronUserDefinitions::~CPPNNeuronUserDefinitions(  )
{

}

CPPNNeuronUserDefinitions::CPPNNeuronUserDefinitions(  ): CPPNNeuronUserDefinitions( "./CPPNNUD" )
{

}

CPPNNeuronUserDefinitions::CPPNNeuronUserDefinitions( std::string path )
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
	maxBias = float( loadData["maxBias"] );
	minBias = float( loadData["minBias"] );
	maximumBiasPercentVariation = float( loadData["maximumBiasPercentVariation"] );
	probabilityOfBiasRandomMutation = float( loadData["probabilityOfBiasRandomMutation"] );
	constantDistanceOfBias = float( loadData["constantDistanceOfBias"] );
	maxAmplifier = float( loadData["maxAmplifier"] );
	minAmplifier = float( loadData["minAmplifier"] );
	probabilityOfAmplifierRandomMutation = float( loadData["probabilityOfAmplifierRandomMutation"] );
	maximumAmplifierPercentVariation = float( loadData["maximumAmplifierPercentVariation"] );
	constantDistanceOfAmplifier = float( loadData["constantDistanceOfAmplifier"] );
	mutationProbability = float( loadData["mutationProbability"] );
	changeFunctionRate = float( loadData["changeFunctionRate"] );
	constantDistanceOfFunction = float( loadData["constantDistanceOfFunction"] );

	loadData.clear();
}

void CPPNNeuronUserDefinitions::printInfo()
{
	std::cout << 
	"constantDistanceOfFunction: " << constantDistanceOfFunction << "\n"<<
	"changeFunctionRate: " << changeFunctionRate << "\n"<<
	"maxBias: " << maxBias << "\n"<<
	"minBias: " << minBias << "\n"<<
	"maximumBiasPercentVariation: " << maximumBiasPercentVariation << "\n"<<
	"probabilityOfBiasRandomMutation: " << probabilityOfBiasRandomMutation << "\n"<<
	"constantDistanceOfBias: " << constantDistanceOfBias << "\n"<<
	"maxAmplifier: " << maxAmplifier << "\n"<<
	"minAmplifier: " << minAmplifier << "\n"<<
	"probabilityOfAmplifierRandomMutation: " << probabilityOfAmplifierRandomMutation << "\n"<<
	"maximumAmplifierPercentVariation: " << maximumAmplifierPercentVariation << "\n"<<
	"constantDistanceOfAmplifier: " << constantDistanceOfAmplifier << "\n" <<
	"mutationProbability: " << mutationProbability << "\n";
}

}