import neat
import numpy as np
import random

p = neat.PyPopulation("../../experiments/user_definitions/user_def_standart", "../../experiments/genetics_encodings/test_encoding", "NEAT_XOR", "./XOR_results_files/")

for i in xrange(p.GENERATIONS):
    print "generation:", i
    o = p.organisms
    for j in xrange(len(p.organisms)):
        o[j].fitness = random.random()
    p.epoch()
    p.print_to_file_currrent_generation()

c = p.champion
print c.fitness, c.ANN_function(), c.JSON()
a = np.array([0.5,0,4.4])
a[0] = 0.0
print c.eval(a)

