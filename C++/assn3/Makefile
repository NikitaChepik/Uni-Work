all: Graph

Graph: GraphTest.o Graph.o
	g++ -fsanitize=address GraphTest.o Graph.o -o Graph

GraphTest.o: test1.cpp Graph.h
	g++ -std=c++14 -Wall -Werror -O2 -fsanitize=address -c test1.cpp
	
Graph.o: Graph.tem Graph.h
	g++ -std=c++14 -Wall -Werror -O2 -fsanitize=address -c Graph.tem

clean:
	rm *o Graph