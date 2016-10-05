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
	float sampleTime_seg = 0.001;
	auto lifn = make_unique<LIFNeuron>( sampleTime_seg );

	lifn->printInfo();
	cout << "===" << endl;

	for (int i = 0; i < 10; ++i)
	{
		lifn->mightMutate();
		lifn->printInfo();
	cout << "===" << endl;
	}
	return 0;
}