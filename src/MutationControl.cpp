#include "MutationControl.hpp"

using namespace NEATSpikes;

//Este  metodo se realizara solo una vez por NEAT.
MutationControl::MutationControl(GlobalInformation * globalInformation, std::vector < Neuron * > * neurons,  bool connectionsBack, int inputAmount, int outputAmount,std::vector < SynapticWeight * > * synapticWeights, std::vector <int> innovationToSynapticWeight, std::vector <int> historicalMarkToNeuron, Neuron * neuronPrototype, SynapticWeight * synapticWeightPrototype )
{
	referenceMapForNeurons = new ReferenceMap(globalInformation, neurons, connectionsBack, inputAmount, outputAmount );
	referenceMapForSynapticsWeight = new ReferenceMap(globalInformation, neurons, connectionsBack, inputAmount, outputAmount );
	this->globalInformation = globalInformation;
	this->neurons = neurons;
	this->synapticWeights = synapticWeights;
	this->innovationToSynapticWeight = innovationToSynapticWeight;
	this->historicalMarkToNeuron = historicalMarkToNeuron;
	this->neuronPrototype = neuronPrototype;
	this->synapticWeightPrototype = synapticWeightPrototype;

	// Solo falta agregar las conecciones iniciales.
	for (int in = 0; in < inputAmount; ++in)
	{
		for (int out = 0; out < outputAmount; ++out)
		{
			referenceMapForSynapticsWeight->set(out, 2*out - in); //Notar que dado que los input se agregan primeros en el vector de neuronas entonces siempre una coneccion de input a output termina en el vector del output dado la forma en que se agregan los vectores a los mapas, por eso se deben usar esos numeros.
		}
	}
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
	vector <int> newNeuronWeightInMap = referenceMapForNeurons->obtainAvaibleMutationRandomly ();
	vector <int> historicalMarkOfNeurons = referenceMapForNeurons->getHistoricalMarkFromReferencePosition(newNeuronWeightInMap);
	
	Neuron * n = neuronPrototype->createNew(neuronPrototype, historicalMarkOfNeurons.at(0), historicalMarkOfNeurons.at(1));
	referenceMapForNeurons->set ( newNeuronWeightInMap , n->getHistoricalMark() );
	(*neurons)->push_back(n);

	//Se actualiza historicalMarkToNeuron
	//========================================================================================
	while( historicalMarkToNeuron.size()-1 >  n->getHistoricalMark() ) 
	{
		historicalMarkToNeuron->push_back(-1);
	}
	historicalMarkToNeuron.at(n->getHistoricalMark()) = (*neurons).size() - 1;



	//Lo segundo es desactivar la coneccion sinaptica existente que unia las mismas neuronas pero directamente
	//========================================================================================
	int innovationOfSynapticToDisactivate =  referenceMapForNeurons->get ( newNeuronWeightInMap );
	(*synapticWeights).at(innovationToSynapticWeight ( innovationOfSynapticToDisactivate ) )->disable();


	//Se actualizan los mapas para que tengan correctamente setados los valores y la cantidad de mutaciones posibles
	//========================================================================================
	referenceMapForSynapticsWeight->addNewReferenceMapVector();
	referenceMapForNeurons->addNewReferenceMapVector();


	//Se agregan las conecciones sinapticas nuevas, una desde la neurona inicial input a la nueva neurona y otra desde la neurona inicial output a la nueva neurona.
	//========================================================================================


	
}

void MutationControl::mutateNewSynapticWeight()
{
	//Si existen mutaciones posibles entonces se busca una equiprobablemente, en caso contrario simplemente no se realiza nada.
	if(getAmountOfSynapticWeightMutationAvaible() > 0)
	{
		//Se obtiene aleatoriamente un par de neuronas disponibles para crear una coneccion sinaptica entre ellas
		vector <int> newSynapticWeightInMap = referenceMapForSynapticsWeight->obtainAvaibleMutationRandomly ();
		//Se calculan los valores de marca historica del par de neuronas.
		vector <int> historicalMarkOfNeurons = referenceMapForSynapticsWeight->getHistoricalMarkFromReferencePosition(newSynapticWeightInMap);
		
		// Se crea la coneccion sinaptica desde el prototipo
		//===============================================================================================
		SynapticWeight * sw = synapticWeightPrototype->createNew(synapticWeightPrototype, historicalMarkOfNeurons.at(0), historicalMarkOfNeurons.at(1) );

		//En el mapa de referencias para conecciones se da el valor en la coordenada antes obtenida aleatoriamente y se disminuye en uno la cantidad de posibles mutaciones de coneccion.
		referenceMapForSynapticsWeight->set ( newSynapticWeightInMap , sw->getInnovation() );


		// Se agrega al vector de conexiones sinapticas de la red neuronal
		//===============================================================================================
		(*synapticWeights).push_back(sw);

		//Se actualiza valor de innovationToSynapticWeight para que todo funcione bien.
		//===============================================================================================

		while(innovationToSynapticWeight.size()-1 > sw->getInnovation())
		{
			innovationToSynapticWeight.push_back(-1);
		}
		innovationToSynapticWeight.at(sw->getInnovation()) = (*synapticWeights).size() - 1;

	}
	else if (getAmountOfSynapticWeightMutationAvaible() < 0)
	{
		std::cerr << "ERROR::MutationControl::mutateNewSynapticWeight::getAmountOfSynapticWeightMutationAvaible less than 0" << std::endl;
		exit(EXIT_FAILURE);
	}
}


MutationControl * MutationControl::duplicate(std::vector < Neuron * > * neurons, std::vector < SynapticWeight * > * synapticWeights, std::vector<int> innovationToSynapticWeight, std::vector <int> historicalMarkToNeuron   )
{
	//Notar que en el duplicate hay que tomar en cuenta que deben ser los mismos vectores de neurons que su respectiva red neuronal por lo tanto no servira copiar esos valores.
}








int MutationControl::getAmountOfNeuronMutationAvaible()
{
	return referenceMapForNeurons->getAmountOfAvaibleMutations();
}
int MutationControl::getAmountOfSynapticWeightMutationAvaible()
{
	return referenceMapForSynapticsWeight->getAmountOfAvaibleMutations();
}