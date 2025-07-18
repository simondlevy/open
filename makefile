CXX ?= g++

FR_CFLAGS = -std=c++11 -Wall -Wextra -Iinclude -Iinclude/utils $(CFLAGS)

newtest: bin/new_processor_tool_risp
	bin/new_processor_tool_risp < short_input.txt

test: bin/processor_tool_risp
	bin/processor_tool_risp < short_input.txt

clean:
	rm -f bin/* obj/* lib/*

bin/processor_tool_risp: src/processor_tool.cpp include/risp.hpp
	$(CXX) $(FR_CFLAGS) -o bin/processor_tool_risp src/processor_tool.cpp 

bin/new_processor_tool_risp: src/new_processor_tool.cpp include/new_risp.hpp
	$(CXX) $(FR_CFLAGS) -o bin/new_processor_tool_risp src/new_processor_tool.cpp 
