#include "LIFNeuronUserDefinitions.hpp"

#include <fstream> // ofstream
#include <map> // map
#include <iostream>
#include <cstring>

namespace NEAT
{

LIFNeuronUserDefinitions::~LIFNeuronUserDefinitions(  )
{

}

LIFNeuronUserDefinitions::LIFNeuronUserDefinitions(  ): LIFNeuronUserDefinitions( "./LIFNUD" )
{

}

LIFNeuronUserDefinitions::LIFNeuronUserDefinitions( std::string path )
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
	probabilityOfResistenceRandomMutation = float(loadData["probabilityOfResistenceRandomMutation"]);
	maximumResistencePercentVariation = float(loadData["maximumResistencePercentVariation"]);
	maxResistence = float(loadData["maxResistence"]);
	minResistence = float(loadData["minResistence"]);
	probabilityOfMembraneTimeConstantRandomMutation = float(loadData["probabilityOfMembraneTimeConstantRandomMutation"]);
	maximumMembraneTimeConstantPercentVariation = float(loadData["maximumMembraneTimeConstantPercentVariation"]);
	maxMembraneTimeConstant = float(loadData["maxMembraneTimeConstant"]);
	minMembraneTimeConstant = float(loadData["minMembraneTimeConstant"]);
	probabilityOfCurrentTimeConstantRandomMutation = float(loadData["probabilityOfCurrentTimeConstantRandomMutation"]);
	maximumCurrentTimeConstantPercentVariation = float(loadData["maximumCurrentTimeConstantPercentVariation"]);
	maxCurrentTimeConstant = float(loadData["maxCurrentTimeConstant"]);
	minCurrentTimeConstant = float(loadData["minCurrentTimeConstant"]);
	constantDistanceResistence = float(loadData["constantDistanceResistence"]);
	constantDistanceMembraneTimeConstant = float(loadData["constantDistanceMembraneTimeConstant"]);
	constantDistanceCurrentTimeConstant = float(loadData["constantDistanceCurrentTimeConstant"]);
	resetVoltage = float(loadData["resetVoltage"]);
	spikeThreshold = float(loadData["spikeThreshold"]);
	mutationProbability = float(loadData["mutationProbability"]);
	
	loadData.clear();
}

void LIFNeuronUserDefinitions::printInfo()
{

	std::cout << 
	"mutationProbability: " << mutationProbability << "\n" <<
	"resetVoltage: " << resetVoltage << "\n" <<
	"spikeThreshold: " << spikeThreshold << "\n" <<
	"probabilityOfResistenceRandomMutation: " << probabilityOfResistenceRandomMutation << "\n" <<
	"maximumResistencePercentVariation: " << maximumResistencePercentVariation << "\n" <<
	"maxResistence: " << maxResistence << "\n" <<
	"minResistence: " << minResistence << "\n" <<
	"probabilityOfMembraneTimeConstantRandomMutation: " << probabilityOfMembraneTimeConstantRandomMutation << "\n" <<
	"maximumMembraneTimeConstantPercentVariation: " << maximumMembraneTimeConstantPercentVariation << "\n" <<
	"maxMembraneTimeConstant: " << maxMembraneTimeConstant << "\n" <<
	"minMembraneTimeConstant: " << minMembraneTimeConstant << "\n" <<
	"probabilityOfCurrentTimeConstantRandomMutation: " << probabilityOfCurrentTimeConstantRandomMutation << "\n" <<
	"maximumCurrentTimeConstantPercentVariation: " << maximumCurrentTimeConstantPercentVariation << "\n" <<
	"maxCurrentTimeConstant: " << maxCurrentTimeConstant << "\n" <<
	"minCurrentTimeConstant: " << minCurrentTimeConstant << "\n" <<
	"constantDistanceResistence: " << constantDistanceResistence << "\n" <<
	"constantDistanceMembraneTimeConstant: " << constantDistanceMembraneTimeConstant << "\n" <<
	"constantDistanceCurrentTimeConstant: " << constantDistanceCurrentTimeConstant << "\n" ;

}

}