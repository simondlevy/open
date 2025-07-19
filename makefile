### TENNLab Open-Source Neuromorphic Framework

# JSP: My goal with this makefile is to keep it simple.  If you want a more
# complex makefile, please create a new file, and then document here how to
# use it.  Please don't modify this file to use your favorite features of make.

# This makefile is super-simple: Its goal is to make one library and two programs:
#
# lib/libframework.a is the library, containing network and processor support.
# bin/network_tool is a command-line tool for creating and manipulating networks.
# bin/processor_tool_risp is a command-line tool for loading networks onto the RISP
#                         simulator and then running the simulator.
#
# You'll note that the processor_tool code is agnostic as to the processor, using
# the processor interface from include/framework.hpp.  It should be clear how to
# compile this for a different processor.

CXX ?= g++

FR_INC = include/framework.hpp

FR_CFLAGS = -std=c++11 -Wall -Wextra -Iinclude -Iinclude/utils $(CFLAGS)

RISP_INC = include/risp.hpp

all: bin/processor_tool_risp

test: bin/processor_tool_risp
	bin/processor_tool_risp < sinewave_pt_input.txt

clean:
	rm -f bin/* obj/* lib/*

bin/processor_tool_risp: src/processor_tool.cpp $(FR_INC) $(RISP_INC) 
	$(CXX) $(FR_CFLAGS) -o bin/processor_tool_risp src/processor_tool.cpp 
