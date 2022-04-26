# This file is part of the C++ dlopen mini HOWTO. You can find the complete
# HOWTO and/or updated versions at
# http://www.isotton.com/howtos/C++-dlopen-mini-HOWTO/
#
# Copyright 2002-2006 Aaron Isotton <aaron@isotton.com>
# Licensed under the GNU GPL.



example: main.cpp panelAddon.hpp plugInFumee.so MySerial.cpp rapidxml.hpp rapidxml_utils.hpp stone.h
	$(CXX) $(CXXFLAGS)	-fPIC -std=c++2a -o example main.cpp panelAddon.hpp plugInFumee.so MySerial.cpp rapidxml.hpp rapidxml_utils.hpp stone.h -lstdc++fs -ldl -lpthread

plugInFumee.so: plugInFumee.cpp panelAddon.hpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o plugInFumee.so plugInFumee.cpp

clean:
	rm -f example plugInFumee.so

run:
	./example

.PHONY: clean
