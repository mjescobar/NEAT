#include "MutationControl.hpp"
#include "BasicNeuron.hpp"
#include "GlobalInformation.hpp"
#include "BasicSynapticWeight.hpp"
#include "BasicNeuron.hpp"
#include "GlobalInformation.hpp"
#include "ANN.hpp"
#include "Niche.hpp"

// Aunque es la prueba de MutationControl es necesario tener una red neuronal y luego probar la clase MutationControl.
using namespace NEATSpikes;
int main(int argc, char ** argv)
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.

	GlobalInformation * globalInfo = new GlobalInformation();
	BasicNeuron * BN = new BasicNeuron(globalInfo, "./BasicNeuronUserDefinitions");
	BasicSynapticWeight * BSW = new BasicSynapticWeight("./BasicSynapticUserDefinitions" , globalInfo);
	ANN * ann = new ANN(BN, BN, BN, BSW, "./userDefANN" , globalInfo);
	Niche * niche =  new Niche(ann, "./NicheUserDef");
	std::vector <ANN *> ann_vector = niche->getNewOrganismVector();
	std::cout << "ann_vector.size(): " << ann_vector.size() << std::endl;
	ann_vector.at(0)->setFitness(1.0);
	niche->maturation();
	niche->obtainOneOrganismToCrossoverWithAnotherNicheOrganism()->printState();
	ANN * ann2 = niche->obtainAChildren();
	std::cerr << "UNOOOOOOOOOO\n";
	if(niche->IsAcepted(ann2) )
	{
		std::cout << "FUE ACEPTADOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n";
		niche->addOrganism(ann2);
		std::cout << "2!!!" << std::endl;
	}
	
	ann_vector = niche->getNewOrganismVector();
	for (unsigned int i = 0; i < ann_vector.size(); ++i)
	{
		ann_vector.at(i)->setFitness( i + 2.0);
	}
	niche->maturation();
	
	niche->printState();
	niche->maturation();
	ann_vector = niche->getOrganismVector();
	niche->printState();
	niche->epoch();
	std::cerr << "EL TAMAANO Es  " << ann_vector.size() << std::endl;
	ann_vector = niche->getOrganismVector();
	niche->printState();
	niche->epoch();
	std::cerr << "EL TAMAANO Es  " << ann_vector.size() << std::endl;
	ann_vector = niche->getOrganismVector();
	niche->printState();
	niche->epoch();
	std::cerr << "EL TAMAANO Es  " << ann_vector.size() << std::endl;
	ann_vector = niche->getOrganismVector();
	niche->printState();
	niche->epoch();
	std::cerr << "EL TAMAANO Es  " << ann_vector.size() << std::endl;


	return 0;
}