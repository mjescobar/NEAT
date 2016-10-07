// Se desea probar el comportamiento de crossOver
// Es dificil de ver porque el metodo crossOver produce en si mutaciones y por lo tanto hay que fijarse bien.



#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 

using namespace NEAT;
using namespace std;

int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	
	auto BNseed = make_unique < BasicNeuron > (  );  
	auto BSWseed = make_unique < BasicSynapticWeight > (); 
	auto ann1  = make_unique < ANN > ( move(BNseed), move(BSWseed)  );
	

	// for (int i = 0; i < 1; ++i)
	// {
	// 	ann1->mightMutate();
	// }

	ann1->setInputs({0,0});
	ann1->spread();
	std::vector <float> out1 = ann1->getOutputs();
	std::cout << "{0,0}->"<< out1.at(0) << std::endl;
	ann1->setInputs({0,0});
	ann1->spread();
	std::vector <float> out2 = ann1->getOutputs();
	std::cout << "{0,0}->"<< out2.at(0) << std::endl;
	ann1->setInputs({1,1});
	ann1->spread();
	std::vector <float> out3 = ann1->getOutputs();
	std::cout << "{1,1}->"<< out3.at(0) << std::endl;


	std::cout << "=======================================================" << std::endl;

	auto ann2 = move(ann1->clone());
	ann2->setInputs({0,0});
	ann2->spread();
	std::vector <float> out4 = ann2->getOutputs();
	std::cout << "{0,0}->"<< out4.at(0) << std::endl;
	ann2->setInputs({0,0});
	ann2->spread();
	std::vector <float> out5 = ann2->getOutputs();
	std::cout << "{0,0}->"<< out5.at(0) << std::endl;
	ann2->setInputs({1,1});
	ann2->spread();
	std::vector <float> out6 = ann2->getOutputs();
	std::cout << "{1,1}->"<< out6.at(0) << std::endl;


	std::cout << "=======================================================" << std::endl;

	auto ann3 = move(ann1->crossOver(*ann2));
	ann3->setInputs({0,0});
	ann3->spread();
	std::vector <float> out7 = ann3->getOutputs();
	std::cout << "{0,0}->"<< out7.at(0) << std::endl;
	ann3->setInputs({0,0});
	ann3->spread();
	std::vector <float> out8 = ann3->getOutputs();
	std::cout << "{0,0}->"<< out8.at(0) << std::endl;
	ann3->setInputs({1,1});
	ann3->spread();
	std::vector <float> out9 = ann3->getOutputs();
	std::cout << "{1,1}->"<< out9.at(0) << std::endl;

	return 0;
}