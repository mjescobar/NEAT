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

	// ANN * ann2 = ann->createNewWithSameTopologieButDiferentValues();

	// std::cout << "ANN 1\n============================================================\n";
	ann->printState();

	std::vector <double> input = {2.0*((double)rand()/RAND_MAX) -1.0 , 2.0*((double)rand()/RAND_MAX) -1.0};
	std::cerr << "input: {" << input.at(0) << "," << input.at(1) << "}\toutput: "  <<  ann->eval(input).at(0) << std::endl;
	return 0;
}