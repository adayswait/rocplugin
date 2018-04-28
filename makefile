all:plugin.h plugin.cc
	g++ plugin.cc -fPIC -shared -o plugin.so