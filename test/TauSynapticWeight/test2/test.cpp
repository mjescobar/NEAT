// Test de funcionalidad de gertDistance

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
	auto tsw2 = make_unique<TauSynapticWeight> ();
	tsw->printInfo();
	tsw2->printInfo();
	cout << "DIff::" << tsw->getDistance(tsw2.get()) << endl;
	
	return 0;
}