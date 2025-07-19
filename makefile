CXX ?= g++

FR_CFLAGS = -std=c++11 -Wall -Wextra -Iinclude -Iinclude/utils $(CFLAGS)

RISP_INC = include/risp.hpp

all: bin/processor_tool_risp

test: bin/processor_tool_risp
	bin/processor_tool_risp < sinewave_pt_input.txt

clean:
	rm -f bin/* obj/* lib/*

bin/processor_tool_risp: src/processor_tool.cpp $(RISP_INC) 
	$(CXX) $(FR_CFLAGS) -o bin/processor_tool_risp src/processor_tool.cpp 
