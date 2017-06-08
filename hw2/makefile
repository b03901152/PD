CC = g++
# DEBUG = -g -pg
# DEBUG = -O3 -pg
DEBUG = -O3
CFLAGS = -Wall $(DEBUG) -std=c++11

all:  FSA
FSA:	
	$(CC) $(CFLAGS) main.cpp FSA.cpp FloorPlanning.cpp BStarTree.cpp -lboost_system -lboost_filesystem -lboost_iostreams -o FSA
clean:
	rm -rf *.o *.gprof FSA lib/*.a lib/*.o bin/*
