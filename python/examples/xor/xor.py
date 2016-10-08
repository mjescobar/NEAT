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
    data = [[[0,0], 0], [[0,1],1],[[1,0],1],[[1,1],0]]
    out = []
    error = 0
    for d in data:
        d_in, d_out = d
        orgm.ann.setInputs(d_in)
        orgm.ann.spread()
        out_ = orgm.ann.getOutputs()
        error += abs(abs(out_[0]) - d_out)
        out.append(out_)
    error_MAX = len(data)
    fitness = (error_MAX - error)**2
    if fitness > maxFitness:
        maxFitness = fitness
        print "MF:", maxFitness, "\t{", out[0][0], ",", out[1][0], ",", out[2][0], ",", out[3][0]
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


