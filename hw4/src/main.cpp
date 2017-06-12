#include <iostream>
#include "FloorPlan.h"
#include "SpanningTree.h"
#include "SteinerTree.h"
#include <string>
#include <vector>

using namespace std;
int main(int argc, char* argv[]) {
  string inputPath;
  string outputPath;
  if ( argc == 1 ) {
    inputPath = "testbench/case4";
    outputPath = "output/output7";
  } 
  else {
    assert( argc == 3 );
    inputPath = (string)argv[1];
    outputPath = (string)argv[2];
  }
	Graph graph;
	FloorPlan FP(graph);
	FP.parse(inputPath);

  SpanningTree _spanningTree(FP);
  _spanningTree.construnction();
  SteinerTree _steinerTree(FP);
  _steinerTree.construnction();

  FP.output(outputPath); 
}