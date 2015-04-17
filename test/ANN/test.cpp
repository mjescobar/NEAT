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
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	


	GlobalInformation * ilo = new GlobalInformation();
	BasicSynapticWeight * BSW = new BasicSynapticWeight(argv[2],ilo);
	BasicNeuron * BN = new BasicNeuron(argv[1]);
	Organism * ann1 = new ANN(BN,BSW,argv[3],ilo);
	Organism * ann2 = new ANN( *ann1);
	Organism * ann3 = new ANN( *ann1);
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->mutate();
	ann1->testPrint();
	ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	ann2->mutate();
	ann2->testPrint();

	Organism * child = ann1->crossover(ann2);
	Organism * child2 = child->crossover(ann3);
	Organism * child3 = ann3->crossover(child2);
	
	child->testPrint();
	child2->testPrint();
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
	child3->mutate();
	child3->mutate();
	child3->mutate();
	child3->mutate();
	child3->mutate();
	child3->testPrint();

	return EXIT_SUCCESS;
}