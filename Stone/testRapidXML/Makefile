# This file is part of the C++ dlopen mini HOWTO. You can find the complete
# HOWTO and/or updated versions at
# http://www.isotton.com/howtos/C++-dlopen-mini-HOWTO/
#
# Copyright 2002-2006 Aaron Isotton <aaron@isotton.com>
# Licensed under the GNU GPL.

all: TestStone run

TestStone:  main.cpp MySerial.cpp rapidxml.hpp
	g++	-fPIC -std=c++2a -o TestStone MySerial.cpp rapidxml.hpp main.cpp -lpthread

clean:
# 	rm -f SerialPort.so

run:
	./TestStone ttyUSB0

# 	.PHONY: run clean
.PHONY: run clean
