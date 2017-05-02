#ifndef FSA_H
#define FSA_H

#include "BStarTree.h"
#include "FloorPlanning.h"
#include <time.h> 

class FSA
{
  public:
    FSA( BStarTree* t )
    {
      tree = t;
      init();
      totalIter = 0;
    }
    ~FSA(){}
    void PseudoGreedyLocalSearch();
    void HillClimbingSearch();
    void HighTempratureRandomSearch();
    void SA();
    void perturb();
    void randomPerturb( bool );
    void init();
    void output( string& );
    void setStartTime( clock_t t ) { startTime = t; };
  private:  
    BStarTree*        tree;
    vector<Node*>     bestTreeNodesPointer;
    vector<Node>      bestTreeNodes;
    vector<unsigned>  bestBlockW;
    double            bestCost;
    double            temp; // temperature
    double            feasibleCounter;
    unsigned          iter; // iteration times
    unsigned          totalIter; // iteration times
    // unsigned          realCostNorm;
    double            c; // User-specified parameters, for update temperature
    double            k; // User-specified parameters, for update temperature
    double            prob; // uphill climb probability
    double            totalCost;
    double            T1;
    double            newCost;
    double            originCost;
    double            initCost;
    double            totalDeltaCost;
    double            costAfterPerturb;
    double            costBeforePerturb;
    clock_t           startTime;
    unsigned          bestIter;
};

#endif