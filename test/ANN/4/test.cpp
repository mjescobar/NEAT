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

	//ANN * ann2 = ann->createNewWithSameTopologieButDiferentValues();
	ANN * ann2 = ann->duplicate();
	ann->mutate();
	ann->mutate();
	ann->mutate();

	std::cout << "ANN 1\n============================================================\n";
	ann->testPrint();
	//ann->printState();

	std::cout << "ANN 2\n============================================================\n";
	ann2->mutate();
	//ann2->mutate();
	//ann2->mutate();

	ann2->testPrint();
	//ann2->printState();

	ANN * ann3 = crossover(ann,ann2);

	std::cout << "ANN 3\n============================================================\n";
	ann3->testPrint();
	//ann3->printState();

	std::vector <double> input = {2.0*((double)rand()/RAND_MAX) -1.0 , 2.0*((double)rand()/RAND_MAX) -1.0};
	std::cerr << "input: {" << input.at(0) << "," << input.at(1) << "}\toutput: "  <<  ann3->eval(input).at(0) << std::endl;

	ann3->saveState("./save/");

	ANN * ann4 = new ANN;
	ann4->load("./save/", BN, BN, BN, BSW, "./userDefANN" , globalInfo);
	ann4->testPrint();
	ann4->printState();
	std::cerr << "input: {" << input.at(0) << "," << input.at(1) << "}\toutput: "  <<  ann4->eval(input).at(0) << std::endl;
	return 0;
}