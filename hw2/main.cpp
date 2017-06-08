#include <iostream>
#include <assert.h> 
#include <typeinfo>
#include <stdlib.h>
#include <time.h> 
#include "FSA.h"
#include "BStarTree.h"
#include "FloorPlanning.h"

using namespace std;

int main( int argc, char* argv[] )
{  
  clock_t startTime = clock();
  srand(time(NULL));
  cout << "argc: " << argc << endl;
  BStarTree BST;
  string blockFile;
  string netFile;
  string outputPath;
  if ( argc == 1 ) {
    cout << "default path" << endl;
    blockFile = "./input_pa2/ami33.block";
    netFile = "./input_pa2/ami33.nets";
    BST.alpha = 0.5;
  } else if( argc == 5 ) {
    BST.alpha = stod( (string)argv[1] );
    blockFile = (string)argv[2];
    netFile = (string)argv[3];
    outputPath = (string)argv[4];
    cout << " blockFile::" << blockFile << endl;
  } else if( argc == 6 ) {
    BST.alpha = stod( (string)argv[1] );
    blockFile = (string)argv[2];
    netFile = (string)argv[3];
    outputPath = (string)argv[4];
    BST.setPlotFlag();
  }
  BST.parse( blockFile, netFile );
  BST.randonConstructTree();
  FSA fsa( &BST );
  fsa.SA();
  fsa.setStartTime( startTime );
  fsa.output( outputPath );
}
