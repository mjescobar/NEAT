#include "Parameter.hpp"

namespace NEAT
{

Parameter::Parameter( float defaultValue )
{
	isMutable = false;
	value = defaultValue;
}

Parameter::Parameter( float probabilityOfRandomMutation, 
		float maximumPercentVariation,
		float maxAdmissibleValue,
		float minAdmissibleValue)
{
	isMutable = true;
	this->probabilityOfRandomMutation = probabilityOfRandomMutation;
	this->maximumPercentVariation = maximumPercentVariation; 
	this->maxAdmissibleValue = maxAdmissibleValue; 
	this->minAdmissibleValue = minAdmissibleValue;
	//Se asigna un valor inicial aleatorio al parametro.
	value = (maxAdmissibleValue - minAdmissibleValue)  * rand()/(double)RAND_MAX + minAdmissibleValue;	
}
	
void Parameter::mutate()
{
	// El procedimiento que se realiza es un procedimiento generico el cual realiza 4 pasos importantes.
	// se da por supuesto que los valores finales despues de la mutacion deben pertenecer al intervalo [min, max] segun los correspondientes min y max de cada caracteristica.


	// Pasos
	// ====
	// 1) pasar los valores actuales que están en el intervalo [min,max] al intervalo [0,1] a través de una transformación afin.
	// 2) Obtener valor random en intervalo [0,1]
	// 3) Se ponderan ambos valores según se indican en el archivo de definiciones de usuario.
	// 4) El resultado es devuelto al intervalo [min, max] a través de la inverza a la función afin usada. 
	// OBSERVACIÓN: Existe la posibilidad que el valor deba ser obtenido de forma absolutamente aleatoria, ese caso es más sencillo.

	if (isMutable)
	{

		if( rand()/(double)RAND_MAX < probabilityOfRandomMutation )
		{
			value = (maxAdmissibleValue - minAdmissibleValue)  * rand()/(double)RAND_MAX + minAdmissibleValue;
		}

		else
		{
			// paso 1
			float value_normalized = (value - minAdmissibleValue)/(maximumPercentVariation - minAdmissibleValue);
			// paso 2
			float random_normalized = rand()/(double)RAND_MAX;
			// paso 3
			value_normalized = value_normalized * (1 - maximumPercentVariation) + random_normalized * maximumPercentVariation;
		 	// paso 4
		 	value = (maximumPercentVariation - minAdmissibleValue) * value_normalized + minAdmissibleValue;	
		}
	}
}

}