//
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
	auto BNseed = make_shared < BasicNeuron > ( );  
	auto BSWseed = make_shared < BasicSynapticWeight > ( ); 
	auto ann1  = make_unique < ANN > ( BNseed, BSWseed );
	auto orgm = make_unique < Organism > ( std::move(ann1) );
	auto race = make_unique < Race > (std::move(orgm));
	auto specie = make_unique <Spicies> (std::move(race));

	specie->printInfo();
	std::cerr << "JJJJJJJJJJ0" <<std::endl;
	for (int i = 0; i < 4; ++i)
	{
		specie->epoch(10);
	}
	specie->printInfo();
	std::cerr << "JJJJJJJJJJ1" <<std::endl;
	for (int i = 0; i < 100; ++i)
	{
		specie->epoch(10);
	}
	std::cerr << "JJJJJJJJJJ2" <<std::endl;
	specie->printInfo();


	return 0;
}