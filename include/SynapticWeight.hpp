#ifndef SYNAPTICWEIGHT_HPP
#define SYNAPTICWEIGHT_HPP	
/*
	Esta clase es abstracta, la  idea es poder probar diferentes tipos de neuronas de forma sencilla.
*/

#include <string>
#include <iostream>
namespace NEATSpikes{
	class SynapticWeight{
	public:
		virtual ~SynapticWeight(){};
		/**
			\brief Bajo cierta probabilidad pueden cambiar características de esta conexión sináptica.
		*/
		virtual void mutate()=0;
	
		/**
			\brief Toda Conexión debe ser capas de guardar toda la información necesaria para poder volver a crearla.
			\param pathToSave es la ruta a donde se quiere guardar la conexión sináptica.
		*/
		virtual void saveState(std::string pathToSave)=0;
		/**
			\brief Se carga una conexión sináptica a través de una anteriormente guardada.
			\param Es la ruta donde está la conexión sináptica guardada que se quiere cargar.
		*/
		virtual void load(std::string PathWhereIsSaved)=0;
		/**
			\brief se imprime toda la información de la conexión sináptica.
		*/
		virtual void printState()=0;
		/**
			\brief Se crea uno con valores random. Esto es necesario porque a priori desde la clase ANN no se sabe el tipo de conexión sináptica que se está usando.
		*/
		virtual SynapticWeight * createNew(int histoticalMark_Neuron_in , int histoticalMark_Neuron_out )=0;

		virtual void disable()=0;
		virtual int getInnovation()=0;
		virtual int getHistoricalMarkOfNeuronIn()=0;
		virtual int getHistoricalMarkOfNeuronOut()=0;
		virtual double getOutput()=0;
		virtual void setInput(double input)=0;
		virtual void spread()=0;
		virtual SynapticWeight * duplicate()=0;
		virtual double getDistance(SynapticWeight * syn)=0;
		virtual void changeValuesRandomly()=0;



	};
}
#endif