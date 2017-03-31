#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <string>
#include <vector>
#include <iostream>
#include "Net.h"
#include "Cell.h"

using namespace std;

class CirMgr
{
  public:
    CirMgr()
    {
      leftNodeNum = 0;
      rightNodeNum = 0;
      maxP = 0;
      for ( unsigned idx = 0; idx < 8; ++idx )
        cases[ idx ] = 0;
    }
    ~CirMgr() {}
    void setMaxP_BucketList_Threshold();
    void set2partEqully_nodeInNets();
    void calcGain( );
    void setMaxGainCell();
    void swapLoop();
    void swapCellAndUpdateGain( Cell* );
    void changeGain( Cell* , int );
    void popOutBucketlist( Cell* );
    void addCellToBucketList( Cell* cell );
    bool setToMaxGainState();
    void printBkt();
    void printCell();
    void printOriginGain();
    void outputResult( char* );
    void test();
    void printNet();
    void iterationLoop();
    void printCutsize();

    Net net;
    double r;
    vector<Cell*> cellList;
    vector<Cell*> bucketList;
    unsigned leftNodeNum;
    unsigned rightNodeNum;
    double HighThreshold;
    double LowThreshold;
    unsigned maxP;
    Cell* existMaxGainCell;
    vector<int> Gain;
    vector<bool> originIsleftPart;
    vector<unsigned> swapCellIDs;
    int cutSize;
    unsigned cases[8];
    unsigned MaxSwapNum;
    unsigned iterationCounter;
};

#endif
