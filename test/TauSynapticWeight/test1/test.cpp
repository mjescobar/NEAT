// Test de funcionalidad de TauSynapticWeight

#include <NEATSpikes> 
#include <memory>
#include <iostream>
#include <ctime>       // time 
#include <cstdlib>     // srand, rand 
using namespace NEAT;
using namespace std;


int main()
{

	srand(time(0)); 
	auto tsw = make_unique<TauSynapticWeight> ();
	tsw->printInfo();
	for (int i = 0; i < 10; ++i)
	{
		tsw->setInput(0.1f*(i+1.f));
		tsw->spread();
		cout << "entrada: "<< 0.1f*(i+1.f)<< " \tsalida: " << tsw->getOutput() << endl;;
	}
	return 0;
}