#include "ANN.hpp"
#include "BasicNeuron.hpp"
#include "BasicSynapticWeight.hpp"
#include "GlobalInformation.hpp"
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace NEATSpikes;

int main(int argc, char ** argv)
{
	srand( time( 0 ) ); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	
	GlobalInformation * ilo = new GlobalInformation();
	BasicSynapticWeight * BSW = new BasicSynapticWeight(argv[2],ilo);
	BasicNeuron * BN = new BasicNeuron(argv[1]);
	Organism * ann1 = new ANN(BN,BSW,argv[3],ilo);
	//Organism * ann2 = new ANN(*ann1);
	
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();

	ann1->mutate();
	ann1->mutate();

	ann1->testPrint();
	ilo->printLayers();
	/*ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	cerr << "ANN2" << endl;
	ann2->testPrint();
	cerr << "ANN1" << endl;
	ann1->testPrint();
	cerr << "HIJO" << endl;*/
	/*Organism * ann3 = crossover(ann1,ann2);
	ann3->testPrint();*/

/*	GlobalInformation * ilo = new GlobalInformation();
	BasicSynapticWeight * BSW = new BasicSynapticWeight(argv[2],ilo);
	BasicNeuron * BN = new BasicNeuron(argv[1]);
	Organism * ann1 = new ANN(BN,BSW,argv[3],ilo);
	Organism * ann2 = new ANN( *ann1);
	Organism * ann3 = new ANN( *ann1);
	cerr << "ANN1.0" << endl;
	ann1->testPrint();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	cerr << "ANN1.1" << endl;
	ann1->testPrint();
	ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	cerr << "ANN2" << endl;
	ann2->testPrint();

	Organism * child = crossover( ann1, ann2);
	Organism * child2 = crossover( child, ann3);
	Organism * child3 = crossover(ann3, child2);
	
cerr << "Hijo 1" << endl;
	child->testPrint();

cerr << "Hijo 3" << endl;
	child3->testPrint();
	child->mutate();
	child->mutate();
	child->mutate();
	child->mutate();
	child->mutate();
	child->mutate();
	child2->mutate();
	child2->mutate();
	child2->mutate();
	child2->mutate();
	child2->mutate();
	child2->mutate();
	child2->mutate();
	cerr << "Hijo 2" << endl;
	child2->testPrint();
	child3->mutate();
	child3->mutate();
	child3->mutate();
	child3->mutate();
	child3->mutate();
cerr << "Hijo 3.2" << endl;

	child3->testPrint();
cerr << "Hijo 3.3" << endl;
Organism * child4 = crossover(child3, child2);
	child4->testPrint();*/

	return EXIT_SUCCESS;
}