#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h> 
#include "cirMgr.h"
#include <typeinfo>
#include <stdlib.h>
#include <time.h> 

using namespace std;


unsigned str2int( string numStr ) {
  stringstream ss;
  ss << numStr;
  unsigned num;
  ss >> num;
  return num;
}

unsigned ignoreFrontChar( string &str ) {
  stringstream ss;
  ss << str;
  while( ss.peek() > 57 || ss.peek() < 48 ) {
    ss.ignore();
    if( ss.eof() )
      return 0;
  }
  unsigned num;
  ss >> num;
  return num;
}

void printRunningTime( clock_t start ) {
  double delta = clock() - start;
  cout<< "Running Time: " <<  delta/CLOCKS_PER_SEC << 's' << endl;
}

int main( int argc, char* argv[] )
{
  clock_t t0;
  t0 = clock();
  bool test = 0;
  ifstream ifs;
  string s;
  stringstream ss;
  srand(time(NULL));
  if ( test )
    ifs.open( "input_pa1/input_5.dat",ifstream::in );
  else
    ifs.open( argv[1],ifstream::in );
  getline( ifs ,s );
  ss << s;
  double r;
  ss >> r;
  // cout << "r = " << r << endl;
  CirMgr cirMgr;
  vector<bool> check;
  unsigned preNetIdx = 0;
  while ( getline( ifs ,s ,';' ) )
  {
    if( ifs.eof() )
      break;
    ss.clear();
    ss.str( "" );
    ss << s;
    ss >> s; // skip "NET"
    ss >> s; 
    unsigned netIdx = ignoreFrontChar(s); // skip n
    assert( netIdx > 0 );
    while ( cirMgr.net.cells.size() < netIdx + 1 )
      cirMgr.net.cells.push_back( vector<Cell*>() );
    if ( preNetIdx == 0 ) {
      for ( unsigned checkIdx = 0; checkIdx < check.size(); checkIdx++ ) 
        check[ checkIdx ] = false;
    }
    else {
      for ( unsigned cellIdx =  0; cellIdx < cirMgr.net.cells[ preNetIdx ].size(); cellIdx++ )
        check[ cirMgr.net.cells[ preNetIdx ][ cellIdx ]->ID ] = false;
    }
    preNetIdx = netIdx;
    while ( getline( ss ,s ,' ' ) ) {
      unsigned cellIdx = ignoreFrontChar(s); //skip c
      if( cellIdx == 0 )
        continue;
      while ( cirMgr.cellList.size() < cellIdx + 1 )
        cirMgr.cellList.push_back( new Cell( cirMgr.cellList.size() ) );
      while ( check.size() < cellIdx +1 )
        check.push_back( false );
      if ( !check[ cellIdx ] ) {
        cirMgr.net.cells[ netIdx ].push_back( cirMgr.cellList[ cellIdx ] );
        cirMgr.cellList[ cellIdx ]->netIDs.push_back(netIdx);
        check[ cellIdx ] = true;
      }
    }
  }
  cirMgr.r = r;
  cirMgr.set2partEqully_nodeInNets();
  cirMgr.iterationLoop();
  cirMgr.outputResult( argv[2] );
  printRunningTime(t0);
}
