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
	tsw->printInfo();
	for (int i = 0; i < 10; ++i)
	{
		tsw->mightMutate();
	}
	tsw->printInfo();
	return 0;
}