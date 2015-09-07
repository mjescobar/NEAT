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
	std::cerr << "mutateNewNeuron::1" << std::endl;
	// Primero se agrega la nueva neurona y luego se agergan las dos conecciones sinapticas.
	//=====================================================================================
	std::vector <int> newNeuronWeightInMap = referenceMapForNeurons->obtainAvaibleMutationRandomly ();
	std::vector <int> historicalMarkOfNeurons = referenceMapForNeurons->getHistoricalMarkFromReferencePosition(newNeuronWeightInMap);
	
	Neuron * n = neuronPrototype->createNew( historicalMarkOfNeurons.at(0), historicalMarkOfNeurons.at(1));

	std::cerr << "mutateNewNeuron::1.5" << std::endl;
	addNeuron (n);
	std::cerr << "mutateNewNeuron::2" << std::endl;

	//Lo segundo es desactivar la coneccion sinaptica existente que unia las mismas neuronas pero directamente
	// notar que puede ser que no existiese antes esta innovacion
	//========================================================================================
	std::cerr << "mutateNewNeuron::3" << std::endl;
	int innovationOfSynapticToDisactivate =  referenceMapForSynapticsWeight->get ( newNeuronWeightInMap );

	if(innovationOfSynapticToDisactivate != -1)
	{
	std::cerr << "mutateNewNeuron::4" << std::endl;
		(*synapticWeights).at( (*innovationToSynapticWeight).at ( innovationOfSynapticToDisactivate ) )->disable();
	}

	std::cerr << "mutateNewNeuron::5" << std::endl;

	//Se agregan las conecciones sinapticas nuevas, una desde la neurona inicial input a la nueva neurona y otra desde la neurona inicial output a la nueva neurona.
	//========================================================================================
	std::cerr << "mutateNewNeuron::6" << std::endl;
	SynapticWeight * sw1 = synapticWeightPrototype->createNew(synapticWeightPrototype, historicalMarkOfNeurons.at(0), n->getHistoricalMark());
	std::cerr << "mutateNewNeuron::7" << std::endl;
	SynapticWeight * sw2 = synapticWeightPrototype->createNew(synapticWeightPrototype,n->getHistoricalMark(), historicalMarkOfNeurons.at(1) );
	std::cerr << "mutateNewNeuron::8" << std::endl;

	std::cerr << "mutateNewNeuron::9" << std::endl;
	addSynapticWeight(sw1);
	std::cerr << "mutateNewNeuron::10" << std::endl;
	addSynapticWeight(sw2);
	std::cerr << "mutateNewNeuron::11" << std::endl;


}


void MutationControl::mutateNewSynapticWeight()
{
	std::cerr << "MutationControl::1" << std::endl;
	//Si existen mutaciones posibles entonces se busca una equiprobablemente, en caso contrario simplemente no se realiza nada.
	if(getAmountOfSynapticWeightMutationAvaible() > 0)
	{
	std::cerr << "MutationControl::2" << std::endl;
		//Se obtiene aleatoriamente un par de neuronas disponibles para crear una coneccion sinaptica entre ellas
		std::vector <int> newSynapticWeightInMap = referenceMapForSynapticsWeight->obtainAvaibleMutationRandomly ();
		//Se calculan los valores de marca historica del par de neuronas.
		std::vector <int> historicalMarkOfNeurons = referenceMapForSynapticsWeight->getHistoricalMarkFromReferencePosition(newSynapticWeightInMap);
		
		// Se crea la coneccion sinaptica desde el prototipo
		//=================================================================================
	std::cerr << "MutationControl::3" << std::endl;
		SynapticWeight * sw = synapticWeightPrototype->createNew(synapticWeightPrototype, historicalMarkOfNeurons.at(0), historicalMarkOfNeurons.at(1) );

	std::cerr << "MutationControl::4" << std::endl;
		addSynapticWeight (sw);
	std::cerr << "MutationControl::5" << std::endl;
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
	std::cerr << "addNeuron::1\t!!!!!!!!!!!!!!!!!!!  " << (*neurons).size()  << std::endl;
	int historicalMarkInitionIn  = neuron->getInitialNeuronInHistoricalMark();
	int historicalMarkInitionOut = neuron->getInitialNeuronOutHistoricalMark();
	
	std::cerr << "addNeuron::2" << std::endl;
	if( !( (int)(*historicalMarkToNeuron).size() > historicalMarkInitionIn &&  (int)(*historicalMarkToNeuron).size() > historicalMarkInitionOut))
	{
	std::cerr << "addNeuron::3" << std::endl;
		std::cerr << "ERROR::MutationControl::addNeuron::Neuron parent are not in ANN" << std::endl;
		exit(EXIT_FAILURE);
		
	}
	std::cerr << "addNeuron::4" << std::endl;
	if(!( (*historicalMarkToNeuron).at(historicalMarkInitionIn) != -1 &&  (*historicalMarkToNeuron).at(historicalMarkInitionOut) != -1) )
	{
	std::cerr << "addNeuron::5" << std::endl;
		std::cerr << "ERROR::MutationControl::addNeuron::Neuron parent are not in ANN" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cerr << "addNeuron::6" << std::endl;
		
 	// En este punto se supondra que esta todo Ok, aunque aun hay factores de reisgo pero es menos probable.
	//======================================================================================

		
	// Se calculan primero el lugar que corresponde a las neuronas input y output en el mapa
	// =====================================================================================

	std::cerr << "addNeuron::7" << std::endl;
	int neuronsPositionOfInitialIn =  (*historicalMarkToNeuron).at(historicalMarkInitionIn);
	std::cerr << "addNeuron::8" << std::endl;
	int neuronsPositionOfInitialOut =  (*historicalMarkToNeuron).at(historicalMarkInitionOut);

	std::cerr << "addNeuron::9" << std::endl;
	int mapCoordinateOne;
	int mapCoordinateTwo;

	std::cerr << "addNeuron::10" << std::endl;
	if ( neuronsPositionOfInitialOut > neuronsPositionOfInitialIn )
	{
	std::cerr << "addNeuron::11" << std::endl;
		mapCoordinateOne = neuronsPositionOfInitialOut;
		mapCoordinateTwo =  2*neuronsPositionOfInitialOut-neuronsPositionOfInitialIn;
	}
	else
	{
	std::cerr << "addNeuron::12" << std::endl;
		mapCoordinateOne = neuronsPositionOfInitialIn;
		mapCoordinateTwo =  neuronsPositionOfInitialOut;
	}


	std::cerr << "addNeuron::13" << std::endl;
	// Se setea el valor en el mapa de referencias
	//========================================================================================
	referenceMapForNeurons->set ( {mapCoordinateOne,mapCoordinateTwo} , neuron->getHistoricalMark() );

	std::cerr << "addNeuron::14" << std::endl;
	//El paso mas importante es agregar la neurona a la red neuronal.
	(*neurons).push_back(neuron);

	std::cerr << "addNeuron::15\t!!!!!!!!!!!!!!!!!!!  " << (*neurons).size() << "\t" << neurons << std::endl;

	//Se actualiza historicalMarkToNeuron
	//========================================================================================
	while( neuron->getHistoricalMark() >= (int)(*historicalMarkToNeuron).size()  ) 
	{
		(*historicalMarkToNeuron).push_back(-1);
	}
	(*historicalMarkToNeuron).at(neuron->getHistoricalMark()) = (*neurons).size() - 1;

	//Se agrega esta neurona en el layer correspondiente
	//========================================================================================
	std::cerr << "addNeuron::16" << std::endl;
	while(  neuron->getLayer()  >= (int)(*historicalMarkAtLayer).size() ) 
	{
		(*historicalMarkAtLayer).push_back({});
	}
	(*historicalMarkAtLayer).at(neuron->getLayer()).push_back(neuron->getHistoricalMark());

	//Se actualizan los mapas para que tengan correctamente setados los valores y la cantidad de mutaciones posibles
	//=========================================================================================
	std::cerr << "addNeuron::17" << std::endl;
	referenceMapForSynapticsWeight->addNewReferenceMapVector();
	std::cerr << "addNeuron::18" << std::endl;
	referenceMapForNeurons->addNewReferenceMapVector();
	std::cerr << "addNeuron::19" << std::endl;
}

void MutationControl::addSynapticWeight( SynapticWeight * synapticWeight )
{
	std::cerr << "addSynapticWeight::1" << std::endl; 
	//Primero se verificara si existen las neuronas a las que esta coneccion sinaptica conecta.
	int historicalMarkInitionIn = synapticWeight->getHistoricalMarkOfNeuronIn();
	int historicalMarkInitionOut = synapticWeight->getHistoricalMarkOfNeuronOut();


	std::cerr << "addSynapticWeight::2" << std::endl; 
	

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
	std::cerr << "addSynapticWeight::2" << std::endl; 
	referenceMapForSynapticsWeight->set ( {mapCoordinateOne,mapCoordinateTwo} , synapticWeight->getInnovation() );


	// Se agrega al vector de conexiones sinapticas de la red neuronal
	//===============================================================================================
	std::cerr << "addSynapticWeight::3" << std::endl; 
	(*synapticWeights).push_back(synapticWeight);

	//Se actualiza valor de innovationToSynapticWeight para que todo funcione bien.
	//===============================================================================================

	std::cerr << "addSynapticWeight::4" << std::endl; 
	while( synapticWeight->getInnovation() >= (int)(*innovationToSynapticWeight).size())
	{
		(*innovationToSynapticWeight).push_back(-1);
	}
	(*innovationToSynapticWeight).at(synapticWeight->getInnovation()) = (*synapticWeights).size() - 1;


	//Se agrega la coneccion sinaptica a la lista de conecciones entrantes/salientes de las neuronas correspondientemente
	std::cerr << "addSynapticWeight::5" << std::endl; 
	(*neurons).at( (*historicalMarkToNeuron).at( historicalMarkInitionIn ) )->addNewOutcomingConection(synapticWeight->getInnovation());
	std::cerr << "addSynapticWeight::6\t" << (*neurons).size() << "\t" << (*historicalMarkToNeuron).size() <<"\t" <<historicalMarkInitionOut  << std::endl;

	std::cerr << "value: " <<  (*historicalMarkToNeuron).at( historicalMarkInitionOut ) << std::endl;
	(*neurons).at( (*historicalMarkToNeuron).at( historicalMarkInitionOut ) )->addNewIncomingConection(synapticWeight->getInnovation());
	std::cerr << "addSynapticWeight::7" << std::endl; 


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