// Se observara el comportamiento de spread al haber realizado mutaciones previamente,
// Se debe probar con conexiones hacia adelante y hacia atras para asegurar el buen funcionamiento
// general de SPREAD

#include <NEATSpikes> // NEAT Library needed see README if you have not installed.
#include <memory> // make_unique
#include <iostream> // cout
#include <cstdlib>     // srand, rand 
#include <ctime>       // time 


int main()
{
	std::srand(std::time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	auto BNUD = std::make_unique < NEAT::BasicNeuronUserDefinitions > ( "./BN_userdef" );
	auto BN1 = std::make_unique < NEAT::BasicNeuron > ( *BNUD );

	for (int i = 0; i < 5; ++i)
	{
		std::cout << "TEST NUMBER " << i+1 << std::endl;
		std::cout << "BN1 Before clone: " << std:: endl;
		BN1->printInfo(); 

		auto BN2 = BN1->clone();
		std::cout << "BN1 After clone: " << std:: endl;
		BN1->printInfo();


		std::cout << "BN2 " << std:: endl;
		BN2->printInfo();

		std::cout << "BN2 is been mutated: " << std:: endl;
		BN2->mightMutate();

		std::cout << "BN1 After BN2 is mutated: " << std:: endl;
		BN1->printInfo();


		std::cout << "BN2 After BN2 is mutated: " << std:: endl;
		BN2->printInfo();

	}

	return 0;

}