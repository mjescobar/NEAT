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
	float inputVoltage = 1.f;

	lifn->printInfo();

	float totalTime_seg = 1.f; 
	for (uint sample_i = 0; sample_i*sampleTime_seg < totalTime_seg ; ++sample_i)
	{
		lifn->sumIncomingVoltage(inputVoltage);
		lifn->spread();
		cout << lifn->getOutput()  << "\t" << lifn->getMembraneVoltage() << "\t" << sample_i << endl;
	}
	return 0;
}