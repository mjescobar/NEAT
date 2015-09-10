#include "MutationControl.hpp"

using namespace NEATSpikes;


MutationControl::MutationControl()
{

}

//Este  metodo se realizara solo una vez por NEAT.
MutationControl::MutationControl(GlobalInformation * globalInformation, std::vector < Neuron * > * neurons,  bool connectionsBack, std::vector < SynapticWeight * > * synapticWeights, std::vector <int> * innovationToSynapticWeight, std::vector <int> * historicalMarkToNeuron, Neuron * neuronPrototype, SynapticWeight * synapticWeightPrototype, std::vector < std::vector< int > > * historicalMarkAtLayer )
{
	referenceMapForNeurons = new ReferenceMap(globalInformation, neurons, connectionsBack );
	referenceMapForSynapticsWeight = new ReferenceMap(globalInformation, neurons, connectionsBack );
	this->globalInformation = globalInformation;
	this->neurons = neurons;
	this->synapticWeights = synapticWeights;
	this->innovationToSynapticWeight = innovationToSynapticWeight;
	this->historicalMarkToNeuron = historicalMarkToNeuron;
	this->neuronPrototype = neuronPrototype;
	this->synapticWeightPrototype = synapticWeightPrototype;
	this->historicalMarkAtLayer = historicalMarkAtLayer;
}

MutationControl::~MutationControl()
{
	delete referenceMapForNeurons;
	delete referenceMapForSynapticsWeight;
}
void MutationControl::mutateNewNeuron()
{
	// Primero se agrega la nueva neurona y luego se agergan las dos conecciones sinapticas.
	//=====================================================================================
	std::vector <int> newNeuronWeightInMap = referenceMapForNeurons->obtainAvaibleMutationRandomly ();
	std::vector <int> historicalMarkOfNeurons = referenceMapForNeurons->getHistoricalMarkFromReferencePosition(newNeuronWeightInMap);
	
	Neuron * n = neuronPrototype->createNew( historicalMarkOfNeurons.at(0), historicalMarkOfNeurons.at(1));

	addNeuron (n);

	//Lo segundo es desactivar la coneccion sinaptica existente que unia las mismas neuronas pero directamente
	// notar que puede ser que no existiese antes esta innovacion
	//========================================================================================
	int innovationOfSynapticToDisactivate =  referenceMapForSynapticsWeight->get ( newNeuronWeightInMap );

	if(innovationOfSynapticToDisactivate != -1)
	{
		(*synapticWeights).at( (*innovationToSynapticWeight).at ( innovationOfSynapticToDisactivate ) )->disable();
	}


	//Se agregan las conecciones sinapticas nuevas, una desde la neurona inicial input a la nueva neurona y otra desde la neurona inicial output a la nueva neurona.
	//========================================================================================
	SynapticWeight * sw1 = synapticWeightPrototype->createNew(synapticWeightPrototype, historicalMarkOfNeurons.at(0), n->getHistoricalMark());
	SynapticWeight * sw2 = synapticWeightPrototype->createNew(synapticWeightPrototype,n->getHistoricalMark(), historicalMarkOfNeurons.at(1) );

	addSynapticWeight(sw1);
	addSynapticWeight(sw2);


}


void MutationControl::mutateNewSynapticWeight()
{
	//Si existen mutaciones posibles entonces se busca una equiprobablemente, en caso contrario simplemente no se realiza nada.
	if(getAmountOfSynapticWeightMutationAvaible() > 0)
	{
		//Se obtiene aleatoriamente un par de neuronas disponibles para crear una coneccion sinaptica entre ellas
		std::vector <int> newSynapticWeightInMap = referenceMapForSynapticsWeight->obtainAvaibleMutationRandomly ();
		//Se calculan los valores de marca historica del par de neuronas.
		std::vector <int> historicalMarkOfNeurons = referenceMapForSynapticsWeight->getHistoricalMarkFromReferencePosition(newSynapticWeightInMap);
		
		// Se crea la coneccion sinaptica desde el prototipo
		//=================================================================================
		SynapticWeight * sw = synapticWeightPrototype->createNew(synapticWeightPrototype, historicalMarkOfNeurons.at(0), historicalMarkOfNeurons.at(1) );

		addSynapticWeight (sw);
	}
	else if (getAmountOfSynapticWeightMutationAvaible() < 0)
	{
		std::cerr << "ERROR::MutationControl::mutateNewSynapticWeight::getAmountOfSynapticWeightMutationAvaible less than 0\t getAmountOfSynapticWeightMutationAvaible is:  " << getAmountOfSynapticWeightMutationAvaible() << std::endl;
		exit(EXIT_FAILURE);
	}
}


MutationControl * MutationControl::duplicate(std::vector < Neuron * > * _neurons, std::vector < SynapticWeight * > * synapticWeights, std::vector<int> * innovationToSynapticWeight, std::vector <int> * historicalMarkToNeuron, std::vector < std::vector< int > > * historicalMarkAtLayer   )
{
	//Notar que en el duplicate hay que tomar en cuenta que deben ser los mismos vectores de neurons que su respectiva red neuronal por lo tanto no servira copiar esos valores.
	MutationControl * result = new  MutationControl;
	// Hay ciertas cosas que se pueden duplicar o simplemente copiar de la existente, otras seran necesarias que sean entregadas desde la red nueronal.

	//Primero los datos que se obtienen desde aqui
	//========================================================================================
	result->referenceMapForSynapticsWeight = referenceMapForSynapticsWeight->duplicate(_neurons);
	result->referenceMapForNeurons = referenceMapForNeurons->duplicate(_neurons);
	result->globalInformation = this->globalInformation;
	result->neuronPrototype = this->neuronPrototype;
	result->synapticWeightPrototype = this->synapticWeightPrototype; 

	//Ahora las que se obtienen desde la red neuronal.
	//========================================================================================
	result->neurons = _neurons;
	result->synapticWeights = synapticWeights;
	result->innovationToSynapticWeight = innovationToSynapticWeight;
	result->historicalMarkToNeuron = historicalMarkToNeuron;
	result->historicalMarkAtLayer = historicalMarkAtLayer;


	return result;
}





void MutationControl::addNeuron(Neuron * neuron)
{
	// Primero se va a hacer un test pequeno para asegurar que todo esta bien, se vera si los historicalMark initial in e initial out existen en esta red neuronal, en caso de que no existiesen entonces generara un error y terminara la ejecucion.
	int historicalMarkInitionIn  = neuron->getInitialNeuronInHistoricalMark();
	int historicalMarkInitionOut = neuron->getInitialNeuronOutHistoricalMark();
	
	if( !( (int)(*historicalMarkToNeuron).size() > historicalMarkInitionIn &&  (int)(*historicalMarkToNeuron).size() > historicalMarkInitionOut))
	{
		std::cerr << "ERROR::MutationControl::addNeuron::Neuron parent are not in ANN" << std::endl;
		exit(EXIT_FAILURE);
		
	}
	if(!( (*historicalMarkToNeuron).at(historicalMarkInitionIn) != -1 &&  (*historicalMarkToNeuron).at(historicalMarkInitionOut) != -1) )
	{
		std::cerr << "ERROR::MutationControl::addNeuron::Neuron parent are not in ANN" << std::endl;
		exit(EXIT_FAILURE);
	}
		
 	// En este punto se supondra que esta todo Ok, aunque aun hay factores de reisgo pero es menos probable.
	//======================================================================================

		
	// Se calculan primero el lugar que corresponde a las neuronas input y output en el mapa
	// =====================================================================================

	int neuronsPositionOfInitialIn =  (*historicalMarkToNeuron).at(historicalMarkInitionIn);
	int neuronsPositionOfInitialOut =  (*historicalMarkToNeuron).at(historicalMarkInitionOut);

	int mapCoordinateOne;
	int mapCoordinateTwo;

	if ( neuronsPositionOfInitialOut > neuronsPositionOfInitialIn )
	{
		mapCoordinateOne = neuronsPositionOfInitialOut;
		mapCoordinateTwo =  2*neuronsPositionOfInitialOut-neuronsPositionOfInitialIn;
	}
	else
	{
		mapCoordinateOne = neuronsPositionOfInitialIn;
		mapCoordinateTwo =  neuronsPositionOfInitialOut;
	}


	// Se setea el valor en el mapa de referencias
	//========================================================================================
	referenceMapForNeurons->set ( {mapCoordinateOne,mapCoordinateTwo} , neuron->getHistoricalMark() );

	//El paso mas importante es agregar la neurona a la red neuronal.
	(*neurons).push_back(neuron);


	//Se actualiza historicalMarkToNeuron
	//========================================================================================
	while( neuron->getHistoricalMark() >= (int)(*historicalMarkToNeuron).size()  ) 
	{
		(*historicalMarkToNeuron).push_back(-1);
	}
	(*historicalMarkToNeuron).at(neuron->getHistoricalMark()) = (*neurons).size() - 1;

	//Se agrega esta neurona en el layer correspondiente
	//========================================================================================
	while(  neuron->getLayer()  >= (int)(*historicalMarkAtLayer).size() ) 
	{
		(*historicalMarkAtLayer).push_back({});
	}
	(*historicalMarkAtLayer).at(neuron->getLayer()).push_back(neuron->getHistoricalMark());

	//Se actualizan los mapas para que tengan correctamente setados los valores y la cantidad de mutaciones posibles
	//=========================================================================================
	referenceMapForSynapticsWeight->addNewReferenceMapVector();
	referenceMapForNeurons->addNewReferenceMapVector();
}

void MutationControl::addSynapticWeight( SynapticWeight * synapticWeight )
{
	//Primero se verificara si existen las neuronas a las que esta coneccion sinaptica conecta.
	int historicalMarkInitionIn = synapticWeight->getHistoricalMarkOfNeuronIn();
	int historicalMarkInitionOut = synapticWeight->getHistoricalMarkOfNeuronOut();


	

	// Se calculan primero el lugar que corresponde a las neuronas input y output en el mapa
	// =====================================================================================

	int neuronsPositionOfInitialIn =  (*historicalMarkToNeuron).at(historicalMarkInitionIn);
	int neuronsPositionOfInitialOut =  (*historicalMarkToNeuron).at(historicalMarkInitionOut);

	int mapCoordinateOne;
	int mapCoordinateTwo;

	if ( neuronsPositionOfInitialOut > neuronsPositionOfInitialIn )
	{
		mapCoordinateOne = neuronsPositionOfInitialOut;
		mapCoordinateTwo =  2*neuronsPositionOfInitialOut-neuronsPositionOfInitialIn;
	}

	else
	{
		mapCoordinateOne = neuronsPositionOfInitialIn;
		mapCoordinateTwo =  neuronsPositionOfInitialOut;
	}	
	

	//En el mapa de referencias para conecciones se da el valor en la coordenada antes obtenida aleatoriamente y se disminuye en uno la cantidad de posibles mutaciones de coneccion.
	referenceMapForSynapticsWeight->set ( {mapCoordinateOne,mapCoordinateTwo} , synapticWeight->getInnovation() );


	// Se agrega al vector de conexiones sinapticas de la red neuronal
	//===============================================================================================
	(*synapticWeights).push_back(synapticWeight);

	//Se actualiza valor de innovationToSynapticWeight para que todo funcione bien.
	//===============================================================================================

	while( synapticWeight->getInnovation() >= (int)(*innovationToSynapticWeight).size())
	{
		(*innovationToSynapticWeight).push_back(-1);
	}
	(*innovationToSynapticWeight).at(synapticWeight->getInnovation()) = (*synapticWeights).size() - 1;


	//Se agrega la coneccion sinaptica a la lista de conecciones entrantes/salientes de las neuronas correspondientemente
	(*neurons).at( (*historicalMarkToNeuron).at( historicalMarkInitionIn ) )->addNewOutcomingConection(synapticWeight->getInnovation());

	(*neurons).at( (*historicalMarkToNeuron).at( historicalMarkInitionOut ) )->addNewIncomingConection(synapticWeight->getInnovation());


}

int MutationControl::getAmountOfNeuronMutationAvaible()
{
	return referenceMapForNeurons->getAmountOfAvaibleMutations();
}
int MutationControl::getAmountOfSynapticWeightMutationAvaible()
{
	return referenceMapForSynapticsWeight->getAmountOfAvaibleMutations();
}

void MutationControl::print()
{
	std::cout << "ReferenceSynapticMap" << std::endl;
	std::cout << "===================="<< std::endl;
	referenceMapForSynapticsWeight->print();  
	std::cout << "ReferenceNeuronMap" << std::endl;
	std::cout << "===================="<< std::endl;
	referenceMapForNeurons->print();
}



void MutationControl::AddInitialNeuron(Neuron * neuron)
{
	(*neurons).push_back(neuron);

	//Se actualiza historicalMarkToNeuron
	//========================================================================================
	while( neuron->getHistoricalMark() >= (int)(*historicalMarkToNeuron).size() ) 
	{
		(*historicalMarkToNeuron).push_back(-1);
	}
	(*historicalMarkToNeuron).at(neuron->getHistoricalMark()) = (*neurons).size() - 1;

	//Se agrega esta neurona en el layer correspondiente
	//========================================================================================
	while( neuron->getLayer() >= (int)(*historicalMarkAtLayer).size() ) 
	{
		(*historicalMarkAtLayer).push_back({});
	}
	(*historicalMarkAtLayer).at(neuron->getLayer()).push_back(neuron->getHistoricalMark());

	//Se actualizan los mapas para que tengan correctamente setados los valores y la cantidad de mutaciones posibles
	//=========================================================================================
	referenceMapForSynapticsWeight->addNewReferenceMapVector();
	referenceMapForNeurons->addNewReferenceMapVector();
}