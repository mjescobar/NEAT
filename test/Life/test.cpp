#include "Life.hpp"
#include "BasicNeuron.hpp"
#include "BasicSynapticWeight.hpp"

using namespace NEATSpikes;
using namespace std;

int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	InformationBetweenLifeAndOrganism * ilo = new InformationBetweenLifeAndOrganism();
	BasicSynapticWeight * BSW = new BasicSynapticWeight("./BSWUserDef");
	BasicNeuron * BN = new BasicNeuron("./BNUserDef");
	Life NeatPopulation = Life("./LifeUserDef", "./NicheUserDef",BN,BSW,"./ANNUserDef",ilo );
	NeatPopulation.printState();
	for (int j = 0; j < 100; ++j)
	{
		vector < vector <Organism *> > AllNewOrganismToCalculateFiteness = NeatPopulation.getAllNewOrganisms();

		for (unsigned int i = 0; i < AllNewOrganismToCalculateFiteness.size(); ++i)
		{
			for (unsigned int j = 0; j < AllNewOrganismToCalculateFiteness.at(i).size() ; ++j)
			{
				AllNewOrganismToCalculateFiteness.at(i).at(j)->setFitness( rand()/(double)RAND_MAX );
			}
		}
		// NeatPopulation.printState();
		NeatPopulation.epoch();
		
	}
	


}