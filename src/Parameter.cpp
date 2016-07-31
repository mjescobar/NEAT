#include "Parameter.hpp"

namespace NEAT
{

Parameter::Parameter( float defaultValue )
{
	isMutable = false;
	value = defaultValue;
}

Parameter::Parameter( float probabilityOfRandomMutation, 
		float probabilityOfDeltaMutation,
		float maximumVariation,
		float maxAdmissibleValue,
		float minAdmissibleValue)
{
	isMutable = true;
	this->probabilityOfRandomMutation = probabilityOfRandomMutation;
	this->maximumVariation = maximumVariation; 
	this->maxAdmissibleValue = maxAdmissibleValue; 
	this->minAdmissibleValue = minAdmissibleValue;
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
			value = (maximumVariation - minAdmissibleValue)*(rand()/(double)RAND_MAX) + minAdmissibleValue;
		}

		else
		{
			// paso 1
			float value_normalized = (value - minAdmissibleValue)/(maximumVariation - minAdmissibleValue);
			// paso 2
			float random_normalized = rand()/(double)RAND_MAX;
			// paso 3
			value_normalized = value_normalized * (1 - maximumVariation) + random_normalized * maximumVariation;
		 	// paso 4
		 	value = (maximumVariation - minAdmissibleValue) * value_normalized + minAdmissibleValue;	
		}
	}
}

}