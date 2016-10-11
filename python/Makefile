CFLAGS=-O3
run:
	swig -python -c++ neat.i
	c++ -std=c++14 -Wall -fPIC $(CFLAGS) -c neat_wrap.cxx $(shell pkg-config python2 --cflags)
	c++ $(CFLAGS) -shared ../objects/*.o neat_wrap.o -o _neat.so
clean:
	rm -rf neat.py neat_wrap.cxx neat.pyc _neat.so neat_wrap.o dist neat.egg-info build
install: run
	pip show neat|grep neat && pip uninstall neat -y || echo " "
	python setup.py bdist_wheel
	pip install dist/neat-*.whl
