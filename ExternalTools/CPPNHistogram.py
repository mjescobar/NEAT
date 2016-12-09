'''

Instalación: 
-----------

instalar Python3 y matplotlib 
 1. python3 está generalmente en los repositorios oficiales de linux
 2. matplotlib puede estar en los repositorios oficiales, sino usando pip3
 	2.1: usando pip3 primero debe instalar pip de los repositorios oficiales, luego
 		(sudo) pip install matplotlib

Uso: en terminal 
----------------

$ python3 CPPNHistogram.py pathToFile

por ejemplo: 

$ python3 CPPNHistogram.py ../experimentSamples/XOR/save/champion


'''
import matplotlib.pyplot as plt
import numpy as np
import sys

class Neuron:
	def __init__(self, neuronPath):
		self.parameters = dict()
		file = open(neuronPath, "r")
		for line in file:
			splited = line.split()
			self.parameters[splited[0]] = splited[1]
		file.close()

class Synapse:
	def __init__(self, synapsePath):
		self.parameters = dict()
		file = open(synapsePath, "r")
		for line in file:
			splited = line.split()
			self.parameters[splited[0]] = splited[1]
		file.close()

class Layer:
	def __init__(self, layerPath):
		self.neurons = []
		file = open(layerPath, "r")
		for line in file:
			splited = line.split()
			if(splited[0] == 'N'):
				self.neurons.append(Neuron(layerPath+'N'+splited[1]))
		file.close()

	def AddNeuron(neuronPath):
		neurons.append(Neuron(neuronPath))

class ANN :
	def __init__(self):
		self.layers = []
		self.synapses = []

	def load(self, annPath):
		file = open(annPath, "r")
		for line in file:
			splited = line.split()
			if(splited[0] == 'L'):
				self.AddLayer(annPath+'L'+splited[1])
			elif(splited[0] == 'S'):
				self.AddSynapse(annPath+'S'+splited[1])
		file.close()

	def AddLayer(self, layerPath):
		self.layers.append(Layer(layerPath))

	def AddSynapse(self, synapsePath):
		self.synapses.append(Synapse(synapsePath))

	def FunctionHistogram(self):
		functionIdNumbers = []
		for layer in self.layers:
			for neuron in layer.neurons:
				functionIdNumbers.append(int(neuron.parameters['functionId']))

		plt.hist( np.array(functionIdNumbers) )
		plt.title("Gaussian Histogram")
		plt.xlabel("Value")
		plt.ylabel("Frequency")
		plt.show()

if __name__ == '__main__':
	ann = ANN()
	ann.load(sys.argv[1])
	ann.FunctionHistogram()
