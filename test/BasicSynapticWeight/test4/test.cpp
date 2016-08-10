// Se observara el comportamiento de las clonaciones 
// El experimento completo sera la repeticion de los siguientes pasos
// Se creara uno inicial, el cual se clonara y luego se mutara y se observara que ocurre en cada uno
// Se debe observar si la clonacion separa correctamente toda la informacion de cada uno y si no se pierde 
// los smart pointers.


#include <NEATSpikes>
#include <memory>
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 
#include <iostream> //cout

int main()
{
	std::srand(std::time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	
	auto BSWUD = std::make_unique < NEAT::BasicSynapticWeightUserDefinitions > ( "./BSWUD" );
	auto BSW1 = std::make_unique < NEAT::BasicSynapticWeight > ( *BSWUD );


	BSWUD->printInfo();
	for (int i = 0; i < 10; ++i)
	{
		std::cout << "TEST NUMBER " << i+1 << std::endl;
		std::cout << "BSW1 Before clone: " << std:: endl;
		BSW1->printInfo(); 

		auto BSW2 = BSW1->clone();
		std::cout << "BSW1 After clone: " << std:: endl;
		BSW1->printInfo();


		std::cout << "BSW2 " << std:: endl;
		BSW2->printInfo();

		std::cout << "BSW2 is been mutated: " << std:: endl;
		BSW2->mightMutate();

		std::cout << "BSW1 After BSW2 is mutated: " << std:: endl;
		BSW1->printInfo();


		std::cout << "BSW2 After BSW2 is mutated: " << std:: endl;
		BSW2->printInfo();
	}

	return 0;
}