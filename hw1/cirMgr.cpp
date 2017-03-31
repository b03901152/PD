#include <iostream>
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib> 
#include "cirMgr.h"
#include <assert.h> 
#include <fstream>
#include <math.h> 
using namespace std;


void
CirMgr::set2partEqully_nodeInNets() {
  // cout << "set2partEqully_nodeInNets" << endl;
  unsigned cellSize = cellList.size() - 1;
  // assert( cellList.size() );
  // cout << "cellSize:" << cellSize << endl;
  // cout << "cellSize/2:" << cellSize/2 << endl;

  rightNodeNum = cellSize/2;
  leftNodeNum = cellSize - cellSize/2;
  // cout << rightNodeNum << "" << leftNodeNum << endl;
  bool test = 0;
  bool suffle = 0;
  if ( test ) {
    cellList[1]->isLeftPart =
    cellList[2]->isLeftPart =
    cellList[3]->isLeftPart = true;
    cellList[4]->isLeftPart =
    cellList[5]->isLeftPart =
    cellList[6]->isLeftPart = false;
  }
  else if ( suffle ) {
    vector<bool> isLeftPart;
    for ( unsigned i = 0; i < (unsigned)(cellSize/2); i++ )
      isLeftPart.push_back(false);
    for ( unsigned i = (cellSize/2); i < cellSize; i++ )
      isLeftPart.push_back(true);
    random_shuffle( isLeftPart.begin(), isLeftPart.end() );
    originIsleftPart = isLeftPart;
    for ( unsigned i = 0; i < isLeftPart.size(); i++ )
      cellList[ i+1 ]->isLeftPart = isLeftPart[i];
  }
  else {
    vector<bool> isLeftPart;
    for ( unsigned i = 0; i < (unsigned)(cellSize/2); i++ )
      cellList[ i+1 ]->isLeftPart = true;
    for ( unsigned i = (cellSize/2); i < cellSize; i++ )
      cellList[ i+1 ]->isLeftPart = false;
  }
  for ( unsigned netID = 0; netID < net.cells.size(); netID++ ) {
    unsigned L = 0;
    unsigned R = 0;
    for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ ) {
      if ( net.cells[ netID ][ cellIdx ]->isLeftPart )
        L++;
      else
        R++;
    }
    net.leftNodeNum.push_back(L);
    net.rightNodeNum.push_back(R);
  }
}

void 
CirMgr::calcGain() {

  for ( unsigned cellID = 0; cellID < cellList.size(); cellID++ )
    cellList[ cellID ]->gain = 0;

  for ( unsigned netID = 0; netID < net.cells.size(); netID++ ) {
    // F gain
    if ( net.leftNodeNum[ netID ] == 1 )
      for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ )
        if ( net.cells[ netID ][ cellIdx ]->isLeftPart ) {
          ( net.cells[ netID ][ cellIdx ]->gain )++;
          break;
        }

    if ( net.rightNodeNum[ netID ] == 1 )
      for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ )
        if ( ! net.cells[ netID ][ cellIdx ]->isLeftPart ) {
          ( net.cells[ netID ][ cellIdx ]->gain )++;
          break;
        }

    // T gain
    if ( net.leftNodeNum[ netID ] == 0 )
      for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ )
        if ( ! net.cells[ netID ][ cellIdx ]->isLeftPart )
            net.cells[ netID ][ cellIdx ]->gain --;

    if ( net.rightNodeNum[ netID ] == 0 )
      for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ )
        if ( net.cells[ netID ][ cellIdx ]->isLeftPart )
            net.cells[ netID ][ cellIdx ]->gain --;
  }
}

void
CirMgr::setMaxGainCell() {
  for ( unsigned i = bucketList.size()-1; i >=0; i-- ) {
    if ( bucketList[i] != NULL ) {
      existMaxGainCell = bucketList[i];
      return;
    }
  }
}

void  
CirMgr::setMaxP_BucketList_Threshold() {
  if ( bucketList.empty() ) {
    for ( unsigned i = 1; i < cellList.size(); i++ ) 
      if ( maxP < cellList[i]->netIDs.size() )
        maxP = cellList[i]->netIDs.size();
    for ( unsigned i = 0; i < ( 2*maxP + 1 ); i++ )
      bucketList.push_back(NULL);
    HighThreshold = (1+r)/2*(cellList.size()-1);
    LowThreshold = (1-r)/2*(cellList.size()-1);
  }
  else {
    for ( unsigned i = 0; i < ( 2*maxP + 1 ); i++ )
      bucketList[i] = NULL;
    for ( unsigned i = 1; i < cellList.size(); i++ ) 
      cellList[i]->locked = false;
  }
  for ( unsigned i = 1; i < cellList.size(); i++ ) {
    cellList[i]->gain += maxP;
    unsigned bukIdx = cellList[i]->gain;
    if ( bucketList[ bukIdx ] == NULL ) {
      bucketList[ bukIdx ] = cellList[i];
      cellList[i]->pre = cellList[i]->post = cellList[i];
    }
    else
      bucketList[ bukIdx ]->addCell( cellList[i] );
  }
}

int counter =0;
void
CirMgr::swapLoop() {
  cout << "iteration counter: " << iterationCounter++ << endl;
  while( 1 ) {
    if ( swapCellIDs.size() > MaxSwapNum )
      break;
    bool moveToRight = (rightNodeNum+1) <= HighThreshold;
    bool moveToLeft = (rightNodeNum-1) >= LowThreshold;
    if ( !moveToRight && !moveToLeft ) 
      return;
    Cell* swapCell;
    Cell* firstCellOfBucketRow;
    if ( existMaxGainCell == NULL )
      return;
    swapCell = firstCellOfBucketRow = existMaxGainCell;
    if ( swapCell->isLeftPart ) {
      if ( !moveToRight )
        do {
            swapCell = swapCell->post;
            if ( swapCell == firstCellOfBucketRow ) {
              unsigned bktIdx = swapCell->gain;
              bktIdx--;
              while ( bucketList[bktIdx] == NULL ) {
                if ( bktIdx == 0 ) {
                  return;
                }
                bktIdx--;
              }
              swapCell = firstCellOfBucketRow = bucketList[ bktIdx ];
            }
        } while ( swapCell->isLeftPart );
    }
    else {
      if ( !moveToLeft ) {
        do {
          swapCell = swapCell->post;
          if ( swapCell == firstCellOfBucketRow ) {
            unsigned bktIdx = swapCell->gain;
            bktIdx--;
            while ( bucketList[bktIdx] == NULL ) {
              if ( bktIdx == 0 ) {
                return;
              }
              bktIdx--;
            }
            swapCell = firstCellOfBucketRow = bucketList[ bktIdx ];
          }
        } while ( !(swapCell->isLeftPart) );
      }
    }
    if ( swapCell == existMaxGainCell ) {
      if ( existMaxGainCell == existMaxGainCell->post ) {
        unsigned bktIdx = swapCell->gain;
        while ( bucketList[bktIdx] == NULL ) {
          bktIdx -= 1;
          if ( bktIdx == 0 ) {
            return;
          }
        }
        existMaxGainCell = bucketList[bktIdx];
      }
      else
        existMaxGainCell = existMaxGainCell->post;
    }
    if ( Gain.empty() )
      Gain.push_back( swapCell->gain - maxP );
    else
      Gain.push_back( Gain[ Gain.size()-1 ] + swapCell->gain - maxP );
    swapCellIDs.push_back( swapCell->ID );
    swapCellAndUpdateGain( swapCell );
  }
}

void
CirMgr::iterationLoop() {
  iterationCounter = 0;
  while( 1 ) {
    
    // cout << endl << "iterationCounter: " << iterationCounter++ << endl;
    MaxSwapNum = 0.1 * cellList.size() + 0.4 * cellList.size() / pow( 2, iterationCounter+1 );
    // printCutsize();
    calcGain();
    setMaxP_BucketList_Threshold();
    setMaxGainCell();
    swapLoop();
    if( !setToMaxGainState() ) break;
  }
}

void
CirMgr::swapCellAndUpdateGain( Cell* cell ) {
  popOutBucketlist( cell );
  cell->locked = true;
  for ( unsigned i = 0; i < cell->netIDs.size(); i++ ) {
    unsigned netID = cell->netIDs[i];
    int deltaGainOfAll = 0;
    unsigned L = net.leftNodeNum[netID];
    unsigned R = net.rightNodeNum[netID];
     if ( cell->isLeftPart ) {
      if ( L == 1 ) {
        --deltaGainOfAll;
      }
      if ( L == 2 ) {
        for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ ) {
          if ( net.cells[ netID ][ cellIdx ]->isLeftPart && 
               net.cells[ netID ][ cellIdx ] != cell ) {
            changeGain( net.cells[ netID ][ cellIdx ], +1 );
            break;
          }
        }
      }
      if ( R == 1 ) {
        for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ ) {
          if ( !net.cells[ netID ][ cellIdx ]->isLeftPart ) {
            changeGain( net.cells[ netID ][ cellIdx ], -1 );
            break;
          }
        }
      }
      if ( R == 0 ) {
        ++deltaGainOfAll;
      }
    } 
    else {
      if ( R == 1 ) {
        --deltaGainOfAll;
      }
      if ( R == 2 ) {
        for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ ) {
          if ( ( !net.cells[ netID ][ cellIdx ]->isLeftPart ) && 
               net.cells[ netID ][ cellIdx ] != cell ) {
            changeGain( net.cells[ netID ][ cellIdx ], +1 );
            break;
          }
        }
      }
      if ( L == 1 ) {
        for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ ) {
          if ( net.cells[ netID ][ cellIdx ]->isLeftPart ) {
            changeGain( net.cells[ netID ][ cellIdx ], -1 );
            break;
          }
        }
      }
      if ( L == 0 ) {
        ++deltaGainOfAll;
      }
    }
    if ( cell->isLeftPart ) {
      -- net.leftNodeNum[netID];  
      ++ net.rightNodeNum[netID];  
    }
    else {
      ++ net.leftNodeNum[netID];  
      -- net.rightNodeNum[netID];  
    }
    if ( deltaGainOfAll != 0 )
      for ( unsigned cellIdx = 0; cellIdx < net.cells[ netID ].size(); cellIdx++ )
        changeGain( net.cells[ netID ][ cellIdx ], deltaGainOfAll );
  }
  if ( cell->isLeftPart ) {
    leftNodeNum--;
    rightNodeNum++;
  }
  else {
    leftNodeNum++;
    rightNodeNum--;
  }
  cell->isLeftPart = !(cell->isLeftPart);
}

void
CirMgr::changeGain( Cell* cell, int g ) {
  if ( cell->locked )
    return;
  // assert(g!=0);
  popOutBucketlist( cell );
  if ( existMaxGainCell == NULL )
    return;
  cell->gain += g;
  addCellToBucketList( cell );
  if ( cell->gain > existMaxGainCell->gain ) {
    existMaxGainCell = cell;
  }
}

void
CirMgr::popOutBucketlist( Cell* cell ) {
  // printBkt();
  if ( existMaxGainCell == NULL )
    return;
  if ( cell == existMaxGainCell ) {
    if ( cell->post == cell ) {
      // assert( cell == bucketList[cell->gain] );
      unsigned bktIdx = cell->gain;
      do {
        bktIdx--;
        if ( bktIdx == 0 ) {
          existMaxGainCell = NULL;
          return;
        }
      } while ( bucketList[bktIdx] == NULL );
      existMaxGainCell = bucketList[bktIdx];
    }
    else
      existMaxGainCell = cell->post;
  }
  // cout << "popOutBucketlist: " << cell->ID << endl;
  // cout << "existMaxGainCell: " << existMaxGainCell->ID << endl;
  if ( cell == cell->pre ) {
    if (  cell != bucketList[cell->gain] ) {
      printBkt();
      cout << "err" << cell->ID << endl;
      cout << "ENDD" << endl;
    }
    // assert( cell == bucketList[cell->gain] );
    bucketList[cell->gain] = 0;
  }
  else {
    if ( bucketList[cell->gain] == cell )
      bucketList[cell->gain] = cell->post;
    cell->pre->post = cell->post;
    cell->post->pre = cell->pre;
    cell->pre = cell->post = cell;  
  }
  // printBkt();
  // cin.get();
}

void
CirMgr::addCellToBucketList( Cell* cell ) {
  // cout << "addCellToBucketList " << cell->ID << endl;
  if ( bucketList[ cell->gain ] == NULL )
    bucketList[ cell->gain ] = cell;
  else 
    bucketList[ cell->gain ]->addCell( cell );
}

void
CirMgr::printBkt() {
  for ( unsigned j = 0; j < bucketList.size(); j++ ) {
    cout << "bucketList idx : " << j << "  ";
    Cell* a = bucketList[j];
    if(a==NULL){
      cout << endl;
      continue;
    }
    Cell* b = a;
    do {
      cout << b->ID << "  ";
      b=b->post;
    } while(a!=b);
    cout << endl;
  }
  cout << endl;
}

void
CirMgr::printCell() {
  cout << "printCell" << endl;
  for ( unsigned j = 0; j < cellList.size(); j++ ) {
    cout << "cell " << j << " netIDs: " ;
    for ( unsigned i = 0; i < cellList[j]->netIDs.size(); i++ ) {
      cout << cellList[j]->netIDs[i] << ' ';
    }
    cout << endl << "isLeftPart: " << cellList[j]->isLeftPart;
    cout << endl << "gain: " << cellList[j]->gain;
    cout << endl;
  }
}

void
CirMgr::printOriginGain() {
  cout << "maxP: " << maxP << endl;
  for ( unsigned j = 1; j < cellList.size(); j++ ) {
    cout << "cell " << j << " gain: " << (cellList[j]->gain) - (int)maxP<< endl;
  }
}

bool
CirMgr::setToMaxGainState() {
  int maxGain = -1;
  unsigned maxGainIdx;
  for ( unsigned j = 0; j < Gain.size(); j++ ) {
    if ( Gain[j] > maxGain ) {
      maxGainIdx = j;
      maxGain = Gain[j];
    }
  }
  if ( maxGain <= 0 )
    maxGainIdx = 0;
  // cout << "Gain.size(): " << Gain.size() << endl;
  // cout << "maxGainIdx: " << maxGainIdx << endl;
  // cout << "maxGain: " << maxGain << endl;
  for ( unsigned j = maxGainIdx + 1; j < swapCellIDs.size(); j++ ) {
    Cell* cell = cellList[ swapCellIDs[j] ];
    for ( unsigned netIdx = 0; netIdx < cell->netIDs.size(); netIdx++ ) {
      unsigned netID = cell->netIDs[netIdx];
      if ( cell->isLeftPart ) {
        net.leftNodeNum[ netID ]--;
        net.rightNodeNum[ netID ]++;
      }
      else {
        net.leftNodeNum[ netID ]++;
        net.rightNodeNum[ netID ]--;
      }
    }
    if ( cell->isLeftPart ) {
      leftNodeNum--;
      rightNodeNum++;
    }
    else {
      leftNodeNum++;
      rightNodeNum--;
    }
    cell->isLeftPart = !(cell->isLeftPart); 
  }
  Gain = vector<int>();
  swapCellIDs = vector<unsigned>();
  return maxGain > 0 ;
}

void
CirMgr::printCutsize() {
  cutSize = 0;
  for ( unsigned j = 1; j < net.cells.size(); j++ ) {
    bool L = false;
    bool R = false;
    for ( unsigned i = 0; i < net.cells[j].size(); i++ ) {
      if ( net.cells[j][i]->isLeftPart ) {
        L = true;
      }
      else {
        R = true;
      }
      if ( L && R ) {
        cutSize++;
        break;
      }
    }
  }
  cout << "Cutsize = " << cutSize << endl;
}

void
CirMgr::outputResult( char* outputPath ) {
  ofstream result;
  result.open( outputPath, ofstream::out );
  vector<unsigned> left;
  vector<unsigned> right;

  cutSize = 0;
  // printCell();
  for ( unsigned j = 1; j < net.cells.size(); j++ ) {
    bool L = false;
    bool R = false;
    for ( unsigned i = 0; i < net.cells[j].size(); i++ ) {
      if ( net.cells[j][i]->isLeftPart ) {
        L = true;
      }
      else {
        R = true;
      }
      if ( L && R ) {
        cutSize++;
        break;
      }
    }
  }
  result << "Cutsize = " << cutSize << endl;
  cout << "Cutsize = " << cutSize << endl;
  for ( unsigned j = 1; j < cellList.size(); j++ ) {
    if ( cellList[j]->isLeftPart )
      left.push_back( cellList[j]->ID );
    else
      right.push_back( cellList[j]->ID );
  }
  result << "G1 " << left.size() << endl;
  // assert( leftNodeNum == left.size() );
  for ( unsigned j = 0; j < left.size(); j++ )
    result << "c" << left[j] << " ";
  result << ';';
  result << endl << "G2 " << right.size() << endl;
  // assert( rightNodeNum == right.size() );
  for ( unsigned j = 0; j < right.size(); j++ )
    result << "c" << right[j] << " ";
  result << ';';
  result.close();
}

void
CirMgr::test() {
  list<Cell*> a;
  a.push_back( new Cell(1) );
  a.push_back( new Cell(2) );
  a.push_back( new Cell(3) );
  a.push_back( new Cell(4) );
  for ( list<Cell*>::iterator it = a.begin(); it!=a.end(); it++ )
    cout << (*it) -> ID << endl;
}

void
CirMgr::printNet() {
  cout << "printNet" << endl;
  for ( unsigned j = 0; j < net.cells.size(); j++ ) {
    cout << "netID: " << j << endl;
    for ( unsigned i = 0; i < net.cells[j].size(); i++ )
      cout << net.cells[j][i]->ID << " ";
    cout << endl;
  }  
}