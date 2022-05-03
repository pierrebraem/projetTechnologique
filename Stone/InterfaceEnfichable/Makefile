# This file is part of the C++ dlopen mini HOWTO. You can find the complete
# HOWTO and/or updated versions at
# http://www.isotton.com/howtos/C++-dlopen-mini-HOWTO/
#
# Copyright 2002-2006 Aaron Isotton <aaron@isotton.com>
# Licensed under the GNU GPL.

all: example run

example: main.cpp panelAddon.hpp plugInMouvement.so MySerial.cpp stone.cpp myMqtt.cpp
	g++	-fPIC -std=c++2a -o example main.cpp MySerial.cpp stone.cpp myMqtt.cpp -ldl -lpthread -lmosquittopp

plugInMouvement.so: plugInMouvement.cpp panelAddon.hpp stone.cpp MySerial.cpp myMqtt.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o plugInMouvement.so plugInMouvement.cpp stone.cpp MySerial.cpp myMqtt.cpp

clean:
#	rm -f example plugInFumee.so

run:
	./example ttyUSB0

.PHONY: run clean