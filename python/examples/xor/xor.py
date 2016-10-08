import neat
BNseed = neat.BasicNeuron()
BSWseed = neat.BasicSynapticWeight()
ann1 = neat.ANN(BNseed, BSWseed)
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


