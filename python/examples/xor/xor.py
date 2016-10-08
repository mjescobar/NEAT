import neat

BNUD = neat.BasicNeuronUserDefinitions()
BNUD.maxBias = 0
BNUD.minBias = 0
BNUD.maximumBiasPercentVariation = 0.1
BNUD.probabilityOfBiasRandomMutation = 0.02
BNUD.constantDistanceOfBias = 0.5
BNUD.maxSigmoidConstant = 4
BNUD.minSigmoidConstant = 2
BNUD.probabilityOfSigmoidConstantRandomMutation = 0.02
BNUD.maximumSigmoidConstantPercentVariation = 0.2
BNUD.constantDistanceOfSigmoidConstant = 1

BSWUD = neat.BasicSynapticWeightUserDefinitions()
BSWUD.maxWeightValue = 1
BSWUD.minWeightValue = -1
BSWUD.maximumWeightPercentVariation = 0.2
BSWUD.probabilityOfWeightRandomMutation = 0.03
BSWUD.probabilityOfEnableADisabledConnection = 0
BSWUD.constantDistanceOfSynapticWeightValue = 0.5

ANNUD = neat.ANNUserDefinitions()
ANNUD.probabilityNewNeuronInLayer = 0.05
ANNUD.probabilityOfNewSynapticWeight = 0
ANNUD.probabilityOfNewUniqueSynapticWeight = 0.05
ANNUD.probabilityOfNewLayer = 0.01
ANNUD.inputsAmount = 2
ANNUD.outputsAmount = 1



BNseed = neat.BasicNeuron(BNUD)
BSWseed = neat.BasicSynapticWeight(BSWUD)
ann1 = neat.ANN(ANNUD, BNseed, BSWseed)
life = neat.Life(ann1)

maxFitness = 0.0;
fitnessAcumm = 0.0;
contador = 0;
maxGeneration = 0.0;

def experiment( orgm ):
    global maxFitness, maxGeneration, fitnessAcumm, contador
    #XOR {0,0}->{0}
    #import ipdb; ipdb.set_trace()
    orgm.ann.setInputs([0.0, 0.0])
    orgm.ann.spread()
    out1 = orgm.ann.getOutputs()
    error = abs(abs(out1[0]) - 0.0)
    #XOR {0,1} -> {1}
    orgm.ann.setInputs([0.0,1.0])
    orgm.ann.spread()
    out2 = orgm.ann.getOutputs()
    error += abs(abs(out2[0]) - 1.0)
    #XOR {1,0} -> {1}
    orgm.ann.setInputs([1.0,0.0])
    orgm.ann.spread()
    out3 = orgm.ann.getOutputs()
    error += abs(abs(out3[0]) - 1.0)
    #XOR {1,1} -> {0}
    orgm.ann.setInputs([1.0,1.0])
    orgm.ann.spread()
    out4 = orgm.ann.getOutputs()
    error += abs(abs(out4[0]) - 0.0)
    #cerr << "error: " << error << "\t{" << out1.at(0) <<", " << out2.at(0) <<", " << out3.at(0) << ", " << out4.at(0) <<"} " << endl;
    error_MAX = 4.0
    fitness = (error_MAX - error)*(error_MAX - error)
    if fitness > maxFitness:
        maxFitness = fitness
        print "MF:", maxFitness, "\t{", out1[0], ",", out2[0], ",", out3[0], ",", out4[0]
        #orgm.printInfo()

    if fitness > maxGeneration:
        maxGeneration = fitness

    orgm.setFitness(fitness)
    fitnessAcumm += fitness
    contador += 1


def sendAllOrganismToExperiment( life ):
    size = life.getSizeOrganism()
    for i in xrange(size):
        orgm = life.getOrganism(i)
        experiment( orgm )


for i in xrange(100):
    sendAllOrganismToExperiment(life)
    fitnessAcumm = 0.0
    contador = 0
    life.epoch()
    print "MG: ", maxGeneration
    maxGeneration = 0.0

print "================================================"
life.printInfo()
print "maxFitness:", maxFitness


