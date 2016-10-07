// Se prueba que compile todo y se cree un organismo correctamente.
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
	
	orgm->printInfo();
	return 0;
}