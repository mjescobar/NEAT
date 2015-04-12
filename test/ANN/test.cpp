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
	
	//RPUEBA 1, PARA VER SI FUNCIONAN LAS MUTACIONES:

	/*for (int j = 0; j < 20; ++j)
	{
		GlobalInformation * ilo = new GlobalInformation();
		BasicSynapticWeight * BSW = new BasicSynapticWeight(argv[2]);
		BasicNeuron * BN = new BasicNeuron(argv[1]);
		ANN * ann = new ANN(BN,BSW,argv[3],ilo);
		//ann->printState();    


		for (int i = 0; i < 20; ++i)
		{
			std::cout << "ANN mutada numero " << i << "\n==========================================\n==========================================\n=========================================="<< std::endl;

			ann->mutate();
			ann->printState();
		}
	}
	
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	*/


	// PRUEBA 2 PARA VER FUNCIONAMIENTO DE EVAL() 

	/*GlobalInformation * ilo = new GlobalInformation();
	BasicSynapticWeight * BSW = new BasicSynapticWeight(argv[2]);
	BasicNeuron * BN = new BasicNeuron(argv[1]);
	ANN * ann = new ANN(BN,BSW,argv[3],ilo);

	vector <double> output = ann->eval({1.0,1.0,1.0});
	ann->printState();
	cerr << "para las entradas 0,0,0 se tiene una salida: " <<  output.at(0) << endl;

	ann->mutate();
	ann->mutate();
	ann->mutate();
	ann->mutate();
	ann->printState();
	output = ann->eval({1.0,1.0,1.0});
	cerr << "para las entradas 0,0,0 se tiene una salida: " <<  output.at(0) << endl;
	*/
	

	//PRUEBA 3 PROBAR FUNCIONAMIENTO EVAL + MUTACIONES.

	// GlobalInformation * ilo = new GlobalInformation();
	// BasicSynapticWeight * BSW = new BasicSynapticWeight(argv[2]);
	// BasicNeuron * BN = new BasicNeuron(argv[1]);
	// ANN * ann = new ANN(BN,BSW,argv[3],ilo);

	// for (int i = 0; i < 20; ++i)
	// {
	// 	std::cout << "ANN mutada numero " << i << "\n==========================================\n==========================================\n=========================================="<< std::endl;

	// 	ann->mutate();
	// 	ann->printState();
	// }
	// vector <double> output = ann->eval({1.0,1.0,1.0});
	// cerr << "para las entradas 0,0,0 se tiene una salida: " <<  output.at(0) << endl;


	//Prueba 4: cruzamiento:


	GlobalInformation * ilo = new GlobalInformation();
	BasicSynapticWeight * BSW = new BasicSynapticWeight(argv[2],ilo);
	BasicNeuron * BN = new BasicNeuron(argv[1]);
	Organism * ann1 = new ANN(BN,BSW,argv[3],ilo);
	//Organism * ann2 = new ANN( *ann1);
	
	ann1->testPrint();
	


	//  for (int i = 0; i < 20; ++i)
	// {

	// 	ann1->mutate();
	// 	ann2->mutate();
	// }
	// ann1->printState();
	// ann2->printState();

	// Organism * ann3 = ann1->crossover(ann2);

	// ann3->printState();
	// cerr << "para las entradas 0,0,0 se tiene una salida: " << ann3->eval({1.0,1.0,1.0}).at(0) << endl; 



	// ann1->testPrint();
	// ann2->testPrint();

	// cout << "organismDistance(ann1, ann2)" << organismDistance(ann1, ann2) << endl; 

	// ann3->testPrint();



	return EXIT_SUCCESS;
}