#include <iostream>
#include <assert.h> 
#include <typeinfo>
#include <stdlib.h>
#include <time.h> 
#include "FSA.h"
#include "BStarTree.h"
#include "FloorPlanning.h"


using namespace std;

void printRunningTime( clock_t start ) {
  double delta = clock() - start;
  cout<< "Running Time: " <<  delta/CLOCKS_PER_SEC << 's' << endl;
}

int main( int argc, char* argv[] )
{  
  clock_t t0 = clock();
  srand(time(NULL));
  BStarTree BST;
  // string blockFile = "../input_pa2_ami33.block";
  // string netFile = "../input_pa2_ami33.nets";

  string blockFile = "../input.block";
  string netFile = "../input.nets";

  BST.parse( blockFile, netFile );
  BST.randonConstructTree();
  // BST.printBST();
  // ifs.open( argv[1],ifstream::in );
  cout <<"aaa";
  printRunningTime( t0 );
}
