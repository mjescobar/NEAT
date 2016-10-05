#from distutils.core import setup, Extension
from setuptools import setup
from setuptools.extension import Extension
from Cython.Build import cythonize



setup(name='neat', version='0.1', ext_modules = cythonize(Extension(
           "neat",
           sources=["neat.pyx", "../src/discrete_probabilities.cpp","../src/NEAT.cpp", "../src/random_function.cpp", "../src/genetic_encoding.cpp"],
           language="c++",
           include_dirs=["../headers"],
      )))

