#include "Input.hpp"

using namespace NEATSpikes;

int Input::id=0;

Input::Input()
{
	identificator=id++;
	lastOutput=0.0;
}

Input::~Input()
{

}

Input::Input(const Input & in)
{
	
}

Input::Input(int _historicalMark, int historicalMark_inicial_input, int historicalMark_inicial_output, int _layer)
{
	layer=_layer;
	historicalMark=_historicalMark;
	identificator=id++;
	historicalMarkNeuronInicialIn=historicalMark_inicial_input;
	historicalMarkNeuronInicialOut=historicalMark_inicial_output;
	lastOutput=0.0;
}
void Input::mutate()
{
	// Input no realiza mutaciones.
}

double Input::eval(){
	lastOutput = inputVoltage;
	inputVoltage = 0.0;
	return lastOutput;
}

void Input::sumIncomingConnectionsOutputs(double incomingOneConnectionOutput)
{
	inputVoltage += incomingOneConnectionOutput;
}
void Input::saveState(std::string pathToSave)
{
	
	std::string finalArchive = pathToSave + "INPUT" + std::to_string(identificator);

	// Se agrega el organismo en el index
	std::ofstream index;
	index.open(pathToSave+"index", std::ios::out | std::ios::app);
	index << identificator << std::endl;
	index.close();

	// Se crea el archivo y se guardan los datos.
	std::ofstream finalFile;
	finalFile.open(finalArchive);
	finalFile << "identificator " << identificator << "\nlayer " << layer << "\nhistoricalMark "<< historicalMark;
	finalFile.close();
}
void Input::load(std::string PathWhereIsSaved)
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
	layer = loadData["layer"];
	historicalMark = loadData["historicalMark"];
	//=========================================================================================
}
void Input::printState()
{
	std::cout << "Input\t" << "historicalMark " << historicalMark << "\tlayer " << layer <<  "\tlastOutput" << lastOutput   <<std::endl;
}
Neuron * Input::createNew(int historicalMark, int historicalMark_inicial_input, int historicalMark_inicial_output, int layer)
{
	Input * I = new Input(historicalMark, historicalMark_inicial_input, historicalMark_inicial_output, layer);
	return dynamic_cast<Neuron*>(I);
	//Neuron & N = dynamic_cast<Neuron&> (*I);
	//return N;
}


int Input::getHistoricalMark()
{
	return historicalMark;
}

int Input::getInitialNeuronInHistoricalMark()
{
	return historicalMarkNeuronInicialIn;
}

int Input::getInitialNeuronOutHistoricalMark()
{
	return historicalMarkNeuronInicialOut;
}

std::vector <int> Input::getOutcomingConnections()
{
	return outcomingConections;
}

std::vector <int> Input::getIncomingConnections()
{
	return incomingConections;
}


void Input::addNewIncomingConection(int innovationOfIncomingConection)
{
	incomingConections.push_back(innovationOfIncomingConection);
}

void Input::addNewOutcomingConection(int innovationOfOutcomingConection)
{
	outcomingConections.push_back(innovationOfOutcomingConection);
}


double Input::getLastOutput(){
	return lastOutput;
}

int Input::getLayer()
{
	return layer;
}



Neuron * Input::duplicate()
{
	Input * I = new Input();
	I->inputVoltage=inputVoltage;
	I->lastOutput=lastOutput;
	I->identificator=identificator;
	I->layer=layer;
	I->historicalMark=historicalMark;
	I->historicalMarkNeuronInicialIn=historicalMarkNeuronInicialIn;
	I->historicalMarkNeuronInicialOut=historicalMarkNeuronInicialOut;
	I->incomingConections=incomingConections;
	I->outcomingConections=outcomingConections;
	return I;
}