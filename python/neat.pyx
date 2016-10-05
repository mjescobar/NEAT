from libcpp.string cimport string
from libcpp.vector cimport vector

from numpy cimport ndarray
cimport numpy as np

cdef extern from "../headers/genetic_encoding.hpp" namespace "ANN_USM":
    cdef cppclass Genetic_Encoding:
        Genetic_Encoding() except +
        vector[double] eval(vector[double])
        string JSON()
        string ANN_function()
        double fitness
        #void run(double* X, int N, int D, double* Y)

cdef class PyGenetic_Encoding:
    cdef Genetic_Encoding *thisptr
    def __cinit__(self):
        #self.thisptr = new Genetic_Encoding()
        pass
    def __dealloc__(self):
        #del self.thisptr
        pass
    def JSON(self):
        return self.thisptr.JSON()
    property fitness:
        def __get__(self):
            return self.thisptr.fitness
        def __set__(self, double value):
            self.thisptr.fitness = value
    def eval(self, ndarray[np.float64_t, ndim=1] a):
        #if a.shape[0] != 
        return self.thisptr.eval(a)
    def ANN_function(self):
        return self.thisptr.ANN_function()

cdef PyGenetic_Encoding_to_Python(Genetic_Encoding *b):
    result = PyGenetic_Encoding()
    result.thisptr = b
    return result

cdef extern from "../headers/NEAT.hpp" namespace "ANN_USM":
    cdef cppclass Population:
        Population(char *, char *, char *, char *) except +
        #void run(double* X, int N, int D, double* Y)
        int GENERATIONS
        int POPULATION_MAX
        void epoch()
        void print_to_file_currrent_generation()
        Genetic_Encoding champion
        vector[Genetic_Encoding] organisms



cdef class PyPopulation:
    cdef Population *thisptr
    def __cinit__(self, str path1, str path2, str _name, str _save_path):
        self.thisptr = new Population(path1.encode('UTF-8') , path2.encode('UTF-8') , _name.encode('UTF-8') , _save_path.encode('UTF-8') )
    def __dealloc__(self):
        del self.thisptr
    @property
    def GENERATIONS(self):
        return self.thisptr.GENERATIONS
    @property
    def POPULATION_MAX(self):
        return self.thisptr.POPULATION_MAX
    def epoch(self):
        self.thisptr.epoch()
    def print_to_file_currrent_generation(self):
        self.thisptr.print_to_file_currrent_generation()
    @property
    def champion(self):
        return PyGenetic_Encoding_to_Python(&self.thisptr.champion)
    @property
    def organisms(self):
        organisms = range(self.thisptr.organisms.size())
        for i in xrange(len(organisms)):
            organisms[i] = PyGenetic_Encoding_to_Python(&self.thisptr.organisms[i])
        return organisms

    """
    def run(np.ndarray[np.double_t, ndim=2] X):
        X = np.ascontiguousarray(X)
        cdef np.ndarray[np.double_t, ndim=2, mode="c"] Y = np.zeros_like(X)
        
        cdef Population myclass
        myclass = Population()
        #myclass.run(&X[0,0], X.shape[0], X.shape[1], &Y[0,0])

        return Y
    """
