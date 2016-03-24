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
	this->globalInformation = in.globalInformation;
	identificator=id++;
	lastOutput=0.0;
}
// Este metodo se debe usar una sola vez por NEAT
Input::Input(GlobalInformation * globalInformation)
{
	this->globalInformation = globalInformation;
	identificator=id++;
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
Neuron * Input::createNewInput(Neuron * prototype)
{
	Input * proto = NULL;
	proto = dynamic_cast<Input*>(prototype);
	if ( proto != NULL)
	{
		Input * I = new Input();
		I->globalInformation = proto->globalInformation;
		I->historicalMark = globalInformation->getNeuronInputHistoricalMark();
		I->layer = globalInformation->getNeuronInputLayer();
		return I;
	}

	std::cerr << "Error::Input::createNew::Prototype is not of Input class" << std::endl;
	exit(EXIT_FAILURE);
}


int Input::getHistoricalMark()
{
	return historicalMark;
}

int Input::getInitialNeuronInHistoricalMark()
{
	return INPUT_INICIAL_IN_OUT;
}

int Input::getInitialNeuronOutHistoricalMark()
{
	return INPUT_INICIAL_IN_OUT;
}

std::vector <int> Input::getOutcomingConnections()
{
	return outcomingConections;
}

std::vector <int> Input::getIncomingConnections()
{
	return incomingConections;
}


void Input::addNewIncomingConection( int innovationOfIncomingConection )
{
	incomingConections.push_back( innovationOfIncomingConection );
}

void Input::addNewOutcomingConection( int innovationOfOutcomingConection )
{
	outcomingConections.push_back( innovationOfOutcomingConection );
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
	I->globalInformation=globalInformation;
	return I;
}