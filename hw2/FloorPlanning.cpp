#include "FloorPlanning.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 

using namespace std;

void FloorPlanning::parse( string& blockFile, string& netFile )
{
  cout << "parse" << endl;
  cout << "blockFile: " << blockFile << endl;
  cout << "netFile: " << netFile << endl;
  ifstream ifs;
  string buf;
  // input.block name
  ifs.open( blockFile, ifstream::in );

  if ( !ifs.is_open() ) {
     cout << "Error occur when opening file " << blockFile  << endl;
     exit(1);
  }
  // getline( ifs, buf );
  ifs >> buf >> buf;
  W = stoi( buf );
  
  ifs >> buf;
  H = stoi( buf );
  
  ifs >> buf >> buf;
  nBlocks = stoi( buf );
  Blocks.resize( nBlocks );

  ifs >> buf >> buf;
  nTerminals = stoi( buf );
  Terminals.resize( nTerminals );

  HashMap<string, Terminal*> hash;
  hash.init();

  for ( unsigned i = 0; i < nBlocks; ++i ) {
    ifs >> buf;
    string n = buf;
    ifs >> buf;
    unsigned w = stoi( buf );
    ifs >> buf;
    unsigned h = stoi( buf );
    Blocks[ i ] = new Block ( w, h, n );
    hash.insert( n, Blocks[ i ] );
  } 

  for ( unsigned i = 0; i < nTerminals; ++i ) {
    ifs >> buf;
    string n = buf;
    ifs >> buf;
    assert( buf == "terminal" );

    ifs >> buf;
    unsigned x = stoi( buf );

    ifs >> buf;
    unsigned y = stoi( buf );

    Terminals[i] = new Terminal( x, y, n );
    hash.insert( n, Terminals[ i ] );
  }

  ifs.close();

  ifs.open( netFile, ifstream::in );


  if ( !ifs.is_open() ) {
     cout << "Error occur when opening file" << netFile << endl;
     exit(1);
  }

  ifs >> buf >> buf;
  nNets = stoi( buf );
  Nets.resize( nNets );
  for ( unsigned i = 0; i < nNets; ++i ) {
    ifs >> buf >> buf;
    unsigned nDegree = stoi( buf );
    Nets[ i ] = new Net(i);
    for ( unsigned j = 0; j < nDegree; ++j ) {
      ifs >> buf;
      Terminal* t = hash.find( buf );
      assert( t != NULL );
      Nets[ i ]->terminals.push_back( t );
      t->nets.push_back( Nets[i] );
    }
  }
  ifs.close();
  hash.reset();
}

