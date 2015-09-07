#include "MutationControl.hpp"
#include "BasicNeuron.hpp"
#include "GlobalInformation.hpp"
#include "BasicSynapticWeight.hpp"
#include "BasicNeuron.hpp"
#include "GlobalInformation.hpp"
#include "ANN.hpp"

// Aunque es la prueba de MutationControl es necesario tener una red neuronal y luego probar la clase MutationControl.
using namespace NEATSpikes;
int main(int argc, char ** argv)
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	
	GlobalInformation * globalInfo = new GlobalInformation();
	BasicNeuron * BN = new BasicNeuron(globalInfo, "./BasicNeuronUserDefinitions");
	BasicSynapticWeight * BSW = new BasicSynapticWeight("./BasicSynapticUserDefinitions" , globalInfo);
	ANN * ann = new ANN(BN, BN, BN, BSW, "./userDefANN" , globalInfo);
	
	std::cout << "ANN 1\n============================================================\n";
	ann->printState();
	ANN * ann2;
	ann2 = ann->duplicate();


	ann->testPrint();
	ann->mutate();
	ann->testPrint();
	ann2->testPrint();
	std::cout << "ANN 1\n============================================================\n";
	ann->printState();
	std::cout << "ANN 2\n============================================================\n";
	ann2->printState();

	vector <double> output = ann->eval({1.0,1.0});
	std::cout << output.at(0)  << std::endl;
	return 0;
}