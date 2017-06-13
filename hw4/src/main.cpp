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
  bool plot;
  if ( argc == 1 ) {
    inputPath = "testbench/case4";
    outputPath = "output/output7";
  } 
  else {
    assert( argc == 3 || argc == 4 );
    plot = argc == 4;
    inputPath = (string)argv[1];
    outputPath = (string)argv[2];
  }
	Graph graph;
	FloorPlan FP(graph);
	FP.parse(inputPath);

  double MST_cost = 0;
  double old_MST_cost = 0;
  SpanningTree _spanningTree(FP);
  _spanningTree.construnction();
  unsigned i = 0;
  while(1) {
    ++i;
    // cerr << "iter:" << i << endl;
    SteinerTree _steinerTree(FP);
    _steinerTree.addSteinerPoints();
    SpanningTree _spanningTree(FP);
    _spanningTree.construnction();
    old_MST_cost = MST_cost;
    MST_cost = _steinerTree.getMSTCost();
    // cout << "MST_cost: " << MST_cost << endl;
    if (i!=1) {
      assert(MST_cost <= old_MST_cost);
      // cout << "decrease ratio: " << (old_MST_cost-MST_cost)/old_MST_cost << "\%\n"  ;
      if ((old_MST_cost-MST_cost)/old_MST_cost < 0.001 )
        break;
    }
    // cout << endl;
  }
  cerr << "iter:" << ++i << endl;
  SteinerTree _steinerTree(FP);
  _steinerTree.construnction();
  if(plot) {
    _steinerTree.plot();
  }
  FP.output(outputPath); 
}
