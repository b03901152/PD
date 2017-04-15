CC = g++
DEBUG = -g -pg
# DEBUG = -O3
CFLAGS = -Wall $(DEBUG) -std=c++11

all:
	$(CC) $(CFLAGS) main.cpp FSA.cpp FloorPlanning.cpp BStarTree.cpp -lboost_system -lboost_filesystem -lboost_iostreams -o hw2 
clean:
	rm -rf *.o *.gprof hw1.exe lib/*.a lib/*.o bin/*
