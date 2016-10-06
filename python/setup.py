#from distutils.core import setup
from setuptools import setup
setup (name = 'neat',
       version = '0.1',
       author = "Linus Casassa <lcasassa@gmail.com>",
       description = """Expose to python the Neat v2. https://github.com/mjescobar/NEAT/""",
       py_modules = ["neat"],
       packages=[''],
       package_data={'': ['_neat.so']},
       )
