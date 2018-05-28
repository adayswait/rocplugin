all:roc_interface.h roc_interface.cc
	g++ roc_interface.cc -fPIC -shared -o echo.so
