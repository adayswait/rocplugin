all:plugin.h plugin.cc
	g++ plugin.cc -fPIC -shared -o echo.so