# This file is part of the C++ dlopen mini HOWTO. You can find the complete
# HOWTO and/or updated versions at
# http://www.isotton.com/howtos/C++-dlopen-mini-HOWTO/
#
# Copyright 2002-2006 Aaron Isotton <aaron@isotton.com>
# Licensed under the GNU GPL.

all: example run

example: main.cpp panelAddon.hpp plugInFumee.so plugInMouvement.so MySerial.cpp stone.cpp
	g++	-fPIC -std=c++2a -o example main.cpp MySerial.cpp stone.cpp -ldl -lpthread

plugInFumee.so: plugInFumee.cpp panelAddon.hpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o plugInFumee.so plugInFumee.cpp

plugInMouvement.so: plugInMouvement.cpp panelAddon.hpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o plugInMouvement.so plugInMouvement.cpp

clean:
#	rm -f example plugInFumee.so

run:
	./example ttyUSB0

.PHONY: run clean