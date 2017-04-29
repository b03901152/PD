CC = g++
# DEBUG = -g -pg
DEBUG = -O3 -pg
CFLAGS = -Wall $(DEBUG) -std=c++11

all:
	$(CC) $(CFLAGS) main.cpp FSA.cpp FloorPlanning.cpp BStarTree.cpp -lboost_system -lboost_filesystem -lboost_iostreams -o visualize
clean:
	rm -rf *.o *.gprof FSA lib/*.a lib/*.o bin/*
