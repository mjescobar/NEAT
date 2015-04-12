#include "BasicSynapticWeight.hpp"
#include <cstdlib>
#include <ctime>

using namespace NEATSpikes;
using namespace std;

int main()
{
	srand(time(0)); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	// Se harán dos conexiones sinápticas, la primera se inicializará con el constructor que carga las características del usuario y inicializa el valor de la conexión sináptica con valor aleatorio, la otra conexión sináptica no será inicializada para posteiormente cargar desde archivo usando el método load. Es inportante notar que  la segunda conexión sináptica cargará la misma información que la primera porque de hecho es parte de la prueba que se guarde correctamente la primera.
	BasicSynapticWeight * bsw = new BasicSynapticWeight("./userDefinitions");
	BasicSynapticWeight * bsw2 = new BasicSynapticWeight;
	// Con el fin de probar la evaluación de una conexión se probará si funciona ante una entrada random.
	
	double random = rand()/(double)RAND_MAX; // Este valor random corresponde a la entrada de la conexión sináptica.
	cout << "First synaptic weight" << endl;
	// Se imprime la primera conexión sináptica según los valores que obtuvo en el constructor.
	bsw->printState(); 
	// Se prueba como actúa con la entrada random, debería multiplicarla por el peso sináptico que se debería imprimir por pantalla con el método printState así que se puede confirmar viendo el resultado.
	bsw->setInput(random);
	bsw->spread();
	cout << "input: " << random << "\toutput: " <<  bsw->getOutput() << endl;
	// Ahora se probará si funcionan los métodos de guardado de información.
	cout << "Saving data" << endl;
	// Se guarda la conexión sináptica
	bsw->saveState("./save/");
	// Se guardan las definiciones de usuario para conexiones sinápticas.
	bsw->saveUserDefinitions("./save/");
	// Ahora se probará si el método para cargar conexiónes está funcional usando el método load, el resultado debería ser que esta otra conexión sináptica tenga las mismas características de la primera.
	cout << "Loading data" << endl;
	bsw2->load("./save/BSW0");
	cout << "Second synaptic weight" << endl;
	// Se imprime la información de la segunda conexión sináptica, debería ser igual a la primera que se mostro por la anterior conexión sináptica.
	bsw2->printState();
	// Se evalúa esta segunda conexión con la misma entrada random que la primera, el resultado debiese ser exactamente el mismo.
	cout << "input: " << random << "\toutput: " <<  bsw2->getOutput() << endl;	
	
	cout << "Now, we mutate the SynapticWeight 5 times" << endl;
	for (int i = 0; i < 100; ++i) // La idea es probar visualmente que no hay problemas evidentes al mutar.
	{
		bsw2->mutate();
		bsw2->printState();
	}

	bsw2->setInput(random);
	bsw2->spread();
	cout << "input: " << random << "\toutput: " <<  bsw2->getOutput() << endl;


	return EXIT_SUCCESS;
}