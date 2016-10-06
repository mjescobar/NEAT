import neat
import numpy as np

def fitness(g):
    e_sum = 0
    result = g.eval([0, 0])
    e_sum += abs(0 - result[0])
    result = g.eval([0, 1])
    e_sum += abs(1 - result[0])
    result = g.eval([1, 1])
    e_sum += abs(0 - result[0])
    result = g.eval([1, 0])
    e_sum += abs(1 - result[0])
    return e_sum

p = neat.PyPopulation("../../experiments/user_definitions/user_def_standart", "../../experiments/genetics_encodings/test_encoding", "NEAT_XOR", "./XOR_results_files/")

for i in xrange(p.GENERATIONS):
    print "generation:", i
    o = p.organisms
    for j in xrange(len(p.organisms)):
        o[j].fitness = fitness(o[j])
    p.epoch()
    p.print_to_file_currrent_generation()

c = p.champion
print c.fitness, c.ANN_function(), c.JSON()
a = np.array([0,0,0], dtype=np.float64)
print c.eval(a)

